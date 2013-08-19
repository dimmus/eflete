#include "image_edit_api.h"

Eina_List *
colorclasses_edit_list_get(Project *project)
{
   Evas_Object *edje_obj;
   GET_OBJ(project, edje_obj);

   return edje_edit_color_classes_list_get(edje_obj);
}

EAPI Eina_Bool
colorclasses_edit_colors_get (Project *project,
   const char *class_name, int *r, int *g, int *b, int *a,
   int *r2, int *g2, int *b2, int *a2, int *r3, int *g3, int *b3, int *a3)
{
   Evas_Object *edje_obj;
   GET_OBJ(project, edje_obj);

   return edje_edit_color_class_colors_get (edje_obj, class_name,
      r, g, b, a, r2, g2, b2, a2, r3, g3, b3, a3);
}

EAPI Eina_Bool
colorclasses_edit_colors_set(Project *project,
const char *class_name, int	r, int g, int b, int a, int r2, int g2, int b2,
int a2, int r3, int g3, int b3, int a3)
{
   Evas_Object *edje_obj;
   GET_OBJ(project, edje_obj);

   return edje_edit_color_class_colors_set (edje_obj, class_name,
      r, g, b, a, r2, g2, b2, a2, r3, g3, b3, a3);
}

EAPI Eina_Bool
colorclass_edit_add (Project *project, const char *name)
{
   Evas_Object *edje_obj;
   GET_OBJ(project, edje_obj);

   return edje_edit_color_class_add(edje_obj,name);
}

EAPI Eina_Bool
colorclass_edit_del (Project *project, const char *name)
{
   Evas_Object *edje_obj;
   GET_OBJ(project, edje_obj);

   return edje_edit_color_class_del(edje_obj,name);
}

