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

#ifndef EWE_TABS_EO_H
#define EWE_TABS_EO_H

#include "ewe_tabs.eo.h"

#if 0
#define EWE_OBJ_TABS_CLASS ewe_obj_tabs_class_get()

const Eo_Class *ewe_obj_tabs_class_get(void) EINA_CONST;

extern EAPI Eo_Op EWE_OBJ_TABS_BASE_ID;

enum
{
   EWE_OBJ_TABS_SUB_ID_ITEM_APPEND,
   EWE_OBJ_TABS_SUB_ID_ITEM_PREPEND,
   EWE_OBJ_TABS_SUB_ID_ITEM_DEL,
   EWE_OBJ_TABS_SUB_ID_ITEMS_LIST_GET,
   EWE_OBJ_TABS_SUB_ID_ITEM_ACTIVATE,
   EWE_OBJ_TABS_SUB_ID_ITEM_CONTENT_SET,
   EWE_OBJ_TABS_SUB_ID_ITEM_CONTENT_UNSET,
   EWE_OBJ_TABS_SUB_ID_ITEM_ICON_SET,
   EWE_OBJ_TABS_SUB_ID_ITEM_ICON_UNSET,
   EWE_OBJ_TABS_SUB_ID_ITEM_BUTTON_SET,
   EWE_OBJ_TABS_SUB_ID_ITEM_BUTTON_UNSET,
   EWE_OBJ_TABS_SUB_ID_ITEM_TITLE_SET,
   EWE_OBJ_TABS_SUB_ID_ITEM_TITLE_GET,
   EWE_OBJ_TABS_SUB_ID_ITEM_DISABLED_SET,
   EWE_OBJ_TABS_SUB_ID_ITEM_DISABLED_GET,
   EWE_OBJ_TABS_SUB_ID_LAST
};

#define EWE_OBJ_TABS_ID(sub_id) (EWE_OBJ_TABS_BASE_ID + sub_id)


#define ewe_obj_tabs_item_append(ret, item, title, style) EWE_OBJ_TABS_ID(EWE_OBJ_TABS_SUB_ID_ITEM_APPEND), EO_TYPECHECK(Ewe_Tabs_Item **, ret), EO_TYPECHECK(Ewe_Tabs_Item *, item), EO_TYPECHECK(Eina_Stringshare *, title), EO_TYPECHECK(Eina_Stringshare *, style)

#define ewe_obj_tabs_item_prepend(ret, item, title, style) EWE_OBJ_TABS_ID(EWE_OBJ_TABS_SUB_ID_ITEM_PREPEND), EO_TYPECHECK(Ewe_Tabs_Item **, ret), EO_TYPECHECK(Ewe_Tabs_Item *, item), EO_TYPECHECK(Eina_Stringshare *, title), EO_TYPECHECK(Eina_Stringshare *, style)

#define ewe_obj_tabs_item_del(ret, item) EWE_OBJ_TABS_ID(EWE_OBJ_TABS_SUB_ID_ITEM_DEL), EO_TYPECHECK(Eina_Bool *, ret), EO_TYPECHECK(Ewe_Tabs_Item *, item)

#define ewe_obj_tabs_items_list_get(ret) EWE_OBJ_TABS_ID(EWE_OBJ_TABS_SUB_ID_ITEMS_LIST_GET), EO_TYPECHECK(const Eina_List **, ret)

#define ewe_obj_tabs_item_activate(ret, item) EWE_OBJ_TABS_ID(EWE_OBJ_TABS_SUB_ID_ITEM_ACTIVATE), EO_TYPECHECK(Eina_Bool *, ret), EO_TYPECHECK(Ewe_Tabs_Item *, item)

#define ewe_obj_tabs_item_content_set(ret, item, content) EWE_OBJ_TABS_ID(EWE_OBJ_TABS_SUB_ID_ITEM_CONTENT_SET), EO_TYPECHECK(Eina_Bool *, ret), EO_TYPECHECK(Ewe_Tabs_Item *, item), EO_TYPECHECK(Evas_Object *, content)

#define ewe_obj_tabs_item_content_unset(ret, item) EWE_OBJ_TABS_ID(EWE_OBJ_TABS_SUB_ID_ITEM_CONTENT_UNSET), EO_TYPECHECK(Evas_Object **, ret), EO_TYPECHECK(Ewe_Tabs_Item *, item)

#define ewe_obj_tabs_item_icon_set(ret, item, icon) EWE_OBJ_TABS_ID(EWE_OBJ_TABS_SUB_ID_ITEM_ICON_SET), EO_TYPECHECK(Eina_Bool *, ret), EO_TYPECHECK(Ewe_Tabs_Item *, item), EO_TYPECHECK(Evas_Object *, icon)

#define ewe_obj_tabs_item_icon_unset(ret, item) EWE_OBJ_TABS_ID(EWE_OBJ_TABS_SUB_ID_ITEM_ICON_UNSET), EO_TYPECHECK(Evas_Object **, ret), EO_TYPECHECK(Ewe_Tabs_Item *, item)

#define ewe_obj_tabs_item_button_set(ret, item, button) EWE_OBJ_TABS_ID(EWE_OBJ_TABS_SUB_ID_ITEM_BUTTON_SET), EO_TYPECHECK(Eina_Bool *, ret), EO_TYPECHECK(Ewe_Tabs_Item *, item), EO_TYPECHECK(Evas_Object *, button)

#define ewe_obj_tabs_item_button_unset(ret, item) EWE_OBJ_TABS_ID(EWE_OBJ_TABS_SUB_ID_ITEM_BUTTON_UNSET), EO_TYPECHECK(Evas_Object **, ret), EO_TYPECHECK(Ewe_Tabs_Item *, item)

#define ewe_obj_tabs_item_title_set(ret, item, title) EWE_OBJ_TABS_ID(EWE_OBJ_TABS_SUB_ID_ITEM_TITLE_SET), EO_TYPECHECK(Eina_Bool *, ret), EO_TYPECHECK(Ewe_Tabs_Item *, item), EO_TYPECHECK(Eina_Stringshare *, title)

#define ewe_obj_tabs_item_title_get(ret, item) EWE_OBJ_TABS_ID(EWE_OBJ_TABS_SUB_ID_ITEM_TITLE_GET), EO_TYPECHECK(Eina_Stringshare **, ret), EO_TYPECHECK(const Ewe_Tabs_Item *, item)

#define ewe_obj_tabs_item_disabled_set(ret, item, disabled) EWE_OBJ_TABS_ID(EWE_OBJ_TABS_SUB_ID_ITEM_DISABLED_SET), EO_TYPECHECK(Eina_Bool *, ret), EO_TYPECHECK(Ewe_Tabs_Item *, item), EO_TYPECHECK(Eina_Bool, disabled)

#define ewe_obj_tabs_item_disabled_get(ret, item) EWE_OBJ_TABS_ID(EWE_OBJ_TABS_SUB_ID_ITEM_DISABLED_GET), EO_TYPECHECK(Eina_Bool *, ret), EO_TYPECHECK(const Ewe_Tabs_Item *, item)
#endif

#endif /* EWE_TABS_EO_H */
