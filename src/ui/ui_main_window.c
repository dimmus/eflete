#include <ui_main_window.h>
#include <settings.h>


static void
_on_done(void *data __UNUSED__,
         Evas_Object *obj __UNUSED__,
         void *event_info __UNUSED__)
{
   ui_main_window_del();
}

void
ui_main_window_del(void)
{
   ui_panes_settings_save();
   INFO("%s: %s - Finished...", PACKAGE_NAME, VERSION);
   elm_exit();
}

static void
_on_window_resize(void *data __UNUSED__,
                   Evas *e __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   int w, h;
   evas_object_geometry_get(obj, NULL, NULL, &w, &h);
   ui_resize_pans(w,h);
}

Eina_Bool
ui_main_window_add(App_Data *ap)
{
   Evas_Object *win, *bg, *layout;

   eina_init();

   win = elm_win_add(NULL, "panes", ELM_WIN_BASIC);
   if (win == NULL) {
        ERR("Failrue create main window.");
        return EINA_FALSE;
   }
   ap->win = win;

   elm_win_title_set(win, PACKAGE);
   evas_object_smart_callback_add(win, "delete,request", _on_done, ap);
   evas_object_event_callback_add(win,
                                   EVAS_CALLBACK_RESIZE,
                                   _on_window_resize,
                                   NULL);

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg,
                                    EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_focus_highlight_enabled_set(win, EINA_FALSE);
   evas_object_show(bg);

   layout = elm_layout_add(win);
   evas_object_size_hint_weight_set(layout,
                                    EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, layout);
   elm_layout_file_set(layout, TET_EDJ, "ui/main_window");
   evas_object_show(layout);
   ap->win_layout = layout;

   if (!ui_menu_add(ap))
     ERR("Failrue add menu on main window.");

   if (!ui_panes_add(ap))
     ERR("Failrue add panes on main window.");

   ui_panes_settings_load(win);
   ap->ws = ws_add(ap->block.canvas);
   evas_object_show(win);

   return EINA_TRUE;
}
