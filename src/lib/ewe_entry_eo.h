#ifndef EWE_ENTRY_EO_H
#define EWE_ENTRY_EO_H

#include "ewe_entry.eo.h"

#if 0

#define EWE_OBJ_ENTRY_CLASS ewe_obj_entry_class_get()

const Eo_Class *ewe_obj_entry_class_get(void) EINA_CONST;

extern EAPI Eo_Op EWE_OBJ_ENTRY_BASE_ID;
#define EWE_OBJ_ENTRY_ID(sub_id) (EWE_OBJ_ENTRY_BASE_ID + sub_id)

enum
{
   EWE_OBJ_ENTRY_SUB_ID_LABEL_TEXT_SET,
   EWE_OBJ_ENTRY_SUB_ID_LABEL_TEXT_GET,
   EWE_OBJ_ENTRY_SUB_ID_LABEL_VISIBLE_SET,
   EWE_OBJ_ENTRY_SUB_ID_LABEL_VISIBLE_GET,
   EWE_OBJ_ENTRY_SUB_ID_REGEX_SET,
   EWE_OBJ_ENTRY_SUB_ID_REGEX_UNSET,
   EWE_OBJ_ENTRY_SUB_ID_REGEX_CHECK,

   EWE_OBJ_ENTRY_SUB_ID_LAST
};

#define ewe_obj_entry_label_text_set(label_text) EWE_OBJ_ENTRY_ID(EWE_OBJ_ENTRY_SUB_ID_LABEL_TEXT_SET), EO_TYPECHECK(const char *, label_text)
#define ewe_obj_entry_label_text_get(ret) EWE_OBJ_ENTRY_ID(EWE_OBJ_ENTRY_SUB_ID_LABEL_TEXT_GET), EO_TYPECHECK(const char **, ret)

#define ewe_obj_entry_label_visible_set(visible) EWE_OBJ_ENTRY_ID(EWE_OBJ_ENTRY_SUB_ID_LABEL_VISIBLE_SET), EO_TYPECHECK(Eina_Bool, visible)
#define ewe_obj_entry_label_visible_get(ret) EWE_OBJ_ENTRY_ID(EWE_OBJ_ENTRY_SUB_ID_LABEL_VISIBLE_GET), EO_TYPECHECK(Eina_Bool *, ret)

#define ewe_obj_entry_regex_set(regex_str, flags, ret) EWE_OBJ_ENTRY_ID(EWE_OBJ_ENTRY_SUB_ID_REGEX_SET), EO_TYPECHECK(const char *, regex_str), EO_TYPECHECK(int, flags), EO_TYPECHECK(Eina_Bool *, ret)
#define ewe_obj_entry_regex_unset(ret) EWE_OBJ_ENTRY_ID(EWE_OBJ_ENTRY_SUB_ID_REGEX_UNSET), EO_TYPECHECK(Eina_Bool *, ret)
#define ewe_obj_entry_regex_check(ret) EWE_OBJ_ENTRY_ID(EWE_OBJ_ENTRY_SUB_ID_REGEX_CHECK), EO_TYPECHECK(Eina_Bool *, ret)

#endif

#endif /* EWE_ENTRY_EO_H */
