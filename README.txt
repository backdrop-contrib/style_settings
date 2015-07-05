## Style (CSS) Settings ##

This module is intended for theme and module maintainers but can be used for
customisations by anyone with basic coding skills.

Allows any module/theme to have their CSS attributes configurable from the UI.
Just by wrapping default CSS values in a code comment. These are substituted by
variables for modules or theme settings. This way the CSS is functional even if
this module is not installed. All is cached so the performance impact is
neglectable.


### How to add configurable CSS to your module or theme? ###

- Add to your '.info' file:
    soft_dependencies[] = style_settings
- Wrap values of the CSS attributes you want to be configurable in comments.
  An example for a module:
    font-size: 80%;
  becomes:
    font-size: /*variable:YOURMODULE_caption_fontsize*/80/*variable*/%;
  An example for a theme:
    font-size: 16px;
  becomes:
    font-size: /*setting:YOURTHEME_base_fontsize*/16/*setting*/px;
  The wrapped values are used as the default.
- Provide form elements on your settings page to configure module variables or
  theme settings (see below).


### How to provide form elements on your settings page? ###

Settings can be added to your settings page the usual way. It only makes sense
to show settings if the Style Settings module is installed and enabled. You
should wrap all Style Settings specific form elements inside:

  if (module_exist('style_settings')) { ... }

For more information on custom theme settings, read:
  https://www.drupal.org/node/177868
A simple example of theme settings is Drupal core's
'/themes/garland/theme-settings.php' and '/themes/garland/garland.info'.

For more information on custom module settings, read:
  https://www.drupal.org/node/1111260
A simple but complete example of module settings is Drupal core's
'/modules/update/update.settings.inc'.

Remember it might be necessary to clear the cache after changing the values at:
  '/admin/config/development/performance'
You could also add to the end of your form submit handler:

  _drupal_flush_css_js();
