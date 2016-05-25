/*
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
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
#include "project_manager.h"
#include "modal_window.h"

#define ITEM_WIDTH 100
#define ITEM_HEIGHT 115

typedef struct _Image_Manager Image_Manager;
typedef struct _Search_Data Search_Data;
typedef struct _Content_Init_Data Content_Init_Data;

struct _Content_Init_Data
{
   Image_Manager *image_manager;
   Image_Item *item_data;
   Evas_Object *image_obj;
};

struct _Search_Data
{
   Evas_Object *search_entry;
   Elm_Object_Item *last_item_found;
};

struct _Image_Manager
{
   Evas_Object *win;
   Evas_Object *panes;
   Evas_Object *property_panes;
   Evas_Object *entry_prev;
   Evas_Object *image;
   Evas_Object *del_button;
   Evas_Object *gengrid;
   Evas_Object *layout;
#ifdef HAVE_TIZEN
   struct {
        Evas_Object *black;
        Evas_Object *tile;
        Evas_Object *white;
   } bg_switcher;
#endif
   Search_Data image_search_data;
};

static Image_Manager mng;
static Elm_Gengrid_Item_Class *gic = NULL;

static char *
_grid_label_get(void *data,
                Evas_Object *obj __UNUSED__,
                const char  *part __UNUSED__)
{
   const Image_Item *it = data;
   return strdup(it->image_name);
}

static void
_image_manager_image_setup(Evas_Object *image,
                           const Image_Item *it)
{
   assert(image != NULL);
   assert(it != NULL);

   if (it->comp_type == EDJE_EDIT_IMAGE_COMP_USER)
     {
        if (ecore_file_exists(it->source))
          elm_thumb_file_set(image, it->source, NULL);
        else
          elm_thumb_file_set(image, EFLETE_THEME, "elm/image/icon/attention");
     }
   else
     {
        elm_thumb_file_set(image, it->source, NULL);
     }
}

static inline Evas_Object *
_image_manager_image_create(Evas_Object *parent,
                            const Image_Item *it)
{
   assert(parent != NULL);
   assert(it != NULL);

   Evas_Object *image = elm_image_add(parent);
   _image_manager_image_setup(image, it);
   return image;
}

/* icon fetching callback */
static Evas_Object *
_grid_content_get(void *data,
                  Evas_Object *obj,
                  const char  *part)
{
   Image_Item *it = data;
   Evas_Object *image_obj = NULL;
   Evas_Object *grid = (Evas_Object *)obj;
   Resource *res;

   assert(it != NULL);
   assert(grid != NULL);

   if (!strcmp(part, "elm.swallow.icon"))
     {
        image_obj = elm_thumb_add(grid);
        elm_object_style_set(image_obj, "noframe");
        _image_manager_image_setup(image_obj, it);
        evas_object_show(image_obj);
     }
   else if (!strcmp(part, "elm.swallow.end"))
     {
        it->is_used = true;
        res = (Resource *) pm_resource_get(ap.project->images, it->image_name);
        if (eina_list_count(res->used_in) == 0)
          {
             image_obj = elm_icon_add(grid);
             elm_image_file_set(image_obj, EFLETE_THEME, "elm/image/icon/attention");
             evas_object_show(image_obj);
             it->is_used = false;
          }
     }

   return image_obj;
}

/* deletion callback */
static void
_grid_del(void *data,
          Evas_Object *obj __UNUSED__)
{
   Image_Item *it = data;

   assert(it != NULL);

   eina_stringshare_del(it->image_name);
   eina_stringshare_del(it->source);
   free(it);
}

static void
_image_info_setup(const Image_Item* it)
{
   Evas_Object *image;

   assert(it != NULL);

   image = _image_manager_image_create(mng.layout, it);
   evas_object_show(image);

   evas_object_data_set(image, "image_name", it->image_name);
}

/* item selection change callback */
static void
_grid_sel_cb(void *data __UNUSED__,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   Image_Item *item = NULL;
   Eina_List *l;
   Eina_List *sel_list;
   Elm_Object_Item *grid_item = NULL;

   sel_list = (Eina_List *)elm_gengrid_selected_items_get(mng.gengrid);
   int selected_images_count = eina_list_count(sel_list);

   /* if no selected images, disable delete button */
   if (selected_images_count != 0)
     elm_object_disabled_set(mng.del_button, true);

   if (selected_images_count == 1)
     {
        item = elm_object_item_data_get(eina_list_data_get(sel_list));
        _image_info_setup(item);
        /* if selected image is not used, we can delete it */
        if (!item->is_used)
          elm_object_disabled_set(mng.del_button, false);

         /* apply picture */
         if (item->comp_type == EDJE_EDIT_IMAGE_COMP_USER)
           {
              if (ecore_file_exists(item->source))
                elm_image_file_set(mng.image, item->source, NULL);
              else
                elm_image_file_set(mng.image, EFLETE_THEME, "elm/image/icon/attention");
           }
         else
           {
              elm_image_file_set(mng.image, item->source, NULL);
           }
         evas_object_image_smooth_scale_set(mng.image, false);
     }
   else
     {
        /* if any of those is not used, set it able to delete */
        EINA_LIST_FOREACH(sel_list, l, grid_item)
          {
             item = elm_object_item_data_get(grid_item);
             if (!item->is_used)
               {
                  elm_object_disabled_set(mng.del_button, false);
                  break;
               }
          }
        elm_image_file_set(mng.image, EFLETE_IMG_PATH EFLETE_DUMMY_IMAGE_NAME, NULL);
     }
   evas_object_smart_callback_call(ap.win, SIGNAL_IMAGE_SELECTED, item);
}

static inline Image_Item *
_image_manager_gengrid_item_data_create(Evas_Object *edje_edit_obj,
                                       External_Resource *res)
{
   Evas_Object *img;

   assert(edje_edit_obj != NULL);
   assert(res != NULL);

   Image_Item *it = (Image_Item *)mem_malloc(sizeof(Image_Item));
   it->image_name = eina_stringshare_add(res->name);
   it->id = edje_edit_image_id_get(edje_edit_obj, it->image_name);
   it->comp_type = edje_edit_image_compression_type_get(edje_edit_obj,
                                                        it->image_name);
   it->quality = edje_edit_image_compression_rate_get(edje_edit_obj,
                                                      it->image_name);
   it->source = eina_stringshare_add(res->source);

   img = _image_manager_image_create(ap.project->global_object, it);
   elm_image_object_size_get(img, &it->width, &it->height);
   evas_object_del(img);

   return it;
}

static Eina_Bool
_on_image_done(void *data __UNUSED__,
               Evas_Object *obj __UNUSED__,
               void *event_info)
{
   Image_Item *it = NULL;
   const Eina_List *images, *l;
   const char *selected;
   Uns_List *image = NULL;
   External_Resource *res;
   const char *file_name;
   Evas_Object *img;

   images = (Eina_List *)event_info;

   EINA_LIST_FOREACH(images, l, selected)
     {
        if (!ecore_file_exists(selected))
          {
             ERR(_("File not exist"));
             continue;
          }
        if (ecore_file_is_dir(selected))
          {
             ERR(_("Unable to add folder"))
             continue;
          }
        file_name = ecore_file_file_get(selected);

        res = mem_calloc(1, sizeof(External_Resource));
        res->name = eina_stringshare_add(file_name);
        res->source = eina_stringshare_printf("%s/images/%s", ap.project->develop_path, file_name);

        if (!ecore_file_exists(res->source))
          {
             ecore_file_cp(selected, res->source);

             ap.project->images = eina_list_sorted_insert(ap.project->images, (Eina_Compare_Cb) resource_cmp, res);
          }
        else
          {
             ERR(_("File exist"));
             free(image);
             free(res);
             continue;
          }
        edje_edit_image_add(ap.project->global_object, selected);
        CRIT_ON_FAIL(editor_save(ap.project->global_object));
        TODO("Remove this line once edje_edit_image_add would be added into Editor Modulei and saving would work properly")
        ap.project->changed = true;

        it = (Image_Item *)mem_malloc(sizeof(Image_Item));
        it->image_name = eina_stringshare_add(file_name);
        it->id = edje_edit_image_id_get(ap.project->global_object, it->image_name);
        elm_gengrid_item_append(mng.gengrid, gic, it, _grid_sel_cb, NULL);
        it->comp_type = edje_edit_image_compression_type_get(ap.project->global_object,
                                                             it->image_name);
        it->quality = edje_edit_image_compression_rate_get(ap.project->global_object,
                                                           it->image_name);

        it->source = eina_stringshare_add(res->source);
        img = _image_manager_image_create(ap.project->global_object, it);
        elm_image_object_size_get(img, &it->width, &it->height);
        evas_object_del(img);
     }

   return true;
}

static void
_image_add_cb(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   popup_fileselector_image_helper(NULL,
                                   NULL,
                                   NULL,
                                   _on_image_done,
                                   data,
                                   true,
                                   false);
}

static void
_image_del_cb(void *data __UNUSED__,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   Elm_Object_Item *grid_item = NULL;
   Image_Item *it = NULL;
   Eina_List *grid_list, *l, *l2;
   External_Resource *res;

   assert(mng.gengrid != NULL);

   grid_list = (Eina_List *)elm_gengrid_selected_items_get(mng.gengrid);

   assert(grid_list != NULL);

   EINA_LIST_FOREACH_SAFE(grid_list, l, l2, grid_item)
     {
        it = elm_object_item_data_get(grid_item);
        res = pm_resource_get(ap.project->images, it->image_name);

        if (!res->used_in)
          {
             ecore_file_unlink(res->source);
             elm_object_item_del(grid_item);
             edje_edit_image_del(ap.project->global_object, it->image_name);
             ap.project->images = pm_resource_del(ap.project->images, res);
          }
        else
          elm_gengrid_item_selected_set(grid_item, false);
     }

   evas_object_smart_callback_call(ap.win, SIGNAL_IMAGE_SELECTED, NULL);
   CRIT_ON_FAIL(editor_save(ap.project->global_object));
   TODO("Remove this line once edje_edit_image_del would be added into Editor Modulei and saving would work properly")
   ap.project->changed = true;
   elm_object_disabled_set(mng.del_button, true);
}

ITEM_SEARCH_FUNC(gengrid, ELM_GENGRID_ITEM_SCROLLTO_MIDDLE, NULL)

static void
_entry_changed_cb(void *data __UNUSED__,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   _gengrid_item_search(mng.gengrid, &(mng.image_search_data),
                        mng.image_search_data.last_item_found);
}

static void
_find_next_cb(void *data __UNUSED__,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   Elm_Object_Item *start_from = NULL;

   if (mng.image_search_data.last_item_found)
     start_from = elm_gengrid_item_next_get(mng.image_search_data.last_item_found);

   _gengrid_item_search(mng.gengrid, &(mng.image_search_data), start_from);
}

static inline Evas_Object *
_image_manager_search_field_create(Evas_Object *parent)
{
   Evas_Object *entry, *icon;

   assert(parent != NULL);

   ENTRY_ADD(parent, entry, true);
   elm_object_part_text_set(entry, "guide", _("Search"));
   ICON_STANDARD_ADD(entry, icon, true, "search");
   elm_object_part_content_set(entry, "elm.swallow.end", icon);
   return entry;
}

static Eina_Bool
_image_manager_init(void)
{
   Eina_List *l = NULL;
   Image_Item *it = NULL;
   Eina_List *images = NULL;
   int counter = 0;
   External_Resource *res;

   images = ap.project->images;

   if (images)
     {
        EINA_LIST_FOREACH(images, l, res)
           {
              counter++;
              if (!res->name)
                {
                   ERR("name not found for image #%d",counter);
                   continue;
                }

              /* skip dummy image */
              if (!strcmp(res->name, EFLETE_DUMMY_IMAGE_NAME)) continue;

              it = _image_manager_gengrid_item_data_create(ap.project->global_object, res);
              elm_gengrid_item_append(mng.gengrid, gic, it, _grid_sel_cb, NULL);
           }
         elm_gengrid_item_bring_in(elm_gengrid_first_item_get(mng.gengrid), ELM_GENGRID_ITEM_SCROLLTO_TOP);
     }

   evas_object_smart_callback_call(ap.win, SIGNAL_IMAGE_SELECTED, NULL);
   return true;
}

static void
_mw_cancel_cb(void *data __UNUSED__,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   Evas_Object *content;

   /* unset and hide the image manager content */
   content = elm_object_content_unset(mng.win);
   evas_object_hide(content);
}

static void
_mw_done_cb(void *data __UNUSED__,
            Evas_Object *obj __UNUSED__,
            void *event_info __UNUSED__)
{
   Evas_Object *content;

   /* unset and hide the image manager content */
   content = elm_object_content_unset(mng.win);
   evas_object_hide(content);
}

static void
_project_closed_cb(void *data __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   elm_gengrid_clear(mng.gengrid);
}

#ifdef HAVE_TIZEN
static void
_bg_cb(void *data,
       Evas_Object *obj,
       void *event_info __UNUSED__)
{
   Bg_Preview bg_mode;
   const char *signal = NULL;
   Evas_Object *entry_prev = data;

   Evas_Object *bg = elm_object_part_content_get(entry_prev, "elm.swallow.background");
   bg_mode = elm_radio_value_get(obj);
   switch (bg_mode)
     {
      case BG_PREVIEW_WHITE:
         signal = "elm,bg,white";
         break;
      case BG_PREVIEW_TILE:
         signal = "elm,bg,tile";
         break;
      case BG_PREVIEW_BLACK:
         signal = "elm,bg,black";
         break;
     }

   elm_layout_signal_emit(bg, signal, "eflete");
}

static Evas_Object *
_radio_switcher_add(Evas_Object *entry_prev,
                    const char *style,
                    Evas_Smart_Cb func,
                    int state_value,
                    Evas_Object *group)
{
   Evas_Object *radio;

   radio = elm_radio_add(ap.win);
   evas_object_show(radio);
   elm_object_style_set(radio, style);
   elm_radio_state_value_set(radio, state_value);
   evas_object_smart_callback_add(radio, "changed", func, entry_prev);
   elm_radio_group_add(radio, group);

   return radio;
}


static Evas_Object*
_add_box_bg(void)
{
   Evas_Object *box_bg;

   BOX_ADD(ap.win, box_bg, true, false);
   elm_box_padding_set(box_bg, 6, 6);
   elm_box_align_set(box_bg, 1.0, 0.5);

   /* add to toolbar bg switcher */
   mng.bg_switcher.white = _radio_switcher_add(mng.entry_prev, "bg_white", _bg_cb, BG_PREVIEW_WHITE, NULL);
   elm_box_pack_end(box_bg, mng.bg_switcher.white);

   mng.bg_switcher.tile = _radio_switcher_add(mng.entry_prev, "bg_tile", _bg_cb, BG_PREVIEW_TILE, mng.bg_switcher.white);
   elm_box_pack_end(box_bg, mng.bg_switcher.tile);

   mng.bg_switcher.black = _radio_switcher_add(mng.entry_prev, "bg_black", _bg_cb, BG_PREVIEW_BLACK, mng.bg_switcher.white);
   elm_box_pack_end(box_bg, mng.bg_switcher.black);

   elm_radio_value_set(mng.bg_switcher.white, BG_PREVIEW_TILE);

   return box_bg;
}
#endif

Evas_Object *
image_manager_add(void)
{
   Evas_Object *button, *ic, *search_entry = NULL;

   assert(ap.project != NULL);

   mng.win = mw_add();
   mw_title_set(mng.win, _("Image manager"));
   evas_object_smart_callback_add(mng.win, "cancel", _mw_cancel_cb, NULL);
   evas_object_smart_callback_add(mng.win, "done", _mw_done_cb, NULL);
   ic = elm_icon_add(mng.win);
   elm_icon_standard_set(ic, "image2");
   mw_icon_set(mng.win, ic);

   if (mng.layout) goto done;

#ifdef HAVE_TIZEN
   Evas_Object *bg, *box_bg;

   mng.layout = elm_layout_add(mng.win);
   elm_layout_theme_set(mng.layout, "layout", "manager", "internal");
   elm_object_part_text_set(mng.layout, "elm.text", _("Preview"));
   elm_layout_text_set(mng.layout, "elm.subtext", _("Image Gallery"));
   mng.panes = elm_panes_add(mng.win);
   elm_object_part_content_set(mng.panes, "left", mng.layout);
   elm_object_part_content_set(mng.panes, "right", ap.property.image_manager);

   mng.entry_prev = elm_layout_add(mng.layout);
   elm_layout_theme_set(mng.entry_prev, "layout", "manager", "preview");
   evas_object_show(mng.entry_prev);

   mng.image = elm_image_add(mng.panes);
   evas_object_show(mng.image);

   bg = elm_layout_add(mng.layout);
   elm_layout_theme_set(bg, "layout", "workspace", "bg");
   elm_object_part_content_set(mng.entry_prev, "elm.swallow.background", bg);
   elm_object_part_content_set(mng.entry_prev, "elm.swallow.entry", mng.image);
   elm_object_signal_emit(mng.entry_prev, "entry,show", "eflete");
   elm_object_part_content_set(mng.layout, "elm.swallow.preview", mng.entry_prev);

   box_bg = _add_box_bg();
   elm_object_part_content_set(mng.layout, "elm.swallow.menu", box_bg);
#else
   mng.layout = elm_layout_add(mng.win);
   elm_layout_theme_set(mng.layout, "layout", "image_manager", "default");
   mng.panes = elm_panes_add(mng.win);
   elm_object_part_content_set(mng.panes, "left", mng.layout);
   mng.property_panes = elm_panes_add(mng.win);
   elm_panes_horizontal_set(mng.property_panes, true);
   elm_object_part_content_set(mng.panes, "right", mng.property_panes);
   mng.image = elm_image_add(mng.panes);
   evas_object_show(mng.image);
   elm_object_part_content_set(mng.property_panes, "left", mng.image);
   elm_object_part_content_set(mng.property_panes, "right", ap.property.image_manager);
#endif

   if (!gic)
     {
        gic = elm_gengrid_item_class_new();
        gic->item_style = "default";
        gic->func.text_get = _grid_label_get;
        gic->func.content_get = _grid_content_get;
        gic->func.del = _grid_del;
     }

   mng.gengrid = elm_gengrid_add(mng.layout);
   elm_object_part_content_set(mng.layout, "elm.swallow.list", mng.gengrid);
   elm_gengrid_item_size_set(mng.gengrid, ITEM_WIDTH, ITEM_HEIGHT);
   elm_gengrid_align_set(mng.gengrid, 0.0, 0.0);
   elm_scroller_policy_set(mng.gengrid, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_AUTO);
   elm_gengrid_multi_select_set(mng.gengrid, true);
   elm_gengrid_multi_select_mode_set(mng.gengrid, ELM_OBJECT_MULTI_SELECT_MODE_WITH_CONTROL);
   elm_gengrid_select_mode_set(mng.gengrid, ELM_OBJECT_SELECT_MODE_ALWAYS);
   evas_object_smart_callback_add(mng.gengrid, "unselected", _grid_sel_cb, NULL);
   evas_object_show(mng.gengrid);

   button = elm_button_add(mng.layout);
   elm_object_style_set(button, "plus");
   evas_object_smart_callback_add(button, "clicked", _image_add_cb, NULL);
   elm_object_part_content_set(mng.layout, "elm.swallow.btn_add", button);

   mng.del_button = elm_button_add(mng.layout);
   elm_object_style_set(mng.del_button, "minus");
   evas_object_smart_callback_add(mng.del_button, "clicked", _image_del_cb, NULL);
   elm_object_part_content_set(mng.layout, "elm.swallow.btn_del", mng.del_button);
   elm_object_disabled_set(mng.del_button, true);

   // Search line add
   search_entry = _image_manager_search_field_create(mng.layout);
   elm_object_part_content_set(mng.layout, "elm.swallow.search", search_entry);
   evas_object_smart_callback_add(search_entry, "changed", _entry_changed_cb, NULL);
   evas_object_smart_callback_add(search_entry, "activated", _find_next_cb, NULL);
   mng.image_search_data.search_entry = search_entry;
   mng.image_search_data.last_item_found = NULL;

   evas_object_smart_callback_add(ap.win, SIGNAL_PROJECT_CLOSED, _project_closed_cb, NULL);

done:
   /* if genlist is empty try to fill it. This happens if the managers called
    * first time or projectwas reopened */
   if (!elm_gengrid_realized_items_get(mng.gengrid))
     _image_manager_init();

   elm_object_content_set(mng.win, mng.panes);
   evas_object_show(mng.win);
   elm_object_focus_set(search_entry, true);
   return mng.win;
}
