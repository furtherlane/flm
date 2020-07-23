<?php

/**
 * @file
 * Classes used by the Facet API module.
 */

/**
 * Facet API adapter example.
 */
class FacetapiExampleAdapter extends FacetapiAdapter {
  /**
   * Returns a boolean flagging whether $this->searcher executed a search.
   */
  public function searchExecuted() {
    return !empty($this->keys);
  }

  /**
   * Returns a boolean flagging whether facets in a realm shoud be displayed.
   */
  public function suppressOutput($realm_name) {
    return FALSE;
  }
}
