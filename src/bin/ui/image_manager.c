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

#define ITEM_WIDTH 100
#define ITEM_HEIGHT 115
#define IMG_MANAGER_KEY "image_manager_key"

typedef struct _Image_Manager Image_Manager;
typedef struct _Search_Data Search_Data;
typedef struct _Item Item;
typedef struct _Content_Init_Data Content_Init_Data;

struct _Item
{
   int id;
   const char* image_name;
   const char* source;
   Edje_Edit_Image_Comp comp_type;
   Eina_Bool is_used;
};

struct _Content_Init_Data
{
   Image_Manager *image_manager;
   Item *item_data;
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
   Evas_Object *gengrid;
   Evas_Object *layout;
   Evas_Object *del_button;
   Search_Data image_search_data;
};

static Elm_Gengrid_Item_Class *gic = NULL;

static void _image_manager_del(Image_Manager *img_mng);

static char *
_grid_label_get(void *data,
                Evas_Object *obj __UNUSED__,
                const char  *part __UNUSED__)
{
   const Item *it = data;
   return strdup(it->image_name);
}

static void
_on_image_manager_del(void * data,
                     Evas *e __UNUSED__,
                     Evas_Object *obj __UNUSED__,
                     void *event_info __UNUSED__)
{
   Image_Manager *img_mng = (Image_Manager *)data;

   assert(img_mng != NULL);

   _image_manager_del(img_mng);
}

static void
_image_manager_del(Image_Manager *img_mng)
{
   assert(img_mng != NULL);

   ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_MAIN, false);

   evas_object_event_callback_del(img_mng->layout, EVAS_CALLBACK_DEL, _on_image_manager_del);

   elm_gengrid_item_class_free(gic);
   gic = NULL;
   evas_object_data_del(img_mng->layout, IMG_MANAGER_KEY);
   evas_object_data_del(img_mng->gengrid, IMG_MANAGER_KEY);
   //evas_object_del(img_mng->gengrid);
   free(img_mng);
}

static void
_image_manager_image_setup(Evas_Object *image,
                          const Item *it)
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
                           const Item *it)
{
   assert(parent != NULL);
   assert(it != NULL);

   Evas_Object *image = elm_image_add(parent);
   _image_manager_image_setup(image, it);
   return image;
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
   Resource *res;

   assert(it != NULL);
   assert(grid != NULL);

   if (!strcmp(part, "elm.swallow.icon"))
     {
        image_obj = elm_thumb_add(grid);
        _image_manager_image_setup(image_obj, it);
        elm_thumb_reload(image_obj);
        evas_object_show(image_obj);
   /* functions for deferred creation of gengrid icons */
   //ecore_job_add(_image_content_setup, image_init_data);
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
#undef MAX_ICON_SIZE

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

static void
_image_info_setup(Image_Manager *img_mng,
                  const Item* it)
{
   Evas_Object *image;

   assert(img_mng != NULL);
   assert(it != NULL);

   image = _image_manager_image_create(img_mng->layout, it);
   evas_object_image_smooth_scale_set(image, false);
   evas_object_show(image);

   evas_object_data_set(image, "image_name", it->image_name);
   evas_object_smart_callback_call(ap.win, SIGNAL_IMAGE_SELECTED, image);
}

/* item selection change callback */
static void
_grid_sel(void *data,
          Evas_Object *obj __UNUSED__,
          void *event_info __UNUSED__)
{
   Image_Manager *img_mng = (Image_Manager *)data;
   Item *item = NULL;
   Eina_List *l;
   Eina_List *sel_list;
   Elm_Object_Item *grid_item = NULL;

   assert(img_mng != NULL);

   sel_list = (Eina_List *)elm_gengrid_selected_items_get(img_mng->gengrid);
   int selected_images_count = eina_list_count(sel_list);

   /* if no selected images, disable delete button */
   if (selected_images_count != 0)
     elm_object_disabled_set(img_mng->del_button, true);

   if (selected_images_count == 1)
     {
        item = elm_object_item_data_get(eina_list_data_get(sel_list));
        _image_info_setup(img_mng, item);
        /* if selected image is not used, we can delete it */
        if (!item->is_used)
          elm_object_disabled_set(img_mng->del_button, false);
     }
   else
     {
        /* if any of those is not used, set it able to delete */
        EINA_LIST_FOREACH(sel_list, l, grid_item)
          {
             item = elm_object_item_data_get(grid_item);
             if (!item->is_used)
               {
                  elm_object_disabled_set(img_mng->del_button, false);
                  break;
               }
          }
        evas_object_smart_callback_call(ap.win, SIGNAL_IMAGE_SELECTED, NULL);
     }
}

static inline Item *
_image_manager_gengrid_item_data_create(Evas_Object *edje_edit_obj,
                                       External_Resource *res)
{
   assert(edje_edit_obj != NULL);
   assert(res != NULL);

   Item *it = (Item *)mem_malloc(sizeof(Item));
   it->image_name = eina_stringshare_add(res->name);
   it->id = edje_edit_image_id_get(edje_edit_obj, it->image_name);
   it->comp_type = edje_edit_image_compression_type_get(edje_edit_obj,
                                                        it->image_name);
   it->source = eina_stringshare_add(res->source);

   return it;
}

static Eina_Bool
_on_image_done(void *data,
               Evas_Object *obj __UNUSED__,
               void *event_info)
{
   Item *it = NULL;
   Elm_Object_Item *item = NULL;
   const Eina_List *images, *l;
   const char *selected;
   Uns_List *image = NULL;
   External_Resource *res;
   const char *file_name;

   Image_Manager *img_mng = (Image_Manager *)data;
   images = (Eina_List *)event_info;

   assert(img_mng != NULL);

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
        editor_save(ap.project->global_object);
        TODO("Remove this line once edje_edit_image_add would be added into Editor Modulei and saving would work properly")
        ap.project->changed = true;

        it = (Item *)mem_malloc(sizeof(Item));
        it->image_name = eina_stringshare_add(file_name);
        it->id = edje_edit_image_id_get(ap.project->global_object, it->image_name);
        item = elm_gengrid_item_append(img_mng->gengrid, gic, it, _grid_sel, img_mng);

        it->source = eina_stringshare_add(res->source);
        elm_gengrid_item_selected_set(item, true);
     }

   return true;
}

static void
_on_button_add_clicked_cb(void *data,
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
_on_button_delete_clicked_cb(void *data,
                             Evas_Object *obj __UNUSED__,
                             void *event_info __UNUSED__)
{
   Image_Manager *img_mng = (Image_Manager *)data;
   Elm_Object_Item *grid_item = NULL;
   Item *it = NULL;
   Eina_List *grid_list, *l, *l2;
   External_Resource *res;

   assert(img_mng != NULL);
   assert(img_mng->gengrid != NULL);

   grid_list = (Eina_List *)elm_gengrid_selected_items_get(img_mng->gengrid);

   assert(grid_list != NULL);

   EINA_LIST_FOREACH_SAFE(grid_list, l, l2, grid_item)
     {
        it = elm_object_item_data_get(grid_item);
        res = pm_resource_get(ap.project->images, it->image_name);

        if (!res->used_in)
          {
             elm_object_item_del(grid_item);
             edje_edit_image_del(ap.project->global_object, it->image_name);
             ap.project->images = pm_resource_del(ap.project->images, res);
          }
        else
          elm_gengrid_item_selected_set(grid_item, false);
     }

   evas_object_smart_callback_call(ap.win, SIGNAL_IMAGE_SELECTED, NULL);
   editor_save(ap.project->global_object);
   TODO("Remove this line once edje_edit_image_del would be added into Editor Modulei and saving would work properly")
   ap.project->changed = true;
}

ITEM_SEARCH_FUNC(gengrid, ELM_GENGRID_ITEM_SCROLLTO_MIDDLE, NULL)

static void
_on_images_search_entry_changed_cb(void *data,
                                   Evas_Object *obj __UNUSED__,
                                   void *event_info __UNUSED__)
{
   Image_Manager *img_mng = data;

   assert(img_mng != NULL);

   _gengrid_item_search(img_mng->gengrid, &(img_mng->image_search_data),
                        img_mng->image_search_data.last_item_found);
}

static void
_search_next_gengrid_item_cb(void *data,
                             Evas_Object *obj __UNUSED__,
                             void *event_info __UNUSED__)
{
   Image_Manager *img_mng = data;
   Elm_Object_Item *start_from = NULL;

   assert(img_mng != NULL);

   if (img_mng->image_search_data.last_item_found)
     {
        start_from =
           elm_gengrid_item_next_get(img_mng->image_search_data.last_item_found);
     }

   _gengrid_item_search(img_mng->gengrid, &(img_mng->image_search_data),
                        start_from);
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
_image_manager_init(Image_Manager *img_mng)
{
   Eina_List *l = NULL;
   Item *it = NULL;
   Eina_List *images = NULL;
   int counter = 0;
   External_Resource *res;

   assert(img_mng != NULL);

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

              it = _image_manager_gengrid_item_data_create(ap.project->global_object,
                                                          res);

              elm_gengrid_item_append(img_mng->gengrid, gic, it, _grid_sel, img_mng);
           }
         elm_gengrid_item_bring_in(elm_gengrid_first_item_get(img_mng->gengrid),
                                   ELM_GENGRID_ITEM_SCROLLTO_TOP);
     }
   elm_scroller_policy_set(img_mng->gengrid, ELM_SCROLLER_POLICY_OFF,
                           ELM_SCROLLER_POLICY_AUTO);
   evas_object_smart_calculate(img_mng->gengrid);

   return true;
}

Evas_Object *
image_manager_add()
{
   Evas_Object *button;
   Evas_Object *ic = NULL;
   Evas_Object *search_entry = NULL;

   assert(ap.project != NULL);

   Image_Manager *img_mng = (Image_Manager *)mem_calloc(1, sizeof(Image_Manager));

   img_mng->layout = elm_layout_add(main_window_get());
   elm_layout_theme_set(img_mng->layout, "layout", "image_manager", "default");

   img_mng->gengrid = elm_gengrid_add(img_mng->layout);
   elm_object_part_content_set(img_mng->layout,
                               "eflete.swallow.grid", img_mng->gengrid);
   elm_gengrid_item_size_set(img_mng->gengrid, ITEM_WIDTH, ITEM_HEIGHT);
   elm_gengrid_align_set(img_mng->gengrid, 0.0, 0.0);
   elm_scroller_policy_set(img_mng->gengrid, ELM_SCROLLER_POLICY_OFF,
                           ELM_SCROLLER_POLICY_OFF);

   elm_gengrid_multi_select_set(img_mng->gengrid, true);
   elm_gengrid_multi_select_mode_set(img_mng->gengrid,
                                     ELM_OBJECT_MULTI_SELECT_MODE_WITH_CONTROL);

   elm_gengrid_select_mode_set(img_mng->gengrid, ELM_OBJECT_SELECT_MODE_ALWAYS);
   evas_object_size_hint_weight_set(img_mng->gengrid,
                                    EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(img_mng->gengrid,
                                   EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_smart_callback_add(img_mng->gengrid, "unselected", _grid_sel,
                                  img_mng);
   evas_object_show(img_mng->gengrid);

   button = elm_button_add(img_mng->layout);
   evas_object_show(button);
   ic = elm_icon_add(button);
   elm_icon_standard_set(ic, "plus");
   elm_object_part_content_set(button, NULL, ic);
   evas_object_smart_callback_add(button, "clicked",
                                  _on_button_add_clicked_cb, img_mng);
   elm_object_part_content_set(img_mng->layout,
                               "eflete.swallow.add_btn", button);

   img_mng->del_button = elm_button_add(img_mng->layout);
   evas_object_show(img_mng->del_button);
   ic = elm_icon_add(img_mng->del_button);
   elm_icon_standard_set(ic, "minus");
   elm_object_part_content_set(img_mng->del_button, NULL, ic);
   evas_object_smart_callback_add(img_mng->del_button, "clicked",
                                  _on_button_delete_clicked_cb, img_mng);
   elm_object_part_content_set(img_mng->layout,
                               "eflete.swallow.del_btn", img_mng->del_button);
   elm_object_disabled_set(img_mng->del_button, true);

   // Search line add
   search_entry = _image_manager_search_field_create(img_mng->layout);
   elm_object_part_content_set(img_mng->layout,
                               "eflete.swallow.search_area", search_entry);
   evas_object_smart_callback_add(search_entry, "changed",
                                  _on_images_search_entry_changed_cb, img_mng);
   evas_object_smart_callback_add(search_entry, "activated",
                                  _search_next_gengrid_item_cb, img_mng);
   img_mng->image_search_data.search_entry = search_entry;
   img_mng->image_search_data.last_item_found = NULL;

   if (!gic)
     {
        gic = elm_gengrid_item_class_new();
        gic->item_style = "default";
        gic->func.text_get = _grid_label_get;
        gic->func.content_get = _grid_content_get;
        gic->func.del = _grid_del;
     }

   elm_object_focus_set(search_entry, true);
   if (!_image_manager_init(img_mng))
     {
        ERR("Filed initialize image manager");
        abort();
     }
   evas_object_data_set(img_mng->gengrid, IMG_MANAGER_KEY, img_mng);
   evas_object_data_set(img_mng->layout, IMG_MANAGER_KEY, img_mng);

   ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_MAIN, true);
   evas_object_event_callback_add(img_mng->layout, EVAS_CALLBACK_DEL, _on_image_manager_del, img_mng);

   return img_mng->layout;
}

#undef ITEM_WIDTH
#undef ITEM_HEIGHT
#undef IMG_MANAGER_KEY
