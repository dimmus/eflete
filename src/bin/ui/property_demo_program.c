/*
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

#include "property_private.h"
#include "property_macros.h"
#include "main_window.h"
#include "history.h"
#include "validator.h"
#include "colorsel.h"
#include "project_manager.h"

#define DEMO_PROGRAM_PROP_DATA "program_prop_data"

#define DEMO_PROGRAM_PROP_DATA_GET() \
   assert(property != NULL); \
   Demo_Program_Prop_Data *pd = evas_object_data_get(property, DEMO_PROGRAM_PROP_DATA); \
   assert(pd != NULL);

struct _Demo_Program_Prop_Data
{
   Evas_Object *box;

   Evas_Object *scroller;
   Evas_Object *frame;
   Evas_Object *name;

   Evas_Object *signal;
   Evas_Object *source;
   Evas_Object *action;
   Evas_Object *signal_name;
   Evas_Object *emitter;

   Demo_Signal *part;
};
typedef struct _Demo_Program_Prop_Data Demo_Program_Prop_Data;

static const char *program_actions[] = {
     N_("NONE"),
     N_("STATE SET"),
     N_("SIGNAL EMIT"),
     N_("DRAG VALUE SET"),
     N_("DRAG VALUE STEP"),
     N_("DRAG VALUE PAGE"),
     N_("PLAY SAMPLE"),
     N_("PLAY TONE"),
     N_("ACTION STOP"),
     NULL
};

static Evas_Object *
prop_item_label_add(Evas_Object *parent,
                    Evas_Object **label,
                    const char *lab_text,
                    const char *text)
{
   assert(parent != NULL);
   assert(label != NULL);

   PROPERTY_ITEM_ADD(parent, lab_text, "1swallow")
   LABEL_ADD(item, *label, text)
   elm_object_part_content_set(item, "elm.swallow.content", *label);
   return item;
}

void
ui_property_demo_program_set(Evas_Object *property, Demo_Signal *part)
{
   DEMO_PROGRAM_PROP_DATA_GET()

   /* setting */
   if (part)
     {
        elm_object_text_set(pd->signal, part->sig_name);
        elm_object_text_set(pd->source, part->source_name);

        switch (part->action)
          {
           case EDJE_ACTION_TYPE_NONE:
              elm_object_text_set(pd->action, program_actions[0]);
              break;
           case EDJE_ACTION_TYPE_STATE_SET:
              elm_object_text_set(pd->action, program_actions[1]);
              break;
           case EDJE_ACTION_TYPE_SIGNAL_EMIT:
              elm_object_text_set(pd->action, program_actions[2]);
              break;
           case EDJE_ACTION_TYPE_DRAG_VAL_SET:
              elm_object_text_set(pd->action, program_actions[3]);
              break;
           case EDJE_ACTION_TYPE_DRAG_VAL_STEP:
              elm_object_text_set(pd->action, program_actions[4]);
              break;
           case EDJE_ACTION_TYPE_DRAG_VAL_PAGE:
              elm_object_text_set(pd->action, program_actions[5]);
              break;
           case EDJE_ACTION_TYPE_SOUND_SAMPLE:
              elm_object_text_set(pd->action, program_actions[6]);
              break;
           case EDJE_ACTION_TYPE_SOUND_TONE:
              elm_object_text_set(pd->action, program_actions[7]);
              break;
           case EDJE_ACTION_TYPE_ACTION_STOP:
              elm_object_text_set(pd->action, program_actions[8]);
              break;
           default:
              elm_object_text_set(pd->action, program_actions[0]);
              break;
          }
        if (part->action == EDJE_ACTION_TYPE_SIGNAL_EMIT)
          {
             elm_object_text_set(pd->signal_name, part->emit_signal);
             elm_object_text_set(pd->emitter, part->emitter);
          }
        else
          {
             elm_object_text_set(pd->signal_name, _(" - "));
             elm_object_text_set(pd->emitter, _(" - "));
          }
     }
   else
     {
        elm_object_text_set(pd->signal, _(" - "));
        elm_object_text_set(pd->source, _(" - "));
        elm_object_text_set(pd->action, _(" - "));
        elm_object_text_set(pd->signal_name, _(" - "));
        elm_object_text_set(pd->emitter, _(" - "));
     }

   pd->part = part;

   /* showing */
   evas_object_show(property);
}

Evas_Object *
ui_property_demo_program_add(Evas_Object *parent)
{
   Demo_Program_Prop_Data *pd;
   Evas_Object *item, *scroller, *box;

   assert(parent != NULL);

   pd = mem_calloc(1, sizeof(Demo_Program_Prop_Data));

   SCROLLER_ADD(parent, scroller);
   BOX_ADD(scroller, box, false, false);
   elm_box_align_set(box, 0.5, 0.0);
   elm_object_content_set(scroller, box);
   pd->scroller = scroller;
   elm_scroller_policy_set(pd->scroller, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);

   evas_object_data_set(pd->scroller, DEMO_PROGRAM_PROP_DATA, pd);
   /* editors */
   FRAME_ADD(box, pd->frame, true, _("Demo Program Property"))
   elm_box_pack_end(box, pd->frame);
   BOX_ADD(parent, pd->box, false, false);
   elm_box_align_set(pd->box, 0.5, 0.0);
   evas_object_hide(pd->box);

   /* Frame with info */
   elm_box_align_set(pd->box, 0.5, 0.0);
   item = prop_item_label_add(pd->box, &pd->signal, _("signal"), _(" - "));
   elm_box_pack_end(pd->box, item);
   item = prop_item_label_add(pd->box, &pd->source, _("source"), _(" - "));
   elm_box_pack_end(pd->box, item);
   item = prop_item_label_add(pd->box, &pd->action, _("action"), _(" - "));
   elm_box_pack_end(pd->box, item);
   item = prop_item_label_add(pd->box, &pd->signal_name, _("emit signal"), _(" - "));
   elm_box_pack_end(pd->box, item);
   item = prop_item_label_add(pd->box, &pd->emitter, _("emitter"), _(" - "));
   elm_box_pack_end(pd->box, item);

   elm_object_content_set(pd->frame, pd->box);

   return pd->scroller;
}
