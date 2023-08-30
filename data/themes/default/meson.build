edc_theme_files = [
  'default.edc'
]

enventor_themes = []

_edje_cc = find_program('edje_cc', native: true)
edje_cc_exe = [_edje_cc]

foreach edc_file : edc_theme_files
  enventor_themes += custom_target('edje_cc_' + edc_file,
    input       : edc_file,
    depfile     : '@BASENAME@.edj.d',
    output      : 'eflete_elm.edj',
    command     : edje_cc_exe +
                  [
                   '-id', join_paths(meson.current_source_dir(), 'images'),
                   '-id', join_paths(meson.current_source_dir(), 'images/splash'),
                   '-fd', join_paths(meson.current_source_dir(),'../../fonts'),
                   '@INPUT@', '@OUTPUT@'
                  ],
    install     : true,
    install_dir : join_paths(dir_pkgdata, 'themes/default'),
  )
endforeach