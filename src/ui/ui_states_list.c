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

Evas_Object *
ui_states_list_add(App_Data *ap, Part *part)
{
	Evas_Object *gl_states;
	Part_State  *state;
	Eina_Inlist *states;

	if(!ap || !part)
		return NULL;

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

	gl_states = elm_genlist_add(ap->block_bottom_left);
	evas_object_size_hint_align_set(gl_states,
									EVAS_HINT_FILL,
									EVAS_HINT_FILL);
	evas_object_size_hint_weight_set(gl_states,
									EVAS_HINT_EXPAND,
									EVAS_HINT_EXPAND);

	EINA_INLIST_FOREACH(states, state)
	{
		elm_genlist_item_append(gl_states, _itc_state,
								state,
								NULL, ELM_GENLIST_ITEM_NONE,
								NULL, NULL);
	}

	return gl_states;
}
