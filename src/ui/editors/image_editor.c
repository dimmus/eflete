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
};

struct _Image_Editor
{
   Project *pr;
   Evas_Object *win;
   Evas_Object *gengrid;
   Evas_Object *legend;
   struct {
      Evas_Smart_Cb choose_func;
      void *data;
   } func_data;
};

static Elm_Gengrid_Item_Class *gic = NULL;

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
   evas_object_data_del(img_edit->win, IMG_EDIT_KEY);
   evas_object_data_del(img_edit->gengrid, IMG_EDIT_KEY);
   evas_object_del(img_edit->legend);
   evas_object_del(img_edit->gengrid);
   evas_object_del(img_edit->win);
   free(img_edit);
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
   char buf[BUFF_MAX];
   Evas_Object *image = NULL;

   if (!strcmp(part, "elm.swallow.icon"))
     {
        image = elm_image_add(grid);
        snprintf(buf, BUFF_MAX, "edje/images/%i", it->id);
        elm_image_file_set(image, img_edit->pr->swapfile, buf);
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

/* item selection change callback */
static void
_grid_sel(void *data,
          Evas_Object *obj __UNUSED__,
          void *event_info)
{
   char buf[BUFF_MAX];
   Edje_Edit_Image_Comp comp;

   const char* comp_str = NULL;
   Evas_Object *edje_edit_obj = NULL;
   Evas_Object *image = NULL;

   int comp_rate = -2;
   int w = 0;
   int h = 0;

   Image_Editor *img_edit = (Image_Editor *)data;
   const Item* it = elm_object_item_data_get(event_info);
   const Eina_List* sel_list = elm_gengrid_selected_items_get(img_edit->gengrid);
   int selected_images_count = eina_list_count(sel_list);

   GET_OBJ(img_edit->pr, edje_edit_obj);

   switch (selected_images_count)
     {
      case 0:
         elm_object_text_set(img_edit->legend, _("No images selected<br><br>"));
         break;
      case 1:
         image = elm_image_add(img_edit->gengrid);
         snprintf(buf, BUFF_MAX, "edje/images/%i", it->id);
         elm_image_file_set(image, img_edit->pr->swapfile, buf);
         elm_image_object_size_get(image, &w, &h);
         comp = edje_edit_image_compression_type_get(edje_edit_obj,
                                                     it->image_name);
         switch(comp)
           {
            case EDJE_EDIT_IMAGE_COMP_RAW:
               comp_str = _("RAW");
               break;
            case EDJE_EDIT_IMAGE_COMP_USER:
               comp_str = _("USER");
               break;
            case EDJE_EDIT_IMAGE_COMP_COMP:
               comp_str = _("COMP");
               break;
            case EDJE_EDIT_IMAGE_COMP_LOSSY:
               comp_str = _("LOSSY");
               comp_rate = edje_edit_image_compression_rate_get(edje_edit_obj,
                                                                 it->image_name);
               break;
            default:
               comp_str = _("Unknown compression type");
               ERR("Unknown compression type");
               break;
           }
         if (comp_rate > 0)
           {
              snprintf(buf, BUFF_MAX,
                       _("Selected image: %s<br>size: %dx%d<br>Compression"
                       " type:%s rate:%d"), it->image_name, w, h, comp_str,
                       comp_rate);
           }
         else
           {
              snprintf(buf, BUFF_MAX,
                      _("Selected image: %s<br>size: %dx%d<br>Compression type: %s"),
                      it->image_name, w, h, comp_str);
           }
         elm_object_text_set(img_edit->legend, buf);
         evas_object_del(image);
         break;
      default:
         snprintf(buf, BUFF_MAX, ngettext("%d image selected<br><br>",
                                          "%d images selected<br><br>",
                                          selected_images_count),
                  selected_images_count);
         elm_object_text_set(img_edit->legend, buf);
     }
}

static void
_on_image_done(void *data,
             Evas_Object *obj __UNUSED__,
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
             NOTIFY_ERROR(_("Error while loading file.<br>"
                          "Please check if file is image"
                          "or/and file is accessible."));
          }
        else
          {
             it = (Item *)mem_malloc(sizeof(Item));
             it->image_name = eina_stringshare_add(ecore_file_file_get(selected));
             it->id = edje_edit_image_id_get(edje_edit_obj, it->image_name);
             elm_gengrid_item_append(img_edit->gengrid, gic, it, _grid_sel, img_edit);
             pm_project_changed(app_create()->project);
          }
     }
   else
     {
        NOTIFY_ERROR(_("Error while loading file.<br>File is not exist"));
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
     pm_project_changed(app_create()->project);
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
            _image_editor_del(img_edit);
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
              it = (Item *)mem_malloc(sizeof(Item));
              it->image_name = eina_stringshare_add(image_name);
              it->id = edje_edit_image_id_get(edje_edit_obj, it->image_name);
              elm_gengrid_item_append(img_edit->gengrid, gic, it, _grid_sel, img_edit);
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
   Evas_Object *box, *bottom_box, *panel_box;
   Evas_Object *_bg = NULL;
   /* temporary solution, while it not moved to modal window */
   App_Data *ap = app_create();

   if (!project)
     {
        ERR("Project doesn't opened");
        return NULL;
     }

   Image_Editor *img_edit = (Image_Editor *)mem_calloc(1, sizeof(Image_Editor));
   img_edit->pr = project;

   img_edit->win = mw_add(_on_button_cancel_clicked_cb, img_edit);
   mw_title_set(img_edit->win, _("Image editor"));

   BOX_ADD(img_edit->win, box, false, false);
   elm_win_inwin_content_set(img_edit->win, box);

   img_edit->gengrid = elm_gengrid_add(box);
   elm_object_style_set(img_edit->gengrid, DEFAULT_STYLE);
   elm_box_pack_end(box, img_edit->gengrid);
   elm_gengrid_item_size_set(img_edit->gengrid, ITEM_WIDTH, ITEM_HEIGHT);
   elm_gengrid_align_set(img_edit->gengrid, 0.5, 0.0);
   elm_scroller_policy_set(img_edit->gengrid, ELM_SCROLLER_POLICY_OFF,
                           ELM_SCROLLER_POLICY_OFF);

   GET_IMAGE(_bg, img_edit->gengrid, "gallery-bg");
   elm_object_part_content_set(img_edit->gengrid, "elm.swallow.background", _bg);
   evas_object_show(_bg);

   if (mode == SINGLE)
     elm_gengrid_multi_select_set(img_edit->gengrid, false);
   else
     elm_gengrid_multi_select_set(img_edit->gengrid, true);

   elm_gengrid_select_mode_set(img_edit->gengrid, ELM_OBJECT_SELECT_MODE_ALWAYS);
   evas_object_size_hint_weight_set(img_edit->gengrid, EVAS_HINT_EXPAND,
                                                            EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(img_edit->gengrid, EVAS_HINT_FILL,
                                                                EVAS_HINT_FILL);
   evas_object_smart_callback_add(img_edit->gengrid, "unselected", _grid_sel,
                                  img_edit);
   evas_object_show(img_edit->gengrid);

   BOX_ADD(box, panel_box, true, false);
   elm_box_pack_end(box, panel_box);

   BOX_ADD(panel_box, bottom_box, true, false);
   evas_object_size_hint_align_set(bottom_box, 0, 0.5);
   elm_box_pack_end(panel_box, bottom_box);

   BUTTON_ADD(bottom_box, button, _("Add image"));
   evas_object_smart_callback_add(button, "clicked", _on_button_add_clicked_cb,
                                   img_edit);
   evas_object_size_hint_max_set(button, 80, 25);
   evas_object_size_hint_min_set(button, 80, 25);
   elm_box_pack_end(bottom_box, button);

   BUTTON_ADD(bottom_box, button, _("Delete image"));
   evas_object_smart_callback_add(button, "clicked", _on_button_delete_clicked_cb,
                                   img_edit);
   evas_object_size_hint_max_set(button, 100, 25);
   evas_object_size_hint_min_set(button, 100, 25);
   elm_box_pack_end(bottom_box, button);

   LABEL_ADD(bottom_box, img_edit->legend, _("No images selected<br><br>"))
   elm_box_pack_end(bottom_box,img_edit->legend);
   evas_object_size_hint_align_set(img_edit->legend, -1.0,-1.0);
   elm_label_ellipsis_set(img_edit->legend, true);

   BOX_ADD(panel_box, bottom_box, true, false);
   evas_object_size_hint_align_set(bottom_box, 1, 0.5);
   elm_box_pack_end(panel_box, bottom_box);

   BUTTON_ADD(bottom_box, button, _("Ok"));
   evas_object_smart_callback_add(button, "clicked", _on_button_ok_clicked_cb,
                                  img_edit);
   evas_object_size_hint_max_set(button, 80, 25);
   evas_object_size_hint_min_set(button, 80, 25);
   elm_box_pack_end(bottom_box, button);

   BUTTON_ADD(bottom_box, button, _("Cancel"));
   evas_object_smart_callback_add(button, "clicked", _on_button_cancel_clicked_cb,
                                   img_edit);
   evas_object_size_hint_max_set(button, 80, 25);
   evas_object_size_hint_min_set(button, 80, 25);
   elm_box_pack_end(bottom_box, button);

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
