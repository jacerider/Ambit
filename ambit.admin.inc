<?php

/**
 * Settings form for the toolkit.
 */
function ambit_settings_form($form, &$form_state) {

  $form['compass'] = array(
    '#type' => 'fieldset',
    '#title' => t('Compass'),
    '#collapsible' => FALSE,
    '#description' => t('Compass is a standalone Ruby script used to compile CSS from Sass stylesheets. To use it, it must be installed on your server and you need to know where it is located. If you are unsure of the exact path consult your ISP or server administrator.'),
  );
  
	module_load_include('inc', 'ambit', 'ambit.compass');
  ambit_compass_exec('version', $output, $errors);
  $output = empty($output) ? t('Compass not found. Please update executable path.') : $output;
  $form['compass']['version'] = array(
    '#type' => 'item',
    '#title' => t('Version information'),
    '#markup' => '<pre>' . check_plain(trim($output)) . '</pre>',
  );

  $form['compass']['ambit_compass_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path of "compass" executable'),
    '#default_value' => variable_get('ambit_compass_path', '/usr/bin/compass'),
    '#required' => TRUE,
    '#description' => t('Specify the complete path to the Compass binary. For example: <kbd>/usr/bin/compass</kbd>'),
    '#element_validate' => array('ambit_compass_validate_path'),
  );


  return system_settings_form($form);
}

function ambit_compass_build_version($form_element, $form_state) {
	module_load_include('inc', 'ambit', 'ambit.compass');
  // make sure path is set and valid before running after build.
  if ($path_errors = ambit_compass_check_path($form_state['values']['ambit_compass_path'])) {
    // check here is primarily for pre-existing bad settings...
    // the #validate should prevent users from adding bad paths.
    $form_element['version'] = array(
      '#type' => 'markup',
      '#markup' => '<p class="error">' . implode('<br />', $path_errors) . '</p>',
    );
  }else {
  	dsm($form_element);
    ambit_compass_exec('version', $output, $errors);
    $form_element['version'] = array(
      '#type' => 'item',
      '#title' => t('Version information'),
      '#markup' => '<pre>' . check_plain(trim($output)) . '</pre>',
    );
  }
  return $form_element;
}

/**
 * Validate the path to compass
 */
function ambit_compass_validate_path($element, &$form_state) {
	module_load_include('inc', 'ambit', 'ambit.compass');
  $errors = ambit_compass_check_path($element['#value']);
  if ($errors) {
    form_set_error('compass][ambit_compass_path', implode('<br />', $errors));
  }
}