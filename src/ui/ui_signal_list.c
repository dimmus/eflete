#include "ui_signal_list.h"

static Elm_Genlist_Item_Class *_itc_signal = NULL;

static char *
_item_signal_label_get(void *data,
                       Evas_Object *obj __UNUSED__,
                       const char *part __UNUSED__)
{
   return strdup(data);
}

Evas_Object *
ui_signal_list_add(App_Data *ap, Eina_List *signals)
{
   Evas_Object *gl_signals;
   Eina_List *l;
   char *signal;

   if(!ap || !signals) return NULL;

   if(!_itc_signal)
     {
        _itc_signal = elm_genlist_item_class_new();
        _itc_signal->item_style = "default";
        _itc_signal->func.text_get = _item_signal_label_get;
        _itc_signal->func.content_get = NULL;
        _itc_signal->func.state_get = NULL;
        _itc_signal->func.del = NULL;
     }

   gl_signals = elm_genlist_add(ap->win);
   evas_object_size_hint_align_set(gl_signals,
                                   EVAS_HINT_FILL,
                                   EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(gl_signals,
                                    EVAS_HINT_EXPAND,
                                    EVAS_HINT_EXPAND);

   EINA_LIST_FOREACH(signals, l, signal)
     {
        elm_genlist_item_append(gl_signals, _itc_signal,
                                signal,
                                NULL, ELM_GENLIST_ITEM_NONE,
                                NULL, NULL);
     }

   return gl_signals;
}
