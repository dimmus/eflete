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
  'Ewe.h',
  'ewe_private.h',
  'ewe_ruler_common.h',
  'ewe_ruler_eo.h',
  'ewe_ruler_legacy.h',
  'ewe_ruler.h',
  'ewe_widget_ruler.h'
]

install_headers(ewe_header_src,
  install_dir : dir_pkginclude + '-' + v_maj
)

ewe_src = files([
  'ewe_ruler.c'
 ])

ewe_lib = library('ewe', ewe_src, eo_gen,
  c_args : [ eflete_cargs,
             '-DHAVE_CONFIG_H',
             '-I' + meson.source_root(),
             '-I' + meson.current_build_dir(),
             '-I' + join_paths(meson.source_root(), 'src/lib'),
             '-I' + join_paths(meson.current_build_dir(), 'src/lib'),
             '-I' + join_paths(meson.source_root(), 'src/bin/logger'),
            #  '-DPACKAGE_DATA_DIR=' + dir_pkgdata,
            #  '-DLOCALE_DIR=' + dir_locale,
            #  '-DPACKAGE_LIB_DIR=' + dir_lib,
             '-DEWE_THEME=' + join_paths(dir_pkgdata, 'themes/default/ewe.edj'),
             '-DEFL_EO_API_SUPPORT',
             '-DEFL_BETA_API_SUPPORT',
             '-DELM_INTERNAL_API_ARGESFSDFEFC',
             '-DEWE_BUILD'
           ],
  dependencies : eflete_deps,
  include_directories : config_dir + [include_directories('.')] + [include_directories(join_paths('..','..'))] + [include_directories(join_paths('..', '..', '..'))],
  version : meson.project_version(),
  install : true
)

ewe = declare_dependency(
  include_directories : [ include_directories('.')],
  link_with : ewe_lib,
  sources : eo_gen,
  dependencies : eflete_deps
)
