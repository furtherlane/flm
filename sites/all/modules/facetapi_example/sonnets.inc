<?php
/**
 * @file
 * Define the migration procedure for the sonnets.
 */
class FacetapiExampleDataSonnet extends XMLMigration {
  public function __construct() {
    parent::__construct(MigrateGroup::getInstance("sonnets"));

    // The URL of the file to load
    $item_url = DRUPAL_ROOT . "/" . drupal_get_path("module", "facetapi_example_data") . "/" . "data/sonnets.xml";

    // The xpath for identifying each item
    $item_xpath = "sonnet";

    // The identifier of the item (relative to $item_xpath)
    $item_id_xpath = "title/@id";

    // The description of the source fields
    // It is an array "field_machine_name" => decription
    $fields = array("title" => t("title"),
            "body" => t("body"));

    // The class to reprent items
    $items_class = new MigrateItemsXML($item_url, $item_xpath, $item_id_xpath);

   // Define a source containing multiple items of type $items_class,
    // with input fields $fields
    $this->source = new MigrateSourceMultiItems($items_class, $fields );

    // Define the desination : a node of type page
    $this->destination = new MigrateDestinationNode('page');

    // Creates the mapping between input fields and output fields
    // Note that XMLMigration overloads addFieldMapping
    // to allow to set the xpath for the input field directly.
    $this->addFieldMapping('title', 'title')->xpath("title");
    $this->addFieldMapping('body', 'body')->xpath("body");
    // Here we set a default value for this field.
    $this->addFieldMapping('promote')->defaultValue(TRUE);

    // Create the map, which keeps track of the migration status for each
    // item.
    $this->map = new MigrateSQLMap($this->machineName,
      array("id" => array("type" => "varchar",
        "length" => 4,
        "not null" => TRUE),
      ),
      MigrateDestinationNode::getKeySchema());
  }
}