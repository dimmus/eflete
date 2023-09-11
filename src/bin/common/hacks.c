#include "eflete.h"

static int _spinner_changed_from_code = 0;
static void
_spinner_changed_hack(void *data EINA_UNUSED, Evas_Object *obj, void *ei)
{
   /* don't call callback if we are setting value to spinner from code */
   if (!_spinner_changed_from_code)
      evas_object_smart_callback_call(obj, signals.elm.spinner.changed_user, ei);
}

void
hack_spinner_value_set(Evas_Object *spinner, double val)
{
   assert(spinner != NULL);

   _spinner_changed_from_code++;
   _elm_spinner_value_set(spinner, val);
   _spinner_changed_from_code--;
}

Evas_Object *
hack_spinner_add(Evas_Object *parent)
{
   Evas_Object *ret = _elm_spinner_add(parent);
   evas_object_smart_callback_add(ret, signals.elm.spinner.changed, _spinner_changed_hack, NULL);
   return ret;
}
