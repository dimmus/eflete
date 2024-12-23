src = files([
  'history_add.c',
  'history_change_add.c',
  'history_del.c',
  'history_undo_redo.c',
  'test_history.c',
])

exe = executable('test_history', src,
  dependencies : test_deps,
  include_directories : test_include_dirs,
  install : false
)

test('Eflete Test Suit - History', exe)