
#ifndef EWE_RULER_COMMON_H
#define EWE_RULER_COMMON_H

typedef struct _Ewe_Ruler_Scale Ewe_Ruler_Scale;
struct _Ewe_Ruler_Scale
{
   Evas_Object       *box;
   Eina_List         *dashes;
   Eina_List         *markers;
   Eina_Stringshare  *format;
   Eina_Stringshare  *style;
   Eina_Stringshare  *full_style;
   double            mark_value_step;
   int               mark_step;
   int               zero_pos;
   Eina_Bool         visible : 1;
   Eina_Bool         show_middle_mark : 1;
};

typedef struct _Ewe_Ruler_Marker Ewe_Ruler_Marker;
struct _Ewe_Ruler_Marker
{
   Evas_Object       *obj;
   Ewe_Ruler_Scale   *scale;
   Eina_Stringshare  *style;
   double            rel_position;
   int               abs_position;
   int               size;
   Eina_Bool         visible : 1;
   Eina_Bool         relative : 1;
};

#endif /* EWE_RULER_COMMON_H */
