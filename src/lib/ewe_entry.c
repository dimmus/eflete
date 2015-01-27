#include "ewe_private.h"
#include "ewe_widget_entry.h"

#define MY_CLASS EWE_ENTRY_CLASS

#define MY_CLASS_NAME "Ewe_Entry"
#define MY_CLASS_NAME_LEGACY "ewe_entry"

#define _ERR_STR_LEN 48

EAPI Evas_Object *
ewe_entry_add(Evas_Object *parent)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(parent, NULL);
   Evas_Object *obj = eo_add(MY_CLASS, parent);
   return obj;
}

static void
_key_down_cb(void *data,
               Evas *evas EINA_UNUSED,
               Evas_Object *obj EINA_UNUSED,
               void *event_info)
{
   Evas_Event_Key_Down *ev = event_info;
   EWE_ENTRY_PARENT_DATA_GET(data, parent_sd);

   if (!strcmp(ev->key, "Return") && (parent_sd->single_line))
     {
        Eo * parent = NULL;
        eo_do(data, parent = eo_parent_get());
        elm_object_focus_next(parent, ELM_FOCUS_NEXT);
     }
}

static void
_focus_out(void *data,
           Evas *evas EINA_UNUSED,
           Evas_Object *obj EINA_UNUSED,
           void *einfo EINA_UNUSED)
{
   EWE_ENTRY_PARENT_DATA_GET(data, parent_sd);
   EWE_ENTRY_DATA_GET(data);
   if (!sd->regex_error)
     edje_object_signal_emit(parent_sd->scr_edje,
        "ewe,entry,regex,check_off", "ewe");
}

static void
_on_rejected(void *data,
             Evas *evas EINA_UNUSED,
             void *einfo EINA_UNUSED)
{
   EWE_ENTRY_PARENT_DATA_GET(data, parent_sd);
   edje_object_signal_emit(parent_sd->scr_edje,
      "ewe,entry,markup,error", "ewe");
}

static void
_entry_changed_signal_cb(void *data,
                         Evas_Object *obj EINA_UNUSED,
                         const char *emission EINA_UNUSED,
                         const char *source EINA_UNUSED)
{
   EWE_ENTRY_PARENT_DATA_GET(data, parent_sd);
   EWE_ENTRY_DATA_GET(data);
   if (parent_sd->single_line)
     {
        char *entry =
                 elm_entry_markup_to_utf8(edje_object_part_text_get(parent_sd->entry_edje,
                    "elm.text"));

        unsigned int i = 0;
        Eina_Bool is_changed_flag = EINA_FALSE;
        while(entry[i] != '\0')
          {
             if (entry[i] == '\n')
               {
                  entry[i] = ' ';
                  if (!is_changed_flag) is_changed_flag = EINA_TRUE;
               }
             i++;
          }

        if (is_changed_flag) ewe_entry_entry_set(data, entry);
        free(entry);
     }
   if (sd->regex_autocheck && evas_object_focus_get(parent_sd->entry_edje))
     ewe_entry_regex_check(data);
}

EOLIAN static void
_ewe_entry_class_constructor(Eo_Class *klass)
{
   evas_smart_legacy_type_register(MY_CLASS_NAME_LEGACY, klass);
}

EOLIAN static void
_ewe_entry_eo_base_constructor(Eo *obj, Ewe_Entry_Data *_pd)
{
   EWE_ENTRY_PARENT_DATA_GET(obj, parent_sd);

   eo_do_super(obj, MY_CLASS, eo_constructor());
   eo_do(obj, evas_obj_type_set(MY_CLASS_NAME_LEGACY));

   _pd->emit_regex_signals  = EINA_FALSE;
   _pd->label_visible = EINA_FALSE;
   _pd->regex_autocheck = EINA_FALSE;
   _pd->regex = NULL;
   _pd->regex_error = EWE_REG_NOERROR;
   _pd->regex_error_str = malloc(sizeof(char)*_ERR_STR_LEN);

   evas_object_event_callback_add(parent_sd->entry_edje, EVAS_CALLBACK_FOCUS_OUT, _focus_out, obj);
   evas_object_event_callback_add(parent_sd->entry_edje, EVAS_CALLBACK_KEY_DOWN, _key_down_cb, obj);
   evas_object_smart_callback_add(obj, "rejected", _on_rejected, obj);
   edje_object_signal_callback_add(parent_sd->entry_edje,
      "entry,changed", "elm.text", _entry_changed_signal_cb, obj);
}

EOLIAN static void
_ewe_entry_eo_base_destructor(Eo *obj, Ewe_Entry_Data *_pd)
{
   free(_pd->regex_error_str);
   ewe_entry_regex_unset(obj);
   eo_do_super(obj, ELM_ENTRY_CLASS, eo_destructor());
}

static void
_process_regexp_error(const Evas_Object *obj, const int regex_error)
{
   EWE_ENTRY_PARENT_DATA_GET(obj, parent_sd);
   if (!parent_sd->scr_edje)
     return;

   if (regex_error)
     edje_object_signal_emit(parent_sd->scr_edje,
        "ewe,entry,regex,error", "ewe");
   else if (evas_object_focus_get(parent_sd->entry_edje))
     edje_object_signal_emit(parent_sd->scr_edje,
        "ewe,entry,regex,valid", "ewe");
   else
     edje_object_signal_emit(parent_sd->scr_edje,
        "ewe,entry,regex,check_off", "ewe");
}

EOLIAN Eina_Bool
_ewe_entry_regex_set(Eo *obj, Ewe_Entry_Data *sd, const char *regex_str, int flags)
{
   if (!obj) return EINA_FALSE;

   if ((!regex_str) || (flags > 3 || flags < 0))
     return EINA_FALSE;

   if (sd->regex)
     regfree(sd->regex);
   if (!sd->regex)
     sd->regex = malloc(sizeof(regex_t));

   sd->regex_error = regcomp(sd->regex, regex_str, flags);
   regerror(sd->regex_error, sd->regex, sd->regex_error_str, _ERR_STR_LEN);

   if (sd->emit_regex_signals)
     _process_regexp_error(obj, sd->regex_error);

   return !sd->regex_error;
}

EOLIAN Eina_Bool
_ewe_entry_regex_unset(Eo *obj, Ewe_Entry_Data *sd)
{
   if (!obj) return EINA_FALSE;

   if (sd->regex)
     {
        regfree(sd->regex);
        free(sd->regex);
     }
   else
     return EINA_FALSE;

   sd->regex = NULL;
   sd->regex_error = 0;

   return EINA_TRUE;
}

EOLIAN Eina_Bool
_ewe_entry_regex_check(Eo *obj, Ewe_Entry_Data *sd)
{
   EWE_ENTRY_PARENT_DATA_GET(obj, parent_sd);

   if ((!obj) || (!sd->regex)) return EINA_FALSE;

   const char *entry = edje_object_part_text_get(parent_sd->entry_edje,
                          "elm.text");
   if (!entry) return EINA_FALSE;

   sd->regex_error = regexec(sd->regex, entry, (size_t)0, NULL, 0);
   regerror(sd->regex_error, sd->regex, sd->regex_error_str, _ERR_STR_LEN);
   if (sd->emit_regex_signals)_process_regexp_error(obj, sd->regex_error);

   return !sd->regex_error;
}

EOLIAN Eina_Bool
_ewe_entry_regex_glow_get(Eo *obj, Ewe_Entry_Data *sd)
{
   if (!obj) return EINA_FALSE;
   return sd->emit_regex_signals;
}

EOLIAN Eina_Bool
_ewe_entry_regex_glow_set(Eo *obj, Ewe_Entry_Data *sd, Eina_Bool glow)
{
   if (!obj) return EINA_FALSE;
   EWE_ENTRY_PARENT_DATA_GET(obj, parent_sd);

   sd->emit_regex_signals = glow;
   if (!glow) edje_object_signal_emit(parent_sd->scr_edje,
                 "ewe,entry,regex,check_off", "ewe");
   return EINA_TRUE;
}

EOLIAN const char *
_ewe_entry_regex_error_text_get(Eo *obj, Ewe_Entry_Data *sd)
{
   if (!obj) return NULL;
   if (sd->regex)
     return sd->regex_error_str;
   return NULL;
}

EOLIAN Eina_Bool
_ewe_entry_regex_autocheck_get(Eo *obj EINA_UNUSED, Ewe_Entry_Data *sd)
{
   return sd->regex_autocheck;
}

EOLIAN Eina_Bool
_ewe_entry_regex_autocheck_set(Eo *obj, Ewe_Entry_Data *sd, Eina_Bool autocheck)
{
   if (!obj) return EINA_FALSE;
   if (sd->regex_autocheck == autocheck)
     return EINA_TRUE;
   EWE_ENTRY_PARENT_DATA_GET(obj, parent_sd);

   sd->regex_autocheck = autocheck;
   if (!sd->regex_autocheck)
     edje_object_signal_emit(parent_sd->scr_edje,
        "ewe,entry,regex,check_off", "ewe");
   return EINA_TRUE;
}

EOLIAN int
_ewe_entry_regex_error_get(Eo *obj, Ewe_Entry_Data *sd)
{
   if (!obj) return EWE_REG_NOERROR;
   return sd->regex_error;
}

EOLIAN static Eina_Bool
_ewe_entry_elm_widget_theme_apply(Eo *obj, Ewe_Entry_Data *sd EINA_UNUSED)
{
   EWE_ENTRY_PARENT_DATA_GET(obj, parent_sd);
   if (parent_sd->scroll)
     {
        if (!parent_sd->single_line)
          edje_object_file_set(parent_sd->scr_edje, EWE_THEME,
             "ewe/scroller/entry/default");
        else
          edje_object_file_set(parent_sd->scr_edje, EWE_THEME,
             "ewe/scroller/entry_single/default");
     }

   if (parent_sd->single_line)
     edje_object_file_set(parent_sd->entry_edje, EWE_THEME,
        "ewe/entry/base_single_line/default");
   else
     {
        switch (parent_sd->line_wrap)
          {
           case ELM_WRAP_MIXED:
             {
                edje_object_file_set(parent_sd->entry_edje, EWE_THEME,
                   "ewe/entry/base/default");
                break;
             }
           case ELM_WRAP_NONE:
             {
                edje_object_file_set(parent_sd->entry_edje, EWE_THEME,
                   "ewe/entry/base_nowrap/default");
                break;
             }
           case ELM_WRAP_CHAR:
             {
                edje_object_file_set(parent_sd->entry_edje, EWE_THEME,
                   "ewe/entry/base_char_wrap/default");
                break;
             }
           case ELM_WRAP_WORD:
             {
                edje_object_file_set(parent_sd->entry_edje, EWE_THEME,
                   "ewe/entry/base_word_wrap/default");
                break;
             }
           case ELM_WRAP_LAST:
             break;
        }
     }
   ewe_entry_entry_set(obj, "");
   return EINA_TRUE;
}

EOLIAN static Eina_Bool
_ewe_entry_elm_widget_disable(Eo *obj, Ewe_Entry_Data *_pd EINA_UNUSED)
{
   EWE_ENTRY_PARENT_DATA_GET(obj, parent_sd);
   if (elm_object_disabled_get(obj))
     {
        edje_object_signal_emit(parent_sd->entry_edje, "elm,state,disabled", "elm");
        edje_object_signal_emit(parent_sd->scr_edje, "elm,state,disabled", "elm");
     }
   else
     {
        edje_object_signal_emit(parent_sd->entry_edje, "elm,state,enabled", "elm");
        edje_object_signal_emit(parent_sd->scr_edje, "elm,state,enabled", "elm");
     }

   return EINA_TRUE;
}

EAPI void
ewe_entry_entry_set(Evas_Object *obj, const char *text)
{
   EWE_ENTRY_CHECK(obj);

   eo_do(obj, elm_obj_layout_text_set(NULL, text));
   eo_do(obj, ewe_obj_entry_regex_check());
}

#include "ewe_entry.eo.c"
#undef _ERR_STR_LEN
