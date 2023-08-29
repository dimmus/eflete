#include "main_window.h"
#include "widget_macro.h"

struct _Splash_Data
{
   Evas_Object *win;
   Splash_Cb setup;
   Splash_Cb teardown;
   Splash_Cb cancel;
   void *data;
   Splash_Status status;
};

typedef struct _Splash_Data Splash_Data;

static Splash_Data sdata;

static void
_on_teardown(void *data __UNUSED__,
             Evas_Object *obj,
             const char *emission __UNUSED__,
             const char *source __UNUSED__)
{
   if (sdata.teardown) sdata.teardown(sdata.data, sdata.status);
   evas_object_del(obj);
}

static void
_on_splash_close(void *data __UNUSED__,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   if (sdata.cancel) sdata.cancel(sdata.data, sdata.status);
   sdata.status = SPLASH_CANCEL;
   elm_layout_signal_emit(sdata.win, "end", "eflete");
}

static void
_on_setup(void *data __UNUSED__,
          Evas_Object *obj __UNUSED__,
          const char *emission __UNUSED__,
          const char *source __UNUSED__)
{
   elm_object_focus_set(elm_object_focused_object_get(ap.win), false);
   elm_layout_signal_emit(sdata.win, "start", "eflete");
   if (sdata.setup)
     if (!sdata.setup(sdata.data, sdata.status))
       {
          elm_layout_signal_emit(sdata.win, "end", "eflete");
          return;
       }
}

Evas_Object *
splash_add(Evas_Object *parent,
           Splash_Cb setup,
           Splash_Cb teardown,
           Splash_Cb cancel,
           void *data)
{
   Evas_Object *bt;

   sdata.setup = setup;
   sdata.teardown = teardown;
   sdata.cancel = cancel;
   sdata.data = data;
   sdata.status = SPLASH_NONE;

   assert(parent != NULL);

   sdata.win = elm_win_inwin_add(parent);
   elm_object_style_set(sdata.win, "splash");

   if (cancel)
     {
        BUTTON_ADD(sdata.win, bt, "Cancel");
        elm_object_content_set(sdata.win, bt);
        evas_object_smart_callback_add(bt, signals.elm.button.clicked, _on_splash_close, NULL);
     }

   elm_layout_signal_callback_add(sdata.win, "setup", "eflete", _on_setup, NULL);
   elm_layout_signal_callback_add(sdata.win, "teardown", "eflete", _on_teardown, NULL);

   ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_MAIN, true);
   return sdata.win;
}

void
splash_del(Evas_Object *obj)
{
   assert(obj != NULL);

   sdata.status = SPLASH_SUCCESS;
   elm_layout_signal_emit(obj, "end", "eflete");
   ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_MAIN, false);
}
