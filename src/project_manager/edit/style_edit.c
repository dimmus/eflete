#include "style_edit.h"

Eina_List *
style_edit_styles_list_get(Project *project)
{
   Evas_Object *edje_obj;
   GET_OBJ(project, edje_obj);

   return edje_edit_styles_list_get(edje_obj);
}

Eina_Bool
style_edit_style_add(Project *project, const char *style)
{
    Evas_Object *edje_obj;
    GET_OBJ(project, edje_obj);

    return edje_edit_style_add(edje_obj, style);
}

void
style_edit_style_del(Project *project, const char *style)
{
    Evas_Object *edje_obj;
    GET_OBJ(project, edje_obj);

    edje_edit_style_del(edje_obj, style);
}

Eina_List *
style_edit_style_tags_list_get(Project *project, const char *style)
{
   Evas_Object *edje_obj;
   GET_OBJ(project, edje_obj);

   return edje_edit_style_tags_list_get(edje_obj, style);
}

Eina_Bool
style_edit_style_tag_add(Project *project,
                         const char *style,
                         const char *tag_name,
                         const char *tag_value)
{
    Evas_Object *edje_obj;
    Eina_Bool res;
    GET_OBJ(project, edje_obj);

    res = edje_edit_style_tag_add(edje_obj, style, tag_name);
    if (res) edje_edit_style_tag_value_set(edje_obj, style, tag_name, tag_value);
    return res;
}

void
style_edit_style_tag_del(Project *project,
                         const char *style,
                         const char *tag_name)
{
    Evas_Object *edje_obj;
    GET_OBJ(project, edje_obj);

    edje_edit_style_tag_del(edje_obj, style, tag_name);
}

const char*
style_edit_style_tag_value_get(Project *project,
                               const char *style,
                               const char *tag_name)
{
    Evas_Object *edje_obj;
    GET_OBJ(project, edje_obj);

    return edje_edit_style_tag_value_get(edje_obj, style, tag_name);
}

void
style_edit_style_tag_value_set(Project *project,
                               const char* style,
                               const char* tag_name,
                               const char* new_value)
{
    Evas_Object *edje_obj;
    GET_OBJ(project, edje_obj);

    edje_edit_style_tag_value_set(edje_obj, style, tag_name, new_value);
}

Eina_Bool
style_edit_save(Project *project)
{
   Evas_Object *edje_obj;
   GET_OBJ(project, edje_obj);

   return edje_edit_save(edje_obj);
}
