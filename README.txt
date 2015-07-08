## Style (CSS) Settings module ##

https://drupal.org/project/style_settings is intended for theme and module
maintainers but can be used for customisations by anyone with basic coding
skills (for example to provide patches for projects that could use some CSS to be configurable). It might simply be needed because a theme or module declares it as a (soft-)dependency.

Allows any module or theme to have their CSS attributes configurable from the UI
just by wrapping default CSS values in a code comment. These are then
substituted in a separate rewritten CSS file by Drupal variables or theme
settings. The CSS is functional even if this module is not installed. All is
cached so the performance impact is neglectable.


### How to add configurable CSS to your module or theme? ###

- Add to your '.info' file:
    soft_dependencies[] = style_settings
  OR if you want to make the Style (CSS) Settings module required:
    dependencies[] = style_settings
- Wrap CSS values you want to be configurable in comments.
  An example for a module:
    font-size: 80%;
  becomes:
    font-size: /*variable:YOURMODULE_caption_fontsize*/80/*variable*/%;
  An example for a theme:
    font-size: 16px;
  becomes:
    font-size: /*setting:YOURTHEME_base_fontsize*/16/*setting*/px;
  The wrapped values are used as the default. Because comments cannot reside
  within url(...), you need to add the comments around it.
- Provide form elements on your settings page to configure module variables or
  theme settings (see below).


### How to provide form elements on your settings page? ###

#### Add a conditional ####

Settings can be added to your settings page the usual way. If you declare the
Style (CSS) Settings module only as a soft-dependency (thus not required), it
makes sense to show settings only if the Style Settings module is enabled by
wrapping all Style Settings specific form elements inside a conditional:

  if (module_exists('style_settings')) { ... }

#### Add ColorPicker fields ####

A lot of CSS attributes contain a color value. Although a simple text field
could hold a color hex value, having also a colorpicker is more convenient and
the choosen color would be visible. However the Drupal form API does currently
not offer a 'colorpicker' form element type.

The Style (CSS) Settings module provides a new form API element
'style_settings_colorpicker' that:
- uses Drupal core's Farbtastic ColorPicker plugin
- makes the current choosen color visible in the settings field itself
- takes care of the validation as being a valid color
- does not depend on any contrib or the Color module
- does not need an additional jQuery library
- stores the variable value as a hex or color name that is valid in CSS files.

For project maintainers adding a color setting to their configuration is as easy
adding a few lines of code. An example how to provide form elements on your
settings page can be found in the 'code snippets' section below.

For more information on custom theme settings, read:
  https://www.drupal.org/node/177868
A simple example of theme settings is Drupal core's
'/themes/garland/theme-settings.php' and '/themes/garland/garland.info'.

For more information on custom module settings, read:
  https://www.drupal.org/node/1111260
A simple but complete example of module settings is Drupal core's
'/modules/update/update.settings.inc'.


### Code snippets ###

Put the following comment at the top of your CSS files that contains variables:

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
    $form['YOURMODULE_bgcolor'] = array(
      '#type' => 'style_settings_colorpicker',
      '#title' => t('Background color'),
      '#default_value' => variable_get('YOURMODULE_bgcolor', 'IndianRed'),
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

Recommended is to add to your form submit handler:

  if (module_exists('style_settings')) {
    _drupal_flush_css_js();
  }

This way changes are visible right away after saving the form. Otherwise it is
necessary to clear the cache after changing CSS variables at:
'/admin/config/development/performance'


### About the "cache" ###

Unnecessary rewriting of CSS files is avoided through a two step process for
each monitored CSS file.

The first step generates a unique checksum key that will be used by the second step but only
if the css/js cache has been flushed.

Only after this the second step rewrites the CSS file using the checksum key if:
- theme settings have changed
OR
- variables have changed
OR
- the original CSS file has changed.
