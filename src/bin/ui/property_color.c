/*
 * Edje Theme Editor
 * Copyright (C) 2013-2015 Samsung Electronics.
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

#include "property_private.h"
#include "main_window.h"
#include "colorsel.h"
#include "property_macros.h"
#include "project_manager.h"

typedef struct _Color_Prop_Data Color_Prop_Data;
struct _Color_Prop_Data
{
   Evas_Object *description;
   Evas_Object *box_object_color;
   Evas_Object *box_outline_color;
   Evas_Object *box_shadow_color;
   Evas_Object *colorsel1;
   Evas_Object *colorsel2;
   Evas_Object *colorsel3;
   Evas_Object *colorsel1_layout;
   Evas_Object *colorsel2_layout;
   Evas_Object *colorsel3_layout;
   ColorClassData color_data;
};

static void
_colorclass_update(Color_Prop_Data *pd)
{
   assert(pd != NULL);

   Colorclass_Item *cc_it = pd->color_data.current_ccl;

   edje_object_color_class_set(pd->color_data.edje_preview,
                               "colorclass_manager/text_example_colorclass",
                               cc_it->r1, cc_it->g1,
                               cc_it->b1, cc_it->a1,
                               cc_it->r2, cc_it->g2,
                               cc_it->b2, cc_it->a2,
                               cc_it->r3, cc_it->g3,
                               cc_it->b3, cc_it->a3);
   edje_edit_color_class_colors_set(ap.project->global_object, cc_it->name,
                                    cc_it->r1, cc_it->g1,
                                    cc_it->b1, cc_it->a1,
                                    cc_it->r2, cc_it->g2,
                                    cc_it->b2, cc_it->a2,
                                    cc_it->r3, cc_it->g3,
                                    cc_it->b3, cc_it->a3);

   editor_save(ap.project->global_object);
   TODO("Remove this line once edje_edit_colorclass API would be added into Editor Module and saving would work properly")
   ap.project->changed = true;
}

/* Colorselector widget callbacks */
#define COLORSELECTOR_CALLBACK(NUMBER) \
static void \
_on_changed_##NUMBER(void *data, \
                     Evas_Object *obj __UNUSED__, \
                     void *event_info __UNUSED__) \
{ \
   Color_Prop_Data *pd = (Color_Prop_Data *)data; \
   assert(pd != NULL); \
   elm_colorselector_color_get(pd->colorsel##NUMBER, \
                               &pd->color_data.current_ccl->r##NUMBER, \
                               &pd->color_data.current_ccl->g##NUMBER, \
                               &pd->color_data.current_ccl->b##NUMBER, \
                               &pd->color_data.current_ccl->a##NUMBER); \
   _colorclass_update(pd); \
}

COLORSELECTOR_CALLBACK(1)
COLORSELECTOR_CALLBACK(2)
COLORSELECTOR_CALLBACK(3)

#undef COLORSELECTOR_CALLBACK

static Evas_Object *
_colorselector_item_add(Evas_Object *box, Evas_Object *colorsel)
{
   PROPERTY_ITEM_ADD(box, "", "colorselector_item");
   elm_layout_content_set(item, "elm.swallow.content", colorsel);
   return item;
}

   /* Creating colorselectors */
#define ADD_COLORSEL(NUMBER, BOX) \
   pd->colorsel##NUMBER = elm_colorselector_add(BOX); \
   elm_colorselector_mode_set(pd->colorsel##NUMBER, ELM_COLORSELECTOR_ALL); \
   evas_object_size_hint_weight_set(pd->colorsel##NUMBER, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_size_hint_align_set(pd->colorsel##NUMBER, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   evas_object_smart_callback_add(pd->colorsel##NUMBER, "changed,user", _on_changed_##NUMBER, pd); \
   evas_object_smart_callback_add(pd->colorsel##NUMBER, "color,item,selected", _on_changed_##NUMBER, pd); \
   evas_object_show(pd->colorsel##NUMBER); \
   pd->colorsel##NUMBER##_layout = _colorselector_item_add(BOX, pd->colorsel##NUMBER); \
   evas_object_hide(pd->colorsel##NUMBER##_layout);

static Evas_Object *
_add_object_color_frame(Color_Prop_Data *pd, Evas_Object *parent)
{
   Evas_Object *frame_object;

   FRAME_ADD(parent, frame_object, true, _("Object color"))

   BOX_ADD(frame_object, pd->box_object_color, EINA_FALSE, EINA_FALSE);
   elm_box_padding_set(pd->box_object_color, 6, 0);
   elm_object_content_set(frame_object, pd->box_object_color);

   ADD_COLORSEL(1, pd->box_object_color);

   return frame_object;
}

static Evas_Object *
_add_outline_color_frame(Color_Prop_Data *pd, Evas_Object *parent)
{
   Evas_Object *frame_outline;

   FRAME_ADD(parent, frame_outline, true, _("Outline color"))

   BOX_ADD(frame_outline, pd->box_outline_color, EINA_FALSE, EINA_FALSE);
   elm_box_padding_set(pd->box_outline_color, 6, 0);
   elm_object_content_set(frame_outline, pd->box_outline_color);

   ADD_COLORSEL(2, pd->box_outline_color);

   return frame_outline;
}

static Evas_Object *
_add_shadow_color_frame(Color_Prop_Data *pd, Evas_Object *parent)
{
   Evas_Object *frame_shadow;

   FRAME_ADD(parent, frame_shadow, true, _("Shadow color"))

   BOX_ADD(frame_shadow, pd->box_shadow_color, EINA_FALSE, EINA_FALSE);
   elm_box_padding_set(pd->box_shadow_color, 6, 0);
   elm_object_content_set(frame_shadow, pd->box_shadow_color);

   ADD_COLORSEL(3, pd->box_shadow_color);

   return frame_shadow;
}

static void
_on_color_selected(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info)
{
   ColorClassData *current_color = (ColorClassData *)event_info;
   Color_Prop_Data *pd = (Color_Prop_Data *)data;
   Eina_Stringshare *description;

   if (current_color)
     {
        Colorclass_Item *ccl = current_color->current_ccl;

        elm_box_pack_end(pd->box_object_color, pd->colorsel1_layout);
        evas_object_show(pd->colorsel1_layout);
        elm_box_pack_end(pd->box_outline_color, pd->colorsel2_layout);
        evas_object_show(pd->colorsel2_layout);
        elm_box_pack_end(pd->box_shadow_color, pd->colorsel3_layout);
        evas_object_show(pd->colorsel3_layout);

        pd->color_data = *current_color;

        elm_colorselector_color_set(pd->colorsel1, ccl->r1, ccl->g1, ccl->b1, ccl->a1);
        elm_colorselector_color_set(pd->colorsel2, ccl->r2, ccl->g2, ccl->b2, ccl->a2);
        elm_colorselector_color_set(pd->colorsel3, ccl->r3, ccl->g3, ccl->b3, ccl->a3);
        description = edje_edit_color_class_description_get(ap.project->global_object, ccl->name);
        elm_entry_entry_set(pd->description, description);
        elm_object_disabled_set(pd->description, false);

        free(current_color);
     }
   else
     {
        elm_box_unpack(pd->box_object_color, pd->colorsel1_layout);
        evas_object_hide(pd->colorsel1_layout);
        elm_box_unpack(pd->box_outline_color, pd->colorsel2_layout);
        evas_object_hide(pd->colorsel2_layout);
        elm_box_unpack(pd->box_shadow_color, pd->colorsel3_layout);
        evas_object_hide(pd->colorsel3_layout);
        elm_entry_entry_set(pd->description, "");
        elm_object_disabled_set(pd->description, true);
     }
}

static void
_on_property_color_del(void * data,
                       Evas *e __UNUSED__,
                       Evas_Object *obj,
                       void *event_info __UNUSED__)
{
   Color_Prop_Data *pd = (Color_Prop_Data *)data;

   evas_object_event_callback_del(obj, EVAS_CALLBACK_DEL, _on_property_color_del);
   free(pd);
}

static void
_on_description_change(void *data,
                       Evas_Object *obj,
                       void *ei __UNUSED__)
{
   Color_Prop_Data *pd = (Color_Prop_Data *)data;
   const char *text;
   assert(pd != NULL);
   text = elm_entry_entry_get(obj);

   Colorclass_Item *cc_it = pd->color_data.current_ccl;
   edje_edit_color_class_description_set(ap.project->global_object, cc_it->name, text);

   editor_save(ap.project->global_object);
   TODO("Remove this line once edje_edit_colorclass API would be added into Editor Module and saving would work properly")
   ap.project->changed = true;
}
static Evas_Object *
prop_color_desctiption_add(Evas_Object *parent, Color_Prop_Data *pd)
{
   assert(parent != NULL);
   assert(pd != NULL);

   PROPERTY_ITEM_ADD(parent,  _("description"), "1swallow");
   ENTRY_ADD(parent, pd->description, true);
   elm_object_disabled_set(pd->description, true);
   elm_object_tooltip_text_set(pd->description, _("Set ColorClass'es description"));
   evas_object_smart_callback_add(pd->description, "changed,user", _on_description_change, pd);
   elm_layout_content_set(item, "elm.swallow.content", pd->description);

   return item;
}

Evas_Object *
ui_property_color_add(Evas_Object *parent)
{
   Color_Prop_Data *pd;
   Evas_Object *scroller, *main_box, *item;

   assert(parent != NULL);

   pd = mem_calloc(1, sizeof(Color_Prop_Data));

   SCROLLER_ADD(parent, scroller);

   BOX_ADD(scroller, main_box, EINA_FALSE, EINA_FALSE);
   elm_box_align_set(main_box, 0.5, 0.0);
   elm_object_content_set(scroller, main_box);

   elm_scroller_policy_set(scroller, ELM_SCROLLER_POLICY_AUTO, ELM_SCROLLER_POLICY_AUTO);

   item = prop_color_desctiption_add(main_box, pd);
   elm_box_pack_end(main_box, item);
   elm_box_pack_end(main_box, _add_object_color_frame(pd, main_box));
   elm_box_pack_end(main_box, _add_outline_color_frame(pd, main_box));
   elm_box_pack_end(main_box, _add_shadow_color_frame(pd, main_box));

   evas_object_smart_callback_add(ap.win, SIGNAL_COLOR_SELECTED, _on_color_selected, pd);
   evas_object_event_callback_add(main_box, EVAS_CALLBACK_DEL, _on_property_color_del, pd);

   return scroller;
}
