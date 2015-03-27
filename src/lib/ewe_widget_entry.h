#ifndef EWE_WIDGET_ENTRY_H
#define EWE_WIDGET_ENTRY_H

#include <regex.h>
#include <elm_widget_entry.h>

typedef struct _Ewe_Entry_Data        Ewe_Entry_Data;
struct _Ewe_Entry_Data {
   Eina_Bool                             label_visible : 1;
   Eina_Bool                             emit_regex_signals : 1;
   Eina_Bool                             regex_autocheck: 1;
   regex_t                              *regex;
   int                                   regex_error;
   char *                                regex_error_str;
};

#define EWE_ENTRY_CHECK(obj) \
  if (!obj || EINA_UNLIKELY(!eo_isa((obj), EWE_ENTRY_CLASS))) \
    return

#define EWE_ENTRY_PARENT_DATA_GET(obj, wd) \
   Elm_Entry_Data *wd = eo_data_scope_get(obj, ELM_ENTRY_CLASS)

#define EWE_ENTRY_DATA_GET(obj) \
   Ewe_Entry_Data *sd = eo_data_scope_get(obj, EWE_ENTRY_CLASS)

#endif /* EWE_WIDGET_ENTRY_H */
