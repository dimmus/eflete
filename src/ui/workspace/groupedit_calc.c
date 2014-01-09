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

#define PART_STATE_GET(obj, part) \
   const char *state; \
   double value; \
   state = edje_edit_part_selected_state_get(obj, part, &value);

#define PART_STATE_FREE \
   edje_edit_string_free(state);

static void
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
_rectangle_param_update(Groupspace_Part *gp, Evas_Object *edit_obj);

static void
_image_param_update(Groupspace_Part *gp, Evas_Object *edit_obj, const char *file);

static void
_text_param_update(Groupspace_Part *gp, Evas_Object *edit_obj);

static void
_textblock_param_update(Groupspace_Part *gp, Evas_Object *edit_obj);

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
   if ((!sd->parts) && (!part)) return false;
   if (!edje_edit_part_add(sd->edit_obj, part, type))
     {
        ERR("Cann't add part %s to edit object %p", part, sd->edit_obj);
        return false;
     }
   if ((type == EDJE_PART_TYPE_IMAGE) && (data))
     edje_edit_state_image_set(sd->edit_obj, part, "default", 0.0, data);

   _part_draw_add(sd, part, type);
   _move_border_to_top(sd);
   evas_object_smart_changed(sd->obj);

   return true;
}

Eina_Bool
_edit_object_part_del(Ws_Groupedit_Smart_Data *sd, const char *part)
{
   if ((!sd->parts) && (!part)) return false;
   if (!edje_edit_part_del(sd->edit_obj, part)) return false;

   _part_draw_del(sd, part);
   //_parts_recalc(sd);
   evas_object_smart_changed(sd->obj);

   return true;
}

static unsigned int
_edit_part_string_key_length(const char *key)
{
   if (!key) return 0;
   return (int)strlen(key) + 1;
}

static int
_edit_part_string_cmp(const char *key1, int key1_length __UNUSED__,
                      const char *key2, int key2_length __UNUSED__)
{
   return strcmp(key1, key2);
}

static void
_edit_part_free_cb(void *data)
{
   Groupspace_Part *gp = (Groupspace_Part *)data;
   evas_object_smart_member_del(gp->draw);
   evas_object_del(gp->draw);
   eina_stringshare_del(gp->name);
   free(gp);
}

void
_parts_hash_new(Ws_Groupedit_Smart_Data *sd)
{
   Eina_List *parts_list, *l;
   const char *name;
   Edje_Part_Type type;

   sd->parts = eina_hash_new(EINA_KEY_LENGTH(_edit_part_string_key_length),
                             EINA_KEY_CMP(_edit_part_string_cmp),
                             EINA_KEY_HASH(eina_hash_superfast),
                             _edit_part_free_cb,
                             10); // use 1024 (2 ^ 10) buckets, I hope it will be enough

   parts_list = edje_edit_parts_list_get(sd->edit_obj);

   evas_event_freeze(sd->e);
   EINA_LIST_FOREACH(parts_list, l, name)
     {
        type = edje_edit_part_type_get(sd->edit_obj, name);
        _part_draw_add(sd, name, type);
     }
   evas_event_thaw(sd->e);
   edje_edit_string_list_free(parts_list);
   _move_border_to_top(sd);
}

void
_parts_hash_free(Ws_Groupedit_Smart_Data *sd)
{
   if (!sd->parts) return;

   eina_hash_free(sd->parts);
   sd->parts = NULL;
}

void
_parts_recalc(Ws_Groupedit_Smart_Data *sd)
{
   int x, y, w, h, xe, ye;
   Eina_Iterator *it;
   void *data;
   Groupspace_Part *gp;
   Edje_Part_Type ept;

   if (!sd->parts) return;

   it = eina_hash_iterator_data_new(sd->parts);
   while (eina_iterator_next(it, &data))
     {
        gp = (Groupspace_Part *)data;
        ept = edje_edit_part_type_get(sd->edit_obj, gp->name);
        switch (ept)
          {
           case EDJE_PART_TYPE_RECTANGLE:
              _rectangle_param_update(gp, sd->edit_obj);
              break;
           case EDJE_PART_TYPE_TEXT:
              _text_param_update(gp, sd->edit_obj);
              break;
           case EDJE_PART_TYPE_IMAGE:
           case EDJE_PART_TYPE_PROXY: // it part like image
              _image_param_update(gp, sd->edit_obj, sd->edit_obj_file);
              break;
           case EDJE_PART_TYPE_TEXTBLOCK:
              _textblock_param_update(gp, sd->edit_obj);
              break;
           case EDJE_PART_TYPE_SPACER:
           case EDJE_PART_TYPE_SWALLOW:
           case EDJE_PART_TYPE_GROUP:
           case EDJE_PART_TYPE_BOX:
           case EDJE_PART_TYPE_TABLE:
           case EDJE_PART_TYPE_EXTERNAL:
           default:
              break;
          }

        edje_object_part_geometry_get(sd->edit_obj, gp->name, &x, &y, &w, &h);
        evas_object_geometry_get(sd->edit_obj, &xe, &ye, NULL, NULL);
        evas_object_resize(gp->draw, w, h);
        evas_object_move(gp->draw, x + xe, y + ye);
        if (gp->visible) evas_object_show(gp->draw);
        else evas_object_hide(gp->draw);

        if ((sd->obj_area.gp) && (!strcmp(sd->obj_area.gp->name, gp->name)))
          {
             if (!sd->obj_area.obj)
               {
                  sd->obj_area.obj = evas_object_rectangle_add(sd->e);
                  evas_object_color_set(sd->obj_area.obj,
                                        255*50/255 , 0*50/255, 0*50/255, 50);
                  sd->obj_area.geom = (Groupedit_Geom *)malloc(sizeof(Groupedit_Geom));
                  if (sd->obj_area.visible) evas_object_show(sd->obj_area.obj);
                  else evas_object_hide(sd->obj_area.obj);
               }
             _part_object_area_calc(sd);
          }
     }
   eina_iterator_free(it);
}

void
_part_draw_add(Ws_Groupedit_Smart_Data *sd, const char *part, Edje_Part_Type type)
{
   Groupspace_Part *gp;

   gp = malloc(sizeof(Groupspace_Part));
   gp->name = eina_stringshare_add(part);
   gp->visible = true;

   switch (type)
     {
      case EDJE_PART_TYPE_RECTANGLE:
         gp->draw = evas_object_rectangle_add(sd->e);
         break;
      case EDJE_PART_TYPE_TEXT:
         gp->draw = evas_object_text_add(sd->e);
         break;
      case EDJE_PART_TYPE_IMAGE:
      case EDJE_PART_TYPE_PROXY: // it part like image
         gp->draw = evas_object_image_add(sd->e);
         break;
      case EDJE_PART_TYPE_SWALLOW:
         gp->draw = _part_swallow_add(sd->e);
         break;
      case EDJE_PART_TYPE_TEXTBLOCK:
         gp->draw = evas_object_textblock_add(sd->e);
         break;
      case EDJE_PART_TYPE_SPACER:
         gp->draw = _part_spacer_add(sd->e);
         break;
      case EDJE_PART_TYPE_GROUP:
      case EDJE_PART_TYPE_BOX:
      case EDJE_PART_TYPE_TABLE:
      case EDJE_PART_TYPE_EXTERNAL:
      default:
         break;
     }
   eina_hash_add(sd->parts, part, gp);
   evas_object_smart_member_add(gp->draw, sd->obj);
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

static void
_part_draw_del(Ws_Groupedit_Smart_Data *sd, const char *part)
{
   Groupspace_Part *gp;

   gp = (Groupspace_Part *)eina_hash_find(sd->parts, part);
   eina_hash_del(sd->parts, part, gp);
}

static Evas_Object *
_part_spacer_add(Evas *e)
{
   Evas_Object *spacer;

   spacer = evas_object_image_add(e);
   evas_object_image_file_set(spacer, TET_IMG_PATH"part_spacer_bg.png", NULL);
   evas_object_image_fill_set(spacer, 0, 0, 8, 8);
   evas_object_image_filled_set(spacer, EINA_FALSE);

   return spacer;
}

static Evas_Object *
_part_swallow_add(Evas *e)
{
   Evas_Object *swallow;

   swallow = evas_object_image_filled_add(e);
   evas_object_image_file_set(swallow, TET_IMG_PATH"part_swallow_bg.png", NULL);
   evas_object_image_filled_set(swallow, false);
   evas_object_image_fill_set(swallow, 0, 0, 17, 17);

   return swallow;
}

static void
_rectangle_param_update(Groupspace_Part *gp, Evas_Object *edit_obj)
{
   int r, g, b, a;
   PART_STATE_GET(edit_obj, gp->name)

   edje_edit_state_color_get(edit_obj, gp->name, state, value, &r, &g, &b, &a);
   evas_object_color_set(gp->draw, r*a/255, g*a/255, b*a/255, a);

   PART_STATE_FREE
}

static void
_image_param_update(Groupspace_Part *gp, Evas_Object *edit_obj, const char *file)
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
   //evas_object_image_fill_set(gp->draw, 0, 0, 1, 1);
   evas_object_image_filled_set(gp->draw, true);

   evas_object_image_smooth_scale_set(gp->draw, EINA_TRUE);

   //evas_object_image_data_update_add()

   PART_STATE_FREE
   edje_edit_string_free(image_normal);
}

static void
_text_param_update(Groupspace_Part *gp, Evas_Object *edit_obj)
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
_textblock_param_update(Groupspace_Part *gp, Evas_Object *edit_obj)
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

   xc = wc = 0;
   rel_to = edje_edit_state_rel2_to_x_get(sd->edit_obj, name, state, value);
   relative = edje_edit_state_rel2_relative_x_get(sd->edit_obj, name, state, value);
   if (rel_to) edje_object_part_geometry_get(sd->edit_obj, rel_to, &xc, NULL, &wc, NULL);
   w = xg + (xc + (int)(wc * relative)) - x;
   edje_edit_string_free(rel_to);

   yc = hc = 0;
   rel_to = edje_edit_state_rel2_to_y_get(sd->edit_obj, name, state, value);
   relative = edje_edit_state_rel2_relative_y_get(sd->edit_obj, name, state, value);
   if (rel_to) edje_object_part_geometry_get(sd->edit_obj, rel_to, NULL, &yc, NULL, &hc);
   h = yg + (yc + (int)(hc * relative)) - y;
   edje_edit_string_free(rel_to);

   sd->obj_area.geom->x = x; sd->obj_area.geom->y = y;
   sd->obj_area.geom->w = w; sd->obj_area.geom->h = h;

   evas_object_resize(sd->obj_area.obj, w, h);
   evas_object_move(sd->obj_area.obj, x, y);

   evas_object_smart_callback_call(sd->obj, SIG_OBJ_AREA_CHANGED, sd->obj_area.geom);

   PART_STATE_FREE
}
