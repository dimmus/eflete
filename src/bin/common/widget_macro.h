#ifndef WIDGET_MACRO_H
#define WIDGET_MACRO_H

#ifdef HAVE_TIZEN
static void
__UNUSED__ _combobox_widget_expanded_cb(void *data __UNUSED__,
                             Evas_Object *obj,
                             void *ei __UNUSED__)
{
   edje_object_signal_emit(elm_layout_edje_get(obj), "expanded", "elm");
}

static void
__UNUSED__ _combobox_widget_dismissed_cb(void *data __UNUSED__,
                              Evas_Object *obj,
                              void *ei __UNUSED__)
{
   edje_object_signal_emit(elm_layout_edje_get(obj), "dismissed", "elm");
}
#endif

TODO("see large comment below")
/**
 * ITEM_ADD use old kind of adding items and style that looks like
 * Add layout with name "eflete/property/item/swallow".
 * This is old kind of layout that is commonly used across whole project.
 * We are moving to new kind of adding items where styles looks more like this:
 * "elm/layout/property/swallow".
 *
 * So, when refactoring and moving will be done, please delete old ITEM_ADD and
 * rename NEW_ITEM_ADD to ITEM_ADD.
 *
 * Currently this macro is used in live view prop, but can be also used in
 * property, and everywhere where special item layouts are required.
 */
#define LAYOUT_PROP_ADD(PARENT, NAME, GROUP, STYLE) \
   item = elm_layout_add(PARENT); \
   elm_layout_theme_set(item, "layout", GROUP, STYLE); \
   evas_object_size_hint_weight_set(item, EVAS_HINT_EXPAND, 0.0); \
   evas_object_size_hint_align_set(item, EVAS_HINT_FILL, 0.0); \
   elm_layout_text_set(item, NULL, NAME); \
   evas_object_show(item);

#define BOX_ADD(PARENT, BOX, ISHORIZONTAL, ISHOMOGENEOS) \
   BOX = elm_box_add(PARENT); \
   evas_object_size_hint_weight_set(BOX, EVAS_HINT_EXPAND, 0.0); \
   evas_object_size_hint_align_set(BOX, EVAS_HINT_FILL, 0.0); \
   elm_box_horizontal_set(BOX, ISHORIZONTAL); \
   elm_box_homogeneous_set(BOX, ISHOMOGENEOS); \
   evas_object_show(BOX);

#define SCROLLER_ADD(PARENT, SCROLLER) \
   SCROLLER = elm_scroller_add(PARENT); \
   evas_object_size_hint_weight_set(SCROLLER, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_size_hint_align_set(SCROLLER, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   elm_scroller_policy_set(SCROLLER, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_AUTO); \
   evas_object_show(SCROLLER);

#define LABEL_ADD(PARENT, LABEL, TEXT) \
   LABEL = elm_label_add(PARENT); \
   elm_object_text_set(LABEL, TEXT); \
   evas_object_show(LABEL);

#define FRAME_ADD(PARENT, FRAME, AUTOCOLLAPSE, TITLE) \
   FRAME = elm_frame_add(PARENT); \
   elm_frame_autocollapse_set(FRAME, AUTOCOLLAPSE); \
   evas_object_size_hint_fill_set(FRAME, EVAS_HINT_FILL, 0.0); \
   elm_object_text_set(FRAME, TITLE); \
   evas_object_show(FRAME);

#define SPINNER_ADD(PARENT, SPINNER, MIN, MAX, STEP, ISEDIT) \
   SPINNER = elm_spinner_add(PARENT); \
   evas_object_size_hint_weight_set(SPINNER, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_size_hint_align_set(SPINNER, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   elm_spinner_min_max_set(SPINNER, MIN, MAX); \
   elm_spinner_step_set(SPINNER, STEP); \
   elm_spinner_editable_set(SPINNER, ISEDIT); \
   evas_object_show(SPINNER);

#define ENTRY_ADD(PARENT, ENTRY, ISSINGLELINE) \
   ENTRY = elm_entry_add(PARENT); \
   evas_object_size_hint_weight_set(ENTRY, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_size_hint_align_set(ENTRY, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   elm_entry_single_line_set(ENTRY, ISSINGLELINE); \
   elm_entry_scrollable_set(ENTRY, EINA_TRUE); \
   evas_object_show(ENTRY);

#ifndef HAVE_TIZEN
/* ap.win for now, because only this parent allow combobox to show it's genlist */
#define COMBOBOX_ADD(PARENT, COMBOBOX) \
   COMBOBOX = elm_combobox_add(ap.win); \
   evas_object_size_hint_weight_set(COMBOBOX, EVAS_HINT_EXPAND, 0); \
   evas_object_size_hint_align_set(COMBOBOX, EVAS_HINT_FILL, 0); \
   evas_object_show(COMBOBOX);
#else
#define COMBOBOX_ADD(PARENT, COMBOBOX) \
   COMBOBOX = elm_combobox_add(ap.win); \
   evas_object_size_hint_weight_set(COMBOBOX, EVAS_HINT_EXPAND, 0); \
   evas_object_size_hint_align_set(COMBOBOX, EVAS_HINT_FILL, 0); \
   evas_object_show(COMBOBOX); \
   evas_object_smart_callback_add(COMBOBOX, "expanded",  _combobox_widget_expanded_cb, NULL); \
   evas_object_smart_callback_add(COMBOBOX, "dismissed", _combobox_widget_dismissed_cb, NULL);
#endif

#define SEGMENT_CONTROL_ADD(PARENT, SC) \
   SC = elm_segment_control_add(PARENT); \
   evas_object_size_hint_weight_set(SC, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_size_hint_align_set(SC, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   evas_object_show(SC);

#define CHECK_ADD(PARENT, CHECK) \
   CHECK = elm_check_add(PARENT); \
   evas_object_size_hint_align_set(CHECK, 0.0, EVAS_HINT_FILL); \
   evas_object_size_hint_weight_set(CHECK, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_show(CHECK);

#define BUTTON_ADD(PARENT, BUTTON, TEXT) \
   BUTTON = elm_button_add(PARENT); \
   evas_object_size_hint_align_set(BUTTON, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   evas_object_size_hint_weight_set(BUTTON, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   elm_object_text_set(BUTTON, TEXT); \
   evas_object_show(BUTTON);

#define ICON_STANDARD_ADD(PARENT, ICON, NOSCALE, ICON_NAME) \
   ICON = elm_icon_add (PARENT);\
   elm_icon_standard_set(ICON, ICON_NAME);\
   elm_image_no_scale_set(ICON, NOSCALE);\
   evas_object_size_hint_align_set(ICON, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   evas_object_size_hint_weight_set(ICON, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_show(ICON);

#define IMAGE_ADD_NEW(PARENT, IMAGE, NAME, STYLE) \
   IMAGE = elm_image_add(PARENT); \
   elm_image_file_set(IMAGE, ap.path.theme_edj, "elm/image/"NAME"/"STYLE);

#define HOVERSEL_ADD(PARENT, HOVERSEL, ISHORIZONTAL) \
   HOVERSEL = elm_hoversel_add(PARENT); \
   evas_object_size_hint_align_set(HOVERSEL, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   evas_object_size_hint_weight_set(HOVERSEL, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   elm_hoversel_horizontal_set(HOVERSEL, ISHORIZONTAL); \
   evas_object_show(HOVERSEL);

#define FILESELECTOR_ADD(FILESELECTOR, PARENT, CALLBACK, DATA) \
   FILESELECTOR = elm_fileselector_add(PARENT); \
   evas_object_size_hint_weight_set(FILESELECTOR, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   elm_fileselector_path_set(FILESELECTOR, profile_get()->general.projects_folder); \
   elm_fileselector_buttons_ok_cancel_set(FILESELECTOR, true); \
   elm_fileselector_expandable_set(FILESELECTOR, false); \
   elm_fileselector_mode_set(FILESELECTOR, ELM_FILESELECTOR_LIST); \
   evas_object_smart_callback_add(FILESELECTOR, "done", CALLBACK, DATA); \
   evas_object_smart_callback_add(FILESELECTOR, "activated", CALLBACK, DATA); \
   evas_object_show(FILESELECTOR);

#define RADIO_ADD(PARENT, RADIO, STATE, TEXT) \
   RADIO = elm_radio_add(PARENT); \
   evas_object_size_hint_weight_set(RADIO, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   elm_radio_state_value_set(RADIO, STATE); \
   elm_object_text_set(RADIO, TEXT); \
   evas_object_show(RADIO);

#endif /* WIDGET_MACRO_H */
