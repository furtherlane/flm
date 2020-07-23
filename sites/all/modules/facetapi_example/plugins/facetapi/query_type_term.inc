<?php

/**
 * @file
 * Term query type plugin for the Example Faceted Navigation  adapter.
 */

/**
 * Plugin for "term" query types.
 */
class FacetapiExampleTerm extends FacetapiQueryType implements FacetapiQueryTypeInterface {

  /**
   * Returns the query type associated with the plugin.
   *
   * @return string
   *   The query type.
   */
  static public function getType() {
    return 'facetapi_example_term';
  }

  /**
   * Adds the filter to the query object.
   *
   * @param $query
   *   An object containing the query in the backend's native API.
   */
  public function execute($query) {
    $name = $this->facet['name'];
    $active_items = $this->adapter->getAllActiveItems();

    foreach ($active_items as $key => $item) {
      $type = $item["field alias"];
      $value = $item["value"];
      if ($type == $name) {// This is our facet
        $query->addFacet($type, $value);
      }
    }
  }

  /**
   * Initializes the facet's build array.
   *
   * @return array
   *   The initialized render array.
   */
  public function build() {
    $build = array();
    // Makes sure there was at least one match.
    if (!$this->adapter->hasMatches) {
      return array();
    }

    // Get back to the query, facets are stored there
    $query = $this->adapter->query;
    $name = $this->facet["name"];

    switch ($name) {
    case "length":
      $res = $query->length_facet;
      break;
    case "suggestion":
      $res = $query->suggestion_facet;
      break;
    default:
      // Nothing to do
      $res = NULL;
    }

    if ($res) {
      foreach ($res as $term => $count) {
    $build[$term] = array("#count" => $count);
      }
    }

    // If a facet is active, it is not present in the results. Add it.
    if ($activeItems = $this->adapter->getActiveItems($this->facet)) {
      foreach ($activeItems as $key => $item) {
    $term = $item["value"];
    $count = 1;
    $build[$term] = array("#count" => $count);
      }
    }
    return $build;
  }
}
