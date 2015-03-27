/* Elementary Widgets Extension
 * Copyright (C) 2014 Samsung Electronics.
 *
 * This file is part of Elementary Widgets Extension.
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

#ifndef EWE_RULER_EO_H
#define EWE_RULER_EO_H

#include "ewe_ruler.eo.h"

#if 0
#define EWE_OBJ_RULER_CLASS ewe_obj_ruler_class_get()

const Eo_Class *ewe_obj_ruler_class_get(void) EINA_CONST;

extern EAPI Eo_Op EWE_OBJ_RULER_BASE_ID;
#define EWE_OBJ_RULER_ID(sub_id) (EWE_OBJ_RULER_BASE_ID + sub_id)

enum
{
   EWE_OBJ_RULER_SUB_ID_ORIENT_SET,
   EWE_OBJ_RULER_SUB_ID_ORIENT_GET,
   EWE_OBJ_RULER_SUB_ID_STEP_MARKS_SET,
   EWE_OBJ_RULER_SUB_ID_ZERO_OFFSET_SET,
   EWE_OBJ_RULER_SUB_ID_LAST
};

#define ewe_obj_ruler_orient_set(ret, orient) EWE_OBJ_RULER_ID(EWE_OBJ_RULER_SUB_ID_ORIENT_SET), EO_TYPECHECK(Eina_Bool *, ret) ,EO_TYPECHECK(Ewe_Ruler_Orient, orient)
#define ewe_obj_ruler_orient_get(ret) EWE_OBJ_RULER_ID(EWE_OBJ_RULER_SUB_ID_ORIENT_GET), EO_TYPECHECK(Ewe_Ruler_Orient*, ret)
#define ewe_obj_ruler_step_between_marks_set(ret, step) EWE_OBJ_RULER_ID(EWE_OBJ_RULER_SUB_ID_STEP_MARKS_SET),EO_TYPECHECK(Eina_Bool *, ret), EO_TYPECHECK(unsigned int, step)
#define ewe_obj_ruler_zero_offset_set(ret, pos) EWE_OBJ_RULER_ID(EWE_OBJ_RULER_SUB_ID_ZERO_OFFSET_SET),EO_TYPECHECK(Eina_Bool *, ret), EO_TYPECHECK(int, pos)

#endif

#endif /* EWE_RULER_EO_H */
