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

#include "history.h"
#include "history_private.h"

static  Elm_Genlist_Item_Class *_itc_change = NULL;
static  Elm_Genlist_Item_Class *_itc_change_inactive = NULL;

static void
_on_change_selected(void *data,
                    Evas_Object *obj  __UNUSED__,
                    void *event_info)
{
   Module *module = (Module *)data;
   Diff *selected = NULL;
   int delta = 0;

   assert(module != NULL);

   int index_curr = 0;

   Elm_Object_Item *glit = (Elm_Object_Item *)event_info;
   selected = elm_object_item_data_get(glit);

   assert(selected != NULL);

   if (module->current_change)
     index_curr = module->current_change->index;

   delta = index_curr - selected->index;
   if (delta > 0)
     history_undo(module->target, delta);
   else if (delta < 0)
     history_redo(module->target, abs(delta));
}

Eina_Bool
_item_state_get(void *data,
                Evas_Object *obj __UNUSED__,
                const char *part)
{
   Diff *diff = (Diff *)data;

   assert(diff != NULL);

   if (((diff->action_type == MODIFY) || (diff->action_type == CONTAINER))
       && ((!strcmp(part, "modify"))))
       return true;
   if ((diff->action_type == ADD) && ((!strcmp(part, "add"))))
       return true;
   if ((diff->action_type == DEL) && ((!strcmp(part, "delete"))))
       return true;
   if ((diff->action_type == RESTACK) && ((!strcmp(part, "restack"))))
       return true;
   return false;
}
static char *
_item_label_get(void *data,
                Evas_Object *obj __UNUSED__,
                const char *part)
{
   Diff *diff = (Diff *)data;

   assert(diff != NULL);

   if ((diff->description) && (!strcmp(part, "elm.text.message")))
     {
        return strdup(diff->description);
     }

   if ((diff->state) && (!strcmp(part, "elm.text.state")))
     {
        return strdup(diff->state);
     }

   if ((diff->source) && (!strcmp(part, "elm.text.source")))
     {
        return strdup(diff->source);
     }

   if ((diff->new) && (!strcmp(part, "elm.text.new")))
     {
        return strdup(diff->new);
     }

   if ((diff->old) && (!strcmp(part, "elm.text.old")))
     {
        return strdup(diff->old);
     }
   return NULL;
}

Evas_Object *
_history_ui_add(Evas_Object *parent)
{
   Evas_Object *genlist = NULL;

   assert(parent != NULL);

   if (!_itc_change)
     {
        _itc_change = elm_genlist_item_class_new();
        _itc_change->item_style = "history";
        _itc_change->func.text_get = _item_label_get;
        _itc_change->func.content_get = NULL;
        _itc_change->func.state_get = _item_state_get;
        _itc_change->func.del = NULL;
     }

   if (!_itc_change_inactive)
     {
        _itc_change_inactive = elm_genlist_item_class_new();
        _itc_change_inactive->item_style = "history_inactive";
        _itc_change_inactive->func.text_get = _item_label_get;
        _itc_change_inactive->func.content_get = NULL;
        _itc_change_inactive->func.state_get = _item_state_get;
        _itc_change_inactive->func.del = NULL;
     }

   genlist = elm_genlist_add(parent);

   evas_object_size_hint_align_set(genlist, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(genlist, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

   return genlist;
}

void
_history_ui_item_update(Diff *change, Eina_Bool active, Eina_Bool current)
{
   if (current)
     {
        elm_genlist_item_show(change->ui_item, ELM_GENLIST_ITEM_SCROLLTO_IN);
        elm_genlist_item_selected_set(change->ui_item, current);
     }

   if (active)
     {
        elm_genlist_item_item_class_update(change->ui_item, _itc_change);
        elm_genlist_item_class_ref(_itc_change);
        elm_genlist_item_class_unref(_itc_change_inactive);
     }
   else
     {
        elm_genlist_item_item_class_update(change->ui_item, _itc_change_inactive);
        elm_genlist_item_selected_set(change->ui_item, false);
        elm_genlist_item_class_ref(_itc_change_inactive);
        elm_genlist_item_class_unref(_itc_change);
     }
}

void
_history_ui_item_add(Diff *change, Module *module)
{
   assert(change != NULL);
   assert(module != NULL);

   History *history = history_get();

   assert(history != NULL);
   assert(history->genlist != NULL);

   change->ui_item = elm_genlist_item_append(history->genlist, _itc_change,
                                             change, NULL, ELM_GENLIST_ITEM_NONE,
                                             _on_change_selected, module);
   elm_object_item_data_set(change->ui_item, change);
   _history_ui_item_update(change, true, true);
}

void
_history_ui_list_reload(History *history, Module *module)
{
   Eina_List *l, *l_next;
   Diff *data = NULL;
   Eina_Bool canceled = false;

   assert(history != NULL);
   assert(module != NULL);

   elm_genlist_clear(history->genlist);

   if (!module->current_change) canceled = true;

   EINA_LIST_FOREACH_SAFE(module->changes, l, l_next, data)
     {
        data->ui_item = elm_genlist_item_append(history->genlist, _itc_change,
                                                data, NULL, ELM_GENLIST_ITEM_NONE,
                                                _on_change_selected, module);

        elm_object_item_data_set(data->ui_item, data);

        if (canceled)
          _history_ui_item_update(data, !canceled, false);

        if (data == module->current_change)
          {
             _history_ui_item_update(data, true, true);
             canceled = true;
          }
     }
   return;
}
