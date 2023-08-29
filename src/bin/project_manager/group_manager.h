#ifndef GROUP_MANAGER_H
#define GROUP_MANAGER_H

#include "eflete.h"

void
gm_group_edit_object_load(Project *pro, Group *group, Evas *e);

void
gm_group_edit_object_reload(Project *pro, Group *group);

void
gm_group_edit_object_unload(Group *group);

Eina_List *
gm_group_used_groups_edj_get(Evas_Object *obj);

Eina_List *
gm_group_used_images_edj_get(Evas_Object *obj);

Eina_List *
gm_group_used_sample_edj_get(Evas_Object *obj);

Eina_List *
gm_group_used_color_classes_edj_get(Evas_Object *obj);

Eina_List *
gm_group_used_styles_edj_get(Evas_Object *obj);

Eina_List *
gm_group_used_fonts_edj_get(Evas_Object *obj);

#endif /* GROUP_MANAGER_H */
