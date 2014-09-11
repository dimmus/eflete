/**
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

#define ITEM_WIDTH 100
#define ITEM_HEIGHT 115
#define IMG_EDIT_KEY "image_editor_key"

typedef struct _Image_Editor Image_Editor;
typedef struct _Item Item;

struct _Item
{
   int id;
   const char* image_name;
   Edje_Edit_Image_Comp comp_type;
};

struct _Image_Editor
{
   Project *pr;
   Evas_Object *win;
   Evas_Object *gengrid;
   Evas_Object *layout;
   Evas_Object *legend;
   struct {
      Evas_Object *layout;
      Evas_Object *image;
      Evas_Object *file_name;
      Evas_Object *location;
      Evas_Object *type;
      Evas_Object *comp;
      Evas_Object *quality;
      Evas_Object *width;
      Evas_Object *height;
   } image_data_fields;
   struct {
      Evas_Smart_Cb choose_func;
      void *data;
   } func_data;
   struct {
      Elm_Object_Item *included;
      Elm_Object_Item *linked;
   } group_items;
};

static Elm_Gengrid_Item_Class *gic = NULL;
static void _image_info_reset(Image_Editor *img_edit);

static char *
_grid_label_get(void *data,
                Evas_Object *obj __UNUSED__,
                const char  *part __UNUSED__)
{
   const Item *it = data;
   return strdup(it->image_name);
}

static void
_image_editor_del(Image_Editor *img_edit)
{
   img_edit->pr = NULL;
   elm_gengrid_item_class_free(gic);
   gic = NULL;
   evas_object_data_del(img_edit->win, IMG_EDIT_KEY);
   evas_object_data_del(img_edit->gengrid, IMG_EDIT_KEY);
   evas_object_del(img_edit->legend);
   evas_object_del(img_edit->gengrid);
   evas_object_del(img_edit->win);
   _image_info_reset(img_edit);
   free(img_edit);
}

/* icon fetching callback */
#define MAX_ICON_SIZE 16
static Evas_Object *
_grid_content_get(void *data,
                  Evas_Object *obj,
                  const char  *part)
{
   Item *it = data;
   Evas_Object *grid = (Evas_Object *)obj;
   Image_Editor *img_edit = evas_object_data_get(grid, IMG_EDIT_KEY);
   char buf[BUFF_MAX];
   Evas_Object *image = NULL;

   if (!strcmp(part, "elm.swallow.icon"))
     {
        image = elm_image_add(grid);
        snprintf(buf, BUFF_MAX, "edje/images/%i", it->id);
        elm_image_file_set(image, img_edit->pr->swapfile, buf);

        int w, h;
        elm_image_object_size_get(image, &w, &h);
        if ((w < MAX_ICON_SIZE) && (h < MAX_ICON_SIZE))
          evas_object_size_hint_max_set(image, MAX_ICON_SIZE, MAX_ICON_SIZE);
        else
          elm_image_resizable_set(image, false, true);
        evas_object_show(image);
        return image;
     }
   return NULL;
}
#undef MAX_ICON_SIZE

/* deletion callback */
static void
_grid_del(void *data,
          Evas_Object *obj __UNUSED__)
{
   Item *it = data;
   eina_stringshare_del(it->image_name);
   free(it);
}

static void
_image_info_reset(Image_Editor *img_edit)
{
   if (!img_edit->image_data_fields.image)
     return;

   elm_object_part_content_unset(img_edit->image_data_fields.layout,
                                 "eflete.swallow.image");
   evas_object_del(img_edit->image_data_fields.image);
   img_edit->image_data_fields.image = NULL;

   ewe_combobox_select_item_set(img_edit->image_data_fields.comp, 0);
   elm_entry_entry_set(img_edit->image_data_fields.file_name, "");
   elm_entry_entry_set(img_edit->image_data_fields.location, "");
   elm_entry_entry_set(img_edit->image_data_fields.type, "");
   elm_entry_entry_set(img_edit->image_data_fields.width, "");
   elm_entry_entry_set(img_edit->image_data_fields.height, "");
}

static void
_image_info_type_setup(Evas_Object *entry,
                       const char *image_name)
{
  char buf[BUFF_MAX];
   const char *dot = strrchr(image_name, '.');
   if ((!dot) || (dot == image_name))
     {
        elm_entry_entry_set(entry, "Unknown");
        return;
     }

   if (eina_str_has_extension(image_name, ".png"))
     snprintf(buf, BUFF_MAX, "PNG image (%s)", dot);
   else if (eina_str_has_extension(image_name, ".gif"))
     snprintf(buf, BUFF_MAX, "GIF image (%s)", dot);
   else if (eina_str_has_extension(image_name, ".jpeg") ||
            eina_str_has_extension(image_name, ".jpg") ||
            eina_str_has_extension(image_name, ".jif") ||
            eina_str_has_extension(image_name, ".jfif"))
     snprintf(buf, BUFF_MAX, "JPEG image (%s)", dot);
   else if (eina_str_has_extension(image_name, ".bmp"))
     snprintf(buf, BUFF_MAX, "BMP image (%s)", dot);
   else if (eina_str_has_extension(image_name, ".tif") ||
            eina_str_has_extension(image_name, ".tiff"))
     snprintf(buf, BUFF_MAX, "TIFF image (%s)", dot);
   else
     {
        elm_entry_entry_set(entry, "Unknown");
        return;
     }
   elm_entry_entry_set(entry, buf);
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
   strncpy(buf, image_name, (strlen(image_name) - strlen(file_name) - 1));
   elm_entry_entry_set(img_edit->image_data_fields.location, buf);
}

static void
_image_info_setup(Image_Editor *img_edit,
                  const Item* it)
{
   Evas_Object *edje_edit_obj = NULL;
   char buf[BUFF_MAX];
   GET_OBJ(img_edit->pr, edje_edit_obj);

   _image_info_reset(img_edit);

   Evas_Object *image = elm_image_add(img_edit->layout);
   snprintf(buf, BUFF_MAX, "edje/images/%i", it->id);
   elm_image_file_set(image, img_edit->pr->swapfile, buf);
   evas_object_image_smooth_scale_set(image, false);
   elm_object_part_content_set(img_edit->image_data_fields.layout,
                               "eflete.swallow.image", image);
   img_edit->image_data_fields.image = image;
   evas_object_show(image);

   Edje_Edit_Image_Comp comp =
      edje_edit_image_compression_type_get(edje_edit_obj, it->image_name);

   if (comp != EDJE_EDIT_IMAGE_COMP_USER)
     {
        elm_entry_entry_set(img_edit->image_data_fields.location, buf);
        elm_entry_entry_set(img_edit->image_data_fields.file_name, it->image_name);
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

   int w, h;
   elm_image_object_size_get(image, &w, &h);
   snprintf(buf, BUFF_MAX, "%d", w);
   elm_entry_entry_set(img_edit->image_data_fields.width, buf);
   snprintf(buf, BUFF_MAX, "%d", h);
   elm_entry_entry_set(img_edit->image_data_fields.height, buf);
   _image_info_type_setup(img_edit->image_data_fields.type, it->image_name);
}

/* item selection change callback */
static void
_grid_sel(void *data,
          Evas_Object *obj __UNUSED__,
          void *event_info __UNUSED__)
{
   char buf[BUFF_MAX];

   Image_Editor *img_edit = (Image_Editor *)data;
   const Eina_List* sel_list = elm_gengrid_selected_items_get(img_edit->gengrid);
   int selected_images_count = eina_list_count(sel_list);

   if (!selected_images_count)
     elm_object_text_set(img_edit->legend, _("No images selected"));
   else
     {
        snprintf(buf, BUFF_MAX, ngettext("%d image selected",
                                         "%d images selected",
                                         selected_images_count),
                 selected_images_count);
        elm_object_text_set(img_edit->legend, buf);
     }

   if (selected_images_count == 1)
     _image_info_setup(img_edit, elm_object_item_data_get(eina_list_data_get(sel_list)));
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
_on_image_done(void *data,
               Evas_Object *obj,
               void *event_info)
{
   Item *it = NULL;
   Evas_Object *edje_edit_obj = NULL;
   const char *selected = event_info;

   if ((!data) || (!selected) || (!strcmp(selected, "")))
     {
        ecore_main_loop_quit();
        return;
     }

   Image_Editor *img_edit = (Image_Editor *)data;

   GET_OBJ(img_edit->pr, edje_edit_obj);

   if ((ecore_file_exists(selected)) && (!ecore_file_is_dir(selected)))
     {
        if (!edje_edit_image_add(edje_edit_obj, selected))
          {
             WIN_NOTIFY_ERROR(obj,
                              _("Error while loading file.<br>"
                                "Please check if file is image"
                                "or/and file is accessible."));
             return;
          }
        else
          {
             it = _image_editor_gengrid_item_data_create(edje_edit_obj,
                                                         ecore_file_file_get(selected));
             elm_gengrid_item_insert_before(img_edit->gengrid, gic, it,
                                            img_edit->group_items.linked,
                                            _grid_sel, img_edit);
             pm_project_changed(app_data_get()->project);
          }
     }
   else
     {
        WIN_NOTIFY_ERROR(obj, _("Error while loading file.<br>File is not exist"));
        return;
     }

   ecore_main_loop_quit();
}

static void
_on_button_add_clicked_cb(void *data,
                         Evas_Object *obj __UNUSED__,
                         void *event_info __UNUSED__)
{
   Evas_Object *win, *bg, *fs;

   MODAL_WINDOW_ADD(win, main_window_get(), _("Add image to library"), _on_image_done, NULL);
   bg = elm_bg_add(win);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);
   elm_win_resize_object_add(win, bg);

   FILESELECTOR_ADD(fs, win, _on_image_done, data);
   elm_win_resize_object_add(win, fs);

   ecore_main_loop_begin();

   evas_object_del(win);

   return;
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
   int deleted = 0, notdeleted = 0;
   Eina_List * in_use = NULL, *used_in = NULL;
   char *name;
   Edje_Part_Image_Use *item;
   Evas_Object *edje_edit_obj = NULL;
   char buf[BUFF_MAX];
   int symbs = 0;
   int images_to_del = 0;

   if (!img_edit->gengrid) return;

   GET_OBJ(img_edit->pr, edje_edit_obj);

   grid_list = (Eina_List *)elm_gengrid_selected_items_get(img_edit->gengrid);
   if (!grid_list) return;
   images_to_del = eina_list_count(grid_list);

   EINA_LIST_FOREACH_SAFE(grid_list, l, l2, grid_item)
     {
        it = elm_object_item_data_get(grid_item);
        if (edje_edit_image_del(edje_edit_obj, it->image_name))
          {
             deleted++;
             elm_object_item_del(grid_item);
          }
        else
          {
             notdeleted++;
             if (notdeleted < 4)
                in_use = eina_list_append(in_use, it->image_name);
             elm_gengrid_item_selected_set(grid_item, false);
          }
     }
   if (notdeleted < images_to_del)
     pm_project_changed(app_data_get()->project);
   if (notdeleted == 1)
     {
        EINA_LIST_FOREACH(in_use, l, name)
           used_in = edje_edit_image_usage_list_get(edje_edit_obj, name, false);
        snprintf(buf, BUFF_MAX, _("Images is used in:"));
        symbs = strlen(buf);
        EINA_LIST_FOREACH(used_in, l, item)
          {
             snprintf(buf + symbs, BUFF_MAX - symbs, _("<br>group: %s<br>part: %s<br>state: \"%s\" %2.1f"),
                      item->group, item->part, item->state.name, item->state.value);
             symbs+= strlen(name);
             break; //TODO: remove this break after warning style remake
          }
          edje_edit_image_usage_list_free(used_in);
        NOTIFY_WARNING("%s", buf);
     }
   else if (notdeleted >1)
     {
        snprintf(buf, BUFF_MAX, ngettext("%d image in use:",
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

   if (img_edit->func_data.choose_func)
     {
        if (!multiselect)
          img_edit->func_data.choose_func(img_edit->func_data.data, img_edit->win,
                                        (char *)item->image_name);
        else
          img_edit->func_data.choose_func(img_edit->func_data.data, img_edit->win,
                                        (Eina_List *)names);
        img_edit->func_data.choose_func = NULL;
        img_edit->func_data.data = NULL;
     }
   _image_editor_del(img_edit);

}

static void
_on_button_cancel_clicked_cb(void *data,
                             Evas_Object *obj __UNUSED__,
                             void *event_info __UNUSED__)
{
   Image_Editor *img_edit = (Image_Editor *)data;
   _image_editor_del(img_edit);
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
                           const char  *part __UNUSED__)
{
   return strdup(data);
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
      eina_stringshare_add("Include"), NULL, NULL);
   img_edit->group_items.linked =
      elm_gengrid_item_append(img_edit->gengrid, ggic,
      eina_stringshare_add("Linked"), NULL, NULL);
   //TODO: Add View group with images that are not included to the edj-file yet.

   elm_gengrid_item_class_free(ggic);
}

static Evas_Object *
_image_info_entry_add(Evas_Object *box,
                      const char *label)
{
   Evas_Object *item = NULL;
   Evas_Object *entry = NULL;

   ITEM_ADD(box, item, label, "eflete/image_editor/item/default");
   EWE_ENTRY_ADD(item, entry, true, DEFAULT_STYLE);
   elm_entry_editable_set(entry, false);
   elm_object_part_content_set(item, "elm.swallow.content", entry);
   elm_box_pack_end(box, item);
   return entry;
}

static Evas_Object *
_image_info_box_create(Image_Editor *img_edit, Evas_Object *parent)
{
   Evas_Object *item = NULL;
   Evas_Object *box = NULL;
   Evas_Object *groups_box = NULL;

   BOX_ADD(parent, box, false, false);
   elm_box_align_set(box, 0.0, 0.0);

   img_edit->image_data_fields.file_name =
      _image_info_entry_add(box, _("file name"));
   img_edit->image_data_fields.location =
      _image_info_entry_add(box, _("location"));
   img_edit->image_data_fields.type =
      _image_info_entry_add(box, _("type"));
   // TODO: Add field with file size info.

   //image compression
   Evas_Object *item_quality = NULL;

   ITEM_ADD(box, item, _("compression"), "eflete/image_editor/item/default");
   BOX_ADD(box, groups_box, true, true);
   EWE_COMBOBOX_ADD(groups_box, img_edit->image_data_fields.comp);
   ewe_combobox_item_add(img_edit->image_data_fields.comp, "NONE");
   ewe_combobox_item_add(img_edit->image_data_fields.comp, "RAW");
   ewe_combobox_item_add(img_edit->image_data_fields.comp, "USER");
   ewe_combobox_item_add(img_edit->image_data_fields.comp, "COMP");
   ewe_combobox_item_add(img_edit->image_data_fields.comp, "LOSSY");
   elm_object_disabled_set(img_edit->image_data_fields.comp, true);
   elm_box_pack_end(groups_box, img_edit->image_data_fields.comp);

   ITEM_ADD(item, item_quality, _("quality"), "eflete/image_editor/item/default");
   SPINNER_ADD(groups_box, img_edit->image_data_fields.quality,
               0, 100, 1, false, DEFAULT_STYLE);
   elm_object_disabled_set(img_edit->image_data_fields.quality, true);
   elm_object_part_content_set(item_quality, "elm.swallow.content",
                               img_edit->image_data_fields.quality);
   elm_box_pack_end(groups_box, item_quality);
   elm_object_part_content_set(item, "elm.swallow.content", groups_box);
   elm_box_pack_end(box, item);

   // Image size
   ITEM_ADD(box, item, _("image"), "eflete/image_editor/item/default");
   BOX_ADD(box, groups_box, true, true);
   img_edit->image_data_fields.width =
      _image_info_entry_add(groups_box, _("width"));
   img_edit->image_data_fields.height =
      _image_info_entry_add(groups_box, _("height"));
   elm_object_part_content_set(item, "elm.swallow.content", groups_box);
   elm_box_pack_end(box, item);

   return box;
}


static void
_image_info_initiate(Image_Editor *img_edit)
{
   Evas_Object *scroller = NULL;
   Evas_Object *box = NULL;

   SCROLLER_ADD(img_edit->layout, scroller);
   evas_object_size_hint_weight_set(scroller,
                                    EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_object_part_content_set(img_edit->layout,
                               "eflete.swallow.image_info", scroller);

   BOX_ADD(scroller, box, false, false);
   elm_object_content_set(scroller, box);
   elm_box_align_set(box, 0.5, 0.0);

   img_edit->image_data_fields.layout = elm_layout_add(box);
   elm_layout_file_set(img_edit->image_data_fields.layout, EFLETE_EDJ,
                       "eflete/image_editor/image_preview");
   evas_object_size_hint_weight_set(img_edit->image_data_fields.layout,
                                    EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(img_edit->image_data_fields.layout);

   elm_box_pack_end(box, img_edit->image_data_fields.layout);
   elm_box_pack_end(box, _image_info_box_create(img_edit, scroller));
   evas_object_show(box);
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
                elm_gengrid_item_append(img_edit->gengrid, gic, it,
                                        _grid_sel, img_edit);
              else
                elm_gengrid_item_insert_before(img_edit->gengrid, gic, it,
                                               img_edit->group_items.linked,
                                               _grid_sel, img_edit);
           }
         elm_gengrid_item_bring_in(elm_gengrid_first_item_get(img_edit->gengrid),
                                   ELM_GENGRID_ITEM_SCROLLTO_TOP);
         eina_list_free(images);
     }
   elm_scroller_policy_set(img_edit->gengrid, ELM_SCROLLER_POLICY_OFF,
                           ELM_SCROLLER_POLICY_AUTO);
   return true;
}

static void
_on_mwin_del(void * data,
             Evas *e __UNUSED__,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   ui_menu_locked_set(ap->menu_hash, false);
}

Evas_Object *
image_editor_window_add(Project *project, Image_Editor_Mode mode)
{
   Evas_Object *button;
   Evas_Object *_bg = NULL;
   Evas_Object *icon = NULL;
   /* temporary solution, while it not moved to modal window */
   App_Data *ap = app_data_get();

   if (!project)
     {
        ERR("Project doesn't opened");
        return NULL;
     }

   Image_Editor *img_edit = (Image_Editor *)mem_calloc(1, sizeof(Image_Editor));
   img_edit->pr = project;

   img_edit->win = mw_add(NULL, img_edit);
   mw_title_set(img_edit->win, _("Image editor"));

   img_edit->layout = elm_layout_add(img_edit->win);
   elm_layout_file_set(img_edit->layout, EFLETE_EDJ, "eflete/image_editor/default");
   elm_win_inwin_content_set(img_edit->win, img_edit->layout);
   img_edit->gengrid = elm_gengrid_add(img_edit->layout);
   elm_object_style_set(img_edit->gengrid, DEFAULT_STYLE);
   elm_object_part_content_set(img_edit->layout, "eflete.swallow.grid", img_edit->gengrid);
   elm_gengrid_item_size_set(img_edit->gengrid, ITEM_WIDTH, ITEM_HEIGHT);
   elm_gengrid_align_set(img_edit->gengrid, 0.5, 0.0);
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
   else
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

   BUTTON_ADD(img_edit->layout, button, NULL);
   ICON_ADD(button, icon, true, "icon-add");
   elm_object_part_content_set(button, NULL, icon);
   evas_object_smart_callback_add(button, "clicked",
                                  _on_button_add_clicked_cb, img_edit);
   elm_object_part_content_set(img_edit->layout,
                               "eflete.swallow.add_btn", button);

   BUTTON_ADD(img_edit->layout, button, NULL);
   ICON_ADD(button, icon, true, "icon-remove");
   elm_object_part_content_set(button, NULL, icon);
   evas_object_smart_callback_add(button, "clicked",
                                  _on_button_delete_clicked_cb, img_edit);
   elm_object_part_content_set(img_edit->layout,
                               "eflete.swallow.del_btn", button);

   LABEL_ADD(img_edit->layout, img_edit->legend, _("No images selected"))
   elm_object_part_content_set(img_edit->layout,
                               "eflete.swallow.label", img_edit->legend);

   BUTTON_ADD(img_edit->layout, button, _("Ok"));
   evas_object_smart_callback_add(button, "clicked", _on_button_ok_clicked_cb,
                                  img_edit);
   elm_object_part_content_set(img_edit->layout,
                               "eflete.swallow.OK_btn", button);

   BUTTON_ADD(img_edit->layout, button, _("Cancel"));
   evas_object_smart_callback_add(button, "clicked", _on_button_cancel_clicked_cb,
                                  img_edit);
   elm_object_part_content_set(img_edit->layout,
                               "eflete.swallow.Cancel_btn", button);

   _image_info_initiate(img_edit);

   if (!gic)
     {
        gic = elm_gengrid_item_class_new();
        gic->item_style = DEFAULT_STYLE;
        gic->func.text_get = _grid_label_get;
        gic->func.content_get = _grid_content_get;
        gic->func.del = _grid_del;
     }

   evas_object_show(img_edit->win);
   if (!_image_editor_init(img_edit))
     {
        _image_editor_del(img_edit);
        ERR("Filed initialize image editor");
        return NULL;
     }
   evas_object_data_set(img_edit->gengrid, IMG_EDIT_KEY, img_edit);
   evas_object_data_set(img_edit->win, IMG_EDIT_KEY, img_edit);

   ui_menu_locked_set(ap->menu_hash, true);
   evas_object_event_callback_add(img_edit->win, EVAS_CALLBACK_DEL, _on_mwin_del, ap);

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

Eina_Bool
image_editor_callback_add(Evas_Object *win, Evas_Smart_Cb func, void *data)
{
   Image_Editor *img_edit = NULL;

   if (!win)
     {
        ERR("Expecting image editor window.");
        return false;
     }
   img_edit = evas_object_data_get(win, IMG_EDIT_KEY);
   if (!img_edit)
     {
        ERR("Image editor does'nt exist");
        return false;
     }

   if (!func)
     {
        ERR("Expecting function.");
        return false;
     }

   //if (!data)
   //  {
   //     ERR("Function's data is missing.");
   //     return false;
   //  }

   img_edit->func_data.choose_func = func;
   img_edit->func_data.data = data;
   return true;
}
