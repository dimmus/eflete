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

#include "ui_ruler.h"
#include "alloc.h"

struct _UI_Ruler_Data
{
   Orient orient;
   Eina_Bool visible;
   Eina_Bool abs_scale_visible;
   Eina_Bool rel_scale_visible;
   int abs_scale_step;
   int abs_scale_dashes_step;
   int abs_scale_zero_pos;
   int rel_start;
   int rel_end;
   int rel_step;
   int pointer_pos;
   Eina_Bool pointer_visible;
};
typedef struct _UI_Ruler_Data UI_Ruler_Data;

static UI_Ruler_Data *
_ruler_data_init(void)
{
   UI_Ruler_Data *_ruler_data = mem_malloc(sizeof(UI_Ruler_Data));

   _ruler_data->orient = HORIZONTAL;
   _ruler_data->visible = EINA_TRUE;
   _ruler_data->abs_scale_visible = EINA_TRUE;
   _ruler_data->rel_scale_visible = EINA_FALSE;
   _ruler_data->abs_scale_step = 50;
   _ruler_data->abs_scale_dashes_step = 5;
   _ruler_data->abs_scale_zero_pos = 0;
   _ruler_data->rel_start = 0;
   _ruler_data->rel_end = 600;
   _ruler_data->rel_step = 2;
   _ruler_data->pointer_pos = 0;
   return _ruler_data;
}

int
ui_ruler_pointer_pos_get(Evas_Object *obj)
{
   UI_Ruler_Data *_ruler_data = evas_object_data_get(obj, RULERDATAKEY);
   if (!_ruler_data)
     {
        ERR("Could not get pointer position, because a ruler is NULL.");
        return 0;
     }
   return _ruler_data->pointer_pos;
}

void
ui_ruler_pointer_pos_set(Evas_Object *obj, int pos)
{
   int x, y, w, h;
   double dx, dy;
   dx = dy = 0.0;
   UI_Ruler_Data *_ruler_data = evas_object_data_get(obj, RULERDATAKEY);
   if (!_ruler_data)
     {
        ERR("Could not set pointer position, because a ruler is NULL.");
        return;
     }
   evas_object_geometry_get(obj, &x, &y, &w, &h);

   if (_ruler_data->orient == HORIZONTAL)
     {
        dx =(double)(pos - x) / w;
        _ruler_data->pointer_pos = pos;
     }
   else
     {
        dy = (double)(pos - y) / h;
        _ruler_data->pointer_pos = pos;
     }
   edje_object_part_drag_value_set(obj, "pointer", dx, dy);
}

Orient
ui_ruler_orient_get(Evas_Object *obj)
{
   UI_Ruler_Data *_ruler_data = evas_object_data_get(obj, RULERDATAKEY);
   if (!_ruler_data)
     {
        ERR("Could not get orientation, because a ruler is NULL.");
        return 0;
     }
   return _ruler_data->orient;
}

void
ui_ruler_orient_set(Evas_Object *obj, Orient orient)
{
   UI_Ruler_Data *_ruler_data = evas_object_data_get(obj, RULERDATAKEY);
   if (!_ruler_data)
     {
        ERR("Could not set orientation, because a ruler is NULL.");
        return;
     }
   _ruler_data->orient = orient;
   if (_ruler_data->orient == HORIZONTAL)
     {
        edje_object_file_set(obj, EFLETE_EDJ, "ui/ruler/horizontal/default");
        edje_object_message_send(obj,EDJE_MESSAGE_NONE,MSG_ORIENT_HOR,0);
     }
   else
     {
        edje_object_file_set(obj, EFLETE_EDJ, "ui/ruler/vertical/default");
        edje_object_message_send(obj,EDJE_MESSAGE_NONE,MSG_ORIENT_VER,0);
     }
}

void
ui_ruler_scale_absolute_dashes_step_set(Evas_Object *obj, int step)
{
   UI_Ruler_Data *_ruler_data = evas_object_data_get(obj, RULERDATAKEY);
   if (!_ruler_data)
     {
        ERR("Could not set abs dashes step, because a ruler is NULL.");
        return;
     }
   _ruler_data->abs_scale_dashes_step = step;
   Edje_Message_Int msg;
   msg.val=step;
   edje_object_message_send(obj,EDJE_MESSAGE_INT,MSG_ABS_DASHES_STEP,&msg);
}

int
ui_ruler_scale_absolute_dashes_step_get(Evas_Object *obj)
{
   UI_Ruler_Data *_ruler_data = evas_object_data_get(obj, RULERDATAKEY);
   if (!_ruler_data)
     {
        ERR("Could not get abs dashes step, because a ruler is NULL.");
        return 0;
     }
   return (int)_ruler_data->abs_scale_dashes_step;
}

void
ui_ruler_step_scale_absolute_set(Evas_Object *obj, int step)
{
   UI_Ruler_Data *_ruler_data = evas_object_data_get(obj, RULERDATAKEY);
   if (!_ruler_data)
     {
        ERR("Could not set abs step, because a ruler is NULL.");
        return;
     }
   _ruler_data->abs_scale_step = step;
   Edje_Message_Int msg;
   msg.val=step;
   edje_object_message_send(obj,EDJE_MESSAGE_INT,MSG_ABS_STEP,&msg);
}

unsigned int
ui_ruler_step_absolute_get(Evas_Object *obj)
{
   UI_Ruler_Data *_ruler_data = evas_object_data_get(obj, RULERDATAKEY);
   if (!_ruler_data)
     {
        ERR("Could not get abs step, because a ruler is NULL.");
        return 0;
     }
   return (int)_ruler_data->abs_scale_step;
}

void
ui_ruler_step_relative_set(Evas_Object *obj, int step)
{
   UI_Ruler_Data *_ruler_data = evas_object_data_get(obj, RULERDATAKEY);
   if (!_ruler_data)
     {
        ERR("Could not set relative step, because a ruler is NULL.");
        return;
     }
   _ruler_data->rel_step=step;
   Edje_Message_Int msg;
   msg.val=step;
   edje_object_message_send(obj,EDJE_MESSAGE_INT,MSG_REL_DASHES,&msg);
}

int
ui_ruler_step_relative_get (Evas_Object *obj)
{
   UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);
   if (!_ruler_data)
     {
        ERR("Could not get relative step, because a ruler is NULL.");
        return 0;
     }
   return _ruler_data->rel_step;
}

void
ui_ruler_scale_relative_visible_set(Evas_Object *obj, Eina_Bool enable)
{
   UI_Ruler_Data *_ruler_data = evas_object_data_get(obj, RULERDATAKEY);
   if (!_ruler_data)
     {
        ERR("Could not set relative scale visibilility, because a ruler is NULL.");
        return;
     }
   _ruler_data->rel_scale_visible = enable;
   Edje_Message_Int msg;
   msg.val=enable;
   edje_object_message_send(obj,EDJE_MESSAGE_INT,MSG_REL_VISIBLE,&msg);
}

Eina_Bool
ui_ruler_scale_relative_visible_get(Evas_Object *obj)
{
   UI_Ruler_Data *_ruler_data = evas_object_data_get(obj, RULERDATAKEY);
   if (!_ruler_data)
     {
        ERR("Could not get relative scale visibilility, because a ruler is NULL.");
        return 0;
     }
   return _ruler_data->rel_scale_visible;
}

void
ui_ruler_scale_absolute_visible_set(Evas_Object *obj, Eina_Bool enable)
{
   UI_Ruler_Data *_ruler_data = evas_object_data_get(obj, RULERDATAKEY);
   if (!_ruler_data)
     {
        ERR("Could not set abs scale visibilility, because a ruler is NULL.");
        return;
     }
   _ruler_data->abs_scale_visible = enable;
   Edje_Message_Int msg;
   msg.val=enable;
   edje_object_message_send(obj,EDJE_MESSAGE_INT,MSG_ABS_VISIBLE,&msg);
}

Eina_Bool
ui_ruler_scale_absolute_visible_get(Evas_Object *obj)
{
   UI_Ruler_Data *_ruler_data = evas_object_data_get(obj, RULERDATAKEY);
   if (!_ruler_data)
     {
        ERR("Could not get abs scale visibilility, because a ruler is NULL.");
        return 0;
     }
   return _ruler_data->abs_scale_visible;
}

void
ui_ruler_scale_relative_position_set(Evas_Object *obj, int start, int end)
{
   UI_Ruler_Data *_ruler_data = evas_object_data_get(obj, RULERDATAKEY);
   if (!_ruler_data)
     {
        ERR("Could not set scale relative position, because a ruler is NULL.");
        return;
     }
   _ruler_data->rel_start = start;
   Edje_Message_Int msg;
   msg.val=start;
   edje_object_message_send(obj,EDJE_MESSAGE_INT,MSG_REL_START,&msg);
   _ruler_data->rel_end = end;
   msg.val=end;
   edje_object_message_send(obj,EDJE_MESSAGE_INT,MSG_REL_END,&msg);
}

void
ui_ruler_scale_relative_position_get(Evas_Object *obj, int *start, int *end)
{
   UI_Ruler_Data *_ruler_data = evas_object_data_get(obj, RULERDATAKEY);
   if (!_ruler_data)
     {
        ERR("Could not get scale relative position, because a ruler is NULL.");
        return;
     }
   if(!start || !end)
     {
        ERR("Could not get scale relative position, because start or end is NULL.");
        return;
     }
   *start = _ruler_data->rel_start;
   *end = _ruler_data->rel_end;
}

void
ui_ruler_scale_absolute_position_zero_set(Evas_Object *obj, int pos)
{
   UI_Ruler_Data *_ruler_data = evas_object_data_get(obj, RULERDATAKEY);
   if (!_ruler_data)
     {
        ERR("Could not set abs zero position, because a ruler is NULL.");
        return;
     }
   _ruler_data->abs_scale_zero_pos=pos;
   Edje_Message_Int msg;
   msg.val=pos;
   edje_object_message_send(obj,EDJE_MESSAGE_INT,MSG_ZERO_POS,&msg);
}

int
ui_ruler_scale_absolute_position_zero_get(Evas_Object *obj)
{
   UI_Ruler_Data *_ruler_data = evas_object_data_get(obj, RULERDATAKEY);
   if (!_ruler_data)
     {
        ERR("Could not get abs zero position, because a ruler is NULL.");
        return 0;
     }
   return (int)_ruler_data->abs_scale_zero_pos;
}

void
ui_ruler_hide(Evas_Object *obj)
{
   UI_Ruler_Data *_ruler_data = evas_object_data_get(obj, RULERDATAKEY);
   if (!_ruler_data)
     {
        ERR("Could not hide ruler, because a ruler is NULL.");
        return;
     }
   _ruler_data->visible = EINA_FALSE;
   Edje_Message_Int msg;
   msg.val=0;
   edje_object_message_send(obj,EDJE_MESSAGE_INT,MSG_ABS_VISIBLE,&msg);
   edje_object_message_send(obj,EDJE_MESSAGE_INT,MSG_REL_VISIBLE,&msg);

   edje_object_signal_emit (obj, "ruler,bg,hide", "");
   evas_object_hide(obj);
}

Eina_Bool
ui_ruler_visible_get(Evas_Object *obj)
{
   UI_Ruler_Data *_ruler_data = evas_object_data_get(obj, RULERDATAKEY);
   if (!_ruler_data)
     {
        ERR("Could not get ruler visibility, because a ruler is NULL.");
        return 0;
     }
   return  _ruler_data->visible;
}

void
ui_ruler_show(Evas_Object *obj)
{
   UI_Ruler_Data *_ruler_data = evas_object_data_get(obj, RULERDATAKEY);
   if (!_ruler_data)
     {
        ERR("Could not show ruler, because a ruler is NULL.");
        return;
     }
   _ruler_data->visible = EINA_TRUE;
   Edje_Message_Int msg;
   msg.val=_ruler_data->abs_scale_visible;
   edje_object_message_send(obj,EDJE_MESSAGE_INT,MSG_ABS_VISIBLE,&msg);
   msg.val=_ruler_data->rel_scale_visible;
   edje_object_message_send(obj,EDJE_MESSAGE_INT,MSG_REL_VISIBLE,&msg);

   edje_object_signal_emit (obj, "ruler,bg,show", "");
   evas_object_show(obj);
}
static void
_on_ruler_free(void *data __UNUSED__,
                      Evas *e __UNUSED__,
                      Evas_Object *obj,
                      void *event_info __UNUSED__)
{
   UI_Ruler_Data *_ruler_data = evas_object_data_del(obj, RULERDATAKEY);
   free(_ruler_data);
}
Evas_Object *
ui_ruler_add(Evas_Object *parent)
{
   Evas_Object *ruler;
   UI_Ruler_Data *ruler_data;

   ruler_data = _ruler_data_init();
   if (!ruler_data)
     {
        ERR ("Unable initialize ruler data structure");
        return NULL;
     }

   ruler = edje_object_add(evas_object_evas_get(parent));
   if (!ruler)
     {
        ERR("Unable to create ruler");
        free(ruler_data);
        return NULL;
     }
   evas_object_data_set(ruler, RULERDATAKEY, ruler_data);
   ui_ruler_orient_set(ruler, HORIZONTAL);
   evas_object_resize(ruler, 100, 40);
   evas_object_show(ruler);
   evas_object_event_callback_add(ruler, EVAS_CALLBACK_FREE,
                                  _on_ruler_free, NULL);
   return ruler;
}

void
ui_ruler_redraw(Evas_Object *obj)
{
   if (!obj)
     {
        ERR("Could not redraw, because a ruler is NULL.");
        return;
     }
   edje_object_message_send(obj,EDJE_MESSAGE_NONE,MSG_REDRAW,0);
}
