#include "ui_main_window.h"
#include "open_file_dialog.h"
#include "style_editor.h"
#include "image_editor.h"

Eina_List *ui_list_menu;

static void
_on_edc_open_menu(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   open_edc_file(ap);
}

static void
_on_edj_open_menu(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   open_edj_file(ap);
}

static void
_on_exit_menu(void *data __UNUSED__,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   ui_main_window_del();
}

static void
_on_view_separate(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   ui_groupspace_separate(ap->ws);
}

static void
_on_view_ruler_hor(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   if (ui_ruler_visible_get(ap->ws->ruler_hor))
     ui_ruler_hide (ap->ws->ruler_hor);
   else
     ui_ruler_show (ap->ws->ruler_hor);
}

static void
_on_view_ruler_ver(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   if (ui_ruler_visible_get(ap->ws->ruler_ver))
     ui_ruler_hide (ap->ws->ruler_ver);
   else
     ui_ruler_show (ap->ws->ruler_ver);
}

static void
_on_view_ruler_rel(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   if (ui_ruler_scale_relative_visible_get(ap->ws->ruler_hor))
     {
        ui_ruler_scale_relative_visible_set (ap->ws->ruler_hor, EINA_FALSE);
        ui_ruler_scale_relative_visible_set (ap->ws->ruler_ver, EINA_FALSE);
     }
   else
     {
        ui_ruler_scale_relative_visible_set (ap->ws->ruler_hor, EINA_TRUE);
        ui_ruler_scale_relative_visible_set (ap->ws->ruler_ver, EINA_TRUE);
     }
}

static void
_on_view_ruler_abs(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   if (ui_ruler_scale_absolute_visible_get(ap->ws->ruler_hor))
     {
        ui_ruler_scale_absolute_visible_set (ap->ws->ruler_hor, EINA_FALSE);
        ui_ruler_scale_absolute_visible_set (ap->ws->ruler_ver, EINA_FALSE);
     }
   else
     {
        ui_ruler_scale_absolute_visible_set (ap->ws->ruler_hor, EINA_TRUE);
        ui_ruler_scale_absolute_visible_set (ap->ws->ruler_ver, EINA_TRUE);
     }
}


static void
_on_style_window_menu(void *data, Evas_Object *obj __UNUSED__,
                       void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   if(ap->project != NULL)
     {
        style_editor_window_add(ap->win, ap->project);
     }
   else
     NOTIFY_ERROR("EDC/EDJ file is not loaded. \n");
}

static void
_on_image_editor_menu(void *data, Evas_Object *obj __UNUSED__,
                       void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   image_editor_init(image_editor_window_add(ap->win), ap->project);
}

static void
_on_ccl_viewer_menu(void *data, Evas_Object *obj __UNUSED__,
                       void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   colorclass_viewer_init(colorclass_viewer_add(ap->win), ap->project);
}

static void
_on_font_viewer_menu(void *data, Evas_Object *obj __UNUSED__,
                       void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   font_viewer_init(font_viewer_add(ap->win), ap->project);
}

static void
_on_about_window_menu(void *data __UNUSED__, Evas_Object *obj __UNUSED__,
                       void *event_info __UNUSED__)
{
//   App_Data *ap = (App_Data *)data;
}

Eina_Bool
ui_menu_add(App_Data *ap)
{
   Evas_Object *tb, *menu;
   Elm_Object_Item *tb_it;
   Elm_Object_Item *eoi = NULL;

   tb = elm_toolbar_add(ap->win_layout);
   if (tb == NULL) return EINA_FALSE;
   else
     ui_list_menu = eina_list_append(ui_list_menu, tb);

   evas_object_size_hint_weight_set(tb, 0.0, 0.0);
   elm_toolbar_shrink_mode_set(tb, ELM_TOOLBAR_SHRINK_NONE);
   elm_toolbar_align_set(tb, 0.0);
   elm_toolbar_homogeneous_set(tb, EINA_TRUE);
   elm_object_part_content_set(ap->win_layout, "swallow/toolbar", tb);
   evas_object_show(tb);
   elm_toolbar_icon_size_set(tb,16);

   elm_toolbar_icon_order_lookup_set(tb, ELM_ICON_LOOKUP_FDO_THEME);
   tb_it = elm_toolbar_item_append(tb, NULL, "File", NULL, NULL);
   if (tb_it == NULL) return EINA_FALSE;

   elm_toolbar_item_menu_set(tb_it, EINA_TRUE);
   menu = elm_toolbar_item_menu_get(tb_it);
   if (menu == NULL) return EINA_FALSE;
   else
      ui_list_menu = eina_list_append(ui_list_menu, menu);

   elm_menu_item_add(menu, NULL, "menu/folder", "Open edc-file",
                     _on_edc_open_menu, ap);
   elm_menu_item_add(menu, NULL, "menu/folder", "Open edj-file",
                     _on_edj_open_menu, ap);
   elm_menu_item_add(menu, NULL, "menu/file", "Save", NULL, NULL);
   elm_menu_item_add(menu, NULL, "menu/close", "Exit", _on_exit_menu, ap);

   tb_it=elm_toolbar_item_append(tb, NULL, "View", NULL, NULL);
   elm_toolbar_item_menu_set(tb_it, EINA_TRUE);
   menu = elm_toolbar_item_menu_get(tb_it);

   eoi = elm_menu_item_add(menu, NULL, "window-new", "Zoom in", NULL, NULL);
   elm_object_item_disabled_set(eoi, EINA_TRUE);
   eoi = elm_menu_item_add(menu, NULL, "window-new", "Zoom out", NULL, NULL);
   elm_object_item_disabled_set(eoi, EINA_TRUE);
   elm_menu_item_add(menu, NULL, "window-new", "Separate", _on_view_separate,
                     ap);
   eoi = elm_menu_item_add(menu, NULL, "window-new", "Legend", NULL, NULL);
   elm_object_item_disabled_set(eoi, EINA_TRUE);
   elm_menu_item_add(menu, NULL, "window-new", "Ruler hor.", _on_view_ruler_hor,
                    ap);
   elm_menu_item_add(menu, NULL, "window-new", "Ruler ver.", _on_view_ruler_ver,
                     ap);
   elm_menu_item_add(menu, NULL, "window-new", "Absolute scale",
                     _on_view_ruler_abs, ap);
   elm_menu_item_add(menu, NULL, "window-new", "Relative scale",
                     _on_view_ruler_rel, ap);

   tb_it=elm_toolbar_item_append(tb, NULL, "Editors", NULL, NULL);
   elm_toolbar_item_menu_set(tb_it, EINA_TRUE);
   menu = elm_toolbar_item_menu_get(tb_it);

   elm_menu_item_add(menu, NULL, "window-new", "Styles",
                    _on_style_window_menu, ap);
   elm_menu_item_add(menu, NULL, "window-new", "Images",
                     _on_image_editor_menu, ap);
   elm_menu_item_add(menu, NULL, "window-new", "Colorclasses",
                     _on_ccl_viewer_menu, ap);
   elm_menu_item_add(menu, NULL, "window-new", "Fonts",
                     _on_font_viewer_menu, ap);

   elm_toolbar_menu_parent_set(tb, ap->win_layout);

   tb_it=elm_toolbar_item_append(tb, NULL, "Help", NULL, NULL);
   elm_toolbar_item_menu_set(tb_it, EINA_TRUE);
   menu = elm_toolbar_item_menu_get(tb_it);
   elm_menu_item_add(menu, NULL, "help-about", "About",
                     _on_about_window_menu, ap);

   tb_it = elm_toolbar_item_append(tb, NULL, "Separator", NULL, NULL);
   elm_toolbar_item_separator_set(tb_it, EINA_TRUE);
   elm_toolbar_icon_order_lookup_set(tb, ELM_ICON_LOOKUP_THEME);

   ap->main_menu = tb;
   return EINA_TRUE;
}
