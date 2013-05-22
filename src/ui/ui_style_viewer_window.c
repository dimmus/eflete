#include <ui_main_window.h>
#include "ui_style_viewer_window.h"

/* TODO: collect all controls (buttons, list, entries) for further FREEing */
Eina_List *ui_list_controls;
static Elm_Genlist_Item_Class *_itc = NULL;

static void
_on_viewer_exit(void *data, Evas_Object *obj __UNUSED__,
				void *event_info __UNUSED__)
{
	Evas_Object *inwin;

	inwin = (Evas_Object *) data;

	evas_object_hide(inwin);
	/*TODO: add some FREE function for controls or so. */
}

Evas_Object*
_form_left_side(Evas_Object* win) {
	Evas_Object *layout, *list, *btn;

	layout = elm_layout_add(win);
	evas_object_size_hint_weight_set(layout,
		EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_layout_file_set(layout, TET_EDJ, "ui/style_viewer_window/list");
	evas_object_show(layout);

	/* TODO: look through exist list in ui_widget_list 
			 and do this list correctly */
    if (!_itc)
    {
       _itc = elm_genlist_item_class_new();
       _itc->item_style = "default";
       _itc->func.text_get = NULL;
       _itc->func.content_get = NULL;
       _itc->func.state_get = NULL;
       _itc->func.del = NULL;
    }

	/* TODO: add some functions here and don't forget to 
			 checkout the edc file */
	list = elm_genlist_add(win);
	elm_object_part_content_set (layout, "swallow/genlist", list);
	evas_object_show(list);

	btn = elm_button_add(win);
	elm_object_text_set(btn, "New Style");
	elm_object_part_content_set (layout, "swallow/button_add_style", btn);
	evas_object_show(btn);

	btn = elm_button_add(win);
	elm_object_text_set(btn, "Delete Style");
	elm_object_part_content_set (layout, "swallow/button_rm_style", btn);
	evas_object_show(btn);

	btn = elm_button_add(win);
	elm_object_text_set(btn, "New Tag");
	elm_object_part_content_set (layout, "swallow/button_add_tag", btn);
	evas_object_show(btn);

	btn = elm_button_add(win);
	elm_object_text_set(btn, "Delete Tag");
	elm_object_part_content_set (layout, "swallow/button_rm_tag", btn);
	evas_object_show(btn);

	return layout;
}

Evas_Object*
_form_right_side(Evas_Object* win) {
	Evas_Object *layout, *entry_tag, *entry_prop, *btn;

	layout = elm_layout_add(win);
	evas_object_size_hint_weight_set(layout,
		EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_layout_file_set(layout, TET_EDJ, "ui/style_viewer_window/property");
	evas_object_show(layout);

	/* TODO: checkout the entry. Is it correct code? */
	entry_tag = elm_entry_add(win);
	elm_entry_single_line_set(entry_tag, EINA_TRUE);
	elm_object_part_content_set (layout, "swallow/tag_entry", entry_tag);
	elm_entry_scrollable_set(entry_tag, EINA_TRUE);
	evas_object_show(entry_tag);

	entry_prop = elm_entry_add(win);
	elm_entry_single_line_set(entry_prop, EINA_TRUE);
	elm_object_part_content_set (layout, "swallow/prop_entry", entry_prop);
	elm_entry_scrollable_set(entry_prop, EINA_TRUE);
	evas_object_show(entry_prop);

	btn = elm_button_add(win);
	elm_object_text_set(btn, "Close Viewer");
	elm_object_part_content_set (layout, "swallow/button_close", btn);
	evas_object_smart_callback_add(btn, "clicked", _on_viewer_exit, win);

	evas_object_show(btn);

	return layout;
}

void
style_viewer_init (App_Data *ap) {
	Evas_Object *inwin;
	Evas_Object *panes, *panes_h, *entry_style;
	Evas_Object *layout_left, *layout_right;


	/* TODO: should I check loaded project like that? */
	if (ap->project != NULL) {

		inwin = elm_win_inwin_add(ap->win);

		panes = elm_panes_add(inwin);
		evas_object_size_hint_weight_set(panes, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		evas_object_size_hint_align_set(panes, EVAS_HINT_FILL, EVAS_HINT_FILL);
		elm_panes_content_left_size_set(panes, 0.2);
		elm_win_inwin_content_set(inwin, panes);
		evas_object_show(panes);

		layout_left = _form_left_side(inwin);
	    elm_object_part_content_set(panes, "left", layout_left);
		evas_object_show(layout_left);

		panes_h = elm_panes_add(inwin);
		evas_object_size_hint_weight_set(panes_h, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		evas_object_size_hint_align_set(panes_h, EVAS_HINT_FILL, EVAS_HINT_FILL);
		elm_panes_horizontal_set(panes_h, EINA_TRUE);
	    elm_object_part_content_set(panes, "right", panes_h);
		evas_object_show(panes_h);

		entry_style = elm_entry_add(inwin);
		evas_object_size_hint_weight_set(entry_style, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		evas_object_size_hint_align_set(entry_style, EVAS_HINT_FILL, EVAS_HINT_FILL);
		elm_entry_scrollable_set(entry_style, EINA_TRUE);
		elm_object_text_set(entry_style, "The quick brown fox jumps over the lazy dog");
	    elm_object_part_content_set(panes_h, "left", entry_style);
		evas_object_show(entry_style);

		layout_right = _form_right_side(inwin);
	    elm_object_part_content_set(panes_h, "right", layout_right);
		evas_object_show(layout_right);

		evas_object_show(inwin);
	}
}
