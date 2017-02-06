/*
 * Edje Theme Editor
 * Copyright (C) 2013-2017 Samsung Electronics.
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
#define EO_BETA_API
#define EFL_BETA_API_SUPPORT
#define EFL_EO_API_SUPPORT

#include "modal_window.h"
#include "property.h"
#include "property_private.h"

typedef struct _Script_Manager Script_Manager;

struct _Script_Manager
{
   Evas_Object *win;
   Evas_Object *panes;
   Evas_Object *entry_prev;
   Evas_Object *image;
   Evas_Object *layout;
   Evas_Object *menu;
};

static Script_Manager mng;

static void
_mw_cancel_cb(void *data __UNUSED__,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   Evas_Object *content;

   /* unset and hide the script manager content */
   content = elm_object_content_unset(mng.win);
   evas_object_hide(content);
}

static void
_mw_done_cb(void *data __UNUSED__,
            Evas_Object *obj __UNUSED__,
            void *event_info __UNUSED__)
{
   Evas_Object *content;

   /* unset and hide the script manager content */
   content = elm_object_content_unset(mng.win);
   evas_object_hide(content);
}

Evas_Object *
script_manager_add(Resource2 *res)
{
   Evas_Object *ic, *script_entry, *search_entry;
   char *code;
   Group2 *group;
   Program2 *program;

   assert(res != NULL);
   assert((res->common.type == RESOURCE2_TYPE_GROUP) ||
          (res->common.type == RESOURCE2_TYPE_PROGRAM));

   assert(ap.project != NULL);

   mng.win = mw_add();
   mw_title_set(mng.win, _("Script manager"));
   evas_object_smart_callback_add(mng.win, signals.eflete.modal_window.cancel, _mw_cancel_cb, NULL);
   evas_object_smart_callback_add(mng.win, signals.eflete.modal_window.done, _mw_done_cb, NULL);
#if !HAVE_TIZEN
   ic = elm_icon_add(mng.win);
   elm_icon_standard_set(ic, "code2");
#else
   IMAGE_ADD_NEW(mng.win, ic, "icon", "logo");
#endif
   mw_icon_set(mng.win, ic);

   mng.layout = elm_layout_add(mng.win);
   elm_layout_theme_set(mng.layout, "layout", "script_manager", "default");

   ENTRY_ADD(mng.layout, script_entry, true);
   elm_entry_single_line_set(script_entry, false);
   elm_object_content_set(mng.layout, script_entry);
   elm_layout_content_set(mng.layout, "elm.swallow.list", script_entry);

   if (res->common.type == RESOURCE2_TYPE_GROUP)
     {
        program = NULL;
        group = (Group2 *)res;
     }
   else
     {
        program = (Program2 *)res;
        group = program->group;
     }

   if (res->common.type == RESOURCE2_TYPE_GROUP)
     {
        code = edje_edit_script_get(group->edit_object);
     }
   else
     {
        code = edje_edit_script_program_get(group->edit_object, program->common.name);
     }

   property_entry_set(script_entry, code);
   free(code);

   ENTRY_ADD(mng.layout, search_entry, true);
   elm_object_part_text_set(search_entry, "guide", _("Search"));
#if !HAVE_TIZEN
   Evas_Object *icon;
   ICON_STANDARD_ADD(search_entry, icon, true, "search");
   elm_object_part_content_set(search_entry, "elm.swallow.end", icon);
#else
   elm_object_style_set(search_entry, "search");
#endif
   elm_layout_content_set(mng.layout, "elm.swallow.search", search_entry);

   elm_object_content_set(mng.win, mng.layout);
   evas_object_show(mng.win);
   return mng.win;
}
