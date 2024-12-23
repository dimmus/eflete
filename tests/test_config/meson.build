src = files([
  'config_get.c',
  'config_load.c',
  'config_save.c',
  'config_init.c',
  'config_panes_sizes_data_update.c',
  'config_shutdown.c',
  'profile_get.c',
  'profile_save.c',
  'profile_load.c',
  'profiles_get.c',
  'test_config.c',
])

exe = executable('test_config', src,
  dependencies : test_deps,
  include_directories : test_include_dirs,
  install : false
)

test('Eflete Test Suit - Config', exe)