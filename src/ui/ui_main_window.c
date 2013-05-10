#include <ui_main_window.h>
#include <ui_settings.h>

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
	Eina_List *l = ui_list_main_window;

	ui_panes_settings_save();
	if (!ui_menu_del())
		WARN ( "Something wrong on delete menu elements.\n");
	if (!ui_panes_del())
		WARN ("Something wrong on delete panes.\n");
	EINA_LIST_FOREACH(ui_list_main_window, l, deleting_element){
		evas_object_del (deleting_element);
	}
	eina_list_free(ui_list_main_window);
	eina_list_free(l);

	elm_exit();
}

static void
_on_window_resize (void *data, Evas *e, Evas_Object *obj, void *event_info)
{
	int w, h;
	evas_object_geometry_get (obj, NULL, NULL, &w, &h);
	ui_resize_pans(w,h);
}

Eina_Bool
ui_main_window_add (App_Data *ap)
{
	Evas_Object *win, *bg, *layout;

	eina_init();

	win = elm_win_add(NULL, "panes", ELM_WIN_BASIC);
    if (win == NULL) {
		ERR("Failrue create main window.");
		return EINA_FALSE;
	}
	ui_list_main_window = eina_list_append(ui_list_main_window, win);

	ap->win = win;

	elm_win_title_set(win, PACKAGE);
	evas_object_smart_callback_add (win, "delete,request", _on_done, NULL);
	evas_object_event_callback_add (win,
		EVAS_CALLBACK_RESIZE,
		_on_window_resize,
		NULL);

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
	ap->win_layout = layout;

	if(!ui_menu_add(ap))
		ERR("Failrue add menu on main window.");

	if(!ui_panes_add(ap))
		ERR("Failrue add panes on main window.");

	ui_panes_settings_load(win);
	evas_object_show(win);


	return EINA_TRUE;
}
