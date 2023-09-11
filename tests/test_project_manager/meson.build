src = files([
  'project_manager_functional.c',
  'test_project_manager.c',
])

exe = executable('test_project_manager', src,
  dependencies : test_deps,
  include_directories : test_include_dirs,
  install : false
)

test('Eflete Test Suit - Project Manager', exe)