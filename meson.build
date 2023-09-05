project('eflete', 'c',
  version : '1.19.99',
  meson_version : '>= 0.50',
  default_options : [
    'warning_level=2',
    'buildtype=plain',
    'c_std=gnu99'
  ]
)

add_global_arguments('-DHAVE_CONFIG_H=1', '-DHAVE_CONFIG=1', language: 'c')
add_global_arguments('-DEFL_BETA_API_SUPPORT=1', language: 'c')

# vers = meson.project_version().split('.')
# ete_version_major = vers[0]
# ete_version_minor = vers[1]
# ete_version_patch = vers[2].split('-')[0]

# install paths
dir_prefix      = get_option('prefix')
dir_pkgdata     = join_paths(dir_prefix, get_option('datadir'), meson.project_name())
dir_lib         = join_paths(dir_prefix, get_option('libdir'))

# configuration
config_h = configuration_data()
config_h.set_quoted('COMPILE_PATH'     , meson.current_build_dir())
config_h.set_quoted('PACKAGE'          , meson.project_name())
config_h.set_quoted('PACKAGE_VERSION'  , meson.project_version())
config_h.set_quoted('PACKAGE_COPYRIGHT', 'Copyright (c) 2014-2023. EFLETE Authors (see AUTHORS).')
config_h.set_quoted('PACKAGE_URL'      , 'https://www.enlightenment.org/about-eflete')
config_h.set_quoted('PACKAGE_TARNAME'  , meson.project_name())
config_h.set_quoted('PACKAGE_BUGREPORT', 'dmitri.chudinov@gmail.com')
config_h.set_quoted('PACKAGE_STRING'   , meson.project_name() + ' ' + meson.project_version())
config_h.set_quoted('PACKAGE_NAME'     , meson.project_name())
# config_h.set_quoted('PACKAGE_BUILD_DIR', meson.build_root())
config_h.set_quoted('PACKAGE_BIN_DIR'  , join_paths(get_option('prefix'), get_option('bindir')))
config_h.set_quoted('PACKAGE_LIB_DIR'  , dir_lib)
config_h.set_quoted('PACKAGE_DATA_DIR' , dir_pkgdata)
config_h.set_quoted('PACKAGE_DOC_DIR'  , join_paths(get_option('prefix'), get_option('infodir'), 'eflete'))
config_h.set_quoted('BUILD_TIME'       , '30.08.23') # should be $(date +"%F\ %H:%M")

config_h.set('__UNUSED_RESULT__'              , '__attribute__((warn_unused_result))')
config_h.set('__UNUSED__'                     , '__attribute__((unused))')

config_dir = [include_directories('.')]

eflete_cargs = [
  '-D_POSIX_C_SOURCE=200809L',
  '-D_XOPEN_SOURCE=500' ]

cc = meson.get_compiler('c')
eflete_cflags_try = [
  # '-Wshadow',
  '-Wstrict-prototypes',
  '-Werror=missing-prototypes',
  '-Wfloat-compare',
  '-Werror=pointer-arith',
  '-Wno-missing-field-initializers',
  '-Wcast-function-type',
  '-Wignored-qualifiers',
  '-Wmissing-parameter-type',
  '-Wnonnull',
  '-Wno-type-limits',
  '-Wold-style-declaration',
  '-Woverride-init',
  '-Wsign-compare',
  '-Wtype-limits',
  '-Wuninitialized',
  '-Wshift-negative-value',
  '-Wunused-but-set-parameter',
  '-Wunused-parameter',
  '-Wimplicit-fallthrough=2',
  '-Wno-unknown-warning-option',
  '-Wno-missing-braces',
  # '-Wconversion',
  '-Wempty-body'
]

add_project_arguments(cc.get_supported_arguments(eflete_cflags_try), language: 'c')

# pkgconfig = import('pkgconfig')

eet_exe = [find_program('eet', native: true)]
edje_cc_exe = [find_program('edje_cc', native: true)]
eolian_gen_exe = find_program('eolian_gen')

efl_req = '>= 1.26.2'
eflete_deps = [
  dependency('elementary'  , version : efl_req),
]

if get_option('enable-debug')
  config_h.set_quoted('HAVE_EFLETE_DEBUG', '1')
endif

if get_option('enable-audio')
  eflete_deps += [ dependency('ecore-audio' , version : efl_req) ]
  config_h.set_quoted('HAVE_AUDIO', '1')
endif

if get_option('enable-enventor')
  config_h.set_quoted('HAVE_ENVENTOR', '1')
endif

if get_option('enable-todo')
  config_h.set_quoted('SHOW_TODO', '1')
endif

subdir('src')
subdir('data')

if get_option('build-tests')
  check = dependency('check')
  subdir('tests')
endif

if get_option('build-doc')
  doxygen = find_program('doxygen', required : false)
  if doxygen.found()
    subdir('doc')
  endif
endif

have_nls = false
if get_option('enable-nls')
  intl_dep = cc.find_library('intl', required: false)
  if intl_dep.found() and cc.has_header('libintl.h')
    eflete_deps += cc.find_library('intl', required: true)
    eflete_cargs += '-DENABLE_NLS'
    have_nls = true
  endif
  config_h.set('ENABLE_NLS', 1)
  config_h.set_quoted('PACKAGE_LOCALE_DIR', join_paths(get_option('prefix'), get_option('localedir')))
  subdir('po')
endif

install_data(['AUTHORS'],
  install_dir :  dir_pkgdata
)

configure_file(
  output : 'eflete_config.h',
  install : false,
  configuration: config_h
)


# output
# summary({'OS...:': host_machine.system(),
#          'NLS..:': have_nls ? 'yes' : 'no',
#         #  'EFL..:': dependency('efl') # introduced in meson 0.57
#         }, section: 'Configuration Options Summary:')

# summary({'prefix.....:': dir_prefix,
#          'bindir.....:': dir_bin,
#          'libdir.....:': dir_lib,
#          'incdir.....:': dir_include,
#          'pkgincdir..:': dir_pkginclude,
#          'datadir....:': dir_data,
#          'pkgdatadir.:': dir_pkgdata,
#          'localedir..:': dir_locale,
#         }, section: 'Directories:')

# summary({'eolian_gen path..:': eolian_gen_exe,
#          'edje_cc path.....:': edje_cc_exe,
#          'eet path.........:': eet_exe,
#         }, section: 'Binaries:')

# summary({'compilation': 'ninja -C build -v',
#          'installation': 'sudo ninja -C build install',
#         }, section: 'Compilation')
