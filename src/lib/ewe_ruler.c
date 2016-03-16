/* Elementary Widgets Extension
 * Copyright (C) 2014 Samsung Electronics.
 *
 * This file is part of Elementary Widgets Extension.
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
#include "ewe_private.h"
#include "ewe_widget_ruler.h"

EAPI Eo_Op EWE_OBJ_RULER_BASE_ID = EO_NOOP;

#define MY_CLASS EWE_RULER_CLASS

#define MY_CLASS_NAME "Ewe_Ruler"
#define MY_CLASS_NAME_LEGACY "ewe_ruler"

#define MIN_STEP 10
#define DEFAULT_STEP 50
#define DEFAULT_FORMAT "%.0f"
#define DEFAULT_STYLE "default"
#define DASHES "ewe/ruler/horizontal/dashes"
#define DASHES_VER "ewe/ruler/vertical/dashes"
#define MARKER "ewe/ruler/marker/horizontal"
#define MARKER_VER "ewe/ruler/marker/vertical"

static const Evas_Smart_Cb_Description _smart_callbacks[] = {
   {NULL, NULL}
};

/*----------------------internal helpers functions----------------------------*/

static void
_delete_extra_dashes(Ewe_Ruler_Scale *scale, int amount)
{
   Evas_Object *list_data;
   Eina_List *l, *l_next;

   EINA_LIST_REVERSE_FOREACH_SAFE(scale->dashes, l, l_next, list_data)
     {
        if (amount)
          {
             scale->dashes = eina_list_remove_list(scale->dashes, l);
             elm_box_unpack(scale->box, list_data);
             evas_object_smart_member_del(list_data);
             evas_object_del(list_data);
          }
        else break;
        amount--;
     }
}

static inline void
_print_mark_value(Evas_Object *mark, Eina_Stringshare *format, double value, Eina_Bool middle)
{
   Eina_Stringshare *buf = NULL;
   buf = eina_stringshare_printf(format, value);
   if (middle)
     edje_object_part_text_set(mark, "ewe.middle.text", buf);
   else
     edje_object_part_text_set(mark, "ewe.text", buf);
   eina_stringshare_del(buf);
}

static void
_add_dashes(Ewe_Ruler_Smart_Data *sd)
{
   Ewe_Ruler_Scale *scale;
   Eina_List *l;
   Evas_Object *object;
   Evas *canvas;

   sd->size_changed = EINA_FALSE;
   sd->position_changed = EINA_TRUE;
   sd->text_changed = EINA_TRUE;

   EINA_LIST_FOREACH(sd->scales, l, scale)
     {

        if (!scale->visible) continue;
        int dashes_count = eina_list_count(scale->dashes);
        int new_dashes_count;

        if (sd->horizontal)
          new_dashes_count = (((sd->geometry.width / scale->mark_step) + 3) - dashes_count);
        else
          new_dashes_count = (((sd->geometry.height / scale->mark_step) + 3) - dashes_count);
        if (new_dashes_count == 0) continue;

        canvas = evas_object_evas_get(sd->obj);
        if (new_dashes_count > 0)
          {
             for (; new_dashes_count > 0; new_dashes_count--)
               {
                  object = edje_object_add(canvas);
                  edje_object_file_set(object, EWE_THEME, scale->full_style);
                  evas_object_smart_member_add(object, sd->obj);
                  evas_object_clip_set(object, sd->clip);
                  scale->dashes = eina_list_append(scale->dashes, object);
                  if (sd->horizontal)
                    evas_object_size_hint_min_set(object, scale->mark_step, sd->geometry.height);
                  else
                    evas_object_size_hint_min_set(object, sd->geometry.width, scale->mark_step);
                  elm_box_pack_end(scale->box, object);
               }
          }
        else
          _delete_extra_dashes(scale, -new_dashes_count);
     }
}

static void
_set_labels(Ewe_Ruler_Smart_Data *sd)
{
   Ewe_Ruler_Scale *scale;
   Eina_List *l, *ls;
   Evas_Object *item;

   EINA_LIST_FOREACH(sd->scales, ls, scale)
     {
        int i = - ((int)(scale->zero_pos / scale->mark_step) + 1);
        EINA_LIST_FOREACH(scale->dashes, l, item)
          {
             _print_mark_value(item, scale->format, i * scale->mark_value_step, EINA_FALSE);
             ++i;
          }

        if (!scale->show_middle_mark) continue;

        i = - ((int)(scale->zero_pos / scale->mark_step) + 1);
        EINA_LIST_FOREACH(scale->dashes, l, item)
          {
             _print_mark_value(item, scale->format, (i + 0.5) * scale->mark_value_step, EINA_TRUE);
             ++i;
          }
     }
   sd->text_changed = EINA_FALSE;
}

static void
_place_markers(Ewe_Ruler_Smart_Data *sd)
{
   Ewe_Ruler_Marker *marker;
   Eina_List *l;
   Ewe_Ruler_Scale *scale;

   EINA_LIST_FOREACH(sd->markers, l, marker)
     {
        if (!marker->visible) continue;

        if (marker->relative)
          {
             scale = (marker->scale) ? marker->scale : eina_list_data_get(sd->scales);
             if (!scale->visible) continue;
             marker->abs_position = (scale->mark_value_step) ? ((marker->rel_position / scale->mark_value_step) * scale->mark_step + scale->zero_pos) : scale->zero_pos;
          }

        if (sd->horizontal)
          {
             /*size of marker(pointer) is set to 1 to make posible centring from style*/
             evas_object_resize(marker->obj, 1, sd->geometry.height);
             evas_object_move(marker->obj,
                              sd->geometry.x + marker->abs_position,
                              sd->geometry.y);
          }
        else
          {
             /*size of marker(pointer) is set to 1 to make posible centring from style*/
             evas_object_resize(marker->obj, sd->geometry.width, 1);
             evas_object_move(marker->obj,
                              sd->geometry.x,
                              marker->abs_position + sd->geometry.y);
          }

        evas_object_raise(marker->obj);
        evas_object_clip_set(marker->obj, sd->clip);
        ///* FIXME: [funny magic] idk why this line has no effect when creating marker object */
     }
}
/*----------------------internal realisation API------------------------------*/

static void
_dashes_size_set(Ewe_Ruler_Smart_Data *sd,  Ewe_Ruler_Scale *scale)
{
   Eina_List *l;
   Evas_Object *object;

   EINA_LIST_FOREACH(scale->dashes, l, object)
     {
        if (sd->horizontal)
          evas_object_size_hint_min_set(object, scale->mark_step, sd->geometry.height);
        else
          evas_object_size_hint_min_set(object, sd->geometry.width, scale->mark_step);
     }
}

EOLIAN static Eina_Bool
_ewe_ruler_horizontal_set(Eo *obj,
                          Ewe_Ruler_Smart_Data *sd,
                          Eina_Bool horizontal)
{
   if (sd->horizontal == horizontal)
     return EINA_FALSE;
   sd->horizontal = horizontal;
   sd->size_changed = EINA_TRUE;

   Ewe_Ruler_Scale *scale;
   Ewe_Ruler_Marker *marker;
   Eina_List *ls, *lm;
   int count;
   const char *dashes, *markers;

   if (sd->horizontal)
     {
        edje_object_file_set(sd->bg, EWE_THEME, "ewe/ruler/horizontal_background/default");
        dashes = DASHES;
        markers = MARKER;
     }
   else
     {
        edje_object_file_set(sd->bg, EWE_THEME, "ewe/ruler/vertical_background/default");
        dashes = DASHES_VER;
        markers = MARKER_VER;
     }
   EINA_LIST_FOREACH(sd->scales, ls, scale)
     {
        eina_stringshare_del(scale->full_style);
        scale->full_style = eina_stringshare_printf("%s/%s", dashes, scale->style);
        elm_box_horizontal_set(scale->box, sd->horizontal);
        _dashes_size_set(sd, scale);
        count = eina_list_count(scale->dashes);
        _delete_extra_dashes(scale, count);
     }
   EINA_LIST_FOREACH(sd->markers, lm, marker)
     {
        eina_stringshare_del(marker->full_style);
        marker->full_style = eina_stringshare_printf("%s/%s", markers, marker->style);
        edje_object_file_set(marker->obj, EWE_THEME, marker->full_style);
     }

   evas_object_smart_changed(obj);
   return EINA_TRUE;
}

EOLIAN static Eina_Bool
_ewe_ruler_horizontal_get(Eo *obj EINA_UNUSED, Ewe_Ruler_Smart_Data *sd)
{
   return sd->horizontal;
}

EOLIAN static Eina_Bool
_ewe_ruler_step_set(Eo *obj,
                    Ewe_Ruler_Smart_Data *sd,
                    Ewe_Ruler_Scale *scale,
                    unsigned int step)
{
   if (!obj) return EINA_FALSE;
   if (!scale) scale = eina_list_data_get(sd->scales);
   if (!scale) return EINA_FALSE;
   if (step < MIN_STEP)
     step = MIN_STEP;
   if (scale->mark_step == (int)step) return EINA_TRUE;
   scale->mark_step = step;
   _dashes_size_set(sd, scale);

   sd->position_changed = EINA_TRUE;
   sd->size_changed = EINA_TRUE;
   evas_object_smart_changed(obj);
   return EINA_TRUE;
}

EOLIAN static unsigned int
_ewe_ruler_step_get(Eo *obj EINA_UNUSED,
                    Ewe_Ruler_Smart_Data *sd,
                    Ewe_Ruler_Scale *scale)
{
   if (!scale) scale = eina_list_data_get(sd->scales);
   if (!scale) return 0;
   return scale->mark_step;
}

EOLIAN static Eina_Bool
_ewe_ruler_value_step_set(Eo *obj,
                          Ewe_Ruler_Smart_Data *sd,
                          Ewe_Ruler_Scale *scale,
                          double step)
{
   if (!obj) return EINA_FALSE;
   if (!scale) scale = eina_list_data_get(sd->scales);
   if (!scale) return EINA_FALSE;
   scale->mark_value_step = step;
   evas_object_smart_changed(obj);
   return EINA_TRUE;
}

EOLIAN static double
_ewe_ruler_value_step_get(Eo *obj EINA_UNUSED,
                          Ewe_Ruler_Smart_Data *sd,
                          Ewe_Ruler_Scale *scale)
{
   if (!scale) scale = eina_list_data_get(sd->scales);
   if (!scale) return 0;
   return scale->mark_value_step;
}

EOLIAN static Eina_Bool
_ewe_ruler_zero_offset_set(Eo *obj,
                           Ewe_Ruler_Smart_Data *sd,
                           Ewe_Ruler_Scale *scale,
                           int pos)
{
   if (!obj) return EINA_FALSE;
   if (!scale) scale = eina_list_data_get(sd->scales);
   if (!scale) return EINA_FALSE;
   if (scale->zero_pos == pos) return EINA_TRUE;
   scale->zero_pos = pos;
   sd->position_changed = EINA_TRUE;
   evas_object_smart_changed(obj);
   return EINA_TRUE;
}

EOLIAN static int
_ewe_ruler_zero_offset_get(Eo *obj EINA_UNUSED,
                           Ewe_Ruler_Smart_Data *sd,
                           Ewe_Ruler_Scale *scale)
{
   if (!scale) scale = eina_list_data_get(sd->scales);
   if (!scale) return 0;
   return  scale->zero_pos;
}

EOLIAN static Eina_Bool
_ewe_ruler_format_set(Eo *obj,
                      Ewe_Ruler_Smart_Data *sd,
                      Ewe_Ruler_Scale *scale,
                      const char *format)
{
   if (!obj) return EINA_FALSE;
   if (!scale) scale = eina_list_data_get(sd->scales);
   if (!scale) return EINA_FALSE;
   eina_stringshare_del(scale->format);
   scale->format = eina_stringshare_add(format);
   sd->text_changed = EINA_TRUE;
   evas_object_smart_changed(obj);
   return EINA_TRUE;
}

EOLIAN static const char *
_ewe_ruler_format_get(Eo *obj EINA_UNUSED,
                      Ewe_Ruler_Smart_Data *sd,
                      Ewe_Ruler_Scale *scale)
{
   if (!scale) scale = eina_list_data_get(sd->scales);
   if (!scale) return NULL;
   return scale->format;
}

EOLIAN static Eina_Bool
_ewe_ruler_style_set(Eo *obj,
                     Ewe_Ruler_Smart_Data *sd,
                     Ewe_Ruler_Scale *scale,
                     const char *style)
{
   if (!scale) scale = eina_list_data_get(sd->scales);
   if (!scale) return EINA_FALSE;
   eina_stringshare_del(scale->style);
   eina_stringshare_del(scale->full_style);
   scale->style = eina_stringshare_add(style);
   if (sd->horizontal)
     scale->full_style = eina_stringshare_printf("%s/%s", DASHES, style);
   else
     scale->full_style = eina_stringshare_printf("%s/%s", DASHES_VER, style);

   int count = eina_list_count(scale->dashes);
   _delete_extra_dashes(scale, count);

   evas_object_smart_changed(obj);
   return EINA_TRUE;
}

EOLIAN static const char *
_ewe_ruler_style_get(Eo *obj EINA_UNUSED,
                     Ewe_Ruler_Smart_Data *sd,
                     Ewe_Ruler_Scale *scale)
{
   if (!scale) scale = eina_list_data_get(sd->scales);
   if (!scale) return 0;
   return scale->style;
}

EOLIAN static Ewe_Ruler_Scale *
_ewe_ruler_scale_add(Eo *obj,
                     Ewe_Ruler_Smart_Data *sd,
                     const char *style)
{
   Ewe_Ruler_Scale *ret = malloc(sizeof(Ewe_Ruler_Scale));
   if (!ret) return NULL;
   if (!style) style = DEFAULT_STYLE;

   sd->scales = eina_list_append(sd->scales, ret);

   ret->box = elm_box_add(obj);
   elm_box_align_set(ret->box, 0.0, 0.0);
   elm_box_horizontal_set(ret->box, sd->horizontal);
   evas_object_smart_member_add(ret->box, obj);
   evas_object_clip_set(ret->box, sd->clip);
   ret->dashes = NULL;
   ret->visible = EINA_TRUE;
   ret->mark_step = DEFAULT_STEP;
   ret->mark_value_step = DEFAULT_STEP;
   ret->markers = NULL;
   ret->style = eina_stringshare_add(style);
   if (sd->horizontal)
     ret->full_style = eina_stringshare_printf("%s/%s", DASHES, style);
   else
     ret->full_style = eina_stringshare_printf("%s/%s", DASHES_VER, style);
   ret->format = eina_stringshare_add(DEFAULT_FORMAT);
   ret->show_middle_mark = EINA_FALSE;
   ret->zero_pos = 0;

   return ret;
}

EOLIAN static Eina_Bool
_ewe_ruler_scale_del(Eo *obj EINA_UNUSED,
                     Ewe_Ruler_Smart_Data *sd,
                     Ewe_Ruler_Scale *scale)
{
   eina_stringshare_del(scale->style);
   eina_stringshare_del(scale->full_style);
   eina_stringshare_del(scale->format);

   int count = eina_list_count(scale->dashes);
   _delete_extra_dashes(scale, count);

   sd->scales = eina_list_remove(sd->scales, scale);
   free(scale);
   return EINA_TRUE;
}

EOLIAN static Eina_Bool
_ewe_ruler_scale_visible_get(Eo *obj EINA_UNUSED,
                             Ewe_Ruler_Smart_Data *sd,
                             Ewe_Ruler_Scale *scale)
{
   if (!scale) scale = eina_list_data_get(sd->scales);
   if (!scale) return EINA_FALSE;
   return scale->visible;
}

EOLIAN static Eina_Bool
_ewe_ruler_scale_visible_set(Eo *obj EINA_UNUSED,
                             Ewe_Ruler_Smart_Data *sd,
                             Ewe_Ruler_Scale *scale,
                             Eina_Bool visible)
{
   Eina_List *l;
   Ewe_Ruler_Marker *marker;

   if (!scale) scale = eina_list_data_get(sd->scales);
   if (!scale) return EINA_FALSE;
   if (scale->visible == visible) return EINA_TRUE;
   scale->visible = visible;

   if (!visible)
     {
        evas_object_hide(scale->box);
        EINA_LIST_FOREACH(scale->markers, l, marker)
          evas_object_hide(marker->obj);
     }
   else if (sd->ruler_visible)
     {
        evas_object_show(scale->box);
        EINA_LIST_FOREACH(scale->markers, l, marker)
          if (marker->visible)
            evas_object_show(marker->obj);
     }
   sd->size_changed = EINA_TRUE;
   evas_object_smart_changed(obj);
   return EINA_TRUE;
}

EOLIAN static Eina_Bool
_ewe_ruler_scale_middle_mark_get(Eo *obj EINA_UNUSED,
                                 Ewe_Ruler_Smart_Data *sd,
                                 Ewe_Ruler_Scale *scale)
{
   if (!scale) scale = eina_list_data_get(sd->scales);
   if (!scale) return EINA_FALSE;
   return scale->show_middle_mark;
}

EOLIAN static Eina_Bool
_ewe_ruler_scale_middle_mark_set(Eo *obj EINA_UNUSED,
                                 Ewe_Ruler_Smart_Data *sd,
                                 Ewe_Ruler_Scale *scale,
                                 Eina_Bool middle_mark)
{
   if (!scale) scale = eina_list_data_get(sd->scales);
   if (!scale) return EINA_FALSE;
   if (scale->show_middle_mark == middle_mark) return EINA_TRUE;
   scale->show_middle_mark = middle_mark;

   if (!middle_mark)
     {
        Eina_List *l;
        Evas_Object *mark;
        EINA_LIST_FOREACH(scale->dashes, l, mark)
          edje_object_part_text_set(mark, "ewe.middle.text", "");
     }

   sd->text_changed = EINA_TRUE;
   evas_object_smart_changed(obj);
   return EINA_TRUE;
}

EOLIAN static Ewe_Ruler_Marker *
_ewe_ruler_marker_add(Eo *obj,
                      Ewe_Ruler_Smart_Data *sd,
                      const char *style)
{
   Ewe_Ruler_Marker *ret = malloc(sizeof(Ewe_Ruler_Marker));
   if (!ret) return NULL;
   if (!style) style = DEFAULT_STYLE;

   sd->markers = eina_list_append(sd->markers, ret);

   ret->style = eina_stringshare_add(style);
   if (sd->horizontal)
     ret->full_style = eina_stringshare_printf("%s/%s", MARKER, style);
   else
     ret->full_style = eina_stringshare_printf("%s/%s", MARKER_VER, style);

   ret->obj = edje_object_add(obj);
   evas_object_clip_set(ret->obj, sd->clip);
   edje_object_file_set(ret->obj, EWE_THEME, ret->full_style);
   evas_object_smart_member_add(ret->obj, obj);

   ret->scale = NULL;
   ret->rel_position = 0;
   ret->abs_position = 0;
   ret->visible = EINA_TRUE;
   ret->relative = EINA_FALSE;

   return ret;
}

EOLIAN static Eina_Bool
_ewe_ruler_marker_del(Eo *obj EINA_UNUSED,
                      Ewe_Ruler_Smart_Data *sd,
                      Ewe_Ruler_Marker *marker)
{
   if (!marker) return EINA_FALSE;
   eina_stringshare_del(marker->style);
   eina_stringshare_del(marker->full_style);

   if (marker->relative)
     marker->scale->markers = eina_list_remove(marker->scale->markers, marker);

   sd->markers = eina_list_remove(sd->markers, marker);

   evas_object_smart_member_del(marker->obj);
   evas_object_del(marker->obj);
   free(marker);
   return EINA_TRUE;
}

EOLIAN static Eina_Bool
_ewe_ruler_marker_relative_set(Eo *obj,
                               Ewe_Ruler_Smart_Data *sd,
                               Ewe_Ruler_Marker *marker,
                               Ewe_Ruler_Scale *scale,
                               double pos)
{
   if (!marker) return EINA_FALSE;
   if (!scale) scale = eina_list_data_get(sd->scales);
   if (!scale) return EINA_FALSE;

   if (marker->relative)
     {
        if (marker->scale != scale)
          {
             marker->scale->markers = eina_list_remove(marker->scale->markers, marker);
             scale->markers = eina_list_append(scale->markers, marker);
             marker->scale = scale;
             if (!scale->visible)
               evas_object_hide(marker->obj);
             else if (marker->visible && sd->ruler_visible)
               evas_object_show(marker->obj);
          }
     }
   else
     {
        marker->relative = EINA_TRUE;
        scale->markers = eina_list_append(scale->markers, marker);
        marker->scale = scale;
     }
   marker->rel_position = pos;

   sd->text_changed = EINA_TRUE;
   evas_object_smart_changed(obj);
   return EINA_TRUE;
}

EOLIAN static double
_ewe_ruler_marker_relative_get(Eo *obj EINA_UNUSED,
                               Ewe_Ruler_Smart_Data *sd,
                               Ewe_Ruler_Marker *marker,
                               Ewe_Ruler_Scale *scale)
{
   if (!marker) return EINA_FALSE;
   if (!scale) scale = eina_list_data_get(sd->scales);
   if (!scale) return 0;
   if (marker->relative)
     marker->abs_position = (marker->scale->mark_value_step) ? ((marker->rel_position / marker->scale->mark_value_step) * marker->scale->mark_step + marker->scale->zero_pos) : marker->scale->zero_pos;

   if (marker->scale == scale)
     return marker->rel_position;
   else
     return scale->mark_value_step * (marker->abs_position - scale->zero_pos) / scale->mark_step;
}

EOLIAN static Eina_Bool
_ewe_ruler_marker_absolute_set(Eo *obj,
                               Ewe_Ruler_Smart_Data *sd,
                               Ewe_Ruler_Marker *marker,
                               int pos)
{
   if (!marker) return EINA_FALSE;

   if (marker->relative)
     {
        marker->scale->markers = eina_list_remove(marker->scale->markers, marker);
        marker->scale = NULL;
        marker->relative = EINA_FALSE;
        if (marker->visible && sd->ruler_visible)
          evas_object_show(marker->obj);
     }
   marker->abs_position = pos;

   sd->text_changed = EINA_TRUE;
   evas_object_smart_changed(obj);
   return EINA_TRUE;
}

EOLIAN static int
_ewe_ruler_marker_absolute_get(Eo *obj EINA_UNUSED,
                               Ewe_Ruler_Smart_Data *sd EINA_UNUSED,
                               Ewe_Ruler_Marker *marker)
{
   if (!marker) return EINA_FALSE;
   if (marker->relative)
     marker->abs_position = (marker->scale->mark_value_step) ? ((marker->rel_position / marker->scale->mark_value_step) * marker->scale->mark_step + marker->scale->zero_pos) : marker->scale->zero_pos;

   return marker->abs_position;
}

EOLIAN static Eina_Bool
_ewe_ruler_marker_visible_get(Eo *obj EINA_UNUSED,
                              Ewe_Ruler_Smart_Data *sd EINA_UNUSED,
                              Ewe_Ruler_Marker *marker)
{
   if (!marker) return EINA_FALSE;
   return marker->visible;
}

EOLIAN static Eina_Bool
_ewe_ruler_marker_visible_set(Eo *obj EINA_UNUSED,
                              Ewe_Ruler_Smart_Data *sd,
                              Ewe_Ruler_Marker *marker,
                              Eina_Bool visible)
{
   if (!marker) return EINA_FALSE;
   if (marker->visible == visible) return EINA_TRUE;
   marker->visible = visible;
   if (!visible)
     evas_object_hide(marker->obj);
   else if (sd->ruler_visible)
     evas_object_show(marker->obj);
   sd->text_changed = EINA_TRUE;
   evas_object_smart_changed(obj);
   return EINA_TRUE;
}

EOLIAN static Eina_Bool
_ewe_ruler_marker_style_set(Eo *obj,
                            Ewe_Ruler_Smart_Data *sd,
                            Ewe_Ruler_Marker *marker,
                            const char *style)
{
   if (!marker) return EINA_FALSE;
   eina_stringshare_del(marker->style);
   eina_stringshare_del(marker->full_style);
   marker->style = eina_stringshare_add(style);
   if (sd->horizontal)
     marker->full_style = eina_stringshare_printf("%s/%s", MARKER, style);
   else
     marker->full_style = eina_stringshare_printf("%s/%s", MARKER_VER, style);
   edje_object_file_set(marker->obj, EWE_THEME, marker->full_style);

   sd->text_changed = EINA_TRUE;
   evas_object_smart_changed(obj);
   return EINA_TRUE;
}

EOLIAN static const char *
_ewe_ruler_marker_style_get(Eo *obj EINA_UNUSED,
                            Ewe_Ruler_Smart_Data *sd EINA_UNUSED,
                            Ewe_Ruler_Marker *marker)
{
   if (!marker) return NULL;
   return marker->style;
}

/*------------------------Smart functions ----------------------------*/

EOLIAN static void
_ewe_ruler_evas_object_smart_add(Eo *obj, Ewe_Ruler_Smart_Data *sd)
{
   evas_obj_smart_add(eo_super(obj, MY_CLASS));
   elm_widget_sub_object_parent_add(obj);

   sd->obj = obj;
   sd->clip = evas_object_rectangle_add(evas_object_evas_get(obj));
   evas_object_smart_member_add(sd->clip, obj);
   sd->bg = edje_object_add(evas_object_evas_get(obj));
   edje_object_file_set(sd->bg, EWE_THEME, "ewe/ruler/horizontal_background/default");
   evas_object_smart_member_add(sd->bg, obj);

   sd->horizontal = EINA_TRUE;
   sd->geometry.width = 0;
   sd->geometry.height = 0;
   sd->geometry.x = 0;
   sd->geometry.y = 0;
   sd->ruler_visible = EINA_FALSE;
   sd->size_changed = EINA_TRUE;
   sd->text_changed = EINA_TRUE;
   sd->position_changed = EINA_TRUE;
   ewe_ruler_scale_add(obj, NULL);
}

EOLIAN static void
_ewe_ruler_evas_object_smart_del(Eo *obj,
                                 Ewe_Ruler_Smart_Data *sd)
{
   Ewe_Ruler_Scale *scale;
   Ewe_Ruler_Marker *marker;
   Eina_List *l, *ls;

   EINA_LIST_FOREACH_SAFE(sd->markers, l, ls, marker)
     ewe_ruler_marker_del(obj, marker);

   EINA_LIST_FOREACH_SAFE(sd->scales, l, ls, scale)
     ewe_ruler_scale_del(obj, scale);

   evas_object_smart_member_del(sd->clip);
   evas_object_del(sd->clip);

   evas_object_smart_member_del(sd->bg);
   evas_object_del(sd->bg);
}

EOLIAN static void
_ewe_ruler_evas_object_smart_show(Eo* obj EINA_UNUSED, Ewe_Ruler_Smart_Data *sd)
{
   if (sd->ruler_visible) return;
   Ewe_Ruler_Scale *scale;
   Ewe_Ruler_Marker *marker;
   Eina_List *ls, *l;
   Evas_Object *data;

   evas_object_show(sd->bg);
   evas_object_show(sd->clip);
   EINA_LIST_FOREACH(sd->scales, ls, scale)
     {
        if (scale->visible) evas_object_show(scale->box);
     }

   sd->ruler_visible = EINA_TRUE;

   EINA_LIST_FOREACH(sd->markers, l, marker)
     if (marker->visible)
       evas_object_show(marker->obj);

   EINA_LIST_FOREACH(sd->scales, ls, scale)
     if (scale->visible)
       {
          EINA_LIST_FOREACH(scale->dashes, l, data)
             evas_object_show(data);
       }
     else
       {
          EINA_LIST_FOREACH(scale->markers, l, marker)
             evas_object_hide(marker->obj);
       }
}

EOLIAN static void
_ewe_ruler_evas_object_smart_hide(Eo* obj, Ewe_Ruler_Smart_Data *sd)
{
   if (!sd->ruler_visible) return;
   evas_obj_smart_hide(eo_super(obj, MY_CLASS));
   Ewe_Ruler_Scale *scale;
   Ewe_Ruler_Marker *marker;
   Eina_List *ls, *l;
   Evas_Object *data;

   evas_object_hide(sd->bg);
   evas_object_hide(sd->clip);
   EINA_LIST_FOREACH(sd->scales, ls, scale)
    {
       evas_object_hide(scale->box);
    }

   EINA_LIST_FOREACH(sd->markers, l, marker)
     evas_object_hide(marker->obj);

   sd->ruler_visible = EINA_FALSE;
   EINA_LIST_FOREACH(sd->scales, ls, scale)
     EINA_LIST_FOREACH(scale->dashes, l, data)
       evas_object_hide(data);
}

EOLIAN static void
_ewe_ruler_evas_object_smart_move(Eo *obj,
                                  Ewe_Ruler_Smart_Data *sd,
                                  Evas_Coord x,
                                  Evas_Coord y)
{

   evas_obj_smart_move(eo_super(obj, MY_CLASS), x, y);

   evas_object_move(sd->clip, x, y);
   evas_object_move(sd->bg, x, y);

   sd->geometry.x = x;
   sd->geometry.y = y;
   sd->position_changed = EINA_TRUE;
   evas_object_smart_changed(obj);
}

EOLIAN static void
_ewe_ruler_evas_object_smart_resize(Eo *obj,
                                    Ewe_Ruler_Smart_Data *sd,
                                    Evas_Coord w,
                                    Evas_Coord h)
{
   if ((w == sd->geometry.width) && (h == sd->geometry.height)) return;

   sd->size_changed = EINA_TRUE;
   sd->geometry.width = w;
   sd->geometry.height = h;

   evas_obj_smart_resize(eo_super(obj, MY_CLASS), w, h);
   evas_object_resize(sd->clip, w, h);
   evas_object_resize(sd->bg, w, h);

   evas_object_smart_changed(obj);
}

EOLIAN static void
_ewe_ruler_evas_object_smart_calculate(Eo *obj EINA_UNUSED,
                                       Ewe_Ruler_Smart_Data *sd)
{
   Eina_List *ls;
   Ewe_Ruler_Scale *scale;
   int offset;

   if (sd->size_changed)
     {
        _add_dashes(sd);
        EINA_LIST_FOREACH(sd->scales, ls, scale)
          {
             _dashes_size_set(sd, scale);
          }
     }
   if (sd->position_changed)
     {
        EINA_LIST_FOREACH(sd->scales, ls, scale)
          {
             if (scale->visible)
               {
                  offset = (scale->zero_pos % scale->mark_step) - scale->mark_step;
                  if (sd->horizontal)
                    evas_object_move(scale->box, sd->geometry.x + offset, sd->geometry.y);
                  else
                    evas_object_move(scale->box, sd->geometry.x, sd->geometry.y + offset);
               }
          }
        sd->position_changed = EINA_FALSE;
     }
   if (sd->text_changed)
     {
        _set_labels(sd);
        _place_markers(sd);
     }
}

EOLIAN static Eina_Bool
_ewe_ruler_elm_widget_theme_apply(Eo *obj, Ewe_Ruler_Smart_Data *sd)
{
   Eina_Bool int_ret;
   int count;
   int_ret = elm_obj_widget_theme_apply(eo_super(obj, MY_CLASS));
   if (!int_ret) return EINA_FALSE;

   Ewe_Ruler_Scale *scale;
   Eina_List *ls;

   if (sd->horizontal)
     {
        edje_object_file_set(sd->bg, EWE_THEME, "ewe/ruler/horizontal_background/default");
        EINA_LIST_FOREACH(sd->scales, ls, scale)
          {
             eina_stringshare_del(scale->full_style);
             scale->full_style = eina_stringshare_printf("%s/%s", DASHES, scale->style);
             count = eina_list_count(scale->dashes);
             _delete_extra_dashes(scale, count);
          }
     }
   else
     {
        edje_object_file_set(sd->bg, EWE_THEME, "ewe/ruler/vertical_background/default");
        EINA_LIST_FOREACH(sd->scales, ls, scale)
          {
             eina_stringshare_del(scale->full_style);
             scale->full_style = eina_stringshare_printf("%s/%s", DASHES_VER, scale->style);
             count = eina_list_count(scale->dashes);
             _delete_extra_dashes(scale, count);
          }
     }
   return EINA_TRUE;
}

/*---------------------------Legacy functions --------------------------------*/

EAPI Evas_Object *
ewe_ruler_add(Evas_Object *parent)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(parent, NULL);
   Evas_Object *obj = eo_add(MY_CLASS, parent);

   return obj;
}

/* -----------------------------EO operations-------------------------------- */

EOLIAN static Eo*
_ewe_ruler_eo_base_constructor(Eo *obj, Ewe_Ruler_Smart_Data *sd)
{
   obj = eo_constructor(eo_super(obj, MY_CLASS));
   sd->obj = obj;
   evas_obj_type_set(obj, MY_CLASS_NAME_LEGACY);
   evas_obj_smart_callbacks_descriptions_set(obj, _smart_callbacks);
   return obj;
}

EOLIAN static void
_ewe_ruler_class_constructor(Eo_Class *klass)
{
   evas_smart_legacy_type_register(MY_CLASS_NAME_LEGACY, klass);
}

#include "ewe_ruler.eo.c"
#undef _EWE_RULER_UNIT_ADD
