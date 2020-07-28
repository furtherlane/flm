<?php

/**
 * @file
 * Hooks provided by the Failed Login Messages module.
 */

/**
 * Alters options for the t() function.
 *
 * @param array $options
 *   An associative array of additional options for the t() function.
 */
function hook_flm_tokens_t_options_alter(&$options) {
  $options['language'] = 'en';
}
