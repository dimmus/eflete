#include <ui_main_window.h>
#include <ui_settings.h>

void _print_current_state(char *);

struct _Panes_Attributes
{
	int left_size;
	int right_size;
};
typedef struct _Panes_Attributes Panes_Attributes;

struct _Window_Attributes
{
	int width;
	int height;
};
typedef struct _Window_Attributes Window_Attributes;

struct _UI_Current_State_Panes
{
	Panes_Attributes panes_left;
	Panes_Attributes panes_left_hor;
	Panes_Attributes panes_right;
	Panes_Attributes panes_right_hor;
	Panes_Attributes panes_center;
	Panes_Attributes panes_center_down;
	Window_Attributes window;
};
typedef struct _UI_Current_State_Panes UI_Current_State_Panes;

Eina_List *ui_list_panes;
UI_Current_State_Panes *ui_csp;
UI_Elements_Settings *us;

UI_Current_State_Panes *
_ui_panes_current_state_struct_init (void)
{
	UI_Current_State_Panes *csp = calloc (1,sizeof(UI_Current_State_Panes));
	csp->panes_left.left_size = 0;
	csp->panes_left.right_size = 0;

	csp->panes_left_hor.left_size = 0;
	csp->panes_left_hor.right_size = 0;

	csp->panes_right.left_size = 0;
	csp->panes_right.right_size = 0;

	csp->panes_right_hor.left_size = 0;
	csp->panes_right_hor.right_size = 0;

	csp->panes_center.left_size = 0;
	csp->panes_center.right_size = 0;

	csp->panes_center_down.left_size = 0;
	csp->panes_center_down.right_size = 0;

	csp->window.width = 1000;
	csp->window.height = 800;

	return csp;
}

void
ui_panes_current_state_get (void)
{
	double size_get=0.0;
	Eina_List *l = ui_list_panes;

	size_get = elm_panes_content_left_size_get ((Evas_Object *)l->data);
	ui_csp->panes_left.left_size = (int)(ui_csp->window.width * size_get);

	size_get = elm_panes_content_right_size_get ((Evas_Object *)l->data);
	ui_csp->panes_left.right_size = (int)(ui_csp->window.width * size_get);

	l = eina_list_next (l);
	size_get = elm_panes_content_left_size_get ((Evas_Object *)l->data);
	ui_csp->panes_left_hor.left_size =
		(int)((ui_csp->window.height-20) * size_get);
	size_get = elm_panes_content_right_size_get ((Evas_Object *)l->data);
	ui_csp->panes_left_hor.right_size =
		(int)((ui_csp->window.height-20) * size_get);

	l = eina_list_next (l);
	size_get = elm_panes_content_left_size_get ((Evas_Object *)l->data);
	ui_csp->panes_right.left_size =
		(int)((ui_csp->window.width - ui_csp->panes_left.left_size)* size_get);
	size_get = 1 - elm_panes_content_left_size_get ((Evas_Object *)l->data);
	ui_csp->panes_right.right_size =
		(int)((ui_csp->window.width-ui_csp->panes_left.left_size)* size_get);

	l = eina_list_next (l);
	size_get = elm_panes_content_left_size_get ((Evas_Object *)l->data);
	ui_csp->panes_right_hor.left_size =
		(int)((ui_csp->window.height-20)*size_get);
	size_get = elm_panes_content_right_size_get ((Evas_Object *)l->data);
	ui_csp->panes_right_hor.right_size =
		(int)((ui_csp->window.height-20) * size_get);

	l = eina_list_next (l);
	size_get = elm_panes_content_left_size_get ((Evas_Object *)l->data);
	ui_csp->panes_center.left_size =
		(int)((ui_csp->window.height-20)* size_get);
	size_get = elm_panes_content_right_size_get ((Evas_Object *)l->data);
	ui_csp->panes_center.right_size =
		(int)((ui_csp->window.height -20)* size_get);

	l = eina_list_next (l);
	size_get = elm_panes_content_left_size_get ((Evas_Object *)l->data);
	ui_csp->panes_center_down.left_size=
		(int)(ui_csp->panes_right.left_size*size_get);
	size_get =elm_panes_content_right_size_get ((Evas_Object *)l->data);
	ui_csp->panes_center_down.right_size=
		(int)(ui_csp->panes_right.left_size*size_get);

}

void
ui_panes_current_state_set (void)
{
	Eina_List *l = ui_list_panes;
	double size_set = 0.0;
	size_set = (double)	(ui_csp->panes_left.left_size) / (ui_csp->window.width);
	elm_panes_content_left_size_set ((Evas_Object *)l->data,size_set);
	l = eina_list_next (l);
	l = eina_list_next (l);

	size_set = (double) (ui_csp->panes_right.right_size)/
		(ui_csp->window.width-ui_csp->panes_left.left_size);

	elm_panes_content_right_size_set ((Evas_Object*)l->data,size_set);
	l = eina_list_next (l);
	l = eina_list_next (l);
	size_set = (double)(ui_csp->panes_center.right_size)/
		(ui_csp->window.height-20);
	elm_panes_content_right_size_set ((Evas_Object *)l->data,size_set);
	l = eina_list_next (l);
}

void
_print_current_state (char *source)
{
	fprintf (stdout,"INFO: %s\n", source);
	fprintf (stdout, "Left        l[%4d] r[%4d]\n\
Left_hor    l[%4d] r[%4d]\nCenter      l[%4d] r[%4d]\nCenter_down l[%4d] r[%4d]\n\
Right       l[%4d] r[%4d]\nRight_hor   l[%4d] r[%4d]\n\
Window      w[%4d] h[%4d]\n\n",
		ui_csp->panes_left.left_size, ui_csp->panes_left.right_size,
		ui_csp->panes_left_hor.left_size, ui_csp->panes_left_hor.right_size,
		ui_csp->panes_center.left_size,ui_csp->panes_center.right_size,
		ui_csp->panes_center_down.left_size,ui_csp->panes_center_down.right_size,
		ui_csp->panes_right.left_size, ui_csp->panes_right.right_size,
		ui_csp->panes_right_hor.left_size, ui_csp->panes_right_hor.right_size,
		ui_csp->window.width, ui_csp->window.height);
}

void
ui_resize_pans (int w, int h)
{
	ui_csp->window.height = h;
	ui_csp->window.width = w;
	ui_panes_current_state_set();
}

static void
_unpress(void *data, Evas_Object *obj, void *event_info)
{
	ui_panes_current_state_get ();
}

void
ui_panes_settings_save ()
{
	ui_panes_current_state_set();
	ui_settings_save();
}

void
ui_panes_settings_load (Evas_Object *win)
{
	ui_settings_load();
	ui_panes_current_state_get ();
}

Eina_Bool
ui_panes_add (App_Data *ap)
{
	Evas_Object *scroller;
	Evas_Object *panes_left, *panes_left_hor, *panes_right;
	Evas_Object *panes_center, *panes_center_down, *panes_right_hor;

	us = ui_element_settings_init();
	if (!us){
		fprintf (stdout, "ERROR: unable initialize settings module");
		return EINA_FALSE;
	}

	panes_left = elm_panes_add(ap->win_layout);
    if (panes_left == NULL)
		return EINA_FALSE;
	ui_list_panes = eina_list_append(ui_list_panes, panes_left);
	evas_object_size_hint_weight_set(panes_left,
		EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(panes_left,
		EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_panes_content_left_size_set(panes_left, 0.2);
	elm_panes_fixed_set (panes_left, EINA_FALSE);
//	evas_object_show(panes_left);

	elm_object_part_content_set (ap->win_layout, "swallow/panes", panes_left);

	panes_left_hor = elm_panes_add(ap->win_layout);
	if (panes_left_hor == NULL)
		return EINA_FALSE;
	ui_list_panes = eina_list_append(ui_list_panes, panes_left_hor);
	elm_panes_horizontal_set (panes_left_hor, EINA_TRUE);
	evas_object_size_hint_weight_set(panes_left_hor,
		EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(panes_left_hor,
		EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_object_part_content_set(panes_left, "left", panes_left_hor);
	elm_panes_content_left_size_set(panes_left_hor, 0.45);
	evas_object_show(panes_left_hor);

	panes_right = elm_panes_add(ap->win_layout);
	if (panes_right == NULL)
			return EINA_FALSE;
	ui_list_panes = eina_list_append(ui_list_panes, panes_right);
	evas_object_size_hint_weight_set(panes_right,
		EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(panes_right,
		EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_object_part_content_set(panes_left, "right", panes_right);
	evas_object_show(panes_right);
	elm_panes_content_left_size_set(panes_right, 0.75);

	panes_right_hor = elm_panes_add(ap->win_layout);
	if (panes_right_hor == NULL)
		return EINA_FALSE;
	ui_list_panes = eina_list_append(ui_list_panes, panes_right_hor);
	elm_panes_horizontal_set (panes_right_hor, EINA_TRUE);
	evas_object_size_hint_weight_set(panes_right_hor,
		EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(panes_right_hor,
		EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_object_part_content_set(panes_right, "right", panes_right_hor);
	elm_panes_content_left_size_set(panes_right_hor, 0.45);
	evas_object_show(panes_right_hor);

	panes_center = elm_panes_add(ap->win_layout);
	if (panes_center == NULL)
		return EINA_FALSE;
	ui_list_panes = eina_list_append(ui_list_panes, panes_center);
	elm_panes_horizontal_set(panes_center, EINA_TRUE);
	evas_object_size_hint_weight_set(panes_center,
		EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(panes_center,
		EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_object_part_content_set(panes_right, "left", panes_center);
	elm_panes_content_left_size_set(panes_center, 0.65);
	evas_object_show(panes_center);

	panes_center_down = elm_panes_add(ap->win_layout);
	if (panes_center_down == NULL)
		return EINA_FALSE;
	ui_list_panes = eina_list_append(ui_list_panes, panes_center_down);
	evas_object_size_hint_weight_set(panes_center_down,
		EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(panes_center_down,
		EVAS_HINT_FILL, EVAS_HINT_FILL);
	elm_object_part_content_set(panes_center, "right", panes_center_down);
	evas_object_show(panes_center_down);

	us->panes_left = panes_left;
	us->panes_right = panes_right;
	us->panes_left_hor = panes_left_hor;
	us->panes_right_hor = panes_right_hor;
	us->panes_center = panes_center;
	us->panes_center_down = panes_center_down;
	us->window = ap->win;


	evas_object_smart_callback_add(panes_left, "unpress", _unpress, NULL);
	evas_object_smart_callback_add(panes_left_hor, "unpress", _unpress, NULL);
	evas_object_smart_callback_add(panes_right, "unpress", _unpress, NULL);
	evas_object_smart_callback_add(panes_right_hor, "unpress", _unpress, NULL);
	evas_object_smart_callback_add(panes_center, "unpress", _unpress, NULL);
	evas_object_smart_callback_add(panes_center_down, "unpress", _unpress, NULL);

	scroller = elm_genlist_add(ap->win_layout);
	elm_object_part_content_set(panes_center_down, "left", scroller);
	evas_object_show(scroller);
	ap->block_bottom_left = scroller;

	scroller = elm_scroller_add(ap->win_layout);
	elm_object_part_content_set(panes_center_down, "right", scroller);
	evas_object_show(scroller);
	ap->block_bottom_right = scroller;

	scroller = elm_layout_add(ap->win_layout);
	elm_object_part_content_set(panes_center, "left", scroller);
	evas_object_show(scroller);
	ap->block_canvas = scroller;

	scroller = elm_genlist_add(ap->win_layout);
	elm_object_part_content_set(panes_left_hor, "left", scroller);
	evas_object_show(scroller);
	ap->block_left_top = scroller;

	scroller = elm_genlist_add(ap->win_layout);
	elm_object_part_content_set(panes_left_hor, "right", scroller);
	evas_object_show(scroller);
	ap->block_left_bottom = scroller;

	scroller = elm_scroller_add(ap->win_layout);
	elm_object_part_content_set(panes_right_hor, "left", scroller);
	evas_object_show(scroller);
	ap->block_right_top = scroller;

	scroller = elm_genlist_add(ap->win_layout);
	elm_object_part_content_set(panes_right_hor, "right", scroller);
	evas_object_show(scroller);
	ap->block_right_bottom = scroller;

	ui_csp =_ui_panes_current_state_struct_init ();
	return EINA_TRUE;
}

Eina_Bool
ui_panes_del ()
{
	Evas_Object *deleting_element;
	Eina_List *l;
    EINA_LIST_FOREACH(ui_list_panes, l, deleting_element){
		evas_object_del (deleting_element);
		if (!deleting_element) return EINA_FALSE;
	}
	eina_list_free(ui_list_panes);
	eina_list_free(l);
	return EINA_TRUE;
}


