## Style (CSS) Settings module ##

https://drupal.org/project/style_settings is intended for theme and module
maintainers but can be used for customisations by anyone with basic coding
skills (for example to provide a patch for projects that could use some CSS to
be configurable). It might simply be needed because a theme or module declares
it as a (soft-)dependency.

Allows any module or theme to have their CSS attributes configurable from the UI
just by wrapping default CSS values (plus unit) in a code comment. These are
then substituted in a separate rewritten CSS file by Drupal variables or theme
settings. The CSS is functional even if this module is not installed.

All is cached so the performance impact is neglectable. The rewritten CSS files
are included in the CSS aggregation mechanism.


### How to add configurable CSS to your module or theme? ###

- Add to your '.info' file:
    soft_dependencies[] = style_settings
  OR if you want to make the Style (CSS) Settings module required:
    dependencies[] = style_settings
- Wrap CSS values you want to be configurable in comments, including its unit
  (px, em, %) if present.
  An example for a module:
    font-size: 80%;
  becomes:
    font-size: /*variable:YOURMODULE_caption_fontsize*/80%/*variable*/;
  An example for a theme:
    font-size: 16px;
  becomes:
    font-size: /*setting:YOURTHEME_base_fontsize*/16px/*setting*/;
  The wrapped values are used as the default. Because comments cannot reside
  within url(...), you need to add the comments around it.
- Provide form elements on your settings page to configure module variables or
  theme settings (see below).

#### Note 1 ####
No code comment are allowed between a CSS value and its unit.

Something like:
  border-width: /*variable:YOURMODULE_thickness*/2/*variable*/px;
results in invalid CSS and is therefore ignored by browsers. Use instead:
  border-width: /*variable:YOURMODULE_thickness*/2px/*variable*/;

The Style Settings form element 'Number' (see below) takes care of putting a
measurement unit behind the value when storing it in a variable.

#### Note 2 ####
For readability it is recommended not to set the values of several CSS
properties simultaneously if it contains inline code comments.
Instead of:
  border: solid /*variable:YOURMODULE_borderwidth*/2px/*variable*/ /*variable:YOURMODULE_bordercolor*/Red/*variable*/;
use:
  border-style: solid;
  border-width: /*variable:YOURMODULE_borderwidth*/2px/*variable*/;
  border-color: /*variable:YOURMODULE_bordercolor*/Red/*variable*/;


### How to provide form elements on your settings page? ###

#### Add a conditional ####

Settings can be added to your settings page the usual way. If you declare the
Style (CSS) Settings module only as a soft-dependency (thus not required), it
makes sense to show settings only if the Style Settings module is enabled by
wrapping all Style Settings specific form elements inside a conditional:

  if (module_exists('style_settings')) { ... }

#### Add a form fields ####

Developers can use any form API element on their settings page to capture a
theme setting or variable. The Style Settings module offers also a few custom
form API element to make building CSS settings easier. Cool UI widgets and
built-in validation of user input. For example a color picker or a slider.

For project maintainers adding a Style Setting field to their configuration is
as easy adding a few lines of code. Examples how to provide different types of
Style Settings can be found in the 'code snippets' section below.

##### Number #####

'#type' => 'style_settings_number',
- Takes care of the validation just by providing a min/max and step value.
- Adds a unit if valid. Input: '2', field_suffix: 'px' => stored variable: '2px'
- Aligns field input to the right to stay close to the unit (field_suffix).

##### ColorPicker #####

A lot of CSS attributes contain a color value. Although a simple text field
could hold a color hex value, having a colorpicker is more convenient.

'#type' =>  'style_settings_colorpicker',
- Uses Drupal core's Farbtastic ColorPicker plugin.
- Makes the current choosen color visible in the settings field itself.
- Only accepts a valid color, a hex value but also a color name.
- Does not depend on any contrib or the Color module.
- Does not need an additional jQuery library.
- Stores the variable as a hex value or color name that is valid in CSS files.

##### Slider (HTML5 range input) #####

'#type' => 'style_settings_slider',
- Exposes a slider widget (a range) in HTML5 capable browsers (all but <= IE9).
- Shows the numeric value that corresponds with the handle position.
- Validation just by providing a min/max and step value.
- Preset to be used for opacity (min:0, max:1, step:0.01) but can be overriden.

#### More info ####

For more information on custom theme settings, read:
  https://www.drupal.org/node/177868
A simple example of theme settings is Drupal core's
'/themes/garland/theme-settings.php' and '/themes/garland/garland.info'.

For more information on custom module settings, read:
  https://www.drupal.org/node/1111260
A simple but complete example of module settings is Drupal core's
'/modules/update/update.settings.inc'.


### Code snippet examples ###

Put the following comment at the top of CSS files that contain style variables:

/**
 * The CSS values that are wrapped in '/*variable' comments are intended for use
 * by https://www.drupal.org/project/style_settings. Enable that module to
 * have those CSS variables exposed in the settings UI.
 */

An example for YOURMODULE.settings.inc or YOURMODULE.admin.inc:

  $style_settings_module = l(t('Style (CSS) Settings module'), 'https://drupal.org/project/style_settings', array(
      'attributes' => array(
        'title' => t('Style (CSS) Settings | Drupal.org'),
        'target' => '_blank',
      ),
  ));
  if (module_exists('style_settings')) {

    // Any form API element can be used 
    // A normal textfield. It does not offer any validation by itself.
    $form['YOURMODULE_caption_align'] = array(
      '#type' => 'textfield',
      '#title' => t('Caption text align'),
      '#default_value' => variable_get('YOURMODULE_caption_align', 'center'),
      '#size' => 12,
    );

    // Style Settings offers several form API elements to help developers build
    // the settings page. Cool UI widgets and built-in validation of user input.

    // Number example in this case with an appended measurement unit (optional).
    // E.g. user input: '2', field_suffix: 'px' => stored variable: '2px'.
    $form['YOURMODULE_borderwidth'] = array(
      '#type' => 'style_settings_number',
      '#title' => t('Border width'),
      '#step' => 1, // In this case if forces an integer as input.
      '#min' => 0, // Could be omitted. Defaults to 0. Set to NULL to ignore.
      '#max' => 10, // Defaults to 1 if omitted.
      // The variable default should include a measurement unit if applicable.
      // Wrapped in floatval() to turn it into a number. E.g. '2px' => '2'.
      '#default_value' => floatval(variable_get('YOURMODULE_borderwidth', '2px')),
      // The suffix gets added to the input on submit if valid measurement unit.
      '#field_suffix' => 'px',
      // Uncomment the line below to NOT align the field input on the right.
//      '#attributes' => NULL,
    );
    // Color picker example.
    $form['YOURMODULE_bordercolor'] = array(
      '#type' => 'style_settings_colorpicker',
      '#title' => t('Border color'),
      '#default_value' => variable_get('YOURMODULE_bordercolor', 'IndianRed'),
    );
    // Slider widget example.
    $form['YOURMODULE_magnifier_icon_opacity'] = array(
      '#type' => 'style_settings_slider',
      '#title' => t('Magnifier icon opacity'),
      '#description' => t('0 = transparent. 1 = opaque.'),
      '#default_value' => variable_get('YOURMODULE_magnifier_icon_opacity', 0.85),
      // Parameters below could be omitted. It is already preset for opacity.
      '#step' => 0.01,
      '#min' => 0,
      '#max' => 1,
      // Added for demonstration purpose.
      'field_suffix' => NULL,
    );
  }
  else {
    $form['YOURMODULE_note'] = array(
      '#markup' => t("Enable the !style_settings_module (<strong>dev version!</strong>) to get them exposed here. They consist of:<ul>
          <li> ... </li>
          <li> ... </li>
          <li> ... </li>
        </ul>", array('!style_settings_module' => $style_settings_module)),
    );
  }

Recommended is to add to the form submit handler:

  if (module_exists('style_settings')) {
    _drupal_flush_css_js();
  }

This way changes are visible right away after saving the form. Otherwise it is
necessary to clear the cache after changing CSS variables at:
'/admin/config/development/performance'


### About the "cache" ###

Unnecessary rewriting of CSS files is avoided through a two step process for
each monitored CSS file.

The first step generates a unique checksum key that will be used by the second
step but only if the css/js cache has been flushed.

The second step rewrites the CSS file using the checksum key if it contains
inline setting or variable code comments AND:
- theme settings have changed
OR
- variables have changed
OR
- the original CSS file has changed.
