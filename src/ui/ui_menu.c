#include <ui_main_window.h>

Eina_List *ui_list_menu;

static void
_on_exit_menu (void *data, Evas_Object *obj, void *event_info)
{
	ui_main_window_del();
}

Eina_Bool
ui_menu_add (UI_Data *ud)
{
	Evas_Object *tb, *menu;
	Elm_Object_Item *tb_it;
    Eina_Bool state_errors = EINA_FALSE;

	tb = elm_toolbar_add(ud->win_layout);
	if (tb == NULL)
		state_errors = EINA_TRUE;
	else
		ui_list_menu = eina_list_append(ui_list_menu, tb);

	evas_object_size_hint_weight_set(tb, 0.0, 0.0);
	elm_toolbar_shrink_mode_set (tb, ELM_TOOLBAR_SHRINK_NONE);
	elm_toolbar_align_set (tb, 0.0);
	elm_toolbar_homogeneous_set (tb, EINA_TRUE);
	elm_object_part_content_set (ud->win_layout, "swallow/toolbar", tb);
	evas_object_show(tb);
	elm_toolbar_icon_size_set (tb,16);

	elm_toolbar_icon_order_lookup_set (tb, ELM_ICON_LOOKUP_FDO_THEME);
	tb_it = elm_toolbar_item_append(tb, "menu/arrow_down", "File", NULL, NULL);
	if (tb_it == NULL)
		state_errors = EINA_TRUE;
	elm_toolbar_item_menu_set(tb_it, EINA_TRUE);
	menu = elm_toolbar_item_menu_get(tb_it);
	if (menu == NULL)
		state_errors = EINA_TRUE;
	else
		ui_list_menu = eina_list_append(ui_list_menu, menu);

	elm_menu_item_add(menu, NULL, "menu/folder", "Open", NULL, NULL);
	elm_menu_item_add(menu, NULL, "menu/file", "Save", NULL, NULL);
	elm_menu_item_add(menu, NULL, "menu/close", "Exit", _on_exit_menu, NULL);

	tb_it=elm_toolbar_item_append(tb,"menu/arrow_down", "Options", NULL, NULL);
	elm_toolbar_item_menu_set(tb_it, EINA_TRUE);
	menu = elm_toolbar_item_menu_get(tb_it);
	elm_menu_item_add(menu, NULL, "menu/apps", "Settings", NULL, NULL);
	elm_toolbar_menu_parent_set(tb, ud->win_layout);

	tb_it = elm_toolbar_item_append (tb, NULL, "Separator", NULL, NULL);
	elm_toolbar_item_separator_set (tb_it, EINA_TRUE);

	elm_toolbar_icon_order_lookup_set (tb, ELM_ICON_LOOKUP_THEME);

	elm_toolbar_item_append(tb, "file", "File", NULL, NULL);
	elm_toolbar_item_append(tb, "folder", "Folder", NULL, NULL);
	elm_toolbar_item_append(tb, "edit", "Edit", NULL, NULL);
	elm_toolbar_item_append(tb, "refresh", "Refresh", NULL, NULL);

	ud->main_menu = tb;
	return ~state_errors;
}

Eina_Bool
ui_menu_del (void)
{
	Evas_Object *deleting_element;
	Eina_Bool state_errors = EINA_FALSE;
	Eina_List *l;

    EINA_LIST_FOREACH(ui_list_menu, l, deleting_element){
		evas_object_del (deleting_element);
		if (deleting_element!=NULL)
			state_errors = EINA_TRUE;
	}
	eina_list_free(ui_list_menu);
	eina_list_free(l);
	return ~state_errors;
}

