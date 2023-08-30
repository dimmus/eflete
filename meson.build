project('eflete', 'c',
  version : '1.19.99',
  meson_version : '>= 0.53',
  default_options : [
    'warning_level=1',
    'buildtype=plain',
    'c_std=gnu99'
  ]
)

vers = meson.project_version().split('.')
ete_version_major = vers[0]
ete_version_minor = vers[1]
ete_version_patch = vers[2].split('-')[0]

# install paths
dir_prefix      = get_option('prefix')
dir_pkgdata     = join_paths(dir_prefix, meson.project_name())

# configuration

config_h = configuration_data()
config_h.set_quoted('PACKAGE_NAME'      , meson.project_name())
config_h.set_quoted('VERSION'   , meson.project_version())
config_h.set_quoted('COMPILE_PATH'   , meson.current_build_dir())
config_h.set_quoted('PACKAGE_DATA_DIR'  , dir_pkgdata)
config_h.set('__UNUSED_RESULT__'              , '__attribute__((warn_unused_result))')
config_h.set('__UNUSED__'                     , '__attribute__((unused))')

config_dir = [include_directories('.')]

add_global_arguments('-DHAVE_CONFIG_H=1', '-DHAVE_CONFIG=1', language: 'c')
add_global_arguments('-DEFL_BETA_API_SUPPORT=1', language: 'c')

# binaries
eflete_cargs = [
  '-D_POSIX_C_SOURCE=200809L',
  '-D_XOPEN_SOURCE=500' ]

cc = meson.get_compiler('c')
eflete_cflags_try = [
  '-Wshadow',
  '-Wstrict-prototypes',
  '-Werror=missing-prototypes',
  '-Werror=pointer-arith',
  '-Wno-missing-field-initializers',
  # '-Wcast-function-type',
  # '-Wignored-qualifiers',
  # '-Wmissing-parameter-type',
  # '-Wnonnull',
  # '-Wno-type-limits',
  # '-Wold-style-declaration',
  # '-Woverride-init',
  # '-Wsign-compare',
  # '-Wtype-limits',
  # '-Wuninitialized',
  # '-Wshift-negative-value',
  # '-Wunused-but-set-parameter',
  # '-Wunused-parameter',
  # '-Wimplicit-fallthrough=2',
  # '-Wno-unknown-warning-option',
  # '-Wno-missing-braces',
  # '-Wconversion',
  # '-Wempty-body'
]

add_global_arguments(cc.get_supported_arguments(eflete_cflags_try), language: 'c')

pkgconfig = import('pkgconfig')

eet_exe = find_program('eet', native: true)
edje_cc_exe = find_program('edje_cc', native: true)
eolian_gen_exe = find_program('eolian_gen')

# libraries

efl_req = '>= 1.26.3'
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
if get_option('enable-nls')
  intl_dep = cc.find_library('intl', required: false)
  if intl_dep.found() and cc.has_header('libintl.h')
    eflete_deps += cc.find_library('intl', required: true)
    eflete_cargs += '-DENABLE_NLS'
    have_nls = true
  endif
endif

subdir('src/lib')
subdir('src/bin')
subdir('data/color')
# subdir('data/desktop')
# subdir('data/fonts')
subdir('data/images')
subdir('data/layouts')
subdir('data/sounds')
# subdir('data/templates')
subdir('data/themes/default')
# subdir('po')
# subdir('doc')
# subdir('tests')

install_data(['AUTHORS'],
  install_dir :  dir_pkgdata
)

# install_data(
#   sources     : 'README.md',
#   install_dir : join_paths(dir_pkgdata, 'docs')
# )

# Use config_h after all subdirs have set values
configure_file(
  output : 'eflete_config.h',
  #install : false, ###-> introduced in meson 0.50
  configuration: config_h
)


# output
summary({'OS...:': host_machine.system(),
         'NLS..:': have_nls ? 'yes' : 'no',
        #  'EFL..:': dependency('efl') # introduced in meson 0.57
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
