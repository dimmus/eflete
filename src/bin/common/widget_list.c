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

#include <Eina.h>
#include "widget_list.h"

static const char *exception[] =
{
   "elm/map/marker/empty/",
   "elm/map/marker/radio/",
   "elm/map/circle/base/",
   "elm/map/scale/base/",
   "elm/scroller/panel/top/",
   "elm/scroller/panel/bottom/",
   "elm/scroller/panel/left/",
   "elm/scroller/panel/right/",
   "elm/index/base/vertical/",
   "elm/index/item/vertical/",
   "elm/index/item_odd/vertical/",
   "elm/index/item/horizontal/",
   "elm/index/item_odd/horizontal/",
   "elm/colorselector/picker/base/",
   "elm/colorselector/item/color/",
   "elm/colorselector/image/colorbar_0/",
   "elm/colorselector/image/colorbar_1/",
   "elm/colorselector/image/colorbar_2/",
   "elm/colorselector/image/colorbar_3/",
   "elm/gengrid/item/default/",
   "elm/entry/handler/start/",
   "elm/entry/handler/end/",
   "elm/entry/emoticon/wtf/",
   "elm/popup/buttons1/popup/",
   "elm/popup/buttons2/popup/",
   "elm/popup/buttons3/popup/",
   "elm/popup/content/popup/",
   "elm/popup/item/popup/",
   NULL
};

const char *
widget_name_get(const Eina_Stringshare *group_name)
{
   int len = strlen(group_name);
   int i;
   char str[32];

   if (group_name[0] != 'e') return NULL;
   if (group_name[1] != 'l') return NULL;
   if (group_name[2] != 'm') return NULL;
   if (group_name[3] != '/') return NULL;

   for (i = 4; i < len; i++)
     {
        if (group_name[i] == '/') break;
        str[i - 4] = group_name[i];
     }
   str[i - 4] = '\0';

   return strdup(str);
}

const char *
style_name_get(const Eina_Stringshare *group_name)
{
   int len = strlen(group_name);
   int ex_len;
   int first, i, j;
   char widget[32], class[32], style[256];

   if (group_name[0] != 'e') return NULL;
   if (group_name[1] != 'l') return NULL;
   if (group_name[2] != 'm') return NULL;
   if (group_name[3] != '/') return NULL;

   style[0] = '\0';

   for (i = 0; exception[i]; i++)
     {
        ex_len = strlen(exception[i]);
        if (!strncmp(exception[i], group_name, ex_len))
          {
             for (j = ex_len; j < len; j++)
               style[j - ex_len] = group_name[j];

             style[j - ex_len] = '\0';
          }
     }

   if (style[0] == '\0')
     {
        first = 4;
        for (i = first; i < len; i++)
          {
             if (group_name[i] == '/') break;
             widget[i - first] = group_name[i];
          }
        widget[i - first] = '\0';

        first = i + 1;
        for (i = first; i < len; i++)
          {
             if (group_name[i] == '/') break;
             class[i - first] = group_name[i];
          }
        class[i - first] = '\0';

        if (!strcmp(widget, "genlist") && strcmp(class, "base")) return NULL;

        first = i + 1;
        for (i = first; i < len; i++)
          {
             style[i - first] = group_name[i];
          }
        style[i - first] = '\0';
     }

   return strdup(style);
}

Eina_Bool
style_name_check(const Eina_Stringshare *group_name, const char *style_name)
{
   const char *style = style_name_get(group_name);

   if (!style) return EINA_FALSE;

   return !strcmp(style_name, style);
}

char* strrstr(char* str, const char* ptn)
{
   unsigned int ptnlen = 0;
   unsigned int i = 0, j = 0;

   ptnlen = strlen(ptn) - 1;

   for (i = strlen(str) - 1; i != 0; i--)
     {
        if (str[i] == ptn[ptnlen])
          {
             if (ptnlen == 0)
               {
                  return &str[i];
               }
             for (j = 1; (j < strlen(ptn)) & (i > j); j++)
               {
                  if (str[i - j] != ptn[ptnlen - j])
                    {
                       break;
                    }
                  if (j + 1 == strlen(ptn))
                    {
                       return &str[i - j];
                    }
               }
          }
     }

   return NULL;
}

const char *
item_style_name_get(const Eina_Stringshare *group_name, Eina_List *style_list)
{
   int len = strlen(group_name);
   int first, i;
   Eina_List *l;
   Tree_Item_Data *style_item;
   char widget[32], class[32], style[256];

   if (group_name[0] != 'e') return NULL;
   if (group_name[1] != 'l') return NULL;
   if (group_name[2] != 'm') return NULL;
   if (group_name[3] != '/') return NULL;

   for (i = 4; i < len; i++)
     {
        if (group_name[i] == '/') break;
     }

   first = 4;
   for (i = first; i < len; i++)
     {
        if (group_name[i] == '/') break;
        widget[i - first] = group_name[i];
     }
   widget[i - first] = '\0';

   first = i + 1;
   for (i = first; i < len; i++)
     {
        if (group_name[i] == '/') break;
        class[i - first] = group_name[i];
     }
   class[i - first] = '\0';

   if (!strcmp(widget, "genlist") && !strcmp(class, "base")) return NULL;

   first = i + 1;
   for (i = first; i < len; i++)
     {
        style[i - first] = group_name[i];
     }
   style[i - first] = '\0';

   EINA_LIST_FOREACH(style_list, l, style_item)
     {
        char *str;

        str = strrstr(style, style_item->name);
        if (str)
          {
             style[strlen(style) - strlen(str) - 1] = '\0';
          }
     }

   return strdup(style);
}

Eina_Bool
item_style_name_check(const Eina_Stringshare *group_name, const char *style_name, Eina_List *style_list)
{
   const char *style = item_style_name_get(group_name, style_list);

   if (!style) return EINA_FALSE;

   return !strcmp(style_name, style);
}

const char *
option_widget_name_get(const char *str, Eina_List **style_list)
{
   int len = strlen(str);
   char widget[32], style[256];
   Eina_List *list = NULL;
   int i, first = 0;
   Eina_Bool is_style = EINA_FALSE;

   for (i = 0; i < len; i++)
     {
        if (str[i] == ':')
          {
             is_style = EINA_TRUE;
             widget[i] = '\0';
             first = i + 1;
             continue;
          }

        if (!is_style)
          {
             widget[i] = str[i];
          }
        else
          {
             if (str[i] == ',')
               {
                  style[i - first] = '\0';
                  list = eina_list_append(list, strdup(style));
                  first = i + 1;
                  continue;
               }
               style[i - first] = str[i];
          }
     }

   if (!is_style)
      widget[i] = '\0';
   else
     {
        style[i - first] = '\0';
        list = eina_list_append(list, strdup(style));
     }

   *style_list = list;

   return strdup(widget);
}
