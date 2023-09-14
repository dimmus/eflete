edc_theme_files = [
  'eflete.edc'
]

foreach edc_file : edc_theme_files
  custom_target('edje_cc_' + edc_file,
    input       : edc_file,
    depfile     : '@BASENAME@.edj.d',
    output      : '@BASENAME@.edj',
    command     : [
                   edje_cc_exe, 
                   '-id', join_paths(meson.current_source_dir(), 'images'),
                   '-fd', join_paths(meson.current_source_dir(),'../fonts'),
                   '@INPUT@', '@OUTPUT@'
                  ],
    install     : true,
    install_dir : join_paths(dir_pkgdata, 'layouts'),
  )
endforeach