#ifndef EWE_WIDGET_RULER_H
#define EWE_WIDGET_RULER_H

#include "ewe_ruler.h"

typedef struct _Ewe_Ruler_Smart_Data Ewe_Ruler_Smart_Data;
struct _Ewe_Ruler_Smart_Data
{
   Eo                *obj;
   Evas_Object       *clip;
   Evas_Object       *bg;
   Eina_List         *scales;
   Eina_List         *markers;
   struct {
      Evas_Coord     width;
      Evas_Coord     height;
      Evas_Coord     x;
      Evas_Coord     y;
   } geometry;
   Eina_Bool         size_changed : 1;
   Eina_Bool         position_changed : 1;
   Eina_Bool         text_changed : 1;
   Eina_Bool         horizontal : 1;
   Eina_Bool         ruler_visible : 1;
};

#define EWE_RULER_DATA_GET(o, sd) \
  Ewe_Ruler_Smart_Data * sd = eo_data_scope_get(o, EWE_RULER_CLASS)

#define EWE_RULER_DATA_GET_OR_RETURN(o, ptr) \
  EWE_RULER_DATA_GET(o, ptr); \
  if (EINA_UNLIKELY(!ptr)) \
    { \
       CRI("No widget data for object %p (%s)", \
           o, evas_object_type_get(o)); \
       return; \
    }

#define EWE_RULER_DATA_GET_OR_RETURN_VAL(o, ptr, val) \
  EWE_RULER_DATA_GET(o, ptr); \
  if (EINA_UNLIKELY(!ptr)) \
    { \
       CRI("No widget data for object %p (%s)", \
           o, evas_object_type_get(o)); \
       return val; \
    }

#define EWE_RULER_CHECK(obj) \
  if (EINA_UNLIKELY(!eo_isa((obj), EWE_RULER_CLASS))) \
    return

#endif /* EWE_WIDGET_RULER_H */
