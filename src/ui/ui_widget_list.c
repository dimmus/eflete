#include "ui_widget_list.h"

static Elm_Genlist_Item_Class *_itc_widget = NULL;
static Elm_Genlist_Item_Class *_itc_style = NULL;
static Elm_Genlist_Item_Class *_itc_style_tree = NULL;
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
_navi_glist_pop(void *data, Evas_Object *obj, void *event_info)
{
	elm_naviframe_item_pop(data);
}

static void
_gl_exp_req(void *data __UNUSED__, Evas_Object * obj __UNUSED__,
			void *event_info)
{
	Elm_Object_Item *glist = (Elm_Object_Item *)event_info;
	elm_genlist_item_expanded_set(glist, EINA_TRUE);
}

static void
_gl_con_req(void *data __UNUSED__, Evas_Object * obj __UNUSED__,
			void *event_info)
{
	Elm_Object_Item *glist = (Elm_Object_Item *)event_info;
	elm_genlist_item_expanded_set(glist, EINA_FALSE);
}

static void
_gl_con(void *data __UNUSED__, Evas_Object * obj __UNUSED__,
			void *event_info)
{
	Elm_Object_Item *glist = (Elm_Object_Item *)event_info;
	elm_genlist_item_subitems_clear(glist);
}

static void
_gl_exp(void *data, Evas_Object * obj __UNUSED__,
			void *event_info)
{
	Elm_Object_Item *glist = (Elm_Object_Item *)event_info;
	Elm_Object_Item *eoi;
	Evas_Object *gl = elm_object_item_widget_get(glist);
	Style *_style;
	Group *_group;
	Eina_Inlist *groups;

	_style = elm_object_item_data_get(glist);
	groups = _style->groups;

	if(!_itc_group)
	{
		_itc_group = elm_genlist_item_class_new();
		_itc_group->item_style = "default";
		_itc_group->func.text_get = _item_group_label_get;
		_itc_group->func.content_get = NULL;
		_itc_group->func.state_get = NULL;
		_itc_group->func.del = NULL;
	}

	EINA_INLIST_FOREACH(groups, _group)
	{
		eoi = elm_genlist_item_append(gl, _itc_group,
									_group,
									glist, ELM_GENLIST_ITEM_NONE,
									NULL, NULL);

		elm_object_item_data_set(eoi, _group);
	}
}

static void
_on_widget_clicked_double(void *data, Evas_Object *obj, void *event_info)
{
	Elm_Object_Item *glit = event_info;
	Elm_Object_Item *eoi;
	Evas_Object *nf, *gl_styles, *bt, *ic;
	Eina_Inlist *styles;
	Widget *_widget;
	Style *_style;

	nf = (Evas_Object *)data;
	_widget = elm_object_item_data_get(glit);
	styles = _widget->styles;

	if(!_itc_style_tree)
	{
		_itc_style_tree = elm_genlist_item_class_new();
		_itc_style_tree->item_style = "tree_effect";
		_itc_style_tree->func.text_get = _item_style_label_get;
		_itc_style_tree->func.content_get = NULL;
		_itc_style_tree->func.state_get = NULL;
		_itc_style_tree->func.del = NULL;
	}

	gl_styles = elm_genlist_add(nf);
	evas_object_size_hint_align_set(gl_styles,
									EVAS_HINT_FILL,
									EVAS_HINT_FILL);
	evas_object_size_hint_weight_set(gl_styles,
									EVAS_HINT_EXPAND,
									EVAS_HINT_EXPAND);

	EINA_INLIST_FOREACH(styles, _style)
	{
		eoi = elm_genlist_item_append(gl_styles, _itc_style_tree,
										_style,
										NULL, ELM_GENLIST_ITEM_TREE,
										NULL, NULL);
		/* maybe add a ref to _widget */
		elm_object_item_data_set(eoi, _style);
	}

	evas_object_smart_callback_add(gl_styles, "expand,request", _gl_exp_req, NULL);
	evas_object_smart_callback_add(gl_styles, "contract,request", _gl_con_req, NULL);
	evas_object_smart_callback_add(gl_styles, "expanded", _gl_exp, NULL);
	evas_object_smart_callback_add(gl_styles, "contracted", _gl_con, nf);

	ic = elm_icon_add(nf);
	elm_icon_standard_set(ic, "arrow_left");
	evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);

	bt = elm_button_add(nf);
	evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_layout_content_set(bt, "icon", ic);
	evas_object_smart_callback_add(bt, "clicked", _navi_glist_pop, nf);

	elm_naviframe_item_push(nf, _widget->widget_name, bt, NULL, gl_styles, NULL);
}

Evas_Object *
ui_widget_list_add(App_Data *ap, Eina_Inlist *widget_list)
{
	Evas_Object *nf;
	Evas_Object *ui_widget_list = NULL;
	Widget *_widget;
	Elm_Object_Item *eoi;
	Elm_Object_Item *it;

	if(!_itc_widget)
	{
		_itc_widget = elm_genlist_item_class_new();
		_itc_widget->item_style = "default";
		_itc_widget->func.text_get = _item_widget_label_get;
		_itc_widget->func.content_get = NULL;
		_itc_widget->func.state_get = NULL;
		_itc_widget->func.del = NULL;
	}

	nf = elm_naviframe_add(ap->block_left_top);
	evas_object_size_hint_align_set(nf, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_size_hint_weight_set(nf, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

	ui_widget_list = elm_genlist_add(nf);
	evas_object_size_hint_align_set(ui_widget_list,
									EVAS_HINT_FILL,
									EVAS_HINT_FILL);
	evas_object_size_hint_weight_set(ui_widget_list,
									EVAS_HINT_EXPAND,
									EVAS_HINT_EXPAND);

	EINA_INLIST_FOREACH(widget_list, _widget)
	{
		eoi = elm_genlist_item_append(ui_widget_list, _itc_widget,
										_widget,
										NULL, ELM_GENLIST_ITEM_NONE,
										NULL, NULL);
		/* maybe add a ref to _widget */
		elm_object_item_data_set(eoi, _widget);
	}

	//clicked,double
	evas_object_smart_callback_add(ui_widget_list, "clicked,double",
									_on_widget_clicked_double, nf);

	it = elm_naviframe_item_push(nf, ap->project->name, NULL, NULL, ui_widget_list, NULL);
	elm_object_item_part_text_set(it, "subtitle", "Widget list");

	return nf;
}
