src = files([
  'cursor_main_get.c',
  'cursor_main_set.c',
  'cursor_type_get.c',
  'cursor_type_set.c',
  'test_cursor.c',
])

exe = executable('test_cursor', src,
  dependencies : test_deps,
  include_directories : test_include_dirs,
  install : false
)

test('Eflete Test Suit - Cursor', exe)