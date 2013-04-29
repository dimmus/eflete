#include <ui_main_window.h>
#include <ui_settings.h>

Eina_List *ui_list_panes;

static void
_unpress(void *data, Evas_Object *obj, void *event_info)
{
	ui_panes_settings_save();
}

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
ui_panes_add (UI_Data *ud)
{
	Evas_Object *scroller;
	Evas_Object *panes_left, *panes_left_hor, *panes_right;
	Evas_Object *panes_center, *panes_center_down, *panes_right_hor;
    Eina_Bool state_errors = EINA_FALSE;

	panes_left = elm_panes_add(ud->win_layout);
    if (panes_left == NULL)
		state_errors = EINA_TRUE;
	ui_list_panes = eina_list_append(ui_list_panes, panes_left);
	evas_object_size_hint_weight_set(panes_left,
		EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(panes_left,
		EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_panes_content_left_size_set(panes_left, 0.2);
	evas_object_show(panes_left);

	elm_object_part_content_set (ud->win_layout, "swallow/panes", panes_left);

	panes_left_hor = elm_panes_add(ud->win_layout);
	if (panes_left_hor == NULL)
		state_errors = EINA_TRUE;
	ui_list_panes = eina_list_append(ui_list_panes, panes_left_hor);
	elm_panes_horizontal_set (panes_left_hor, EINA_TRUE);
	evas_object_size_hint_weight_set(panes_left_hor,
		EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(panes_left_hor,
		EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_object_part_content_set(panes_left, "left", panes_left_hor);
	elm_panes_content_left_size_set(panes_left_hor, 0.45);
	evas_object_show(panes_left_hor);

	panes_right = elm_panes_add(ud->win_layout);
	if (panes_right == NULL)
		state_errors = EINA_TRUE;
	ui_list_panes = eina_list_append(ui_list_panes, panes_right);
	evas_object_size_hint_weight_set(panes_right,
		EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(panes_right,
		EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_object_part_content_set(panes_left, "right", panes_right);
	evas_object_show(panes_right);
	elm_panes_content_left_size_set(panes_right, 0.75);

	panes_right_hor = elm_panes_add(ud->win_layout);
	if (panes_right_hor == NULL)
		state_errors = EINA_TRUE;
	ui_list_panes = eina_list_append(ui_list_panes, panes_right_hor);
	elm_panes_horizontal_set (panes_right_hor, EINA_TRUE);
	evas_object_size_hint_weight_set(panes_right_hor,
		EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(panes_right_hor,
		EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_object_part_content_set(panes_right, "right", panes_right_hor);
	elm_panes_content_left_size_set(panes_right_hor, 0.45);
	evas_object_show(panes_right_hor);

	panes_center = elm_panes_add(ud->win_layout);
	if (panes_center == NULL)
		state_errors = EINA_TRUE;
	ui_list_panes = eina_list_append(ui_list_panes, panes_center);
	elm_panes_horizontal_set(panes_center, EINA_TRUE);
	evas_object_size_hint_weight_set(panes_center,
		EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(panes_center,
		EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_object_part_content_set(panes_right, "left", panes_center);
	elm_panes_content_left_size_set(panes_center, 0.65);
	evas_object_show(panes_center);

	panes_center_down = elm_panes_add(ud->win_layout);
	if (panes_center_down == NULL)
		state_errors = EINA_TRUE;
	ui_list_panes = eina_list_append(ui_list_panes, panes_center_down);
	evas_object_size_hint_weight_set(panes_center_down,
		EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(panes_center_down,
		EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_object_part_content_set(panes_center, "right", panes_center_down);
	evas_object_show(panes_center_down);

	evas_object_smart_callback_add(panes_left, "unpress", _unpress, NULL);
	evas_object_smart_callback_add(panes_left_hor, "unpress", _unpress, NULL);
	evas_object_smart_callback_add(panes_right, "unpress", _unpress, NULL);
	evas_object_smart_callback_add(panes_right_hor, "unpress", _unpress, NULL);
	evas_object_smart_callback_add(panes_center, "unpress", _unpress, NULL);
	evas_object_smart_callback_add(panes_center_down, "unpress", _unpress, NULL);

	scroller = elm_scroller_add(ud->win_layout);
	elm_object_part_content_set(panes_center_down, "left", scroller);
	evas_object_show(scroller);
	ud->block_bottom_left = scroller;

	scroller = elm_scroller_add(ud->win_layout);
	elm_object_part_content_set(panes_center_down, "right", scroller);
	evas_object_show(scroller);
	ud->block_bottom_right = scroller;

	scroller = elm_scroller_add(ud->win_layout);
	elm_object_part_content_set(panes_center, "left", scroller);
	evas_object_show(scroller);
	ud->block_canvas = scroller;

	scroller = elm_scroller_add(ud->win_layout);
	elm_object_part_content_set(panes_left_hor, "left", scroller);
	evas_object_show(scroller);
	ud->block_left_top = scroller;

	scroller = elm_scroller_add(ud->win_layout);
	elm_object_part_content_set(panes_left_hor, "right", scroller);
	evas_object_show(scroller);
	ud->block_left_bottom = scroller;

	scroller = elm_scroller_add(ud->win_layout);
	elm_object_part_content_set(panes_right_hor, "left", scroller);
	evas_object_show(scroller);
	ud->block_right_top = scroller;

	scroller = elm_scroller_add(ud->win_layout);
	elm_object_part_content_set(panes_right_hor, "right", scroller);
	evas_object_show(scroller);
	ud->block_right_bottom = scroller;

	ui_panes_settings_load ();
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


