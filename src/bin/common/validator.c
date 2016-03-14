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
#include "validator.h"
#include "project_manager.h"
#include <regex.h>

struct _Resource_Name_Validator
{
   Eina_Stringshare *signal;
   int status;
   regex_t regex;
   Eina_List **list;
   Eina_Bool sorted;
   Resource *res;
};

void
resource_name_validator_list_set(Resource_Name_Validator *validator, Eina_List **list, Eina_Bool sorted)
{
   assert(validator != NULL);
   assert(list != NULL);

   validator->list = list;
   validator->sorted = sorted;
}

void
resource_name_validator_resource_set(Resource_Name_Validator *validator, Resource *resource)
{
   assert(validator != NULL);

   validator->res = resource;
}

Resource_Name_Validator *
resource_name_validator_new(const char *pattern, const char *sig)
{
   Resource_Name_Validator *validator;

   assert(pattern != NULL);

   validator = mem_calloc(1, sizeof(Resource_Name_Validator));
   validator->signal = eina_stringshare_add(sig ? sig : "default");
   validator->status = regcomp(&validator->regex, pattern, REG_EXTENDED | REG_NOSUB) ? ELM_REG_BADPAT : ELM_REG_NOERROR;

   return validator;
}

void
resource_name_validator_free(Resource_Name_Validator *validator)
{
   assert(validator != NULL);

   eina_stringshare_del(validator->signal);
   regfree(&validator->regex);
   free(validator);
}

Elm_Regexp_Status
resource_name_validator_status_get(Resource_Name_Validator *validator)
{
   assert(validator != NULL);

   return validator->status;
}

Eina_Bool
resource_name_validator_helper(void *data, const Eo_Event *event)
{
   Resource *res;
   Elm_Validate_Content *vc = event->event_info;
   Resource_Name_Validator *validator = (Resource_Name_Validator *)data;

   assert(validator != NULL);
   assert(validator->list != NULL);

   validator->status = regexec(&validator->regex, vc->text, (size_t)0, NULL, 0) ? ELM_REG_NOMATCH : ELM_REG_NOERROR;
   if (validator->status == ELM_REG_NOERROR) /* new name match regex */
     {
        /* check if resource with this name already exists in list */
        if (validator->sorted)
          res = pm_resource_get(*validator->list, vc->text);
        else
          res = pm_resource_unsorted_get(*validator->list, vc->text);

        if (!res) /* name is free */
          validator->status = ELM_REG_NOERROR;
        else if ((validator->res != NULL) && (validator->res == res)) /* name is taken by the same resource (it is not changed) */
          validator->status = ELM_REG_NOERROR;
        else /* name is taken by another resource */
          validator->status = ELM_REG_NOMATCH;
     }
   vc->signal = validator->signal;
   return validator->status ? EO_CALLBACK_STOP : EO_CALLBACK_CONTINUE;
}
