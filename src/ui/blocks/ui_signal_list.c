#include "ui_signal_list.h"

#define SIGNALS_LIST "signals_list"

static Elm_Genlist_Item_Class *_itc_signal = NULL;

static char *
_item_signal_label_get(void *data,
                       Evas_Object *obj __UNUSED__,
                       const char *part __UNUSED__)
{
   return strdup(data);
}

static void
_on_signal_selected(void *data __UNUSED__,
                    Evas_Object *obj,
                    void *event_info)
{
   Elm_Object_Item *glit = (Elm_Object_Item *)event_info;
   evas_object_smart_callback_call(obj, "sl,signal,select", glit);
}

Evas_Object *
ui_signal_list_add(Evas_Object *parent)
{
   Evas_Object *gl_signals;

   if (!parent) return NULL;

   if (!_itc_signal)
     {
        _itc_signal = elm_genlist_item_class_new();
        _itc_signal->item_style = "default";
        _itc_signal->func.text_get = _item_signal_label_get;
        _itc_signal->func.content_get = NULL;
        _itc_signal->func.state_get = NULL;
        _itc_signal->func.del = NULL;
     }

   gl_signals = elm_genlist_add(parent);
   evas_object_size_hint_align_set(gl_signals, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(gl_signals, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_smart_callback_add(gl_signals, "selected",
                                 _on_signal_selected, NULL);

   return gl_signals;
}

Eina_Bool
ui_signal_list_data_set(Evas_Object *object, Group *group)
{
   Eina_List *signals, *l;
   char *signal;
   Evas_Object *gl_signals = (Evas_Object *)object;

   if ((!object) || (!group)) return EINA_FALSE;

   signals = evas_object_data_get(gl_signals, SIGNALS_LIST);
   if (signals)
     {
        wm_program_signals_list_free(signals);
        evas_object_data_del(gl_signals, SIGNALS_LIST);
     }

   signals = wm_program_signals_list_get(group);
   EINA_LIST_FOREACH(signals, l, signal)
     {
        elm_genlist_item_append(object, _itc_signal,
                                signal,
                                NULL, ELM_GENLIST_ITEM_NONE,
                                NULL, NULL);
     }
   evas_object_data_set(gl_signals, SIGNALS_LIST, signals);

   return EINA_TRUE;
}

void
ui_signal_list_callback_add(Evas_Object *object,
                            const char *event,
                            Edje_Signal_Cb func,
                            void *data)
{
   if (!object) return;
   elm_object_signal_callback_add(object, event, "", func, data);
}
