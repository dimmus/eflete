project('eflete', 'c',
  version : '1.19.99',
  meson_version : '>= 0.49.2',
  default_options : [
    'buildtype=release',
    'warning_level=0',
    'werror=false',
    'c_std=gnu99'
  ]
)

add_global_arguments('-DHAVE_CONFIG_H=1', '-DHAVE_CONFIG=1', language: 'c')
add_global_arguments('-DEFL_BETA_API_SUPPORT=1', language: 'c')

version = meson.project_version()
if get_option('buildtype') != 'release'
    git_command = find_program('git', required : false)

    if git_command.found()
        git_commit = run_command(git_command, 'rev-parse', '--short', '@', check: true).stdout().strip()

        if git_commit != ''
            version += ' (git-' + git_commit + ')'
        endif
    endif
endif

# install paths
dir_prefix  = get_option('prefix')
dir_pkgdata = join_paths(dir_prefix, get_option('datadir'), meson.project_name())
dir_lib     = join_paths(dir_prefix, get_option('libdir'))
dir_bin     = join_paths(dir_prefix, get_option('bindir'))
dir_doc     = join_paths(dir_prefix, get_option('infodir'), 'eflete')

# configuration
config_h = configuration_data()
config_h.set_quoted('PACKAGE_NAME'     , meson.project_name())
config_h.set_quoted('PACKAGE_VERSION'  , version)
config_h.set_quoted('PACKAGE_BUILD_DIR', meson.current_build_dir())
config_h.set_quoted('PACKAGE_BUILD_TIME', '2023') # should be the time like $(date +"%F\ %H:%M")
config_h.set('__UNUSED_RESULT__', '__attribute__((warn_unused_result))')

config_dir = [include_directories('.')]

eflete_cargs = [
  '-D_POSIX_C_SOURCE=200809L',
  '-D_XOPEN_SOURCE=500' ]

cc = meson.get_compiler('c')
eflete_cflags_try = [
  '-Wshadow',
  '-Wno-implicit-fallthrough',
]

if get_option('buildtype') != 'release'
  eflete_cflags_try += [
    '-Wstrict-prototypes',
    '-Wmissing-prototypes',
    '-Wmissing-parameter-type',
    '-Wmissing-braces',
    '-Wmissing-field-initializers',
    '-Wpointer-arith',
    '-Wcast-function-type',
    '-Wignored-qualifiers',
    '-Wnonnull',
    '-Wold-style-declaration',
    '-Wold-style-definition',
    '-Woverride-init',
    '-Wsign-compare',
    '-Wtype-limits',
    '-Wuninitialized',
    '-Wshift-negative-value',
    '-Wunused-but-set-parameter',
    '-Wunused-parameter',
    '-Wunused-function',
    '-Wimplicit-function-declaration',
    '-Wempty-body',
    '-Wundef',
    '-Wlogical-op',
    '-Winit-self',
    '-Wfloat-equal',
    '-Wendif-labels',
    '-Wstrict-aliasing=2',
    '-Woverflow',
    '-Wformat=1',
    '-Wendif-labels',
  ]
endif

add_project_arguments(cc.get_supported_arguments(eflete_cflags_try), language: 'c')

eet_exe = find_program('eet', native: true)
edje_cc_exe = find_program('edje_cc', native: true)
eolian_gen_exe = find_program('eolian_gen')

efl_req = '>= 1.26.2'
eflete_deps = [ dependency('elementary', version : efl_req) ]

if get_option('enable-debug')
  config_h.set_quoted('HAVE_EFLETE_DEBUG', '1')
endif

if get_option('enable-audio')
  eflete_deps += [ dependency('ecore-audio' , version : efl_req) ]
  config_h.set_quoted('HAVE_AUDIO', '1')
endif

enventor_support = '       enventor: @0@'.format(get_option('enable-enventor'))
if get_option('enable-enventor')
  enventor_support = '       enventor: false (sorry, does not implemented yet) '
  # config_h.set_quoted('HAVE_ENVENTOR', '1')
endif

tizen_support = '          tizen: @0@'.format(get_option('enable-tizen'))
if get_option('enable-tizen')
  tizen_support = '          tizen: false (sorry, does not implemented yet) '
  # config_h.set('HAVE_TIZEN', '1')
endif

if get_option('enable-todo')
  config_h.set_quoted('SHOW_TODO', '1')
endif

subdir('src')
subdir('data')

if get_option('build-tests')
  config_h.set_quoted('HAVE_GETTIMEOFDAY' , '1')
  check = dependency('check')
  subdir('tests')
endif

documentation_support = '  documentation: @0@'.format(get_option('build-doc'))
if get_option('build-doc')
  documentation_support = '  documentation: false (sorry, does not implemented yet)'
  # doxygen = find_program('doxygen', required : false)
  # if doxygen.found()
    # subdir('doc')
  # endif
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
  configuration: config_h
)

summary = [
  'Eflete',
  '----------------',
  '        version: @0@'.format(version),
  '',
  '         prefix: @0@'.format(dir_prefix),
  '         libdir: @0@'.format(dir_lib),
  '        datadir: @0@'.format(dir_pkgdata),
  '         bindir: @0@'.format(dir_bin),
  '         docdir: @0@'.format(dir_doc),
  '',
  documentation_support,
  enventor_support,
  tizen_support,
  '          audio: @0@'.format(get_option('enable-audio')),
  '      debugging: @0@'.format(get_option('enable-debug')),
  '           todo: @0@'.format(get_option('enable-todo')),
  '          tests: @0@'.format(get_option('build-tests')),
  '',
]

message('\n'.join(summary))