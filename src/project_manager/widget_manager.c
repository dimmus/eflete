#include "widget_manager.h"

static char **arr;
static char tmp[PATH_MAX];
static int size;
static char empty = '\0';

#define WM_WIDGET_NAME_GET(widget, group) \
	arr = eina_str_split(group, "/", 3); \
	widget = strdup(arr[1]); \
	free(arr[0]); \
	free(arr);

#define WM_STYLE_NAME_GET(style, group) \
	arr = eina_str_split(group, "/", 0); \
	for (size = 0; arr[size]; size++) \
		; \
	style = strdup(arr[size - 1]);\
	free(arr[0]); \
	free(arr);

#define WM_GROUP_NAME_GET(group_name, style, group) \
	arr = eina_str_split(group, "/", 0); \
	strcpy(tmp, arr[2]); \
	for (size = 3; arr[size]; size++) \
	{ \
		if (strcmp(arr[size],style)) \
		{ \
			strcat(tmp, "/"); \
			strcat(tmp, arr[size]); \
		} \
	} \
	group_name = strdup(tmp); \
	free(arr[0]); \
	free(arr);

static void
wm_part_state_text_add(Evas_Object *obj,
						const char *part_name,
						Part_State *state)
{
	if (!obj || !part_name || !state)
		return;

	state->text = calloc(1, sizeof(Part_State_Text));

	state->text->text = edje_edit_state_text_get(obj, part_name, state->name, state->value);
	state->text->font = edje_edit_state_font_get(obj, part_name, state->name, state->value);
	state->text->size = edje_edit_state_text_size_get(obj, part_name, state->name, state->value);
	state->text->align_x = edje_edit_state_text_align_x_get(obj, part_name, state->name, state->value);
	state->text->align_y = edje_edit_state_text_align_y_get(obj, part_name, state->name, state->value);
	state->text->elipsis = edje_edit_state_text_elipsis_get(obj, part_name, state->name, state->value);
	state->text->fit_x = edje_edit_state_text_fit_x_get(obj, part_name, state->name, state->value);
	state->text->fit_y = edje_edit_state_text_fit_y_get(obj, part_name, state->name, state->value);
	state->text->fonts_list = edje_edit_fonts_list_get(obj);
}

static void
wm_part_state_text_free(Part_State *state)
{
	if (!state || !state->text)
		return;

	edje_edit_string_free(state->text->text);
	edje_edit_string_free(state->text->font);
	edje_edit_string_list_free(state->text->fonts_list);

	free(state->text);
	state->text = NULL;
}

static void
wm_part_state_image_add(Evas_Object *obj, const char *part_name, Part_State *state)
{
	if (!obj || !part_name || !state)
		return;

	state->image = calloc(1, sizeof(Part_State_Image));

	state->image->normal_image = edje_edit_state_image_get(obj, part_name, state->name, state->value);

	state->image->id = edje_edit_image_id_get(obj, state->image->normal_image);
	state->image->compression_type = edje_edit_image_compression_type_get(obj, state->image->normal_image);
	state->image->compression_rate = edje_edit_image_compression_rate_get(obj, state->image->normal_image);

	edje_edit_state_image_border_get(obj, part_name, state->name, state->value,
									&state->image->left, &state->image->right,
									&state->image->top, &state->image->bottom);

	state->image->border_fill = edje_edit_state_image_border_fill_get(obj, part_name, state->name, state->value);

	state->image->fill_origin_relative_x = edje_edit_state_fill_origin_relative_x_get(obj, part_name, state->name, state->value);
	state->image->fill_origin_relative_y = edje_edit_state_fill_origin_relative_y_get(obj, part_name, state->name, state->value);
	state->image->fill_origin_offset_x = edje_edit_state_fill_origin_offset_x_get(obj, part_name, state->name, state->value);
	state->image->fill_origin_offset_y = edje_edit_state_fill_origin_offset_y_get(obj, part_name, state->name, state->value);

	state->image->fill_size_relative_x = edje_edit_state_fill_size_relative_x_get(obj, part_name, state->name, state->value);
	state->image->fill_size_relative_y = edje_edit_state_fill_size_relative_y_get(obj, part_name, state->name, state->value);
	state->image->fill_size_offset_x = edje_edit_state_fill_size_offset_x_get(obj, part_name, state->name, state->value);
	state->image->fill_size_offset_y = edje_edit_state_fill_size_offset_y_get(obj, part_name, state->name, state->value);

	state->image->tweens_images = edje_edit_state_tweens_list_get(obj, part_name, state->name, state->value);
}

static void
wm_part_state_image_free(Part_State *state)
{
	if (!state || !state->image )
		return;

	edje_edit_string_free(state->image->normal_image);
	free(state->image);
	state->image = NULL;
}

static void
wm_part_states_add(Evas_Object *obj, Part *part)
{
	Eina_List *states, *l;
	const char *state, *state_name;
	char **state_copy;
	double state_value;
	Part_State *new_state;

	if (!obj || !part )
		return;

	states = edje_edit_part_states_list_get(obj, part->name);

	EINA_LIST_FOREACH(states, l, state)
	{
		new_state = calloc(1, sizeof(Part_State));

		/* State has format "NAME X.X" we need to extract NAME and double value separately */
		/* Split source name on 2 strings */
		state_copy = eina_str_split(state, " ", 2);
		/* Get state NAME */
		new_state->name = state_name = eina_stringshare_add(state_copy[0]);
		/* Get state X.X value */
		new_state->value = state_value = atof(state_copy[1]);

		new_state->rel1_relative_x = edje_edit_state_rel1_relative_x_get(obj, part->name, state_name, state_value);
		new_state->rel1_relative_y = edje_edit_state_rel1_relative_y_get(obj, part->name, state_name, state_value);
		new_state->rel2_relative_x = edje_edit_state_rel2_relative_x_get(obj, part->name, state_name, state_value);
		new_state->rel2_relative_y = edje_edit_state_rel2_relative_y_get(obj, part->name, state_name, state_value);

		new_state->rel1_offset_x = edje_edit_state_rel1_offset_x_get(obj, part->name, state_name, state_value);
		new_state->rel1_offset_y = edje_edit_state_rel1_offset_y_get(obj, part->name, state_name, state_value);
		new_state->rel2_offset_x = edje_edit_state_rel2_offset_x_get(obj, part->name, state_name, state_value);
		new_state->rel2_offset_y = edje_edit_state_rel2_offset_y_get(obj, part->name, state_name, state_value);

		new_state->rel1_to_x_name = edje_edit_state_rel1_to_x_get(obj, part->name, state_name, state_value);
		new_state->rel1_to_y_name = edje_edit_state_rel1_to_y_get(obj, part->name, state_name, state_value);
		new_state->rel2_to_x_name = edje_edit_state_rel2_to_x_get(obj, part->name, state_name, state_value);
		new_state->rel2_to_y_name = edje_edit_state_rel2_to_y_get(obj, part->name, state_name, state_value);

		edje_edit_state_color_get(obj, part->name, state_name, state_value, &new_state->color[0],
				&new_state->color[1],	&new_state->color[2], &new_state->color[3]);
		edje_edit_state_color2_get(obj, part->name, state_name, state_value, &new_state->color2[0],
				&new_state->color2[1],	&new_state->color2[2], &new_state->color2[3]);
		edje_edit_state_color3_get(obj, part->name, state_name, state_value, &new_state->color3[0],
				&new_state->color3[1],	&new_state->color3[2], &new_state->color3[3]);

		new_state->align_x = edje_edit_state_align_x_get(obj, part->name, state_name, state_value);
		new_state->align_y = edje_edit_state_align_y_get(obj, part->name, state_name, state_value);

		new_state->min_w = edje_edit_state_min_w_get(obj, part->name, state_name, state_value);
		new_state->min_h = edje_edit_state_min_h_get(obj, part->name, state_name, state_value);
		new_state->max_w = edje_edit_state_max_w_get(obj, part->name, state_name, state_value);
		new_state->max_h = edje_edit_state_max_h_get(obj, part->name, state_name, state_value);

		new_state->aspect_min = edje_edit_state_aspect_min_get(obj, part->name, state_name, state_value);
		new_state->aspect_max = edje_edit_state_aspect_max_get(obj, part->name, state_name, state_value);
		new_state->aspect_pref = edje_edit_state_aspect_pref_get(obj, part->name, state_name, state_value);

		new_state->visible = edje_edit_state_visible_get(obj, part->name, state_name, state_value);
		new_state->color_class = edje_edit_state_color_class_get(obj, part->name, state_name, state_value);

		if (part->type == EDJE_PART_TYPE_TEXT || part->type == EDJE_PART_TYPE_TEXTBLOCK)
			wm_part_state_text_add(obj, part->name, new_state);
		else
			new_state->text = NULL;

		if (part->type == EDJE_PART_TYPE_IMAGE)
			wm_part_state_image_add(obj, part->name, new_state);
		else
			new_state->image = NULL;

		free(state_copy[0]);
		free(state_copy);

		part->states = eina_inlist_append(part->states, EINA_INLIST_GET(new_state));
	}

	edje_edit_string_list_free(states);
}

static void
wm_part_states_free(Part* part)
{
	Part_State *aux;

	if (!part)
		return;

	while (part->states)
	{
		aux = EINA_INLIST_CONTAINER_GET(part->states, Part_State);

		eina_stringshare_del(aux->name);
		edje_edit_string_free(aux->color_class);
		edje_edit_string_free(aux->rel1_to_x_name);
		edje_edit_string_free(aux->rel1_to_y_name);
		edje_edit_string_free(aux->rel2_to_x_name);
		edje_edit_string_free(aux->rel2_to_y_name);

		if (part->type == EDJE_PART_TYPE_TEXT || part->type == EDJE_PART_TYPE_TEXT)
			wm_part_state_text_free(aux);

		if (part->type == EDJE_PART_TYPE_IMAGE)
			wm_part_state_image_free(aux);

		part->states = eina_inlist_remove(part->states, part->states);
		free(aux);
	}
}

Part *
wm_part_add(Evas_Object *obj, const char *part_name)
{
	Part *result;

	if (!part_name || !obj)
		return NULL;

	result = calloc(1, sizeof(Part));
	result->__type = PART;

	result->name = eina_stringshare_add(part_name);
	result->api_name = edje_edit_part_api_name_get(obj, part_name);
	result->description = edje_edit_part_api_description_get(obj, part_name);
	result->type = edje_edit_part_type_get(obj, part_name);
	result->clip_to = edje_edit_part_clip_to_get(obj, part_name);
	result->source = edje_edit_part_source_get(obj, part_name);
	result->effect = edje_edit_part_effect_get(obj, part_name);
	result->selected_state_name = edje_edit_part_selected_state_get(obj, part_name,	&result->selected_state);
	result->mouse_events = edje_edit_part_mouse_events_get(obj, part_name);
	result->repeat_events = edje_edit_part_repeat_events_get(obj, part_name);
	result->ignore_flags = edje_edit_part_ignore_flags_get(obj, part_name);
	result->scale = edje_edit_part_scale_get(obj, part_name);
	result->drag_state_x = edje_edit_part_drag_x_get(obj, part_name);
	result->drag_state_y = edje_edit_part_drag_y_get(obj, part_name);
	result->drag_step_x = edje_edit_part_drag_step_x_get(obj, part_name);
	result->drag_step_y = edje_edit_part_drag_step_y_get(obj, part_name);
	result->drag_count_x = edje_edit_part_drag_count_x_get(obj, part_name);
	result->drag_count_y = edje_edit_part_drag_count_y_get(obj, part_name);
	result->drag_confine = edje_edit_part_drag_confine_get(obj, part_name);
	result->drag_event = edje_edit_part_drag_event_get(obj, part_name);

	wm_part_states_add(obj, result);

	return result;
}

Eina_Bool
wm_part_free(Part *part)
{
	if (!part)
		return EINA_FALSE;

	eina_stringshare_del(part->name);
	edje_edit_string_free(part->api_name);
	edje_edit_string_free(part->description);
	edje_edit_string_free(part->clip_to);
	edje_edit_string_free(part->source);
	edje_edit_string_free(part->selected_state_name);
	edje_edit_string_free(part->drag_confine);
	edje_edit_string_free(part->drag_event);

	wm_part_states_free(part);

	free(part);
	part = NULL;

	return EINA_TRUE;
}

Program *
wm_program_add(Evas_Object* obj, const char* program_name)
{
	Program *program;

	if (!obj || !program_name)
		return NULL;

	program = calloc(1, sizeof(Program));

	program->name = eina_stringshare_add(program_name);
	program->source = edje_edit_program_source_get(obj, program_name);
	program->signal = edje_edit_program_signal_get(obj, program_name);
	program->in_form = edje_edit_program_in_from_get(obj, program_name);
	program->in_range = edje_edit_program_in_range_get(obj, program_name);
	program->action = edje_edit_program_action_get(obj, program_name);
	program->targets = edje_edit_program_targets_get(obj, program_name);
	program->afters = edje_edit_program_afters_get(obj, program_name);
	program->state = edje_edit_program_state_get(obj, program_name);
	program->state2 = edje_edit_program_state2_get(obj, program_name);
	program->api_name = edje_edit_program_api_name_get(obj, program_name);
	program->api_description = edje_edit_program_api_description_get(obj, program_name);
	program->value = edje_edit_program_value_get(obj, program_name);
	program->value2 = edje_edit_program_value2_get(obj, program_name);
	program->transition = edje_edit_program_transition_get(obj, program_name);
	program->transition_time = edje_edit_program_transition_time_get(obj, program_name);
	program->filter_part = edje_edit_program_filter_part_get(obj, program_name);

	return program;
}

Eina_Bool
wm_program_free(Program *program)
{
	if (!program)
		return EINA_FALSE;

	eina_stringshare_del(program->name);
	edje_edit_string_free(program->source);
	edje_edit_string_free(program->signal);
	edje_edit_string_free(program->state);
	edje_edit_string_free(program->state2);
	edje_edit_string_free(program->api_name);
	edje_edit_string_free(program->api_description);
	edje_edit_string_free(program->filter_part);
	edje_edit_string_list_free(program->afters);
	edje_edit_string_list_free(program->targets);

	free(program);
	program = NULL;

	return EINA_TRUE;
}

Eina_List *
wm_program_signals_list_get(const Eina_Inlist *programs)
{
	Program *program;
	Eina_List *result = NULL;

	if (!programs)
		return NULL;

	EINA_INLIST_FOREACH(programs, program)
	{
		if (program->signal != NULL)
			result = eina_list_append(result, program->signal);
	}

	return result;
}

Eina_Bool
wm_program_signals_list_free(Eina_List *signals)
{
	if (!signals)
		return EINA_FALSE;

	eina_list_free(signals);
	signals = NULL;

	return EINA_TRUE;
}

Group *
wm_group_add(const char *group_name, const char *full_group_name)
{
	Group *group_edje;

	if (!full_group_name || !group_name)
		return NULL;

	group_edje = calloc(1, sizeof(Group));
	group_edje->group_name = strdup(group_name);
	group_edje->full_group_name = strdup(full_group_name);
	group_edje->obj = NULL;
	group_edje->parts = NULL;
	group_edje->programs = NULL;
	group_edje->__type = GROUP;

	return group_edje;
}

void
wm_group_data_load(Group *group, Evas *e, const char *edj)
{
	Evas_Object *edje_edit_obj;
	Eina_List *parts_list, *programs_list, *l;
	char *name;
	Part *part;
	Program *program;

	if (!group || !e)
		return;

	edje_edit_obj = edje_edit_object_add(e);

	if (!edje_object_file_set(edje_edit_obj, edj, group->full_group_name))
	{
		evas_object_del(edje_edit_obj);
		return;
	}
	group->obj = edje_edit_obj;
	group->min_w = edje_edit_group_min_w_get(group->obj);
	group->min_h = edje_edit_group_min_h_get(group->obj);
	group->max_w = edje_edit_group_max_w_get(group->obj);
	group->max_h = edje_edit_group_max_h_get(group->obj);
	group->current_w = -1;
	group->current_h = -1;

	parts_list = edje_edit_parts_list_get(group->obj);
	EINA_LIST_FOREACH(parts_list, l, name)
	{
		part = wm_part_add(group->obj, name);
		group->parts = eina_inlist_append(group->parts,
												EINA_INLIST_GET(part));
	}
	edje_edit_string_list_free(parts_list);

	programs_list = edje_edit_programs_list_get(group->obj);
	EINA_LIST_FOREACH(programs_list, l, name)
	{
		program = wm_program_add(group->obj, name);
		group->programs = eina_inlist_append(group->programs,
													EINA_INLIST_GET(program));
	}
	edje_edit_string_list_free(programs_list);
}

Eina_Bool
wm_group_free(Group *group)
{
	Evas *evas;
	Ecore_Evas *buffer;
	Part *part;
	Program *program;

	if (!group)
		return EINA_FALSE;

	if (!group->obj)
	{
		evas = evas_object_evas_get(group->obj);
		buffer = ecore_evas_ecore_evas_get(evas);

		evas_object_del(group->obj);
		ecore_evas_free(buffer);
	}

	while (group->parts)
	{
		part = EINA_INLIST_CONTAINER_GET(group->parts, Part);
		group->parts = eina_inlist_remove(group->parts, group->parts);
		wm_part_free(part);
	}

	while (group->programs)
	{
		program = EINA_INLIST_CONTAINER_GET(group->programs, Program);
		group->programs = eina_inlist_remove(group->programs, group->programs);
		wm_program_free(program);
	}

	if (!group->group_name)
		eina_stringshare_del(group->group_name);
	eina_stringshare_del(group->full_group_name);

	free(group);
	group = NULL;

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
	style_edje->__type = STYLE;

	EINA_LIST_FOREACH(groups, l, group_name_full)
	{
		WM_GROUP_NAME_GET(group_name, style_edje->style_name, group_name_full);
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
	_widget->__type = WIDGET;

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
			style_name_next = &empty;

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
				widget_name_next = &empty;

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
