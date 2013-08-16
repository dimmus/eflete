#include "efl_tet.h"
#include "ui_main_window.h"

App_Data *app = NULL;

EAPI_MAIN int
elm_main()
{

   if (!app_init()) return -1;

   #ifdef HAVE_CONFIG_H
      INFO("%s: %s - Started...", PACKAGE_NAME, VERSION);
   #else
      CRIT("Could not find 'config.h'");
   #endif

   app = app_create();
   ui_main_window_add(app);

   elm_run();
   elm_shutdown();
   app_shutdown();
   return 0;
}

ELM_MAIN();
