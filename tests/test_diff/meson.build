src = files([
  'diff_free.c',
  'diff_undo_redo.c',
  'diff_update.c',
  'test_diff.c',
])

exe = executable('test_diff', src,
  dependencies : test_deps,
  include_directories : test_include_dirs,
  install : false
)

test('Eflete Test Suit - Diff', exe)