/*
 * Edje Theme Editor
 * Copyright (C) 2013-2015 Samsung Electronics.
 *
 * This file is part of Edje Theme Editor.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; If not, see www.gnu.org/licenses/lgpl.html.
 */

#include "main_window.h"
#include "widget_macro.h"
#include "project_manager2.h"
#include "config.h"
#include "shortcuts.h"

static Evas_Object *helper;
static Evas_Object *fs;
static Helper_Done_Cb dismiss_func;
static void* func_data;

static Popup_Current current;

struct _Search_Data
{
   Evas_Object *search_entry;
   Elm_Object_Item *last_item_found;
};
typedef struct _Search_Data Search_Data;
struct _Helper_Data
{
   Evas_Object *gengrid;
   Evas_Object *button; /* to avoid eflete_main_loop_quit, look at 431 in here */
   Evas_Object *follow_up;
   Search_Data image_search_data;
};
typedef struct _Helper_Data Helper_Data;

/* this one is for gengrid items */
struct _Item
{
   const char* image_name;
   const char* source;
   Image_Instance_Type type;
};
typedef struct _Item Item;
static Elm_Gengrid_Item_Class *gic = NULL;
static Elm_Gengrid_Item_Class *gic_set = NULL;

static void
_delete_object_job(void *data)
{
   shortcuts_object_check_pop(data);
   evas_object_del(data);
   current = POPUP_NONE;
}

/* async popup */
#define POPUP_DATA "POPUP_DATA"
typedef struct {
   Evas_Object *popup;
   struct {
      Evas_Object *ok,
                  *cancel,
                  *save,
                  *dont_save,
                  *replace,
                  *append,
                  *reset;
   } button;
} Popup_Data;

static void
_popup_del_job(void *data)
{
   Popup_Data *pd= data;
   evas_object_del(pd->popup);
   free(pd);
}

static void
_popup_btn_cb(void *data,
              Evas_Object *obj,
              void *ei __UNUSED__)
{
   Popup_Data *pd = evas_object_data_get(obj, POPUP_DATA);

   assert(pd != NULL);
   assert(pd->popup != NULL);

   ecore_job_add(_popup_del_job, pd);
   shortcuts_object_check_pop(pd->popup);
   evas_object_smart_callback_call(pd->popup, POPUP_CLOSE_CB, data);
   /* menu clould be deleted in POPUP_CLOSE_CB in exit confirmation popup */
   if (ap.menu)
     ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_MAIN, false);
}

static Evas_Object *
_button_add(Popup_Data *pd, int *btn_pos, const char *text, Popup_Button pb)
{
   Evas_Object *btn;
   static const char* position_name[] = { "button1", "button2", "button3" };

   assert(pd != NULL);
   assert(btn_pos != NULL);
   assert(text != NULL);

   if (pb == BTN_NONE)
     return NULL;

   assert(*btn_pos < 3); /* maximum buttons count */

   BUTTON_ADD(pd->popup, btn, text);
   evas_object_data_set(btn, POPUP_DATA, pd);
   evas_object_smart_callback_add(btn, signals.elm.button.clicked, _popup_btn_cb, (void *)pb);
   elm_object_part_content_set(pd->popup, position_name[*btn_pos], btn);
   *btn_pos = *btn_pos + 1;

   return btn;
}

static void
_popup_button_click(void *data,
                    Evas_Object *obj __UNUSED__,
                    void *event_info __UNUSED__)
{
   Evas_Object *btn = data;

   assert(btn != NULL);

   if (!elm_object_disabled_get(btn))
     evas_object_smart_callback_call(btn, signals.elm.button.clicked, NULL);
}

Evas_Object *
popup_add(const char *title,
          const char *msg,
          Popup_Button popup_btns,
          Popup_Content_Get_Func content_get,
          void *data)
{
   Popup_Data *pd;

   /* only one content will be setted to popup: or message, or used content */
   assert((msg != NULL) != (content_get != NULL));

   pd = mem_calloc(1, sizeof(Popup_Data));
   pd->popup = elm_popup_add(ap.win);
   elm_popup_orient_set(pd->popup, ELM_POPUP_ORIENT_CENTER);
   elm_object_part_text_set(pd->popup, "title,text", title);
   elm_popup_content_text_wrap_type_set(pd->popup, ELM_WRAP_WORD);

   evas_object_data_set(pd->popup, POPUP_DATA, pd);

   int bt_num = 0;
   pd->button.ok        = _button_add(pd, &bt_num, _("Ok"),         popup_btns & BTN_OK);
   pd->button.save      = _button_add(pd, &bt_num, _("Save"),       popup_btns & BTN_SAVE);
   pd->button.replace   = _button_add(pd, &bt_num, _("Replace"),    popup_btns & BTN_REPLACE);
   pd->button.dont_save = _button_add(pd, &bt_num, _("Don't save"), popup_btns & BTN_DONT_SAVE);
   pd->button.cancel    = _button_add(pd, &bt_num, _("Cancel"),     popup_btns & BTN_CANCEL);
   pd->button.reset     = _button_add(pd, &bt_num, _("Reset"),      popup_btns & BTN_RESET);

   if (pd->button.ok)
     evas_object_smart_callback_add(pd->popup, signals.shortcut.popup.done, _popup_button_click, pd->button.ok);
   if (pd->button.cancel)
     evas_object_smart_callback_add(pd->popup, signals.shortcut.popup.cancel, _popup_button_click, pd->button.cancel);

   if (msg)
     elm_object_text_set(pd->popup, msg);
   else /* content_get != NULL */
     {
        Evas_Object *to_focus = NULL;
        Evas_Object *content = content_get(data, pd->popup, &to_focus);
        elm_object_content_set(pd->popup, content);
        if (to_focus)
          elm_object_focus_set(to_focus, true);
     }

   shortcuts_object_push(pd->popup);
   evas_object_show(pd->popup);

   ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_MAIN, true);

   elm_object_focus_set(pd->button.ok, false);
   elm_object_focus_set(pd->button.save, false);
   elm_object_focus_set(pd->button.replace, false);
   elm_object_focus_set(pd->button.dont_save, false);
   elm_object_focus_set(pd->button.cancel, false);
   elm_object_focus_set(pd->button.reset, false);

   return pd->popup;
}

void
popup_button_disabled_set(Evas_Object *popup, Popup_Button btn, Eina_Bool disabled)
{
   assert(popup != NULL);

   if (btn == BTN_NONE) return;

   Popup_Data *pd = evas_object_data_get(popup, POPUP_DATA);
   switch (btn)
     {
      case BTN_OK:
         elm_object_disabled_set(pd->button.ok, disabled);
         break;
      case BTN_SAVE:
         elm_object_disabled_set(pd->button.save, disabled);
         break;
      case BTN_REPLACE:
         elm_object_disabled_set(pd->button.replace, disabled);
         break;
      case BTN_DONT_SAVE:
         elm_object_disabled_set(pd->button.dont_save, disabled);
         break;
      case BTN_CANCEL:
         elm_object_disabled_set(pd->button.cancel, disabled);
         break;
      case BTN_RESET:
         elm_object_disabled_set(pd->button.reset, disabled);
         break;
      default:
         ERR("Unknown button.");
         abort(); /* only one single button allowed */
     }
}
/* end of async popup */

#if HAVE_TIZEN
#define FS_W 510
#define FS_H 500
#else
#define FS_W 430
#define FS_H 460
#endif

#define GENGRID_W 522
#define GENGRID_H 388

#define COLOR_W 261
#define COLOR_H 300

static void
_helper_obj_follow(void *data __UNUSED__,
                   Evas *e __UNUSED__,
                   Evas_Object *obj,
                   void *event_info __UNUSED__)
{
   int w, h; /* window */
   int nx, ny, nh, nw; /* follow object */
   int hh, hw; /* helper */

   int move_x, move_y;

   evas_object_geometry_get(ap.win, NULL, NULL, &w, &h);
   evas_object_geometry_get(obj, &nx, &ny, &nw, &nh);
   evas_object_geometry_get(helper, NULL, NULL, &hw, &hh);

   move_x = nx - (hw + 12 - nw); /* 12 - it's a helper border */
   if (ny + nh + hh < h)
     move_y = ny + nh;
   else
     move_y = ny - hh;

   evas_object_move(helper, move_x, move_y);
}

static void
_helper_win_follow(void *data __UNUSED__,
                  Evas *e __UNUSED__,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   int w, h, nx, ny;

   evas_object_geometry_get(ap.win, NULL, NULL, &w, &h);
   nx = (w / 2) - ((FS_W + 12) / 2);
   ny = (h / 2) - ((FS_H + 12) / 2);
   evas_object_move(helper, nx, ny);
}

static void
_helper_property_follow(void *data __UNUSED__,
                        Evas *e __UNUSED__,
                        Evas_Object *obj,
                        void *event_info __UNUSED__)
{
   int w, h; /* window */
   int nx, ny, nh, nw; /* follow object */
   int hh, hw; /* helper */

   int move_x, move_y;

   evas_object_geometry_get(ap.win, NULL, NULL, &w, &h);
   evas_object_geometry_get(obj, &nx, &ny, &nw, &nh);
   evas_object_geometry_get(helper, NULL, NULL, &hw, &hh);

   move_x = nx - (hw + 12 - nw); /* 12 - it's a helper border */
   if (ny + nh + hh < h)
     move_y = ny + nh;
   else
     move_y = ny - hh;

   evas_object_move(helper, move_x, move_y);
}

static void
_helper_property_color_follow(void *data __UNUSED__,
                              Evas *e __UNUSED__,
                              Evas_Object *obj,
                              void *event_info __UNUSED__)
{
   int w, h; /* window */
   int nx, ny, nh, nw; /* follow object */
   int hh, hw; /* helper */

   int move_x, move_y;

   evas_object_geometry_get(ap.win, NULL, NULL, &w, &h);
   evas_object_geometry_get(obj, &nx, &ny, &nw, &nh);
   evas_object_geometry_get(helper, NULL, NULL, &hw, &hh);

   move_x = nx - (hw + 12 - nw); /* 12 - it's a helper border */
   if (ny + nh + hh < h)
     move_y = ny + nh;
   else
     move_y = ny - hh;

   evas_object_move(helper, move_x, move_y);
}

static void
_helper_colorclass_dismiss(void *data,
                           Evas_Object *obj __UNUSED__,
                           const char *signal __UNUSED__,
                           const char *source __UNUSED__)
{
   Helper_Data *helper_data = (Helper_Data *)data;

   assert(helper_data != NULL);

   Evas_Object *follow_up = helper_data->follow_up;

   evas_object_event_callback_del_full(follow_up, EVAS_CALLBACK_RESIZE, _helper_obj_follow, NULL);
   evas_object_event_callback_del_full(follow_up, EVAS_CALLBACK_MOVE, _helper_obj_follow, NULL);
   evas_object_event_callback_del_full(follow_up, EVAS_CALLBACK_RESIZE, _helper_property_color_follow, NULL);
   evas_object_event_callback_del_full(follow_up, EVAS_CALLBACK_MOVE, _helper_property_color_follow, NULL);

   if (!follow_up)
     evas_object_event_callback_del_full(ap.win, EVAS_CALLBACK_RESIZE, _helper_win_follow, NULL);

   if (dismiss_func)
     ((Helper_Done_Cb)dismiss_func)(func_data, fs, NULL);
   dismiss_func = NULL;
   func_data = NULL;
   /* using eflete_main_loop_quit/begin doesn't work here since it blocks
      thumbs inside of a gengrid.
      though to avoid SIGSEV deleting button first and then popup works
      perfectly good */
   if (helper_data->button)
     evas_object_del(helper_data->button);

   free(helper_data);

   ecore_job_add(_delete_object_job, helper);
}

#if !HAVE_TIZEN
static void
_colorclass_done(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   _helper_colorclass_dismiss(data, helper, NULL, NULL);
}
#endif

void
popup_fileselector_helper_dismiss()
{
   Evas_Object *follow_up = (Evas_Object *) helper;

   evas_object_event_callback_del_full(follow_up, EVAS_CALLBACK_RESIZE, _helper_obj_follow, NULL);
   evas_object_event_callback_del_full(follow_up, EVAS_CALLBACK_MOVE, _helper_obj_follow, NULL);
   evas_object_event_callback_del_full(follow_up, EVAS_CALLBACK_RESIZE, _helper_property_follow, NULL);
   evas_object_event_callback_del_full(follow_up, EVAS_CALLBACK_MOVE, _helper_property_follow, NULL);

   if (!follow_up)
     evas_object_event_callback_del_full(ap.win, EVAS_CALLBACK_RESIZE, _helper_win_follow, NULL);

   Helper_Data *helper_data = evas_object_data_get(helper, "STRUCT");
   if (helper_data) free(helper_data);

   ecore_job_add(_delete_object_job, helper);
}

static void
_helper_dismiss(void *data __UNUSED__,
                Evas_Object *obj __UNUSED__,
                const char *signal __UNUSED__,
                const char *source __UNUSED__)
{
   Evas_Object *follow_up = (Evas_Object *) data;

   evas_object_event_callback_del_full(follow_up, EVAS_CALLBACK_RESIZE, _helper_obj_follow, NULL);
   evas_object_event_callback_del_full(follow_up, EVAS_CALLBACK_MOVE, _helper_obj_follow, NULL);
   evas_object_event_callback_del_full(follow_up, EVAS_CALLBACK_RESIZE, _helper_property_follow, NULL);
   evas_object_event_callback_del_full(follow_up, EVAS_CALLBACK_MOVE, _helper_property_follow, NULL);

   if (!follow_up)
     evas_object_event_callback_del_full(ap.win, EVAS_CALLBACK_RESIZE, _helper_win_follow, NULL);

   Helper_Data *helper_data = evas_object_data_get(helper, "STRUCT");
   if (helper_data) free(helper_data);

   ecore_job_add(_delete_object_job, helper);
}

static void
_done(void *data __UNUSED__,
      Evas_Object *obj,
      void *event_info)
{
   Eina_List *selected_paths = NULL;
   Eina_Stringshare *selected;
   Eina_Bool res = false;

   if (ap.last_path)
     eina_stringshare_del(ap.last_path);
   ap.last_path = eina_stringshare_add(elm_fileselector_path_get(fs));

   if (!event_info) res = true;
   if (!res && dismiss_func)
     {
        if (elm_fileselector_multi_select_get(fs))
          selected_paths = (Eina_List *)elm_fileselector_selected_paths_get(fs);
        else
          {
             if (elm_fileselector_is_save_get(fs))
               selected = eina_stringshare_printf("%s/%s",
                                                  elm_fileselector_path_get(fs),
                                                  elm_fileselector_current_name_get(fs));
             else
               selected = eina_stringshare_add(elm_fileselector_selected_get(fs));
             selected_paths = eina_list_append(selected_paths, selected);
          }
        res = ((Helper_Done_Cb)dismiss_func)(func_data, obj, selected_paths);

        if (!elm_fileselector_multi_select_get(fs))
          EINA_LIST_FREE(selected_paths, selected)
             eina_stringshare_del(selected);
     }

   if (res)
     {
        dismiss_func = NULL;
        func_data = NULL;
        _helper_dismiss(data, helper, NULL, NULL);
     }
}

static void
_fileselector_helper(const char *title,
                     Evas_Object *follow_up,
                     const char *path,
                     Eina_Bool multi,
                     Eina_Bool is_save,
                     Helper_Done_Cb func,
                     void *data,
                     Elm_Fileselector_Filter_Func filter_cb)
{
   Evas_Object *scroller;

   dismiss_func = func;
   func_data = data;

   helper = elm_layout_add(ap.win);
   elm_layout_theme_set(helper, "layout", "popup", title ? "hint_title" : "hint");
   elm_layout_signal_callback_add(helper, "hint,dismiss", "eflete", _helper_dismiss, follow_up);

   fs = elm_fileselector_add(ap.win);
#if HAVE_TIZEN
   /* Dirty Hack */
   Evas_Object *files_list;
   files_list = elm_object_part_content_get(fs, "elm.swallow.files");
   elm_object_style_set(files_list, "manager");
   /*------------*/
#endif
   elm_fileselector_expandable_set(fs, false);
   elm_fileselector_is_save_set(fs, is_save);
   elm_fileselector_multi_select_set(fs, multi);
   if (filter_cb)
     {
        elm_fileselector_custom_filter_append(fs, filter_cb, NULL, "edj");
        evas_object_hide(elm_layout_content_unset(fs, "elm.swallow.filters"));
     }
   else elm_fileselector_folder_only_set(fs, true);

   elm_fileselector_path_set(fs, (path && (strcmp(path, ""))) ? path :
                             (ap.last_path) ? ap.last_path : profile_get()->general.projects_folder);
   evas_object_smart_callback_add(fs, signals.elm.fileselector.done, _done, follow_up);
   evas_object_smart_callback_add(fs, signals.elm.fileselector.activated, _done, follow_up);
   evas_object_size_hint_min_set(helper, FS_W, FS_H);
   evas_object_resize(helper, FS_W, FS_H);

   /* scroller is necessary to fix issues with fileselector size */
   SCROLLER_ADD(ap.win, scroller);
   elm_object_content_set(scroller, fs);

   if (title) elm_object_text_set(helper, title);
   elm_layout_content_set(helper, "elm.swallow.content", scroller);
   evas_object_size_hint_weight_set(fs, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(fs, EVAS_HINT_FILL, EVAS_HINT_FILL);
   if (follow_up)
     {
        _helper_obj_follow(NULL, NULL, follow_up, NULL);
        evas_object_event_callback_add(follow_up, EVAS_CALLBACK_RESIZE, _helper_obj_follow, NULL);
        evas_object_event_callback_add(follow_up, EVAS_CALLBACK_MOVE, _helper_obj_follow, NULL);
     }
   else
     {
        _helper_win_follow(NULL, NULL, NULL, NULL);
        evas_object_event_callback_add(ap.win, EVAS_CALLBACK_RESIZE, _helper_win_follow, NULL);
     }
   shortcuts_object_push(helper);
   evas_object_show(helper);
}

void
popup_fileselector_folder_helper(const char *title, Evas_Object *follow_up, const char *path,
                                 Helper_Done_Cb func, void *data,
                                 Eina_Bool multi, Eina_Bool is_save)
{
   _fileselector_helper(title, follow_up, path, multi, is_save, func, data, NULL);
   current = POPUP_FILESELECTOR_FOLDER_HELPER;
}

static Eina_Bool
_edj_filter(const char *path,
            Eina_Bool dir,
            void *data __UNUSED__)
{
   if (dir) return true;

   if (eina_str_has_extension(path, ".edj"))
     return true;
   return false;
}

void
popup_fileselector_edj_helper(const char *title, Evas_Object *follow_up, const char *path,
                              Helper_Done_Cb func, void *data,
                              Eina_Bool multi, Eina_Bool is_save)
{
   _fileselector_helper(title, follow_up, path, multi, is_save, func, data, _edj_filter);
   current = POPUP_FILESELECTOR_EDJ_HELPER;
}

static Eina_Bool
_edc_filter(const char *path,
            Eina_Bool dir,
            void *data __UNUSED__)
{
   if (dir) return true;

   if (eina_str_has_extension(path, ".edc"))
     return true;
   return false;
}

void
popup_fileselector_edc_helper(const char *title, Evas_Object *follow_up, const char *path,
                              Helper_Done_Cb func, void *data,
                              Eina_Bool multi, Eina_Bool is_save)
{
   _fileselector_helper(title, follow_up, path, multi, is_save, func, data, _edc_filter);
   current = POPUP_FILESELECTOR_EDC_HELPER;
}

static Eina_Bool
_images_filter(const char *path,
               Eina_Bool dir,
               void *data __UNUSED__)
{
   int i;
   Eina_Bool res;
   const char *image_formats[] = { "png", "jpg", "jpeg", "jfif", "xpm", "tif",
                                   "tiff", "gif", "pbm", "pgm", "ppm", "pnm",
                                   "bmp", "wbmp", "webp", "psd", "tga", "svg",
                                   NULL};
   if (dir) return true;
   i = 0;
   while(image_formats[i])
     {
        res = eina_str_has_extension(path, image_formats[i++]);
        if (res) return true;
     }

   return false;
}

void
popup_fileselector_image_helper(const char *title, Evas_Object *follow_up, const char *path,
                                Helper_Done_Cb func, void *data,
                                Eina_Bool multi, Eina_Bool is_save)
{
   _fileselector_helper(title, follow_up, path, multi, is_save, func, data, _images_filter);
   current = POPUP_FILESELECTOR_IMAGE_HELPER;
}

static Eina_Bool
_sounds_filter(const char *path,
               Eina_Bool dir,
               void *data __UNUSED__)
{
   int i;
   Eina_Bool res;
   const char *sound_formats[] = { "wav", "ogg", "flac", NULL};

   if (dir) return true;
   i = 0;
   while(sound_formats[i])
     {
        res = eina_str_has_extension(path, sound_formats[i++]);
        if (res) return true;
     }

   return false;
}

void
popup_fileselector_sound_helper(const char *title, Evas_Object *follow_up, const char *path,
                                Helper_Done_Cb func, void *data,
                                Eina_Bool multi, Eina_Bool is_save)
{
   _fileselector_helper(title, follow_up, path, multi, is_save, func, data, _sounds_filter);
   current = POPUP_FILESELECTOR_SOUND_HELPER;
}

void
popup_fileselector_file_set(const char *file)
{
   Evas_Object *entry;

   entry = elm_object_part_content_get(fs, "elm.swallow.filename");
   elm_entry_entry_set(entry, file);
}

#define ITEM_WIDTH 100
#define ITEM_HEIGHT 115
#define GROUP_ITEM_WIDTH 36
#define GROUP_ITEM_HEIGHT 36

static void
_done_image(void *data,
            Evas_Object *obj __UNUSED__,
            void *event_info __UNUSED__)
{
   Eina_Bool res = true;
   Item *item = NULL;
   const Eina_List *sel_list, *l;
   Eina_List *ret_list = NULL;
   Elm_Object_Item *item_list;

   Helper_Data *helper_data = (Helper_Data *)data;

   if (dismiss_func)
     {
        sel_list = elm_gengrid_selected_items_get(helper_data->gengrid);
        EINA_LIST_FOREACH(sel_list, l, item_list)
          {
             item = elm_object_item_data_get(item_list);
             ret_list = eina_list_append(ret_list, eina_stringshare_add(item->image_name));
          }
        res = ((Helper_Done_Cb)dismiss_func)(func_data, helper_data->gengrid, ret_list);
        eina_list_free(ret_list);
     }

   if (res)
     {
        dismiss_func = NULL;
        func_data = NULL;
        /* using eflete_main_loop_quit/begin doesn't work here since it blocks
           thumbs inside of a gengrid.
           though to avoid SIGSEV deleting button first and then popup works
           perfectly good */
        if (helper_data->button)
          evas_object_del(helper_data->button);
        _helper_dismiss(helper_data->follow_up, helper, NULL, NULL);
     }
}

/* deletion callback */
static void
_grid_del(void *data,
          Evas_Object *obj __UNUSED__)
{
   Item *it = data;

   assert(it != NULL);

   eina_stringshare_del(it->image_name);
   eina_stringshare_del(it->source);
   free(it);
}

static Eina_Bool
_image_gengrid_init(Helper_Data *helper_data)
{
   Eina_List *l = NULL;
   Item *it = NULL;
   Eina_List *images = NULL;
   int counter = 0;
   Image2 *res;
   Image_Set2 *image_set = NULL;

   images = ap.project->RM.images;

   /* initial zero image */
   it = (Item *)mem_malloc(sizeof(Item));
   it->image_name = eina_stringshare_add(EFLETE_DUMMY_IMAGE_NAME);
   it->source = eina_stringshare_add(EFLETE_DUMMY_IMAGE_NAME);
   elm_gengrid_item_append(helper_data->gengrid, gic, it, NULL, NULL);

   if (images)
     {
        EINA_LIST_FOREACH(images, l, res)
           {
              counter++;
              if (!res->common.name)
                {
                   ERR("name not found for image #%d",counter);
                   continue;
                }
              if (!strcmp(res->common.name, EFLETE_DUMMY_IMAGE_NAME)) continue;

              it = (Item *)mem_malloc(sizeof(Item));
              it->image_name = eina_stringshare_add(res->common.name);
              it->source = eina_stringshare_add(res->source);
              it->type = SINGLE_IMAGE;
              elm_gengrid_item_append(helper_data->gengrid, gic, it, NULL, NULL);
           }
         elm_gengrid_item_bring_in(elm_gengrid_first_item_get(helper_data->gengrid),
                                   ELM_GENGRID_ITEM_SCROLLTO_TOP);
     }

   counter = 0;
   EINA_LIST_FOREACH(ap.project->RM.image_sets, l, image_set)
     {
        counter++;
        if (!image_set->common.name)
          {
             ERR("name not found for image #%d",counter);
             continue;
          }

        it = (Item *)mem_malloc(sizeof(Item));
        it->image_name = eina_stringshare_add(image_set->common.name);
        res = eina_list_data_get(image_set->common.uses___);
        it->source = eina_stringshare_add(res->source);
        it->type = IMAGE_SET;
        elm_gengrid_item_append(helper_data->gengrid, gic_set, it, NULL, NULL);
     }

   elm_scroller_policy_set(helper_data->gengrid, ELM_SCROLLER_POLICY_OFF,
                           ELM_SCROLLER_POLICY_AUTO);
   evas_object_smart_calculate(helper_data->gengrid);

   return true;
}

static char *
_grid_label_get(void *data,
                Evas_Object *obj __UNUSED__,
                const char  *part __UNUSED__)
{
   const Item *it = data;
   Resource2 *res;

   if (!strcmp(part, "elm.text.count"))
     {
        res = resource_manager_find(ap.project->RM.image_sets, it->image_name);
        int count = eina_list_count(res->common.uses___);
        if (count <= 4) return strdup("");
        char buf[256];
        snprintf(buf, 256, "%d", count);
        return strdup(buf);
     }
   else
     {
        if (strcmp(it->image_name, EFLETE_DUMMY_IMAGE_NAME) == 0)
          return strdup("None");
        return strdup(it->image_name);
     }
}

/* icon fetching callback */
#define MAX_ICON_SIZE 16
static Evas_Object *
_grid_content_get(void *data,
                  Evas_Object *obj,
                  const char  *part)
{
   Item *it = data;
   Evas_Object *image_obj = NULL;
   Evas_Object *grid = (Evas_Object *)obj;
   Resource2 *res;

   assert(it != NULL);
   assert(grid != NULL);

   if (!strcmp(part, "elm.swallow.icon"))
     {
#ifndef _WIN32
        image_obj = elm_thumb_add(grid);
        if (strcmp(it->image_name, EFLETE_DUMMY_IMAGE_NAME) != 0)
          elm_thumb_file_set(image_obj, it->source, NULL);
#else
        TODO("Remove this urgly hack when we fix thumbs on Windows")
        image_obj = elm_image_add(grid);
        if (strcmp(it->image_name, EFLETE_DUMMY_IMAGE_NAME) != 0)
          elm_image_file_set(image_obj, it->source, NULL);
#endif /* _WIN32 */
        elm_object_style_set(image_obj, "noframe");
        evas_object_show(image_obj);
     }
   else if ((!strcmp(part, "elm.swallow.end") && (strcmp(it->image_name, EFLETE_DUMMY_IMAGE_NAME) != 0)))
     {
        res = resource_manager_find(ap.project->RM.images, it->image_name);
        if (eina_list_count(res->common.used_in) == 0)
          {
             image_obj = elm_icon_add(grid);
             elm_image_file_set(image_obj, ap.path.theme_edj, "elm/image/icon/attention");
             evas_object_show(image_obj);
          }
     }

   return image_obj;
}
#undef MAX_ICON_SIZE

/* icon fetching callback */
static Evas_Object *
_grid_image_set_content_get(void *data,
                            Evas_Object *obj,
                            const char  *part)
{
   Item *it = data;
   Evas_Object *image_obj = NULL;
   Evas_Object *grid = (Evas_Object *)obj;
   Resource2 *res;
   Image2 *img_res = NULL;

   assert(it != NULL);
   assert(grid != NULL);

   if (!strcmp(part, "elm.swallow.end") && (strcmp(it->image_name, EFLETE_DUMMY_IMAGE_NAME) != 0))
     {
        res = resource_manager_find(ap.project->RM.image_sets, it->image_name);
        if (!res->common.used_in)
          {
             image_obj = elm_icon_add(grid);
             elm_image_file_set(image_obj, ap.path.theme_edj, "elm/image/icon/attention");
             evas_object_show(image_obj);
          }
     }
   else
     {
#ifndef _WIN32
        image_obj = elm_thumb_add(grid);
#else
        image_obj = elm_image_add(grid);
#endif /* _win32 */
        elm_object_style_set(image_obj, "noframe");
        res = resource_manager_find(ap.project->RM.image_sets, it->image_name);
        int count = eina_list_count(res->common.uses___);

        if (!strcmp(part, "elm.swallow.first"))
          {
             img_res = (Image2 *)(eina_list_nth(res->common.uses___, 0));
          }
        else if (!strcmp(part, "elm.swallow.second"))
          {
             img_res = (Image2 *)(eina_list_nth(res->common.uses___, 1));
          }
        else if (!strcmp(part, "elm.swallow.third"))
          {
             img_res = (Image2 *)(eina_list_nth(res->common.uses___, 2));
          }
        else if (!strcmp(part, "elm.swallow.fouth"))
          {
             if (count > 4) goto empty_content;
             img_res = (Image2 *)(eina_list_nth(res->common.uses___, 3));
          }
        if (!img_res) goto empty_content;
#ifndef _WIN32
        elm_thumb_file_set(image_obj, img_res->source, NULL);
#else
        elm_image_file_set(image_obj, img_res->source, NULL);
#endif /* _WIN32 */
        elm_object_style_set(image_obj, "noframe");
        evas_object_show(image_obj);
     }
   return image_obj;

empty_content:
   evas_object_del(image_obj);
   return NULL;
}

ITEM_SEARCH_FUNC(gengrid, ELM_GENGRID_ITEM_SCROLLTO_MIDDLE, NULL)

static void
_on_images_search_entry_changed_cb(void *data,
                                   Evas_Object *obj __UNUSED__,
                                   void *event_info __UNUSED__)
{
   Helper_Data *helper_data = data;

   assert(helper_data != NULL);

   _gengrid_item_search(helper_data->gengrid, &(helper_data->image_search_data),
                        helper_data->image_search_data.last_item_found);
}

static void
_search_next_gengrid_item_cb(void *data,
                             Evas_Object *obj __UNUSED__,
                             void *event_info __UNUSED__)
{
   Helper_Data *helper_data = data;
   Elm_Object_Item *start_from = NULL;

   assert(helper_data != NULL);

   if (helper_data->image_search_data.last_item_found)
     {
        start_from =
           elm_gengrid_item_next_get(helper_data->image_search_data.last_item_found);
     }

   _gengrid_item_search(helper_data->gengrid, &(helper_data->image_search_data),
                        start_from);
}

static void
_image_manager_add_job(void *data __UNUSED__)
{
   image_manager_add();
}

static void
_btn_image_manager_cb(void *data __UNUSED__,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{
   _helper_dismiss(NULL, helper, NULL, NULL);
   ecore_job_add(_image_manager_add_job, NULL);
}

void
popup_gengrid_image_helper(const char *title, Evas_Object *follow_up,
                           Helper_Done_Cb func, void *data,
                           Eina_Bool multi)
{
   Evas_Object *entry, *icon, *button;
   Helper_Data *helper_data = (Helper_Data *)mem_calloc(1, sizeof(Helper_Data));

   dismiss_func = func;
   func_data = data;

   helper_data->follow_up = follow_up;

   evas_object_del(helper);
   helper = elm_layout_add(ap.win);
   current = POPUP_GENGRID_IMAGE_HELPER;
   elm_layout_theme_set(helper, "layout", "popup", title ? "hint_title" : "hint");
   evas_object_data_set(helper, "STRUCT", helper_data);
   elm_layout_signal_callback_add(helper, "hint,dismiss", "eflete", _helper_dismiss, follow_up);

   fs = elm_layout_add(helper);
   elm_layout_theme_set(fs, "layout", "image_manager", "usage_info");
   evas_object_size_hint_weight_set(fs, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(fs, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(fs);

   helper_data->gengrid = elm_gengrid_add(fs);
   if (multi)
     {
        elm_gengrid_multi_select_set(helper_data->gengrid, true);

        BUTTON_ADD(fs, helper_data->button, _("Ok"))
        elm_object_part_content_set(helper, "elm.swallow.ok", helper_data->button);
        evas_object_smart_callback_add(helper_data->button, signals.elm.button.clicked, _done_image, helper_data);
        evas_object_show(helper_data->button);
     }
   else
     {
        elm_gengrid_multi_select_set(helper_data->gengrid, false);
        elm_gengrid_multi_select_mode_set(helper_data->gengrid,
                                          ELM_OBJECT_MULTI_SELECT_MODE_WITH_CONTROL);
        evas_object_smart_callback_add(helper_data->gengrid, signals.elm.gengrid.clicked_double, _done_image, helper_data);
     }
   elm_gengrid_item_size_set(helper_data->gengrid, ITEM_WIDTH, ITEM_HEIGHT);
   elm_gengrid_align_set(helper_data->gengrid, 0.0, 0.0);
   elm_scroller_policy_set(helper_data->gengrid, ELM_SCROLLER_POLICY_OFF,
                           ELM_SCROLLER_POLICY_OFF);
   elm_object_part_content_set(fs, "eflete.swallow.genlist", helper_data->gengrid);

   if (!gic)
     {
        gic = elm_gengrid_item_class_new();
        gic->item_style = "default";
        gic->func.text_get = _grid_label_get;
        gic->func.content_get = _grid_content_get;
        gic->func.del = _grid_del;
     }

   if (!gic_set)
     {
        gic_set = elm_gengrid_item_class_new();
        gic_set->item_style = "image_set";
        gic_set->func.text_get = _grid_label_get;
        gic_set->func.content_get = _grid_image_set_content_get;
        gic_set->func.del = _grid_del;
     }


   _image_gengrid_init(helper_data);

   ENTRY_ADD(fs, entry, true);
   elm_object_part_text_set(entry, "guide", _("Search"));
#if !HAVE_TIZEN
   ICON_STANDARD_ADD(entry, icon, true, "search");
   elm_object_part_content_set(entry, "elm.swallow.end", icon);
#else
   elm_object_style_set(entry, "search");
#endif

   elm_object_part_content_set(fs, "eflete.swallow.search_line", entry);
   evas_object_smart_callback_add(entry, signals.elm.entry.changed,
                                  _on_images_search_entry_changed_cb, helper_data);
   evas_object_smart_callback_add(entry, signals.elm.entry.activated,
                                  _search_next_gengrid_item_cb, helper_data);
   helper_data->image_search_data.search_entry = entry;
   helper_data->image_search_data.last_item_found = NULL;

   BUTTON_ADD(fs, button, NULL);
   ICON_STANDARD_ADD(button, icon, true, "image2");
   elm_object_content_set(button, icon);
   evas_object_smart_callback_add(button, signals.elm.button.clicked, _btn_image_manager_cb, NULL);
   elm_object_part_content_set(fs, "eflete.swallow.button", button);
   elm_layout_signal_emit(fs, "button,show", "eflete");

   /* small hack, hide not necessary button */
   evas_object_hide(elm_layout_content_unset(fs, "elm.swallow.cancel"));
   evas_object_size_hint_min_set(helper, GENGRID_W, GENGRID_H);
   evas_object_resize(helper, GENGRID_W, GENGRID_H);

   if (title) elm_object_text_set(helper, title);
   elm_layout_content_set(helper, "elm.swallow.content", fs);
   evas_object_size_hint_weight_set(fs, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(fs, EVAS_HINT_FILL, EVAS_HINT_FILL);
   if (follow_up)
     {
        _helper_property_follow(NULL, NULL, follow_up, NULL);
        evas_object_event_callback_add(follow_up, EVAS_CALLBACK_RESIZE, _helper_property_follow, NULL);
        evas_object_event_callback_add(follow_up, EVAS_CALLBACK_MOVE, _helper_property_follow, NULL);
     }
   else
     {
        _helper_win_follow(NULL, NULL, NULL, NULL);
        evas_object_event_callback_add(ap.win, EVAS_CALLBACK_RESIZE, _helper_win_follow, NULL);
     }

   shortcuts_object_push(helper);
   evas_object_show(helper);
}

void
popup_gengrid_helper_item_select(const char *item_title)
{
   Elm_Object_Item *item;
   Helper_Data *helper_data = evas_object_data_get(helper, "STRUCT");

   assert(helper_data != NULL);

   item = elm_gengrid_search_by_text_item_get(helper_data->gengrid, NULL, "elm.text", item_title, 0);
   elm_gengrid_item_selected_set(item, true);
}

void
popup_colorselector_helper(Evas_Object *follow_up,
                           Helper_Done_Cb func, Evas_Smart_Cb func_change,
                           void *data, int r, int g, int b, int a)
{
   Helper_Data *helper_data = (Helper_Data *)mem_calloc(1, sizeof(Helper_Data));

   dismiss_func = func;
   func_data = data;

   helper_data->follow_up = follow_up;

   evas_object_del(helper);
   helper = elm_layout_add(ap.win);

   current = POPUP_COLORSELECTOR_HELPER;
#if HAVE_TIZEN
   elm_layout_theme_set(helper, "layout", "popup", "colorselector");
#else
   elm_layout_theme_set(helper, "layout", "popup", "hint");
#endif

   evas_object_data_set(helper, "STRUCT", helper_data);
   elm_layout_signal_callback_add(helper, "hint,dismiss", "eflete", _helper_colorclass_dismiss, helper_data);

   fs = elm_colorselector_add(helper);
   elm_colorselector_mode_set(fs, ELM_COLORSELECTOR_ALL);
   elm_colorselector_color_set(fs, r, g, b, a);
   evas_object_size_hint_weight_set(fs, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(fs, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(fs);

#if HAVE_TIZEN
   /* Dirty hack for set size to color picker */
   Evas_Object *picker_base = elm_layout_content_get(fs, "elm.picker");
   Evas_Object *picker_box = elm_layout_content_get(picker_base, "elm.swallow.picker");
   Evas_Object *picker_layout = eina_list_data_get(elm_box_children_get(picker_box));
   Evas_Object *picker_image = edje_object_part_swallow_get(picker_layout, "elm.picker");

   evas_object_size_hint_aspect_set(picker_image, EVAS_ASPECT_CONTROL_NONE, 0, 0);
   evas_object_size_hint_min_set(picker_image, 96, 48);

   elm_colorselector_palette_clear(fs);
   elm_colorselector_palette_name_set(fs, "eflete_tizen");
   elm_colorselector_palette_color_add(fs, 229, 3, 3, 255);
   elm_colorselector_palette_color_add(fs, 20, 218, 20, 255);
   elm_colorselector_palette_color_add(fs, 25, 46, 201, 255);
   elm_colorselector_palette_color_add(fs, 236, 196, 9, 255);
   elm_colorselector_palette_color_add(fs, 237, 10, 234, 255);
   elm_colorselector_palette_color_add(fs, 26, 234, 217, 255);
   elm_colorselector_palette_color_add(fs, 0, 0, 0, 255);
   elm_colorselector_palette_color_add(fs, 255, 255, 255, 255);
   elm_colorselector_palette_color_add(fs, 200, 200, 200, 255);
#endif
   evas_object_smart_callback_add(fs, signals.elm.colorselector.changed, func_change, data);
   evas_object_smart_callback_add(fs, signals.elm.colorselector.color_item_selected, func_change, data);
   evas_object_smart_callback_add(fs, signals.elm.colorselector.color_item_longpressed, func_change, data);

   /* small hack, hide not necessary button */
   evas_object_hide(elm_layout_content_unset(fs, "elm.swallow.cancel"));
   evas_object_size_hint_min_set(helper, COLOR_W, COLOR_H);
   evas_object_resize(helper, COLOR_W, COLOR_H);

#if !HAVE_TIZEN
   BUTTON_ADD(fs, helper_data->button, _("Ok"))
   elm_object_part_content_set(helper, "elm.swallow.ok", helper_data->button);
   evas_object_smart_callback_add(helper_data->button, signals.elm.button.clicked, _colorclass_done, helper_data);
   evas_object_show(helper_data->button);
#endif

   elm_layout_content_set(helper, "elm.swallow.content", fs);
   evas_object_size_hint_weight_set(fs, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(fs, EVAS_HINT_FILL, EVAS_HINT_FILL);
   if (follow_up)
     {
        _helper_property_color_follow(NULL, NULL, follow_up, NULL);
        evas_object_event_callback_add(follow_up, EVAS_CALLBACK_MOVE, _helper_property_color_follow, NULL);
     }
   else
     {
        _helper_win_follow(NULL, NULL, NULL, NULL);
        evas_object_event_callback_add(ap.win, EVAS_CALLBACK_RESIZE, _helper_win_follow, NULL);
     }

   shortcuts_object_push(helper);
   evas_object_show(helper);
}

void
popup_log_message_helper(const char *msg)
{
   Evas_Object *box, *en, *lab;

   helper = elm_layout_add(ap.win);
   current = POPUP_LOG_MESSAGE_HELPER;
   elm_layout_theme_set(helper, "layout", "popup", "hint");
   elm_layout_signal_callback_add(helper, "hint,dismiss", "eflete", _helper_dismiss, ap.win);
   evas_object_size_hint_min_set(helper, FS_W, FS_H);
   evas_object_resize(helper, FS_W, FS_H);

   BOX_ADD(helper, box, false, false)
   elm_box_padding_set(box, 0, 6);
   LABEL_ADD(helper, lab, _("<font_size=14>Import edc-file error"))
   evas_object_show(lab);
   ENTRY_ADD(box, en, false)
   elm_entry_editable_set(en, false);
   elm_box_pack_end(box, lab);
   elm_box_pack_end(box, en);
   elm_layout_content_set(helper, NULL, box);

   elm_entry_entry_set(en, msg);

   _helper_win_follow(NULL, NULL, NULL, NULL);
   evas_object_event_callback_add(ap.win, EVAS_CALLBACK_RESIZE, _helper_win_follow, NULL);

   shortcuts_object_push(helper);
   evas_object_show(helper);
}

void
popup_active_helper_close(void *data,
                          Evas *e __UNUSED__,
                          Evas_Object *obj __UNUSED__,
                          void *event_info __UNUSED__)
{
   Popup_Current request = ((Popup_Current)(uintptr_t)data);
   if (request == current)
     elm_layout_signal_emit(helper, "hint,dismiss", "eflete");
}

#undef FS_W
#undef FS_H

#undef GENGRID_W
#undef GENGRID_H
