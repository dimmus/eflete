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

#include "project_manager.h"
#include "enventor_module.h"

#ifdef HAVE_ENVENTOR

#define MAX_SCALE 3.0
#define MIN_SCALE 0.5
#define STEP_SCALE 0.1f

static void
_on_enventor_mouse_wheel(void *data __UNUSED__,
                         Evas *evas __UNUSED__,
                         Evas_Object *enventor,
                         void *event_info)
{
   Evas_Event_Mouse_Wheel *event = (Evas_Event_Mouse_Wheel *)event_info ;

   if (!evas_key_modifier_is_set(event->modifiers, "Control"))  return;

   double scale = enventor_object_font_scale_get(enventor);
   scale += event->z >= 0 ? -STEP_SCALE: +STEP_SCALE;

   if ((scale <= MIN_SCALE) || (scale >= MAX_SCALE)) return;

   enventor_object_font_scale_set(enventor, scale);
}


Evas_Object *
enventor_object_init(Evas_Object *parent)
{
  Evas_Object *enventor = NULL;
  enventor = enventor_object_add(parent);
  evas_object_event_callback_add(enventor, EVAS_CALLBACK_MOUSE_WHEEL,
                                 _on_enventor_mouse_wheel, NULL);
  return enventor;
}

Eina_Bool
enventor_object_project_load(Evas_Object *enventor, Project *project)
{
  Eina_Stringshare *file = NULL;
  Eina_Tmpstr *tmpstr = NULL;
  char **tmp;
  unsigned int i = 0, tokens_count = 0;
  Style *style = NULL;
  Eina_Stringshare *path = NULL;

  if ((!enventor) || (!project) || (!project->current_style)) return false;
  style = project->current_style;

  if (!project->enventor)
    project->enventor = (Enventor_Data *)mem_calloc(1, sizeof(Enventor_Data));

  if (!project->enventor->file)
    {
       /* Prepare edc file and resources for using in enventor mode.
        * Project will created in temporary directory (linux: "/tmp";
        * Windows: path of enviroment variables TEMP|TMP|USERPROFILE|WINDIR).
        * Name of project directory compose by next rule: all terminants "/"
        * will replaced with "_" and generate unieque id suffix.
        * For example:
        * elm/bubble/base/default -> /tmp/elm_bubble_base_default_fGhds1/
        */

       tmp = eina_str_split_full(style->full_group_name, "/", 0, &tokens_count);
       if (!tmp[0]) return false;
       file = eina_stringshare_add(tmp[0]);
       for (i = 1; tokens_count - 1 > 0; i++, tokens_count--)
        file = eina_stringshare_printf("%s_%s", file, tmp[i]);
       free(tmp[0]);
       free(tmp);
       path = eina_stringshare_printf("%s_XXXXXX", file);
       eina_file_mkdtemp(path, &tmpstr);
       eina_stringshare_del(path);

       project->enventor->file = eina_stringshare_printf("%s/%s.edc", tmpstr, file);
       project->enventor->path = eina_stringshare_add(tmpstr);

       eina_tmpstr_del(tmpstr);
    }

  if (!pm_project_style_source_code_export(project, style, project->enventor->file))
    ERR("Source code of the current style was not written to the file %s", project->enventor->file);

  enventor_object_file_set(enventor, project->enventor->file);

  enventor_object_path_set(enventor, ENVENTOR_RES_FONT, project->res.fonts);
  enventor_object_path_set(enventor, ENVENTOR_RES_IMAGE, project->res.images);
  enventor_object_path_set(enventor, ENVENTOR_RES_SOUND, project->res.sounds);
  return true;
}

#endif /* HAVE_ENVENTOR */
