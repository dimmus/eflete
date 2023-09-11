src = files([
  'change_add.c',
  'change_diff_add.c',
  'change_diff_merge_add.c',
  'change_free.c',
  'test_change.c',
])

exe = executable('test_change', src,
  dependencies : test_deps,
  include_directories : test_include_dirs,
  install : false
)

test('Eflete Test Suit - Change', exe)