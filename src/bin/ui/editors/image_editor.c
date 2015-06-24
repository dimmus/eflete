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
   Project *pr;
   Evas_Object *win;
   Evas_Object *fs_win;
   Evas_Object *gengrid;
   Evas_Object *layout;
   Search_Data image_search_data;
   Search_Data usage_search_data;
   Evas_Object *tabs;
   Eina_List *unapplied_list;
   struct {
      Ewe_Tabs_Item *tab;
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
   struct {
      Elm_Object_Item *included;
      Elm_Object_Item *linked;
   } group_items;
};

static Elm_Gengrid_Item_Class *gic = NULL;
static Elm_Genlist_Item_Class *_itc_group = NULL;
static Elm_Genlist_Item_Class *_itc_part = NULL;
static Elm_Genlist_Item_Class *_itc_state = NULL;

static void _image_info_reset(Image_Editor *img_edit);
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
_on_mwin_del(void * data,
             Evas *e __UNUSED__,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   Image_Editor *img_edit = (Image_Editor *)data;
   _image_editor_del(img_edit);
}

static void
_image_editor_del(Image_Editor *img_edit)
{
   App_Data *ap = app_data_get();
   ui_menu_items_list_disable_set(ap->menu, MENU_ITEMS_LIST_MAIN, false);
   ap->modal_editor--;

   evas_object_event_callback_del(img_edit->win, EVAS_CALLBACK_DEL, _on_mwin_del);

   img_edit->pr = NULL;
   elm_gengrid_item_class_free(gic);
   elm_genlist_item_class_free(_itc_group);
   elm_genlist_item_class_free(_itc_part);
   elm_genlist_item_class_free(_itc_state);
   gic = NULL;
   _itc_group = NULL;
   _itc_part = NULL;
   _itc_state = NULL;
   evas_object_data_del(img_edit->win, IMG_EDIT_KEY);
   evas_object_data_del(img_edit->gengrid, IMG_EDIT_KEY);
   //evas_object_del(img_edit->gengrid);
   mw_del(img_edit->win);
   _image_info_reset(img_edit);
   free(img_edit);
}

static void
_image_editor_image_setup(Evas_Object *image,
                          Image_Editor *img_edit,
                          const Item *it)
{
   Eina_Stringshare *str;

   if (it->comp_type == EDJE_EDIT_IMAGE_COMP_USER)
     {
        if (ecore_file_exists(it->image_name))
          evas_object_image_file_set(image, it->image_name, NULL);
        else
          elm_image_file_set(image, EFLETE_RESOURCES, "no_image_warning");
     }
   else
     {
        str = eina_stringshare_printf("edje/images/%i", it->id);
        if (it->id > -1)
          elm_image_file_set(image, img_edit->pr->dev, str);
        else
          elm_image_file_set(image, it->image_name, NULL);
        eina_stringshare_del(str);
     }
}

static inline Evas_Object *
_image_editor_image_create(Evas_Object *parent,
                           Image_Editor *img_edit,
                           const Item *it)
{
   Evas_Object *image = elm_image_add(parent);
   _image_editor_image_setup(image, img_edit, it);
   return image;
}

/* functions for deferred creation of gengrid icons */
#define MAX_ICON_SIZE 16
static void
_image_content_setup(void *data)
{
   int w, h;
   Content_Init_Data *image_init_data = data;

   _image_editor_image_setup(image_init_data->image_obj,
                             image_init_data->image_editor,
                             image_init_data->item_data);

   elm_image_object_size_get(image_init_data->image_obj, &w, &h);
   if ((w < MAX_ICON_SIZE) && (h < MAX_ICON_SIZE))
     evas_object_size_hint_max_set(image_init_data->image_obj,
                                   MAX_ICON_SIZE, MAX_ICON_SIZE);
   else
     elm_image_resizable_set(image_init_data->image_obj, false, true);
   evas_object_show(image_init_data->image_obj);

   free(image_init_data);
}
#undef MAX_ICON_SIZE

static void
_image_usage_icon_setup(void *data)
{
   Eina_List *used_in;
   Evas_Object *edje_edit_obj;
   Content_Init_Data *image_init_data = data;

   GET_OBJ(image_init_data->image_editor->pr, edje_edit_obj);
   used_in = edje_edit_image_usage_list_get(edje_edit_obj,
                                            image_init_data->item_data->image_name,
                                            false);
   if (eina_list_count(used_in) == 0)
     {
        elm_image_file_set(image_init_data->image_obj,
                           EFLETE_RESOURCES, "no_image_warning");
        elm_image_no_scale_set(image_init_data->image_obj, true);
        evas_object_show(image_init_data->image_obj);
     }
   edje_edit_image_usage_list_free(used_in);

   free(image_init_data);
}

/* icon fetching callback */
static Evas_Object *
_grid_content_get(void *data,
                  Evas_Object *obj,
                  const char  *part)
{
   Item *it = data;
   Evas_Object *grid = (Evas_Object *)obj;
   Image_Editor *img_edit = evas_object_data_get(grid, IMG_EDIT_KEY);

   Content_Init_Data *image_init_data = mem_malloc(sizeof(Content_Init_Data));
   image_init_data->item_data = it;
   image_init_data->image_editor = img_edit;

   if (!strcmp(part, "elm.swallow.icon"))
     {
        image_init_data->image_obj = elm_image_add(grid);
        ecore_job_add(_image_content_setup, image_init_data);
     }
   else if (!strcmp(part, "elm.swallow.end"))
     {
        image_init_data->image_obj = elm_icon_add(grid);
        ecore_job_add(_image_usage_icon_setup, image_init_data);
     }

   return image_init_data->image_obj;
}

/* deletion callback */
static void
_grid_del(void *data,
          Evas_Object *obj __UNUSED__)
{
   Item *it = data;
   eina_stringshare_del(it->image_name);
   free(it);
}

static inline void
_image_info_update_usage_info(Image_Editor *img_edit, int list_count)
{
   Eina_Stringshare *title;
   if (list_count)
     title = eina_stringshare_printf(_("Usage (%d)"), list_count);
   else
     title = eina_stringshare_add(_("Usage ( - )"));

   elm_object_disabled_set(img_edit->usage_search_data.search_entry,
                           !list_count);
   elm_entry_entry_set(img_edit->usage_search_data.search_entry, "");
   img_edit->usage_search_data.last_item_found = NULL;

   ewe_tabs_item_title_set(img_edit->tabs,
                           img_edit->image_usage_fields.tab, title);
   eina_stringshare_del(title);
}

static void
_image_info_reset(Image_Editor *img_edit)
{
   if (img_edit->image_data_fields.image)
     {
        elm_object_part_content_unset(img_edit->layout, "eflete.swallow.image");
        evas_object_del(img_edit->image_data_fields.image);
        img_edit->image_data_fields.image = NULL;

        _image_info_update_usage_info(img_edit, 0);
        elm_genlist_clear(img_edit->image_usage_fields.genlist);
        img_edit->image_data_fields.image_name = NULL;
     }

   ewe_combobox_select_item_set(img_edit->image_data_fields.comp, 0);
   elm_entry_entry_set(img_edit->image_data_fields.file_name, "");
   elm_object_part_text_set(img_edit->image_data_fields.layout,
                            "text.location", " - ");
   elm_object_part_text_set(img_edit->image_data_fields.layout,
                            "text.type", " - ");
   elm_object_part_text_set(img_edit->image_data_fields.layout,
                            "text.width", " - ");
   elm_object_part_text_set(img_edit->image_data_fields.layout,
                            "text.height", " - ");
}

static void
_image_info_type_setup(Evas_Object *layout,
                       const char *image_name)
{
   char buf[BUFF_MAX];
   const char *dot = strrchr(image_name, '.');
   if ((!dot) || (dot == image_name))
     {
        elm_object_part_text_set(layout, "text.type", _("Unknown"));
        return;
     }

   if (eina_str_has_extension(image_name, ".png"))
     snprintf(buf, BUFF_MAX, _("PNG image (%s)"), dot);
   else if (eina_str_has_extension(image_name, ".gif"))
     snprintf(buf, BUFF_MAX, _("GIF image (%s)"), dot);
   else if (eina_str_has_extension(image_name, ".jpeg") ||
            eina_str_has_extension(image_name, ".jpg") ||
            eina_str_has_extension(image_name, ".jif") ||
            eina_str_has_extension(image_name, ".jfif"))
     snprintf(buf, BUFF_MAX, _("JPEG image (%s)"), dot);
   else if (eina_str_has_extension(image_name, ".bmp"))
     snprintf(buf, BUFF_MAX, _("BMP image (%s)"), dot);
   else if (eina_str_has_extension(image_name, ".tif") ||
            eina_str_has_extension(image_name, ".tiff"))
     snprintf(buf, BUFF_MAX, _("TIFF image (%s)"), dot);
   else
     {
        elm_object_part_text_set(layout, "text.type", _("Unknown"));
        return;
     }
   elm_object_part_text_set(layout, "text.type", buf);
}

static void
_image_info_location_setup(Image_Editor *img_edit,
                           const char *image_name)
{
   const char *file_name = strrchr(image_name, '/');
   if (!file_name) file_name = image_name;
   else file_name++;

   elm_entry_entry_set(img_edit->image_data_fields.file_name, file_name);
   char buf[BUFF_MAX];
   if (image_name == file_name)
     elm_object_part_text_set(img_edit->image_data_fields.layout,
                              "text.location", "-");
   else
     {
        strncpy(buf, image_name, (strlen(image_name) - strlen(file_name) - 1));
        elm_object_part_text_set(img_edit->image_data_fields.layout,
                                 "text.location", buf);
     }
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


static void
_image_info_usage_update(Image_Editor *img_edit)
{
   Eina_List *usage_list, *l;
   Evas_Object *list;
   Elm_Object_Item *it_group, *it_part;
   Edje_Part_Image_Use *image;
   Evas_Object *edje_edit_obj;
   const char *cur_group = NULL;
   const char *cur_part = NULL;
   Eina_Stringshare *state_name;

   if (!img_edit->image_data_fields.image_name) return;

   GET_OBJ(img_edit->pr, edje_edit_obj);

   usage_list =
      edje_edit_image_usage_list_get(edje_edit_obj,
                                     img_edit->image_data_fields.image_name,
                                     false);
   if (!eina_list_count(usage_list))
     {
        edje_edit_image_usage_list_free(usage_list);
        return;
     }

   genlist_item_classes_init();

   list = img_edit->image_usage_fields.genlist;

   EINA_LIST_FOREACH(usage_list, l, image)
     {
        if ((!cur_group) || (strcmp(cur_group, image->group)))
          {
             it_group = elm_genlist_item_append(list, _itc_group,
                                                eina_stringshare_add(image->group),
                                                NULL, ELM_GENLIST_ITEM_NONE,
                                                NULL, NULL);
             cur_group = image->group;
             cur_part = NULL;
          }
        if ((!cur_part) || (strcmp(cur_part, image->part)))
          {
             it_part = elm_genlist_item_append(list, _itc_part,
                                               eina_stringshare_add(image->part),
                                               it_group, ELM_GENLIST_ITEM_NONE,
                                               NULL, NULL);
             cur_part = image->part;
          }
        if ((cur_part) && (!strcmp(cur_part, image->part)))
          {
             state_name = eina_stringshare_printf("\"%s\" %.1f",
                                                  image->state.name,
                                                  image->state.value);
             elm_genlist_item_append(list, _itc_state, state_name,
                                     it_part, ELM_GENLIST_ITEM_NONE,
                                     NULL, NULL);
          }
       }

   edje_edit_image_usage_list_free(usage_list);
}

static void
_image_info_setup(Image_Editor *img_edit,
                  const Item* it)
{
   Evas_Object *edje_edit_obj = NULL;
   Eina_Stringshare *str;
   Evas_Object *image;
   Edje_Edit_Image_Comp comp;
   Eina_List *usage_list;
   int w, h;

   GET_OBJ(img_edit->pr, edje_edit_obj);

   _image_info_reset(img_edit);
   img_edit->image_data_fields.image_name = it->image_name;

   image = _image_editor_image_create(img_edit->layout, img_edit, it);
   evas_object_image_smooth_scale_set(image, false);
   elm_object_part_content_set(img_edit->layout, "eflete.swallow.image", image);
   img_edit->image_data_fields.image = image;
   evas_object_show(image);

   comp =  edje_edit_image_compression_type_get(edje_edit_obj, it->image_name);

   if (comp != EDJE_EDIT_IMAGE_COMP_USER)
     {
        str = eina_stringshare_printf("edje/images/%i", it->id);
        elm_object_part_text_set(img_edit->image_data_fields.layout,
                                 "text.location", str);
        elm_entry_entry_set(img_edit->image_data_fields.file_name, it->image_name);
        eina_stringshare_del(str);
     }
   else
     _image_info_location_setup(img_edit, it->image_name);

   ewe_combobox_select_item_set(img_edit->image_data_fields.comp, comp + 1);

   if (comp == EDJE_EDIT_IMAGE_COMP_LOSSY)
     {
        int quality = edje_edit_image_compression_rate_get(edje_edit_obj,
                                                           it->image_name);
        elm_spinner_value_set(img_edit->image_data_fields.quality, quality);
     }
   else
      elm_spinner_value_set(img_edit->image_data_fields.quality, 0);

   elm_image_object_size_get(image, &w, &h);
   str = eina_stringshare_printf("%d", w);
   elm_object_part_text_set(img_edit->image_data_fields.layout,
                            "text.width", str);
   eina_stringshare_del(str);
   str = eina_stringshare_printf("%d", h);
   elm_object_part_text_set(img_edit->image_data_fields.layout,
                            "text.height", str);
   eina_stringshare_del(str);

   _image_info_type_setup(img_edit->image_data_fields.layout, it->image_name);

   usage_list = edje_edit_image_usage_list_get(edje_edit_obj,
                                               it->image_name, false);
   _image_info_update_usage_info(img_edit, eina_list_count(usage_list));
   _image_info_usage_update(img_edit);
   edje_edit_image_usage_list_free(usage_list);
   evas_object_smart_calculate(img_edit->image_data_fields.layout);
}

static void
_search_reset_cb(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   Search_Data *search_data = data;
   search_data->last_item_found = NULL;
}

/* item selection change callback */
static void
_grid_sel(void *data,
          Evas_Object *obj __UNUSED__,
          void *event_info __UNUSED__)
{
   Image_Editor *img_edit = (Image_Editor *)data;
   const Eina_List* sel_list = elm_gengrid_selected_items_get(img_edit->gengrid);
   int selected_images_count = eina_list_count(sel_list);

   if (selected_images_count == 1)
     _image_info_setup(img_edit,
                       elm_object_item_data_get(eina_list_data_get(sel_list)));
   else
     _image_info_reset(img_edit);
}

static inline Item *
_image_editor_gengrid_item_data_create(Evas_Object *edje_edit_obj,
                                       const char *image_name)
{
   Item *it = (Item *)mem_malloc(sizeof(Item));
   it->image_name = eina_stringshare_add(image_name);
   it->id = edje_edit_image_id_get(edje_edit_obj, it->image_name);
   it->comp_type = edje_edit_image_compression_type_get(edje_edit_obj,
                                                        it->image_name);
   return it;
}

static void
_fs_del(void *data)
{
   Elm_Object_Item *it;
   Image_Editor *edit = (Image_Editor *)data;

   if (edit->fs_win)
     {
        evas_object_del(edit->fs_win);
        edit->fs_win = NULL;
        it = elm_gengrid_item_prev_get(edit->group_items.linked);
        elm_gengrid_item_selected_set(it, true);
     }
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
   Style *style = NULL;
   Uns_List *image = NULL;

   Image_Editor *img_edit = (Image_Editor *)data;

   if ((!selected) || (!strcmp(selected, "")))
     goto del;

   GET_STYLE(img_edit->pr, style);
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
        image = mem_malloc(sizeof(Uns_List));
        image->data = (void *)eina_stringshare_add(selected);
        image->act_type = ACTION_TYPE_ADD;
        img_edit->unapplied_list = eina_list_append(img_edit->unapplied_list,
                                                    image);

        it = (Item *)mem_malloc(sizeof(Item));
        it->image_name = eina_stringshare_add(selected);
        it->id = -1;
        item = elm_gengrid_item_insert_before(img_edit->gengrid, gic, it,
                                              img_edit->group_items.linked,
                                              _grid_sel, img_edit);
        elm_gengrid_item_selected_set(item, true);
     }
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

   edit->fs_win  = mw_add(NULL, NULL);
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
   Eina_List * in_use = NULL, *used_in = NULL;
   char *name;
   Edje_Part_Image_Use *item;
   Style *style = NULL;
   char buf[BUFF_MAX];
   int symbs = 0;
   Uns_List *image = NULL;
   Eina_List *used;

   if (!img_edit->gengrid) return;

   GET_STYLE(img_edit->pr, style);

   grid_list = (Eina_List *)elm_gengrid_selected_items_get(img_edit->gengrid);
   if (!grid_list) return;

   EINA_LIST_FOREACH_SAFE(grid_list, l, l2, grid_item)
     {
        it = elm_object_item_data_get(grid_item);
        used = edje_edit_image_usage_list_get(style->obj, it->image_name, EINA_TRUE);
        if (!used)
          {
             elm_object_item_del(grid_item);

             image = mem_malloc(sizeof(Uns_List));
             image->data = (void *)eina_stringshare_add(it->image_name);
             image->act_type = ACTION_TYPE_DEL;
             img_edit->unapplied_list = eina_list_append(img_edit->unapplied_list,
                                                         image);
          }
        else
          {
             notdeleted++;
             if (notdeleted < 4)
                in_use = eina_list_append(in_use, it->image_name);
             elm_gengrid_item_selected_set(grid_item, false);
          }
     }
   if (notdeleted == 1)
     {
        name = eina_list_nth(in_use, 0);
        used_in = edje_edit_image_usage_list_get(style->obj, name, false);
        snprintf(buf, BUFF_MAX, _("Image is used in:"));
        symbs = strlen(buf);
        EINA_LIST_FOREACH(used_in, l, item)
          {
             snprintf(buf + symbs, BUFF_MAX - symbs, _("<br>group: %s<br>part: %s<br>state: \"%s\" %2.1f"),
                      item->group, item->part, item->state.name, item->state.value);
             symbs+= strlen(name);
             break; TODO("remove this break after warning style remake")
          }
          edje_edit_image_usage_list_free(used_in);
        NOTIFY_WARNING("%s", buf);
     }
   else if (notdeleted >1)
     {
        snprintf(buf, BUFF_MAX, NGETTEXT("%d image in use:",
                                         "%d images in use:",
                                         notdeleted),
                 notdeleted);
        symbs = strlen(buf);
        EINA_LIST_FOREACH(in_use, l, name)
          {
             snprintf(buf + symbs, BUFF_MAX - symbs, "<br>%s", name);
             symbs+= strlen(name);
          }
        if (notdeleted >= 4)
           snprintf(buf + symbs, BUFF_MAX - symbs, "<br>...");
        NOTIFY_WARNING("%s", buf);
     }
   eina_list_free(in_use);
}

static void
_on_button_ok_clicked_cb(void *data,
                         Evas_Object *obj __UNUSED__,
                         void *event_info __UNUSED__)
{
   Image_Editor *img_edit = (Image_Editor *)data;
   Eina_Bool multiselect = false;
   const Eina_List *items;
   Eina_List *l, *names = NULL;
   Elm_Object_Item *it;
   const Item *item = NULL;
   char *ei;

   if (!img_edit->gengrid)
     {
        _image_editor_del(img_edit);
        return;
     }

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
            WIN_NOTIFY_WARNING(img_edit->win, _("Image not selected"));
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
        ei = strdup(item->image_name);
        evas_object_smart_callback_call(img_edit->win, SIG_IMAGE_SELECTED, ei);
        free(ei);
     }
   else
     evas_object_smart_callback_call(img_edit->win, SIG_IMAGE_SELECTED,
                                     (Eina_List *) names);

   _image_editor_del(img_edit);
}

static void
_on_button_close_clicked_cb(void *data,
                            Evas_Object *obj __UNUSED__,
                            void *event_info __UNUSED__)
{
   Image_Editor *img_edit = (Image_Editor *)data;
   eina_list_free(img_edit->unapplied_list);
   _image_editor_del(img_edit);
}

static void
_on_button_apply_clicked_cb(void *data,
                            Evas_Object *obj __UNUSED__,
                            void *event_info __UNUSED__)
{
   Image_Editor *img_edit = (Image_Editor *)data;
   Eina_List *l;
   Uns_List *it = NULL;
   App_Data *ap = app_data_get();
   Style *style = NULL;
   GET_STYLE(img_edit->pr, style);

   EINA_LIST_FOREACH(img_edit->unapplied_list, l, it)
     {
        if (it->act_type == ACTION_TYPE_DEL)
          {
             if (edje_edit_image_del(style->obj, it->data))
               ap->project->nsimage_list = eina_list_append(ap->project->nsimage_list, it);
          }
        else if (edje_edit_image_add(style->obj, it->data))
          ap->project->nsimage_list = eina_list_append(ap->project->nsimage_list, it);
     }
   pm_save_to_dev(img_edit->pr, style, false);
   eina_list_free(img_edit->unapplied_list);
   project_changed(false);
   _image_editor_del(img_edit);
}

ITEM_SEARCH_FUNC(genlist, ELM_GENLIST_ITEM_SCROLLTO_MIDDLE, NULL)
ITEM_SEARCH_FUNC(gengrid, ELM_GENGRID_ITEM_SCROLLTO_MIDDLE, NULL)

static void
_on_images_search_entry_changed_cb(void *data,
                                   Evas_Object *obj __UNUSED__,
                                   void *event_info __UNUSED__)
{
   Image_Editor *img_edit = data;
   _gengrid_item_search(img_edit->gengrid, &(img_edit->image_search_data),
                        img_edit->image_search_data.last_item_found);
}

static void
_on_usage_search_entry_changed_cb(void *data,
                                  Evas_Object *obj __UNUSED__,
                                  void *event_info __UNUSED__)
{
   Image_Editor *img_edit = data;
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

   if (img_edit->usage_search_data.last_item_found)
     {
        start_from =
           elm_genlist_item_next_get(img_edit->usage_search_data.last_item_found);
     }

   _genlist_item_search(img_edit->image_usage_fields.genlist,
                        &(img_edit->usage_search_data), start_from);
}

static void
_search_next_gengrid_item_cb(void *data,
                             Evas_Object *obj __UNUSED__,
                             void *event_info __UNUSED__)
{
   Image_Editor *img_edit = data;
   Elm_Object_Item *start_from = NULL;

   if (img_edit->image_search_data.last_item_found)
     {
        start_from =
           elm_gengrid_item_next_get(img_edit->image_search_data.last_item_found);
     }

   _gengrid_item_search(img_edit->gengrid, &(img_edit->image_search_data),
                        start_from);
}

static void
_image_editor_gengrid_group_items_add(Image_Editor *img_edit)
{
   Elm_Gengrid_Item_Class *ggic = elm_gengrid_item_class_new();

   ggic->item_style = "group_index";
   ggic->func.text_get = _grid_group_item_label_get;
   ggic->func.content_get = NULL;
   ggic->func.state_get = NULL;
   ggic->func.del = _grid_group_item_del;

   img_edit->group_items.included =
      elm_gengrid_item_append(img_edit->gengrid, ggic,
      eina_stringshare_add(_("<b>Include</b>")), NULL, NULL);
   img_edit->group_items.linked =
      elm_gengrid_item_append(img_edit->gengrid, ggic,
      eina_stringshare_add(_("<b>Linked</b>")), NULL, NULL);

   TODO("Add View group with images that are not included to the edj-file yet.")

   elm_gengrid_item_class_free(ggic);
}

static Evas_Object *
_image_info_box_create(Image_Editor *img_edit)
{
   Evas_Object *scroller;
   SCROLLER_ADD(img_edit->layout, scroller);
   Evas_Object *layout = elm_layout_add(scroller);
   elm_layout_theme_set(layout, "layout", "image_editor", "image_info");
   evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(layout, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(layout);

   EWE_ENTRY_ADD(layout, img_edit->image_data_fields.file_name, true);
   elm_entry_editable_set(img_edit->image_data_fields.file_name, false);
   elm_object_part_content_set(layout, "eflete.swallow.image_info.name",
                               img_edit->image_data_fields.file_name);

   EWE_COMBOBOX_ADD(layout, img_edit->image_data_fields.comp);
   ewe_combobox_item_add(img_edit->image_data_fields.comp, "NONE");
   ewe_combobox_item_add(img_edit->image_data_fields.comp, "RAW");
   ewe_combobox_item_add(img_edit->image_data_fields.comp, "USER");
   ewe_combobox_item_add(img_edit->image_data_fields.comp, "COMP");
   ewe_combobox_item_add(img_edit->image_data_fields.comp, "LOSSY");
   elm_object_disabled_set(img_edit->image_data_fields.comp, true);
   elm_object_part_content_set(layout, "eflete.swallow.compression_type",
                               img_edit->image_data_fields.comp);

   SPINNER_ADD(layout, img_edit->image_data_fields.quality, 0, 100, 1, false);
   elm_object_disabled_set(img_edit->image_data_fields.quality, true);
   elm_object_part_content_set(layout, "eflete.swallow.compression_quality",
                               img_edit->image_data_fields.quality);

   elm_object_content_set(scroller, layout);
   img_edit->image_data_fields.layout = layout;
   _image_info_reset(img_edit);
   return scroller;
}

static inline Evas_Object *
_image_editor_search_field_create(Evas_Object *parent)
{
   Evas_Object *entry, *icon;
   ENTRY_ADD(parent, entry, true);
   elm_object_style_set(entry, "eflete/search_field");
   elm_object_part_text_set(entry, "guide", _("Search"));
   ICON_ADD(entry, icon, true, "icon-search");
   elm_object_part_content_set(entry, "elm.swallow.end", icon);
   return entry;
}

static Evas_Object *
_image_usage_layout_create(Image_Editor *img_edit, Evas_Object *parent)
{
   Evas_Object *layout = NULL;
   Evas_Object *genlist = NULL;
   Evas_Object *entry = NULL;

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
   evas_object_smart_callback_add(img_edit->image_usage_fields.genlist,
                                  "pressed", _search_reset_cb,
                                  &(img_edit->usage_search_data));
   img_edit->usage_search_data.search_entry = entry;
   img_edit->usage_search_data.last_item_found = NULL;
   return layout;
}


static void
_image_info_initiate(Image_Editor *img_edit)
{
   Ewe_Tabs_Item *it = NULL;

   img_edit->tabs = ewe_tabs_add(img_edit->layout);
   elm_object_style_set(img_edit->tabs, "bookmark");

   it = ewe_tabs_item_append(img_edit->tabs, NULL, "Image Info", "bookmark");
   ewe_tabs_item_content_set(img_edit->tabs, it,
                             _image_info_box_create(img_edit));
   it = ewe_tabs_item_append(img_edit->tabs, NULL, "", "bookmark");
   ewe_tabs_item_content_set(img_edit->tabs, it,
                             _image_usage_layout_create(img_edit,
                                                        img_edit->layout));
   img_edit->image_usage_fields.tab = it;

   elm_object_part_content_set(img_edit->layout,
                               "eflete.swallow.image_info", img_edit->tabs);

   _image_info_update_usage_info(img_edit, 0);
   evas_object_show(img_edit->tabs);
}

Eina_Bool
_image_editor_init(Image_Editor *img_edit)
{
   Eina_List *l = NULL;
   Item *it = NULL;
   const char* image_name = NULL;
   Eina_List *images = NULL;
   int counter = 0;
   Evas_Object *edje_edit_obj = NULL;

   if (!img_edit) return false;
   GET_OBJ(img_edit->pr, edje_edit_obj);

   _image_editor_gengrid_group_items_add(img_edit);
   images = edje_edit_images_list_get(edje_edit_obj);

   if (images)
     {
        EINA_LIST_FOREACH(images, l, image_name)
           {
              counter++;
              if (!image_name)
                {
                   ERR("name not found for image #%d",counter);
                   continue;
                }
              it = _image_editor_gengrid_item_data_create(edje_edit_obj,
                                                          image_name);
              if (it->comp_type == EDJE_EDIT_IMAGE_COMP_USER)
                elm_gengrid_item_insert_before(img_edit->gengrid, gic, it,
                                        img_edit->group_items.included,
                                        _grid_sel, img_edit);
              else
                elm_gengrid_item_insert_before(img_edit->gengrid, gic, it,
                                               img_edit->group_items.linked,
                                               _grid_sel, img_edit);
           }
         elm_gengrid_item_bring_in(elm_gengrid_first_item_get(img_edit->gengrid),
                                   ELM_GENGRID_ITEM_SCROLLTO_TOP);
         edje_edit_string_list_free(images);
     }
   elm_scroller_policy_set(img_edit->gengrid, ELM_SCROLLER_POLICY_OFF,
                           ELM_SCROLLER_POLICY_AUTO);
   evas_object_smart_calculate(img_edit->gengrid);

   return true;
}

Evas_Object *
image_editor_window_add(Project *project, Image_Editor_Mode mode)
{
   Evas_Object *button;
   Evas_Object *_bg = NULL;
   Evas_Object *ic = NULL;
   Evas_Object *search_entry = NULL;
   /* temporary solution, while it not moved to modal window */
   App_Data *ap = app_data_get();

   if (!project)
     {
        ERR("Project isn't opened");
        return NULL;
     }

   Image_Editor *img_edit = (Image_Editor *)mem_calloc(1, sizeof(Image_Editor));
   img_edit->pr = project;

   img_edit->win = mw_add(_on_button_close_clicked_cb, img_edit);
   if (!img_edit->win)
     {
        free(img_edit);
        return NULL;
     }
   if (mode == SINGLE)
     mw_title_set(img_edit->win, _("Image manager: choose image"));
   else if (mode == TWEENS)
     mw_title_set(img_edit->win, _("Image manager: select tween images"));
   else if (mode == MULTIPLE)
     mw_title_set(img_edit->win, _("Image manager"));

   ic = elm_icon_add(img_edit->win);
   elm_icon_standard_set(ic, "image");
   mw_icon_set(img_edit->win, ic);

   img_edit->layout = elm_layout_add(img_edit->win);
   elm_layout_theme_set(img_edit->layout, "layout", "image_editor", "default");
   elm_win_inwin_content_set(img_edit->win, img_edit->layout);

   img_edit->gengrid = elm_gengrid_add(img_edit->layout);
   elm_object_part_content_set(img_edit->layout,
                               "eflete.swallow.grid", img_edit->gengrid);
   elm_gengrid_item_size_set(img_edit->gengrid, ITEM_WIDTH, ITEM_HEIGHT);
   elm_gengrid_group_item_size_set(img_edit->gengrid, GROUP_ITEM_WIDTH, GROUP_ITEM_HEIGHT);
   elm_gengrid_align_set(img_edit->gengrid, 0.0, 0.0);
   elm_scroller_policy_set(img_edit->gengrid, ELM_SCROLLER_POLICY_OFF,
                           ELM_SCROLLER_POLICY_OFF);

   GET_IMAGE(_bg, img_edit->gengrid, "gallery-bg");
   elm_object_part_content_set(img_edit->gengrid, "elm.swallow.background", _bg);
   evas_object_show(_bg);

   if (mode == SINGLE)
     {
       elm_gengrid_multi_select_set(img_edit->gengrid, false);
       evas_object_smart_callback_add(img_edit->gengrid, "clicked,double",
                                      _on_button_ok_clicked_cb, img_edit);
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
        evas_object_smart_callback_add(img_edit->gengrid, "clicked,double",
                                       _on_button_ok_clicked_cb, img_edit);
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
   evas_object_smart_callback_add(img_edit->gengrid, "pressed", _search_reset_cb,
                                  &(img_edit->image_search_data));
   img_edit->image_search_data.search_entry = search_entry;
   img_edit->image_search_data.last_item_found = NULL;

   _image_info_initiate(img_edit);

   if (mode != MULTIPLE)
     {
        BUTTON_ADD(img_edit->layout, button, _("Ok"));
        evas_object_smart_callback_add(button, "clicked", _on_button_ok_clicked_cb,
                                       img_edit);
        elm_object_part_content_set(img_edit->layout,
                                    "eflete.swallow.ok_btn", button);
     }

   BUTTON_ADD(img_edit->layout, button, _("Close"));
   evas_object_smart_callback_add(button, "clicked", _on_button_close_clicked_cb,
                                  img_edit);
   elm_object_part_content_set(img_edit->layout,
                               "eflete.swallow.close_btn", button);

   BUTTON_ADD(img_edit->layout, button, _("Apply"));
   evas_object_smart_callback_add(button, "clicked", _on_button_apply_clicked_cb,
                                  img_edit);
   elm_object_part_content_set(img_edit->layout,
                               "eflete.swallow.ok_btn", button);

   if (!gic)
     {
        gic = elm_gengrid_item_class_new();
        gic->item_style = "default";
        gic->func.text_get = _grid_label_get;
        gic->func.content_get = _grid_content_get;
        gic->func.del = _grid_del;
     }

   evas_object_show(img_edit->win);
   elm_object_focus_set(search_entry, true);
   if (!_image_editor_init(img_edit))
     {
        _image_editor_del(img_edit);
        ERR("Filed initialize image editor");
        return NULL;
     }
   evas_object_data_set(img_edit->gengrid, IMG_EDIT_KEY, img_edit);
   evas_object_data_set(img_edit->win, IMG_EDIT_KEY, img_edit);

   ui_menu_items_list_disable_set(ap->menu, MENU_ITEMS_LIST_MAIN, true);
   evas_object_event_callback_add(img_edit->win, EVAS_CALLBACK_DEL, _on_mwin_del, img_edit);

   ap->modal_editor++;
   return img_edit->win;
}

Eina_Bool
image_editor_file_choose(Evas_Object *win, const char *selected)
{
   Elm_Object_Item *grid_item = NULL;
   const Item* it = NULL;
   Image_Editor *img_edit = NULL;

   if (!selected) return false;

   if (!win)
     {
        ERR("Expecting Image editor window.");
        return false;
     }

   img_edit = evas_object_data_get(win, IMG_EDIT_KEY);
   if (!img_edit)
     {
        ERR("Image editor does'nt exist");
        return false;
     }

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
