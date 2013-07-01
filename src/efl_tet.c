#include "efl_tet.h"
#include "ui_main_window.h"

App_Data *ap = NULL;

App_Data *
app_create (void)
{
   return mem_calloc(1, sizeof (App_Data));
}

void
app_free(App_Data *ap)
{
   pm_free(ap->project);
   ws_free(ap->ws);
   free(ap);
}

EAPI_MAIN int
elm_main()
{

   if (!app_init()) return -1;

   #ifdef HAVE_CONFIG_H
      INFO("%s: %s - Started...", PACKAGE_NAME, VERSION);
   #else
      CRIT("Could not find 'config.h'");
   #endif

   ap = app_create();
   ui_main_window_add(ap);

   elm_run();
   elm_shutdown();
   app_shutdown();
   return 0;
}

Eina_Bool
app_init ()
{
   if (!eina_init())
     {
        CRIT("Cannot initialize the Eina library");
        return EINA_FALSE;
     }

   if (!efreet_init())
     {
        CRIT("Cannot initialize the Efreet system");
        return EINA_FALSE;
     }

   if (!ecore_init())
     {
        CRIT("Cannot initialize the Ecore library");
        return EINA_FALSE;
     }

   if (!edje_init())
     {
        CRIT("Cannot initialize the Edje Library");
        return EINA_FALSE;
     }

   if (!logger_init())
     {
        CRIT("Cannot initialize the logger library");
        return EINA_FALSE;
     }

   if (!ecore_evas_init())
     {
        CRIT("Cannot initialize the Ecore_Evas system");
        return EINA_FALSE;
     }

   elm_theme_extension_add(NULL, TET_EDJ);
   return EINA_TRUE;
}

void
app_shutdown ()
{
   app_free(ap);
   elm_theme_extension_del(NULL, TET_EDJ);
   eina_shutdown();
   efreet_shutdown();
   ecore_shutdown();
   edje_shutdown();
   logger_shutdown();
   ecore_evas_shutdown();
}

ELM_MAIN();
