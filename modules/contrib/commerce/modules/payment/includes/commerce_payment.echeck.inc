<?php

/**
 * @file
 * eCheck helper functions for Drupal commerce.
 *
 * Suggested values taken from:
 * http://www.authorize.net/files/echecknetuserguide.pdf
 */

// Used to capture eCheck funds immediately.
define('COMMERCE_ECHECK_WEB', 'echeck_web');

/**
 * Returns a set of eCheck form elements that payment method modules can
 *   incorporate into their submission form callbacks.
 *
 * @param $fields
 *   An array specifying the eCheck fields that should be included on the form; the
 *     routing (aba) number and account number fields are always present.
 * @param $default
 *   An array of default values for the available eCheck fields.
 */
function commerce_payment_echeck_form($fields = array(), $default = array()) {
  // Merge default values into the default array.
  $default += array(
    'aba_code' => '',
    'acct_num' => '',
    'type' => '', // CHECKING, BUSINESSCHECKING, SAVINGS
    'bank_name' => '',
    'acct_name' => '',
    'check_num' => '',
    'code' => '',
  );

  $form['echeck'] = array(
    '#tree' => TRUE,
  );

  // Always add a field for the eCheck number.
  $form['echeck']['aba_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Routing number'),
    '#default_value' => $default['aba_code'],
    '#attributes' => array('autocomplete' => 'off'),
    '#required' => TRUE,
    '#maxlength' => 9,
    '#size' => 10,
  );

  // Always add a field for the eCheck number.
  $form['echeck']['acct_num'] = array(
    '#type' => 'textfield',
    '#title' => t('Account number'),
    '#default_value' => $default['acct_num'],
    '#attributes' => array('autocomplete' => 'off'),
    '#required' => TRUE,
    '#maxlength' => 20,
    '#size' => 21,
  );

  // Add an account type selector if specified.
  if (isset($fields['type'])) {
    $form['echeck']['type'] = array(
      '#type' => 'select',
      '#title' => t('Account type'),
      '#options' => array_intersect_key(commerce_payment_echeck_types(), drupal_map_assoc((array) $fields['type'])),
      '#default_value' => $default['type'],
    );
    $form['echeck']['valid_types'] = array(
      '#type' => 'value',
      '#value' => $fields['type'],
    );
  }

  else {
    $form['echeck']['valid_types'] = array(
      '#type' => 'value',
      '#value' => array(),
    );
  }

  // Add a field for the bank name if specified.
  if (isset($fields['bank_name'])) {
    $form['echeck']['bank_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Bank name'),
      '#default_value' => $default['bank_name'],
      '#attributes' => array('autocomplete' => 'off'),
      '#required' => TRUE,
      '#maxlength' => 64,
      '#size' => 32,
    );
  }

  // Add a field for the account owner if specified.
  if (isset($fields['acct_name'])) {
    $form['echeck']['acct_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Account owner'),
      '#default_value' => $default['acct_name'],
      '#attributes' => array('autocomplete' => 'off'),
      '#required' => TRUE,
      '#maxlength' => 64,
      '#size' => 32,
    );
  }

  // Add a field for the check number if specified.
  if (isset($fields['check_num'])) {
    $form['echeck']['check_num'] = array(
      '#type' => 'textfield',
      '#title' => t('Check number'),
      '#default_value' => $default['code'],
      '#attributes' => array('autocomplete' => 'off'),
      '#required' => TRUE,
      '#maxlength' => 9,
      '#size' => 9,
    );
  }

  return $form;
}

/**
 * Validates a set of eCheck details entered via the eCheck form.
 *
 * @param $details
 *   An array of eCheck details as retrieved from the eCheck array in
 *   the form values of a form containing the eCheck form.
 * @param $settings
 *   Settings used for calling validation functions and setting form errors:
 *   - form_parents: an array of parent elements identifying where the echeck
 *     form was situated in the form array
 *
 * @return
 *   TRUE or FALSE indicating the validity of all the data.
 *
 * @see commerce_payment_echeck_form()
 */
function commerce_payment_echeck_validate($details, $settings) {
  $prefix = implode('][', $settings['form_parents']) . '][';
  $valid = TRUE;

  // Validate the aba (routing) number.
  if (!commerce_payment_validate_echeck_aba($details['aba_code'])) {
    form_set_error($prefix . 'aba_code', t('You have entered an invalid routing number.'));
    $valid = FALSE;
  }

  if (!commerce_payment_validate_echeck_account($details['acct_num'])) {
    form_set_error($prefix . 'acct_num', t('You have entered an invalid account number.'));
    $valid = FALSE;
  }

  return $valid;
}


/**
 * Validates a eCheck aba number.
 *
 * @param $number
 *   The eCheck number to validate.
 *
 * @return
 *   TRUE or FALSE indicating the number's validity.
 *
 * @see http://www.zend.com//code/codex.php?ozid=968&single=1
 */

function commerce_payment_validate_echeck_aba($number) {
  // First, check for 9 digits and non-numeric characters.
  if (preg_match("/^[0-9]{9}$/", $number)) {
    $n = 0;
    for ($i = 0; $i < 9; $i += 3) {
      $n += (substr($number,$i,1) * 3)
      + (substr($number,$i + 1,1) * 7)
      + (substr($number,$i + 2,1));
    }

    // If the resulting sum is an even multiple of ten (but not zero),
    // the aba routing number is good.

    if ($n != 0 && $n % 10 == 0) {
      return TRUE; // found good aba
    }
    else {
      return FALSE;
    }
  }
  else {
    return FALSE;
  }
}

/**
 * Validates a eCheck account number.
 *
 * @param $number
 *   The eCheck number to validate.
 *
 * @return
 *   TRUE or FALSE indicating the number's validity.
 *
 */
function commerce_payment_validate_echeck_account($number) {
  // First, check for up to 20 digits and numeric characters.
  if (preg_match("/^[0-9]{1,20}$/", $number)) {
    return TRUE; // found good routing number
  }
  else {
    return FALSE;
  }
}


/**
 * Returns an associative array of eCheck types.
 */
function commerce_payment_echeck_types() {
  return array(
    'checking' => t('Checking'),
    'business_checking' => t('Business Checking'),
    'savings' => t('Savings'),
  );
}
