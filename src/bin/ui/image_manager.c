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
#define EO_BETA_API
#define EFL_BETA_API_SUPPORT
#define EFL_EO_API_SUPPORT


#include "main_window.h"
#include "project_manager2.h"
#include "modal_window.h"
#include "config.h"
#include "validator.h"
#include "shortcuts.h"

#ifndef HAVE_TIZEN
   #define ITEM_WIDTH 100
   #define ITEM_HEIGHT 115
#else
   #define ITEM_WIDTH 87
   #define ITEM_HEIGHT 114
#endif

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
   Evas_Object *menu;
   struct {
        Evas_Object *entry;
        Evas_Object *grid;
        Evas_Object *box;
        Resource_Name_Validator *validator;
   } image_set;
#ifdef HAVE_TIZEN
   struct {
        Evas_Object *black;
        Evas_Object *tile;
        Evas_Object *white;
   } bg_switcher;
#endif
   Search_Data image_search_data;

   Elm_Object_Item *image_set_header;
   Elm_Object_Item *image_header;
   Elm_Object_Item *vector_header;
};

static Eina_Bool _on_image_done(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info);
static void _image_info_setup(const Image_Item* it);

static Image_Manager mng;
static Elm_Gengrid_Item_Class *gic_group = NULL;
static Elm_Gengrid_Item_Class *gic_vector = NULL;
static Elm_Gengrid_Item_Class *gic = NULL;
static Elm_Gengrid_Item_Class *gic_set = NULL;

static char *
_grid_label_get(void *data,
                Evas_Object *obj __UNUSED__,
                const char  *part)
{
   const Image_Item *it = data;
   Resource2 *res;

   if (!strcmp(part, "elm.text.count"))
     {
        res = resource_manager_find(ap.project->RM.image_sets, it->image_name);
        int count = eina_list_count(res->common.uses___);
        if (count <= 4 && count > 0) return strdup("");
        char buf[256];
        snprintf(buf, 256, "%d", count);
        return strdup(buf);
     }
   else
     {
        return strdup(it->image_name);
     }
}

static char *
_grid_vector_label_get(void *data,
                       Evas_Object *obj __UNUSED__,
                       const char  *part __UNUSED__)
{
   const Eina_Stringshare *vector_name = data;

   return strdup(vector_name);
}

static void
_image_manager_image_setup(Evas_Object *image,
                           const Image_Item *it)
{
   assert(image != NULL);
   assert(it != NULL);

#ifndef _WIN32
   if (it->comp_type == EDJE_EDIT_IMAGE_COMP_USER)
     {
        if (ecore_file_exists(it->source))
          elm_image_file_set(image, it->source, NULL);
        else
          elm_image_file_set(image, ap.path.theme_edj, "elm/image/icon/attention");
     }
   else
     {
        elm_image_file_set(image, it->source, NULL);
     }
#else
   TODO("Remove this urgly hack when we fix thumbs on Windows")
   if (it->comp_type == EDJE_EDIT_IMAGE_COMP_USER)
     {
        if (ecore_file_exists(it->source))
          elm_image_file_set(image, it->source, NULL);
        else
          elm_image_file_set(image, ap.path.theme_edj, "elm/image/icon/attention");
     }
   else
     {
        elm_image_file_set(image, it->source, NULL);
     }
#endif /* _WIN32 */
}

static inline Evas_Object *
_image_manager_image_create(Evas_Object *parent,
                            const Image_Item *it)
{
   assert(parent != NULL);
   assert(it != NULL);

   Evas_Object *image = elm_image_add(parent);
   elm_image_smooth_set(image, false);
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
   Resource2 *res;

   assert(it != NULL);
   assert(grid != NULL);

   if (!strcmp(part, "elm.swallow.icon"))
     {
#ifndef _WIN32
        image_obj = elm_thumb_add(grid);
#else
        image_obj = elm_image_add(grid);
#endif /* _win32 */
        elm_object_style_set(image_obj, "noframe");
        _image_manager_image_setup(image_obj, it);
        evas_object_show(image_obj);
     }
   else if (!strcmp(part, "elm.swallow.end"))
     {
        it->is_used = true;
        res = resource_manager_find(ap.project->RM.images, it->image_name);
        if (!res->common.used_in)
          {
             image_obj = elm_icon_add(grid);
             elm_image_file_set(image_obj, ap.path.theme_edj, "elm/image/icon/attention");
             evas_object_show(image_obj);
             it->is_used = false;
          }
     }

   return image_obj;
}


static void
_grid_image_set_image_sel(void *data,
                          Evas_Object *obj __UNUSED__,
                          void *event_info __UNUSED__)
{
   Image_Item *it = (Image_Item *)data;

   assert(it != NULL);

   evas_object_smart_callback_call(ap.win, SIGNAL_IMAGE_SELECTED, it);
}

static Eina_Bool
_on_image_set_image_done(void *data,
                         Evas_Object *obj __UNUSED__,
                         void *event_info)
{
   const Eina_List *images = NULL, *l = NULL;
   const char *selected = NULL;
   const char *file_name = NULL;
   Image_Item *image_set = (Image_Item *)data;

   images = (const Eina_List *)event_info;
   _on_image_done(NULL, NULL, (void *)eina_list_clone(images));

   EINA_LIST_FOREACH(images, l, selected)
     {
        if (ecore_file_is_dir(selected))
          {
             ERR(_("Unable to add a folder"));
             continue;
          }
        file_name = ecore_file_file_get(selected);

        CRIT_ON_FAIL(editor_image_set_image_add(ap.project->global_object, image_set->image_name, file_name, true));

     }
   _image_info_setup(image_set);
   elm_gengrid_realized_items_update(mng.gengrid);
   return true;
}

static void
_image_set_image_add(void *data,
                     Evas_Object *obj __UNUSED__,
                     void *event_info __UNUSED__)
{
#if HAVE_TIZEN
   popup_fileselector_image_helper(_("Add new image into set"),
                                   NULL,
                                   NULL,
                                   _on_image_set_image_done,
                                   data,
                                   true,
                                   false);
#else
   popup_fileselector_image_helper(NULL,
                                   NULL,
                                   NULL,
                                   _on_image_set_image_done,
                                   data,
                                   true,
                                   false);
#endif
}

static void
_image_set_image_del(void *data __UNUSED__,
                     Evas_Object *obj __UNUSED__,
                     void *event_info __UNUSED__)
{
   Image_Item *image_set = (Image_Item *)data;
   Image_Item *image = NULL;
   Eina_List *sel_list = NULL, *l = NULL, *l2 = NULL;
   Elm_Object_Item *gengrid_item = NULL;
   Elm_Object_Item *gengrid_next_item = NULL;

   assert(mng.image_set.grid != NULL);
   sel_list = (Eina_List *)elm_gengrid_selected_items_get(mng.image_set.grid);

   EINA_LIST_FOREACH_SAFE(sel_list, l, l2, gengrid_item)
     {
        image = (Image_Item *)elm_object_item_data_get(gengrid_item);
        CRIT_ON_FAIL(editor_image_set_image_del(ap.project->global_object, image_set->image_name, image->image_name, true));

        elm_gengrid_item_selected_set(gengrid_item, false);
        gengrid_next_item = elm_gengrid_item_next_get(gengrid_item);
        elm_object_item_del(gengrid_item);
     }
   elm_gengrid_item_selected_set(gengrid_next_item, true);
   elm_gengrid_realized_items_update(mng.image_set.grid);
   elm_gengrid_realized_items_update(mng.gengrid);
}

static inline Evas_Object *
_image_manager_image_set_grid_create(Evas_Object *parent,
                                     const Image_Item *it)
{
   Resource2 *res = NULL;
   Image2 *image_res = NULL;
   Eina_List *l = NULL;
   Evas_Object *layout = NULL;
   Evas_Object *button = NULL;
   Image_Item *image_set_item = NULL;

   assert(parent != NULL);
   assert(it != NULL);

   layout = elm_layout_add(parent);
#ifdef HAVE_TIZEN
   elm_layout_theme_set(layout, "layout", "image_manager", "image_set");
#else
   elm_layout_theme_set(layout, "layout", "image_manager", "default");
#endif
   evas_object_del(mng.image_set.grid);
   mng.image_set.grid = elm_gengrid_add(layout);
   elm_gengrid_item_size_set(mng.image_set.grid, ITEM_WIDTH, ITEM_HEIGHT);
   elm_gengrid_align_set(mng.image_set.grid, 0.0, 0.0);
   elm_scroller_policy_set(mng.image_set.grid, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_AUTO);
   elm_gengrid_multi_select_mode_set(mng.image_set.grid, ELM_OBJECT_MULTI_SELECT_MODE_WITH_CONTROL);
   elm_gengrid_select_mode_set(mng.image_set.grid, ELM_OBJECT_SELECT_MODE_ALWAYS);

   res = resource_manager_find(ap.project->RM.image_sets, it->image_name);
   int place = 0;
   EINA_LIST_FOREACH(res->common.uses___, l, image_res)
     {
        image_set_item = (Image_Item *)mem_calloc(1, sizeof(Image_Item));
        image_set_item->type = IMAGE_SET_ITEM;
        image_set_item->image_name = eina_stringshare_add(image_res->common.name);
        image_set_item->source = eina_stringshare_add(image_res->source);
        image_set_item->height = image_res->height;
        image_set_item->width = image_res->width;
        image_set_item->quality = image_res->quality;
        image_set_item->comp_type = image_res->comp_type;
        image_set_item->set.name = eina_stringshare_add(res->common.name);
        image_set_item->set.position = place;
        edje_edit_image_set_image_border_get(ap.project->global_object, res->common.name, place,
                                             &image_set_item->set.border_l, &image_set_item->set.border_r,
                                             &image_set_item->set.border_t, &image_set_item->set.border_b);
        edje_edit_image_set_image_max_get(ap.project->global_object, res->common.name, place,
                                          &image_set_item->set.max_w, &image_set_item->set.max_h);
        edje_edit_image_set_image_min_get(ap.project->global_object, res->common.name, place,
                                          &image_set_item->set.min_w, &image_set_item->set.min_h);
        image_set_item->set.border_scale = edje_edit_image_set_image_border_scale_get(ap.project->global_object,
                                                                                      res->common.name,
                                                                                      place);
        elm_gengrid_item_append(mng.image_set.grid, gic, image_set_item, _grid_image_set_image_sel , (void *)image_set_item);
        place++;
     }

   elm_object_part_content_set(layout, "elm.swallow.list", mng.image_set.grid);

   button = elm_button_add(layout);
   elm_object_style_set(button, "plus_managers");
   evas_object_smart_callback_add(button, signals.elm.button.clicked, _image_set_image_add, it);
   elm_object_part_content_set(layout, "elm.swallow.btn_add", button);

   button = elm_button_add(layout);
   elm_object_style_set(button, "minus_managers");
   evas_object_smart_callback_add(button, signals.elm.button.clicked, _image_set_image_del, it);
   elm_object_part_content_set(layout, "elm.swallow.btn_del", button);

   evas_object_show(layout);
   return layout;
}
/* icon fetching callback */
static Evas_Object *
_grid_image_set_content_get(void *data,
                            Evas_Object *obj,
                            const char  *part)
{
   Image_Item *it = data;
   Evas_Object *image_obj = NULL;
   Evas_Object *grid = (Evas_Object *)obj;
   Resource2 *res;
   const char *source = NULL;
   Image2 *img_res = NULL;

   assert(it != NULL);
   assert(grid != NULL);

   elm_object_style_set(image_obj, "noframe");
   if (!strcmp(part, "elm.swallow.end"))
     {
        it->is_used = true;
        res = resource_manager_find(ap.project->RM.image_sets, it->image_name);
        if (!res->common.used_in)
          {
             image_obj = elm_icon_add(grid);
             elm_image_file_set(image_obj, ap.path.theme_edj, "elm/image/icon/attention");
             evas_object_show(image_obj);
             it->is_used = false;
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
        source = it->source;

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
        it->source = img_res->source;
        _image_manager_image_setup(image_obj, it);
        it->source = source;
        evas_object_show(image_obj);
     }
   return image_obj;

empty_content:
   evas_object_del(image_obj);
   return NULL;
}

/* deletion callback */
static void
_grid_vector_del(void *data,
                 Evas_Object *obj __UNUSED__)
{
   Eina_Stringshare *vector_name = data;

   assert(vector_name != NULL);

   eina_stringshare_del(vector_name);
}

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

static char *
_grid_group_label_get(void *data,
                      Evas_Object *obj __UNUSED__,
                      const char  *part __UNUSED__)
{
   return strdup(data);
}

static void
_grid_group_del(void *data,
                Evas_Object *obj __UNUSED__)
{
   eina_stringshare_del(data);
}

static void
_image_info_setup(const Image_Item* it)
{
   Evas_Object *content = NULL;

   assert(it != NULL);

   if (it->type == SINGLE_IMAGE)
     content = _image_manager_image_create(mng.layout, it);
   else if (it->type == IMAGE_SET)
     content =  _image_manager_image_set_grid_create(mng.layout, it);

   evas_object_show(content);

   mng.image = content;
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
   Evas_Object *to_del  = NULL;
   char buf[PATH_MAX];

   sel_list = (Eina_List *)elm_gengrid_selected_items_get(mng.gengrid);
   int selected_images_count = eina_list_count(sel_list);

   /* if no selected images, disable delete button */
   if (selected_images_count != 0)
     elm_object_disabled_set(mng.del_button, true);

#ifdef HAVE_TIZEN
   to_del = elm_object_part_content_unset(mng.entry_prev, "elm.swallow.entry");
#else
   to_del = elm_object_part_content_unset(mng.property_panes, "left");
#endif
   if (to_del == mng.image)
     mng.image = NULL;
   else if (to_del == mng.image_set.grid)
     mng.image_set.grid = NULL;
   evas_object_del(to_del);

   if (selected_images_count == 1)
     {
        item = elm_object_item_data_get(eina_list_data_get(sel_list));

        /* if selected image is not used, we can delete it */
        if (!item->is_used)
          elm_object_disabled_set(mng.del_button, false);

        _image_info_setup(item);
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
        snprintf(buf, sizeof(buf), "%s"EFLETE_DUMMY_IMAGE_NAME, ap.path.image_path);
        mng.image = elm_image_add(mng.property_panes);
        elm_image_file_set(mng.image, buf, NULL);
        evas_object_show(mng.image);
     }
#ifdef HAVE_TIZEN
   elm_object_part_content_set(mng.entry_prev, "elm.swallow.entry", mng.image);
#else
   elm_object_part_content_set(mng.property_panes, "left", mng.image);
#endif
   evas_object_smart_callback_call(ap.win, SIGNAL_IMAGE_SELECTED, item);
}

static inline Image_Item *
_image_manager_gengrid_item_data_set_create(Evas_Object *edje_edit_obj,
                                            Image_Set2 *res)
{
   Evas_Object *img;

   assert(edje_edit_obj != NULL);
   assert(res != NULL);

   Image_Item *it = (Image_Item *)mem_calloc(1, sizeof(Image_Item));
   it->image_name = eina_stringshare_add(res->common.name);
   it->id = edje_edit_image_set_id_get(edje_edit_obj, it->image_name);
   it->comp_type = 0;
   it->quality = 0;

   Image2 *res_image = NULL;
   res_image = eina_list_data_get(res->common.uses___);
   if (res_image) it->source = eina_stringshare_add(res_image->source);
   else it->source = eina_stringshare_printf("%s%s", ap.path.image_path, EFLETE_DUMMY_IMAGE_NAME);
   it->type = IMAGE_SET;

   img = _image_manager_image_create(ap.project->global_object, it);
   elm_image_object_size_get(img, &it->width, &it->height);
   evas_object_del(img);

   return it;
}

static inline Image_Item *
_image_manager_gengrid_item_data_create(Evas_Object *edje_edit_obj,
                                       Image2 *res)
{
   Evas_Object *img;

   assert(edje_edit_obj != NULL);
   assert(res != NULL);

   Image_Item *it = (Image_Item *)mem_malloc(sizeof(Image_Item));
   it->image_name = eina_stringshare_add(res->common.name);
   it->id = edje_edit_image_id_get(edje_edit_obj, it->image_name);
   it->comp_type = edje_edit_image_compression_type_get(edje_edit_obj,
                                                        it->image_name);
   it->quality = edje_edit_image_compression_rate_get(edje_edit_obj,
                                                      it->image_name);
   it->source = eina_stringshare_add(res->source);
   it->type = SINGLE_IMAGE;

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
   const char *file_name;
   Evas_Object *img;
   Eina_Stringshare *res_path;

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

        res_path = eina_stringshare_printf("%s/images/%s", ap.project->develop_path, file_name);

        if (!ecore_file_exists(res_path))
          {
             ecore_file_cp(selected, res_path);
          }
        else
          {
             ERR(_("File exist"));
             free(image);
             eina_stringshare_del(res_path);
             continue;
          }
        CRIT_ON_FAIL(editor_image_add(ap.project->global_object, selected, true));

        it = (Image_Item *)mem_malloc(sizeof(Image_Item));
        it->image_name = eina_stringshare_add(file_name);
        it->type = SINGLE_IMAGE;
        it->id = edje_edit_image_id_get(ap.project->global_object, it->image_name);
        elm_gengrid_item_insert_before(mng.gengrid, gic, it, mng.image_set_header, _grid_sel_cb, NULL);
        it->comp_type = edje_edit_image_compression_type_get(ap.project->global_object,
                                                             it->image_name);
        it->quality = edje_edit_image_compression_rate_get(ap.project->global_object,
                                                           it->image_name);

        it->source = eina_stringshare_add(res_path);
        img = _image_manager_image_create(ap.project->global_object, it);
        elm_image_object_size_get(img, &it->width, &it->height);
        evas_object_del(img);
        eina_stringshare_del(res_path);
     }

   return true;
}

static void
_new_image_add_cb(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
#if HAVE_TIZEN
   popup_fileselector_image_helper(_("Choose image"),
                                   NULL,
                                   NULL,
                                   _on_image_done,
                                   data,
                                   true,
                                   false);
#else
   popup_fileselector_image_helper(NULL,
                                   NULL,
                                   NULL,
                                   _on_image_done,
                                   data,
                                   true,
                                   false);
#endif
}

static void
_menu_dismissed_cb(void *data __UNUSED__,
                   Evas_Object *obj,
                   void *event_info __UNUSED__)
{
   shortcuts_object_check_pop(obj);
}

static void
_menu_dismiss_cb(void *data __UNUSED__,
                 Evas_Object *obj,
                 void *event_info __UNUSED__)
{
   elm_menu_close(obj);
   shortcuts_object_check_pop(obj);
}



static void
_validation(void *data,
            Evas_Object *obj __UNUSED__,
            void *event_info __UNUSED__)
{
   Evas_Object *popup = data;
   Eina_Bool validate = EINA_FALSE;

   assert(popup != NULL);

   if (resource_name_validator_status_get(mng.image_set.validator) != ELM_REG_NOERROR)
     {
        validate = EINA_FALSE;
        elm_object_signal_emit(mng.image_set.entry, "validation,default,fail", "elm");
     }
   else
     {
        validate = EINA_TRUE;
        elm_object_signal_emit(mng.image_set.entry, "validation,default,pass", "elm");
     }


   if (!validate)
       popup_button_disabled_set(popup, BTN_OK, true);
   else
       popup_button_disabled_set(popup, BTN_OK, false);
}

static void
_image_set_add(void)
{
   Eina_Stringshare *image_set_name = eina_stringshare_add(elm_entry_entry_get(mng.image_set.entry));
   Image_Item *it = NULL;

   CRIT_ON_FAIL(editor_image_set_add(ap.project->global_object, image_set_name, true));
   it = (Image_Item *)mem_calloc(1, sizeof(Image_Item));
   it->image_name = eina_stringshare_add(image_set_name);
   it->id = edje_edit_image_set_id_get(ap.project->global_object, it->image_name);
   it->type = IMAGE_SET;
   elm_gengrid_item_insert_before(mng.gengrid, gic_set, it, mng.vector_header, _grid_sel_cb, NULL);
   eina_stringshare_del(image_set_name);
}

static void
_image_set_add_popup_close_cb(void *data __UNUSED__,
                         Evas_Object *obj __UNUSED__,
                         void *event_info)
{
   Popup_Button btn_res = (Popup_Button) event_info;

   if (BTN_CANCEL != btn_res)
     _image_set_add();

   resource_name_validator_free(mng.image_set.validator);
   evas_object_del(mng.image_set.box);
}


Evas_Object *
_add_image_set_content_get(void *data __UNUSED__, Evas_Object *popup, Evas_Object **to_focus)
{
   Evas_Object *item, *box;

   BOX_ADD(mng.win, box, false, false);
   elm_box_padding_set(box, 0, 10);
   LAYOUT_PROP_ADD(box, _("Image set name:"), "popup", "1swallow")
   ENTRY_ADD(item, mng.image_set.entry, true);
   efl_event_callback_add(mng.image_set.entry, ELM_ENTRY_EVENT_VALIDATE,
                          resource_name_validator_helper, mng.image_set.validator);
   evas_object_smart_callback_add(mng.image_set.entry, signals.elm.entry.changed, _validation, popup);
   elm_object_part_text_set(mng.image_set.entry, "guide", _("Type a new image set name"));
   elm_object_part_content_set(item, "elm.swallow.content", mng.image_set.entry);
   /* need to manualy set not valid string for triggered validator */
   elm_entry_entry_set(mng.image_set.entry, NULL);
   elm_box_pack_end(box, item);
   mng.image_set.box = box;

   if (to_focus) *to_focus = mng.image_set.entry;
   return box;
}

static void
_new_image_set_add_cb(void *data __UNUSED__,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   Evas_Object *popup;
   mng.image_set.validator = resource_name_validator_new(NAME_REGEX, NULL);
   resource_name_validator_list_set(mng.image_set.validator, &ap.project->RM.image_sets, true);

   popup = popup_add(_("Create a new image set"), NULL, BTN_OK|BTN_CANCEL, _add_image_set_content_get, mng.image_set.entry);
   popup_button_disabled_set(popup, BTN_OK, true);
   evas_object_smart_callback_add(popup, POPUP_CLOSE_CB, _image_set_add_popup_close_cb, NULL);
}

static void
_image_add_cb(void *data __UNUSED__,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   Evas_Coord x, y, h;

   evas_object_geometry_get(obj, &x, &y, NULL, &h);

   elm_menu_move(mng.menu, x, y + h);
   evas_object_show(mng.menu);
   shortcuts_object_push(mng.menu);
}

static void
_image_del_cb(void *data __UNUSED__,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   Elm_Object_Item *grid_item = NULL;
   Image_Item *it = NULL;
   Eina_List *grid_list, *l, *l2;
   Image2 *res;
   Image_Set2 *res_is = NULL;

   assert(mng.gengrid != NULL);

   grid_list = (Eina_List *)elm_gengrid_selected_items_get(mng.gengrid);

   assert(grid_list != NULL);

   EINA_LIST_FOREACH_SAFE(grid_list, l, l2, grid_item)
     {
        it = elm_object_item_data_get(grid_item);
        if (it->type == SINGLE_IMAGE)
          {
             res = (Image2 *)resource_manager_find(ap.project->RM.images, it->image_name);

             if (!res->common.used_in)
               {
                  ecore_file_unlink(res->source);
                  CRIT_ON_FAIL(editor_image_del(ap.project->global_object, it->image_name, true));
                  /* Unselect and remove image from property */
               }
          }
        else if (it->type == IMAGE_SET)
          {
             res_is = (Image_Set2 *)resource_manager_find(ap.project->RM.image_sets, it->image_name);
             if (!res_is->common.used_in)
               {
                  CRIT_ON_FAIL(editor_image_set_del(ap.project->global_object, it->image_name, true));
               }
          }
        elm_gengrid_item_selected_set(grid_item, false);
        elm_object_item_del(grid_item);
     }

   evas_object_smart_callback_call(ap.win, SIGNAL_IMAGE_SELECTED, NULL);
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
   Evas_Object *entry;

   assert(parent != NULL);

   ENTRY_ADD(parent, entry, true);
   elm_object_part_text_set(entry, "guide", _("Search"));
#if !HAVE_TIZEN
   Evas_Object *icon;
   ICON_STANDARD_ADD(entry, icon, true, "search");
   elm_object_part_content_set(entry, "elm.swallow.end", icon);
#else
   elm_object_style_set(entry, "search");
#endif

   return entry;
}

static Eina_Bool
_image_manager_init(void)
{
   Eina_List *l = NULL;
   Image_Item *it = NULL;
   Eina_List *images = NULL;
   int counter = 0;
   Image2 *res = NULL;
   Image_Set2 *image_set = NULL;
   Vector2 *vector = NULL;
   Eina_Stringshare *name;

   images = ap.project->RM.images;

   /*
    * Add group image
    */
   name = eina_stringshare_add(_("Image"));
   mng.image_header = elm_gengrid_item_append(mng.gengrid, gic_group, name, NULL, NULL);
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

              it = _image_manager_gengrid_item_data_create(ap.project->global_object, res);
              elm_gengrid_item_append(mng.gengrid, gic, it, _grid_sel_cb, NULL);
           }
         elm_gengrid_item_bring_in(elm_gengrid_first_item_get(mng.gengrid), ELM_GENGRID_ITEM_SCROLLTO_TOP);
     }

   /*
    * Add group image-set
    */
   name = eina_stringshare_add(_("Image Set"));
   mng.image_set_header = elm_gengrid_item_append(mng.gengrid, gic_group, name, NULL, NULL);
   counter = 0;
   EINA_LIST_FOREACH(ap.project->RM.image_sets, l, image_set)
     {
        counter++;
        if (!image_set->common.name)
          {
             ERR("name not found for image #%d",counter);
             continue;
          }

        it = _image_manager_gengrid_item_data_set_create(ap.project->global_object, image_set);
        elm_gengrid_item_append(mng.gengrid, gic_set, it, _grid_sel_cb, NULL);
     }

   /*
    * Add group vectors
    */
   name = eina_stringshare_add(_("Vectors"));
   mng.vector_header = elm_gengrid_item_append(mng.gengrid, gic_group, name, NULL, NULL);
   counter = 0;
   EINA_LIST_FOREACH(ap.project->RM.vectors, l, vector)
     {
        counter++;
        if (!vector->common.name)
          {
             ERR("name not found for vector #%d",counter);
             continue;
          }

        elm_gengrid_item_append(mng.gengrid, gic_vector, eina_stringshare_add(vector->common.name), NULL, NULL);
     }

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
   evas_object_del(mng.image);
   elm_gengrid_clear(mng.gengrid);
}

Evas_Object *
image_manager_add(void)
{
   Evas_Object *button, *ic, *search_entry = NULL;

   assert(ap.project != NULL);

   mng.win = mw_add();
   mw_title_set(mng.win, _("Image manager"));
   evas_object_smart_callback_add(mng.win, signals.eflete.modal_window.cancel, _mw_cancel_cb, NULL);
   evas_object_smart_callback_add(mng.win, signals.eflete.modal_window.done, _mw_done_cb, NULL);
#if !HAVE_TIZEN
   ic = elm_icon_add(mng.win);
   elm_icon_standard_set(ic, "image2");
#else
   IMAGE_ADD_NEW(mng.win, ic, "icon", "logo");
#endif
   mw_icon_set(mng.win, ic);

   if (mng.layout) goto done;

#ifdef HAVE_TIZEN
   Evas_Object *bg;

   mng.layout = elm_layout_add(mng.win);
   elm_layout_theme_set(mng.layout, "layout", "manager", "internal");
   elm_object_part_text_set(mng.layout, "elm.text", _("Preview"));
   elm_layout_text_set(mng.layout, "elm.subtext", _("Image Gallery"));
   mng.panes = elm_panes_add(mng.win);
   elm_panes_content_right_size_set(mng.panes, 0);
   elm_panes_content_right_min_size_set(mng.panes, 400);
   elm_object_style_set(mng.panes, "manager");
   elm_object_part_content_set(mng.panes, "left", mng.layout);
   elm_object_part_content_set(mng.panes, "right", ap.property.image_manager);

   mng.entry_prev = elm_layout_add(mng.layout);
   elm_layout_theme_set(mng.entry_prev, "layout", "manager", "preview");
   evas_object_show(mng.entry_prev);

   bg = elm_layout_add(mng.layout);
   elm_layout_theme_set(bg, "layout", "workspace", "bg");
   elm_object_part_content_set(mng.entry_prev, "elm.swallow.background", bg);
   elm_object_signal_emit(mng.entry_prev, "entry,show", "eflete");
   elm_object_part_content_set(mng.layout, "elm.swallow.preview", mng.entry_prev);
#else
   mng.layout = elm_layout_add(mng.win);
   elm_layout_theme_set(mng.layout, "layout", "image_manager", "default");
   mng.panes = elm_panes_add(mng.win);
   elm_object_part_content_set(mng.panes, "left", mng.layout);
   elm_panes_content_right_size_set(mng.panes, 0);
   elm_panes_content_right_min_size_set(mng.panes, 400);
   mng.property_panes = elm_panes_add(mng.win);
   elm_panes_horizontal_set(mng.property_panes, true);
   elm_object_part_content_set(mng.panes, "right", mng.property_panes);
   elm_object_part_content_set(mng.property_panes, "right", ap.property.image_manager);
   elm_panes_content_left_size_set(mng.property_panes, 0);
   elm_panes_content_left_min_size_set(mng.property_panes, 400);
#endif

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

   if (!gic_group)
     {
        gic_group = elm_gengrid_item_class_new();
        gic_group->item_style = "group_index";
        gic_group->func.text_get = _grid_group_label_get;
        gic_group->func.content_get = NULL;
        gic_group->func.del = _grid_group_del;
     }

   if (!gic_vector)
     {
        gic_vector = elm_gengrid_item_class_new();
        gic_vector->item_style = "default";
        gic_vector->func.text_get = _grid_vector_label_get;
        gic_vector->func.content_get = NULL; /* Not until it would be able to get svg from edj */
        gic_vector->func.del = _grid_vector_del;
     }

   mng.gengrid = elm_gengrid_add(mng.layout);
   elm_object_part_content_set(mng.layout, "elm.swallow.list", mng.gengrid);
   elm_gengrid_item_size_set(mng.gengrid, ITEM_WIDTH, ITEM_HEIGHT);
   elm_gengrid_align_set(mng.gengrid, 0.0, 0.0);
   elm_scroller_policy_set(mng.gengrid, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_AUTO);
   elm_gengrid_multi_select_set(mng.gengrid, true);
   elm_gengrid_multi_select_mode_set(mng.gengrid, ELM_OBJECT_MULTI_SELECT_MODE_WITH_CONTROL);
   elm_gengrid_select_mode_set(mng.gengrid, ELM_OBJECT_SELECT_MODE_DEFAULT);
   evas_object_smart_callback_add(mng.gengrid, signals.elm.gengrid.unselected, _grid_sel_cb, NULL);
   evas_object_show(mng.gengrid);

   button = elm_button_add(mng.layout);
   elm_object_style_set(button, "plus_managers");
   evas_object_smart_callback_add(button, signals.elm.button.clicked, _image_add_cb, NULL);
   elm_object_part_content_set(mng.layout, "elm.swallow.btn_add", button);

   mng.del_button = elm_button_add(mng.layout);
   elm_object_style_set(mng.del_button, "minus_managers");
   evas_object_smart_callback_add(mng.del_button, signals.elm.button.clicked, _image_del_cb, NULL);
   elm_object_part_content_set(mng.layout, "elm.swallow.btn_del", mng.del_button);
   elm_object_disabled_set(mng.del_button, true);

   mng.menu = elm_menu_add(ap.win);
   elm_menu_item_add(mng.menu, NULL, "image", _("Image"), _new_image_add_cb, NULL);
   elm_menu_item_add(mng.menu, NULL, "image_set", _("Image set"), _new_image_set_add_cb, NULL);
   evas_object_smart_callback_add(mng.menu, "dismissed", _menu_dismissed_cb, NULL);
   evas_object_smart_callback_add(mng.menu, signals.shortcut.popup.cancel, _menu_dismiss_cb, NULL);

   // Search line add
   search_entry = _image_manager_search_field_create(mng.layout);
   elm_object_part_content_set(mng.layout, "elm.swallow.search", search_entry);
   evas_object_smart_callback_add(search_entry, signals.elm.entry.changed, _entry_changed_cb, NULL);
   evas_object_smart_callback_add(search_entry, signals.elm.entry.activated, _find_next_cb, NULL);
   mng.image_search_data.search_entry = search_entry;
   mng.image_search_data.last_item_found = NULL;

   evas_object_smart_callback_add(ap.win, SIGNAL_PROJECT_CLOSED, _project_closed_cb, NULL);

done:
   /* if genlist is empty try to fill it. This happens if the managers called
    * first time or projectwas reopened */
   if (!elm_gengrid_realized_items_get(mng.gengrid))
     _image_manager_init();
   else
     elm_gengrid_realized_items_update(mng.gengrid);

   elm_object_content_set(mng.win, mng.panes);
   evas_object_show(mng.win);
   elm_object_focus_set(search_entry, true);
   return mng.win;
}
