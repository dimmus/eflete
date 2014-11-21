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

static  Elm_Genlist_Item_Class *_itc_module = NULL;
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

   if (!module) return;

   int index_curr = 0;

   Elm_Object_Item *glit = (Elm_Object_Item *)event_info;
   selected = elm_object_item_data_get(glit);
   if (!selected) return;

   if (module->current_change)
     index_curr = module->current_change->index;

   delta = index_curr - selected->index;
   if (delta > 0)
     history_undo(module->target, delta);
   else if (delta < 0)
     history_redo(module->target, abs(delta));
   return;
}

static void
_on_discard_changes_selected(void *data,
                           Evas_Object *obj __UNUSED__,
                           void *event_info __UNUSED__)
{
   Module *module = (Module *)data;

   int index_curr = 0;

   if (module->current_change)
     index_curr = module->current_change->index;

   history_undo(module->target, index_curr);
}


static char *
_module_item_label_get(void *data __UNUSED__,
                Evas_Object *obj __UNUSED__,
                const char *part __UNUSED__)
{
   return strdup("Discard all changes");
}

Eina_Bool
_item_state_get(void *data,
                Evas_Object *obj __UNUSED__,
                const char *part)
{
   Diff *diff = (Diff *)data;

   if (((diff->action_type == MODIFY) || (diff->action_type == HLIGHT))
       && ((!strcmp(part, "modify"))))
       return true;
   if ((diff->action_type == ADD) && ((!strcmp(part, "add"))))
       return true;
   if ((diff->action_type == DEL) && ((!strcmp(part, "delete"))))
       return true;
   return false;
}
static char *
_item_label_get(void *data,
                Evas_Object *obj __UNUSED__,
                const char *part __UNUSED__)
{
   Diff *diff = (Diff *)data;

   if (!strcmp(part, "elm.text.message"))
     {
        return strdup(diff->description);
     }

   if ((!strcmp(part, "elm.text.state")) && (diff->state))
     {
        return strdup(diff->state);
     }

   if (!strcmp(part, "elm.text.source"))
     {
        return strdup(diff->source);
     }

   if (!strcmp(part, "elm.text.new"))
     {
        return strdup(diff->new);
     }

   if (!strcmp(part, "elm.text.old"))
     {
        return strdup(diff->old);
     }
   return strdup(" ");
}

Evas_Object *
_history_ui_add(Evas_Object *parent)
{
   Evas_Object *genlist = NULL;

   if (!_itc_module)
     {
        _itc_module = elm_genlist_item_class_new();
        _itc_module->item_style = "eflete/level1";
        _itc_module->func.text_get = _module_item_label_get;
        _itc_module->func.content_get = NULL;
        _itc_module->func.state_get = NULL;
        _itc_module->func.del = NULL;
     }

   if (!_itc_change)
     {
        _itc_change = elm_genlist_item_class_new();
        _itc_change->item_style = "eflete/history";
        _itc_change->func.text_get = _item_label_get;
        _itc_change->func.content_get = NULL;
        _itc_change->func.state_get = _item_state_get;
        _itc_change->func.del = NULL;
     }

   if (!_itc_change_inactive)
     {
        _itc_change_inactive = elm_genlist_item_class_new();
        _itc_change_inactive->item_style = "eflete/history_inactive";
        _itc_change_inactive->func.text_get = _item_label_get;
        _itc_change_inactive->func.content_get = NULL;
        _itc_change_inactive->func.state_get = _item_state_get;
        _itc_change_inactive->func.del = NULL;
     }

   genlist = elm_genlist_add(parent);

   elm_object_style_set(genlist, "eflete/default");
   evas_object_size_hint_align_set(genlist, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(genlist, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

   return genlist;
}

void
_history_ui_item_update(Diff *change, Eina_Bool active, Eina_Bool current)
{
   if (active)
     {
        elm_genlist_item_item_class_update(change->ui_item, _itc_change);
        elm_genlist_item_class_ref(_itc_change);
        elm_genlist_item_class_unref(_itc_change_inactive);
     }
   else
     {
        elm_genlist_item_item_class_update(change->ui_item, _itc_change_inactive);
        elm_genlist_item_class_ref(_itc_change_inactive);
        elm_genlist_item_class_unref(_itc_change);
     }

   if (current)
     {
        elm_genlist_item_bring_in(change->ui_item, ELM_GENLIST_ITEM_SCROLLTO_IN);
        elm_genlist_item_selected_set(change->ui_item, current);
     }
}

void
_history_ui_item_add(Diff *change, Module *module)
{
   History *history = history_get();
   if ((!history) || (!history->genlist)) return;

   change->ui_item = elm_genlist_item_append(history->genlist, _itc_change,
                                             change, NULL, ELM_GENLIST_ITEM_NONE,
                                             _on_change_selected, module);
   elm_object_item_data_set(change->ui_item, change);
   _history_ui_item_update(change, true, true);
}

void
_history_module_ui_item_add(History *history, Module *module)
{
   module->ui_item = elm_genlist_item_append(history->genlist, _itc_module,
                                             NULL, NULL, ELM_GENLIST_ITEM_NONE,
                                             _on_discard_changes_selected, module);
}

void
_history_ui_list_reload(History *history, Module *module)
{
   Eina_List *l, *l_next;
   Diff *data = NULL;
   Eina_Bool canceled = false;

   elm_genlist_clear(history->genlist);
   _history_module_ui_item_add(history, module);

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

