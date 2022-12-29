project('eflete', 'c',
  version : '1.18.99',
  meson_version : '>= 0.53',
  default_options : [
    'warning_level=1',
    'buildtype=plain',
    'c_std=gnu99'
  ]
)

v_array = meson.project_version().split('.')
v_maj = v_array[0]
v_min = v_array[1]
v_mic = v_array[2]

# install paths
dir_prefix      = get_option('prefix')
dir_include     = join_paths(dir_prefix,  get_option('includedir'))
dir_pkginclude  = join_paths(dir_include, meson.project_name())
dir_data        = join_paths(dir_prefix,  get_option('datadir'))
dir_pkgdata     = join_paths(dir_data,    meson.project_name())
dir_bin         = join_paths(dir_prefix,  get_option('bindir'))
dir_lib         = join_paths(dir_prefix,  get_option('libdir'))
dir_locale      = join_paths(dir_prefix,  get_option('localedir'))

# configuration

config_h = configuration_data()
config_h.set_quoted('PACKAGE_NAME'      , meson.project_name())
config_h.set_quoted('PACKAGE_VERSION'   , meson.project_version())
config_h.set_quoted('PACKAGE_COPYRIGHT' , 'Copyright (c) 2014-2022. Eflete Authors (see AUTHORS).')
config_h.set_quoted('PACKAGE_URL'       , 'https://www.enlightenment.org')
config_h.set_quoted('PACKAGE_BUGREPORT' , 'enlightenment-devel@lists.sourceforge.net')
config_h.set_quoted('PACKAGE_STRING'    , meson.project_name() + ' ' + meson.project_version())
config_h.set_quoted('PACKAGE_BIN_DIR'   , dir_bin)
config_h.set_quoted('PACKAGE_LIB_DIR'   , dir_lib)
config_h.set_quoted('PACKAGE_DATA_DIR'  , dir_pkgdata)
config_h.set_quoted('LOCALE_DIR'        , dir_locale)

config_h.set('EFL_BETA_API_SUPPORT'           , '1')
config_h.set('EFL_EO_API_SUPPORT'             , '1')
config_h.set('EFL_UI_FOCUS_OBJECT_PROTECTED'  , '1')
config_h.set('EFLETE_BETA_API_SUPPORT'        , '1')

config_h.set_quoted('EFL_CFLAGS', run_command(find_program('pkg-config'), '--libs', '--cflags', 'elementary').stdout().strip())

config_dir = [include_directories('.')]

add_global_arguments('-DHAVE_CONFIG_H=1', '-DHAVE_CONFIG=1', language: 'c')
add_global_arguments('-DEFL_BETA_API_SUPPORT=1', language: 'c')

# binaries

eflete_cargs = [
  '-fvisibility=hidden',
  '-D_POSIX_C_SOURCE=200809L',
  '-D_XOPEN_SOURCE=500' ]

cc = meson.get_compiler('c')
eflete_cflags = []
eflete_cflags_try = [
  '-Wshadow',
  '-Wstrict-prototypes',
  '-Werror=missing-prototypes',
  '-Werror=pointer-arith',
  '-Wno-missing-field-initializers']

foreach cf: eflete_cflags_try
  if cc.has_argument(cf) == true
    eflete_cflags += cf
  endif
endforeach
add_global_arguments(eflete_cflags, language: 'c')

have_visibility_hidden = cc.has_argument('-fvisibility=hidden')
if have_visibility_hidden
  add_global_arguments('-fvisibility=hidden', language: 'c')
endif

pkgconfig = import('pkgconfig')
# windows = import('windows')

eet_exe = find_program('eet', native: true)
edje_cc_exe = find_program('edje_cc', native: true)
eolian_gen_exe = find_program('eolian_gen')

# libraries

efl_req = '>= 1.19.00'
eflete_deps = [
  dependency('eina'       , version : efl_req),
  dependency('eet'        , version : efl_req),
  dependency('evas'       , version : efl_req),
  dependency('ecore'      , version : efl_req),
  dependency('ecore-input', version : efl_req),
  dependency('ecore-imf'  , version : efl_req),
  dependency('ecore-con'  , version : efl_req),
  dependency('ecore-audio', version : efl_req),
  dependency('edje'       , version : efl_req),
  dependency('eo'         , version : efl_req),
  dependency('eio'        , version : efl_req),
  dependency('eldbus'     , version : efl_req),
  dependency('efreet'     , version : efl_req),
  dependency('efreet-mime', version : efl_req),
  dependency('elementary' , version : efl_req),
  dependency('efl'        , version : efl_req)
]

have_nls = false
if get_option('nls') == true
  intl_dep = cc.find_library('intl', required: false)
  if intl_dep.found() == true and cc.has_header('libintl.h') == true
    eflete_deps += cc.find_library('intl', required: true)
    eflete_cargs += '-DENABLE_NLS'
    have_nls = true
  endif
endif

subdir('src/lib')
subdir('src/bin')
# subdir('data/color')
# subdir('data/desktop')
# subdir('data/fonts')
# subdir('data/images')
# subdir('data/layouts')
# subdir('data/sounds')
# subdir('data/templates')
# subdir('data/themes/default')
# subdir('po')
# subdir('doc')
# subdir('tests')

# install_data(['AUTHORS'],
#   install_dir :  join_paths(get_option('prefix'), get_option('infodir'), 'eflete')
# )

# install_data(
#   sources     : 'README.md',
#   install_dir : join_paths(dir_pkgdata, 'docs')
# )

# Use config_h after all subdirs have set values
configure_file(
  output : 'config.h',
  install : false,
  configuration: config_h
)


# output
summary({'OS...:': host_machine.system(),
         'NLS..:': have_nls ? 'yes' : 'no',
         'EFL..:': dependency('efl')
        }, section: 'Configuration Options Summary:')

summary({'prefix.....:': dir_prefix,
         'bindir.....:': dir_bin,
         'libdir.....:': dir_lib,
         'incdir.....:': dir_include,
         'pkgincdir..:': dir_pkginclude,
         'datadir....:': dir_data,
         'pkgdatadir.:': dir_pkgdata,
         'localedir..:': dir_locale,
        }, section: 'Directories:')

summary({'eolian_gen path..:': eolian_gen_exe,
         'edje_cc path.....:': edje_cc_exe,
         'eet path.........:': eet_exe,
        }, section: 'Binaries:')

summary({'compilation': 'ninja -C build -v',
         'installation': 'sudo ninja -C build install',
        }, section: 'Compilation')
