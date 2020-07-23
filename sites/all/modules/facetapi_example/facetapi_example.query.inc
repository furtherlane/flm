<?php
/**
 * @file
 * Implementation of the actual query to the search engine.
 */

module_load_include("inc", "facetapi_example", "facetapi_example.engine");

/**
 * Implements the query object required by facetapi
 *
 * Responsibilities:
 * - handle the native search
 * - handler the addition of facets to the search
 */
class FacetapiExampleQuery {

  /**
   * The facet data, as returned by the search engine
   */
  var $suggestion_facet = NULL;

  /**
   * The facet data, as returned by the search engine
   */
  var $length_facet = NULL;

  /**
   * The search key
   */
  var $keys;

  /**
   * The "suggestion" filter value
   */
  var $suggestion;

  /**
   * The "length" filter value
   */
  var $length;

  /**
   * Search conditions are ignored in this toy search engine
   */
  function __construct($keys) {
    $this->keys = $keys;
    $this->suggestion = '';
    $this->length = '';
  }

  /**
   * Add a filter value for a facet
   */
  function addFacet($type, $value) {
    if ($type == "suggestion") {
      $this->suggestion = $value;
    }
    elseif ($type == "length") {
      $this->length = $value;
    }
  }

  /**
   * Actually run the query
   */
  function execute() {
    list($results, $suggest_words, $suggest_length) =
      facetapi_example_search($this->keys,
                  $this->suggestion,
                  $this->length);


    $results = node_load_multiple($results);
    $rendered = array();

    $this->suggestion_facet = $suggest_words;
    $this->length_facet = $suggest_length;

    // Build the result list
    foreach ($results as $node) {
      $build = node_view($node, 'search_index');
      unset($build['#theme']);
      $node->rendered = drupal_render($build);
      $node_extra = module_invoke_all('node_search_result', $node);

      $uri = entity_uri('node', $node);

      $rendered[] = array(
             'link' => url($uri['path'], array_merge($uri['options'], array('absolute' => TRUE))),
             'title' => $node->title,
             'type' => check_plain(node_type_get_name($node)),
             'user' => theme('username', array('account' => $node)),
             'date' => $node->changed,
             'extra' => $node_extra,
             'node' => $node
              );
    }
    return $rendered;
  }
}