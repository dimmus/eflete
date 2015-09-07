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

#include "diff.h"

TODO("Add pointer to History");
typedef Eina_Bool (* function_type_int) (Evas_Object *, int);

static Eina_Bool
_apply(Evas_Object *obj, Function_Info *fi)
{
   switch (fi->type)
     {
      case FUNCTION_TYPE_NONE:
         return true;
      case FUNCTION_TYPE_INT:
         return ((function_type_int)fi->function)(obj, fi->args.type_int.ival);
     }
   return false;
}

Eina_Bool
diff_undo(Evas_Object *obj, Diff_ *diff)
{
   assert(obj != NULL);
   assert(diff != NULL);

   return _apply(obj, &diff->undo);
}

Eina_Bool
diff_redo(Evas_Object *obj, Diff_ *diff)
{
   assert(obj != NULL);
   assert(diff != NULL);

   return _apply(obj, &diff->redo);
}
