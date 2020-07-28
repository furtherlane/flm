<?php

/**
 * @file
 * Failed Login Messages tokens.
 */

/**
 * Implements hook_token_info().
 */
function flm_token_info() {
  $info = array();

  $info['types']['flm'] = array(
    'name' => t('FLM'),
    'description' => t('Failed Login Messages'),
  );

  $info['tokens']['flm']['user-limit'] = array(
    'name' => t('Failed login user limit'),
  );

  $info['tokens']['flm']['user-penultimate-number'] = array(
    'name' => t('User login penultimate attempt number'),
  );

  $info['tokens']['flm']['user-window'] = array(
    'name' => t('Failed login user window'),
  );

  $info['tokens']['flm']['ip-limit'] = array(
    'name' => t('Failed login IP limit'),
  );

  $info['tokens']['flm']['ip-window'] = array(
    'name' => t('Failed login IP window'),
  );

  return $info;
}

/**
 * Implements hook_tokens().
 */
function flm_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  if ($type != 'flm') {
    return;
  }

  $t_options = array();

  if (!empty($data['language']) && $data['language']->language) {
    $t_options['langcode'] = $data['language']->language;
  }

  drupal_alter('flm_tokens_t_options', $t_options);

  foreach ($tokens as $name => $original) {
    switch ($name) {
      case 'user-limit':
        $replacements[$tokens[$name]] = variable_get('user_failed_login_user_limit', 5);

        break;

      case 'user-penultimate-number':
        $replacements[$tokens[$name]] = variable_get('user_failed_login_user_limit', 5) - 1;

        break;

      case 'user-window':
        $interval = flm_format_interval(variable_get('user_failed_login_user_window', 21600), 1, $t_options);

        $replacements[$tokens[$name]] = $interval;

        break;

      case 'ip-limit':
        $replacements[$tokens[$name]] = variable_get('user_failed_login_ip_limit', 50);

        break;

      case 'ip-window':
        $interval = flm_format_interval(variable_get('user_failed_login_ip_window', 3600), 1, $t_options);

        $replacements[$tokens[$name]] = $interval;

        break;

    }
  }

  return $replacements;
}

/**
 * Formats a time interval with the requested granularity.
 *
 * @param int $interval
 *   The length of the interval in seconds.
 * @param int $granularity
 *   How many different units to display in the string.
 * @param array $options
 *   An associative array of additional options. See t() for allowed keys.
 *
 * @return string
 *   A string representation of the interval.
 */
function flm_format_interval($interval, $granularity = 2, $options = array()) {
  $units = array(
    '1 year|@count years' => 31536000,
    '1 month|@count months' => 2592000,
    '1 week|@count weeks' => 604800,
    '1 day|@count days' => 86400,
    '1 hour|@count hours' => 3600,
    '1 minute|@count minutes' => 60,
    '1 second|@count seconds' => 1
  );
  $output = '';
  foreach ($units as $key => $value) {
    $key = explode('|', $key);
    if ($interval >= $value) {
      $output .= ($output ? ' ' : '') . format_plural(floor($interval / $value), $key[0], $key[1], array(), $options);
      $interval %= $value;
      $granularity--;
    }

    if ($granularity == 0) {
      break;
    }
  }

  return $output;
}
