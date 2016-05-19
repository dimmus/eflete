/*
 * Edje Theme Editor
 * Copyright (C) 2013-2016 Samsung Electronics.
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

#include "property.h"
#include "property_private.h"
#include "sound_player.h"
#include "main_window.h"
#include "project_manager.h"

#define SOUND_MASK(TYPE) (1u << TYPE)
#define SOUND_SAMPLE SOUND_MASK(SOUND_TYPE_SAMPLE)
#define SOUND_TONE SOUND_MASK(SOUND_TYPE_TONE)

struct _Property_Sound_Data {
   Sound_Data *snd;
   External_Resource *sample;
   Tone_Resource *tone;
   Property_Attribute items[PROPERTY_SOUND_ITEM_LAST];
};
typedef struct _Property_Sound_Data Property_Sound_Data;
static Property_Sound_Data sound_pd;

/* mapping enums to strings */
static const char *edje_sound_compression[] = { N_("RAW"),
                                                N_("COMP"),
                                                N_("LOSSY"),
                                                N_("AS_IS"),
                                                NULL};

struct _Property_Sound_Update_Info {
   Property_Attribute *pa;
   Property_Action *action;
};
typedef struct _Property_Sound_Update_Info Property_Sound_Update_Info;

/* array to find item by Property_Item */
static Property_Sound_Update_Info attribute_map[PROPERTY_SOUND_ITEM_LAST];

static void
_fill_combobox_with_enum(Evas_Object *control, const char **array)
{
   int i = 0;

   assert(control != NULL);
   assert(array != NULL);

   while (array[i] != NULL)
     {
        ewe_combobox_item_add(control, array[i]);
        ++i;
     }
}

static void
_init_cb(Property_Attribute *pa, Property_Action *action)
{
   assert(pa != NULL);
   assert(action != NULL);
   assert(action->control != NULL);

   switch (pa->type.sound_item)
     {
      case PROPERTY_SOUND_ITEM_NAME:
         elm_object_disabled_set(action->control, true);
         break;
      case PROPERTY_SOUND_ITEM_COMPRESSION_QUALITY:
         elm_object_disabled_set(action->control, true);
         elm_spinner_label_format_set(action->control, "%.2f");
         break;
      case PROPERTY_SOUND_ITEM_FREQ:
         elm_spinner_min_max_set(action->control, 200, 20000);
         elm_object_disabled_set(action->control, true);
         break;
#ifdef HAVE_AUDIO
      case PROPERTY_SOUND_ITEM_DURATION:
#endif
      case PROPERTY_SOUND_ITEM_TYPE:
      case PROPERTY_SOUND_ITEM_SIZE:
      case PROPERTY_SOUND_ITEM_FILE_NAME:
      case PROPERTY_SOUND_ITEM_PLAYER:
         break;
      case PROPERTY_SOUND_ITEM_COMPRESSION_TYPE:
         elm_object_disabled_set(action->control, true);
         _fill_combobox_with_enum(action->control, edje_sound_compression);
         break;
      default:
         TODO("remove default case after all attributes will be added");
         CRIT("init callback not found for %s (%s)", pa->name, action->name ? action->name : "unnamed");
         abort();
         break;
     }
}

static void
_update_cb(Property_Attribute *pa, Property_Action *action)
{
   Eina_Stringshare *str_val1;
   double double_val1;

   assert(pa != NULL);
   assert(action != NULL);
   assert(action->control != NULL);

   switch (pa->type.sound_item)
     {
      case PROPERTY_SOUND_ITEM_NAME:
         if (sound_pd.snd)
           property_entry_set(action->control, sound_pd.snd->name);
         break;
      case PROPERTY_SOUND_ITEM_FILE_NAME:
         if (sound_pd.sample)
           elm_layout_text_set(action->control, NULL, ecore_file_file_get(sound_pd.sample->source));
         break;
#ifdef HAVE_AUDIO
      case PROPERTY_SOUND_ITEM_DURATION:
         break;
#endif
      case PROPERTY_SOUND_ITEM_TYPE:
         if (sound_pd.sample)
           elm_layout_text_set(action->control, NULL,
                               eina_stringshare_printf(_("%s Format Sound"), sound_pd.snd->type_label));
         break;
      case PROPERTY_SOUND_ITEM_SIZE:
         if (sound_pd.sample)
           {
              str_val1 = eina_stringshare_printf("%.2f KB", ecore_file_size(sound_pd.sample->source) / 1024.0);
              elm_layout_text_set(action->control, NULL, str_val1);
              eina_stringshare_del(str_val1);
           }
         break;

      case PROPERTY_SOUND_ITEM_COMPRESSION_TYPE:
         if (sound_pd.sample)
           ewe_combobox_select_item_set(action->control,
                                        (int) edje_edit_sound_compression_type_get(ap.project->global_object,
                                                                                   sound_pd.snd->name));
         break;
      case PROPERTY_SOUND_ITEM_COMPRESSION_QUALITY:
         if (sound_pd.sample)
           {
              double_val1 = edje_edit_sound_compression_rate_get(ap.project->global_object,
                                                              sound_pd.snd->name);
              elm_spinner_value_set(action->control, double_val1);
           }
         break;
      case PROPERTY_SOUND_ITEM_FREQ:
         if (sound_pd.tone)
           elm_spinner_value_set(action->control, sound_pd.tone->freq);
         break;

      case PROPERTY_SOUND_ITEM_PLAYER:
         break;
      default:
         TODO("remove default case after all attributes will be added");
         CRIT("update callback not found for %s (%s)", pa->name, action->name ? action->name : "unnamed");
         abort();
         break;
     }
}

/* blocks */
static inline void
_action_internal(Property_Action *action, const char *name, const char *units,
                 Property_Control control_type)
{
   /* check that we haven't inited this action already */
   assert(action->update_cb == NULL);

   action->name = name;
   action->units = units;
   action->control_type = control_type;
   action->update_cb = _update_cb;
   action->init_cb = _init_cb;
}

static inline void
_action1(Property_Attribute *pa, const char *name, const char *units,
         Property_Control control_type)
{
   _action_internal(&pa->action1, name, units, control_type);

   assert(attribute_map[pa->type.sound_item].pa == NULL);
   assert(attribute_map[pa->type.sound_item].action == NULL);

   attribute_map[pa->type.sound_item].pa = pa;
   attribute_map[pa->type.sound_item].action = &pa->action1;
}

static inline void
_action2(Property_Attribute *pa, const char *name, const char *units,
         Property_Control control_type)
{
   _action_internal(&pa->action2, name, units, control_type);

   assert(attribute_map[pa->type.sound_item].pa == NULL);
   assert(attribute_map[pa->type.sound_item].action == NULL);

   attribute_map[pa->type.sound_item].pa = pa;
   attribute_map[pa->type.sound_item].action = &pa->action2;
}


static Eina_List *
_subitems_get(Property_Attribute *pa)
{
   Eina_List *items = NULL;

   assert(pa != NULL);
#define APPEND(TYPE) items = eina_list_append(items, &sound_pd.items[TYPE]);
   switch (pa->type.sound_item)
     {
      case PROPERTY_SOUND_ITEM_PREVIEW_TITLE:
         APPEND(PROPERTY_SOUND_ITEM_PLAYER);
         break;
      case PROPERTY_SOUND_ITEM_INFO_TITLE:
         APPEND(PROPERTY_SOUND_ITEM_NAME);
         APPEND(PROPERTY_SOUND_ITEM_FILE_NAME);
#ifdef HAVE_AUDIO
         APPEND(PROPERTY_SOUND_ITEM_DURATION);
#endif
         APPEND(PROPERTY_SOUND_ITEM_TYPE);
         APPEND(PROPERTY_SOUND_ITEM_SIZE);
         APPEND(PROPERTY_SOUND_ITEM_COMPRESSION_TYPE);
         APPEND(PROPERTY_SOUND_ITEM_COMPRESSION_QUALITY);
         APPEND(PROPERTY_SOUND_ITEM_FREQ);
         break;
      default:
         CRIT("items callback not found for %s", pa->name);
         abort();
     }

   return items;
#undef APPEND
}

static Eina_Bool
_filter_cb(Property_Attribute *pa)
{
   assert(pa != NULL);

   if (sound_pd.snd)
     return !!(pa->filter_data.sound_types & SOUND_MASK(sound_pd.snd->type));
   else
     return false;
}

static void
_init_items()
{
   Property_Sound_Item it;
#define IT sound_pd.items[it]
   for (it = 0 /* first element of enum */; it < PROPERTY_SOUND_ITEM_LAST; it++)
     {
        IT.type.sound_item = it;
        IT.filter_cb = _filter_cb;

        /* default value: show for all types */
        IT.filter_data.sound_types = ~0u;
        switch(it)
          {
             /* group block */
           case PROPERTY_SOUND_ITEM_PREVIEW_TITLE:
              IT.name = "preview";
              IT.expandable = true;
              IT.expanded = true;
              IT.expand_cb = _subitems_get;
              break;
           case PROPERTY_SOUND_ITEM_PLAYER:
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_SOUND_PLAYER);
              break;

              /* part block */
           case PROPERTY_SOUND_ITEM_INFO_TITLE:
              IT.name = "info";
              IT.expandable = true;
              IT.expanded = true;
              IT.expand_cb = _subitems_get;
              IT.filter_data.sound_types = SOUND_SAMPLE | SOUND_TONE;
              break;
           case PROPERTY_SOUND_ITEM_NAME:
              IT.name = "name";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_ENTRY);
              break;
           case PROPERTY_SOUND_ITEM_FILE_NAME:
              IT.name = "file_name";
              IT.filter_data.sound_types = SOUND_SAMPLE;
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_LABEL);
              break;
#ifdef HAVE_AUDIO
           case PROPERTY_SOUND_ITEM_DURATION:
              IT.filter_data.sound_types = SOUND_SAMPLE;
              IT.name = "duration";
              break;
#endif
           case PROPERTY_SOUND_ITEM_TYPE:
              IT.filter_data.sound_types = SOUND_SAMPLE;
              IT.name = "type";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_LABEL);
              break;
           case PROPERTY_SOUND_ITEM_SIZE:
              IT.filter_data.sound_types = SOUND_SAMPLE;
              IT.name = "size";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_LABEL);
              break;
           case PROPERTY_SOUND_ITEM_COMPRESSION_TYPE:
              IT.filter_data.sound_types = SOUND_SAMPLE;
              IT.name = "compression type";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_COMBOBOX);
              break;
           case PROPERTY_SOUND_ITEM_COMPRESSION_QUALITY:
              IT.filter_data.sound_types = SOUND_SAMPLE;
              IT.name = "compression quality";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_SPINNER);
              break;
           case PROPERTY_SOUND_ITEM_FREQ:
              IT.filter_data.sound_types = SOUND_TONE;
              IT.name = "frequency";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_SPINNER);
              break;

           case PROPERTY_SOUND_ITEM_LAST:
              break;
          }
     }
#undef IT
}

static void
_on_grid_clicked(void *data,
                     Evas_Object *obj __UNUSED__,
                     void *event_info)
{
   Property_Data *pd = data;
   sound_pd.snd = (Sound_Data *)event_info;

   if (sound_pd.snd && sound_pd.snd->type == SOUND_TYPE_SAMPLE)
     {
        sound_pd.sample = (External_Resource *)sound_pd.snd->resource;
        sound_pd.tone = NULL;
     }
   else if (sound_pd.snd && sound_pd.snd->type == SOUND_TYPE_TONE)
     {
        sound_pd.sample = NULL;
        sound_pd.tone = (Tone_Resource *)sound_pd.snd->resource;
     }
   else
     {
        sound_pd.sample = NULL;
        sound_pd.tone = NULL;
     }

   assert(pd != NULL);

   sound_player_sound_set(sound_pd.snd);

   GENLIST_FILTER_APPLY(pd->genlist);
   property_item_update_recursively(&sound_pd.items[PROPERTY_SOUND_ITEM_INFO_TITLE]);
}

/* public */
void
property_sound_manager_init(Property_Data *pd)
{
   assert(pd != NULL);

   _init_items();

   /* register global callbacks */
   evas_object_smart_callback_add(ap.win, SIGNAL_SOUND_SELECT, _on_grid_clicked, pd);
   evas_object_smart_callback_add(ap.win, SIGNAL_SOUND_UNSELECTED, _on_grid_clicked, pd);
}

Eina_List *
property_sound_manager_items_get()
{
   Eina_List *items = NULL;

#ifdef HAVE_AUDIO
   items = eina_list_append(items, &sound_pd.items[PROPERTY_SOUND_ITEM_PREVIEW_TITLE]);
#endif
   items = eina_list_append(items, &sound_pd.items[PROPERTY_SOUND_ITEM_INFO_TITLE]);

   return items;
}
