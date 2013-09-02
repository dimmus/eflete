/**
 * Edje Editor Tool
 * Copyright (C) 2013 Samsung Electronics.
 *
 * This file is part of Edje Editor Tool.
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
 * along with this program; If not, see <http://www.gnu.org/licenses/>.
 */

#include "ui_groupspace.h"
#include "ui_highlight.h"

#define GS_WS_KEY "gs_workspace_key"
#define GS_PROJECT_KEY "gs_project_key"
#define GS_BOX_KEY "gs_box_key"
#define GS_PART_DATA_KEY "gs_part_key"
#define GS_GROUP_KEY "gs_group_key"

static void
_gs_rect_update(Group *group, Part *part);
static void
_gs_image_update(Group *group, Part *part, Project *project);
static void
_gs_text_update(Group *group, Part *part);
static void
_gs_spaser_swallow_group_update(Group *group, Part *part);
static void
_gs_textblock_update(Group *group, Part *part);


static void
_main_box_layout(Evas_Object *o, Evas_Object_Box_Data *p, void *data)
{
   int x, y, w, h;
   int xx, yy;
   Evas_Object *edje_part;
   Evas_Object *edje_group = (Evas_Object *)data;
   Eina_List *l;
   Evas_Object_Box_Option *opt;

   evas_object_geometry_get(o, &xx, &yy, &w, &h);
   evas_object_resize(edje_group, w, h);
   evas_object_move(edje_group, xx, yy);
   evas_object_geometry_get(edje_group, &xx, &yy, &w, &h);
   EINA_LIST_FOREACH(p->children, l, opt)
     {
        edje_part = evas_object_data_get(opt->obj, GS_PART_DATA_KEY);
        /* if there is a child that is highlight, then edje_part will be NULL */
        if (edje_part)
          {
            evas_object_geometry_get(edje_part, &x, &y, &w, &h);
            evas_object_move(opt->obj, x, y);
            evas_object_resize(opt->obj, w, h);
            evas_object_geometry_get(opt->obj, &x, &y, &w, &h);
          }
    }
}

static void
_separate_layout(Evas_Object *o, Evas_Object_Box_Data *p, void *data)
{
   int x, y, w, h;
   Evas_Object *edje_part;
   Evas_Object *edje_group = (Evas_Object *)data;
   Eina_List *l;
   Evas_Object_Box_Option *opt;
   int count = 0;

   evas_object_geometry_get(o, &x, &y, &w, &h);
   evas_object_resize(edje_group, w, h);
   evas_object_move(edje_group, x, y);
   EINA_LIST_FOREACH(p->children, l, opt)
     {
        edje_part = evas_object_data_get(opt->obj, GS_PART_DATA_KEY);
        evas_object_geometry_get(edje_part, &x, &y, &w, &h);
        evas_object_move(opt->obj, x + count * 50, y + count * 20);
        evas_object_resize(opt->obj, w, h);
        evas_object_geometry_get(opt->obj, &x, &y, &w, &h);
        count++;
    }
}

static void
_new_state_add(void *data __UNUSED__,
               Evas_Object *obj __UNUSED__,
               void *event_info)
{
   char *state_data = (char *)event_info;
   char name[BUFF_MAX];
   char value[BUFF_MAX];
   snprintf(name, sizeof(name), "%s", strtok(state_data, "|"));
   snprintf(value, sizeof(value), "%s", strtok(NULL, "|"));
   DBG("STATE: received [%s]; name [%s], value[%s]", state_data, name, value);
}

static void
_new_rect_add(void *data __UNUSED__,
              Evas_Object *obj,
              void *event_info)
{
   Evas_Object *groupspace = (Evas_Object *)obj;

   Evas_Object *box = evas_object_data_get(groupspace, GS_BOX_KEY);
   Group *group = evas_object_data_get(groupspace, GS_GROUP_KEY);
   Part *part = (Part *)mem_calloc(1, sizeof(Part));
   Workspace *ws = evas_object_data_get(groupspace, GS_WS_KEY);
   const Evas_Object *edje_part = NULL;
   char *name = (char *)event_info;
   if (edje_edit_part_exist(group->obj, name))
     {
        NOTIFY_INFO(3, "Part with name [%s] alredy exist.", name);
        return;
     }

   part->name = eina_stringshare_add(name);

   edje_edit_part_add(group->obj, part->name, EDJE_PART_TYPE_RECTANGLE);
   edje_edit_state_add(group->obj, part->name, "default", 0.0);
   edje_edit_state_color_set(group->obj, part->name, "default", 0.0,
                             255, 255, 255, 255);
   part->obj = evas_object_rectangle_add(ws->canvas);
   part->type = EDJE_PART_TYPE_RECTANGLE;
   _gs_rect_update(group, part);

   edje_part = edje_object_part_object_get(group->obj, part->name);
   evas_object_data_set(part->obj, GS_PART_DATA_KEY, edje_part);
   group->parts = eina_inlist_append(group->parts, EINA_INLIST_GET(part));
   evas_object_box_append(box, part->obj);
   evas_object_show(part->obj);
   evas_object_smart_calculate(box);
   evas_object_smart_callback_call(group->obj, "wl,part,added", part);

}

static void
_new_img_add(void *data __UNUSED__,
             Evas_Object *obj,
             void *event_info)
{
   Evas_Object *groupspace = (Evas_Object *)obj;
   Evas_Object *box = evas_object_data_get(groupspace, GS_BOX_KEY);
   Group *group = evas_object_data_get(groupspace, GS_GROUP_KEY);
   Part *part = (Part *)mem_calloc(1, sizeof(Part));
   Workspace *ws = evas_object_data_get(groupspace, GS_WS_KEY);
   Project *project = evas_object_data_get(obj, GS_PROJECT_KEY);

   const Evas_Object *edje_part = NULL;
   char *name = (char *)event_info;

   if (edje_edit_part_exist(group->obj, name))
     {
        NOTIFY_INFO(3, "Part with name [%s] alredy exist.", name);
        return;
     }

   part->name = eina_stringshare_add(name);
   part->obj = evas_object_image_filled_add(ws->canvas);
   part->type = EDJE_PART_TYPE_IMAGE;

   edje_edit_part_add(group->obj, part->name, EDJE_PART_TYPE_IMAGE);
   edje_edit_state_add(group->obj, part->name, "default", 0.0);
   _gs_image_update(group, part, project);
   edje_part = edje_object_part_object_get(group->obj, part->name);

   evas_object_data_set(part->obj, GS_PART_DATA_KEY, edje_part);
   group->parts = eina_inlist_append(group->parts, EINA_INLIST_GET(part));
   evas_object_box_append(box, part->obj);
   evas_object_show(part->obj);
   evas_object_smart_calculate(box);
   evas_object_smart_callback_call(group->obj, "wl,part,added", part);

}

static void
_new_txt_add(void *data __UNUSED__,
             Evas_Object *obj,
             void *event_info)
{
   Evas_Object *groupspace = (Evas_Object *)obj;
   Evas_Object *box = evas_object_data_get(groupspace, GS_BOX_KEY);
   Group *group = evas_object_data_get(groupspace, GS_GROUP_KEY);
   Part *part = (Part *)mem_calloc(1, sizeof(Part));
   Workspace *ws = evas_object_data_get(groupspace, GS_WS_KEY);

   const Evas_Object *edje_part = NULL;
   char *name = (char *)event_info;

   if (edje_edit_part_exist(group->obj, name))
     {
        NOTIFY_INFO(3, "Part with name [%s] alredy exist.", name);
        return;
     }

   part->name = eina_stringshare_add(name);

   edje_edit_part_add(group->obj, part->name, EDJE_PART_TYPE_TEXT);
   edje_edit_state_add(group->obj, part->name, "default", 0.0);
   edje_edit_state_text_size_set(group->obj, part->name, "default", 0.0, 10);
   edje_edit_state_color_set(group->obj, part->name, "default", 0.0, 0, 0, 0, 255);
   edje_edit_state_font_set(group->obj, part->name, "default", 0.0,
                            "Sans");
   part->obj = evas_object_text_add(ws->canvas);
   part->type = EDJE_PART_TYPE_TEXT;
   _gs_text_update(group, part);

   edje_part = edje_object_part_object_get(group->obj, part->name);
   evas_object_data_set(part->obj, GS_PART_DATA_KEY, edje_part);
   group->parts = eina_inlist_append(group->parts, EINA_INLIST_GET(part));
   evas_object_box_append(box, part->obj);
   evas_object_show(part->obj);
   evas_object_smart_calculate(box);
   evas_object_smart_callback_call(group->obj, "wl,part,added", part);

}

static void
_new_txtblock_add(void *data __UNUSED__,
                   Evas_Object *obj,
                   void *event_info)
{
   Evas_Object *groupspace = (Evas_Object *)obj;
   Evas_Object *box = evas_object_data_get(groupspace, GS_BOX_KEY);
   Group *group = evas_object_data_get(groupspace, GS_GROUP_KEY);
   Part *part = (Part *)mem_calloc(1, sizeof(Part));
   Workspace *ws = evas_object_data_get(groupspace, GS_WS_KEY);

   const Evas_Object *edje_part = NULL;
   char *name = (char *)event_info;
   Evas_Textblock_Style *ts = NULL;

   if (edje_edit_part_exist(group->obj, name))
     {
        NOTIFY_INFO(3, "Part with name [%s] alredy exist.", name);
        return;
     }

   part->name = eina_stringshare_add(name);
   edje_edit_part_add(group->obj, part->name, EDJE_PART_TYPE_TEXTBLOCK);
   edje_edit_state_add(group->obj, part->name, "default", 0.0);
   edje_edit_state_text_size_set(group->obj, part->name, "default", 0.0, 10);

   ts = evas_textblock_style_new();
   evas_textblock_style_set(ts, "font=Sans font_size=10 color=#000000");

   part->obj = evas_object_textblock_add(ws->canvas);
   part->type = EDJE_PART_TYPE_TEXTBLOCK;
   _gs_textblock_update(group, part);
   evas_object_textblock_style_set(part->obj, ts);

   edje_part = edje_object_part_object_get(group->obj, part->name);
   evas_object_data_set(part->obj, GS_PART_DATA_KEY, edje_part);
   group->parts = eina_inlist_append(group->parts, EINA_INLIST_GET(part));
   evas_object_box_append(box, part->obj);
   evas_object_show(part->obj);
   evas_object_smart_calculate(box);
   evas_object_smart_callback_call(group->obj, "wl,part,added", part);
}

static void
_new_swallow_add(void *data __UNUSED__,
                 Evas_Object *obj,
                 void *event_info)
{
   Evas_Object *groupspace = (Evas_Object *)obj;
   Evas_Object *box = evas_object_data_get(groupspace, GS_BOX_KEY);
   Group *group = evas_object_data_get(groupspace, GS_GROUP_KEY);
   Part *part = (Part *)mem_calloc(1, sizeof(Part));
   Workspace *ws = evas_object_data_get(groupspace, GS_WS_KEY);
   const Evas_Object *edje_part = NULL;
   char *name = (char *)event_info;

   if (edje_edit_part_exist(group->obj, name))
     {
        NOTIFY_INFO(3, "Part with name [%s] alredy exist.", name);
        return;
     }

   part->name = eina_stringshare_add(name);
   part->type = EDJE_PART_TYPE_SWALLOW;
   part->obj = evas_object_image_add(ws->canvas);
   evas_object_image_file_set(part->obj,
               TET_IMG_PATH"swallow_spacer_mask.png", NULL);
   evas_object_image_fill_set(part->obj, 2, 2, 10, 10);
   evas_object_color_set(part->obj, 180, 108, 0, 255);

   edje_edit_part_add(group->obj, part->name, EDJE_PART_TYPE_SWALLOW);
   edje_edit_state_add(group->obj, part->name, "default", 0.0);
   _gs_spaser_swallow_group_update(group, part);
   edje_part = edje_object_part_object_get(group->obj, part->name);
   evas_object_data_set(part->obj, GS_PART_DATA_KEY, edje_part);

   group->parts = eina_inlist_append(group->parts, EINA_INLIST_GET(part));
   evas_object_box_append(box, part->obj);
   evas_object_show(part->obj);
   evas_object_smart_calculate(box);
   evas_object_smart_callback_call(group->obj, "wl,part,added", part);

}

static void
_new_spacer_add(void *data __UNUSED__,
                Evas_Object *obj,
                void *event_info)
{
   Evas_Object *groupspace = (Evas_Object *)obj;
   Evas_Object *box = evas_object_data_get(groupspace, GS_BOX_KEY);
   Group *group = evas_object_data_get(groupspace, GS_GROUP_KEY);
   Part *part = (Part *)mem_calloc(1, sizeof(Part));
   Workspace *ws = evas_object_data_get(groupspace, GS_WS_KEY);

   const Evas_Object *edje_part = NULL;
   char *name = (char *)event_info;

   if (edje_edit_part_exist(group->obj, name))
     {
        NOTIFY_INFO(3, "Part with name [%s] alredy exist.", name);
        return;
     }

   part->name = eina_stringshare_add(name);
   part->type = EDJE_PART_TYPE_SPACER;
   part->obj = evas_object_image_add(ws->canvas);
   evas_object_image_file_set(part->obj,
               TET_IMG_PATH"swallow_spacer_mask.png", NULL);
   evas_object_image_fill_set(part->obj, 2, 2, 10, 10);
   evas_object_color_set(part->obj, 15, 60, 162, 255);

   edje_edit_part_add(group->obj, part->name, EDJE_PART_TYPE_SPACER);
   edje_edit_state_add(group->obj, part->name, "default", 0.0);
   _gs_spaser_swallow_group_update(group, part);

   edje_part = edje_object_part_object_get(group->obj, part->name);
   evas_object_data_set(part->obj, GS_PART_DATA_KEY, edje_part);
   group->parts = eina_inlist_append(group->parts, EINA_INLIST_GET(part));
   evas_object_box_append(box, part->obj);
   evas_object_show(part->obj);
   evas_object_smart_calculate(box);
   evas_object_smart_callback_call(group->obj, "wl,part,added", part);

}

static void
_part_add(void *data __UNUSED__,
          Evas_Object *obj __UNUSED__,
          void *event_info __UNUSED__)
{
   Workspace *ws = (Workspace *)data;
   evas_object_smart_callback_call(ws->groupspace, "gs,dialog,add", NULL);
}

static void
_state_add(void *data __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Workspace *ws = (Workspace *)data;
   evas_object_smart_callback_call(ws->groupspace, "gs,state,add", NULL);
}

static void
_part_delete(void *data,
             Evas_Object *obj __UNUSED__,
             void *event_info)
{
   Workspace *ws = (Workspace *)data;
   Evas_Object *box = evas_object_data_get(ws->groupspace, GS_BOX_KEY);
   Group *group = evas_object_data_get(ws->groupspace, GS_GROUP_KEY);
   Part *part = (Part *)event_info;

   group->parts = eina_inlist_remove(group->parts, EINA_INLIST_GET(part));
   evas_object_box_remove(box, part->obj);
   evas_object_box_remove(box, ws->highlight.space_hl);
   evas_object_box_remove(box, ws->highlight.highlight);
   ws->highlight.part = NULL;
   evas_object_data_del(part->obj, GS_PART_DATA_KEY);
   edje_edit_part_del(group->obj, part->name);
   evas_object_del(part->obj);
//   evas_object_del(ws->highlight.obj);
   free(part);

   edje_object_calc_force(group->obj);
}


static void
_layer_up_change(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info)
{
   Workspace *ws = (Workspace *)data;
   Evas_Object *box = evas_object_data_get(ws->groupspace, GS_BOX_KEY);
   Group *group = evas_object_data_get(ws->groupspace, GS_GROUP_KEY);

   Eina_List *childs = NULL;
   Eina_List *l = NULL;
   Evas_Object *child = NULL;
   Evas_Object *prev_child = NULL;
   Evas_Object *edje_part = NULL;
   Part *part = (Part *)event_info;

   if(!edje_edit_part_restack_below(group->obj, part->name))
     {
        NOTIFY_INFO(3, "Failed part[%s] restack above", part->name);
        return;
     }
   edje_object_calc_force(group->obj);

   childs = evas_object_box_children_get(box);
   EINA_LIST_FOREACH(childs, l, child)
     {
        edje_part = evas_object_data_get(child, GS_PART_DATA_KEY);
        if ((edje_part) && (part->obj == child)) break;
     }
   prev_child = eina_list_data_get(eina_list_prev(l));
   if (!prev_child)  return;
   evas_object_box_remove(box, child);
   evas_object_box_insert_before(box, child, prev_child);

/**
 * FIXME:
 * For correct restack elements in box container we are need to remove all childs
 * from box. Change order of childs in Eina_list and after this push back elements
 * into box container. Otherwise elements in box container does'nt restack
 * correctly.
 */
   childs = evas_object_box_children_get(box);
   evas_object_box_remove_all(box, EINA_FALSE);
   EINA_LIST_FOREACH(childs, l, child)
     {
        evas_object_box_append(box, child);
     }
   evas_object_smart_calculate(box);
   eina_list_free(l);

   Eina_Inlist *prev_elm =  NULL;
   Eina_Inlist *itr = NULL;
   Part *_part = NULL;

   for(itr = group->parts; itr != NULL; itr = itr->next)
     {
         _part = EINA_INLIST_CONTAINER_GET(itr, Part);
         if (_part == part)
           {
              prev_elm = itr->prev;
              group->parts = eina_inlist_remove(group->parts, itr);
              group->parts = eina_inlist_prepend_relative(group->parts,
                                                   itr, prev_elm);
              break;
           }
     }

}

static void
_layer_down_change(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info)
{
   Workspace *ws = (Workspace *)data;
   Evas_Object *box = evas_object_data_get(ws->groupspace, GS_BOX_KEY);
   Group *group = evas_object_data_get(ws->groupspace, GS_GROUP_KEY);
   Eina_List *childs = NULL;
   Eina_List *l = NULL;
   Evas_Object *child = NULL;
   Evas_Object *next_child = NULL;
   Evas_Object *edje_part = NULL;
   Part *part = (Part *)event_info;

   if (!edje_edit_part_restack_above(group->obj, part->name))
     NOTIFY_INFO(3, "Failed part restack below");
   childs = evas_object_box_children_get(box);
   EINA_LIST_FOREACH(childs, l, child)
     {
        edje_part = evas_object_data_get(child, GS_PART_DATA_KEY);
        if ((edje_part) && (part->obj == child))  break;
     }
   next_child = eina_list_data_get(eina_list_next(l));
   if (!next_child)  return;
   evas_object_box_remove(box, child);
   evas_object_box_insert_after(box, child, next_child);

/**
 * FIXME:
 * For correct restack elements in box container we are need to remove all childs
 * from box. Change order of childs in Eina_list and after this push back elements
 * into box container. Otherwise elements in box container does'nt restack
 * correctly.
 */
   childs = evas_object_box_children_get(box);
   evas_object_box_remove_all(box, EINA_FALSE);
   EINA_LIST_FOREACH(childs, l, child)
     {
        evas_object_box_append(box, child);
     }
   evas_object_smart_calculate(box);
   edje_object_calc_force(group->obj);

   Eina_Inlist *next_elm =  NULL;
   Eina_Inlist *itr = NULL;
   Part *_part = NULL;
   for (itr = group->parts; itr != NULL; itr = itr->next)
     {
        _part = EINA_INLIST_CONTAINER_GET(itr, Part);
        if (_part == part)
          {
             next_elm = itr->next;
             group->parts = eina_inlist_remove(group->parts, itr);
             group->parts = eina_inlist_append_relative(group->parts,
                                                         itr, next_elm);
             break;
          }
     }
}

static void
_gs_mouse_move_cb(void *data, Evas *e, Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   int x, y;
   Workspace *ws = (Workspace*)data;
   evas_pointer_output_xy_get(e, &x, &y);
   ui_ruler_pointer_pos_set(ws->ruler_hor, x);
   ui_ruler_pointer_pos_set(ws->ruler_ver, y);
}

static void
_gs_hilight_move_resize(void *data,
                        Evas *e __UNUSED__,
                        Evas_Object *obj __UNUSED__,
                        void *ei __UNUSED__)
{
   Workspace *ws = (Workspace*)data;

   if (ws->highlight.part)
     ui_object_highlight_move(ws);
}

static void
_gs_resize_cb(void *data,
              Evas *e __UNUSED__,
              Evas_Object *obj,
              void *event_info __UNUSED__)
{
   int x, y, w, h;
   int x1,y1,w1,h1;//FIXME: change var names
   Workspace *ws = (Workspace*)data;
   evas_object_geometry_get(obj, &x, &y, &w, &h);
   evas_object_geometry_get(ws->bg, &x1, &y1, &w1, &h1);
   //FIXME:avoid hardcode val(25)
   ui_ruler_scale_absolute_position_zero_set(ws->ruler_hor, x-x1-25);
   ui_ruler_scale_absolute_position_zero_set(ws->ruler_ver, y-y1-25);
   ui_ruler_scale_relative_position_set(ws->ruler_hor, x-x1-25, x + w-x1-25);
   ui_ruler_scale_relative_position_set(ws->ruler_ver, y-y1-25, y + h-y1-25);
   ui_ruler_redraw(ws->ruler_hor);
   ui_ruler_redraw(ws->ruler_ver);

   if (ws->highlight.part)
     ui_object_highlight_move(ws);
}

static void
_gs_image_update(Group *group,
                 Part *part,
                 Project *project)
{
   Evas_Load_Error err;
   char buf[BUFF_MAX];
   const char *image_normal;
   double fill_rel_x, fill_rel_y;
   int id;
   int bl, br, bt, bb;
   int r, g, b, a;

   edje_edit_part_selected_state_set(group->obj,
                                     part->name,
                                     part->curr_state,
                                     part->curr_state_value);

   image_normal = edje_edit_state_image_get(group->obj,
                                            part->name,
                                            part->curr_state,
                                            part->curr_state_value);
   if (image_normal)
     {
         id = edje_edit_image_id_get(group->obj, image_normal);
         snprintf(buf, sizeof(buf), "edje/images/%i", id);
         evas_object_image_file_set(part->obj, project->swapfile, buf);
         err = evas_object_image_load_error_get(part->obj);
         if (err != EVAS_LOAD_ERROR_NONE)
           WARN("Could not update image. Error string is \"%s\"\n", evas_load_error_str(err));
     }

   edje_edit_state_color_get(group->obj, part->name,
                             part->curr_state, part->curr_state_value,
                             &r, &g, &b, &a);
   evas_object_color_set(part->obj, r * a / 255, g * a / 255, b * a / 255, a);

   edje_edit_state_image_border_get(group->obj, part->name,
                                    part->curr_state, part->curr_state_value,
                                    &bl, &br, &bt, &bb);
   evas_object_image_border_set(part->obj, bl, br, bt, bb);

   fill_rel_x = edje_edit_state_fill_origin_relative_x_get(group->obj,
                                                           part->name,
                                                           part->curr_state,
                                                           part->curr_state_value);
   fill_rel_y = edje_edit_state_fill_origin_relative_y_get(group->obj,
                                                           part->name,
                                                           part->curr_state,
                                                           part->curr_state_value);

   evas_object_image_fill_set(part->obj, fill_rel_x, fill_rel_y, 0, 0);

   edje_edit_string_free(image_normal);
}

static void
_gs_rect_update(Group *group,
                Part *part)
{
   int r, g, b, a;
   edje_edit_part_selected_state_set(group->obj,
                                     part->name,
                                     part->curr_state,
                                     part->curr_state_value);

   edje_edit_state_color_get(group->obj, part->name,
                             part->curr_state, part->curr_state_value,
                             &r, &g, &b, &a);
   evas_object_color_set(part->obj, r * a / 255, g * a / 255, b *a / 255, a);
}

static void
_gs_textblock_update(Group *group,
                     Part *part)
{
   Evas_Textblock_Style *ts = NULL;
   const Evas_Textblock_Style *obj_style;
   const Evas_Object *get_style;
   const char *text;

   edje_edit_part_selected_state_set(group->obj,
                                     part->name,
                                     part->curr_state,
                                     part->curr_state_value);

   get_style = edje_object_part_object_get(group->obj, part->name);
   obj_style = evas_object_textblock_style_get(get_style);
   text = evas_textblock_style_get(obj_style);
   ts = evas_textblock_style_new();
   evas_textblock_style_set(ts, text);
   evas_object_textblock_style_set(part->obj, ts);

   text = edje_edit_state_text_get(group->obj,
                                   part->name,
                                   part->curr_state,
                                   part->curr_state_value);
   if (text) evas_object_textblock_text_markup_set(part->obj, text);
   else
     {
        evas_object_textblock_text_markup_set(part->obj, part->name);
        edje_object_part_text_set(group->obj, part->name, part->name);
     }
   edje_edit_string_free(text);
}

static void
_gs_text_update(Group *group, Part *part)
{
   const char *text, *font;
   int text_size;
   int r, g, b, a;

   edje_edit_part_selected_state_set(group->obj,
                                     part->name,
                                     part->curr_state,
                                     part->curr_state_value);

   text = edje_edit_state_text_get(group->obj,
                                   part->name,
                                   part->curr_state,
                                   part->curr_state_value);
   if (text) evas_object_text_text_set(part->obj, text);
   else
     {
        evas_object_text_text_set(part->obj, part->name);
        edje_object_part_text_set(group->obj, part->name, part->name);
     }

   font = edje_edit_state_font_get(group->obj, part->name,
                                   part->curr_state, part->curr_state_value);
   text_size = edje_edit_state_text_size_get(group->obj, part->name,
                                             part->curr_state, part->curr_state_value);
   evas_object_text_font_set(part->obj, font, text_size);

   edje_edit_state_color_get(group->obj, part->name,
                             part->curr_state, part->curr_state_value,
                             &r, &g, &b, &a);
   evas_object_color_set(part->obj, r, g, b, a);

   edje_edit_string_free(text);
   edje_edit_string_free(font);
}

static void
_gs_spaser_swallow_group_update(Group *group,
                          Part *part)
{
   edje_edit_part_selected_state_set(group->obj,
                                     part->name,
                                     part->curr_state,
                                     part->curr_state_value);
}

/*
static void
_gs_group_part_update(Group *group,
                      Part *part)
                      //const char *state,
                      //double value)
{
   edje_edit_part_selected_state_set(group->obj, part->name, state, value);
}
*/

static void
__box_recalc(void *data,
             Evas *e __UNUSED__,
             Evas_Object *obj __UNUSED__,
             void *ei __UNUSED__)
{
   Evas_Object *box = (Evas_Object *)data;
   evas_object_smart_calculate(box);
}

static void
_gs_group_draw(Group *group,
               Workspace *ws,
               Project *project)
{
   Part *_part;
   const Evas_Object *edje_part = NULL;
   Evas_Object *box =  evas_object_data_get(ws->groupspace, GS_BOX_KEY);
   const char *source;
   Edje_Part_Type type;
   if (!ws->canvas) return;
   edje_object_scale_set(group->obj, ws->zoom_step);
   EINA_INLIST_FOREACH(group->parts, _part)
     {
        type = edje_edit_part_type_get(group->obj, _part->name);
        if (type == EDJE_PART_TYPE_RECTANGLE)
          {
             if (!_part->obj) _part->obj = evas_object_rectangle_add(ws->canvas);
             _gs_rect_update(group, _part);
          }
        if (type == EDJE_PART_TYPE_IMAGE)
          {
             if (!_part->obj) _part->obj = evas_object_image_filled_add(ws->canvas);
             _gs_image_update(group, _part, project);
          }
        if (type == EDJE_PART_TYPE_SPACER)
          {
            if (!_part->obj)  _part->obj = evas_object_image_add(ws->canvas);
             evas_object_image_file_set(_part->obj,
               TET_IMG_PATH"swallow_spacer_mask.png", NULL);
             evas_object_image_fill_set(_part->obj, 2, 2, 10, 10);
             evas_object_color_set(_part->obj, 15, 60, 162, 255);
             _gs_spaser_swallow_group_update(group, _part);
          }
        if (type == EDJE_PART_TYPE_SWALLOW)
          {
             if (!_part->obj) _part->obj = evas_object_image_add(ws->canvas);
             evas_object_image_file_set(_part->obj,
               TET_IMG_PATH"swallow_spacer_mask.png", NULL);
             evas_object_image_fill_set(_part->obj, 2, 2, 10, 10);
             evas_object_color_set(_part->obj, 180, 108, 0, 255);
             _gs_spaser_swallow_group_update(group, _part);
          }
        if (type == EDJE_PART_TYPE_TEXT)
          {
             if (!_part->obj) _part->obj = evas_object_text_add(ws->canvas);
             _gs_text_update(group, _part);
          }
        if (type == EDJE_PART_TYPE_TEXTBLOCK)
          {
             if (!_part->obj) _part->obj = evas_object_textblock_add(ws->canvas);
             _gs_textblock_update(group, _part);
          }
        if (type == EDJE_PART_TYPE_GROUP)
          {
             source = edje_edit_part_source_get(group->obj, _part->name);
             if (!_part->obj) _part->obj = wm_group_object_find(project->widgets, source);
             _gs_spaser_swallow_group_update(group, _part);
             edje_edit_string_free(source);
          }

        _part->show = EINA_TRUE;
        evas_object_event_callback_add(_part->obj, EVAS_CALLBACK_MOUSE_MOVE,
                                       _gs_mouse_move_cb, ws);
        edje_part = evas_object_data_get(_part->obj, GS_PART_DATA_KEY);
        if (!edje_part)
          {
             edje_part = edje_object_part_object_get(group->obj, _part->name);
             evas_object_data_set(_part->obj, GS_PART_DATA_KEY, edje_part);
          }

        evas_object_event_callback_add((Evas_Object *)edje_part,
                                       EVAS_CALLBACK_MOVE,
                                       __box_recalc, box);
        evas_object_event_callback_add((Evas_Object *)edje_part,
                                       EVAS_CALLBACK_MOVE,
                                       _gs_hilight_move_resize, ws);

        evas_object_event_callback_add((Evas_Object *)edje_part,
                                       EVAS_CALLBACK_RESIZE,
                                       __box_recalc, box);
        evas_object_event_callback_add((Evas_Object *)edje_part,
                                       EVAS_CALLBACK_RESIZE,
                                       _gs_hilight_move_resize, ws);

        evas_object_box_append(box, _part->obj);
        evas_object_show(_part->obj);
     }
   evas_object_show(box);
}

const Evas_Object *
_gs_group_part_get(Group *group, const char *name)
{
   Part *part = NULL;

   EINA_INLIST_FOREACH(group->parts, part)
     {
        if (!strcmp(part->name, name))
          return evas_object_data_get(part->obj, GS_PART_DATA_KEY);
     }
   return NULL;
}

void
ui_groupsapce_part_space_geometry_get(Group *group, Part *part,
                                      int *x, int *y, int *w, int *h)
{
   Evas_Object *edje_part = NULL;
   const char *state_name = NULL;
   double state_val = 0.0;

   const char *rel_to = NULL;
   const Evas_Object *rel_to_object = NULL;
   int r_x, r_y, r_h, r_w;
   int g_x, g_y, g_h, g_w;

   double relative_val = 0;

   if ((!group) || (!part))
     {
        ERR("Coud'nt take relative space geometry");
        return;
     }

   edje_part = evas_object_data_get(part->obj, GS_PART_DATA_KEY);
   if (!edje_part)
     {
        ERR("Coud'nt take edje object from part");
        return;
     }
   state_name = edje_object_part_state_get(group->obj, part->name, &state_val);

   rel_to = edje_edit_state_rel1_to_x_get(group->obj, part->name, state_name,
                                          state_val);
   relative_val = edje_edit_state_rel1_relative_x_get(group->obj, part->name,
                                                      state_name, state_val);
   evas_object_geometry_get(group->obj, &g_x, &g_y, &g_w, &g_h);
   if (rel_to)
     {
         rel_to_object = _gs_group_part_get(group, rel_to);
         if (!rel_to_object)
           {
              ERR("Failed get rel 1 to x object");
              return;
           }
         else
           {
              evas_object_geometry_get(rel_to_object, &r_x, NULL, &r_w, NULL);
              *x = r_x + r_w * relative_val;
           }
     }
   else
     *x = g_x + g_w * relative_val;

   rel_to = edje_edit_state_rel1_to_y_get(group->obj, part->name, state_name,
                                          state_val);
   relative_val = edje_edit_state_rel1_relative_y_get(group->obj, part->name,
                                                      state_name, state_val);
   if (rel_to)
     {
         rel_to_object = _gs_group_part_get(group, rel_to);
         if (!rel_to_object)
           {
              ERR("Failed get rel 1 to y object");
              return;
           }
         else
           {
              evas_object_geometry_get(rel_to_object, NULL, &r_y, NULL, &r_h);
              *y = r_y + r_h * relative_val;
           }
     }
   else
        *y = g_y + g_h * relative_val;

   rel_to = edje_edit_state_rel2_to_x_get(group->obj, part->name, state_name,
                                          state_val);
   relative_val = edje_edit_state_rel2_relative_x_get(group->obj, part->name,
                                                      state_name, state_val);
   if (rel_to)
     {
         rel_to_object = _gs_group_part_get(group, rel_to);
         if (!rel_to_object)
           {
              ERR("Failed get rel 2 to x object");
              return;
           }
         else
           {
              evas_object_geometry_get(rel_to_object, &r_x, NULL, &r_w, NULL);
              *w = (r_x + r_w * relative_val) - *x;
           }
     }
   else
     *w = g_w * relative_val - (*x - g_x);

   rel_to = edje_edit_state_rel2_to_y_get(group->obj, part->name, state_name,
                                          state_val);
   relative_val = edje_edit_state_rel2_relative_y_get(group->obj, part->name,
                                                      state_name, state_val);
   if (rel_to)
     {
         rel_to_object = _gs_group_part_get(group, rel_to);
         if (!rel_to_object)
           {
              ERR("Failed get rel 2 to y object");
              return;
           }
         else
           {
              evas_object_geometry_get(rel_to_object, NULL, &r_y, NULL, &r_h);
              *h = (r_y + r_h * relative_val) - *y;
           }
     }
   else
     *h = g_h * relative_val - (*y - g_y);

}

const Evas_Object *
ui_groupspace_part_edje_get(Part *part)
{
   return evas_object_data_get(part->obj, GS_PART_DATA_KEY);
}

Group *
ui_groupspace_group_get(Evas_Object *groupspace)
{
   return evas_object_data_get(groupspace, GS_GROUP_KEY);
}

Evas_Object *
ui_groupspace_box_get(Evas_Object *groupspace)
{
   return evas_object_data_get(groupspace, GS_BOX_KEY);
}

Eina_Bool
ui_groupspace_part_state_add(Evas_Object *groupspace, Part *part,
                             char *state_name, double state_value)
{
   Group *group = evas_object_data_get(groupspace, GS_GROUP_KEY);
   if (!edje_edit_state_add(group->obj, part->name, state_name, state_value))
     {
        if (edje_edit_state_exist(group->obj, part->name, state_name, state_value))
          {
             NOTIFY_WARNING("State alredy exist, try another name or value");
          }
        else
          NOTIFY_ERROR("Coud'nt add new state to part [%s]", part->name);
        return EINA_FALSE;
     }

   return EINA_TRUE;
}

void
ui_groupspace_part_state_update(Evas_Object *groupspace,
                                Part *part)
{
   Group *group =  evas_object_data_get(groupspace, GS_GROUP_KEY);
   Project *project = evas_object_data_get(groupspace, GS_PROJECT_KEY);
   Evas_Object *box = evas_object_data_get(groupspace, GS_BOX_KEY);
   Edje_Part_Type type = edje_edit_part_type_get(group->obj, part->name);

   if (type == EDJE_PART_TYPE_IMAGE)
     _gs_image_update(group, part, project);
   if (type == EDJE_PART_TYPE_RECTANGLE)
     _gs_rect_update(group, part);
   if (type == EDJE_PART_TYPE_TEXT)
     _gs_text_update(group, part);
   if (type == EDJE_PART_TYPE_SPACER)
     _gs_spaser_swallow_group_update(group, part);
   if (type == EDJE_PART_TYPE_SWALLOW)
     _gs_spaser_swallow_group_update(group, part);
   if (type == EDJE_PART_TYPE_GROUP)
     _gs_spaser_swallow_group_update(group, part);
   if (type == EDJE_PART_TYPE_TEXTBLOCK)
     _gs_textblock_update(group, part);

   /* FIXME:  */
   evas_object_smart_calculate(box);
}

Evas_Object *
ui_groupspace_add(Evas_Object *parent)
{
   Evas_Object *_groupspace = NULL;
   Evas_Object *_box = NULL;
   _groupspace = elm_layout_add(parent);
   elm_layout_file_set(_groupspace, TET_EDJ, "base/groupspace");

   _box = evas_object_box_add(evas_object_evas_get(parent));
   elm_object_part_content_set(_groupspace, "groupspace", _box);

   evas_object_hide(parent);
   evas_object_data_set(_groupspace, GS_BOX_KEY, _box);
   evas_object_hide(_groupspace);
   return _groupspace;
}

void
ui_groupspace_set(Workspace *ws, Project *project, Group *group)
{
   const Evas_Object *part_bottom = edje_object_part_object_get(
      elm_layout_edje_get(ws->groupspace), "bottom_pad");
   const Evas_Object *part_top = edje_object_part_object_get(
      elm_layout_edje_get(ws->groupspace), "top_pad");
   int x, y, gw, gh, w, h;
   double dx, dy;
   Evas_Object *_box = evas_object_data_get(ws->groupspace, GS_BOX_KEY);

   if (group)
     {
        elm_object_part_content_set(ws->groupspace, "edje_group", group->obj);
        evas_object_box_layout_set(_box, _main_box_layout, group->obj, NULL);
        evas_object_show(group->obj);
     }
   else
     WARN("Edje edit group object was deleted. Could'nt set it into groupspace");
   elm_layout_signal_emit (ws->groupspace, "groupspace,show", "");

   elm_config_cursor_engine_only_set(EINA_FALSE);
   elm_object_cursor_set((Evas_Object *)part_top, "top_left_corner");
   elm_object_cursor_set((Evas_Object *)part_bottom, "bottom_right_corner");
   elm_config_cursor_engine_only_set(EINA_TRUE);

   evas_object_geometry_get(ws->groupspace, NULL, NULL, &w, &h);
   elm_scroller_region_get(ws->scroller, NULL, NULL, &gw, &gh);
   x = (int)((w - gw) / 2);
   y = (int)((h - gh) / 2);
   elm_scroller_region_bring_in(ws->scroller, x, y, gw, gh);

   /*
    *  value 0.01 needed for groupspace offset from top left and bottom right
    *  corners workspace area
    */
   dx = (double)x / w + 0.01;
   dy = (double)y / h + 0.01;
   edje_object_part_drag_value_set(elm_layout_edje_get(ws->groupspace),
                                   "top_pad", dx, dy);
   dx = (double)(x + gw) / w - 0.01;
   dy = (double)(y + gh) / h - 0.01;
   edje_object_part_drag_value_set(elm_layout_edje_get(ws->groupspace),
                                   "bottom_pad", dx, dy);

   evas_object_event_callback_add(ws->groupspace, EVAS_CALLBACK_MOUSE_MOVE,
                                  _gs_mouse_move_cb, ws);
   evas_object_smart_callback_add(group->obj, "gs,layer,up", _layer_up_change, ws);
   evas_object_smart_callback_add(group->obj, "gs,layer,down", _layer_down_change, ws);
   evas_object_smart_callback_add(group->obj, "gs,part,delete", _part_delete, ws);
   evas_object_smart_callback_add(group->obj, "gs,part,add", _part_add, ws);
   evas_object_smart_callback_add(group->obj, "gs,state,add", _state_add, ws);

   evas_object_smart_callback_add(ws->groupspace, "gs,rect,add", _new_rect_add, ws);
   evas_object_smart_callback_add(ws->groupspace, "gs,img,add", _new_img_add, ws);
   evas_object_smart_callback_add(ws->groupspace, "gs,txt,add", _new_txt_add, ws);
   evas_object_smart_callback_add(ws->groupspace, "gs,swallow,add", _new_swallow_add, ws);
   evas_object_smart_callback_add(ws->groupspace, "gs,txtblock,add", _new_txtblock_add, ws);
   evas_object_smart_callback_add(ws->groupspace, "gs,spacer,add", _new_spacer_add, ws);
   evas_object_smart_callback_add(ws->groupspace, "gs,new_state,add", _new_state_add, ws);

   evas_object_event_callback_add(group->obj, EVAS_CALLBACK_RESIZE,
                                  _gs_resize_cb, ws);
   evas_object_data_set(ws->groupspace, GS_GROUP_KEY, group);
   evas_object_data_set(ws->groupspace, GS_WS_KEY, ws);
   evas_object_data_set(ws->groupspace, GS_PROJECT_KEY, project);

   ui_ruler_scale_absolute_dashes_step_set(ws->ruler_hor, 5);
   ui_ruler_scale_absolute_dashes_step_set(ws->ruler_ver, 5);
   ui_ruler_redraw(ws->ruler_hor);
   ui_ruler_redraw(ws->ruler_ver);
}

void
ui_groupspace_update(Evas_Object *obj)
{
   Workspace *ws = evas_object_data_get(obj, GS_WS_KEY);
   Group *group =  evas_object_data_get(obj, GS_GROUP_KEY);
   Project *project = evas_object_data_get(obj, GS_PROJECT_KEY);
   _gs_group_draw(group, ws, project);
   evas_object_show(obj);
}

void
ui_groupspace_unset(Evas_Object *obj)
{
   Eina_List *list = NULL;
   Eina_List *l = NULL;
   Evas_Object *data = NULL;
   Workspace *ws = evas_object_data_get(obj, GS_WS_KEY);
   if (!ws) return;
   Evas_Object *box = evas_object_data_get(ws->groupspace, GS_BOX_KEY);
   Group *group = evas_object_data_del(ws->groupspace, GS_GROUP_KEY);

   evas_object_smart_callback_del(group->obj, "gs,layer,up", _layer_up_change);
   evas_object_smart_callback_del(group->obj, "gs,layer,down", _layer_down_change);
   evas_object_smart_callback_del(group->obj, "gs,part,delete", _part_delete);
   evas_object_smart_callback_del(group->obj, "gs,part,add", _part_add);
   evas_object_smart_callback_del(group->obj, "gs,state,add", _state_add);

   evas_object_smart_callback_del(ws->groupspace, "gs,rect,add", _new_rect_add);
   evas_object_smart_callback_del(ws->groupspace, "gs,img,add", _new_img_add);
   evas_object_smart_callback_del(ws->groupspace, "gs,txt,add", _new_txt_add);
   evas_object_smart_callback_del(ws->groupspace, "gs,swallow,add", _new_swallow_add);
   evas_object_smart_callback_del(ws->groupspace, "gs,txtblock,add", _new_txtblock_add);
   evas_object_smart_callback_del(ws->groupspace, "gs,spacer,add", _new_spacer_add);

   list = evas_object_box_children_get(box);
   EINA_LIST_FOREACH(list, l, data)
        evas_object_hide(data);
   eina_list_free(list);
   evas_object_box_remove_all(box, EINA_FALSE);
   evas_object_hide(box);
   evas_object_del(ws->highlight.space_hl);
   evas_object_del(ws->highlight.highlight);
   ws->highlight.highlight = NULL;
   ws->highlight.space_hl = NULL;
   elm_object_part_content_unset(ws->groupspace, "edje_group");
   elm_layout_signal_emit (ws->groupspace, "groupspace,hide", "");
}

void
ui_groupspace_separate(Workspace *ws)
{
   Evas_Object *box = evas_object_data_get(ws->groupspace, GS_BOX_KEY);
   Group *group = evas_object_data_get(ws->groupspace, GS_GROUP_KEY);
   if ((!group) || (!box)) return;
   if (ws->separated)
     evas_object_box_layout_set(box, _separate_layout, group->obj, NULL);
   else
     evas_object_box_layout_set(box, _main_box_layout, group->obj, NULL);
   ws->separated = !ws->separated;

   if (ws->highlight.highlight)
     {
        evas_object_del(ws->highlight.highlight);
        evas_object_del(ws->highlight.space_hl);
        ws->highlight.highlight = NULL;
        ws->highlight.space_hl = NULL;
     }
}
