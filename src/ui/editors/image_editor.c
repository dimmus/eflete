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
};

typedef struct _Image_Editor Image_Editor;

static Image_Editor window;
static Elm_Gengrid_Item_Class *gic = NULL;

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
   free(it);
}
static void
_on_button_ok_clicked_cb(void *data,
                         Evas_Object *obj __UNUSED__,
                         void *event_info __UNUSED__)
{
   Evas_Object *inwin = (Evas_Object *)data;
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
         //img_size
         image = elm_image_add(window.legend);//parent is set to pass safety check
         snprintf(buf, BUFF_MAX, "edje/images/%i", it->id);
         elm_image_file_set(image, window.pr->swapfile, buf);
         int w=0, h=0;
         elm_image_object_size_get(image, &w, &h);
         //compression
         Edje_Edit_Image_Comp comp =
            image_edit_image_compression_type_get(window.pr, it->image_name);
         const char* comp_str;
         int comp_rate=-2;
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
           }
         //output into label
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
image_editor_window_add(Evas_Object *parent)
{
   Evas_Object *mwin;
   Evas_Object *button;
   Evas_Object *box, *bottom_box;

   mwin = mw_add(parent);
   mw_title_set(mwin, "Image editor");

   box = elm_box_add(mwin);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_inwin_content_set(mwin, box);
   evas_object_show(box);

   window.gengrid = elm_gengrid_add(box);
   elm_box_pack_end(box, window.gengrid);
   elm_gengrid_item_size_set(window.gengrid, 100, 100);
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
   elm_object_text_set(button, "Add image");
   elm_box_pack_end(bottom_box,button);
   evas_object_show(button);

   button = elm_button_add(bottom_box);
   elm_object_text_set(button, "Delete image");
   elm_box_pack_end(bottom_box,button);
   evas_object_show(button);

   button = elm_button_add(bottom_box);
   elm_object_text_set(button, "Ok");
   evas_object_smart_callback_add(button, "clicked", _on_button_ok_clicked_cb,
                                   mwin);
   elm_box_pack_end(bottom_box,button);
   evas_object_show(button);

   button = elm_button_add(bottom_box);
   elm_object_text_set(button, "Cancel");
   evas_object_smart_callback_add(button, "clicked", _on_button_cancel_clicked_cb,
                                   mwin);
   elm_box_pack_end(bottom_box,button);
   evas_object_show(button);

   window.legend = elm_label_add(bottom_box);
   elm_object_text_set(window.legend, "No images selected<br><br>");
   elm_box_pack_end(bottom_box,window.legend);
   evas_object_size_hint_weight_set(window.legend, EVAS_HINT_EXPAND,
                                                            EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(window.legend, -1.0,-1.0);
   elm_label_line_wrap_set(window.legend, ELM_WRAP_CHAR);
   elm_label_ellipsis_set(window.legend, EINA_TRUE);
   evas_object_show(window.legend);

   if (!gic)
   {
      gic = elm_gengrid_item_class_new();
      gic->item_style = "custom";
      gic->func.text_get = _grid_label_get;
      gic->func.content_get = _grid_content_get;
      gic->func.del = _grid_del;
   } // we only create the first time its needed. we dont unref/free
   evas_object_show(mwin);

   return mwin;
}

void
image_editor_init (Evas_Object *img_edit, Project *project)
{
   if (!project)
     {
        NOTIFY_ERROR ("EDJ/EDC file is not loaded");
        evas_object_del(img_edit);
        return;
     }
   window.pr=project;
   Eina_List *l;
   const char* image_name;
   Eina_List *images = image_edit_images_list_get(window.pr);
   int counter=0;
   EINA_LIST_FOREACH(images,l,image_name)
     {
        counter++;
        if (!image_name)
          {
             //FIXME: edje_edit_images_list_get returns list with some image names
             //missing. image is still accesable via id, but it's not enough
             //for manipulations
             ERR("name not found for image #%d",counter);
             continue;
          }
        Item *it;
        it = (Item *)malloc(sizeof(*it));
        it->image_name = strdup(image_name);
        it->id=image_edit_image_id_get (window.pr, it->image_name);
        elm_gengrid_item_append(window.gengrid, gic, it, _grid_sel, NULL);
     }
   eina_list_free(images);
}
