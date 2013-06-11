#include "ui_groupspace.h"

/*Prototypes functions for internal use*/
static Evas_Object *
_gs_rect_draw (Evas_Object * _view_part,Group *group, Part *part);

static void
_gs_group_draw (Evas_Object *view_part , Group *group, App_Data *ap);

static void
_gs_move_parts (Evas_Object *view_part , Group *group);

static void
_gs_mouse_move_cb (void *data, Evas *e,	Evas_Object *obj __UNUSED__,
		   void *event_info __UNUSED__)
{
   int x, y;
   Workspace *ws = (Workspace*)data;
   evas_pointer_output_xy_get (e, &x, &y);
   ui_ruler_pointer_pos_set (ws->ruler_hor, x);
   ui_ruler_pointer_pos_set (ws->ruler_ver, y);
}

static void
_gs_resize_cb (void *data, Evas *e __UNUSED__,	Evas_Object *obj,
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

static Evas_Object *
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
     NOTIFY_WARNING (view_part, "Could not load image. Error string is \"%s\"\n",
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

static Evas_Object *
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
/*
static char *
_text_fit (Part *part, Evas_Object *text_obj, int _width)
{
   Evas_Coord _w = 0, _h = 0;
   int uc1 = -1, uc2 = -1, c1 = -1, c2 = -1;
   size_t length;
   int l = 0, r = 0;
   int pntr_pos = -1;
   char *buf = NULL;
   char *text;
   int i=0, loop=0;
   int extra = 6; *//*1 [terminator] + 2*3 [elipsis]*/
 /*  Part_State *current_state = NULL;
   current_state = EINA_INLIST_CONTAINER_GET (part->states, Part_State);

   length = strlen (current_state->text->text);
   text = malloc (length);
   strcpy (text, current_state->text->text);

   evas_object_text_style_pad_get (text_obj, &l, &r, NULL, NULL);

   _w = evas_object_text_horiz_advance_get (text_obj);
   _h = evas_object_text_vert_advance_get (text_obj);

   pntr_pos = ((_width - _w) * current_state->text->elipsis);
   DBG ("l-pntr_pos [%d] _w [%d] h[%d] _width[%d]",(l- pntr_pos), _w, _h, _width);
   if (_w > _width)
     {
        if (current_state->text->elipsis != 0.0)
          uc1 = evas_object_text_last_up_to_pos (text_obj, l - pntr_pos, _h/2);

        if (current_state->text->elipsis != 1.0)
          {
            if ((_width - pntr_pos - r) < 0)
              uc1 = evas_object_text_last_up_to_pos (text_obj, 0, _h/2);
            else
              uc2 = evas_object_text_last_up_to_pos (text_obj, _width -
                    pntr_pos - r, _h/2);
          }

        if ((uc1 < 0) && (uc2 < 0))
          {
             uc1 = 0;
             uc2 = 0;
          }
     }
   fprintf (stdout, "INFO: uc1[%d] uc2[%d]\n",uc1, uc2);
   if (!(((uc1 >= 0) || (uc2 >= 0)) && (_w > _width)))
     return text;

   if ((uc1 == 0) && (uc2 == 0)) return text;

   length = (length < ((size_t)8192 - extra)) ? length : ((size_t)8192 - extra);
   if (!(buf = malloc (length + extra))) return text;
    DBG ("length [%d]", (int)length);
   i = 0;
   if (uc1 >= 0)
     {
        c1 = 0;
        for ( ; i < uc1 ; i++)
          c1 = evas_string_char_next_get(text, c1, NULL);
     }
   if (uc2 >= 0)
     {
        if (c1 >= 0)
          {
             c2 = c1;
          }
        else c2 = 0;

        for ( ; i < uc2 ; i++)
          c2 = evas_string_char_next_get(text, c2, NULL);
     }

   buf[0] = '\0';


   while (((c1 >= 0) || (c2 >= 0)) && (_w > _width))
     {
        loop++;
        if (_width <= 0.0)
          {
             buf[0] = 0;
             break;
          }

        if ((c1 >= 0) && (c2 >= 0))
          {
             if ((loop & 0x1))
               {
                  if (c1 >= 0)
                    c1 = evas_string_char_next_get(text, c1, NULL);
               }
             else
               {
                  if (c2 >= 0)
                    {
                       c2 = evas_string_char_prev_get(text, c2, NULL);
                       if (c2 < 0)
                         {
                            buf[0] = 0;
                            break;
                         }
                    }
               }
          }
        else
          {
             if (c1 >= 0)
               c1 = evas_string_char_next_get(text, c1, NULL);
             else if (c2 >= 0)
               {
                  c2 = evas_string_char_prev_get(text, c2, NULL);
                  if (c2 < 0)
                    {
                       buf[0] = 0;
                       break;
                    }
               }
          }
        if ((c1 >= 0) && (c2 >= 0))
          {
             if (c1 >= c2)
               {
                  buf[0] = 0;
                  break;
               }
          }
        else if ((c1 > 0 && (size_t) c1 >= length) || c2 == 0)
          {
             buf[0] = 0;
             break;
          }

        buf[0] = 0;
     }

   fprintf (stdout, "INFO: uc1[%d] uc2[%d] c1[%d] c2[%d] \n",uc1, uc2, c1, c2);

   if (c1 >= 0)
     {
        strcpy(buf, _ELLIP_STR);

        if (c2 >= 0)
          {
             strncat(buf, text + c1, c2 - c1);
             strcat(buf, _ELLIP_STR);
          }
        else
          strcat(buf, text + c1);
     }
   else
     {
        if (c2 >= 0)
          {
             strncpy(buf, text, c2);
             buf[c2] = 0;
             strcat(buf, _ELLIP_STR);
          }
        else
          strcpy(buf, text);
     }
   fprintf (stdout, "DEBUG:\nBuf: [%s]\n",buf);

   fprintf (stdout, "Text: [%s]\n",text);
   return buf;
}*/
/*
static Evas_Object *
_text_align (Part *part __UNUSED__)
{
 return NULL;
}
*/
static Evas_Object *
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

   evas_object_color_set (_text, _current_state->color[0],
                          _current_state->color[1],_current_state->color[2],
                          _current_state->color[3]);
   evas_object_move (_text, x, y);
   evas_object_resize (_text, w, h);
   evas_object_show (_text);
/**
 * Ellipsis parametr of text will be relised when EVAS library 1.8 came.
 * With method
 * void evas_object_text_ellipsis_set 	(Evas_Object *obj, double ellipsis )
 * http://docs.enlightenment.org/auto/efl/group__Evas__Object__Text.html#ga06da1d81bf84cfb6247791b4696125f9/
 */
   return _text;
}

static Evas_Object *
_gs_spacer_draw (Evas_Object * view_part __UNUSED__,
                 Group *group __UNUSED__, Part *part __UNUSED__)
{
   Evas_Object *_spacer;
   Evas *canvas;
   int x, y, w, h;
   Evas_Load_Error err;

   canvas = evas_object_evas_get (view_part);
   _spacer = evas_object_image_add(canvas);
   evas_object_image_file_set (_spacer, TET_IMG_PATH"spacer.png", NULL);
   evas_object_image_fill_set (_spacer, 2, 2, 10, 10);
   edje_object_part_geometry_get (group->obj, part->name, &x, &y, &w, &h);
   evas_object_move (_spacer, x, y);
   evas_object_resize (_spacer, w, h);
   evas_object_image_fill_spread_set (_spacer, EVAS_TEXTURE_REPEAT);
   err = evas_object_image_load_error_get(_spacer);
   if (err != EVAS_LOAD_ERROR_NONE)
     NOTIFY_WARNING (view_part, "Could not load image. Error string is \"%s\"\n",
           evas_load_error_str(err));



   evas_object_show (_spacer);
   return _spacer;
}

static Evas_Object *
_gs_swallow_draw (Evas_Object * view_part __UNUSED__,
                 Group *group __UNUSED__, Part *part __UNUSED__)
{
   Evas_Object *_swallow;
   Evas *canvas;
   int x, y, w, h;
   Evas_Load_Error err;

   canvas = evas_object_evas_get (view_part);
   _swallow = evas_object_image_add (canvas);
   evas_object_image_file_set (_swallow, TET_IMG_PATH"swallow.png", NULL);
   evas_object_image_fill_set (_swallow, 2, 2, 10, 10);
   edje_object_part_geometry_get (group->obj, part->name, &x, &y, &w, &h);
   evas_object_move (_swallow, x, y);
   evas_object_resize (_swallow, w, h);
   evas_object_image_fill_spread_set (_swallow, EVAS_TEXTURE_REPEAT);
   err = evas_object_image_load_error_get(_swallow);
   if (err != EVAS_LOAD_ERROR_NONE)
     NOTIFY_WARNING (view_part, "Could not load image. Error string is \"%s\"\n",
           evas_load_error_str(err));


   evas_object_show (_swallow);
   return _swallow;
}

static Evas_Object *
_gs_textblock_draw (Evas_Object * view_part __UNUSED__,
                 Group *group __UNUSED__, Part *part __UNUSED__)
{
   return NULL;
}

static Evas_Object *
_gs_group_part_draw (Evas_Object * view_part __UNUSED__,
                 Group *group __UNUSED__, Part *part __UNUSED__)
{
   return NULL;
}


static void
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

static void
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
