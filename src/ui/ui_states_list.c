#include "ui_states_list.h"

static Elm_Genlist_Item_Class *_itc_state = NULL;

static char *
_item_state_label_get(void *data,
						Evas_Object *obj __UNUSED__,
						const char *part __UNUSED__)
{
	char state_name[BUFF_MAX];
	Part_State *state = (Part_State *)data;

	sprintf(state_name, "%s %f", state->name, state->value);

	return strdup(state_name);
}

static void
_on_state_selected(void *data,
					Evas_Object *obj __UNUSED__,
					void *event_info)
{
	Elm_Object_Item *glit = (Elm_Object_Item *)event_info;
	App_Data *ap = (App_Data *)data;
	Part_State *state;
	Evas_Object *prop_view, *part_view, *state_view;

	state = elm_object_item_data_get(glit);

	prop_view = ui_block_property_get(ap);
	part_view = ui_property_part_view_get(prop_view);
	state_view = ui_prop_part_info_state_view_add(part_view, state);
	ui_prop_part_info_state_set(part_view, state_view);
	evas_object_show(state_view);
}

Evas_Object *
ui_states_list_add(App_Data *ap, Part *part)
{
	Evas_Object *gl_states;
	Part_State  *state;
	Eina_Inlist *states;
	Elm_Object_Item *eoi;

	if(!ap || !part)
	{
		return NULL;
	}

	states = part->states;

	if(!_itc_state)
	{
		_itc_state = elm_genlist_item_class_new();
		_itc_state->item_style = "default";
		_itc_state->func.text_get = _item_state_label_get;
		_itc_state->func.content_get = NULL;
		_itc_state->func.state_get = NULL;
		_itc_state->func.del = NULL;
	}

	gl_states = elm_genlist_add(ap->win);
	evas_object_size_hint_align_set(gl_states,
									EVAS_HINT_FILL,
									EVAS_HINT_FILL);
	evas_object_size_hint_weight_set(gl_states,
									EVAS_HINT_EXPAND,
									EVAS_HINT_EXPAND);

	EINA_INLIST_FOREACH(states, state)
	{
		eoi = elm_genlist_item_append(gl_states, _itc_state,
										state,
										NULL, ELM_GENLIST_ITEM_NONE,
										NULL, NULL);
		elm_object_item_data_set(eoi, state);
	}

	evas_object_smart_callback_add(gl_states, "selected",
									_on_state_selected, ap);

	return gl_states;
}
