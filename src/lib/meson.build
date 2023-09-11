widgets = ['ewe_ruler']

ewe_src = files([])
include_widgets_dir = []

foreach widget : widgets
   subdir(widget)
   include_widgets_dir += [ include_directories(join_paths('..','..','src/lib/' + widget )) ]
endforeach

ewe_lib_lib = static_library('ewe', ewe_src, eo_gen,
  c_args : [ eflete_cargs,
             '-DEWE_THEME="' + join_paths(dir_pkgdata, 'themes/default/ewe.edj"'),
           ],
  dependencies : eflete_deps,
  include_directories : [ include_directories('.'),
                          include_directories(join_paths('..','..','src/bin/logger'))] 
                          + include_widgets_dir,
  install : false
)

ewe_lib = declare_dependency(
  include_directories : [ include_directories('.')] + include_widgets_dir,
  link_with : ewe_lib_lib,
  sources : eo_gen,
  dependencies : eflete_deps
)
