/* Edje Theme Editor
* Copyright (C) 2013 Samsung Electronics.
*
* This file is part of Edje Theme Editor.
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
* along with this program; If not, see www.gnu.org/licenses/gpl-2.0.html.
*/

#include "groupedit_private.h"
#include "alloc.h"

#define PART_STATE_GET(obj, part) \
   const char *state; \
   double value; \
   state = edje_edit_part_selected_state_get(obj, part, &value);

#define PART_STATE_FREE \
   edje_edit_string_free(state);

static void
_groupedit_part_free(Groupedit_Part *gp);

static Groupedit_Part *
_part_draw_add(Ws_Groupedit_Smart_Data *sd, const char *part, Edje_Part_Type type);

static void
_move_border_to_top(Ws_Groupedit_Smart_Data *sd);

static void
_part_draw_del(Ws_Groupedit_Smart_Data *sd, const char *part);

static Evas_Object *
_part_spacer_add(Evas *e);

static Evas_Object *
_part_swallow_add(Evas *e);

static void
_part_object_area_calc(Ws_Groupedit_Smart_Data *sd);

static void
_rectangle_param_update(Groupedit_Part *gp, Evas_Object *edit_obj);

static void
_image_param_update(Groupedit_Part *gp, Evas_Object *edit_obj, const char *file);

static void
_text_param_update(Groupedit_Part *gp, Evas_Object *edit_obj);

static void
_textblock_param_update(Groupedit_Part *gp, Evas_Object *edit_obj);

void
_edit_object_load(Ws_Groupedit_Smart_Data *sd)
{
   Eina_List *parts_list, *l;
   Edje_Part_Type ept;
   const char *state, *name, *text;
   double value;

   parts_list = edje_edit_parts_list_get(sd->edit_obj);
   EINA_LIST_FOREACH(parts_list, l, name)
     {
        ept = edje_edit_part_type_get(sd->edit_obj, name);
        state = edje_edit_part_selected_state_get(sd->edit_obj, name, &value);
        if (ept == EDJE_PART_TYPE_TEXT || ept == EDJE_PART_TYPE_TEXTBLOCK)
          {
             text = edje_edit_state_text_get(sd->edit_obj, name, state, value);
             if (text) edje_object_part_text_set(sd->edit_obj, name, text);
             else edje_object_part_text_set(sd->edit_obj, name, name);
             edje_edit_string_free(text);
          }
        edje_edit_string_free(state);
     }
   edje_edit_string_list_free(parts_list);
}

Eina_Bool
_edit_object_part_add(Ws_Groupedit_Smart_Data *sd, const char *part,
                      Edje_Part_Type type, const char *data)
{
   Groupedit_Part *gp;

   if (!part) return false;
   if (!edje_edit_part_add(sd->edit_obj, part, type))
     {
        ERR("Cann't add part %s to edit object %p", part, sd->edit_obj);
        return false;
     }
   if ((type == EDJE_PART_TYPE_IMAGE) && (data))
     edje_edit_state_image_set(sd->edit_obj, part, "default", 0.0, data);

   gp = _part_draw_add(sd, part, type);
   sd->parts = eina_list_append(sd->parts, gp);
   _move_border_to_top(sd);
   evas_object_smart_changed(sd->obj);

   return true;
}

Eina_Bool
_edit_object_part_del(Ws_Groupedit_Smart_Data *sd, const char *part)
{
   if ((!sd->parts) || (!part)) return false;
   if (!edje_edit_part_del(sd->edit_obj, part)) return false;

   _selected_item_return_to_place(sd);
   _part_draw_del(sd, part);
   evas_object_smart_changed(sd->obj);

   return true;
}

/**
  * Internal function, which provide restack parts at parts list and restack
  * part primitives at canvas.
  * If param part_to is NULL, part, which name stored at param part, will
  * restack below or above at one position. Else part will restack to part which
  * name stored at part_to.
  *
  * @param sd The groupedit smart data structure for get access to parts list.
  * @param part The name of part to stack.
  * @param part_to The name of part which to stack.
  * @param mode If true operation restack above will be process, else is
  * restack below.
  *
  * @return false on failure, true on success.
  */
static Eina_Bool
_part_restack(Ws_Groupedit_Smart_Data *sd,
              const char *part,
              const char *part_to,
              Eina_Bool mode)
{
   Eina_List *l = NULL;
   Groupedit_Part *ge_part_to, *ge_part;

   ge_part = _parts_list_find(sd->parts, part);
   if (!ge_part) return false;
   if (sd->separated) sd->to_select = ge_part;
   /* Here find part_to in parts stack. ge_part will be restack abow or below
    * ge_part_to object. */
   if (part_to)
     ge_part_to = _parts_list_find(sd->parts, part_to);
   else
     {
        l = eina_list_data_find_list(sd->parts, ge_part);
        if (mode)
          ge_part_to = eina_list_data_get(eina_list_prev(l));
        else
          ge_part_to = eina_list_data_get(eina_list_next(l));
     }

   if (!ge_part_to) return false;

   /* For changing position ge_part in Eina_List of parts, at first need to
    * remove node with data ge_part, and for second prepend/append ge_part to
    * list of parts. (Operation prepend use when restack above, and
    * append if restack below)
    */
   sd->parts = eina_list_remove(sd->parts, ge_part);

   if (mode)
     {
        sd->parts = eina_list_prepend_relative(sd->parts, ge_part, ge_part_to);
        if (sd->separated)
          {
             _select_item_move_to_top(sd);
             return true;
          }
        evas_object_stack_below(ge_part->bg, ge_part_to->draw);
        evas_object_stack_below(ge_part->clipper, ge_part_to->draw);
        evas_object_stack_below(ge_part->draw, ge_part_to->draw);
        evas_object_stack_below(ge_part->border, ge_part_to->draw);
        evas_object_stack_below(ge_part->item, ge_part_to->draw);
     }
   else
     {
        sd->parts = eina_list_append_relative(sd->parts, ge_part, ge_part_to);
        if (sd->separated)
          {
             _select_item_move_to_top(sd);
             return true;
          }
        evas_object_stack_above(ge_part->bg, ge_part_to->draw);
        evas_object_stack_above(ge_part->clipper, ge_part_to->draw);
        evas_object_stack_above(ge_part->item, ge_part_to->item);
        evas_object_stack_above(ge_part->border, ge_part_to->item);
        evas_object_stack_above(ge_part->draw, ge_part_to->item);
     }

   return true;
}

Eina_Bool
_edit_object_part_restack_above(Ws_Groupedit_Smart_Data *sd,
                                const char *part,
                                const char *part_above)
{
   Eina_Bool ret = false;
   if ((!sd->parts) || (!part)) return false;
   if (part_above)
     ret = edje_edit_part_restack_part_above(sd->edit_obj, part, part_above);
   else
     ret = edje_edit_part_restack_below(sd->edit_obj, part);

   if (!ret)
     {
        ERR("Failed restack in edje_edit object");
        return false;
     }
   _part_restack(sd, part, part_above, true);
   evas_object_smart_changed(sd->obj);
   return true;
}

Eina_Bool
_edit_object_part_restack_below(Ws_Groupedit_Smart_Data *sd,
                                const char *part,
                                const char *part_below)
{
   Eina_Bool ret = false;
   if ((!sd->parts) || (!part)) return false;
   if (part_below)
     ret = edje_edit_part_restack_part_below(sd->edit_obj, part, part_below);
   else
     ret = edje_edit_part_restack_above(sd->edit_obj, part);

   if (!ret)
     {
        ERR("Failed restack in edje_edit object");
        return false;
     }
   _part_restack(sd, part, part_below, false);
   evas_object_smart_changed(sd->obj);
   return true;
}

void
_parts_list_new(Ws_Groupedit_Smart_Data *sd)
{
   Groupedit_Part *gp;
   Eina_List *parts_list, *l;
   const char *name;
   Edje_Part_Type type;

   parts_list = edje_edit_parts_list_get(sd->edit_obj);

   evas_event_freeze(sd->e);
   EINA_LIST_FOREACH(parts_list, l, name)
     {
        type = edje_edit_part_type_get(sd->edit_obj, name);
        gp = _part_draw_add(sd, name, type);
        sd->parts = eina_list_append(sd->parts, gp);
     }
   evas_event_thaw(sd->e);
   edje_edit_string_list_free(parts_list);
   _move_border_to_top(sd);
}

void
_parts_list_free(Ws_Groupedit_Smart_Data *sd)
{
   Groupedit_Part *gp;
   if (!sd->parts) return;

   EINA_LIST_FREE(sd->parts, gp)
     _groupedit_part_free(gp);
   sd->parts = NULL;
}

static void
_groupedit_part_free(Groupedit_Part *gp)
{
   evas_object_smart_member_del(gp->bg);
   evas_object_del(gp->bg);
   evas_object_smart_member_del(gp->clipper);
   evas_object_del(gp->clipper);
   evas_object_smart_member_del(gp->draw);
   evas_object_del(gp->draw);
   eina_stringshare_del(gp->name);
   evas_object_smart_member_del(gp->border);
   evas_object_del(gp->border);
   evas_object_smart_member_del(gp->item);
   evas_object_del(gp->item);

   //free(gp);
}

Groupedit_Part *
_parts_list_find(Eina_List *parts, const char *part)
{
   Eina_List *l;
   Groupedit_Part *gp;

   if ((!parts) || (!part)) return NULL;
   EINA_LIST_FOREACH(parts, l, gp)
     {
        if ((gp->name == part) || (!strcmp(part, gp->name)))
          return gp;
     }
   return NULL;
}

void
_selected_item_return_to_place(Ws_Groupedit_Smart_Data *sd)
{
   Groupedit_Part *gp;
   Eina_List *l, *ln;

   if (!sd->selected) return;
   l = eina_list_data_find_list(sd->parts, sd->selected);
   ln = eina_list_prev(l);
   if (!ln)
     {
        ln = eina_list_next(l);
        if (!ln) return;
        gp = (Groupedit_Part *)eina_list_data_get(ln);
        evas_object_stack_below(sd->selected->draw, gp->draw);
        evas_object_stack_below(sd->selected->border, gp->draw);
        evas_object_stack_below(sd->selected->item, gp->draw);
        DBG("Separete mode, return to place part %s. Restack below the part %s", sd->selected->name, gp->name);
     }
   else
     {
        gp = (Groupedit_Part *)eina_list_data_get(ln);
        evas_object_stack_above(sd->selected->item, gp->item);
        evas_object_stack_above(sd->selected->border, gp->draw);
        evas_object_stack_above(sd->selected->draw, gp->item);
        DBG("Separete mode, return to place part %s. Restack above the part %s", sd->selected->name, gp->name);
     }
   evas_object_hide(sd->selected->clipper);
   evas_object_clip_unset(sd->selected->bg);
   evas_object_hide(sd->selected->bg);
   evas_object_stack_below(sd->selected->border, sd->selected->draw);
   edje_object_signal_emit(sd->selected->item, "item,unselected", "eflete");

   sd->selected = NULL;
}

static void
_item_bg_add(Ws_Groupedit_Smart_Data *sd)
{
   int x, y, w, h, sw, sh;
   Eina_Bool filled;
   Groupedit_Part *gp;
   void *data; /* raw image data */
   gp = sd->to_select;

   data = evas_object_image_data_get(sd->bg, true);
   evas_object_geometry_get(sd->bg, &x, &y, &w, &h);
   filled = evas_object_image_filled_get(sd->bg);
   evas_object_image_size_get(sd->bg, &sw, &sh);

   evas_object_image_size_set(gp->bg, sw, sh);
   evas_object_image_data_copy_set(gp->bg, data);
   evas_object_image_filled_set(gp->bg, filled);
   evas_object_image_fill_set(gp->bg, 0, 0, sw, sh);
   evas_object_resize(gp->bg, w, h);
   evas_object_move(gp->bg, x, y);
}

void
_select_item_move_to_top(Ws_Groupedit_Smart_Data *sd)
{
   if (sd->selected == sd->to_select) return;
   if (sd->selected) _selected_item_return_to_place(sd);

   _item_bg_add(sd);
   evas_object_show(sd->to_select->bg);
   evas_object_show(sd->to_select->clipper);
   evas_object_clip_set(sd->to_select->bg, sd->to_select->clipper);

   evas_object_raise(sd->to_select->bg);
   evas_object_raise(sd->to_select->draw);
   evas_object_raise(sd->to_select->border);
   evas_object_raise(sd->to_select->item);
   sd->selected = sd->to_select;
   edje_object_signal_emit(sd->selected->item, "item,selected", "eflete");

   sd->to_select = NULL;
}

static void
_part_separete_mod_mouse_click_cb(void *data,
                                  Evas *e __UNUSED__,
                                  Evas_Object *obj __UNUSED__,
                                  void *event_info)
{
   Groupedit_Part *gp = (Groupedit_Part *)data;
   Ws_Groupedit_Smart_Data *sd = evas_object_data_get(gp->item, "sd");
   Evas_Event_Mouse_Down *emd = (Evas_Event_Mouse_Down *)event_info;

   if (emd->button != 1) return;
   sd->to_select = gp;
   _select_item_move_to_top(sd);
   evas_object_smart_callback_call(sd->obj, SIG_PART_SELECTED,
                                   (void *)sd->selected->name);
}

static void
_part_separete_mod_mouse_in_cb(void *data,
                               Evas *e __UNUSED__,
                               Evas_Object *obj __UNUSED__,
                               void *event_info __UNUSED__)
{
   Groupedit_Part *gp = (Groupedit_Part *)data;
   Ws_Groupedit_Smart_Data *sd = evas_object_data_get(gp->item, "sd");
   if (sd->selected == gp) return;
   edje_object_signal_emit(gp->item, "item,mouse,in", "eflete");
}

static void
_part_separete_mod_mouse_out_cb(void *data,
                                Evas *e __UNUSED__,
                                Evas_Object *obj __UNUSED__,
                                void *event_info __UNUSED__)
{
   Groupedit_Part *gp = (Groupedit_Part *)data;
   Ws_Groupedit_Smart_Data *sd = evas_object_data_get(gp->item, "sd");
   if (sd->selected == gp) return;
   edje_object_signal_emit(gp->item, "item,mouse,out", "eflete");
}

#define GP_GEOMETRY_SET \
   evas_object_resize(gp->border, w, h); \
   evas_object_move(gp->border, x + xe + offset_x, y + ye + offset_y); \
   evas_object_resize(gp->item, sd->con_current_size->w, sd->con_current_size->h); \
   evas_object_move(gp->item, xe + offset_x, ye + offset_y); \
   evas_object_resize(gp->clipper, sd->con_current_size->w, sd->con_current_size->h); \
   evas_object_move(gp->clipper, xe + offset_x, ye + offset_y); \

static void
_part_text_recalc_apply(Ws_Groupedit_Smart_Data *sd,
                        Groupedit_Part *gp,
                        int offset_x,
                        int offset_y)
{
   Evas_Coord x, y, xe, ye, ro_x, ro_y;
   int w, h, ro_w, ro_h;
   const Evas_Object *ro;

   edje_object_part_geometry_get(sd->edit_obj, gp->name, &x, &y, &w, &h);
   evas_object_geometry_get(sd->edit_obj, &xe, &ye, NULL, NULL);
   ro = edje_object_part_object_get(sd->edit_obj, gp->name);
   evas_object_geometry_get(ro, &ro_x, &ro_y, &ro_w, &ro_h);

   evas_object_resize(gp->draw, ro_w, ro_h);
   evas_object_move(gp->draw, ro_x + offset_x, ro_y + offset_y);

   GP_GEOMETRY_SET
}

static void
_part_recalc_apply(Ws_Groupedit_Smart_Data *sd,
                   Groupedit_Part *gp,
                   int offset_x,
                   int offset_y)
{
   Evas_Coord x, y, xe, ye;
   int w, h;

   edje_object_part_geometry_get(sd->edit_obj, gp->name, &x, &y, &w, &h);
   evas_object_geometry_get(sd->edit_obj, &xe, &ye, NULL, NULL);

   evas_object_resize(gp->draw, w, h);
   evas_object_move(gp->draw, x + xe + offset_x, y + ye + offset_y);

   GP_GEOMETRY_SET
}

#undef GP_GEOMETRY_SET

void
_parts_recalc(Ws_Groupedit_Smart_Data *sd)
{
   Eina_List *l;
   Groupedit_Part *gp;
   Edje_Part_Type ept;
   int i = 0, offset_x, offset_y;

   if (!sd->parts) return;

   EINA_LIST_FOREACH(sd->parts, l, gp)
     {
        if (sd->separated)
          {
             i++;
             evas_object_show(gp->item);
          }
        else evas_object_hide(gp->item);

        offset_x = i * SEP_ITEM_PAD_X;
        offset_y = i * SEP_ITEM_PAD_Y;

        ept = edje_edit_part_type_get(sd->edit_obj, gp->name);
        switch (ept)
          {
           case EDJE_PART_TYPE_RECTANGLE:
              _rectangle_param_update(gp, sd->edit_obj);
              _part_recalc_apply(sd, gp, offset_x, offset_y);
              break;

           case EDJE_PART_TYPE_TEXT:
              _text_param_update(gp, sd->edit_obj);
              _part_text_recalc_apply(sd, gp, offset_x, offset_y);
              break;

           case EDJE_PART_TYPE_IMAGE:
           case EDJE_PART_TYPE_PROXY: // it part like image
              _image_param_update(gp, sd->edit_obj, sd->edit_obj_file);
              _part_recalc_apply(sd, gp, offset_x, offset_y);
              break;

           case EDJE_PART_TYPE_TEXTBLOCK:
              _textblock_param_update(gp, sd->edit_obj);
              _part_recalc_apply(sd, gp, offset_x, offset_y);
              break;

           case EDJE_PART_TYPE_SPACER:
           case EDJE_PART_TYPE_SWALLOW:
           case EDJE_PART_TYPE_GROUP:
           case EDJE_PART_TYPE_BOX:
           case EDJE_PART_TYPE_TABLE:
           case EDJE_PART_TYPE_EXTERNAL:
              _part_recalc_apply(sd, gp, offset_x, offset_y);
           default:
              break;
          }
        if (gp->visible)
          {
             evas_object_show(gp->draw);
             evas_object_show(gp->border);
          }
        else
          {
             evas_object_hide(gp->draw);
             evas_object_hide(gp->border);
          }
     }
     _part_object_area_calc(sd);
}

#define BORDER_ADD(R, G, B, A) \
   gp->border = evas_object_image_add(sd->e); \
   evas_object_image_file_set(gp->border, BORDER_IMG, NULL); \
   evas_object_color_set(gp->border, R*A/255, G*A/255, B*A/255, A); \
   evas_object_image_border_set(gp->border, 1, 1, 1, 1); \
   evas_object_image_filled_set(gp->border, true); \
   evas_object_image_border_center_fill_set(gp->border, EVAS_BORDER_FILL_NONE);

static Groupedit_Part *
_part_draw_add(Ws_Groupedit_Smart_Data *sd, const char *part, Edje_Part_Type type)
{
   Groupedit_Part *gp;

   gp = mem_malloc(sizeof(Groupedit_Part));
   gp->name = eina_stringshare_add(part);
   gp->visible = true;
   gp->border = NULL;

   gp->bg = evas_object_image_add(sd->e);

   gp->clipper = evas_object_rectangle_add(sd->e);
   switch (type)
     {
      case EDJE_PART_TYPE_RECTANGLE:
         gp->draw = evas_object_rectangle_add(sd->e);
         BORDER_ADD(0, 0, 0, 0)
         break;
      case EDJE_PART_TYPE_TEXT:
         gp->draw = evas_object_text_add(sd->e);
         BORDER_ADD(122, 122, 122, 255)
         break;
      case EDJE_PART_TYPE_IMAGE:
      case EDJE_PART_TYPE_PROXY: // it part like image
         gp->draw = evas_object_image_add(sd->e);
         BORDER_ADD(0, 0, 0, 0)
         break;
      case EDJE_PART_TYPE_SWALLOW:
         gp->draw = _part_swallow_add(sd->e);
         BORDER_ADD(150, 143, 158, 255)
         break;
      case EDJE_PART_TYPE_TEXTBLOCK:
         gp->draw = evas_object_textblock_add(sd->e);
         BORDER_ADD(122, 122, 122, 255)
         break;
      case EDJE_PART_TYPE_SPACER:
         gp->draw = _part_spacer_add(sd->e);
         BORDER_ADD(101, 117, 133, 255)
         break;
      case EDJE_PART_TYPE_GROUP:
      case EDJE_PART_TYPE_BOX:
      case EDJE_PART_TYPE_TABLE:
      case EDJE_PART_TYPE_EXTERNAL:
      default:
         /* Temporary solution for type parts, which not implemented yet.
          * Here created transparent rectangle as draw evas primitives.
          * TODO: add support for all part types.
          */
         gp->draw = evas_object_rectangle_add(sd->e);
         evas_object_color_set(gp->draw, 0, 0, 0, 0);
         break;
     }
   gp->item = edje_object_add(sd->e);
   edje_object_file_set(gp->item, EFLETE_EDJ, "eflete/group/item/default");
   evas_object_data_set(gp->item, "sd", sd);
   evas_object_event_callback_add(gp->item, EVAS_CALLBACK_MOUSE_DOWN,
                                  _part_separete_mod_mouse_click_cb, gp);
   evas_object_event_callback_add(gp->item, EVAS_CALLBACK_MOUSE_IN,
                                  _part_separete_mod_mouse_in_cb, gp);
   evas_object_event_callback_add(gp->item, EVAS_CALLBACK_MOUSE_OUT,
                                  _part_separete_mod_mouse_out_cb, gp);


   evas_object_smart_member_add(gp->bg, sd->obj);
   evas_object_smart_member_add(gp->clipper, sd->obj);
   evas_object_smart_member_add(gp->draw, sd->obj);
   evas_object_smart_member_add(gp->border, sd->obj);
   evas_object_smart_member_add(gp->item, sd->obj);
   return gp;
}

#undef BORDER_ADD

static void
_part_draw_del(Ws_Groupedit_Smart_Data *sd, const char *part)
{
   Groupedit_Part *gp;

   gp = _parts_list_find(sd->parts, part);
   _groupedit_part_free(gp);
   sd->parts = eina_list_remove(sd->parts, gp);
}

static void
_move_border_to_top(Ws_Groupedit_Smart_Data *sd)
{
   evas_object_smart_member_del(sd->container);
   evas_object_smart_member_del(sd->handler_TL.obj);
   evas_object_smart_member_del(sd->handler_BR.obj);

   evas_object_smart_member_add(sd->container, sd->obj);
   evas_object_smart_member_add(sd->handler_TL.obj, sd->obj);
   evas_object_smart_member_add(sd->handler_BR.obj, sd->obj);
}

static Evas_Object *
_part_spacer_add(Evas *e)
{
   Evas_Object *spacer;

   spacer = evas_object_image_add(e);
   evas_object_image_file_set(spacer, SPACER_IMG, NULL);
   evas_object_image_fill_set(spacer, 0, 0, 8, 8);
   evas_object_image_filled_set(spacer, false);

   return spacer;
}

static Evas_Object *
_part_swallow_add(Evas *e)
{
   Evas_Object *swallow;

   swallow = evas_object_image_filled_add(e);
   evas_object_image_file_set(swallow, SWALLOW_IMG, NULL);
   evas_object_image_filled_set(swallow, false);
   evas_object_image_fill_set(swallow, 0, 0, 17, 17);

   return swallow;
}

static void
_rectangle_param_update(Groupedit_Part *gp, Evas_Object *edit_obj)
{
   int r, g, b, a;
   PART_STATE_GET(edit_obj, gp->name)

   edje_edit_state_color_get(edit_obj, gp->name, state, value, &r, &g, &b, &a);
   evas_object_color_set(gp->draw, r*a/255, g*a/255, b*a/255, a);

   PART_STATE_FREE
}

static void
_image_param_update(Groupedit_Part *gp, Evas_Object *edit_obj, const char *file)
{
   Evas_Load_Error err;
   const char *image_normal;
   char buf[512]; /* FIXME: change 512 to BUFF_MAX */
   int r, g, b, a;
   int id;
   int bl, br, bt, bb;
   PART_STATE_GET(edit_obj, gp->name)

   state = edje_edit_part_selected_state_get(edit_obj, gp->name, &value);
   edje_edit_state_color_get(edit_obj, gp->name, state, value, &r, &g, &b, &a);
   evas_object_color_set(gp->draw, r*a/255, g*a/255, b*a/255, a);

   image_normal = edje_edit_state_image_get(edit_obj, gp->name, state, value);
   if (!image_normal) return;
   id = edje_edit_image_id_get(edit_obj, image_normal);
   snprintf(buf, sizeof(buf), "edje/images/%i", id);
   evas_object_image_file_set(gp->draw, file, buf);
   err = evas_object_image_load_error_get(gp->draw);
   if (err != EVAS_LOAD_ERROR_NONE)
     fprintf(stdout, "Could not update image:\"%s\"\n",  evas_load_error_str(err));

   edje_edit_state_image_border_get(edit_obj, gp->name, state, value,
                                    &bl, &br, &bt, &bb);
   evas_object_image_border_set(gp->draw, bl, br, bt, bb);

   /* FIXME: need see in edje, to know how to work with edje fill param */
   /* evas_object_image_fill_set(gp->draw, 0, 0, 1, 1); */
   evas_object_image_filled_set(gp->draw, true);

   evas_object_image_smooth_scale_set(gp->draw, true);

   /* evas_object_image_data_update_add() */

   PART_STATE_FREE
   edje_edit_string_free(image_normal);
}

static void
_text_param_update(Groupedit_Part *gp, Evas_Object *edit_obj)
{
   const char *font, *text;
   int text_size;
   int r, g, b, a; // main color
   int sr, sg, sb, sa; // shadow
   int or, og, ob, oa; // outline
   double elipsis;
   Evas_Text_Style_Type style;
   Edje_Text_Effect effect;
   PART_STATE_GET(edit_obj, gp->name)

   state = edje_edit_part_selected_state_get(edit_obj, gp->name, &value);

   font = edje_edit_state_font_get(edit_obj, gp->name, state, value);
   text_size = edje_edit_state_text_size_get(edit_obj, gp->name, state, value);
   evas_object_text_font_set(gp->draw, font, text_size);

   text = edje_edit_state_text_get(edit_obj, gp->name, state, value);
   if (text) evas_object_text_text_set(gp->draw, text);
   else
     {
         edje_object_part_text_set(edit_obj, gp->name, gp->name);
         evas_object_text_text_set(gp->draw, gp->name);
     }

   elipsis = edje_edit_state_text_elipsis_get(edit_obj, gp->name, state, value);
   evas_object_text_ellipsis_set(gp->draw, elipsis);

   edje_edit_state_color_get(edit_obj, gp->name, state, value, &r, &g, &b, &a);
   evas_object_color_set(gp->draw, r*a/255, g*a/255, b*a/255, a);

   edje_edit_state_color2_get(edit_obj, gp->name, state, value, &sr, &sg, &sb, &sa);
   evas_object_text_shadow_color_set(gp->draw, sr, sg, sb, sa);

   edje_edit_state_color3_get(edit_obj, gp->name, state, value, &or, &og, &ob, &oa);
   evas_object_text_outline_color_set(gp->draw, or, og, ob, oa);

   style = EVAS_TEXT_STYLE_PLAIN;
   effect = edje_edit_part_effect_get(edit_obj, gp->name);
   switch (effect & EDJE_TEXT_EFFECT_MASK_BASIC)
     {
      case EDJE_TEXT_EFFECT_NONE:
      case EDJE_TEXT_EFFECT_PLAIN:
         style = EVAS_TEXT_STYLE_PLAIN;
         break;
      case EDJE_TEXT_EFFECT_OUTLINE:
         style = EVAS_TEXT_STYLE_OUTLINE;
         break;
      case EDJE_TEXT_EFFECT_SOFT_OUTLINE:
         style = EVAS_TEXT_STYLE_SOFT_OUTLINE;
         break;
      case EDJE_TEXT_EFFECT_SHADOW:
         style = EVAS_TEXT_STYLE_SHADOW;
         break;
      case EDJE_TEXT_EFFECT_SOFT_SHADOW:
         style = EVAS_TEXT_STYLE_SOFT_SHADOW;
         break;
      case EDJE_TEXT_EFFECT_OUTLINE_SHADOW:
         style = EVAS_TEXT_STYLE_OUTLINE_SHADOW;
         break;
      case EDJE_TEXT_EFFECT_OUTLINE_SOFT_SHADOW:
         style = EVAS_TEXT_STYLE_OUTLINE_SOFT_SHADOW;
         break;
      case EDJE_TEXT_EFFECT_FAR_SHADOW:
         style = EVAS_TEXT_STYLE_FAR_SHADOW;
         break;
      case EDJE_TEXT_EFFECT_FAR_SOFT_SHADOW:
         style = EVAS_TEXT_STYLE_FAR_SOFT_SHADOW;
         break;
      case EDJE_TEXT_EFFECT_GLOW:
         style = EVAS_TEXT_STYLE_GLOW;
         break;
      default:
         style = EVAS_TEXT_STYLE_PLAIN;
         break;
     }

   switch (effect & EDJE_TEXT_EFFECT_MASK_SHADOW_DIRECTION)
     {
      case EDJE_TEXT_EFFECT_SHADOW_DIRECTION_BOTTOM_RIGHT:
         EVAS_TEXT_STYLE_SHADOW_DIRECTION_SET
            (style, EVAS_TEXT_STYLE_SHADOW_DIRECTION_BOTTOM_RIGHT);
         break;
      case EDJE_TEXT_EFFECT_SHADOW_DIRECTION_BOTTOM:
         EVAS_TEXT_STYLE_SHADOW_DIRECTION_SET
            (style, EVAS_TEXT_STYLE_SHADOW_DIRECTION_BOTTOM);
         break;
      case EDJE_TEXT_EFFECT_SHADOW_DIRECTION_BOTTOM_LEFT:
         EVAS_TEXT_STYLE_SHADOW_DIRECTION_SET
            (style, EVAS_TEXT_STYLE_SHADOW_DIRECTION_BOTTOM_LEFT);
         break;
      case EDJE_TEXT_EFFECT_SHADOW_DIRECTION_LEFT:
         EVAS_TEXT_STYLE_SHADOW_DIRECTION_SET
            (style, EVAS_TEXT_STYLE_SHADOW_DIRECTION_LEFT);
         break;
      case EDJE_TEXT_EFFECT_SHADOW_DIRECTION_TOP_LEFT:
         EVAS_TEXT_STYLE_SHADOW_DIRECTION_SET
            (style, EVAS_TEXT_STYLE_SHADOW_DIRECTION_TOP_LEFT);
         break;
      case EDJE_TEXT_EFFECT_SHADOW_DIRECTION_TOP:
         EVAS_TEXT_STYLE_SHADOW_DIRECTION_SET
            (style, EVAS_TEXT_STYLE_SHADOW_DIRECTION_TOP);
         break;
      case EDJE_TEXT_EFFECT_SHADOW_DIRECTION_TOP_RIGHT:
         EVAS_TEXT_STYLE_SHADOW_DIRECTION_SET
            (style, EVAS_TEXT_STYLE_SHADOW_DIRECTION_TOP_RIGHT);
         break;
      case EDJE_TEXT_EFFECT_SHADOW_DIRECTION_RIGHT:
         EVAS_TEXT_STYLE_SHADOW_DIRECTION_SET
            (style, EVAS_TEXT_STYLE_SHADOW_DIRECTION_RIGHT);
         break;
      default:
         break;
     }
   evas_object_text_style_set(gp->draw, style);

   PART_STATE_FREE
   edje_edit_string_free(font);
   edje_edit_string_free(text);
}

static void
_textblock_param_update(Groupedit_Part *gp, Evas_Object *edit_obj)
{
   Evas_Textblock_Style *ts = NULL;
   const Evas_Textblock_Style *obj_style;
   const Evas_Object *get_style;
   const char *text;
   double valign;
   PART_STATE_GET(edit_obj, gp->name)

   state = edje_edit_part_selected_state_get(edit_obj, gp->name, &value);

   get_style = edje_object_part_object_get(edit_obj, gp->name);
   obj_style = evas_object_textblock_style_get(get_style);
   text = evas_textblock_style_get(obj_style);

   ts = evas_textblock_style_new();
   evas_textblock_style_set(ts, text);
   evas_object_textblock_style_set(gp->draw, ts);
   valign = edje_edit_state_text_align_y_get(edit_obj, gp->name, state, value);
   evas_object_textblock_valign_set(gp->draw, valign);

   text = edje_edit_state_text_get(edit_obj, gp->name, state, value);
   if (text) evas_object_textblock_text_markup_set(gp->draw, text);
   else
     {
        edje_object_part_text_set(edit_obj, gp->name, gp->name);
        evas_object_textblock_text_markup_set(gp->draw, gp->name);
     }

   PART_STATE_FREE
   edje_edit_string_free(text);
}

static void
_part_object_area_calc(Ws_Groupedit_Smart_Data *sd)
{
   Eina_Stringshare *rel_to;
   int xc, yc, wc, hc;
   int xg, yg, wg, hg; // groupedit geometry
   int x, w, y, h;
   double relative;

   if (!sd->obj_area.gp)
     {
        x = sd->con_current_size->x; y = sd->con_current_size->y;
        w = sd->con_current_size->w; h = sd->con_current_size->h;
     }
   else
     {
        PART_STATE_GET(sd->edit_obj, sd->obj_area.gp->name)
        const char *name = sd->obj_area.gp->name;

        evas_object_geometry_get(sd->edit_obj, &xg, &yg, &wg, &hg);

        xc = wc = 0;
        rel_to = edje_edit_state_rel1_to_x_get(sd->edit_obj, name, state, value);
        relative = edje_edit_state_rel1_relative_x_get(sd->edit_obj, name, state, value);
        if (rel_to) edje_object_part_geometry_get(sd->edit_obj, rel_to, &xc, NULL, &wc, NULL);
        x = xg + (xc + (int)(wc * relative));
        edje_edit_string_free(rel_to);

        yc = hc = 0;
        rel_to = edje_edit_state_rel1_to_y_get(sd->edit_obj, name, state, value);
        relative = edje_edit_state_rel1_relative_x_get(sd->edit_obj, name, state, value);
        if (rel_to) edje_object_part_geometry_get(sd->edit_obj, rel_to, NULL, &yc, NULL, &hc);
        y = yg + (yc + (int)(hc * relative));
        edje_edit_string_free(rel_to);

        xc = 0; wc = sd->con_current_size->w;
        rel_to = edje_edit_state_rel2_to_x_get(sd->edit_obj, name, state, value);
        relative = edje_edit_state_rel2_relative_x_get(sd->edit_obj, name, state, value);
        if (rel_to) edje_object_part_geometry_get(sd->edit_obj, rel_to, &xc, NULL, &wc, NULL);
        w = xg + (xc + (int)(wc * relative)) - x;
        edje_edit_string_free(rel_to);

        yc = 0; hc = sd->con_current_size->h;
        rel_to = edje_edit_state_rel2_to_y_get(sd->edit_obj, name, state, value);
        relative = edje_edit_state_rel2_relative_y_get(sd->edit_obj, name, state, value);
        if (rel_to) edje_object_part_geometry_get(sd->edit_obj, rel_to, NULL, &yc, NULL, &hc);
        h = yg + (yc + (int)(hc * relative)) - y;
        edje_edit_string_free(rel_to);

        evas_object_geometry_get(sd->obj, &xc, &yc, NULL, NULL);

        evas_object_resize(sd->obj_area.obj, w, h);
        evas_object_move(sd->obj_area.obj, x, y);

        PART_STATE_FREE

        x -= xc; y -= yc;
     }

   sd->obj_area.geom->x = x; sd->obj_area.geom->y = y;
   sd->obj_area.geom->w = w; sd->obj_area.geom->h = h;

   evas_object_smart_callback_call(sd->obj, SIG_OBJ_AREA_CHANGED, sd->obj_area.geom);
}
