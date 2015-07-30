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
 * along with this program; If not, see
 * www.gnu.org/licenses/lgpl.html.
 */

#ifndef EWE_COMBOBOX_EO_H
#define EWE_COMBOBOX_EO_H

#include "ewe_combobox.eo.h"

#if 0
#define EWE_OBJ_COMBOBOX_CLASS ewe_obj_combobox_class_get()

const Eo_Class *ewe_obj_combobox_class_get(void) EINA_CONST;

extern EAPI Eo_Op EWE_OBJ_COMBOBOX_BASE_ID;

enum
{
   EWE_OBJ_COMBOBOX_SUB_ID_ITEM_ADD,
   EWE_OBJ_COMBOBOX_SUB_ID_ITEM_DEL,
   EWE_OBJ_COMBOBOX_SUB_ID_ITEMS_LIST_GET,
   EWE_OBJ_COMBOBOX_SUB_ID_ITEM_TITLE_SET,
   EWE_OBJ_COMBOBOX_SUB_ID_ITEM_TITLE_GET,
   EWE_OBJ_COMBOBOX_SUB_ID_SELECT_ITEM_SET,
   EWE_OBJ_COMBOBOX_SUB_ID_SELECT_ITEM_GET,
   EWE_OBJ_COMBOBOX_SUB_ID_EXPANDED_GET,
   EWE_OBJ_COMBOBOX_SUB_ID_LAST
};

#define EWE_OBJ_COMBOBOX_ID(sub_id) (EWE_OBJ_COMBOBOX_BASE_ID + sub_id)

/**
 * @def ewe_obj_combobox_expanded_get
 *
 * @brief Returns whether the combobox is expanded.
 *
 * @param[out] ret
 *
 * @see ewe_combobox_expanded_get
 *
 * @ingroup Combobox
 */
#define ewe_obj_combobox_expanded_get(ret) EWE_OBJ_COMBOBOX_ID(EWE_OBJ_COMBOBOX_SUB_ID_EXPANDED_GET), EO_TYPECHECK(Eina_Bool *, ret)

#define ewe_obj_combobox_item_add(ret, title) EWE_OBJ_COMBOBOX_ID(EWE_OBJ_COMBOBOX_SUB_ID_ITEM_ADD), EO_TYPECHECK(Ewe_Combobox_Item **, ret), EO_TYPECHECK(Eina_Stringshare *, title)

#define ewe_obj_combobox_item_del(ret, item) EWE_OBJ_COMBOBOX_ID(EWE_OBJ_COMBOBOX_SUB_ID_ITEM_DEL), EO_TYPECHECK(Eina_Bool *, ret), EO_TYPECHECK(Ewe_Combobox_Item *, item)

#define ewe_obj_combobox_items_list_get(ret) EWE_OBJ_COMBOBOX_ID(EWE_OBJ_COMBOBOX_SUB_ID_ITEMS_LIST_GET), EO_TYPECHECK(const Eina_List **, ret)

#define ewe_obj_combobox_item_title_set(ret, item, title) EWE_OBJ_COMBOBOX_ID(EWE_OBJ_COMBOBOX_SUB_ID_ITEM_TITLE_SET), EO_TYPECHECK(Eina_Bool *, ret), EO_TYPECHECK(Ewe_Combobox_Item *, item), EO_TYPECHECK(Eina_Stringshare *, title)

#define ewe_obj_combobox_item_title_get(ret, item) EWE_OBJ_COMBOBOX_ID(EWE_OBJ_COMBOBOX_SUB_ID_ITEM_TITLE_GET), EO_TYPECHECK(Eina_Stringshare **, ret), EO_TYPECHECK(const Ewe_Combobox_Item *, item)

#define ewe_obj_combobox_select_item_set(ret, item) EWE_OBJ_COMBOBOX_ID(EWE_OBJ_COMBOBOX_SUB_ID_SELECT_ITEM_SET), EO_TYPECHECK(Eina_Bool *, ret), EO_TYPECHECK(Ewe_Combobox_Item *, item)

#define ewe_obj_combobox_select_item_get(ret) EWE_OBJ_COMBOBOX_ID(EWE_OBJ_COMBOBOX_SUB_ID_SELECT_ITEM_GET), EO_TYPECHECK(Ewe_Combobox_Item **, ret)
#endif

#endif /* EWE_COMBOBOX_EO_H */
