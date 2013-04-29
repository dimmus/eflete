#include <ui_main_window.h>

Eina_List *ui_list_main_window;

static void
_on_done(void *data, Evas_Object *obj, void *event_info)
{
	ui_main_window_del();
}

void
ui_main_window_del ()
{
	Evas_Object *deleting_element;
	Eina_List *l;

	if (!ui_menu_del())
		fprintf (stdout, "Failrue on deleting menu elements.");
	if (!ui_panes_del())
		fprintf (stdout, "Failrue on deleting panes.");
	EINA_LIST_FOREACH(ui_list_main_window, l, deleting_element){
		evas_object_del (deleting_element);
	}
	eina_list_free(ui_list_main_window);
	eina_list_free(l);

	elm_exit();
}

Eina_Bool
ui_main_window_add (UI_Data *ud)
{
	Evas_Object *win, *bg, *layout;

	eina_init();

	win = elm_win_add(NULL, "panes", ELM_WIN_BASIC);
    if (win == NULL) {
		fprintf (stdout,"Failrue create main window.");
		return EINA_FALSE;
	}
	ui_list_main_window = eina_list_append(ui_list_main_window, win);

	ud->win = win;

	elm_win_title_set(win, PACKAGE);
	evas_object_smart_callback_add(win, "delete,request", _on_done, NULL);

	bg = elm_bg_add(win);
	ui_list_main_window = eina_list_append(ui_list_main_window, bg);
	elm_win_resize_object_add(win, bg);
	evas_object_size_hint_weight_set(bg,
		EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_focus_highlight_enabled_set(win, EINA_FALSE);
	evas_object_show(bg);

	layout = elm_layout_add(win);
	ui_list_main_window = eina_list_append(ui_list_main_window, layout);
	evas_object_size_hint_weight_set(layout,
		EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(win, layout);
	elm_layout_file_set(layout, TET_EDJ, "ui/main_window");
	evas_object_show(layout);
	ud->win_layout = layout;

	if(!ui_menu_add(ud))
		fprintf(stdout,"Failrue add menu on main window.");

	if(!ui_panes_add(ud))
		fprintf(stdout,"Failrue add panes on main window.");

	evas_object_resize(win, 1200, 800);
	evas_object_show(win);

	return EINA_TRUE;
}
