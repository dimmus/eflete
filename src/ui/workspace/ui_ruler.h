#ifndef UI_RULER_HEADER_H
#define UI_RULER_HEADER_H

#include <Elementary.h>
#include "logger.h"

#define RULERDATAKEY "ruler_data_key"

enum _Orient
{
	HORIZONTAL = 0,
	VERTICAL
};
typedef enum _Orient Orient;


Evas_Object *
ui_ruler_add (Evas_Object *parent);

void
ui_ruler_orient_set (Evas_Object *obj, Orient orient);

Orient
ui_ruler_orient_get (Evas_Object *obj);

int
ui_ruler_pointer_pos_get (Evas_Object *obj);

void
ui_ruler_pointer_visible_set (Evas_Object *obj, Eina_Bool visible);

Eina_Bool
ui_ruler_pointer_visible_get (Evas_Object *obj);

void
ui_ruler_step_scale_set (Evas_Object *obj,unsigned int step);

unsigned int
ui_ruler_step_scale_get (Evas_Object *obj);

void
ui_ruler_type_relative_set (Evas_Object *obj, Eina_Bool enable);

void
ui_ruler_type_absolute_set (Evas_Object *obj, Eina_Bool enable);

void
ui_ruler_pointer_pos_set (Evas_Object *obj, int pos);

#endif /* UI_RULER_HEADER_H */
