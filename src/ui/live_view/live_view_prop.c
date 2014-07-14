#include "live_view_prop.h"

#define PROP_DATA_GET(ret) \
   Prop_Data *pd = evas_object_data_get(property, PROP_DATA); \
   if (!pd) \
     { \
        ERR("Can not show group data, because object[%p] is not a property object", property); \
        return ret; \
     }

struct _Prop_Data
{
   Evas_Object *live_object;
   Evas_Object *visual;
   Evas_Object *scale_spinner;
   double current_scale;
   const char *widget;
   Style *style;
   struct {
      Evas_Object *frame;
      Evas_Object *swallows;
   } prop_swallow;
   struct {
      Evas_Object *frame;
      Evas_Object *texts;
   } prop_text;
};
typedef struct _Prop_Data Prop_Data;

Eina_Bool
live_view_property_style_set(Evas_Object *property __UNUSED__,
                             Evas_Object *object __UNUSED__,
                             Style *style __UNUSED__,
                             const char *widget __UNUSED__)
{

   return true;
}

Evas_Object *
live_view_property_add(Evas_Object *parent __UNUSED__)
{

   return NULL;
}

Eina_Bool
live_view_property_style_unset(Evas_Object *property __UNUSED__)
{

   return true;
}
