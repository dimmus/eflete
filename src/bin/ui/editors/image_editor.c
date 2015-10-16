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

#include "image_editor.h"
#include "main_window.h"
#include "editor.h"

TODO("Rename this file to image_manager")

#define ITEM_WIDTH 100
#define ITEM_HEIGHT 115
#define GROUP_ITEM_WIDTH 36
#define GROUP_ITEM_HEIGHT 36
#define IMG_EDIT_KEY "image_editor_key"

typedef struct _Image_Editor Image_Editor;
typedef struct _Search_Data Search_Data;
typedef struct _Item Item;
typedef struct _Content_Init_Data Content_Init_Data;

struct _Item
{
   int id;
   const char* image_name;
   const char* source;
   Edje_Edit_Image_Comp comp_type;
};

struct _Content_Init_Data
{
   Image_Editor *image_editor;
   Item *item_data;
   Evas_Object *image_obj;
};

struct _Search_Data
{
   Evas_Object *search_entry;
   Elm_Object_Item *last_item_found;
};

struct _Image_Editor
{
   Evas_Object *win;
   Evas_Object *fs_win;
   Evas_Object *gengrid;
   Evas_Object *layout;
   Search_Data image_search_data;
   Search_Data usage_search_data;
   struct {
      Evas_Object *tabs;
      Evas_Object *image_info;
      Evas_Object *image_usage;
      Elm_Object_Item *item_image_info;
      Elm_Object_Item *item_image_usage;
   } property;
   struct {
      Evas_Object *genlist;
   } image_usage_fields;
   struct {
      const char  *image_name;
      Evas_Object *layout;
      Evas_Object *image;
      Evas_Object *file_name;
      Evas_Object *comp;
      Evas_Object *quality;
   } image_data_fields;
};

static Elm_Gengrid_Item_Class *gic = NULL;
static Elm_Genlist_Item_Class *_itc_group = NULL;
static Elm_Genlist_Item_Class *_itc_part = NULL;
static Elm_Genlist_Item_Class *_itc_state = NULL;

static void _image_editor_del(Image_Editor *img_edit);

static char *
_grid_label_get(void *data,
                Evas_Object *obj __UNUSED__,
                const char  *part __UNUSED__)
{
   const Item *it = data;
   return strdup(it->image_name);
}

static void
_on_image_editor_del(void * data,
                     Evas *e __UNUSED__,
                     Evas_Object *obj __UNUSED__,
                     void *event_info __UNUSED__)
{
   Image_Editor *img_edit = (Image_Editor *)data;

   assert(img_edit != NULL);

   _image_editor_del(img_edit);
}

static void
_image_editor_del(Image_Editor *img_edit)
{
   assert(img_edit != NULL);

   ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_MAIN, false);

   evas_object_event_callback_del(img_edit->layout, EVAS_CALLBACK_DEL, _on_image_editor_del);

   elm_gengrid_item_class_free(gic);
   elm_genlist_item_class_free(_itc_group);
   elm_genlist_item_class_free(_itc_part);
   elm_genlist_item_class_free(_itc_state);
   gic = NULL;
   _itc_group = NULL;
   _itc_part = NULL;
   _itc_state = NULL;
   evas_object_data_del(img_edit->layout, IMG_EDIT_KEY);
   evas_object_data_del(img_edit->gengrid, IMG_EDIT_KEY);
   //evas_object_del(img_edit->gengrid);
   free(img_edit);
}

static void
_image_editor_image_setup(Evas_Object *image,
                          Image_Editor *img_edit,
                          const Item *it)
{
   assert(image != NULL);
   assert(img_edit != NULL);
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
_image_editor_image_create(Evas_Object *parent,
                           Image_Editor *img_edit,
                           const Item *it)
{
   assert(parent != NULL);
   assert(img_edit != NULL);
   assert(it != NULL);

   Evas_Object *image = elm_image_add(parent);
   _image_editor_image_setup(image, img_edit, it);
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

   Image_Editor *img_edit = evas_object_data_get(grid, IMG_EDIT_KEY);

   assert(img_edit != NULL);

   if (!strcmp(part, "elm.swallow.icon"))
     {
        image_obj = elm_thumb_add(grid);
        _image_editor_image_setup(image_obj, img_edit, it);
        elm_thumb_reload(image_obj);
        evas_object_show(image_obj);
   /* functions for deferred creation of gengrid icons */
   //ecore_job_add(_image_content_setup, image_init_data);
     }
   else if (!strcmp(part, "elm.swallow.end"))
     {
        res = (Resource *) pm_resource_get(ap.project->images, it->image_name);
        if (eina_list_count(res->used_in) == 0)
          {
             image_obj = elm_icon_add(grid);
             elm_image_file_set(image_obj, EFLETE_THEME, "elm/image/icon/attention");
             evas_object_show(image_obj);
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
   free(it);
}

void
_grid_group_item_del(void *data, Evas_Object *obj __UNUSED__)
{
   if (data)
     eina_stringshare_del(data);
}

static char *
_grid_group_item_label_get(void *data,
                           Evas_Object *obj __UNUSED__,
                           const char  *part)
{
   if ((part) && (!strcmp(part, "elm.text")))
     return strdup(data);
   else
     return NULL;
}

static inline void
genlist_item_classes_init()
{
   if (!_itc_group)
     {
        _itc_group = elm_genlist_item_class_new();
        _itc_group->item_style = "title";
        _itc_group->func.text_get = _grid_group_item_label_get;
        _itc_group->func.content_get = NULL;
        _itc_group->func.state_get = NULL;
        _itc_group->func.del = _grid_group_item_del;
     }
   if (!_itc_part)
     {
        _itc_part = elm_genlist_item_class_new();
        _itc_part->item_style = "default";
        _itc_part->func.text_get = _grid_group_item_label_get;
        _itc_part->func.content_get = NULL;
        _itc_part->func.state_get = NULL;
        _itc_part->func.del = _grid_group_item_del;
     }
   if (!_itc_state)
     {
        _itc_state = elm_genlist_item_class_new();
        _itc_state->item_style = "default";
        _itc_state->func.text_get = _grid_group_item_label_get;
        _itc_state->func.content_get = NULL;
        _itc_state->func.state_get = NULL;
        _itc_state->func.del = _grid_group_item_del;
     }
}

/*
USABLE FOR GENLIST OF IMAGE USAGE

be used like this:
>   Resource *res = (Resource *) pm_resource_get(ap.project->images, it->image_name);
>   _image_info_update_usage_info(img_edit, eina_list_count(res->used_in));
>   _image_info_usage_update(img_edit, res);
>   evas_object_smart_calculate(img_edit->image_data_fields.layout);

static void
_image_info_usage_update(Image_Editor *img_edit, Resource *res)
{
   Eina_List *l;
   Evas_Object *list;
   Elm_Object_Item *it_group, *it_part;
   const char *cur_group = NULL;
   const char *cur_part = NULL;
   State *state;

   assert(img_edit != NULL);
   assert(res != NULL);

   if (!img_edit->image_data_fields.image_name) return;

   if (!eina_list_count(res->used_in))
     return;

   genlist_item_classes_init();

   list = img_edit->image_usage_fields.genlist;

   EINA_LIST_FOREACH(res->used_in, l, state)
     {
        if ((!cur_group) || (strcmp(cur_group, state->part->group->name)))
          {
             it_group = elm_genlist_item_append(list, _itc_group,
                                                eina_stringshare_add(state->part->group->name),
                                                NULL, ELM_GENLIST_ITEM_NONE,
                                                NULL, NULL);
             cur_group = state->part->group->name;
             cur_part = NULL;
          }
        if ((!cur_part) || (strcmp(cur_part, state->part->name)))
          {
             it_part = elm_genlist_item_append(list, _itc_part,
                                               eina_stringshare_add(state->part->name),
                                               it_group, ELM_GENLIST_ITEM_NONE,
                                               NULL, NULL);
             cur_part = state->part->name;
          }
        if ((cur_part) && (!strcmp(cur_part, state->part->name)))
          {
             elm_genlist_item_append(list, _itc_state, state->name,
                                     it_part, ELM_GENLIST_ITEM_NONE,
                                     NULL, NULL);
          }
     }
}
*/

static void
_image_info_setup(Image_Editor *img_edit,
                  const Item* it)
{
   Evas_Object *image;

   assert(img_edit != NULL);
   assert(it != NULL);

   img_edit->image_data_fields.image_name = it->image_name;

   image = _image_editor_image_create(img_edit->layout, img_edit, it);
   evas_object_image_smooth_scale_set(image, false);
   img_edit->image_data_fields.image = image;
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
   Image_Editor *img_edit = (Image_Editor *)data;
   Item *item = NULL;

   assert(img_edit != NULL);

   const Eina_List* sel_list = elm_gengrid_selected_items_get(img_edit->gengrid);
   int selected_images_count = eina_list_count(sel_list);

   if (selected_images_count == 1)
     {
        item = elm_object_item_data_get(eina_list_data_get(sel_list));
        _image_info_setup(img_edit, item);
     }
   else
     evas_object_smart_callback_call(ap.win, SIGNAL_IMAGE_SELECTED, NULL);
}

static inline Item *
_image_editor_gengrid_item_data_create(Evas_Object *edje_edit_obj,
                                       External_Resource *res)
{
   assert(edje_edit_obj != NULL);
   assert(res != NULL);

   Item *it = (Item *)mem_malloc(sizeof(Item));
   it->image_name = eina_stringshare_add(res->name);
   it->id = edje_edit_image_id_get(edje_edit_obj, it->image_name);
   it->comp_type = edje_edit_image_compression_type_get(edje_edit_obj,
                                                        it->image_name);
   it->source = res->source;

   return it;
}

static void
_fs_del(void *data)
{
   Image_Editor *edit = (Image_Editor *)data;

   assert(edit != NULL);
   assert(edit->fs_win != NULL);

   evas_object_del(edit->fs_win);
   edit->fs_win = NULL;
}

static void
_on_image_done(void *data,
               Evas_Object *obj,
               void *event_info)
{
   Item *it = NULL;
   Elm_Object_Item *item = NULL;
   const Eina_List *images, *l;
   const char *selected = event_info;
   Uns_List *image = NULL;
   External_Resource *res;
   const char *file_name;

   Image_Editor *img_edit = (Image_Editor *)data;

   assert(img_edit != NULL);

   if ((!selected) || (!strcmp(selected, "")))
     goto del;
   images = elm_fileselector_selected_paths_get(obj);

   EINA_LIST_FOREACH(images, l, selected)
     {
        if (!ecore_file_exists(selected))
          {
             WIN_NOTIFY_ERROR(obj, _("File not exist"));
             continue;
          }
        if (ecore_file_is_dir(selected))
          {
             WIN_NOTIFY_ERROR(obj, _("Unable to add folder"))
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
             WIN_NOTIFY_ERROR(obj, _("File exist"));
             free(image);
             free(res);
             continue;
          }
        edje_edit_image_add(ap.project->global_object, selected);

        it = (Item *)mem_malloc(sizeof(Item));
        it->image_name = eina_stringshare_add(file_name);
        it->id = edje_edit_image_id_get(ap.project->global_object, it->image_name);
        item = elm_gengrid_item_append(img_edit->gengrid, gic, it, _grid_sel, img_edit);

        it->source = res->source;
        elm_gengrid_item_selected_set(item, true);
     }
   editor_save(ap.project->global_object);
   TODO("Remove this line once edje_edit_image_add would be added into Editor Modulei and saving would work properly")
   ap.project->changed = true;
del:
   ecore_job_add(_fs_del, img_edit);
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
                                   "bmp", "wbmp", "webp", "psd", "tga", NULL};

   if (dir) return true;

   i = 0;
   while(image_formats[i])
     {
        res = eina_str_has_extension(path, image_formats[i++]);
        if (res) return true;
     }

   return false;
}

static void
_on_button_add_clicked_cb(void *data,
                          Evas_Object *obj __UNUSED__,
                          void *event_info __UNUSED__)
{
   Evas_Object *fs, *ic;
   Image_Editor *edit = data;

   assert(edit != NULL);

   edit->fs_win  = mw_add(NULL, NULL, NULL);
   mw_title_set(edit->fs_win, "Add image to the library");
   ic = elm_icon_add(edit->fs_win);
   elm_icon_standard_set(ic, "folder");
   mw_icon_set(edit->fs_win, ic);
   evas_object_show(edit->fs_win);

   FILESELECTOR_ADD(fs, edit->fs_win, _on_image_done, data);
   elm_fileselector_custom_filter_append(fs, _images_filter, NULL, _("Images files"));
   elm_fileselector_mime_types_filter_append(fs, "*", _("All files"));
   elm_fileselector_multi_select_set(fs, true);
   elm_win_inwin_content_set(edit->fs_win, fs);
}

static void
_on_button_delete_clicked_cb(void *data,
                             Evas_Object *obj __UNUSED__,
                             void *event_info __UNUSED__)
{
   Image_Editor *img_edit = (Image_Editor *)data;
   Elm_Object_Item *grid_item = NULL;
   Item *it = NULL;
   Eina_List *grid_list, *l, *l2;
   int notdeleted = 0;
   Eina_List * in_use = NULL;
   char buf[BUFF_MAX];
   int symbs = 0;
   External_Resource *res;
   State *state;

   assert(img_edit != NULL);
   assert(img_edit->gengrid != NULL);

   grid_list = (Eina_List *)elm_gengrid_selected_items_get(img_edit->gengrid);

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
          {
             notdeleted++;
             if (notdeleted < 4)
                in_use = eina_list_append(in_use, res);
             elm_gengrid_item_selected_set(grid_item, false);
          }
     }
   if (notdeleted == 1)
     {
        res = eina_list_nth(in_use, 0);

        snprintf(buf, BUFF_MAX, _("Image is used in:"));
        symbs = strlen(buf);
        EINA_LIST_FOREACH(res->used_in, l, state)
          {
             snprintf(buf + symbs, BUFF_MAX - symbs, _("<br>group: %s<br>part: %s<br>state: \"%s\" %2.1f"),
                      state->part->group->name, state->part->name, state->parsed_name, state->parsed_val);
             symbs += strlen(res->name);
             break; TODO("remove this break after warning style remake")
          }
        NOTIFY_WARNING("%s", buf);
     }
   else if (notdeleted >1)
     {
        snprintf(buf, BUFF_MAX, NGETTEXT("%d image in use:",
                                         "%d images in use:",
                                         notdeleted),
                 notdeleted);
        symbs = strlen(buf);
        EINA_LIST_FOREACH(in_use, l, res)
          {
             snprintf(buf + symbs, BUFF_MAX - symbs, "<br>%s", res->name);
             symbs += strlen(res->name);
          }
        if (notdeleted >= 4)
           snprintf(buf + symbs, BUFF_MAX - symbs, "<br>...");
        NOTIFY_WARNING("%s", buf);
     }

   editor_save(ap.project->global_object);
   TODO("Remove this line once edje_edit_image_del would be added into Editor Modulei and saving would work properly")
   ap.project->changed = true;
}

TODO("Refactor and uncomment when savings and other stuff of project would be more stable")
/*
static void
_on_button_apply_clicked_cb(void *data,
                            Evas_Object *obj __UNUSED__,
                            void *event_info __UNUSED__)
{
   Image_Editor *img_edit = (Image_Editor *)data;
   Uns_List *unit = NULL;
   Eina_List *l, *names = NULL;
   Eina_Bool multiselect = false;
   const Eina_List *items;
   Elm_Object_Item *it;
   Item *item = NULL;
   char *ei;

   assert(img_edit != NULL);

   EINA_LIST_FOREACH(img_edit->unapplied_list, l, unit)
     {
        if (unit->act_type == ACTION_TYPE_DEL)
          {
             if (edje_edit_image_del(ap.project->global_object, unit->data))
               ap.project->nsimage_list = eina_list_append(ap.project->nsimage_list, unit);
          }
        else if (edje_edit_image_add(ap.project->global_object, unit->data))
          ap.project->nsimage_list = eina_list_append(ap.project->nsimage_list, unit);
     }
   //pm_save_to_dev(img_edit->pr, NULL, false);

   eina_list_free(img_edit->unapplied_list);

   assert(img_edit->gengrid != NULL);

   multiselect = elm_gengrid_multi_select_get(img_edit->gengrid);

   if (multiselect)
     {
        items = elm_gengrid_selected_items_get(img_edit->gengrid);
        EINA_LIST_FOREACH((Eina_List *)items, l, it)
          {
             item = elm_object_item_data_get(it);
             if (!item) continue;
             names = eina_list_append(names, eina_stringshare_add(item->image_name));
          }
     }
   else
     {
        it = elm_gengrid_selected_item_get(img_edit->gengrid);
        if (!it)
          {
            WIN_NOTIFY_WARNING(main_window_get(), _("Image not selected"));
            return;
          }
        item = elm_object_item_data_get(it);
        if (!item)
          {
             _image_editor_del(img_edit);
             return;
          }
     }

   if (!multiselect)
     {
        TODO("FIX IT! This line breaks images with / in name");
        ei = strdup(ecore_file_file_get(item->image_name));
        evas_object_smart_callback_call(main_window_get(), SIG_IMAGE_SELECTED, ei);
        free(ei);
     }
   else
     evas_object_smart_callback_call(main_window_get(), SIG_IMAGE_SELECTED,
                                     (Eina_List *) names);

   //project_changed(false);
   _image_editor_del(img_edit);
}

ITEM_SEARCH_FUNC(genlist, ELM_GENLIST_ITEM_SCROLLTO_MIDDLE, NULL)
*/
ITEM_SEARCH_FUNC(gengrid, ELM_GENGRID_ITEM_SCROLLTO_MIDDLE, NULL)

static void
_on_images_search_entry_changed_cb(void *data,
                                   Evas_Object *obj __UNUSED__,
                                   void *event_info __UNUSED__)
{
   Image_Editor *img_edit = data;

   assert(img_edit != NULL);

   _gengrid_item_search(img_edit->gengrid, &(img_edit->image_search_data),
                        img_edit->image_search_data.last_item_found);
}

/*
DELETE LATER: USABLE FOR USAGE LIST

static void
_on_usage_search_entry_changed_cb(void *data,
                                  Evas_Object *obj __UNUSED__,
                                  void *event_info __UNUSED__)
{
   Image_Editor *img_edit = data;

   assert(img_edit != NULL);

   _genlist_item_search(img_edit->image_usage_fields.genlist,
                        &(img_edit->usage_search_data),
                        img_edit->usage_search_data.last_item_found);
}

static void
_search_next_genlist_item_cb(void *data,
                             Evas_Object *obj __UNUSED__,
                             void *event_info __UNUSED__)
{
   Image_Editor *img_edit = data;
   Elm_Object_Item *start_from = NULL;

   assert(img_edit != NULL);

   if (img_edit->usage_search_data.last_item_found)
     {
        start_from =
           elm_genlist_item_next_get(img_edit->usage_search_data.last_item_found);
     }

   _genlist_item_search(img_edit->image_usage_fields.genlist,
                        &(img_edit->usage_search_data), start_from);
}
*/

static void
_search_next_gengrid_item_cb(void *data,
                             Evas_Object *obj __UNUSED__,
                             void *event_info __UNUSED__)
{
   Image_Editor *img_edit = data;
   Elm_Object_Item *start_from = NULL;

   assert(img_edit != NULL);

   if (img_edit->image_search_data.last_item_found)
     {
        start_from =
           elm_gengrid_item_next_get(img_edit->image_search_data.last_item_found);
     }

   _gengrid_item_search(img_edit->gengrid, &(img_edit->image_search_data),
                        start_from);
}

static inline Evas_Object *
_image_editor_search_field_create(Evas_Object *parent)
{
   Evas_Object *entry, *icon;

   assert(parent != NULL);

   ENTRY_ADD(parent, entry, true);
   elm_object_part_text_set(entry, "guide", _("Search"));
   ICON_STANDARD_ADD(entry, icon, true, "search");
   elm_object_part_content_set(entry, "elm.swallow.end", icon);
   return entry;
}

/*
DELETE LATER: USABLE FOR USAGE LIST

static Evas_Object *
_image_usage_layout_create(Image_Editor *img_edit, Evas_Object *parent)
{
   Evas_Object *layout = NULL;
   Evas_Object *genlist = NULL;
   Evas_Object *entry = NULL;

   assert(img_edit != NULL);
   assert(parent != NULL);

   layout = elm_layout_add(parent);
   elm_layout_theme_set(layout, "layout", "image_editor", "usage_info");
   evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(layout, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(layout);

   genlist = elm_genlist_add(layout);
   evas_object_size_hint_weight_set(genlist, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(genlist, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(genlist);
   elm_object_part_content_set(layout, "eflete.swallow.genlist", genlist);
   img_edit->image_usage_fields.genlist = genlist;

   entry = _image_editor_search_field_create(layout);
   elm_object_part_content_set(layout, "eflete.swallow.search_line", entry);
   evas_object_smart_callback_add(entry, "changed",
                                  _on_usage_search_entry_changed_cb, img_edit);
   evas_object_smart_callback_add(entry, "activated",
                                  _search_next_genlist_item_cb, img_edit);
   img_edit->usage_search_data.search_entry = entry;
   img_edit->usage_search_data.last_item_found = NULL;
   evas_object_hide(layout);
   return layout;
}
*/

Eina_Bool
_image_editor_init(Image_Editor *img_edit)
{
   Eina_List *l = NULL;
   Item *it = NULL;
   Eina_List *images = NULL;
   int counter = 0;
   External_Resource *res;

   assert(img_edit != NULL);

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
              it = _image_editor_gengrid_item_data_create(ap.project->global_object,
                                                          res);

              elm_gengrid_item_append(img_edit->gengrid, gic, it, _grid_sel, img_edit);
           }
         elm_gengrid_item_bring_in(elm_gengrid_first_item_get(img_edit->gengrid),
                                   ELM_GENGRID_ITEM_SCROLLTO_TOP);
     }
   elm_scroller_policy_set(img_edit->gengrid, ELM_SCROLLER_POLICY_OFF,
                           ELM_SCROLLER_POLICY_AUTO);
   evas_object_smart_calculate(img_edit->gengrid);

   return true;
}

Evas_Object *
image_editor_window_add(Image_Editor_Mode mode)
{
   Evas_Object *button;
   Evas_Object *ic = NULL;
   Evas_Object *search_entry = NULL;

   assert(ap.project != NULL);

   Image_Editor *img_edit = (Image_Editor *)mem_calloc(1, sizeof(Image_Editor));

   img_edit->layout = elm_layout_add(main_window_get());
   elm_layout_theme_set(img_edit->layout, "layout", "image_editor", "default");

   img_edit->gengrid = elm_gengrid_add(img_edit->layout);
   elm_object_part_content_set(img_edit->layout,
                               "eflete.swallow.grid", img_edit->gengrid);
   elm_gengrid_item_size_set(img_edit->gengrid, ITEM_WIDTH, ITEM_HEIGHT);
   elm_gengrid_group_item_size_set(img_edit->gengrid, GROUP_ITEM_WIDTH, GROUP_ITEM_HEIGHT);
   elm_gengrid_align_set(img_edit->gengrid, 0.0, 0.0);
   elm_scroller_policy_set(img_edit->gengrid, ELM_SCROLLER_POLICY_OFF,
                           ELM_SCROLLER_POLICY_OFF);

   if (mode == SINGLE)
     {
        elm_gengrid_multi_select_set(img_edit->gengrid, false);
     }
   else if (mode == MULTIPLE)
     {
        elm_gengrid_multi_select_set(img_edit->gengrid, true);
        elm_gengrid_multi_select_mode_set(img_edit->gengrid,
                                          ELM_OBJECT_MULTI_SELECT_MODE_WITH_CONTROL);
     }
   else if (mode == TWEENS)
     {
        elm_gengrid_multi_select_set(img_edit->gengrid, true);
        elm_gengrid_multi_select_mode_set(img_edit->gengrid,
                                          ELM_OBJECT_MULTI_SELECT_MODE_WITH_CONTROL);
     }

   elm_gengrid_select_mode_set(img_edit->gengrid, ELM_OBJECT_SELECT_MODE_ALWAYS);
   evas_object_size_hint_weight_set(img_edit->gengrid,
                                    EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(img_edit->gengrid,
                                   EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_smart_callback_add(img_edit->gengrid, "unselected", _grid_sel,
                                  img_edit);
   evas_object_show(img_edit->gengrid);

   button = elm_button_add(img_edit->layout);
   evas_object_show(button);
   ic = elm_icon_add(button);
   elm_icon_standard_set(ic, "plus");
   elm_object_part_content_set(button, NULL, ic);
   evas_object_smart_callback_add(button, "clicked",
                                  _on_button_add_clicked_cb, img_edit);
   elm_object_part_content_set(img_edit->layout,
                               "eflete.swallow.add_btn", button);

   button = elm_button_add(img_edit->layout);
   evas_object_show(button);
   ic = elm_icon_add(button);
   elm_icon_standard_set(ic, "minus");
   elm_object_part_content_set(button, NULL, ic);
   evas_object_smart_callback_add(button, "clicked",
                                  _on_button_delete_clicked_cb, img_edit);
   elm_object_part_content_set(img_edit->layout,
                               "eflete.swallow.del_btn", button);

   // Search line add
   search_entry = _image_editor_search_field_create(img_edit->layout);
   elm_object_part_content_set(img_edit->layout,
                               "eflete.swallow.search_area", search_entry);
   evas_object_smart_callback_add(search_entry, "changed",
                                  _on_images_search_entry_changed_cb, img_edit);
   evas_object_smart_callback_add(search_entry, "activated",
                                  _search_next_gengrid_item_cb, img_edit);
   img_edit->image_search_data.search_entry = search_entry;
   img_edit->image_search_data.last_item_found = NULL;

   if (!gic)
     {
        gic = elm_gengrid_item_class_new();
        gic->item_style = "default";
        gic->func.text_get = _grid_label_get;
        gic->func.content_get = _grid_content_get;
        gic->func.del = _grid_del;
     }

   elm_object_focus_set(search_entry, true);
   if (!_image_editor_init(img_edit))
     {
        ERR("Filed initialize image editor");
        abort();
     }
   evas_object_data_set(img_edit->gengrid, IMG_EDIT_KEY, img_edit);
   evas_object_data_set(img_edit->layout, IMG_EDIT_KEY, img_edit);

   ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_MAIN, true);
   evas_object_event_callback_add(img_edit->layout, EVAS_CALLBACK_DEL, _on_image_editor_del, img_edit);

   return img_edit->layout;
}

Eina_Bool
image_editor_file_choose(Evas_Object *win, const char *selected)
{
   Elm_Object_Item *grid_item = NULL;
   const Item* it = NULL;
   Image_Editor *img_edit = NULL;

   if (!selected) return false;

   assert(win != NULL);

   img_edit = evas_object_data_get(win, IMG_EDIT_KEY);

   assert(img_edit != NULL);

   grid_item = elm_gengrid_first_item_get(img_edit->gengrid);

   while(grid_item)
     {
        const char* item_style =
           elm_gengrid_item_item_class_get(grid_item)->item_style;
        if (!strcmp(item_style, "group_index"))
          {
             grid_item = elm_gengrid_item_next_get(grid_item);
             continue;
          }

        it = elm_object_item_data_get(grid_item);
        if (!it) return false;
        if (!strcmp(it->image_name, selected))
          {
             elm_gengrid_item_selected_set(grid_item, true);
             elm_gengrid_item_bring_in(grid_item,
                                       ELM_GENGRID_ITEM_SCROLLTO_MIDDLE);
             return true;
          }
        grid_item = elm_gengrid_item_next_get(grid_item);
     }
   return false;
}

#undef ITEM_WIDTH
#undef ITEM_HEIGHT
#undef GROUP_ITEM_WIDTH
#undef GROUP_ITEM_HEIGHT
#undef IMG_EDIT_KEY
