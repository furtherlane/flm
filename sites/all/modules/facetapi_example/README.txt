
The facetapi_example module is voluntartily simple implementation of the 
facetapi. Its goal is to provide a starting point for implementers.

It uses a custom search engine, which is not written whith performance in
mind. It can only index a hundred of nodes in english, with the nodes body
containing less than a hundred words.

Usage
-----
To see it work, install the module, enable the facets in the search engine
parameters, and index the site content.

You should use the provided drush command, as the indexing can take a long
time to complete.

Acknowledgements 
----------------
This module and tutorial has been developped by Jouve (http://jouve.com)
