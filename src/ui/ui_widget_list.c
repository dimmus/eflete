#include "ui_widget_list.h"

static Elm_Genlist_Item_Class *_itc_widget = NULL;
static Elm_Genlist_Item_Class *_itc_style = NULL;
static Elm_Genlist_Item_Class *_itc_group = NULL;
static Elm_Genlist_Item_Class *_itc_part = NULL;

static char *
_item_part_label_get(void *data,
					Evas_Object *obj __UNUSED__,
					const char *part __UNUSED__)
{
	Part *p = (Part *)data;
	if(!p->name)
	{
		ERR("It impossible, but it is occurred, part name is missing!");
		return NULL;
	}
	return strdup(p->name);
}

static Evas_Object *
_item_part_content_get(void *data __UNUSED__,
					Evas_Object *obj,
					const char *part)
{
	Evas_Object *icon = elm_icon_add(obj);
	if (!strcmp(part, "elm.swallow.icon"))
		elm_image_file_set(icon, TET_IMG_PATH"eye_open.png", NULL);

	evas_object_size_hint_aspect_set(icon, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);

	return icon;
}

static char *
_item_group_label_get(void *data,
					Evas_Object *obj __UNUSED__,
					const char *part __UNUSED__)
{
	char tmp[64];
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
_navi_gl_styles_pop(void *data,
					Evas_Object *obj __UNUSED__,
					void *event_info __UNUSED__)
{
	App_Data *ap = (App_Data *)data;
	Evas_Object *nf = ui_block_content_get(ap->block_left_top);
	elm_naviframe_item_pop(nf);
}

static void
_navi_gl_parts_pop(void *data,
					Evas_Object *obj __UNUSED__,
					void *event_info __UNUSED__)
{
	App_Data *ap = (App_Data *)data;
	Evas_Object *nf = ui_block_content_get(ap->block_left_top);
	Evas_Object *gl_signals = ui_block_content_get(ap->block_left_bottom);

	elm_naviframe_item_pop(nf);
	elm_genlist_clear(gl_signals);
}

static void
_on_group_clicked_double(void *data, Evas_Object *obj, void *event_info)
{
	Elm_Object_Item *glit = (Elm_Object_Item *)event_info;
	Elm_Object_Item *eoi;
	App_Data *ap;
	Evas_Object *nf, *gl_parts, *bt, *ic, *gl_signals;
	Eina_Inlist *parts;
	Eina_List *signals;
	Group *_group;
	Part *_part;

	ap = (App_Data *)data;
	nf = ui_block_content_get(ap->block_left_top);
	if(!nf)
	{
		ERR("Naviframe is missing!");
		return;
	}
	_group = elm_object_item_data_get(glit);
	if(_group->__type != GROUP)
		return;
	if(!_group->obj)
		wm_group_data_load(_group, ap->ws->canvas, ap->project->swapfile);

	parts = _group->parts;

	if(!_itc_part)
	{
		_itc_part = elm_genlist_item_class_new();
		_itc_part->item_style = "default";
		_itc_part->func.text_get = _item_part_label_get;
		_itc_part->func.content_get = _item_part_content_get;
		_itc_part->func.state_get = NULL;
		_itc_part->func.del = NULL;
	}

	gl_parts = elm_genlist_add(nf);
	evas_object_size_hint_align_set(gl_parts,
									EVAS_HINT_FILL,
									EVAS_HINT_FILL);
	evas_object_size_hint_weight_set(gl_parts,
									EVAS_HINT_EXPAND,
									EVAS_HINT_EXPAND);

	EINA_INLIST_FOREACH(parts, _part)
	{
		eoi = elm_genlist_item_append(gl_parts, _itc_part,
										_part,
										NULL, ELM_GENLIST_ITEM_NONE,
										NULL, NULL);
		elm_object_item_data_set(eoi, _part);
	}

	/* Get signals list of a group and show them */
	signals = wm_program_signals_list_get(_group->programs);
	gl_signals = ui_signal_list_add(ap, signals);
	wm_program_signals_list_free(signals);
	ui_block_content_set(ap->block_left_bottom, gl_signals);

	ic = elm_icon_add(nf);
	elm_icon_standard_set(ic, "arrow_left");
	evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);

	bt = elm_button_add(nf);
	evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_layout_content_set(bt, "icon", ic);
	evas_object_smart_callback_add(bt, "clicked", _navi_gl_parts_pop, ap);

	elm_naviframe_item_push(nf, _group->full_group_name, bt, NULL, gl_parts, NULL);
}

static void
_on_widget_clicked_double(void *data, Evas_Object *obj, void *event_info)
{
	Elm_Object_Item *glit = (Elm_Object_Item *)event_info;
	Elm_Object_Item *glit_style, *glit_group;
	App_Data *ap;
	Evas_Object *nf, *gl_styles, *bt, *ic;
	Eina_Inlist *styles, *groups;
	Widget *_widget;
	Style *_style;
	Group *_group;

	ap = (App_Data *)data;
	nf = ui_block_content_get(ap->block_left_top);
	if(!nf)
	{
		ERR("Naviframe is missing!");
		return;
	}
	_widget = elm_object_item_data_get(glit);
	styles = _widget->styles;

	if(!_itc_style)
	{
		_itc_style = elm_genlist_item_class_new();
		_itc_style->item_style = "default";//"tree_effect";
		_itc_style->func.text_get = _item_style_label_get;
		_itc_style->func.content_get = NULL;
		_itc_style->func.state_get = NULL;
		_itc_style->func.del = NULL;
	}

	if(!_itc_group)
	{
		_itc_group = elm_genlist_item_class_new();
		_itc_group->item_style = "default";
		_itc_group->func.text_get = _item_group_label_get;
		_itc_group->func.content_get = NULL;
		_itc_group->func.state_get = NULL;
		_itc_group->func.del = NULL;
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
		glit_style = elm_genlist_item_append(gl_styles, _itc_style,
											_style,
											NULL, ELM_GENLIST_ITEM_NONE,
											NULL, NULL);
		groups = _style->groups;

		EINA_INLIST_FOREACH(groups, _group)
		{
			glit_group = elm_genlist_item_append(gl_styles, _itc_group,
												_group,
												glit_style, ELM_GENLIST_ITEM_NONE,
												NULL, NULL);
			elm_object_item_data_set(glit_group, _group);
		}
	}

	evas_object_smart_callback_add(gl_styles, "clicked,double",
									_on_group_clicked_double, ap);

	ic = elm_icon_add(nf);
	elm_icon_standard_set(ic, "arrow_left");
	evas_object_size_hint_aspect_set(ic, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);

	bt = elm_button_add(nf);
	evas_object_size_hint_align_set(bt, EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_layout_content_set(bt, "icon", ic);
	evas_object_smart_callback_add(bt, "clicked", _navi_gl_styles_pop, ap);

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

	evas_object_smart_callback_add(ui_widget_list, "clicked,double",
									_on_widget_clicked_double, ap);

	it = elm_naviframe_item_push(nf, ap->project->name, NULL, NULL, ui_widget_list, NULL);
	elm_object_item_part_text_set(it, "subtitle", "Widget list");

	return nf;
}
