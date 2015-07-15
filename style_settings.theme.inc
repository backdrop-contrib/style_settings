<?php

/**
 * Returns HTML for a slider form element.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #title, #value, #description, #min, #max, #attributes,
 *     #step.
 *
 * @ingroup themeable
 */
function theme_style_settings_slider($variables) {
  $element = $variables['element'];
  // A unique identifier for the case of multiple sliders on one page.
  $id = $element['#parents'][0];
  $element['#attributes']['type'] = 'range';
  $element['#attributes']['oninput'] = 'showValue' . $id . '(this.value)';
  element_set_attributes($element, array('id', 'name', 'value', 'step', 'min', 'max'));
  _form_set_class($element, array('form-text', 'form-range'));
  $output = '<input' . drupal_attributes($element['#attributes']) . ' /> 
    <span id="' . $id . '">' . $element['#attributes']['value'] . '</span>
    <script type="text/javascript">
    function showValue' . $id . '(newValue) {
      document.getElementById("' . $id . '").innerHTML=newValue;
    }
    </script>';

  return $output;
}