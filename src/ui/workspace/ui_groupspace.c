#include "ui_groupspace.h"
#include "ui_highlight.h"

#define GS_WS_KEY "gs_workspace_key"
#define GS_PROJECT_KEY "gs_project_key"
#define GS_BOX_KEY "gs_box_key"
#define GS_PART_DATA_KEY "gs_part_key"
#define GS_GROUP_KEY "gs_group_key"

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
_gs_resize_cb(void *data, Evas *e __UNUSED__, Evas_Object *obj,
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

   evas_object_geometry_get(ws->highlight.obj, &x, &y, &w, &h);
   evas_object_move(ws->highlight.highlight, x, y);
   evas_object_resize(ws->highlight.highlight, w, h);
}

static void
_gs_image_update(Group *group,
                 Part *part,
                 const char *state,
                 double value,
                 Project *project)
{
   Evas_Load_Error err;
   char buf[BUFF_MAX];
   const char *image_normal;
   int id;
   int bl, br, bt, bb;
   int r, g, b, a;

   edje_edit_part_selected_state_set(group->obj, part->name, state, value);

   image_normal = edje_edit_state_image_get(group->obj, part->name, state, value);
   id = edje_edit_image_id_get(group->obj, image_normal);
   snprintf(buf, sizeof(buf), "edje/images/%i", id);
   evas_object_image_file_set(part->obj, project->swapfile, buf);

   err = evas_object_image_load_error_get(part->obj);
   if (err != EVAS_LOAD_ERROR_NONE)
     WARN("Could not update image. Error string is \"%s\"\n", evas_load_error_str(err));

   edje_edit_state_color_get(group->obj, part->name, state, value, &r, &g, &b, &a);
   evas_object_color_set(part->obj, r * a / 255, g * a / 255, b * a / 255, a);

   edje_edit_state_image_border_get(group->obj, part->name, state, value,
                                    &bl, &br, &bt, &bb);
   evas_object_image_border_set(part->obj, bl, br, bt, bb);

   evas_object_image_fill_set(part->obj,
                    edje_edit_state_fill_origin_relative_x_get(group->obj, part->name, state, value),
                    edje_edit_state_fill_origin_relative_y_get(group->obj, part->name, state, value),
                    0, 0);

   edje_edit_string_free(image_normal);
}

static void
_gs_rect_update(Group *group,
                Part *part,
                const char *state,
                double value)
{
   int r, g, b, a;
   edje_edit_part_selected_state_set(group->obj, part->name, state, value);

   edje_edit_state_color_get(group->obj, part->name, state, value,
                             &r, &g, &b, &a);
   evas_object_color_set(part->obj, r * a / 255, g * a / 255, b *a / 255, a);
}

static void
_gs_textblock_update(Group *group, Part *part, const char *state, double value)
{
   Evas_Textblock_Style *ts = NULL;
   const Evas_Textblock_Style *obj_style;
   const Evas_Object *get_style;
   const char *text;

   edje_edit_part_selected_state_set(group->obj, part->name, state, value);
   get_style = edje_object_part_object_get(group->obj, part->name);
   obj_style = evas_object_textblock_style_get(get_style);
   text = evas_textblock_style_get(obj_style);
   ts = evas_textblock_style_new();
   evas_textblock_style_set(ts, text);
   evas_object_textblock_style_set(part->obj, ts);

   text = edje_edit_state_text_get(group->obj, part->name, state, value);
   if (text) evas_object_textblock_text_markup_set(part->obj, text);
   else
     {
        evas_object_textblock_text_markup_set(part->obj, part->name);
        edje_object_part_text_set(group->obj, part->name, part->name);
     }

   edje_edit_string_free(text);
}

static void
_gs_text_update(Group *group, Part *part, const char *state, double value)
{
   const char *text, *font;
   int text_size;
   int r, g, b, a;

   edje_edit_part_selected_state_set(group->obj, part->name, state, value);

   text = edje_edit_state_text_get(group->obj, part->name, state, value);
   if (text) evas_object_text_text_set(part->obj, text);
   else
     {
        evas_object_text_text_set(part->obj, part->name);
        edje_object_part_text_set(group->obj, part->name, part->name);
     }

   font = edje_edit_state_font_get(group->obj, part->name, state, value);
   text_size = edje_edit_state_text_size_get(group->obj, part->name, state, value);
   evas_object_text_font_set(part->obj, font, text_size);

   edje_edit_state_color_get(group->obj, part->name, state, value,
                             &r, &g, &b, &a);
   evas_object_color_set(part->obj, r, g, b, a);

   edje_edit_string_free(text);
   edje_edit_string_free(font);
}

static void
_gs_spaser_swallow_update(Group *group,
                          Part *part,
                          const char *state,
                          double value)
{
   edje_edit_part_selected_state_set(group->obj, part->name, state, value);
}

static void
_gs_group_part_update(Group *group, Part *part, const char *state, double value)
{
   edje_edit_part_selected_state_set(group->obj, part->name, state, value);
}

static void
_gs_group_draw(Group *group,
               Workspace *ws,
               Project *project __UNUSED__)
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
             _gs_rect_update(group, _part, "default", 0.0);
          }
        if (type == EDJE_PART_TYPE_IMAGE)
          {
             if (!_part->obj) _part->obj = evas_object_image_filled_add(ws->canvas);
             _gs_image_update(group, _part, "default", 0.0, project);
          }
        if (type == EDJE_PART_TYPE_SPACER)
          {
            if (!_part->obj)  _part->obj = evas_object_image_add(ws->canvas);
             evas_object_image_file_set(_part->obj,
               TET_IMG_PATH"swallow_spacer_mask.png", NULL);
             evas_object_image_fill_set(_part->obj, 2, 2, 10, 10);
             evas_object_color_set(_part->obj, 15, 60, 162, 255);
             _gs_spaser_swallow_update(group, _part, "default", 0.0);
          }
        if (type == EDJE_PART_TYPE_SWALLOW)
          {
             if (!_part->obj) _part->obj = evas_object_image_add(ws->canvas);
             evas_object_image_file_set(_part->obj,
               TET_IMG_PATH"swallow_spacer_mask.png", NULL);
             evas_object_image_fill_set(_part->obj, 2, 2, 10, 10);
             evas_object_color_set(_part->obj, 180, 108, 0, 255);
             _gs_spaser_swallow_update(group, _part, "default", 0.0);
          }
        if (type == EDJE_PART_TYPE_TEXT)
          {
             if (!_part->obj) _part->obj = evas_object_text_add(ws->canvas);
             _gs_text_update(group, _part, "default", 0.0);
          }
        if (type == EDJE_PART_TYPE_TEXTBLOCK)
          {
             if (!_part->obj) _part->obj = evas_object_textblock_add(ws->canvas);
             _gs_textblock_update(group, _part, "default", 0.0);
          }
        if (type == EDJE_PART_TYPE_GROUP)
          {
             source = edje_edit_part_source_get(group->obj, _part->name);
             if (!_part->obj) _part->obj = wm_group_object_find(project->widgets, source);
             _gs_group_part_update(group, _part, "default", 0.0);
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

        evas_object_box_append(box, _part->obj);
        evas_object_show(_part->obj);
     }
   evas_object_show(box);
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

void
ui_groupspace_part_state_update(Evas_Object *groupspace,
                                Part *part,
                                const char *state,
                                double value)
{
   Group *group =  evas_object_data_get(groupspace, GS_GROUP_KEY);
   Project *project = evas_object_data_get(groupspace, GS_PROJECT_KEY);
   Evas_Object *box = evas_object_data_get(groupspace, GS_BOX_KEY);
   Edje_Part_Type type = edje_edit_part_type_get(group->obj, part->name);

   if (type == EDJE_PART_TYPE_IMAGE)
     _gs_image_update(group, part, state, value, project);
   if (type == EDJE_PART_TYPE_RECTANGLE)
     _gs_rect_update(group, part, state, value);
   if (type == EDJE_PART_TYPE_TEXT)
     _gs_text_update(group, part, state, value);
   if (type == EDJE_PART_TYPE_SPACER)
     _gs_spaser_swallow_update(group, part, state, value);
   if (type == EDJE_PART_TYPE_SWALLOW)
     _gs_spaser_swallow_update(group, part, state, value);
   if (type == EDJE_PART_TYPE_GROUP)
     _gs_group_part_update(group, part, state, value);
   if (type == EDJE_PART_TYPE_TEXTBLOCK)
     _gs_textblock_update(group, part, state, value);

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

   elm_object_cursor_set((Evas_Object *)part_top, "top_left_corner");
   elm_object_cursor_set((Evas_Object *)part_bottom, "bottom_right_corner");

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
   Evas_Object *box = evas_object_data_get(ws->groupspace, GS_BOX_KEY);
   evas_object_data_del(ws->groupspace, GS_GROUP_KEY);

   list = evas_object_box_children_get(box);
   EINA_LIST_FOREACH(list, l, data)
        evas_object_hide(data);
   eina_list_free(list);
   evas_object_box_remove_all(box, EINA_FALSE);
   evas_object_hide(box);
   evas_object_del(ws->highlight.highlight);
   ws->highlight.highlight = NULL;
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
        ws->highlight.highlight = NULL;
     }
}
