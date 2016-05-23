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

#define FONT_DEFAULT "DEFAULT='align=middle font=Sans font_size=24 color=#000000 "

struct _Property_Textblock_Data {
   Eina_Stringshare *font;
   int font_size;
   int font_style_weight;
   int font_style_width;
   struct {
      int r, g, b, a;
   } color;

   Property_Attribute items[PROPERTY_TEXTBLOCK_ITEM_LAST];
};
typedef struct _Property_Textblock_Data Property_Textblock_Data;
static Property_Textblock_Data textblock_pd;
#define tpd textblock_pd

struct _Property_Textblock_Update_Info {
   Property_Attribute *pa;
   Property_Action *action;
};
typedef struct _Property_Textblock_Update_Info Property_Textblock_Update_Info;

/* array to find item by Attribute */
static Property_Textblock_Update_Info attribute_map[ATTRIBUTE_TEXTBLOCK_ITEM_LAST];

static const char *font_weight_list[] = { "Normal",
                                          "Thin",
                                          "Ultralight",
                                          "Light",
                                          "Book",
                                          "Medium",
                                          "Semibold",
                                          "Bold",
                                          "Ultrabold",
                                          "Black",
                                          "Extrablack",
                                          NULL};

static const char *font_width_list[] = { "Normal",
                                         "Ultracondensed",
                                         "Extracondensed",
                                         "Condensed",
                                         "Extracondensed",
                                         "Semicondensed",
                                         "Semiexpanded",
                                         "Expanded",
                                         "Ultraexpanded",
                                         NULL};


/************************************************************************/
/*************        LOCAL FUNCTIONS TO SPLIT STYLE         ************/
/************************************************************************/

static int
_combobox_get_num(Eina_Tmpstr *tmp, const char **array)
{
   int i = 0;

   assert(tmp != NULL);
   assert(array != NULL);

   while (array[i] != NULL)
     {
        if (strcmp(tmp, array[i])) return i;
        ++i;
     }
   return 0; /* NONE */
}

static Eina_Bool
_hex_to_rgb(const char *hex, int *r, int *g, int *b, int *a)
{
   assert(hex != NULL);
   assert(r != NULL);
   assert(g != NULL);
   assert(b != NULL);
   assert(a != NULL);

   unsigned long val;
   char *end;
   int length = (int)strlen(hex);
   switch (length)
     {
      case 7:
        {
           val = strtoul(hex + 1, &end, 16);
           *r = (val & 0xff0000) >> 16;
           *g = (val & 0xff00) >> 8;
           *b = val & 0xff;
           *a = 255;
           break;
        }
      case 9:
        {
           val = strtoul(hex + 1, &end, 16);
           *r = (val & 0xff000000) >> 24;
           *g = (val & 0xff0000) >> 16;
           *b = (val & 0xff00) >> 8;
           *a = val & 0xff;
           break;
        }
      case 5:
        {
           val = strtoul(hex + 1, &end, 16);
           *r = (val & 0xf000) >> 12;
           *g = (val & 0xf00) >> 8;
           *b = (val & 0xf0) >> 4;
           *a = val & 0xf;
           *r = (*r << 4) | *r;
           *g = (*g << 4) | *g;
           *b = (*b << 4) | *b;
           *a = (*a << 4) | *a;
           break;
        }
      case 4:
        {
           val = strtoul(hex + 1, &end, 16);
           *r = (val & 0xf00) >> 8;
           *g = (val & 0xf0) >> 4;
           *b = val & 0xf;
           *a = 255;
           *r = (*r << 4) | *r;
           *g = (*g << 4) | *g;
           *b = (*b << 4) | *b;
           break;
        }
      default:
        {
           *r = 0;
           *g = 0;
           *b = 0;
           *a = 255;
        }
     }
   return true;
}

static Eina_Tmpstr*
_tag_value_get(const char* text_style, char* a_tag)
{
   assert(text_style != NULL);
   assert(a_tag != NULL);

   char *tag_list_copy = mem_malloc(strlen(text_style) + 1);
   Eina_Tmpstr *result = NULL;
   char *token;

   strcpy(tag_list_copy, text_style);
   token = strtok(tag_list_copy, " ");

   while (token)
     {
        char* equals_sign = strchr(token, '=');
        if (equals_sign)
          {
             *equals_sign = 0;
             if (!strcmp(token, a_tag))
               {
                  equals_sign++;

                  if (equals_sign[strlen(equals_sign) - 1] != '\'')
                    result = eina_tmpstr_add(equals_sign);
                  else
                    result = eina_tmpstr_add_length(equals_sign, strlen(equals_sign) - 1);
                  if (!strstr(FONT_DEFAULT, a_tag)) break;
               }
          }
        token = strtok(0, " ");
     }
   free(tag_list_copy);

   return result;
}

/************************************************************************/
/************************************************************************/
/************************************************************************/

/* local callbacks */
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

static Eina_List *
_subitems_get(Property_Attribute *pa)
{
   Eina_List *items = NULL;

   assert(pa != NULL);
#define APPEND(TYPE) items = eina_list_append(items, &textblock_pd.items[TYPE]);
   switch (pa->type.textblock_item)
     {
      case PROPERTY_TEXTBLOCK_ITEM_TEXT_TITLE:
         APPEND(PROPERTY_TEXTBLOCK_ITEM_TEXT_FONT_NAME);
         APPEND(PROPERTY_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WEIGHT);
         APPEND(PROPERTY_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WIDTH);
         APPEND(PROPERTY_TEXTBLOCK_ITEM_TEXT_COLOR);
         APPEND(PROPERTY_TEXTBLOCK_ITEM_TEXT_SIZE);
         break;
      default:
         CRIT("items callback not found for %s", pa->name);
         abort();
     }

   return items;
#undef APPEND
}

static void
_change_cb(Property_Attribute *pa, Property_Action *action)
{
   Eina_Stringshare *str_val1 = NULL;
   Ewe_Combobox_Item *cb_item = NULL;
   double double_val1 = 0.0;
   int r, g, b, a;

   assert(pa != NULL);
   assert(action != NULL);
   assert(action->control != NULL);

   switch (action->control_type)
     {
      case PROPERTY_CONTROL_SPINNER:
         double_val1 = elm_spinner_value_get(action->control);
         break;
      case PROPERTY_CONTROL_ENTRY:
         str_val1 = property_entry_get(action->control);
         break;
      case PROPERTY_CONTROL_COMBOBOX:
         TODO("change this after migrating to elm_combobox");
         cb_item = ewe_combobox_select_item_get(action->control);
         break;
      case PROPERTY_CONTROL_COLOR:
         property_color_control_color_get(action->control, &r, &g, &b, &a);
         break;
      default:
         break;
     }

   switch (action->type.attribute_textblock)
     {
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_NAME:
         if (tpd.font)
           eina_stringshare_del(tpd.font);
         tpd.font = eina_stringshare_add(str_val1);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WEIGHT:
         tpd.font_style_weight = cb_item->index;
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WIDTH:
         tpd.font_style_width = cb_item->index;
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_COLOR:
         tpd.color.r = r;
         tpd.color.g = g;
         tpd.color.b = b;
         tpd.color.a = a;
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_SIZE:
         tpd.font_size = double_val1;
         break;

      default:
         TODO("remove default case after all attributes will be added");
         CRIT("change callback not found for %s (%s)", pa->name, action->name ? action->name : "unnamed");
         abort();
         break;
     }
}

static void
_update_cb(Property_Attribute *pa, Property_Action *action)
{
   assert(pa != NULL);
   assert(action != NULL);
   assert(action->control != NULL);

   switch (action->type.attribute_textblock)
     {
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_NAME:
         property_entry_set(action->control, tpd.font);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WEIGHT:
         ewe_combobox_select_item_set(action->control, tpd.font_style_weight);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WIDTH:
         ewe_combobox_select_item_set(action->control, tpd.font_style_width);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_COLOR:
         property_color_control_color_set(action->control,
                                          tpd.color.r,
                                          tpd.color.g,
                                          tpd.color.b,
                                          tpd.color.a);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_SIZE:
         elm_spinner_value_set(action->control, tpd.font_size);
         break;
      default:
         TODO("remove default case after all attributes will be added");
         CRIT("update callback not found for %s (%s)", pa->name, action->name ? action->name : "unnamed");
         abort();
         break;
     }
}

static void
_init_cb(Property_Attribute *pa, Property_Action *action)
{
   assert(pa != NULL);
   assert(action != NULL);
   assert(action->control != NULL);

   switch (action->type.attribute_textblock)
     {
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_NAME:
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_COLOR:
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WEIGHT:
         _fill_combobox_with_enum(action->control, font_weight_list);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WIDTH:
         _fill_combobox_with_enum(action->control, font_width_list);
         break;
      case ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_SIZE:
         elm_spinner_min_max_set(action->control, 1, 9999);
         break;
      default:
         TODO("remove default case after all attributes will be added");
         CRIT("init callback not found for %s (%s)", pa->name, action->name ? action->name : "unnamed");
         abort();
         break;
     }
}

/* blocks */
static inline void
_action_internal(Property_Action *action, const char *name, const char *units,
                 Property_Control control_type, Attribute_Textblock_Item attribute)
{
   /* check that we haven't inited this action already */
   assert(action->update_cb == NULL);

   action->name = name;
   action->units = units;
   action->control_type = control_type;
   action->type.attribute_textblock = attribute;
   action->init_cb = _init_cb;
   action->update_cb = _update_cb;
   action->change_cb = _change_cb;
}

static inline void
_action1(Property_Attribute *pa, const char *name, const char *units,
         Property_Control control_type, Attribute_Textblock_Item attribute)
{
   _action_internal(&pa->action1, name, units, control_type, attribute);

   assert(attribute_map[attribute].pa == NULL);
   assert(attribute_map[attribute].action == NULL);

   attribute_map[attribute].pa = pa;
   attribute_map[attribute].action = &pa->action1;
}

static inline void
_action2(Property_Attribute *pa, const char *name, const char *units,
         Property_Control control_type, Attribute_Textblock_Item attribute)
{
   _action_internal(&pa->action2, name, units, control_type, attribute);

   assert(attribute_map[attribute].pa == NULL);
   assert(attribute_map[attribute].action == NULL);

   attribute_map[attribute].pa = pa;
   attribute_map[attribute].action = &pa->action2;
}

static Eina_Bool
_filter_cb(Property_Attribute *pa)
{
   assert(pa != NULL);

   /* will be updated later on */
   return true;
}

static void
_init_items()
{
   Property_Textblock_Item it;
#define IT textblock_pd.items[it]
   for (it = 0 /* first element of enum */; it < PROPERTY_TEXTBLOCK_ITEM_LAST; it++)
     {
        IT.type.textblock_item = it;
        IT.filter_cb = _filter_cb;

        /* default value: show for all types */
        IT.filter_data.textblock_types = ~0u;
        switch(it)
          {
           case PROPERTY_TEXTBLOCK_ITEM_TEXT_TITLE:
              IT.name = "text";
              IT.expandable = true;
              IT.expanded = true;
              IT.expand_cb = _subitems_get;
              break;
           case PROPERTY_TEXTBLOCK_ITEM_TEXT_FONT_NAME:
              IT.name = "font name";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_ENTRY, ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_NAME);
              break;
           case PROPERTY_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WEIGHT:
              IT.name = "font style (weight)";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_COMBOBOX, ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WEIGHT);
              break;
           case PROPERTY_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WIDTH:
              IT.name = "font style (width)";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_COMBOBOX, ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_FONT_STYLE_WIDTH);
              break;
           case PROPERTY_TEXTBLOCK_ITEM_TEXT_COLOR:
              IT.name = "color";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_COLOR, ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_COLOR);
              break;
           case PROPERTY_TEXTBLOCK_ITEM_TEXT_SIZE:
              IT.name = "size";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_SPINNER, ATTRIBUTE_TEXTBLOCK_ITEM_TEXT_SIZE);
              break;

           case PROPERTY_TEXTBLOCK_ITEM_LAST:
              break;
          }
     }
#undef IT
}

static void
_on_style_selected(void *data __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *event_info)
{
   Style_Data *current_style = (Style_Data *)event_info;
   Eina_Tmpstr *tmp;
   const char *value;

   if (current_style)
     {
        /* do a split */
        value = eina_strbuf_string_get(current_style->style);

        tmp = _tag_value_get(value, "font");
        if (!tmp) tmp = eina_tmpstr_add("");
        if (tpd.font) eina_stringshare_del(tpd.font);
        tpd.font = eina_stringshare_add(tmp);
        eina_tmpstr_del(tmp);

        tmp = _tag_value_get(value, "font_size");
        if (!tmp) tmp = eina_tmpstr_add("");
        tpd.font_size = atof(tmp);
        eina_tmpstr_del(tmp);

        tmp = _tag_value_get(value, "font_weight");
        if (!tmp) tmp = eina_tmpstr_add("Normal");
        tpd.font_style_weight = _combobox_get_num(tmp, font_weight_list);
        eina_tmpstr_del(tmp);

        tmp = _tag_value_get(value, "font_width");
        if (!tmp) tmp = eina_tmpstr_add("Normal");
        tpd.font_style_width = _combobox_get_num(tmp, font_width_list);
        eina_tmpstr_del(tmp);

        tmp = _tag_value_get(value, "color");
        if (!_hex_to_rgb(tmp, &tpd.color.r, &tpd.color.g, &tpd.color.b, &tpd.color.a))
          {
             ERR("Can't convert color value");
             abort();
          }
        eina_tmpstr_del(tmp);
     }
   else
     {
        if (tpd.font)
          eina_stringshare_del(tpd.font);
        tpd.font = NULL;
     }

   property_item_update_recursively(&textblock_pd.items[PROPERTY_TEXTBLOCK_ITEM_TEXT_TITLE]);
}

#undef tpd

/* public */
void
property_textblock_manager_init(Property_Data *pd)
{
   assert(pd != NULL);

   _init_items();

   evas_object_smart_callback_add(ap.win, SIGNAL_STYLE_SELECTED, _on_style_selected, NULL);
}

Eina_List *
property_textblock_manager_items_get()
{
   Eina_List *items = NULL;

   items = eina_list_append(items, &textblock_pd.items[PROPERTY_TEXTBLOCK_ITEM_TEXT_TITLE]);

   return items;
}
