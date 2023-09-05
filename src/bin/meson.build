include_dirs = [ 
  include_directories('.'),
  include_directories(join_paths('..')),
  include_directories(join_paths('..','..')),
  include_directories(join_paths('..','..','src/lib')),
  include_directories(join_paths('..','..','src/bin/alloc')),
  include_directories(join_paths('..','..','src/bin/common')),
  include_directories(join_paths('..','..','src/bin/config')),
  include_directories(join_paths('..','..','src/bin/editor')),
  include_directories(join_paths('..','..','src/bin/external')),
  include_directories(join_paths('..','..','src/bin/logger')),
  include_directories(join_paths('..','..','src/bin/project_manager')),
  include_directories(join_paths('..','..','src/bin/resource_manager')),
  include_directories(join_paths('..','..','src/bin/ui')),
  include_directories(join_paths('..','..','src/bin/ui/live_view')),
  include_directories(join_paths('..','..','src/bin/ui/live_view/elementary')),
  include_directories(join_paths('..','..','src/bin/ui/modal_window')),
  include_directories(join_paths('..','..','src/bin/ui/property')),
  include_directories(join_paths('..','..','src/bin/ui/shortcuts')),
  include_directories(join_paths('..','..','src/bin/ui/sound_player')),
  include_directories(join_paths('..','..','src/bin/ui/workspace')),
]

### ete shared library

packages = [
  'alloc',
  'common',
  'config',
  'editor',
  'external',
  'logger',
  'project_manager',
  'resource_manager',
  'ui'
]

src = files([
    'eflete.c',
])

foreach package : packages
   subdir(package)
endforeach

ete_lib_lib = library('ete', src,
  dependencies : [ eflete_deps, ewe_lib ],
  include_directories : include_dirs,
  install : true
)

ete_lib = declare_dependency(
  include_directories : [ include_directories('.') ],
  link_with : ete_lib_lib,
)

### eflete

src_main = files([
    'main.c'
])

executable('eflete', src_main,
  dependencies : [ eflete_deps, ewe_lib, ete_lib ],
  include_directories : include_dirs,
  install : true
)

### eflete_exporter

src_exporter = files([
    'exporter/eflete_exporter.c',
])

executable('eflete_exporter', src_exporter,
  dependencies : [ eflete_deps, ewe_lib, ete_lib ],
  include_directories : include_dirs,
  install : true
)