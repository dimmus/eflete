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

#ifndef DIFF_H
#define DIFF_H
#include "eflete.h"

typedef enum {
   FUNCTION_TYPE_NONE = 0,
   FUNCTION_TYPE_INT,
   FUNCTION_TYPE_STRING_STRING_DOUBLE_DOUBLE,
   FUNCTION_TYPE_STRING_STRING_DOUBLE_INT
} Function_Type;

struct _Function_Info {
   Function_Type type;  /**< function type */
   void *function;      /**< pointer to function */
   union {
      /* One struct for each function type */
      struct {
         int ival;
      } type_int;
      struct {
         Eina_Stringshare *s1;
         Eina_Stringshare *s2;
         double d1;
         double d2;
      } type_ssdd;
      struct {
         Eina_Stringshare *s1;
         Eina_Stringshare *s2;
         double d1;
         int i1;
      } type_ssdi;
   } args;              /**< function args not including Evas_Object * */
};

typedef struct _Function_Info Function_Info;

struct _Diff_ {
   Function_Info undo;
   Function_Info redo;
};

typedef struct _Diff_ Diff;

Eina_Bool
diff_undo(Evas_Object *obj, Diff *diff);

Eina_Bool
diff_redo(Evas_Object *obj, Diff *diff);

void
diff_update(Diff *diff, Diff *new_diff);

void
diff_free(Diff *diff);

#endif /* DIFF_H */
