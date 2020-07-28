<?php

/**
 * @file
 * Failed Login Messages admin pages callbacks.
 */

/**
 * Failed Login Messages admin settings form.
 */
function flm_user_admin_settings() {
  $form = array();

  $form['user'] = array(
    '#type' => 'fieldset',
    '#title' => t('User'),
    '#collapsible' => TRUE,
  );

  $form['user']['user_failed_login_user_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Failed login user limit'),
    '#required' => TRUE,
    '#element_validate' => array('element_validate_integer_positive'),
    '#default_value' => variable_get('user_failed_login_user_limit', 5),
  );

  $form['user']['user_failed_login_user_window'] = array(
    '#type' => 'textfield',
    '#title' => t('Failed login user window'),
    '#required' => TRUE,
    '#element_validate' => array('element_validate_integer_positive'),
    '#default_value' => variable_get('user_failed_login_user_window', 21600),
  );

  $form['user']['flm_user_failed_login_message_default'] = array(
    '#type' => 'textarea',
    '#title' => t('Default message'),
    '#default_value' => variable_get('flm_user_failed_login_message_default'),
  );

  $form['user']['flm_user_failed_login_message_penultimate'] = array(
    '#type' => 'textarea',
    '#title' => t('Penultimate message'),
    '#default_value' => variable_get('flm_user_failed_login_message_penultimate'),
  );

  $form['user']['flm_user_failed_login_message_last'] = array(
    '#type' => 'textarea',
    '#title' => t('Last message'),
    '#default_value' => variable_get('flm_user_failed_login_message_last'),
  );

  $form['ip'] = array(
    '#type' => 'fieldset',
    '#title' => t('IP'),
    '#collapsible' => TRUE,
  );

  $form['ip']['user_failed_login_ip_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Failed login IP limit'),
    '#required' => TRUE,
    '#element_validate' => array('element_validate_integer_positive'),
    '#default_value' => variable_get('user_failed_login_ip_limit', 50),
  );

  $form['ip']['user_failed_login_ip_window'] = array(
    '#type' => 'textfield',
    '#title' => t('Failed login IP window'),
    '#required' => TRUE,
    '#element_validate' => array('element_validate_integer_positive'),
    '#default_value' => variable_get('user_failed_login_ip_window', 3600),
  );

  $form['ip']['flm_user_failed_login_ip_message_default'] = array(
    '#type' => 'textarea',
    '#title' => t('Default message'),
    '#default_value' => variable_get('flm_user_failed_login_ip_message_default'),
  );

  if (module_exists('token')) {
    $form['token_help'] = array(
      '#title' => t('Dynamic tokens'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form['token_help']['token'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('flm'),
      '#global_types' => FALSE,
    );
  }

  return system_settings_form($form);
}
