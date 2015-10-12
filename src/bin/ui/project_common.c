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

#include "main_window.h"

void
exist_permission_check(const char *path, const char *name,
                       const char *title, const char *msg)
{
   Eina_Strbuf *buf, *buf_msg;
   Popup_Button btn_res;

   assert(path != NULL);
   assert(name != NULL);
   assert(title != NULL);
   /* we alwayes imported and exported project to folder by given path, means
    * that we alwayes create a new folder for project or exported source.
    * So need to check there is the folder "path/name" */
   buf = eina_strbuf_new();
   eina_strbuf_append_printf(buf, "%s/%s", path, name);
   if (!ecore_file_exists(eina_strbuf_string_get(buf))) return;
   btn_res = popup_want_action(title, msg, NULL,
                               NULL, BTN_REPLACE | BTN_CANCEL, NULL, NULL);
   if (btn_res == BTN_CANCEL) return;
   if (!ecore_file_can_write(eina_strbuf_string_get(buf)))
     {
        buf_msg = eina_strbuf_new();
        eina_strbuf_append_printf(buf_msg, _("Haven't permision to overwrite '%s' in '%s'"), name, path);
        popup_want_action(title, eina_strbuf_string_get(buf_msg), NULL, NULL, BTN_OK, NULL, NULL);
        eina_strbuf_free(buf_msg);
     }
   ecore_file_recursive_rm(eina_strbuf_string_get(buf));
   eina_strbuf_free(buf);
}
