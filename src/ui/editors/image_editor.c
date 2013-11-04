/* Edje Theme Editor
* Copyright (C) 2013 Samsung Electronics.
*
* This file is part of Edje Theme Editor.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; If not, see .
*/

#include "image_editor.h"
#include "alloc.h"
#include "widget_define.h"

typedef struct _Item
{
   int id;
   const char* image_name;
} Item;

struct _Image_Editor
{
   Project *pr;
   Evas_Object *mwin;
   Evas_Object *gengrid;
   Evas_Object *legend;
   struct {
      Evas_Smart_Cb choose_func;
      void *data;
   } func_data;
};

typedef struct _Image_Editor Image_Editor;

static Image_Editor window;
static Elm_Gengrid_Item_Class *gic = NULL;

static void
_grid_sel(void *data, Evas_Object *obj, void *event_info);

static char *
_grid_label_get(void *data,
                Evas_Object *obj __UNUSED__,
                const char  *part __UNUSED__)
{
   const Item *it = data;
   return strdup(it->image_name);
}

/* icon fetching callback */
static Evas_Object *
_grid_content_get(void *data,
                  Evas_Object *obj,
                  const char  *part __UNUSED__)
{
   Item *it = data;
   Evas_Object *grid = (Evas_Object *)obj;
   if (!strcmp(part, "elm.swallow.icon"))
   {
      Evas_Object *image = elm_image_add(grid);
      char buf[BUFF_MAX];
      snprintf(buf, BUFF_MAX, "edje/images/%i", it->id);
      elm_image_file_set(image, window.pr->swapfile, buf);
      evas_object_size_hint_aspect_set(image, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
      evas_object_show(image);
      return image;
   }
   return NULL;
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

static void
_on_image_done(void *data,
             Evas_Object *obj,
             void *event_info)
{
   const char *selected = event_info;
   Evas_Object *inwin = (Evas_Object *)data;
   Item *it;

   if (selected)
     {
        if (ecore_file_exists(selected))
          {
             if (!image_edit_image_add(window.pr, selected))
               {
                  NOTIFY_ERROR("Error while loading file.<br>"
                               "Please check if file is image"
                               "or/and file is accessible.");
               }
             else
               {
                  it = (Item *)mem_malloc(sizeof(Item));
                  it->image_name = eina_stringshare_add(ecore_file_file_get(selected));
                  it->id = image_edit_image_id_get(window.pr, it->image_name);
                  elm_gengrid_item_append(window.gengrid, gic, it, _grid_sel, NULL);
               }
          }
        else
          {
             NOTIFY_ERROR("Error while loading file.<br>"
                          "File is not exist");
          }
     }
   evas_object_hide(elm_object_parent_widget_get(obj));
   evas_object_del(obj);
   evas_object_del(inwin);
}

static void
_on_mw_fileselector_close(void *data __UNUSED__,
                       Evas *e __UNUSED__,
                       Evas_Object *obj,
                       void *event_info __UNUSED__)
{
   evas_object_smart_callback_call(obj, "done", NULL);
}

static void
_on_button_add_clicked_cb(void *data,
                         Evas_Object *obj __UNUSED__,
                         void *event_info __UNUSED__)
{
   Evas_Object *fs, *inwin, *box;

   inwin = mw_add(data);

   OPEN_DIALOG_ADD(inwin, box, fs, "Add image to library");

   evas_object_event_callback_add(inwin, EVAS_CALLBACK_FREE,
                                  _on_mw_fileselector_close, NULL);
   evas_object_smart_callback_add(fs, "done", _on_image_done, inwin);
   /* After migrating to EFL 1.8.0 (1.7.99) uncomment this codeline.
      evas_object_smart_callback_add(fs, "activated", _on_image_done, inwin);
    */

   elm_win_inwin_activate(inwin);

   return;
}

static void
_on_button_delete_clicked_cb(void *data __UNUSED__,
                             Evas_Object *obj __UNUSED__,
                             void *event_info __UNUSED__)
{
   Elm_Object_Item *grid_item;
   Item *it;
   Eina_List *grid_list, *l, *l2;

   if (!window.gengrid) return;

   grid_list = (Eina_List *)elm_gengrid_selected_items_get(window.gengrid);
   if (!grid_list) return;

   EINA_LIST_FOREACH_SAFE(grid_list, l, l2, grid_item)
     {
        it = elm_object_item_data_get(grid_item);
        image_edit_image_del(window.pr, it->image_name);
        elm_object_item_del(grid_item);
     }
   eina_list_free(grid_list);
}

static void
_on_button_ok_clicked_cb(void *data,
                         Evas_Object *obj __UNUSED__,
                         void *event_info __UNUSED__)
{
   Evas_Object *inwin = (Evas_Object *)data;
   if (!window.gengrid)
     {
        evas_object_del(inwin);
        return;
     }

   Elm_Object_Item *it = elm_gengrid_selected_item_get(window.gengrid);
   if (!it)
     {
        evas_object_del(inwin);
        return;
     }
   const Item* item = elm_object_item_data_get(it);
   if (!item)
     {
        evas_object_del(inwin);
        return;
     }

   if (window.func_data.choose_func)
     {
        window.func_data.choose_func(window.func_data.data, inwin, (char *)item->image_name);
        window.func_data.choose_func = NULL;
        window.func_data.data = NULL;
     }

   evas_object_del(inwin);
}

static void
_on_button_cancel_clicked_cb(void *data,
                             Evas_Object *obj __UNUSED__,
                             void *event_info __UNUSED__)
{
   Evas_Object *inwin = (Evas_Object *)data;
   evas_object_del(inwin);
}

/* item selection change callback */
static void
_grid_sel(void *data __UNUSED__,
          Evas_Object *obj __UNUSED__,
          void *event_info)
{
   char buf[BUFF_MAX];
   Evas_Object *image;
   const Item* it = elm_object_item_data_get(event_info);
   const Eina_List* sel_list = elm_gengrid_selected_items_get(window.gengrid);
   int selected_images_count = eina_list_count(sel_list);
   switch (selected_images_count)
     {
      case 0:
         elm_object_text_set(window.legend,"No images selected<br><br>");
         break;
      case 1:
         image = elm_image_add(window.legend);
         snprintf(buf, BUFF_MAX, "edje/images/%i", it->id);
         elm_image_file_set(image, window.pr->swapfile, buf);
         int w = 0, h = 0;
         elm_image_object_size_get(image, &w, &h);
         Edje_Edit_Image_Comp comp =
            image_edit_image_compression_type_get(window.pr, it->image_name);
         const char* comp_str;
         int comp_rate = -2;
         switch(comp)
           {
            case EDJE_EDIT_IMAGE_COMP_RAW:
               comp_str = "RAW";
               break;
            case EDJE_EDIT_IMAGE_COMP_USER:
               comp_str = "USER";
               break;
            case EDJE_EDIT_IMAGE_COMP_COMP:
               comp_str = "COMP";
               break;
            case EDJE_EDIT_IMAGE_COMP_LOSSY:
               comp_str = "LOSSY";
               comp_rate = image_edit_image_compression_rate_get(window.pr,
                                                                 it->image_name);
               break;
            default:
               comp_str = "Unknown compression type";
               ERR("Unknown compression type");
           }
         if (comp_rate > 0)
           {
              snprintf(buf, BUFF_MAX,
                       "Selected image: %s<br>size: %dx%d<br>Compression type:%s rate:%d",
                       it->image_name, w, h, comp_str, comp_rate);
           }
         else
           {
              snprintf(buf, BUFF_MAX,
                       "Selected image: %s<br>size: %dx%d<br>Compression type:%s",
                       it->image_name, w, h, comp_str);
           }
         elm_object_text_set(window.legend,buf);

         evas_object_del(image);
         break;
      default:
         snprintf(buf,BUFF_MAX,"%d images selected<br><br>",selected_images_count);
         elm_object_text_set(window.legend,buf);
     }
}

Evas_Object *
image_editor_window_add(Evas_Object *parent, Image_Editor_Mode mode)
{
   Evas_Object *mwin;
   Evas_Object *button;
   Evas_Object *box, *bottom_box;

   if (!parent)
     {
        ERR("Parent object is NULL!");
        return NULL;
     }

   mwin = mw_add(parent);
   mw_title_set(mwin, "Image editor");

   box = elm_box_add(mwin);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_inwin_content_set(mwin, box);
   evas_object_show(box);

   window.gengrid = elm_gengrid_add(box);
   elm_box_pack_end(box, window.gengrid);
   elm_gengrid_item_size_set(window.gengrid, 100, 100);

   if (mode == SINGLE)
     elm_gengrid_multi_select_set(window.gengrid, EINA_FALSE);
   else
     elm_gengrid_multi_select_set(window.gengrid, EINA_TRUE);

   elm_gengrid_select_mode_set(window.gengrid, ELM_OBJECT_SELECT_MODE_ALWAYS);
   evas_object_size_hint_weight_set(window.gengrid, EVAS_HINT_EXPAND,
                                                            EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(window.gengrid, EVAS_HINT_FILL,
                                                                EVAS_HINT_FILL);
   evas_object_smart_callback_add(window.gengrid, "unselected", _grid_sel, NULL);
   evas_object_show(window.gengrid);

   bottom_box = elm_box_add(box);
   elm_box_pack_end(box,bottom_box);
   elm_box_horizontal_set(bottom_box,EINA_TRUE);
   evas_object_size_hint_weight_set(bottom_box, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(bottom_box, -1.0,0);
   evas_object_show(bottom_box);

   button = elm_button_add(bottom_box);
   elm_object_style_set(button, DEFAULT_STYLE);
   elm_object_text_set(button, "Add image");
   evas_object_smart_callback_add(button, "clicked", _on_button_add_clicked_cb,
                                   parent);
   elm_box_pack_end(bottom_box,button);
   evas_object_show(button);

   button = elm_button_add(bottom_box);
   elm_object_style_set(button, DEFAULT_STYLE);
   elm_object_text_set(button, "Delete image");
   evas_object_smart_callback_add(button, "clicked", _on_button_delete_clicked_cb,
                                   NULL);
   elm_box_pack_end(bottom_box,button);
   evas_object_show(button);

   button = elm_button_add(bottom_box);
   elm_object_style_set(button, DEFAULT_STYLE);
   elm_object_text_set(button, "Ok");
   evas_object_smart_callback_add(button, "clicked", _on_button_ok_clicked_cb,
                                   mwin);
   elm_box_pack_end(bottom_box,button);
   evas_object_show(button);

   button = elm_button_add(bottom_box);
   elm_object_style_set(button, DEFAULT_STYLE);
   elm_object_text_set(button, "Cancel");
   evas_object_smart_callback_add(button, "clicked", _on_button_cancel_clicked_cb,
                                   mwin);
   elm_box_pack_end(bottom_box,button);
   evas_object_show(button);

   LABEL_ADD(bottom_box, window.legend, "No images selected<br><br>")
   elm_box_pack_end(bottom_box,window.legend);
   evas_object_size_hint_weight_set(window.legend, EVAS_HINT_EXPAND,
                                                            EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(window.legend, -1.0,-1.0);
   elm_label_line_wrap_set(window.legend, ELM_WRAP_CHAR);
   elm_label_ellipsis_set(window.legend, EINA_TRUE);

   if (!gic)
   {
      gic = elm_gengrid_item_class_new();
      gic->item_style = "custom";
      gic->func.text_get = _grid_label_get;
      gic->func.content_get = _grid_content_get;
      gic->func.del = _grid_del;
   } /* we only create the first time its needed. we dont unref/free */

   window.mwin = mwin;
   evas_object_show(mwin);

   return mwin;
}

void
image_editor_init(Evas_Object *img_edit, Project *project)
{
   if (!project)
     {
        NOTIFY_ERROR ("EDJ/EDC file is not loaded");
        evas_object_del(img_edit);
        return;
     }

   if (!img_edit)
     {
        ERR("Expecting Image editor window");
        return;
     }

   window.pr = project;
   Eina_List *l;
   Item *it;
   const char* image_name;
   Eina_List *images = image_edit_images_list_get(window.pr);
   int counter = 0;
   EINA_LIST_FOREACH(images, l, image_name)
     {
        counter++;
        if (!image_name)
          {
             /*
               FIXME: edje_edit_images_list_get returns list with some image names
               missing. image is still accesable via id, but it's not enough
               for manipulations.
              */
             ERR("name not found for image #%d",counter);
             continue;
          }
        it = (Item *)mem_malloc(sizeof(Item));
        it->image_name = eina_stringshare_add(image_name);
        it->id = image_edit_image_id_get(window.pr, it->image_name);
        elm_gengrid_item_append(window.gengrid, gic, it, _grid_sel, NULL);
     }
   eina_list_free(images);
}

void
image_editor_file_choose(Evas_Object *img_edit, const char *selected)
{
   Elm_Object_Item *grid_item = NULL;
   const Item* it;

   if (!selected) return;
   if (!img_edit)
     {
        ERR("Expecting Image editor window.");
        return;
     }

   grid_item = elm_gengrid_first_item_get(window.gengrid);
   while(grid_item)
     {
        it = elm_object_item_data_get(grid_item);
        if (strcmp(it->image_name, selected) == 0)
          {
             elm_gengrid_item_selected_set(grid_item, EINA_TRUE);
             elm_gengrid_item_bring_in(grid_item, ELM_GENGRID_ITEM_SCROLLTO_MIDDLE);
             return;
          }
        grid_item = elm_gengrid_item_next_get(grid_item);
     }
}

void
image_editor_callback_add(Evas_Object *img_edit, Evas_Smart_Cb func, void *data)
{
   if (!img_edit)
     {
        ERR("Expecting image editor window.");
        return;
     }

   if (img_edit != window.mwin)
     {
        ERR("Expecting image editor window. <br>"
            "Its not last registered window.");
        return;
     }

   if (!func)
     {
        ERR("Expecting function.");
        return;
     }

   if (!data)
     {
        ERR("Function's data is missing.");
        return;
     }

   window.func_data.choose_func = func;
   window.func_data.data = data;
}
