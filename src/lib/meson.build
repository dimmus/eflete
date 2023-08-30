eolian_gen = find_program('eolian_gen')

eo_src = ['ewe_ruler']
eo_csrc = []
eo_gen = []

foreach eo : eo_src
  eo_file = eo + '.eo'
  eo_csrc += eo + '.c'

  eo_gen += custom_target('eolian_gen_' + eo_file,
    input : eo_file,
    output : [eo_file + '.h'],
    command : [eolian_gen, '-I', meson.current_source_dir(),
                           '-o', 'i:' + join_paths(meson.current_build_dir(), eo + '.c'),
                           '-o', 'h:' + join_paths(meson.current_build_dir(), eo_file + '.h'),
                           '-o', 'c:' + join_paths(meson.current_build_dir(), eo_file + '.c'),
                           '-o', 'd:' + join_paths(meson.current_build_dir(), eo_file + '.d'),
                           '-gchd', '@INPUT@'])
endforeach


ewe_header_src = [
  'Ewe.h'
]

install_headers(ewe_header_src,
  install_dir : dir_pkginclude + '-' + ete_version_major
)

ewe_src = files([
  'ewe_ruler.c'
 ])

ewe_lib = library('ewe', ewe_src, eo_gen,
  c_args : [ eflete_cargs,
             '-DEWE_THEME="' + join_paths(dir_pkgdata, 'themes/default/ewe.edj"'),
           ],
  # gnu_symbol_visibility : 'hidden',
  dependencies : eflete_deps,
  include_directories : [ include_directories('.'),
                          include_directories(join_paths('..','..','src/bin/logger'))],
  version : meson.project_version(),
  install : true
)

ewe = declare_dependency(
  include_directories : [ include_directories('.')],
  link_with : ewe_lib,
  sources : eo_gen,
  dependencies : eflete_deps
)
