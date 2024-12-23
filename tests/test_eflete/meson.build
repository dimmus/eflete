src = files([
  'app_init.c',
  'app_shutdown.c',
  'test_eflete.c',
])

exe = executable('test_eflete', src,
  dependencies : test_deps,
  include_directories : test_include_dirs,
  install : false
)

test('Eflete Test Suit - Eflete', exe)