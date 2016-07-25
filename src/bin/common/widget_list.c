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

#include "resource_manager.h"
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

Eina_Stringshare *
option_widget_name_get(const char *str, Eina_List **style_list)
{
   int len = strlen(str);
   char widget[32], style[256];
   Eina_List *list = NULL;
   int i, first = 0;
   Eina_Bool is_style = EINA_FALSE, copying = EINA_FALSE;

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
             if (str[i] == '[')
               copying = EINA_TRUE;
             else if (str[i] == ']')
               copying = EINA_FALSE;

             if (!copying && str[i] == ',')
               {
                  style[i - first] = '\0';
                  list = eina_list_append(list, eina_stringshare_add(style));
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
        list = eina_list_append(list, eina_stringshare_add(style));
     }

   *style_list = list;

   return eina_stringshare_add(widget);
}

Eina_Stringshare *
option_style_name_get(const char *str, Eina_List **cp_style_list)
{
   int len = strlen(str);
   char style[32], cp_style[256];
   Eina_List *list = NULL;
   int i, first = 0;
   Eina_Bool is_cp_style = EINA_FALSE;

   for (i = 0; i < len; i++)
     {
        if (str[i] == '[')
          {
             is_cp_style = EINA_TRUE;
             style[i] = '\0';
             first = i + 1;
             continue;
          }
        else if (str[i] == ']')
          break;

        if (!is_cp_style)
          {
             style[i] = str[i];
          }
        else
          {
             if (str[i] == ',')
               {
                  cp_style[i - first] = '\0';
                  list = eina_list_append(list, eina_stringshare_add(cp_style));
                  first = i + 1;
                  continue;
               }
               cp_style[i - first] = str[i];
          }
     }

   if (!is_cp_style)
      style[i] = '\0';
   else
     {
        cp_style[i - first] = '\0';
        list = eina_list_append(list, eina_stringshare_add(cp_style));
     }

   *cp_style_list = list;

   return eina_stringshare_add(style);
}

Eina_List *
widget_prefix_list_get(Eina_List *collections, const char *widget_name, const char *style_name)
{
   Eina_List *l, *list = NULL;
   Eina_Stringshare *group_name;
   char prefix[1024];
   const char *widget = NULL;
   const char *style = NULL;
   int i, end = 0;

   EINA_LIST_FOREACH(collections, l, group_name)
     {
        widget = widget_name_get(group_name);

        if (widget && !strcmp(widget, widget_name))
          {
             style = style_name_get(group_name);
             if (style && !strcmp(style, style_name))
               {
                  end = strlen(group_name) - strlen(strrchr(group_name, '/')) + 1;
                  for (i = 0; i < end; i++)
                    prefix[i] = group_name[i];
                  prefix[i] = '\0';

                  list = eina_list_append(list, strdup(prefix));
               }
          }
     }
   return list;
}

Eina_Stringshare *
widget_prefix_get(const char *group_name, int level, int *symbols)
{
   const char *pos;
   char prefix[BUFF_MAX];
   int i, len;

   assert(group_name != NULL);
   assert(level >= 0);

   pos = group_name - 1;
   for (i = 0; (pos) && (i <= level); i++)
     {
        pos = strchr(pos + 1, '/');
     }
   if (!pos) return NULL;

   len = pos - group_name + 1;
   strncpy(prefix, group_name, len);
   prefix[len] = '\0';
   if (symbols) *symbols = len;

   return eina_stringshare_add(prefix);
}

void
widget_tree_items_get(Eina_List *groups,
                      const char *prefix,
                      Eina_List **folders_out,
                      Eina_List **groups_out)
{
   int cmp;
   int level = 0;
   const char *pos;
   Group *group, *group_next;
   size_t prefix_len;
   Eina_Stringshare *group_prefix;
   int group_prefix_len;
   Eina_List *l, *lnext;

   assert(prefix != NULL);
   assert(folders_out != NULL);
   assert(groups_out != NULL);

   prefix_len = strlen(prefix);

   pos = prefix - 1;
   while ((pos = strchr(pos + 1, '/')))
     level++;

   EINA_LIST_FOREACH(groups, l, group)
     {
        cmp = strncmp(group->name, prefix, prefix_len);
        /* skipping all groups with different prefix */
        if (cmp < 0)
          {
             continue;
          }
        if (cmp > 0)
          {
             if (strlen(group->name) < prefix_len)
               continue;
             else
               break; /* there is no sense to check all next groups because list is sorted */
          }

        group_prefix = widget_prefix_get(group->name, level, &group_prefix_len);
        if (group_prefix)
          {
             *folders_out= eina_list_append(*folders_out, eina_stringshare_ref(group_prefix));

             /* skipping other groups from the same "folder" */
             lnext = l;
             while ((lnext = eina_list_next(lnext)))
               {
                  group_next = eina_list_data_get(lnext);
                  if ((group_next) && (!strncmp(group_next->name, group_prefix, group_prefix_len)))
                    l = lnext;
                  else
                    break;
               }
          }
        else
          {
             *groups_out = eina_list_append(*groups_out, group);
          }
        eina_stringshare_del(group_prefix);
     }
}
