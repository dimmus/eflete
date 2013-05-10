#include "ui_widget_list.h"

static Elm_Genlist_Item_Class *_itc_widget = NULL;
static Elm_Genlist_Item_Class *_itc_class = NULL;
static Elm_Genlist_Item_Class *_itc_group = NULL;
static Elm_Genlist_Item_Class *_itc_part = NULL;

static char *
_item_group_label_get(void *data,
					Evas_Object *obj __UNUSED__,
					const char *part __UNUSED__)
{
	Group *g = (Group *)data;
	if(!g->group_name)
	{
		ERR("It impossible, but it is occurred, group name is missing!");
		return NULL;
	}
	return strdup(g->group_name);
}

static void
_style_sel(void *data, Evas_Object *obj, void *event_info)
{
	Evas_Object *nf = NULL;
	Evas_Object *glist_style;
	Style *_style = NULL;
	Group *_group;
	//elm_genlist_item_selected_set((Elm_Object_Item *)event_info, EINA_FALSE);
	//DBG("");

	/* Get naviframe */
	nf = (Evas_Object *)data;
	_style = elm_object_item_data_get((Elm_Object_Item *)event_info);

	if(!_itc_group)
	{
		_itc_group = elm_genlist_item_class_new();
		_itc_group->item_style = "tree_effect";
		_itc_group->func.text_get = _item_group_label_get;
		_itc_group->func.content_get = NULL;
		_itc_group->func.state_get = NULL;
		_itc_group->func.del = NULL;
	}

	if(!_itc_part)
	{
		_itc_part = elm_genlist_item_class_new();
		_itc_part->item_style = "default";
		_itc_part->func.text_get = NULL; //_item_style_label_get;
		_itc_part->func.content_get = NULL;
		_itc_part->func.state_get = NULL;
		_itc_part->func.del = NULL;
	}

	EINA_INLIST_FOREACH(_style->groups, _group)
	{
		DBG("%s", _group->group_name);
	}
}

static char *
_item_widget_label_get(void *data,
						Evas_Object *obj __UNUSED__,
						const char *part __UNUSED__)
{
	Widget *w = (Widget *)data;
	if(!w->widget_name)
	{
		ERR("It impossible, but it is occurred, widget name is missing!");
		return NULL;
	}
	return strdup(w->widget_name);
}

static char *
_item_style_label_get(void *data,
						Evas_Object *obj __UNUSED__,
						const char *part __UNUSED__)
{
	Style *st = (Style *)data;
	if(!st->style_name)
	{
		ERR("It impossible, but it is occurred, style name is missing!");
		return NULL;
	}
	return strdup(st->style_name);
}

static void
ui_wl_exp_req(void *data __UNUSED__,
				Evas_Object *obj __UNUSED__,
				void *event_info)
{
	Elm_Object_Item *glit = (Elm_Object_Item *)event_info;
	elm_genlist_item_expanded_set(glit, EINA_TRUE);
}

static void
ui_wl_exp_con(void *data __UNUSED__,
				Evas_Object *obj __UNUSED__,
				void *event_info)
{
	Elm_Object_Item *glit = (Elm_Object_Item *)event_info;
	elm_genlist_item_expanded_set(glit, EINA_FALSE);
}

static void
ui_wl_exp(void *data, Evas_Object *obj, void *event_info)
{
	Elm_Object_Item *gl_item = (Elm_Object_Item *)event_info;
	Elm_Object_Item *gl_item_style = NULL;
	Evas_Object *glist = elm_object_item_widget_get(gl_item);
	Evas_Object *nf = elm_object_parent_widget_get(glist);
	Widget *_widget = elm_object_item_data_get(gl_item);
	Style *_style;

	if(!_widget)
	{
		ERR("Widget list item could not contained a data about widget!");
		return;
	}

	EINA_INLIST_FOREACH(_widget->styles, _style)
	{
		gl_item_style = elm_genlist_item_append(glist, _itc_class,
												(void *)_style, gl_item,
												ELM_GENLIST_ITEM_NONE,
												_style_sel, (void *)nf);
		elm_object_item_data_set(gl_item_style, _style);
	}
}

static void
ui_wl_con(void *data __UNUSED__,
			Evas_Object *obj __UNUSED__,
			void *event_info)
{
	Elm_Object_Item *gl_item = (Elm_Object_Item *)event_info;
	elm_genlist_item_subitems_clear(gl_item);
}

Evas_Object *
ui_widget_list_add(App_Data *ap, Eina_Inlist *widget_list)
{
	Evas_Object *nf;
	Evas_Object *ui_widget_list = NULL;
	Widget *_widget;
	Elm_Object_Item *eoi;

	if(!_itc_widget)
	{
		_itc_widget = elm_genlist_item_class_new();
		_itc_widget->item_style = "tree_effect";
		_itc_widget->func.text_get = _item_widget_label_get;
		_itc_widget->func.content_get = NULL;
		_itc_widget->func.state_get = NULL;
		_itc_widget->func.del = NULL;
	}

	if(!_itc_class)
	{
		_itc_class = elm_genlist_item_class_new();
		_itc_class->item_style = "default";
		_itc_class->func.text_get = _item_style_label_get;
		_itc_class->func.content_get = NULL;
		_itc_class->func.state_get = NULL;
		_itc_class->func.del = NULL;
	}

	nf = elm_naviframe_add(ap->block_left_top);
	evas_object_size_hint_align_set(nf, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_size_hint_weight_set(nf, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

	ui_widget_list = elm_genlist_add(ap->block_left_top);
	evas_object_size_hint_align_set(ui_widget_list,
									EVAS_HINT_FILL,
									EVAS_HINT_FILL);
	evas_object_size_hint_weight_set(ui_widget_list,
									EVAS_HINT_EXPAND,
									EVAS_HINT_EXPAND);
	elm_genlist_tree_effect_enabled_set(ui_widget_list, EINA_TRUE);

	DBG("count items: %d", eina_inlist_count(widget_list));
	EINA_INLIST_FOREACH(widget_list, _widget)
	{
		eoi = elm_genlist_item_append(ui_widget_list, _itc_widget,
										_widget,
										NULL, ELM_GENLIST_ITEM_TREE,
										NULL, NULL);
		/* maybe add a ref to _widget */
		elm_object_item_data_set(eoi, _widget);
	}

	evas_object_smart_callback_add(ui_widget_list, "expand,request",
									ui_wl_exp_req, ui_widget_list);
	evas_object_smart_callback_add(ui_widget_list, "contract,request",
									ui_wl_exp_con, ui_widget_list);
	evas_object_smart_callback_add(ui_widget_list, "expanded",
									ui_wl_exp, ui_widget_list);
	evas_object_smart_callback_add(ui_widget_list, "contracted",
									ui_wl_con, ui_widget_list);

	elm_naviframe_item_push(nf, NULL, NULL, NULL, ui_widget_list, NULL);

	return nf;
}
