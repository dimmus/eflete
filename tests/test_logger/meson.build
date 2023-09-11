src = files([
  'logger_init.c',
  'logger_shutdown.c',
  'test_logger.c',
])

exe = executable('test_logger', src,
  dependencies : test_deps,
  include_directories : test_include_dirs,
  install : false
)

test('Eflete Test Suit - Logger', exe)