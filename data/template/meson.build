edc_template_files = [
    'macros.edc',
    'template.edc',
]

eflete_templates = []

foreach edc_file : edc_template_files
  enventor_templates += custom_target('edje_cc_' + edc_file,
    input       : edc_file,
    depfile     : '@BASENAME@.edj.d',
    output      : '@BASENAME@.edj',
    command     : edje_cc_exe +
                  [
                   '-beta',
                   '-sd', join_paths(meson.current_source_dir(), 'data/template/sounds'),
                   '-id', join_paths(meson.current_source_dir(), 'data/template/images'),
                   '@INPUT@', '@OUTPUT@'
                  ],
    install     : true,
    install_dir : join_paths(dir_pkgdata, 'templates'),
  )
endforeach

install_data(
  sources     : edc_template_files,
  install_dir : join_paths(dir_pkgdata, 'themes/default/templates')
)