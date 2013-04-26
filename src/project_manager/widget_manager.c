#include "widget_manager.h"

#define WM_WIDGET_NAME_GET(widget, group) \
	char **arr; \
	arr = eina_str_split(group, "/", 3); \
	widget = strdup(arr[1]); \
	free(arr);

#define WM_CLASS_NAME_GET(class, group) \
	char **arr; \
	arr = eina_str_split(group, "/", 4); \
	class = strdup(arr[2]); \
	free(arr);

#define WM_STYLE_NAME_GET(style, group) \
	char **arr; \
	arr = eina_str_split(group, "/", 5); \
	style = strdup(arr[3]);\
	free(arr);

const char empty[] = "\0";

Eina_List *
wm_styles_list_free(Eina_List *list)
{
	char *data;

	EINA_LIST_FREE(list, data)
		free(data);

	list = NULL;

	return list;
}

Part *
wm_part_add(const Evas_Object *obj, const char *part)
{
	Part *part_edje;

	if(!obj | !part)
		return NULL;

	part_edje = calloc(1, sizeof(*part_edje));
	part_edje->part_name = strdup(part);
	part_edje->show = EINA_TRUE;

	return part_edje;
}

Eina_Bool
wm_part_free(Part *part)
{
	if(!part)
		return EINA_FALSE;

	if(	evas_object_ref_get(part->obj) != 0)
		evas_object_unref(part->obj);
	evas_object_del(part->obj);

	free(part->part_name);
	free(part);

	return EINA_TRUE;
}

Group *
wm_group_add(const char *group, const char *full_name_group)
{
	Group *group_edje;

	if(!group | !full_name_group)
		return NULL;

	group_edje = calloc(1, sizeof(*group_edje));
	group_edje->group_name = strdup(group);
	group_edje->full_group_name = strdup(full_name_group);
	group_edje->obj = NULL;
	group_edje->show = EINA_TRUE;
	group_edje->parts = NULL;

	return group_edje;
}

Eina_Bool
wm_group_free(Group *group)
{
	Part *part;
	Eina_Inlist *list;

	if(!group)
		return EINA_FALSE;

	if(	evas_object_ref_get(group->obj) != 0)
		evas_object_unref(group->obj);
	evas_object_del(group->obj);

	list = group->parts;
	if(list)
	{
		while(list)
		{
			part = EINA_INLIST_CONTAINER_GET(list, Part);
			list = eina_inlist_remove(list, list);
			wm_part_free(part);
		}
	}

	free(part->part_name);
	free(part);

	return EINA_TRUE;

}

WClass *
wm_wclass_add(const char *wclass, Eina_List *groups)
{
	WClass *class_edje;
	Eina_List *l;
	Group *group_edje;
	char *style_name, *group_name_full;

	if(!wclass || !groups)
		return NULL;

	class_edje = calloc(1, sizeof(*class_edje));
	class_edje->class_name = strdup(wclass);
	class_edje->groups = NULL;

	EINA_LIST_FOREACH(groups, l, group_name_full)
	{
		WM_STYLE_NAME_GET(style_name, group_name_full);
		group_edje = wm_group_add(style_name, group_name_full);
		class_edje->groups = eina_inlist_append(class_edje->groups,
							EINA_INLIST_GET(group_edje));
	}
	return class_edje;
}

Eina_Bool
wm_wclass_free(WClass *wclass)
{
	Group *group_edje;
	Eina_Inlist *list;

	if(!wclass)
		return EINA_FALSE;

	list = wclass->groups;

	if(list)
	{
		while(list)
		{
			group_edje = EINA_INLIST_CONTAINER_GET(list, Group);
			list = eina_inlist_remove(list, list);
			wm_group_free(group_edje);
		}
	}

	free(wclass->class_name);
	free(wclass);

	return EINA_TRUE;
}

Widget *
wm_widget_add(const char *widget, Eina_List *widget_styles)
{
	Widget *_widget = NULL;
	Eina_List *l, *l_next;
	Eina_List *groups = NULL;
	WClass *wclass;
	char *wclass_name, *wclass_name_next;
	char *group, *group_next;

	if(!widget)
		return NULL;

	_widget = calloc(1, sizeof(*_widget));
	_widget->widget_name = strdup(widget);
	_widget->classes = NULL;

	EINA_LIST_FOREACH_SAFE(widget_styles, l, l_next, group)
	{
		WM_CLASS_NAME_GET(wclass_name, group);
		if(l_next)
		{
			group_next = eina_list_data_get(l_next);
			WM_CLASS_NAME_GET(wclass_name_next, group_next);
		}
		else
			wclass_name_next = &empty;

		groups = eina_list_append(groups, strdup(group));

		if(strcmp(wclass_name, wclass_name_next) != 0)
		{
			wclass = wm_wclass_add(wclass_name, groups);
			_widget->classes = eina_inlist_append(_widget->classes,
							EINA_INLIST_GET(wclass));
			groups = wm_styles_list_free(groups);
		}
	}
	return _widget;
}

Eina_Bool
wm_widget_free(Widget *widget)
{
	WClass *wclass;
	Eina_Inlist  *list;

	if(!widget)
		return EINA_FALSE;

	list = widget->classes;

	if(list)
	{
		while(list)
		{
			wclass = EINA_INLIST_CONTAINER_GET(list, WClass);
			list = eina_inlist_remove(list, list);
			wm_wclass_free(wclass);
		}
	}
	free(widget->widget_name);
	free(widget);

	return EINA_TRUE;
}

int sort_collection_cb(const void *data1, const void *data2)
{
	const char *str1 = data1;
	const char *str2 = data2;

	if(!str1)
		return 1;
	if(!str2)
		return -1;

	return (strcmp(str1, str2));
}

Eina_Inlist *
wm_widget_list_new(const char *file)
{
	Widget *widget;
	Eina_List *collection, *l, *l_next;
	Eina_List *widget_styles = NULL;
	Eina_Inlist *widget_list = NULL;
	char *widget_name, *widget_name_next;
	char *group, *group_next;
	const char prefix[] = "elm/";
	Eina_Error error;

	if(!file)
		return NULL;

	collection = edje_file_collection_list(file);
	if(!collection)
	{
		error = eina_error_get();
		ERR(eina_error_msg_get(error));
	}

	collection = eina_list_sort(collection,
								eina_list_count(collection),
								sort_collection_cb);

	EINA_LIST_FOREACH_SAFE(collection, l, l_next, group)
	{
		if(!eina_str_has_prefix(group, prefix))
			ERR("Invalid style name in group: %s", group);
		else
		{
			WM_WIDGET_NAME_GET(widget_name, group);
			if(l_next)
			{
				group_next = eina_list_data_get(l_next);
				WM_WIDGET_NAME_GET(widget_name_next, group_next);
			}
			else
				widget_name_next = &empty;

			widget_styles = eina_list_append(widget_styles, strdup(group));

			if(strcmp(widget_name, widget_name_next) != 0)
			{
				widget = wm_widget_add(widget_name, widget_styles);
				widget_list = eina_inlist_append(widget_list,
								EINA_INLIST_GET(widget));
				widget_styles = wm_styles_list_free(widget_styles);
			}
		}
	}
	edje_file_collection_list_free(collection);
	return widget_list;
}

Eina_Bool
wm_widget_list_free(Eina_Inlist *widget_list)
{
	Widget *widget;

	if(!widget_list)
		return EINA_FALSE;

	while(widget_list)
	{
		widget = EINA_INLIST_CONTAINER_GET(widget_list, Widget);
		widget_list = eina_inlist_remove(widget_list, widget_list);
		wm_widget_free(widget);
	}

	return EINA_TRUE;
}

#undef WM_WIDGET_NAME_GET
#undef WM_CLASS_NAME_GET
#undef WM_STYLE_NAME_GET
