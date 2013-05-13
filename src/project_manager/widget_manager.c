#include "widget_manager.h"

char **arr;
static char *empty = "\0";
static unsigned int arr_index;

#define FREE(arr) \
	for(arr_index = 0; arr_index < sizeof(arr)/sizeof(char *); arr_index++) \
		free(arr[arr_index]); \
	free(arr); \
	arr = NULL;

#define WM_WIDGET_NAME_GET(widget, group) \
	arr = eina_str_split(group, "/", 3); \
	widget = strdup(arr[1]); \
	FREE(arr);

#define WM_STYLE_NAME_GET(style, group) \
	arr = eina_str_split(group, "/", 4); \
	style = strdup(arr[3]); \
	FREE(arr);

#define WM_GROUP_NAME_GET(group_name, group) \
	arr = eina_str_split(group, "/", 4); \
	group_name = strdup(arr[2]);\
	FREE(arr);

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
	Part *part = NULL;
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

Style *
wm_style_add(const char *style, Eina_List *groups)
{
	Style *style_edje;
	Eina_List *l;
	Group *group_edje;
	char *group_name, *group_name_full;

	if(!style || !groups)
		return NULL;

	style_edje = calloc(1, sizeof(*style_edje));
	style_edje->style_name = strdup(style);
	style_edje->groups = NULL;

	EINA_LIST_FOREACH(groups, l, group_name_full)
	{
		WM_GROUP_NAME_GET(group_name, group_name_full);
		group_edje = wm_group_add(group_name, group_name_full);
		style_edje->groups = eina_inlist_append(style_edje->groups,
							EINA_INLIST_GET(group_edje));
	}

	return style_edje;
}

Eina_Bool
wm_style_free(Style *style)
{
	Group *group_edje;
	Eina_Inlist *list;

	if(!style)
		return EINA_FALSE;

	list = style->groups;

	if(list)
	{
		while(list)
		{
			group_edje = EINA_INLIST_CONTAINER_GET(list, Group);
			list = eina_inlist_remove(list, list);
			wm_group_free(group_edje);
		}
	}

	free(style->style_name);
	free(style);

	return EINA_TRUE;
}

int
sort_style_cb(const void *data1, const void *data2)
{
	const char *str1 = data1;
	const char *str2 = data2;
	char *data1_style;
	char *data2_style = NULL;

	if(!str1)
		return 1;
	if(!str2)
		return -1;

	WM_STYLE_NAME_GET(data1_style, str1);
	WM_STYLE_NAME_GET(data2_style, str2);

	return (strcmp(data1_style, data2_style));
}

Widget *
wm_widget_add(const char *widget, Eina_List *groups)
{
	Widget *_widget = NULL;
	Eina_List *l, *l_next;
	Eina_List *widget_groups = NULL;
	Style *style;
	char *style_name, *style_name_next;
	char *group, *group_next;

	if(!widget)
		return NULL;

	_widget = calloc(1, sizeof(*_widget));
	_widget->widget_name = strdup(widget);
	_widget->styles = NULL;

	groups = eina_list_sort(groups,
							eina_list_count(groups),
							sort_style_cb);

	EINA_LIST_FOREACH_SAFE(groups, l, l_next, group)
	{
		WM_STYLE_NAME_GET(style_name, group);
		if(l_next)
		{
			group_next = eina_list_data_get(l_next);
			WM_STYLE_NAME_GET(style_name_next, group_next);
		}
		else
			style_name_next = empty;

		widget_groups = eina_list_append(widget_groups, group);

		if(strcmp(style_name, style_name_next) != 0)
		{
			style = wm_style_add(style_name, widget_groups);
			_widget->styles = eina_inlist_append(_widget->styles,
							EINA_INLIST_GET(style));
			widget_groups = eina_list_free(widget_groups);
			widget_groups = NULL;
		}
	}
	return _widget;
}

Eina_Bool
wm_widget_free(Widget *widget)
{
	Style *style;
	Eina_Inlist  *list;

	if(!widget)
		return EINA_FALSE;

	list = widget->styles;

	if(list)
	{
		while(list)
		{
			style = EINA_INLIST_CONTAINER_GET(list, Style);
			list = eina_inlist_remove(list, list);
			wm_style_free(style);
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

	DBG("Start to parse the edje group collection.");
	collection = edje_file_collection_list(file);
	if(!collection)
	{
		error = eina_error_get();
		ERR("%s", eina_error_msg_get(error));
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
				widget_name_next = empty;

			widget_styles = eina_list_append(widget_styles, group);

			if(strcmp(widget_name, widget_name_next) != 0)
			{
				widget = wm_widget_add(widget_name, widget_styles);
				widget_list = eina_inlist_append(widget_list,
								EINA_INLIST_GET(widget));
				widget_styles = eina_list_free(widget_styles);
				widget_styles = NULL;
			}
		}
	}
	edje_file_collection_list_free(collection);
	DBG("Parse the edje group collection id finished! The list of widget - created.");
	INFO("The theme consist a styles for %i widgets.",
			eina_inlist_count(widget_list));

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
#undef WM_STYLE_NAME_GET
#undef WM_GROUP_NAME_GET
