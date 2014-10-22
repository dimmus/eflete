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

#include "animator_private.h"

#define MY_CLASS_NAME "Prog_Sequence"
#define LABELS_W 200
#define LABELS_H 25
#define TIME_LIM 100
#define PIX_PER_SEC 250
#define PIX_PER_LINE 40
#define PROGRAM_COLOR 52, 70, 84, 255
#define PROGRAM_SELECTED_COLOR 58, 110, 155, 255
#define BG_COLOR 50, 53, 54, 255
#define LABELS_BG_COLOR 20, 20, 20, 20

struct _Part_State
{
   Eina_Stringshare *name;
   Eina_Stringshare *state;
   double state_value;
};
typedef struct _Part_State Part_State;

struct _Run_Prog
{
   Eina_Stringshare *name;
   double start_time;
   double delay;
   double time;
   Evas_Object *obj;
};
typedef struct _Run_Prog Run_Prog;

struct _Playback_State
{
   Part_State *parts;
   Eina_List *runprogs;
   double time;
};

typedef struct _Playback_State Playback_State;


typedef struct _Prog_Sequence_Smart_Data Prog_Sequence_Smart_Data;
struct _Prog_Sequence_Smart_Data
{
   Evas_Object_Smart_Clipped_Data base;
   Evas *e;
   Evas_Object *obj;
   Evas_Object *ruler;
   Evas_Object *bg;
   Evas_Object *labels_bg;
   Evas_Object *parent;
   Live_View *live;
   Style *style;
   Eina_List *timeline;
   Eina_List *program_list;
   Eina_List *labels;
   double max_time;
   Eina_Stringshare *program;
};

#define PROG_SEQUENCE_DATA_GET(o, ptr)  \
   Prog_Sequence_Smart_Data *ptr = evas_object_smart_data_get(o);

#define PROG_SEQUENCE_DATA_GET_OR_RETURN_VAL(o, ptr, val)               \
   PROG_SEQUENCE_DATA_GET(o, ptr)                                       \
   if (!ptr)                                                            \
     {                                                                  \
        ERR("No program_sequence data for object %p (%s)!",             \
            o, evas_object_type_get(o));                                \
        return val;                                                     \
     }

static void _prog_sequence_smart_add(Evas_Object *o);
static void _prog_sequence_smart_del(Evas_Object *o);
static void _prog_sequence_smart_show(Evas_Object *o);
static void _prog_sequence_smart_hide(Evas_Object *o);
static void _prog_sequence_smart_move(Evas_Object *o, Evas_Coord x, Evas_Coord y);
static void _prog_sequence_smart_resize(Evas_Object *o, Evas_Coord w, Evas_Coord h);
static void _prog_sequence_smart_calculate(Evas_Object *o);
static void _overlay_move(Prog_Sequence_Smart_Data *sd, int x, int y);

EVAS_SMART_SUBCLASS_NEW(MY_CLASS_NAME, _prog_sequence,
                        Evas_Smart_Class, Evas_Smart_Class,
                        evas_object_smart_clipped_class_get, NULL);

static void
_prog_sequence_smart_set_user(Evas_Smart_Class *sc)
{
   evas_object_smart_clipped_smart_set(sc);

   sc->add = _prog_sequence_smart_add;
   sc->del = _prog_sequence_smart_del;
   sc->show = _prog_sequence_smart_show;
   sc->hide = _prog_sequence_smart_hide;

   sc->move = _prog_sequence_smart_move;
   sc->resize = _prog_sequence_smart_resize;
   sc->calculate = _prog_sequence_smart_calculate;
}

/***************** evas_object functions **************************************/

static void
_prog_sequence_smart_add(Evas_Object *o)
{
   EVAS_SMART_DATA_ALLOC(o, Prog_Sequence_Smart_Data)

   _prog_sequence_parent_sc->add(o);
   priv->e = evas_object_evas_get(o);

   priv->obj = o;
}

static void
_prog_sequence_smart_del(Evas_Object *o)
{
   PROG_SEQUENCE_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID)

   evas_object_smart_member_del(sd->bg);
   evas_object_smart_member_del(sd->labels_bg);
   evas_object_smart_member_del(sd->ruler);

   _prog_sequence_parent_sc->del(o);
}

static void
_prog_sequence_smart_show(Evas_Object *o)
{
   if (evas_object_visible_get(o)) return;

   PROG_SEQUENCE_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID);

   evas_object_show(sd->bg);
   evas_object_show(sd->labels_bg);
   evas_object_show(sd->ruler);

   _prog_sequence_parent_sc->show(o);
}

static void
_prog_sequence_smart_hide(Evas_Object *o)
{
   if (!evas_object_visible_get(o)) return;

   PROG_SEQUENCE_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID)

   evas_object_hide(sd->bg);
   evas_object_hide(sd->labels_bg);
   evas_object_hide(sd->ruler);

   _prog_sequence_parent_sc->hide(o);
}

static void
_prog_sequence_smart_move(Evas_Object *o,
                          Evas_Coord x,
                          Evas_Coord y)
{
   Evas_Coord ox, oy;
   PROG_SEQUENCE_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID)

   evas_object_geometry_get(o, &ox, &oy, NULL, NULL);
   if ((ox == x) && (oy == y)) return;

   _prog_sequence_parent_sc->move(o, x, y);
   _overlay_move(sd, x, y);
}

static void
_prog_sequence_smart_resize(Evas_Object *o,
                            Evas_Coord w,
                            Evas_Coord h)
{
   Evas_Coord ow, oh;
   PROG_SEQUENCE_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID)

   evas_object_geometry_get(o, NULL, NULL, &ow, &oh);
   if ((ow == w) && (oh == h)) return;

   evas_object_smart_changed(o);
}

static void
_prog_sequence_smart_calculate(Evas_Object *o)
{
   Evas_Coord x, y, w, h;
   Evas_Coord ow, oh;
   Run_Prog *rp;
   Eina_List *l, *lp;
   int line_num;
   Eina_Stringshare *name;

   PROG_SEQUENCE_DATA_GET_OR_RETURN_VAL(o, sd, RETURN_VOID)
   evas_object_geometry_get(sd->obj, NULL, NULL, &ow, &oh);
   evas_object_resize(sd->bg, ow, oh);
   evas_object_resize(sd->labels_bg, LABELS_W, oh);
   evas_object_resize(sd->ruler, ow - LABELS_W, LABELS_H);

   evas_object_geometry_get(o, &x, &y, &w, &h);
   evas_object_size_hint_min_set(o, sd->max_time * PIX_PER_SEC + 2 * LABELS_W,
                                 eina_list_count(sd->program_list) * PIX_PER_LINE + LABELS_H);

   _overlay_move(sd, x, y);

   EINA_LIST_FOREACH(sd->timeline, l, rp)
     {
        line_num = -1;
        EINA_LIST_FOREACH(sd->program_list, lp, name)
          {
             line_num++;
             /* both rp->name and name are stringshares so this check is
                equal to !strcmp(rp->name, name) but is alot faster */
             if (rp->name == name) break;
          }

        /* TODO: simplify after replacing rectangles with secialized objects */
        evas_object_geometry_set(rp->obj,
                                 x + rp->start_time * PIX_PER_SEC + LABELS_W,
                                 y + ((rp->time?0.1:0.25) + line_num) * PIX_PER_LINE + LABELS_H,
                                 (rp->time?rp->time:.1) * PIX_PER_SEC,
                                 (rp->time?.9:.5) * PIX_PER_LINE);
     }
}
/***************** program sequence custom ************************************/

static void
_overlay_move(Prog_Sequence_Smart_Data *sd, int x, int y)
{
   Evas_Object *label;
   Evas_Coord px, py;
   Eina_List *l;
   int line_num = 0;

   /* assuming that parent is scroller and its x-coord is starting point of viewport.
      labels block shouldn't scroll along X-axis.
      labels in list are sorted by y-coord */
   evas_object_geometry_get(sd->parent, &px, &py, NULL, NULL);
   EINA_LIST_FOREACH(sd->labels, l, label)
     {
         evas_object_move(label,
                          px,
                          y + (0.25 + line_num) * PIX_PER_LINE + LABELS_H);
         line_num++;
     }
   evas_object_move(sd->labels_bg, px, y);
   evas_object_move(sd->ruler, x + LABELS_W, py);
}

static int
_run_prog_cmp(const void *d1, const void *d2)
{
   const Run_Prog *p1 = d1;
   const Run_Prog *p2 = d2;
   /* assuming that list sort functions accepting "<0, 0, >0" instead of "-1, 0, 1" */
   return p1->start_time - p2->start_time;
}

static Run_Prog *
_item_create(Prog_Sequence_Smart_Data *sd, const char* program, double time)
{
   Run_Prog *rp = mem_malloc(sizeof(Run_Prog));

   rp->start_time = time;
   rp->name = eina_stringshare_add(program);
   rp->delay = edje_edit_program_in_from_get(sd->style->obj, program);
   rp->time = edje_edit_program_transition_time_get(sd->style->obj, program);

   /* TODO: replace with special object */
   rp->obj = evas_object_rectangle_add(sd->e);
   evas_object_smart_member_add(rp->obj, sd->obj);
   evas_object_show(rp->obj);

   /* selected program should be highlighted */
   if (rp->name == sd->program)
     evas_object_color_set(rp->obj, PROGRAM_SELECTED_COLOR);
   else
     evas_object_color_set(rp->obj, PROGRAM_COLOR);
   return rp;
}

static void
_item_free(Run_Prog *rp)
{
   eina_stringshare_del(rp->name);
   evas_object_del(rp->obj);
   free(rp);
}

static void
_timeline_init(Prog_Sequence_Smart_Data *sd, Eina_Stringshare *program)
{
   Eina_List *list = NULL, *lnear, *l;
   Eina_List *afters_list = NULL, *la, *l_check;
   const char *after_name;
   Run_Prog *cur, *rp_check;
   double time = 0;
   int cmp;

   Evas_Object *obj = sd->style->obj;
   Run_Prog *rp;
   rp = _item_create(sd, program, time);
   list = eina_list_append(list, rp);
   sd->max_time = 0; /* used to calculate min width and max playback time */

   EINA_LIST_FOREACH(list, l, cur)
     {
        time = cur->start_time + cur->delay + cur->time;

        /* timelimit is needed to break infinite loops */
        if (time > TIME_LIM) break;

        afters_list = edje_edit_program_afters_get(obj, cur->name);
        EINA_LIST_FOREACH(afters_list, la, after_name)
          {
             rp = _item_create(sd, after_name, time);
             if (time + rp->delay > sd->max_time)
               sd->max_time = time + rp->delay;

             /* inserting programs into list sorted by start time */
             lnear = eina_list_search_sorted_near_list(list, _run_prog_cmp, rp, &cmp);
             if (cmp < 0)
               list = eina_list_append_relative_list(list, rp, lnear);
             else if (cmp > 0)
               list = eina_list_prepend_relative_list(list, rp, lnear);
             else /* Found at least one program with the same start time in
                     timeline list. To avoid infinite loop of 0-time programs
                     we should make additonal check */
               {
                  /* finding first program with the same start_time */
                  l_check = lnear;
                  while (!!(l_check = eina_list_prev(l_check)))
                    {
                       rp_check = eina_list_data_get(l_check);
                       if (rp_check->start_time == time)
                         lnear = l_check;
                       else
                         break;
                    }
                  /* checking that there are no other program with the same name and start_time */
                  /* assuming that foreach macro can start not only from first node */
                  EINA_LIST_FOREACH(lnear, l_check, rp_check)
                    {
                       if (rp_check->start_time > time) break; /* Hooray! That was only paranoia */
                       if (!strcmp(rp_check->name, after_name))
                         {
                            /* 0-time loop found. rp should not be added to timeline*/
                            _item_free(rp);
                            rp = NULL;
                            break;
                         }
                       lnear = l_check;
                    }
                  if (rp) list = eina_list_append_relative_list(list, rp, lnear);
               }
          }
        eina_list_free(afters_list);
     }

   sd->timeline = list;
}

static void
_timeline_free(Prog_Sequence_Smart_Data *sd)
{
   Run_Prog *cur;
   EINA_LIST_FREE(sd->timeline, cur)
     {
         eina_stringshare_del(cur->name);
         _item_free(cur);
     }
   sd->timeline = NULL;
}

Evas_Object *
prog_sequence_add(Evas_Object *parent, Style *style, Live_View *live)
{
   Evas *e;
   Evas_Object *obj;

   if (!parent) return NULL;
   if (!live) return NULL;
   if (!style) return NULL;

   e = evas_object_evas_get(parent);
   obj = evas_object_smart_add(e, _prog_sequence_smart_class_new());
   PROG_SEQUENCE_DATA_GET_OR_RETURN_VAL(obj, sd, NULL);
   sd->parent = parent;
   sd->live = live;
   sd->style = style;

   sd->bg = evas_object_rectangle_add(sd->e);
   evas_object_color_set(sd->bg, BG_COLOR);
   evas_object_smart_member_add(sd->bg, obj);

   sd->labels_bg = evas_object_rectangle_add(sd->e);
   evas_object_color_set(sd->labels_bg, LABELS_BG_COLOR);
   evas_object_smart_member_add(sd->labels_bg, obj);

   sd->ruler = ewe_ruler_add(parent);
   ewe_ruler_step_set(sd->ruler, NULL, PIX_PER_SEC);
   ewe_ruler_value_step_set(sd->ruler, NULL, 1);
   ewe_ruler_scale_middle_mark_set(sd->ruler, NULL, true);
   ewe_ruler_format_set(sd->ruler, NULL, "%.1f");
   evas_object_smart_member_add(sd->ruler, obj);

   return obj;
}

Eina_Bool
prog_sequence_program_set(Evas_Object *obj,
                          const char *program_name,
                          Eina_List *program_list)
{
   Eina_Stringshare *program;
   Evas_Object *label;
   Eina_List *l;
   PROG_SEQUENCE_DATA_GET_OR_RETURN_VAL(obj, sd, false);

   if (sd->program)
     eina_stringshare_del(sd->program);
   sd->program = eina_stringshare_add(program_name);

   sd->program_list = program_list;

   _timeline_free(sd);
   _timeline_init(sd, sd->program);

   /* labels should be added after timeline_init to be higher in draw stack */
   EINA_LIST_FREE(sd->labels, label)
     evas_object_del(label);
   EINA_LIST_FOREACH(sd->program_list, l, program)
     {
        LABEL_ADD(sd->parent, label, program);
        evas_object_smart_member_add(label, sd->obj);
        evas_object_resize(label, LABELS_W, LABELS_H);

        sd->labels = eina_list_append(sd->labels, label);
     }
   evas_object_stack_below(sd->labels_bg, eina_list_data_get(sd->labels));
   evas_object_raise(sd->ruler);

   evas_object_smart_changed(obj);
   return true;
}

