dep_intl = []

if get_option('enable-nls')
  i18n = import('i18n')
#   add_project_arguments('-DGETTEXT_PACKAGE="ensen"', language:'c')
  i18n.gettext(meson.project_name(),
             args: [
                # '--directory=' + meson.source_root(),
                '--keyword=_',
                '--keyword=d_:1',
                '--keyword=P_:1,2',
                '--keyword=dP_:1,2',
                '--keyword=N_',
                '--keyword=NP_:1,2',
                '--from-code=UTF-8',
                '--foreign-user'
              ])
  config_h.set('HAVE_GETTEXT', '1')
  dep_intl = cc.find_library('intl', required : false)
  if dep_intl.found() and cc.has_header('libintl.h')
    eflete_deps += cc.find_library('intl', required: true)
    eflete_cargs += '-DENABLE_NLS'
    have_nls = true
  endif
endif

# maintainer shortcut for updating po stuff
# if (meson.version().split('.')[1] < 56)
  run_target ('po',
    command: ['ninja', '-C', meson.current_build_dir(),
      meson.project_name() + '-pot',
      meson.project_name() + '-update-po']
  )
# else
#   run_target ('po',
#     command: ['ninja', '-C', meson.project_build_root(),
#       meson.project_name() + '-pot',
#       meson.project_name() + '-update-po']
#   )
# endif