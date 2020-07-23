<?php
/**
 * @file
 * Implementation of the simple search engine
 */

/**
 * Basic indexing, with SQL query
 */
function facetapi_example_index() {
  $query = db_select("node", "n")
    ->fields("n", array("nid", "title"));
  $b = $query->join("field_data_body", "b", "nid = entity_id");
  $query->fields($b, array("body_value"));

  $result = $query->execute();
  $count = $result->rowCount();
  $word_index = array();
  $pairs_index = array();
  $length_index = array();

  $current = 0;
  foreach ($result as $i) {
    $current ++;
    $start = microtime(TRUE);
    $msg = "Processing $current / $count ";

    $words = facetapi_example_prepare($i->title . " " . $i->body_value);
    $msg .=  "with " . count($words) . " words";
    _facetapi_example_append($length_index, (string) count($words), $i->nid);

    $word_list = _facetapi_example_get_wordlist($words);
    foreach ($word_list as $word) {
      _facetapi_example_append($word_index, $word, $i->nid);
    }

    _facetapi_compute_distance($word_list, $words, $pairs_index);

    $time = microtime(TRUE)- $start;
    $perword = $time / $count;
    $msg .=  " - time : $time s - $perword s / word";
    _facetapi_example_message($msg, "status", FALSE);
  }
  //Write the indexes to file.
  _facetapi_example_serialize("words", $word_index);
  _facetapi_example_serialize("length", $length_index);
  _facetapi_example_serialize("pairs", $pairs_index);
  return "";
}

/**
 * Performs a search
 *
 * @param string $key : the word to search
 * @param string $proximity : the "proximity facet" filter
 * @param string $length : the "length facet" filter
 */
function facetapi_example_search($key, $proximity="", $length="") {
  $index = _facetapi_example_load("words");
  $prox_index =  _facetapi_example_load("pairs");
  $length_index = _facetapi_example_load("length");

  //First, get the node IDs for the results
  if (array_key_exists($key, $index)) {
    $result_set = $index[$key];
  }
  else {
    $result_set = array();
  }

  // Add the proximity filter
  if ($proximity != '') {
    // Facet is defined : nothing more to do
    $result_set = array_intersect($result_set, $index[$proximity]);
  }
  // Add the length filter
  if ($length != "" ) { //Filter result set
    $result_set = array_intersect($result_set, $length_index[$length]);

  }

  // Compute the proximity facet if no filter given
  $proxi_facet = array();
  if ($proximity == "") {
    // First compute the possible words pairs containing this key
    // These are the suggestions
    $words = array_keys($prox_index);
    foreach ($words as $word1) {
      // $key can be the first or second element of the pair
      if ($word1 == $key) {
        $proxi_facet = array_merge($proxi_facet, $prox_index[$word1]);
      }
      else {
        if (array_key_exists($key, $prox_index[$word1])) {
          $proxi_facet[$word1] = $prox_index[$word1][$key];
        }
      }
    }
  }

  // We now have an array : suggestion => proximity
  // Order to keep the most "relevant" ones
  arsort($proxi_facet, SORT_NUMERIC);
  $proxies = array();
  $counter = 0;
  // And for each suggestion, get the count of results matching
  // both $key and $suggestion
  foreach ($proxi_facet as $key => $val) {
    $count = count(array_intersect($result_set, $index[$key]));
    if ($count) {
      $proxies[$key] = $count;
    }
    if (++ $counter >= 20) {
      break;
    }
  }

  // Compute the length facet if length filter is inactive
  $length_facet = array();
  if ($length == "") {
    foreach ($length_index as $length => $set) {
      $count = count(array_intersect($result_set, $set));
      if ($count) {
        $length_facet[$length] = $count;
      }
    }
  }
  return array($result_set, $proxies, $length_facet);
}

/**
 * Utility function to display a message on the console or in message area
 */
function _facetapi_example_message($message, $type, $display_in_drupal = TRUE) {
  if (function_exists("drush_log")) {
    // Write to the console through Drush
    drush_log($message, $type);
  }
  elseif ($display_in_drupal) {
    switch ($type) { // Reduce the drush types to a few standard ones
    case "error":
    case "warning":
      $drupal_type = $type;
    break;
    default:
      $drupal_type = "status";
    }
    drupal_set_message(check_plain($message), $drupal_type);
  }
  else {
    // Not in Drush and display disabled for Drupal: do nothing
  }
}

/**
 * Serialize an index into the directory specified by facetapi_example_index_dir
 */
function _facetapi_example_serialize($name, &$value) {
  $dir = variable_get("facetapi_example_index_dir", "/tmp");
  $data = serialize($value);
  $f = fopen($dir . "/" . $name . ".idx", "w");
  fwrite($f, $data);
  fclose($f);
}

/**
 * Loads an index from the directory specified by facetapi_example_index_dir
 */
function _facetapi_example_load($name) {
  $dir = variable_get("facetapi_example_index_dir", "/tmp");
  $file = $dir . "/" . $name . ".idx";
  $data = unserialize(file_get_contents($file));
  return $data;
}

/**
 * Gets the sorted list of unique words
 */
function _facetapi_example_get_wordlist(array &$words) {
  $result = array();
  foreach ($words as $word) {
    $result[$word] = 1;
  }

  $tmp = array_keys($result);
  sort($tmp);
  return $tmp;
}

/**
 * Computes the minimum distance between all words in the article
 */
function _facetapi_compute_distance(array &$words, array &$text, &$pairs_index) {
  // 2nd word of the pair must be lower than the first
  foreach ($words as $word1) {
    foreach ($words as $word2) {
      if ($word2 >= $word1) {
        break;
      }
      // Compute min dist between the 2 words
      $flag = 0;
      $count = 0;
      $min = count($text);
      foreach ($text as $word) {
    //drupal_set_message(" - - $word");
    if ($word == $word1 or $word == $word2) {
      if (!$flag) {
        $word == $word1 ? $flag = 1 : $flag = 2;
        $count = 0;
      }
      else {
        if ( ($word == $word1 and $flag == 1) or
          ($word == $word2 and $flag == 2)) {
          // Twice the same word in a row
          $count = 0;
        }
        else {
          if ($count < $min) {
            $min = $count;
          }
          // Toggle the flag
          $flag = ($flag == 1 ? 2: 1);
          $count = 0;
        }
      } // closes if (!$flag)
    }
    $count ++;
      } // closes foreach ($text as $word)
      // Index by word1, then word2
      if (! array_key_exists($word1, $pairs_index)) {
        $pairs_index[$word1] = array();
      }
      if (! array_key_exists($word2, $pairs_index[$word1])) {
        $pairs_index[$word1][$word2] = 0;
      }
      $pairs_index[$word1][$word2] += 1/$min;
    } // closes foreach word2
  }
}

/**
 * Add a new value to the index, creating the array for key if needed
 */
function _facetapi_example_append(array &$content, $key, $value) {
  if (! array_key_exists($key, $content)) {
    $content[$key] = array();
  }
  $content[$key][] = $value;
}


/**
 * Checks if a word is interessting - callback for facetapi_example_prepare
 */
function _facetapi_example_filter($v) {
  return strlen($v) > 3;
}

/**
 * Prepares the text for indexing
 *
 * Strips everything but ASCII letters
 */
function facetapi_example_prepare($text) {
  $text = strtolower($text);
  $text = preg_replace("/[^\w]/", " ", $text);
  $res = explode(" ", $text);
  $res = array_filter($res, "_facetapi_example_filter");
  return $res;
}