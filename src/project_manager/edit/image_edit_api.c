#include "image_edit_api.h"

Eina_List *
image_edit_images_list_get(Project *project)
{
   Evas_Object *edje_obj;
   GET_OBJ(project, edje_obj);

   return edje_edit_images_list_get(edje_obj);
}

EAPI int
image_edit_image_id_get (Project *project, const char *image_name)
{
   Evas_Object *edje_obj;
   GET_OBJ(project, edje_obj);

   return edje_edit_image_id_get(edje_obj,image_name);
}

EAPI Edje_Edit_Image_Comp
image_edit_image_compression_type_get (Project *project, const char *image)
{
   Evas_Object *edje_obj;
   GET_OBJ(project,edje_obj);

   return edje_edit_image_compression_type_get (edje_obj, image);
}

EAPI int
image_edit_image_compression_rate_get  (  Project *project, const char *   image )
{
   Evas_Object *edje_obj;
   GET_OBJ(project,edje_obj);

   return edje_edit_image_compression_rate_get(edje_obj,image);
}
