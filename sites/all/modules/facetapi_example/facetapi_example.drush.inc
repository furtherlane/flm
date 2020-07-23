<?php
/**
 * @file
 * Drush command to index the site.
 */

/**
 * Implements hook_drush_command().
 */
function facetapi_example_drush_command() {
  $items = array();
  $items["facetapi-example-index"] =
    array("description" => "Creates the index",
      // We need to have the full Drupal running
      "boostrap" => DRUSH_BOOTSTRAP_FULL,
      // Callback is automatic
      );
  return $items;
}

/**
 * Performs the work for the command "facetapi-example-index"
 *
 * Note that Drush automatically defines the callback if no callback is
 * defined. It does this by taking the name of the command and turning "-"
 * into "_" and prefixing with "drush_".
 * Here, the command is "facetapi-example-index", so the callback is
 * drush_facetapi_example_index.
 */

function drush_facetapi_example_index() {
  module_load_include("inc", "facetapi_example", "facetapi_example.engine");
  facetapi_example_index();
  drush_log("Indexing finished", "success");
}
