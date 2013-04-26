#include <ui_main_window.h>

Eina_List *ui_list_panes;

void
ui_panes_settings_save ()
{
	_panes_settings_save(ui_list_panes);
}

void
ui_panes_settings_load ()
{
	_panes_settings_load (ui_list_panes);
}

Eina_Bool
ui_panes_add (Evas_Object *layout)
{
	Evas_Object *rect;
	Evas_Object *panes_left, *panes_left_hor, *panes_right;
	Evas_Object *panes_center, *panes_center_down, *panes_right_hor;
	Eina_Bool state_errors = EINA_FALSE;

	panes_left = elm_panes_add(layout);
    if (panes_left == NULL)
	state_errors = EINA_TRUE;
	else
		ui_list_panes = eina_list_append(ui_list_panes, panes_left);

	evas_object_size_hint_weight_set(panes_left,
		EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(panes_left,
		EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_panes_content_left_size_set(panes_left, 0.2);
	evas_object_show(panes_left);

	elm_object_part_content_set (layout, "swallow/panes", panes_left);

	panes_left_hor = elm_panes_add(layout);
	if (panes_left_hor == NULL)
		state_errors = EINA_TRUE;
	else
		ui_list_panes = eina_list_append(ui_list_panes, panes_left_hor);
	elm_panes_horizontal_set (panes_left_hor, EINA_TRUE);
	evas_object_size_hint_weight_set(panes_left_hor,
		EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(panes_left_hor,
		EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_object_part_content_set(panes_left, "left", panes_left_hor);
	elm_panes_content_left_size_set(panes_left_hor, 0.45);
	evas_object_show(panes_left_hor);

	panes_right = elm_panes_add(layout);
	if (panes_right == NULL)
		state_errors = EINA_TRUE;
	else
		ui_list_panes = eina_list_append(ui_list_panes, panes_right);
	evas_object_size_hint_weight_set(panes_right,
		EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(panes_right,
		EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_object_part_content_set(panes_left, "right", panes_right);
	evas_object_show(panes_right);
	elm_panes_content_left_size_set(panes_right, 0.75);

	panes_right_hor = elm_panes_add(layout);
	if (panes_right_hor == NULL)
		state_errors = EINA_TRUE;
	else
		ui_list_panes = eina_list_append(ui_list_panes, panes_right_hor);
	elm_panes_horizontal_set (panes_right_hor, EINA_TRUE);
	evas_object_size_hint_weight_set(panes_right_hor,
		EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(panes_right_hor,
		EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_object_part_content_set(panes_right, "right", panes_right_hor);
	elm_panes_content_left_size_set(panes_right_hor, 0.45);
	evas_object_show(panes_right_hor);

	panes_center = elm_panes_add(layout);
	if (panes_center == NULL)
		state_errors = EINA_TRUE;
	else
		ui_list_panes = eina_list_append(ui_list_panes, panes_center);
	elm_panes_horizontal_set(panes_center, EINA_TRUE);
	evas_object_size_hint_weight_set(panes_center,
		EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(panes_center,
		EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_object_part_content_set(panes_right, "left", panes_center);
	elm_panes_content_left_size_set(panes_center, 0.65);
	evas_object_show(panes_center);

	panes_center_down = elm_panes_add(layout);
	if (panes_center_down == NULL)
		state_errors = EINA_TRUE;
	else
		ui_list_panes = eina_list_append(ui_list_panes, panes_center_down);
	evas_object_size_hint_weight_set(panes_center_down,
		EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(panes_center_down,
		EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_object_part_content_set(panes_center, "right", panes_center_down);
	evas_object_show(panes_center_down);

	rect = evas_object_rectangle_add(evas_object_evas_get(layout));
	evas_object_color_set(rect, 20, 100, 128, 200);
	elm_object_part_content_set(panes_center_down, "left", rect);
	evas_object_show(rect);

	rect = evas_object_rectangle_add(evas_object_evas_get(layout));
	evas_object_color_set(rect, 20, 100, 128, 200);
	elm_object_part_content_set(panes_center_down, "right", rect);
	evas_object_show(rect);

	rect = evas_object_rectangle_add(evas_object_evas_get(layout));
	evas_object_color_set(rect, 128, 128, 128, 200);
	elm_object_part_content_set(panes_center, "left", rect);
	evas_object_show(rect);

	rect = evas_object_rectangle_add(evas_object_evas_get(layout));
	evas_object_color_set(rect, 0, 128, 50, 200);
	elm_object_text_set(rect, "left_up");
	elm_object_part_content_set(panes_left_hor, "left", rect);
	evas_object_show(rect);

	rect = evas_object_rectangle_add(evas_object_evas_get(layout));
	evas_object_color_set(rect, 0, 128, 50, 200);
	elm_object_part_content_set(panes_left_hor, "right", rect);
	evas_object_show(rect);

	rect = evas_object_rectangle_add(evas_object_evas_get(layout));
	evas_object_color_set(rect, 128, 128, 0, 200);
	elm_object_part_content_set(panes_right_hor, "left", rect);
	evas_object_show(rect);

	rect = evas_object_rectangle_add(evas_object_evas_get(layout));
	evas_object_color_set(rect, 128, 128, 0, 200);
	elm_object_part_content_set(panes_right_hor, "right", rect);
	evas_object_show(rect);
//	ui_panes_settings_save ();
	return ~state_errors;
}

Eina_Bool
ui_panes_del ()
{
	Evas_Object *deleting_element;
	Eina_Bool state_errors = EINA_FALSE;
	Eina_List *l;

    EINA_LIST_FOREACH(ui_list_panes, l, deleting_element){
		evas_object_del (deleting_element);
		if (deleting_element!=NULL)
			state_errors = EINA_TRUE;
	}
	eina_list_free(ui_list_panes);
	eina_list_free(l);
	return ~state_errors;
}

