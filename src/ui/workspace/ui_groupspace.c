#include "ui_groupspace.h"

#define GS_GROUP_KEY "gs_group_key"
#define GS_WS_KEY "gs_workspace_key"
#define GS_PROJECT_KEY "gs_project_key"
#define GS_BOX_KEY "gs_box_key"
#define GS_PART_DATA_KEY "gs_part_key"

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
        evas_object_geometry_get(edje_part, &x, &y, &w, &h);
        evas_object_move(opt->obj, x, y);
        evas_object_resize(opt->obj, w, h);
        evas_object_geometry_get(opt->obj, &x, &y, &w, &h);
    }
}

static void
_separate_layout(Evas_Object *o, Evas_Object_Box_Data *p, void *data)
{
   int x, y, w, h;
   int xx, yy;
   Evas_Object *edje_part;
   Evas_Object *edje_group = (Evas_Object *)data;
   Eina_List *l;
   Evas_Object_Box_Option *opt;
   int count = 0;

   evas_object_geometry_get(o, &xx, &yy, &w, &h);
   evas_object_resize(edje_group, w, h);
   evas_object_move(edje_group, xx, yy);
   evas_object_geometry_get(edje_group, &xx, &yy, &w, &h);
   EINA_LIST_FOREACH(p->children, l, opt)
     {
        edje_part = evas_object_data_get(opt->obj, GS_PART_DATA_KEY);
        evas_object_geometry_get(edje_part, &x, &y, &w, &h);
        evas_object_move(opt->obj, xx + count * 50, yy + count * 20);
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

   ui_object_highlight_move(ws);
}

static void
_gs_image_update(Project *project, Group *group, Part *part, Part_State *state)
{
   Evas_Load_Error err;
   char buf[4096];
   int x, y, w, h;

   edje_edit_part_selected_state_set(group->obj, part->name, state->name,
                                     state->value);

   snprintf(buf, sizeof(buf), "edje/images/%i", state->image->id);
   evas_object_image_file_set(part->obj, project->swapfile, buf);

   err = evas_object_image_load_error_get(part->obj);
   if (err != EVAS_LOAD_ERROR_NONE)
     WARN("Could not update image. Error string is \"%s\"\n",
           evas_load_error_str(err));

   edje_object_part_geometry_get(group->obj, part->name, &x, &y, &w, &h);

   evas_object_image_border_set(part->obj, state->image->left,
      state->image->right, state->image->top, state->image->bottom);


   evas_object_image_fill_set(part->obj,
      state->image->fill_origin_relative_x,
      state->image->fill_origin_relative_y, w, h);

   evas_object_move (part->obj, x, y);
   evas_object_resize (part->obj, w, h);

   if(state->color[3])
     evas_object_color_set(part->obj, state->color[0], state->color[1],
                           state->color[2], state->color[3]);
   else
     evas_object_color_set(part->obj, 0, 0, 0, 0);

   if (state->visible)
     {
        evas_object_show(part->obj);
        part->show = EINA_TRUE;
     }
   else
     {
        evas_object_hide(part->obj);
        part->show = EINA_FALSE;
     }
}

static Evas_Object *
_gs_image_draw(Evas *canvas, Group *group , Part *part,	Project *project,
               Part_State *state)
{
   Evas_Object *_image;
   char buf[BUFF_MAX];
   int x, y, w, h;

   Part_State *_current_state = NULL;
   if (!state)
     _current_state = EINA_INLIST_CONTAINER_GET(part->states, Part_State);
   else
     _current_state = state;

   _image = evas_object_image_filled_add(canvas);

   snprintf(buf, sizeof(buf), "edje/images/%i", _current_state->image->id);
   evas_object_image_file_set(_image, project->swapfile, buf);

   edje_object_part_geometry_get(group->obj, part->name, &x, &y, &w, &h);

   evas_object_move(_image, x, y);
   evas_object_resize(_image, w, h);

   evas_object_image_border_set(_image, _current_state->image->left,
      _current_state->image->right, _current_state->image->top,
      _current_state->image->bottom);

   evas_object_image_fill_set(_image,
      _current_state->image->fill_origin_relative_x,
      _current_state->image->fill_origin_relative_y, w, h);

   if(_current_state->color[3])
     evas_object_color_set(_image, _current_state->color[0],
        _current_state->color[1],_current_state->color[2],
        _current_state->color[3]);
   else
     evas_object_color_set(_image, 0, 0, 0, 0);

   if (_current_state->visible)
     {
        evas_object_show(_image);
        part->show = EINA_TRUE;
     }
   else
     {
        evas_object_hide(part->obj);
        part->show = EINA_FALSE;
     }
  return _image;
}

static void
_gs_rect_update(Group *group, Part *part, Part_State *state)
{
   int x, y, w, h;

   edje_edit_part_selected_state_set(group->obj, part->name, state->name,
                                     state->value);

   edje_object_part_geometry_get (group->obj, part->name, &x, &y, &w, &h);
   evas_object_move (part->obj, x, y);
   evas_object_resize (part->obj, w, h);

   if(state->color[3])
     evas_object_color_set(part->obj, state->color[0], state->color[1],
                           state->color[2], state->color[3]);
   else
     evas_object_color_set(part->obj, 0, 0, 0, 0);

   if (state->visible)
     {
        evas_object_show(part->obj);
        part->show = EINA_TRUE;
     }
   else
     {
        evas_object_hide(part->obj);
        part->show = EINA_FALSE;
     }
}

static Evas_Object *
_gs_rect_draw(Evas *canvas, Group *group, Part *part, Part_State *state)
{
   Evas_Object *_rectangle;
   int x, y, w, h;
   Part_State *_current_state = NULL;

   if (!state)
     _current_state = EINA_INLIST_CONTAINER_GET(part->states, Part_State);
   else
     _current_state = state;

   if (!canvas)
     ERR ("Coud not to take canvas");
   edje_object_part_geometry_get(group->obj, part->name, &x, &y, &w, &h);

   _rectangle = evas_object_rectangle_add(canvas);

   evas_object_move(_rectangle, x, y);
   evas_object_resize(_rectangle, w, h);

   if(_current_state->color[3])
     evas_object_color_set(_rectangle, _current_state->color[0],
        _current_state->color[1],_current_state->color[2],
        _current_state->color[3]);
   else
     evas_object_color_set(_rectangle, 0, 0, 0, 0);

   if (_current_state->visible)
     {
        evas_object_show(_rectangle);
        part->show = EINA_TRUE;
     }
   else
     part->show = EINA_FALSE;
   return _rectangle;
}

static void
_gs_textblock_update(Group *group, Part *part, Part_State *state)
{
   Evas_Textblock_Style *ts = NULL;
   int x, y, w, h;

   edje_edit_part_selected_state_set(group->obj, part->name, state->name,
                                     state->value);

   edje_edit_state_text_set(group->obj, part->name, state->name,
                            state->value, part->name);
   edje_object_part_geometry_get(group->obj, part->name, &x, &y, &w, &h);

   ts = evas_textblock_style_new();

   evas_textblock_style_set(ts,
      evas_textblock_style_get(
         evas_object_textblock_style_get(
            edje_object_part_object_get(group->obj, part->name))));

   evas_object_textblock_style_set(part->obj, ts);

   evas_object_textblock_text_markup_set(part->obj,
      edje_edit_state_text_get(group->obj, part->name, state->name,
                               state->value));

   evas_object_textblock_valign_set(part->obj, state->align_y);

   evas_object_move (part->obj, x, y);
   evas_object_resize (part->obj, w, h);

   if (state->visible)
     {
        evas_object_show(part->obj);
        part->show = EINA_TRUE;
     }
   else
     {
        evas_object_hide(part->obj);
        part->show = EINA_FALSE;
     }

}

static Evas_Object *
_gs_textblock_draw(Evas *canvas, Group *group, Part *part,
                   Part_State *state)
{
   Evas_Object *_textblock = NULL;
   int x, y, w, h;
   Evas_Textblock_Style *ts = NULL;

   Part_State *_current_state = NULL;
   if (!state)
     _current_state = EINA_INLIST_CONTAINER_GET(part->states, Part_State);
   else
     _current_state = state;

   edje_edit_state_text_set(group->obj, part->name, _current_state->name,
                            _current_state->value, part->name);
   edje_object_part_geometry_get(group->obj, part->name, &x, &y, &w, &h);

   _textblock = evas_object_textblock_add(canvas);

   ts = evas_textblock_style_new();

/**
 * I know that its non readable code. Later i fix it.
 * It's take style from edje object and implement this style
 * to non constant style'
 */
   evas_textblock_style_set(ts,
      evas_textblock_style_get(
         evas_object_textblock_style_get(
            edje_object_part_object_get(group->obj, part->name))));

   evas_object_textblock_style_set(_textblock, ts);

   evas_object_textblock_text_markup_set(_textblock,
      edje_edit_state_text_get(group->obj, part->name, _current_state->name,
                               _current_state->value));

   evas_object_textblock_valign_set(_textblock, _current_state->align_y);

   evas_object_move(_textblock, x, y);
   evas_object_resize(_textblock, w, h);
   if (_current_state->visible)
     {
        evas_object_show(_textblock);
        part->show = EINA_TRUE;
     }
   else
     part->show = EINA_FALSE;

   return _textblock;
}

static void
_gs_text_update(Group *group, Part *part, Part_State *state)
{
   int x, y, w, h;

   edje_edit_part_selected_state_set(group->obj, part->name, state->name,
                                     state->value);

   edje_edit_state_text_set(group->obj, part->name, state->name,
                            state->value, part->name);
   edje_object_calc_force(group->obj);
   edje_object_part_geometry_get(group->obj, part->name, &x, &y, &w, &h);
   evas_object_text_text_set(part->obj,
                             edje_edit_state_text_get(group->obj, part->name,
                                 state->name, state->value));

   evas_object_text_font_set(part->obj, state->text->font,
      state->text->size);

   evas_object_move(part->obj,(int)( x + w * state->text->align_x),
                   (int)(y + h * state->text->align_y));
   if (w && h) evas_object_resize(part->obj, w, h);

   if(state->color[3])
     evas_object_color_set(part->obj, state->color[0], state->color[1],
                           state->color[2], state->color[3]);
   else
     evas_object_color_set(part->obj, 0, 0, 0, 0);
  if (state->visible)
     {
        evas_object_show(part->obj);
        part->show = EINA_TRUE;
     }
   else
     {
        part->show = EINA_FALSE;
        evas_object_hide(part->obj);
     }
}


static Evas_Object *
_gs_text_draw(Evas *canvas, Group *group, Part *part, Part_State *state)
{
   Evas_Object *_text = NULL;
   int x, y, w, h;
   Part_State *_current_state = NULL;
   if (!state)
     _current_state = EINA_INLIST_CONTAINER_GET(part->states, Part_State);
   else
     _current_state = state;

   edje_edit_state_text_set(group->obj, part->name, _current_state->name,
                            _current_state->value, part->name);
   edje_object_calc_force(group->obj);
   edje_object_part_geometry_get(group->obj, part->name, &x, &y, &w, &h);
   _text = evas_object_text_add(canvas);
   evas_object_text_text_set(_text,
                             edje_edit_state_text_get(group->obj, part->name,
                                _current_state->name, _current_state->value));

   evas_object_text_font_set(_text, _current_state->text->font,
      _current_state->text->size);

   evas_object_move(_text,(int)( x + w * _current_state->text->align_x),
                   (int)(y + h * _current_state->text->align_y));
   if (w && h) evas_object_resize(_text, w, h);

   if(_current_state->color[3])
     evas_object_color_set(_text, _current_state->color[0],
        _current_state->color[1],_current_state->color[2],
        _current_state->color[3]);
   else
     evas_object_color_set(_text, 0, 0, 0, 0);

   if (_current_state->visible)
     {
        evas_object_show(_text);
        part->show = EINA_TRUE;
     }
   else
     part->show = EINA_FALSE;
/**
 * Ellipsis parametr of text will be relised when EVAS library 1.8 came.
 * With method
 * void evas_object_text_ellipsis_set 	(Evas_Object *obj, double ellipsis )
 * http://docs.enlightenment.org/auto/efl/group__Evas__Object__Text.html#ga06da1d81bf84cfb6247791b4696125f9/
 */
   return _text;
}

static void
_gs_spacer_update(Group *group, Part *part, Part_State *state)
{
   int x, y, w, h;

   edje_edit_part_selected_state_set(group->obj, part->name, state->name,
                                     state->value);

   edje_object_part_geometry_get (group->obj, part->name, &x, &y, &w, &h);
   evas_object_move (part->obj, x, y);
   evas_object_resize (part->obj, w, h);
   if (state->visible)
     {
        evas_object_show(part->obj);
        part->show = EINA_TRUE;
     }
   else
     {
        part->show = EINA_FALSE;
        evas_object_hide(part->obj);
     }
}


static Evas_Object *
_gs_spacer_draw(Evas *canvas, Group *group, Part *part,
                Part_State *state)
{
   Evas_Object *_spacer = NULL;
   int x, y, w, h;
   Part_State *_current_state = NULL;
   if (!state)
     _current_state = EINA_INLIST_CONTAINER_GET(part->states, Part_State);
   else
     _current_state = state;

   _spacer = evas_object_image_add(canvas);
   evas_object_image_file_set (_spacer, TET_IMG_PATH"swallow_spacer_mask.png", NULL);
   evas_object_image_fill_set (_spacer, 2, 2, 10, 10);
   edje_object_part_geometry_get (group->obj, part->name, &x, &y, &w, &h);
   evas_object_color_set(_spacer, 15, 60, 162, 180);
   evas_object_move (_spacer, x, y);
   evas_object_resize (_spacer, w, h);
   evas_object_image_fill_spread_set (_spacer, EVAS_TEXTURE_REPEAT);

   evas_object_move(_spacer, x, y);
   evas_object_resize(_spacer, w, h);

   if (_current_state->visible)
     {
        evas_object_show(_spacer);
        part->show = EINA_TRUE;
     }
   else
     part->show = EINA_FALSE;
 return _spacer;
}

static void
_gs_swallow_update(Group *group, Part *part, Part_State *state)
{
   int x, y, w, h;

   edje_edit_part_selected_state_set(group->obj, part->name, state->name,
                                     state->value);

   edje_object_part_geometry_get (group->obj, part->name, &x, &y, &w, &h);
   evas_object_move (part->obj, x, y);
   evas_object_resize (part->obj, w, h);


  if (state->visible)
     {
        evas_object_show(part->obj);
        part->show = EINA_TRUE;
     }
   else
     {
        part->show = EINA_FALSE;
        evas_object_hide(part->obj);

     }
}


static Evas_Object *
_gs_swallow_draw(Evas *canvas, Group *group, Part *part, Part_State *state)
{
   Evas_Object *_swallow = NULL;
   int x, y, w, h;
   Part_State *_current_state = NULL;
   if (!state)
     _current_state = EINA_INLIST_CONTAINER_GET(part->states, Part_State);
   else
     _current_state = state;

   _swallow = evas_object_image_add (canvas);
   evas_object_image_file_set (_swallow, TET_IMG_PATH"swallow_spacer_mask.png",
                               NULL);
   evas_object_image_fill_set (_swallow, 2, 2, 10, 10);
   edje_object_part_geometry_get (group->obj, part->name, &x, &y, &w, &h);
   evas_object_color_set(_swallow, 219, 108, 0, 180);
   evas_object_move (_swallow, x, y);
   evas_object_resize (_swallow, w, h);
   evas_object_image_fill_spread_set (_swallow, EVAS_TEXTURE_REPEAT);

   evas_object_move(_swallow, x, y);
   evas_object_resize(_swallow, w, h);
   if (_current_state->visible)
     {
        evas_object_show(_swallow);
        part->show = EINA_TRUE;
     }
   else
     part->show = EINA_FALSE;
   return _swallow;
}

static void
_gs_group_part_update(Group *group, Part *part, Part_State * state)
{
   int x, y, w, h;

   edje_edit_part_selected_state_set(group->obj, part->name, state->name,
                                     state->value);

   edje_object_part_geometry_get (group->obj, part->name, &x, &y, &w, &h);
   evas_object_move (part->obj, x, y);
   evas_object_resize (part->obj, w, h);
   if (state->visible)
     {
        evas_object_show(part->obj);
        part->show = EINA_TRUE;
     }
   else
     {
        evas_object_hide(part->obj);
        part->show = EINA_FALSE;
     }
}


static Evas_Object *
_gs_group_part_draw(Project *project, Group *group, Part *part,
                    Part_State * state)
{
   int x, y, w, h;
   Evas_Object *_group = NULL;
   Part_State *_current_state = NULL;
   if (!state)
     _current_state = EINA_INLIST_CONTAINER_GET(part->states, Part_State);
   else
     _current_state = state;

   edje_object_part_geometry_get(group->obj, part->name, &x, &y, &w, &h);
   _group = wm_group_object_find(project->widgets, part->source);
   evas_object_move(_group, x, y);
   evas_object_resize(_group, w, h);
   if (_current_state->visible)
     {
        evas_object_show(_group);
        part->show = EINA_TRUE;
     }
   else
     part->show = EINA_FALSE;

   return _group;
}

static void
_gs_text_part_geometry_recalc(Part *part, Part_State *state, int *x, int *y,
                              int *w, int *h)
{
   int _text_size = 0;
   Part_State *_current_state = NULL;
   if(!state)
     _current_state = EINA_INLIST_CONTAINER_GET(part->states, Part_State);
   else
     _current_state = state;
   if (_current_state->text->align_x == 0.5)
     {
        if (!_current_state->text->text) _text_size = strlen(part->name);
        else _text_size =  strlen(_current_state->text->text);
     }
   *x += *w * _current_state->text->align_x +
      _current_state->rel1_offset_x -
      (_text_size * _current_state->text->size) / 4;
   *y += *h * _current_state->text->align_y +
      _current_state->rel1_offset_y - _current_state->text->size;
   *w = *w - _current_state->rel2_offset_x -
      *w * _current_state->text->align_x;
   *h = _current_state->text->size + 5;
   if (*x < 0) *x *= (-1);
   if (*h < _current_state->text->size)
     *h = _current_state->text->size + 5;
}

static void
_gs_group_draw(Group *group, Workspace *ws, Project *project)
{
   Part *_part = NULL;
   Evas_Object *edje_part;
   Evas_Object *_part_object = NULL;
   Evas_Object *box =  evas_object_data_get(ws->groupspace, GS_BOX_KEY);
   EINA_INLIST_FOREACH(group->parts, _part)
     {
        if (_part->type == EDJE_PART_TYPE_RECTANGLE)
          _part_object = _gs_rect_draw(ws->canvas, group, _part, NULL);
        if (_part->type == EDJE_PART_TYPE_IMAGE)
          _part_object = _gs_image_draw(ws->canvas, group, _part, project, NULL);
        if (_part->type == EDJE_PART_TYPE_TEXT)
          _part_object = _gs_text_draw(ws->canvas, group, _part, NULL);
        if (_part->type == EDJE_PART_TYPE_SPACER)
          _part_object = _gs_spacer_draw(ws->canvas, group, _part, NULL);
        if (_part->type == EDJE_PART_TYPE_SWALLOW)
          _part_object = _gs_swallow_draw(ws->canvas, group, _part, NULL);
        if (_part->type == EDJE_PART_TYPE_GROUP)
          _part_object = _gs_group_part_draw(project, group, _part, NULL);
        if (_part->type == EDJE_PART_TYPE_TEXTBLOCK)
          _part_object = _gs_textblock_draw(ws->canvas, group, _part, NULL);

        if (_part_object)
          {
             evas_object_event_callback_add(_part_object,
                                            EVAS_CALLBACK_MOUSE_MOVE,
                                            _gs_mouse_move_cb, ws);
             edje_part = (Evas_Object *) edje_object_part_object_get(group->obj,
                                                                _part->name);
             _part->obj = _part_object;
             evas_object_data_set(_part->obj, GS_PART_DATA_KEY, edje_part);
             evas_object_box_append(box, _part->obj);
          }
        else
          ERR ("Part object did'n create");
     }
   evas_object_show(box);
}

void
ui_groupspace_part_state_update(Part *part, Part_State *state,
                                Evas_Object *groupspace)
{
   Group *group =  evas_object_data_get(groupspace, GS_GROUP_KEY);
   Project *project = evas_object_data_get(groupspace, GS_PROJECT_KEY);
   Part *_part = NULL;
   Evas_Object *box = evas_object_data_get(groupspace, GS_BOX_KEY);

   int x, y, w, h;

   EINA_INLIST_FOREACH(group->parts, _part)
     {
        if (!strcmp(part->name, _part->name))
          {
             if (_part->type == EDJE_PART_TYPE_IMAGE)
               _gs_image_update(project, group, _part, state);
             if (_part->type == EDJE_PART_TYPE_RECTANGLE)
               _gs_rect_update(group, _part, state);
              if (_part->type == EDJE_PART_TYPE_TEXT)
                _gs_text_update(group, _part, state);
              if (_part->type == EDJE_PART_TYPE_SPACER)
                _gs_spacer_update(group, _part, state);
              if (_part->type == EDJE_PART_TYPE_SWALLOW)
                _gs_swallow_update(group, _part, state);
              if (_part->type == EDJE_PART_TYPE_GROUP)
                _gs_group_part_update(group, _part, state);
              if (_part->type == EDJE_PART_TYPE_TEXTBLOCK)
                _gs_textblock_update(group, _part, state);


             edje_object_part_geometry_get(group->obj, _part->name,
                                    &x, &y, &w, &h);
             if (_part->type == EDJE_PART_TYPE_TEXT)
               _gs_text_part_geometry_recalc(_part, state, &x, &y, &w, &h);
             evas_object_resize(box, w-1, h-1);
           break;
          }
     }
}

Evas_Object *
ui_groupspace_add(Evas_Object *parent)
{
   Evas_Object *_groupspace = NULL;

   _groupspace = elm_layout_add(parent);
   elm_layout_file_set(_groupspace, TET_EDJ, "base/groupspace");
   evas_object_hide(parent);
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
   Evas_Object *_box;
   int x, y, gw, gh, w, h;
   double dx, dy;

   _box = evas_object_box_add(ws->canvas);
   elm_object_part_content_set(ws->groupspace, "groupspace", _box);
   elm_object_part_content_set(ws->groupspace, "edje_group", group->obj);
   evas_object_box_layout_set(_box, _main_box_layout, group->obj, NULL);
   evas_object_show(group->obj);
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
   evas_object_data_set(ws->groupspace, GS_BOX_KEY, _box);
   evas_object_data_set(ws->groupspace, GS_GROUP_KEY, group);
   evas_object_data_set(ws->groupspace, GS_WS_KEY, ws);
   evas_object_data_set(ws->groupspace, GS_PROJECT_KEY, project);
}

void
ui_groupspace_update (Evas_Object *obj)
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
   Workspace *ws = evas_object_data_get(obj, GS_WS_KEY);
   elm_object_part_content_unset(ws->groupspace, "groupspace");
   elm_layout_signal_emit (ws->groupspace, "groupspace,hide", "");
   Evas_Object *box = evas_object_data_get(ws->groupspace, GS_BOX_KEY);

   evas_object_box_remove_all(box, EINA_TRUE);
   evas_object_hide(ws->groupspace);
}

void
ui_groupspace_separate(Evas_Object *groupspace, Eina_Bool separate)
{
   Evas_Object *box = evas_object_data_get(groupspace, GS_BOX_KEY);
   Group *group = evas_object_data_get(groupspace, GS_GROUP_KEY);

   if (separate)
     evas_object_box_layout_set(box, _separate_layout, group->obj, NULL);
   else
     evas_object_box_layout_set(box, _main_box_layout, group->obj, NULL);
}
