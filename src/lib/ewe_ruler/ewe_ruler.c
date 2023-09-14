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


#define EFL_UI_WIDGET_PROTECTED
#define EFL_ACCESS_OBJECT_PROTECTED
#define EFL_ACCESS_WIDGET_ACTION_PROTECTED
#define ELM_LAYOUT_PROTECTED
#define EFL_PART_PROTECTED

#include "ewe_private.h"
#include "ewe_widget_ruler.h"
#undef EAPI
#define EAPI
#include "ewe_ruler.eo.h"

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

#define EWE_RULER_EXTRA_OPS \
   EFL_CANVAS_GROUP_ADD_OPS(ewe_ruler)

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
                  edje_object_file_set(object, sd->theme_file, scale->full_style);
                  scale->dashes = eina_list_append(scale->dashes, object);
                  if (sd->horizontal)
                    evas_object_size_hint_min_set(object, scale->mark_step, sd->geometry.height);
                  else
                    evas_object_size_hint_min_set(object, sd->geometry.width, scale->mark_step);
                  elm_box_pack_end(scale->box, object);
                  evas_object_show(object);
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
             marker->abs_position = !EINA_DBL_EQ(scale->mark_value_step, 0) ? ((marker->rel_position / scale->mark_value_step) * scale->mark_step + scale->zero_pos) : scale->zero_pos;
          }

        if (sd->horizontal)
          {
             /*size of marker(pointer) is set to 1 to make posible centring from style*/
             evas_object_resize(marker->obj, marker->size, sd->geometry.height);
             evas_object_move(marker->obj,
                              sd->geometry.x + marker->abs_position,
                              sd->geometry.y);
          }
        else
          {
             /*size of marker(pointer) is set to 1 to make posible centring from style*/
             evas_object_resize(marker->obj, sd->geometry.width, marker->size);
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
   Eina_Strbuf *buf;

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
        elm_layout_file_set(sd->bg, sd->theme_file, "ewe/ruler/horizontal_background/default");
        dashes = DASHES;
        markers = MARKER;
     }
   else
     {
        elm_layout_file_set(sd->bg, sd->theme_file, "ewe/ruler/vertical_background/default");
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
   buf = eina_strbuf_new();
   EINA_LIST_FOREACH(sd->markers, lm, marker)
     {
        eina_strbuf_reset(buf);
        eina_strbuf_append_printf(buf, "%s/%s", markers, marker->style);
        elm_layout_file_set(marker->obj, sd->theme_file, eina_strbuf_string_get(buf));
     }

   eina_strbuf_free(buf);
   evas_object_smart_changed(obj);
   return EINA_TRUE;
}

EOLIAN static Eina_Bool
_ewe_ruler_horizontal_get(const Eo *obj EINA_UNUSED, Ewe_Ruler_Smart_Data *sd)
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
_ewe_ruler_step_get(const Eo *obj EINA_UNUSED,
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
_ewe_ruler_value_step_get(const Eo *obj EINA_UNUSED,
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
_ewe_ruler_zero_offset_get(const Eo *obj EINA_UNUSED,
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
_ewe_ruler_format_get(const Eo *obj EINA_UNUSED,
                      Ewe_Ruler_Smart_Data *sd,
                      Ewe_Ruler_Scale *scale)
{
   if (!scale) scale = eina_list_data_get(sd->scales);
   if (!scale) return NULL;
   return scale->format;
}

EOLIAN static Eina_Bool
_ewe_ruler_ewe_style_set(Eo *obj,
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
_ewe_ruler_ewe_style_get(const Eo *obj EINA_UNUSED,
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

   ret->box = elm_box_add(obj);
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

   sd->scales = eina_list_append(sd->scales, ret);

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
_ewe_ruler_scale_visible_get(const Eo *obj EINA_UNUSED,
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
_ewe_ruler_scale_middle_mark_get(const Eo *obj EINA_UNUSED,
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
          elm_layout_text_set(mark, "ewe.middle.text", "");
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
   Eina_Strbuf *buf;
   Ewe_Ruler_Marker *ret = malloc(sizeof(Ewe_Ruler_Marker));
   if (!ret) return NULL;
   if (!style) style = DEFAULT_STYLE;

   ret->style = eina_stringshare_add(style);
   buf = eina_strbuf_new();
   if (sd->horizontal)
     eina_strbuf_append_printf(buf, MARKER"/%s", style);
   else
     eina_strbuf_append_printf(buf, MARKER_VER"/%s", style);

   ret->obj = elm_layout_add(obj);
   evas_object_clip_set(ret->obj, sd->clip);
   elm_layout_file_set(ret->obj, sd->theme_file, eina_strbuf_string_get(buf));
   evas_object_smart_member_add(ret->obj, obj);

   if (sd->horizontal)
     edje_object_size_min_calc(elm_layout_edje_get(ret->obj), &ret->size, NULL);
   else
     edje_object_size_min_calc(elm_layout_edje_get(ret->obj), NULL, &ret->size);

   ret->scale = NULL;
   ret->rel_position = 0;
   ret->abs_position = 0;
   ret->visible = EINA_TRUE;
   ret->relative = EINA_FALSE;

   sd->markers = eina_list_append(sd->markers, ret);

   eina_strbuf_free(buf);
   return ret;
}

EOLIAN static Eina_Bool
_ewe_ruler_marker_del(Eo *obj EINA_UNUSED,
                      Ewe_Ruler_Smart_Data *sd,
                      Ewe_Ruler_Marker *marker)
{
   if (!marker) return EINA_FALSE;
   eina_stringshare_del(marker->style);

   if (marker->relative)
     marker->scale->markers = eina_list_remove(marker->scale->markers, marker);

   sd->markers = eina_list_remove(sd->markers, marker);

   evas_object_smart_member_del(marker->obj);
   evas_object_del(marker->obj);
   free(marker);
   return EINA_TRUE;
}

EOLIAN static void
_ewe_ruler_marker_size_set(Eo *obj,
                           Ewe_Ruler_Smart_Data *sd EINA_UNUSED,
                           Ewe_Ruler_Marker *marker,
                           int size)
{
   marker->size = size;
   evas_object_smart_changed(obj);
}

EOLIAN static int
_ewe_ruler_marker_size_get(const Eo *obj EINA_UNUSED,
                           Ewe_Ruler_Smart_Data *sd EINA_UNUSED,
                           Ewe_Ruler_Marker *marker)
{
   return marker->size;
}

EOLIAN static void
_ewe_ruler_marker_text_set(Eo *obj EINA_UNUSED,
                           Ewe_Ruler_Smart_Data *sd EINA_UNUSED,
                           Ewe_Ruler_Marker *marker,
                           const char *text)
{
   elm_layout_text_set(marker->obj, "ewe.text", text);
}

EOLIAN static const char *
_ewe_ruler_marker_text_get(const Eo *obj EINA_UNUSED,
                           Ewe_Ruler_Smart_Data *sd EINA_UNUSED,
                           Ewe_Ruler_Marker *marker)
{
   return elm_layout_text_get(marker->obj, "ewe.text");
}

EOLIAN static void
_ewe_ruler_marker_tooltip_set(Eo *obj EINA_UNUSED,
                              Ewe_Ruler_Smart_Data *sd EINA_UNUSED,
                              Ewe_Ruler_Marker *marker,
                              const char *text)
{
   elm_object_tooltip_text_set(marker->obj, text);
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
_ewe_ruler_marker_relative_get(const Eo *obj EINA_UNUSED,
                               Ewe_Ruler_Smart_Data *sd,
                               Ewe_Ruler_Marker *marker,
                               Ewe_Ruler_Scale *scale)
{
   if (!marker) return EINA_FALSE;
   if (!scale) scale = eina_list_data_get(sd->scales);
   if (!scale) return 0;
   if (marker->relative)
     marker->abs_position = !EINA_DBL_EQ(marker->scale->mark_value_step, 0) ? ((marker->rel_position / marker->scale->mark_value_step) * marker->scale->mark_step + marker->scale->zero_pos) : marker->scale->zero_pos;

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
_ewe_ruler_marker_absolute_get(const Eo *obj EINA_UNUSED,
                               Ewe_Ruler_Smart_Data *sd EINA_UNUSED,
                               Ewe_Ruler_Marker *marker)
{
   if (!marker) return EINA_FALSE;
   if (marker->relative)
     marker->abs_position = !EINA_DBL_EQ(marker->scale->mark_value_step, 0) ? ((marker->rel_position / marker->scale->mark_value_step) * marker->scale->mark_step + marker->scale->zero_pos) : marker->scale->zero_pos;

   return marker->abs_position;
}

EOLIAN static Eina_Bool
_ewe_ruler_marker_visible_get(const Eo *obj EINA_UNUSED,
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
   Eina_Strbuf *buf;

   if (!marker) return EINA_FALSE;
   eina_stringshare_del(marker->style);
   marker->style = eina_stringshare_add(style);

   buf = eina_strbuf_new();
   if (sd->horizontal)
     eina_strbuf_append_printf(buf, MARKER"/%s", style);
   else
     eina_strbuf_append_printf(buf, MARKER_VER"/%s", style);
   elm_layout_file_set(marker->obj, sd->theme_file, eina_strbuf_string_get(buf));

   eina_strbuf_free(buf);
   evas_object_smart_changed(obj);
   return EINA_TRUE;
}

EOLIAN static const char *
_ewe_ruler_marker_style_get(const Eo *obj EINA_UNUSED,
                            Ewe_Ruler_Smart_Data *sd EINA_UNUSED,
                            Ewe_Ruler_Marker *marker)
{
   if (!marker) return NULL;
   return marker->style;
}

/*------------------------Smart functions ----------------------------*/

EOLIAN static void
_ewe_ruler_efl_canvas_group_group_add(Eo *obj, Ewe_Ruler_Smart_Data *sd)
{
   const char *data_dir = NULL;
   Eina_Strbuf *theme_path = NULL;

   efl_canvas_group_add(efl_super(obj, MY_CLASS));
   //elm_widget_sub_object_parent_add(obj);

   data_dir = elm_app_data_dir_get();
   if (data_dir && strcmp(data_dir, ""))
     {
        theme_path = eina_strbuf_new();
        eina_strbuf_append_printf(theme_path,
                           "%s"EINA_PATH_SEP_S"themes"EINA_PATH_SEP_S"default"EINA_PATH_SEP_S"ewe.edj",
                           data_dir);
        sd->theme_file = eina_strbuf_string_steal(theme_path);
        eina_strbuf_free(theme_path);
     }
   else
     {
        sd->theme_file = strdup(EWE_THEME);
     }

   sd->obj = obj;
   sd->clip = evas_object_rectangle_add(evas_object_evas_get(obj));
   evas_object_smart_member_add(sd->clip, obj);
   sd->bg = elm_layout_add(obj);
   elm_layout_file_set(sd->bg, sd->theme_file, "ewe/ruler/horizontal_background/default");
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
_ewe_ruler_efl_canvas_group_group_del(Eo *obj, Ewe_Ruler_Smart_Data *sd)
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

   free(sd->theme_file);
   
   // efl_canvas_group_member_remove(efl_super(obj, MY_CLASS), sub_obj);
   // efl_unref(obj);
}

static void
_ewe_ruler_show(Eo* obj, Ewe_Ruler_Smart_Data *sd)
{
   if (sd->ruler_visible) return;
   efl_gfx_entity_visible_set(efl_super(obj, MY_CLASS), EINA_TRUE);

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

static void
_ewe_ruler_hide(Eo* obj, Ewe_Ruler_Smart_Data *sd)
{
   if (!sd->ruler_visible) return;
   efl_gfx_entity_visible_set(efl_super(obj, MY_CLASS), EINA_FALSE);

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
_ewe_ruler_efl_gfx_entity_visible_set(Eo *obj, Ewe_Ruler_Smart_Data *sd, Eina_Bool vis)
{
   if (_evas_object_intercept_call(obj, EVAS_OBJECT_INTERCEPT_CB_VISIBLE, 0, vis))
     return;

   if (vis)
     _ewe_ruler_show(obj, sd);
   else
     _ewe_ruler_hide(obj, sd);
}

EOLIAN static void
_ewe_ruler_efl_gfx_entity_position_set(Eo *obj,
                                Ewe_Ruler_Smart_Data *sd,
                                Eina_Position2D pos)
{
   int x = pos.x;
   int y = pos.y;
   if (_evas_object_intercept_call(obj, EVAS_OBJECT_INTERCEPT_CB_MOVE, 0, x, y))
     return;

   efl_gfx_entity_position_set(efl_super(obj, MY_CLASS), pos);

   evas_object_move(sd->clip, x, y);
   evas_object_move(sd->bg, x, y);

   sd->geometry.x = x;
   sd->geometry.y = y;
   sd->position_changed = EINA_TRUE;
   evas_object_smart_changed(obj);
}

EOLIAN static void
_ewe_ruler_efl_gfx_entity_size_set(Eo *obj,
                            Ewe_Ruler_Smart_Data *sd,
                            Eina_Size2D size)
{
   int w = size.w;
   int h = size.h;
   if ((w == sd->geometry.width) && (h == sd->geometry.height)) return;
   if (_evas_object_intercept_call(obj, EVAS_OBJECT_INTERCEPT_CB_RESIZE, 0, w, h))
     return;

   sd->size_changed = EINA_TRUE;
   sd->geometry.width = w;
   sd->geometry.height = h;

   evas_object_resize(sd->clip, w, h);
   evas_object_resize(sd->bg, w, h);

   evas_object_smart_changed(obj);

   efl_gfx_entity_size_set(efl_super(obj, MY_CLASS), size);
}

EOLIAN static void
_ewe_ruler_efl_canvas_group_group_calculate(Eo *obj EINA_UNUSED,
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
        EINA_LIST_FOREACH(sd->scales, ls, scale)
          {
             int dashes_count = eina_list_count(scale->dashes);
             if (sd->horizontal)
               {
               evas_object_size_hint_min_set(scale->box, dashes_count * scale->mark_step, sd->geometry.height);
               evas_object_resize(scale->box, dashes_count * scale->mark_step, sd->geometry.height);
               }
             else
               {
               evas_object_size_hint_min_set(scale->box, sd->geometry.width, dashes_count * scale->mark_step);
               evas_object_resize(scale->box, sd->geometry.width, dashes_count * scale->mark_step);
               }

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

EOLIAN static Eina_Error
_ewe_ruler_efl_ui_widget_theme_apply(Eo *obj, Ewe_Ruler_Smart_Data *sd)
{
   Eina_Error int_ret = EFL_UI_THEME_APPLY_ERROR_GENERIC;
   int count;
   int_ret = efl_ui_widget_theme_apply(efl_super(obj, MY_CLASS));
   if (int_ret == EFL_UI_THEME_APPLY_ERROR_GENERIC) return int_ret;

   Ewe_Ruler_Scale *scale;
   Eina_List *ls;

   if (sd->horizontal)
     {
        elm_layout_file_set(sd->bg, sd->theme_file, "ewe/ruler/horizontal_background/default");
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
        elm_layout_file_set(sd->bg, sd->theme_file, "ewe/ruler/vertical_background/default");
        EINA_LIST_FOREACH(sd->scales, ls, scale)
          {
             eina_stringshare_del(scale->full_style);
             scale->full_style = eina_stringshare_printf("%s/%s", DASHES_VER, scale->style);
             count = eina_list_count(scale->dashes);
             _delete_extra_dashes(scale, count);
          }
     }
   return int_ret;
}

/*---------------------------Legacy functions --------------------------------*/

Evas_Object *
ewe_ruler_add(Evas_Object *parent)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(parent, NULL);
   Evas_Object *obj = efl_add(MY_CLASS, parent);

   return obj;
}

/* -----------------------------EO operations-------------------------------- */

EOLIAN static Eo*
_ewe_ruler_efl_object_constructor(Eo *obj, Ewe_Ruler_Smart_Data *sd)
{
   obj = efl_constructor(efl_super(obj, MY_CLASS));
   sd->obj = obj;
   evas_object_smart_callbacks_descriptions_set(obj, _smart_callbacks);

   return obj;
}

EOLIAN static void
_ewe_ruler_class_constructor(Efl_Class *klass)
{
   evas_smart_legacy_type_register(MY_CLASS_NAME_LEGACY, klass);
}

#include "ewe_ruler.eo.c"
#undef _EWE_RULER_UNIT_ADD
