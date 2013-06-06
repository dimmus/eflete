#include "ui_groupspace.h"

/*Prototypes functions for internal use*/
Evas_Object *
_gs_rect_draw (Evas_Object * _view_part,Group *group, Part *part);

void
_gs_group_draw (Evas_Object *view_part , Group *group, App_Data *ap);

void
_gs_move_parts (Evas_Object *view_part , Group *group);

void
_gs_part_geometry_calc (Evas_Object *view_part, Group *group, Part *part,
						int *x, int *y, int *w, int *h);

static void
_gs_mouse_move_cb (void *data, Evas *e,
					Evas_Object *obj __UNUSED__,
					void *event_info __UNUSED__)
{
   int x, y;
   Workspace *ws = (Workspace*)data;
   evas_pointer_output_xy_get (e, &x, &y);
   ui_ruler_pointer_pos_set (ws->ruler_hor, x);
   ui_ruler_pointer_pos_set (ws->ruler_ver, y);
}

static void
_gs_resize_cb (void *data, Evas *e __UNUSED__,
					Evas_Object *obj ,
					void *event_info __UNUSED__)
{
   int x, y, w, h;
   Workspace *ws = (Workspace*)data;
   evas_object_geometry_get (obj, &x, &y, &w, &h);
   ui_ruler_scale_absolute_position_zero_set (ws->ruler_hor, x);
   ui_ruler_scale_absolute_position_zero_set (ws->ruler_ver, y);
   ui_ruler_scale_relative_position_set (ws->ruler_hor, x, x+w);
   ui_ruler_scale_relative_position_set (ws->ruler_ver, y, y+h);

   ui_object_highlight_move(ws);
}

static void
_gs_resize_parts_cb (void *data, Evas *e __UNUSED__,
			Evas_Object *obj, void *event_info __UNUSED__)
{
   _gs_move_parts (obj, (Group*)(data));
}

Evas_Object *
_gs_image_draw (Evas_Object *view_part, Group *group , Part *part,
		Project *project)
{
   Evas_Load_Error err;
   Evas_Object *_image;
   Evas *canvas;
   char buf[4096];
   int x, y, w, h;

   Part_State *_current_state =
      EINA_INLIST_CONTAINER_GET (part->states, Part_State);

   canvas = evas_object_evas_get (view_part);
   _image = evas_object_image_filled_add (canvas);

   snprintf (buf, sizeof(buf), "edje/images/%i",_current_state->image->id);
   evas_object_image_file_set (_image, project->swapfile, buf);

   edje_object_part_geometry_get (group->obj, part->name, &x, &y, &w, &h);

   err = evas_object_image_load_error_get(_image);
   if (err != EVAS_LOAD_ERROR_NONE)
     WARN ("Could not load image. Error string is \"%s\"\n",
           evas_load_error_str(err));

   evas_object_move (_image, x, y);
   evas_object_resize (_image, w, h);

   evas_object_image_border_set (_image, _current_state->image->left,
                                 _current_state->image->right,
                                 _current_state->image->top,
                                 _current_state->image->bottom);

   evas_object_image_fill_set (_image,
                               _current_state->image->fill_origin_relative_x,
                               _current_state->image->fill_origin_relative_y,
                               w, h);

   evas_object_show (_image);
   return _image;
}

Evas_Object *
_gs_rect_draw (Evas_Object * view_part, Group *group, Part *part)
{
   Evas_Object *_rectangle;
   Evas *canvas;
   int x, y, w, h;
   Part_State *_current_state = NULL;
   _current_state = EINA_INLIST_CONTAINER_GET (part->states, Part_State);
   canvas = evas_object_evas_get (view_part);
   edje_object_part_geometry_get (group->obj, part->name, &x, &y, &w, &h);

   _rectangle = evas_object_rectangle_add (canvas);
   evas_object_move (_rectangle, x, y);
   evas_object_resize (_rectangle, w, h);
   evas_object_color_set (_rectangle, _current_state->color[0],
                          _current_state->color[1],_current_state->color[2],
                          _current_state->color[3]);

   evas_object_show (_rectangle);
   return _rectangle;
}

Evas_Object *
_gs_text_draw (Evas_Object * view_part, Group *group, Part *part)
{
   Evas_Object *_text;
   Evas *canvas;
   int x, y, w, h;
   Part_State *_current_state = NULL;
   _current_state = EINA_INLIST_CONTAINER_GET (part->states, Part_State);


   canvas = evas_object_evas_get (view_part);
   edje_object_part_geometry_get (group->obj, part->name, &x, &y, &w, &h);
   _text = evas_object_text_add (canvas);
   evas_object_text_font_set (_text, _current_state->text->font,
                              _current_state->text->size);
   evas_object_text_text_set (_text, _current_state->text->text);

   evas_object_move (_text, x, y);
   evas_object_resize (_text, w, h);

   evas_object_show (_text);
   return _text;
}

Evas_Object *
_gs_spacer_draw (Evas_Object * view_part __UNUSED__,
                 Group *group __UNUSED__, Part *part __UNUSED__)
{
   Evas_Object *_spacer;
   Evas *canvas;
   int x, y, w, h;
   Evas_Load_Error err;


   canvas = evas_object_evas_get (view_part);
   _spacer = evas_object_image_filled_add (canvas);
   evas_object_image_file_set (_spacer, TET_IMG_PATH"spacer.png", NULL);
   evas_object_image_fill_set (_spacer, 0, 0, 30, 30);
   edje_object_part_geometry_get (group->obj, part->name, &x, &y, &w, &h);
   err = evas_object_image_load_error_get(_spacer);
   if (err != EVAS_LOAD_ERROR_NONE)
     WARN ("Could not load image. Error string is \"%s\"\n",
           evas_load_error_str(err));

   evas_object_move (_spacer, x, y);
   evas_object_resize (_spacer, w, h);


   evas_object_show (_spacer);
   return _spacer;
}

Evas_Object *
_gs_swallow_draw (Evas_Object * view_part __UNUSED__,
                 Group *group __UNUSED__, Part *part __UNUSED__)
{
   Evas_Object *_swallow;
   Evas *canvas;
   int x, y, w, h;
   Evas_Load_Error err;

   canvas = evas_object_evas_get (view_part);
   _swallow = evas_object_image_filled_add (canvas);
   evas_object_image_file_set (_swallow, TET_IMG_PATH"swallow.png", NULL);
   evas_object_image_fill_set (_swallow, 5, 15, 10, 10);
   edje_object_part_geometry_get (group->obj, part->name, &x, &y, &w, &h);
   err = evas_object_image_load_error_get(_swallow);
   if (err != EVAS_LOAD_ERROR_NONE)
     WARN ("Could not load image. Error string is \"%s\"\n",
           evas_load_error_str(err));

   evas_object_move (_swallow, x, y);
   evas_object_resize (_swallow, w, h);

   evas_object_show (_swallow);
   return _swallow;
}

Evas_Object *
_gs_textblock_draw (Evas_Object * view_part __UNUSED__,
                 Group *group __UNUSED__, Part *part __UNUSED__)
{
   return NULL;
}

Evas_Object *
_gs_group_part_draw (Evas_Object * view_part __UNUSED__,
                 Group *group __UNUSED__, Part *part __UNUSED__)
{
   return NULL;
}


void
_gs_part_object_coord_get (const char* name, Group *group,
			   int *x, int *y, int *x2, int *y2)
{
   int _x, _y, _w, _h;
   Eina_Bool _find_part = EINA_FALSE;
   Part *_part = NULL;
   _x=_y=_w=_h=0;

   EINA_INLIST_FOREACH (group->parts, _part)
     {
        if (!strcmp(_part->name, name))
          {
             evas_object_geometry_get (_part->obj, &_x, &_y, &_w, &_h);
             _find_part = EINA_TRUE;
             break;
          }
     }
   if (_find_part)
     {
        if (x) {*x = _x;}
        if (y) {*y = _y;}
        if (x2) {*x2 = _w+_x;}
        if (y2) {*y2 = _h+_y;}
     }
}

void
_gs_move_parts (Evas_Object *view_part , Group *group)
{
   int x, y, w, h, x1, y1, w1, h1;
   Part *_part = NULL;

   evas_object_geometry_get (view_part, &x1, &y1, &w1, &h1);
   evas_object_resize (group->obj, w1, h1);
   evas_object_move (group->obj, x1, y1);
   EINA_INLIST_FOREACH (group->parts, _part)
     {
        edje_object_part_geometry_get (group->obj, _part->name, &x, &y, &w, &h);
        evas_object_move (_part->obj, x+x1, y+y1);
        evas_object_resize (_part->obj, w, h);
     }
}

void
_gs_group_draw (Evas_Object *view_part , Group *group, App_Data *ap)
{
   Part *_part = NULL;
   Evas_Object *_part_object = NULL;
   EINA_INLIST_FOREACH (group->parts, _part)
     {
        if (_part->type == EDJE_PART_TYPE_RECTANGLE)
          _part_object=_gs_rect_draw(view_part, group, _part);
        if (_part->type == EDJE_PART_TYPE_IMAGE)
          _part_object = _gs_image_draw (view_part, group, _part,ap->project);
        if (_part->type == EDJE_PART_TYPE_TEXT)
          _part_object = _gs_text_draw (view_part, group, _part);
        if (_part->type == EDJE_PART_TYPE_SPACER)
          _part_object = _gs_spacer_draw (view_part, group, _part);
        if (_part->type == EDJE_PART_TYPE_SWALLOW)
          _part_object = _gs_swallow_draw (view_part, group, _part);
        if (_part->type == EDJE_PART_TYPE_GROUP)
          _part_object = _gs_group_part_draw (view_part, group, _part);
        if (_part->type == EDJE_PART_TYPE_TEXTBLOCK)
          _part_object = _gs_textblock_draw (view_part, group, _part);

        evas_object_event_callback_add(_part_object, EVAS_CALLBACK_MOUSE_MOVE,
                                       _gs_mouse_move_cb, ap->ws);
        _part->obj = _part_object;
     }
}

void
ui_groupspace_add (App_Data *ap, Group *group)
{
   Evas_Object *parent = ap->block.canvas;
   Evas_Object *_groupspace;
   Evas_Object *_part_view;

   ui_groupspace_del (ap->ws->groupspace, group);

   _groupspace = elm_layout_add (parent);

   elm_object_part_content_set (parent,
                                "base/workspace/groupspace",_groupspace);
   elm_layout_file_set(_groupspace, TET_EDJ, "base/groupspace");

   _part_view = elm_layout_add(_groupspace);
   elm_object_part_content_set (_groupspace,
                                "base/groupspace/groupspace",_part_view);
   evas_object_show (_part_view);

   evas_object_event_callback_add(_groupspace, EVAS_CALLBACK_MOUSE_MOVE,
                                  _gs_mouse_move_cb, ap->ws);

   evas_object_event_callback_add  (_part_view, EVAS_CALLBACK_RESIZE,
                                    _gs_resize_cb, ap->ws);
   evas_object_event_callback_add  (_part_view,EVAS_CALLBACK_RESIZE,
                                    _gs_resize_parts_cb, group );
   ap->ws->groupspace = _groupspace;
   _gs_group_draw (_part_view, group, ap);
}

void
ui_groupspace_del (Evas_Object *obj, Group *group)
{
   Part *_part = NULL;

   EINA_INLIST_FOREACH (group->parts, _part)
      evas_object_del (_part->obj);

   if (obj) evas_object_del (obj);

   obj=NULL;
}
