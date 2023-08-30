color_files = [
  'color.src'
]

colors = []

_eet = find_program('eet', native: true)
eet_exe = [_eet]

foreach color_file : color_files
  colors += custom_target('eet_' + color_file,
    input       : color_file,
    # depfile     : '@BASENAME@.edj.d',
    output      : '@BASENAME@.eet',
    command     : eet_exe +
                  [
                   '-e', '@OUTPUT@',
                   'color', '@INPUT@',
                   '1'
                  ],
    install     : true,
    install_dir : join_paths(dir_pkgdata, 'themes/default/color'),
  )
endforeach