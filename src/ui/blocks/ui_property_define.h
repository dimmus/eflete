#define OBJ_DATA "group_data"

#define ITEM_ADD(parent, item, text) \
   item = elm_layout_add(parent); \
   evas_object_size_hint_weight_set(item, EVAS_HINT_EXPAND, 0.0); \
   evas_object_size_hint_align_set(item, EVAS_HINT_FILL, 0.0); \
   elm_layout_file_set(item, TET_EDJ, "property/item/default"); \
   elm_object_text_set(item, text); \
   evas_object_show(item);

#define ITEM_INT_GROUP_CALLBACK(Value) \
static void \
__on_##Value##_change(void *data, \
                      Evas_Object *obj, \
                      void *ei __UNUSED__) \
{ \
   Group *group = (Group *)data; \
   int value = (int)elm_spinner_value_get(obj); \
   edje_edit_##Value##_set(group->obj, value); \
   group->isModify = EINA_TRUE; \
}

#define ITEM_2SPINNER_GROUP(text, value1, value2) \
Evas_Object * \
prop_item_##value1##_##value2##_add(Evas_Object *parent, \
                                    Group *group, \
                                    double min, \
                                    double max, \
                                    double step, \
                                    const char *tooltip1, \
                                    const char *tooltip2) \
{ \
   Evas_Object *item, *box, *spinner1, *spinner2; \
   ITEM_ADD(parent, item, text) \
   BOX_ADD(parent, box, EINA_TRUE, EINA_TRUE); \
   SPINNER_ADD(box, spinner1, min, max, step, EINA_TRUE, "default") \
   elm_spinner_value_set(spinner1, edje_edit_##value1##_get(group->obj)); \
   elm_object_tooltip_text_set(spinner1, tooltip1); \
   elm_box_pack_end(box, spinner1); \
   evas_object_smart_callback_add(spinner1, "changed", \
                                  __on_##value1##_change, group); \
   SPINNER_ADD(box, spinner2, min, max, step, EINA_TRUE, "default") \
   elm_spinner_value_set(spinner2, edje_edit_##value2##_get(group->obj)); \
   elm_object_tooltip_text_set(spinner2, tooltip2); \
   elm_box_pack_end(box, spinner2); \
   evas_object_smart_callback_add(spinner2, "changed", \
                                  __on_##value2##_change, group); \
   elm_object_part_content_set(item, "elm.swallow.content", box); \
   return item; \
}

#define ITEM_2INT_GROUP_CREATE(text, value1, value2) \
   ITEM_INT_GROUP_CALLBACK(value1) \
   ITEM_INT_GROUP_CALLBACK(value2) \
   ITEM_2SPINNER_GROUP(text, value1, value2)

#define ITEM_STRING_PART_CALLBACK(Value) \
static void \
__on_state_##Value##_change(void *data, \
                            Evas_Object *obj, \
                            void *ei __UNUSED__) \
{ \
   Part *part = (Part *)data; \
   Group *group = evas_object_data_get(obj, OBJ_DATA); \
   const char *value = elm_entry_entry_get(obj); \
   edje_edit_part_##Value##_set(group->obj, part->name, value); \
   group->isModify = EINA_TRUE; \
}

#define ITEM_BOOL_PART_CALLBACK(Value) \
static void \
__on_##Value##_change(void *data, \
                                Evas_Object *obj, \
                                void *ei __UNUSED__) \
{ \
   Part *part = (Part *)data; \
   Group *group = evas_object_data_get(obj, OBJ_DATA); \
   Eina_Bool value = elm_check_state_get(obj); \
   edje_edit_##Value##_set(group->obj, part->name, value); \
   group->isModify = EINA_TRUE; \
}

#define ITEM_1ENTRY_PART(text, value) \
Evas_Object * \
prop_item_part_##value##_add(Evas_Object *parent, \
                              Group *group, \
                              Part *part, \
                              const char *tooltip) \
{ \
   Evas_Object *item, *entry; \
   ITEM_ADD(parent, item, text) \
   ENTRY_ADD(parent, entry, EINA_TRUE) \
   elm_entry_entry_set(entry, edje_edit_part_##value##_get(group->obj, part->name)); \
   evas_object_data_set(entry, OBJ_DATA, group); \
   elm_object_tooltip_text_set(entry, tooltip); \
   evas_object_smart_callback_add(entry, "activated", \
                                  __on_state_##value##_change, part); \
   elm_object_part_content_set(item, "elm.swallow.content", entry); \
   return item; \
}

#define ITEM_1CHEACK_PART(text, value) \
Evas_Object * \
prop_item_##value##_add(Evas_Object *parent, \
                        Group *group, \
                        Part *part, \
                        const char *tooltip) \
{ \
   Evas_Object *item, *check; \
   ITEM_ADD(parent, item, text) \
   CHECK_ADD(parent, check, "simplified_toggle") \
   elm_check_state_set(check, edje_edit_##value##_get(group->obj, \
                                                      part->name)); \
   evas_object_data_set(check, OBJ_DATA, group); \
   elm_object_tooltip_text_set(check, tooltip); \
   evas_object_smart_callback_add(check, "changed", \
                                  __on_##value##_change, part); \
   elm_object_part_content_set(item, "elm.swallow.content", check); \
   return item; \
}

#define ITEM_1BOOL_PART_CREATE(text, value) \
   ITEM_BOOL_PART_CALLBACK(value) \
   ITEM_1CHEACK_PART(text, value)

#define ITEM_1SEL_PART_CREATE(text, value) \
   ITEM_SEL_PART_CALLBACK(value) \
   ITEM_1SEL_PART(text, value)

#define ITEM_1STRING_PART_CREATE(text, value) \
   ITEM_STRING_PART_CALLBACK(value) \
   ITEM_1ENTRY_PART(text, value)

#define ITEM_STRING_STATE_CALLBACK(Value) \
static void \
__on_state_##Value##_change(void *data, \
                            Evas_Object *obj, \
                            void *ei __UNUSED__) \
{ \
   Part *part = (Part *)data; \
   Group *group = evas_object_data_get(obj, OBJ_DATA); \
   const char *value = elm_entry_entry_get(obj); \
   edje_edit_state_##Value##_set(group->obj, part->name, \
                                part->curr_state, part->curr_state_value, \
                                value); \
   group->isModify = EINA_TRUE; \
}

#define ITEM_DOUBLE_STATE_CALLBACK(Value) \
static void \
__on_state_##Value##_change(void *data, \
                            Evas_Object *obj, \
                            void *ei __UNUSED__) \
{ \
   Part *part = (Part *)data; \
   Group *group = evas_object_data_get(obj, OBJ_DATA); \
   double value = elm_spinner_value_get(obj); \
   edje_edit_state_##Value##_set(group->obj, part->name, \
                                 part->curr_state, part->curr_state_value, \
                                 value); \
   group->isModify = EINA_TRUE; \
}

#define ITEM_INT_STATE_CALLBACK(Value) \
static void \
__on_state_##Value##_change(void *data, \
                            Evas_Object *obj, \
                            void *ei __UNUSED__) \
{ \
   Part *part = (Part *)data; \
   Group *group = evas_object_data_get(obj, OBJ_DATA); \
   int value = (int)elm_spinner_value_get(obj); \
   edje_edit_state_##Value##_set(group->obj, part->name, \
                                 part->curr_state, part->curr_state_value, \
                                 value); \
   group->isModify = EINA_TRUE; \
}

#define ITEM_BOOL_STATE_CALLBACK(Value) \
static void \
__on_state_##Value##_change(void *data, \
                            Evas_Object *obj, \
                            void *ei __UNUSED__) \
{ \
   Part *part = (Part *)data; \
   Group *group = evas_object_data_get(obj, OBJ_DATA); \
   Eina_Bool value = elm_check_state_get(obj); \
   edje_edit_state_##Value##_set(group->obj, part->name, \
                                 part->curr_state, part->curr_state_value, \
                                 value); \
   group->isModify = EINA_TRUE; \
}

#define ITEM_COLOR_STATE_CALLBACK(Value) \
static void \
__on_state_##Value##_change(void *data, \
                            Evas_Object *obj, \
                            void *ei __UNUSED__) \
{ \
   int r, g, b, a; \
   Part *part = (Part *)data; \
   Group *group = evas_object_data_get(obj, OBJ_DATA); \
   const char *value = elm_entry_entry_get(obj); \
   char **c = eina_str_split(value, " ", 4); \
   r = atoi(c[0]); g = atoi(c[1]); b = atoi(c[2]); a = atoi(c[3]); \
   edje_edit_state_##Value##_set(group->obj, part->name, \
                                part->curr_state, part->curr_state_value, \
                                r, g, b, a); \
   evas_object_color_set(part->obj, r*a/255, g*a/255, b*a/255, a); \
   free(c[0]); \
   free(c); \
   group->isModify = EINA_TRUE; \
}

#define ITEM_IM_BORDER_STATE_CALLBACK(Value) \
static void \
__on_state_##Value##_change(void *data, \
                            Evas_Object *obj, \
                            void *ei __UNUSED__) \
{ \
   Part *part = (Part *)data; \
   Group *group = evas_object_data_get(obj, OBJ_DATA); \
   const char *value = elm_entry_entry_get(obj); \
   char **c = eina_str_split(value, " ", 4); \
   edje_edit_state_##Value##_set(group->obj, part->name, \
                                part->curr_state, part->curr_state_value, \
                                atoi(c[0]), atoi(c[1]), atoi(c[2]), atoi(c[3])); \
   free(c[0]); \
   free(c); \
   group->isModify = EINA_TRUE; \
}

#define ITEM_1CHEACK_STATE(text, value) \
Evas_Object * \
prop_item_state_##value##_add(Evas_Object *parent, \
                              Group *group, \
                              Part *part, \
                              const char *tooltip)\
{ \
   Evas_Object *item, *check; \
   ITEM_ADD(parent, item, text) \
   CHECK_ADD(parent, check, "simplified_toggle") \
   elm_check_state_set(check, edje_edit_state_##value##_get(group->obj, \
                                                      part->name, \
                                                      part->curr_state, \
                                                      part->curr_state_value)); \
   evas_object_data_set(check, OBJ_DATA, group); \
   elm_object_tooltip_text_set(check, tooltip); \
   evas_object_smart_callback_add(check, "changed", \
                                  __on_state_##value##_change, part); \
   elm_object_part_content_set(item, "elm.swallow.content", check); \
   return item; \
}

#define ITEM_2CHEACK_STATE(text, value1, value2) \
Evas_Object * \
prop_item_state_##value1##_##value2##_add(Evas_Object *parent, \
                                          Group *group, \
                                          Part *part, \
                                          const char *tooltip1, \
                                          const char *tooltip2) \
{ \
   Evas_Object *item, *box, *check1, *check2; \
   ITEM_ADD(parent, item, text) \
   BOX_ADD(item, box, EINA_TRUE, EINA_TRUE) \
   CHECK_ADD(box, check1, "simplified_toggle") \
   elm_check_state_set(check1, edje_edit_state_##value1##_get(group->obj, \
                                                      part->name, \
                                                      part->curr_state, \
                                                      part->curr_state_value)); \
   evas_object_data_set(check1, OBJ_DATA, group); \
   elm_object_tooltip_text_set(check1, tooltip1); \
   elm_box_pack_end(box, check1); \
   evas_object_smart_callback_add(check1, "changed", \
                                  __on_state_##value1##_change, part); \
   CHECK_ADD(box, check2, "simplified_toggle") \
   elm_check_state_set(check2, edje_edit_state_##value2##_get(group->obj, \
                                                      part->name, \
                                                      part->curr_state, \
                                                      part->curr_state_value)); \
   evas_object_data_set(check2, OBJ_DATA, group); \
   elm_object_tooltip_text_set(check2, tooltip2); \
   elm_box_pack_end(box, check2); \
   evas_object_smart_callback_add(check2, "changed", \
                                  __on_state_##value2##_change, part); \
   elm_object_part_content_set(item, "elm.swallow.content", box); \
   return item; \
}


#define ITEM_1SPINNER_STATE(text, value) \
Evas_Object * \
prop_item_state_##value##_add(Evas_Object *parent, \
                              Group *group, \
                              Part *part, \
                              double min, \
                              double max, \
                              double step, \
                              const char *fmt, \
                              const char *tooltip) \
{ \
   Evas_Object *item, *spinner; \
   ITEM_ADD(parent, item, text) \
   SPINNER_ADD(item, spinner, min, max, step, EINA_TRUE, "default") \
   elm_spinner_label_format_set(spinner, fmt); \
   elm_spinner_value_set(spinner, edje_edit_state_##value##_get(group->obj, \
                                                            part->name, \
                                                            part->curr_state, \
                                                            part->curr_state_value)); \
   evas_object_data_set(spinner, OBJ_DATA, group); \
   elm_object_tooltip_text_set(spinner, tooltip); \
   evas_object_smart_callback_add(spinner, "changed", \
                                  __on_state_##value##_change, part); \
   elm_object_part_content_set(item, "elm.swallow.content", spinner); \
   return item;\
}

#define ITEM_2SPINNER_STATE(text, value1, value2) \
Evas_Object * \
prop_item_state_##value1##_##value2##_add(Evas_Object *parent, \
                                          Group *group, \
                                          Part *part, \
                                          double min, \
                                          double max, \
                                          double step, \
                                          const char *fmt, \
                                          Eina_Bool feedback, \
                                          const char *tooltip1, \
                                          const char *tooltip2) \
{ \
   Evas_Object *item, *box, *spinner1, *spinner2; \
   ITEM_ADD(parent, item, text) \
   BOX_ADD(item, box, EINA_TRUE, EINA_TRUE) \
   SPINNER_ADD(box, spinner1, min, max, step, EINA_TRUE, "default") \
   elm_spinner_label_format_set(spinner1, fmt); \
   elm_spinner_value_set(spinner1, edje_edit_state_##value1##_get(group->obj, \
                                                            part->name, \
                                                            part->curr_state, \
                                                            part->curr_state_value)); \
   elm_box_pack_end(box, spinner1); \
   evas_object_data_set(spinner1, OBJ_DATA, group); \
   elm_object_tooltip_text_set(spinner1, tooltip1); \
   evas_object_smart_callback_add(spinner1, "changed", \
                                  __on_state_##value1##_change, part); \
   if (feedback) \
     { \
       evas_object_event_callback_del(part->obj, EVAS_CALLBACK_RESIZE, \
                                      _obj_x_change_feedback); \
       evas_object_event_callback_add(part->obj, EVAS_CALLBACK_RESIZE, \
                                      _obj_x_change_feedback, spinner1); \
     } \
   SPINNER_ADD(box, spinner2, min, max, step, EINA_TRUE, "default") \
   elm_spinner_label_format_set(spinner2, fmt); \
   elm_spinner_value_set(spinner2, edje_edit_state_##value2##_get(group->obj, \
                                                            part->name, \
                                                            part->curr_state, \
                                                            part->curr_state_value)); \
   elm_box_pack_end(box, spinner2); \
   evas_object_data_set(spinner2, OBJ_DATA, group); \
   elm_object_tooltip_text_set(spinner2, tooltip2); \
   evas_object_smart_callback_add(spinner2, "changed", \
                                  __on_state_##value2##_change, part); \
   if (feedback) \
     { \
        evas_object_event_callback_del(part->obj, EVAS_CALLBACK_RESIZE, \
                                       _obj_y_change_feedback); \
        evas_object_event_callback_add(part->obj, EVAS_CALLBACK_RESIZE, \
                                       _obj_y_change_feedback, spinner2); \
     } \
   elm_object_part_content_set(item, "elm.swallow.content", box); \
   return item;\
}

#define ITEM_1ENTRY_STATE(text, value) \
Evas_Object * \
prop_item_state_##value##_add(Evas_Object *parent, \
                              Group *group, \
                              Part *part, \
                              const char *tooltip) \
{ \
   Evas_Object *item, *entry; \
   ITEM_ADD(parent, item, text) \
   ENTRY_ADD(parent, entry, EINA_TRUE) \
   elm_entry_entry_set(entry, edje_edit_state_##value##_get(group->obj, \
                                                            part->name, \
                                                            part->curr_state, \
                                                            part->curr_state_value)); \
   evas_object_data_set(entry, OBJ_DATA, group); \
   elm_object_tooltip_text_set(entry, tooltip); \
   evas_object_smart_callback_add(entry, "activated", \
                                  __on_state_##value##_change, part); \
   elm_object_part_content_set(item, "elm.swallow.content", entry); \
   return item; \
}

#define ITEM_2ENTRY_STATE(text, value1, value2) \
Evas_Object * \
prop_item_state_##value1##_##value2##_add(Evas_Object *parent, \
                                          Group *group, \
                                          Part *part, \
                                          const char *tooltip1, \
                                          const char *tooltip2) \
{ \
   Evas_Object *item, *box, *entry1, *entry2; \
   ITEM_ADD(parent, item, text) \
   BOX_ADD(item, box, EINA_TRUE, EINA_TRUE) \
   ENTRY_ADD(box, entry1, EINA_TRUE) \
   ENTRY_ADD(box, entry2, EINA_TRUE) \
   elm_entry_entry_set(entry1, edje_edit_state_##value1##_get(group->obj, \
                                                              part->name, \
                                                              part->curr_state, \
                                                              part->curr_state_value)); \
   evas_object_data_set(entry1, OBJ_DATA, group); \
   elm_object_tooltip_text_set(entry1, tooltip1); \
   evas_object_smart_callback_add(entry1, "activated", \
                                  __on_state_##value1##_change, part); \
   elm_box_pack_end(box, entry1); \
   elm_entry_entry_set(entry2, edje_edit_state_##value2##_get(group->obj, \
                                                              part->name, \
                                                              part->curr_state, \
                                                              part->curr_state_value)); \
   evas_object_data_set(entry2, OBJ_DATA, group); \
   elm_object_tooltip_text_set(entry2, tooltip2); \
   evas_object_smart_callback_add(entry2, "activated", \
                                  __on_state_##value2##_change, part); \
   elm_box_pack_end(box, entry2); \
   elm_object_part_content_set(item, "elm.swallow.content", box); \
   return item; \
}

#define ITEM_COLOR_STATE(text, value) \
Evas_Object * \
prop_item_state_##value##_add(Evas_Object *parent, \
                              Group *group, \
                              Part *part, \
                              const char *tooltip) \
{ \
   Evas_Object *item, *box, *entry, *image; \
   char buff[BUFF_MAX]; \
   int r, g, b, a; \
   edje_edit_state_##value##_get(group->obj, part->name, \
                                 part->curr_state, part->curr_state_value, \
                                 &r, &g, &b, &a); \
   ITEM_ADD(parent, item, text) \
   BOX_ADD(parent, box, EINA_TRUE, EINA_TRUE) \
   ENTRY_ADD(box, entry, EINA_TRUE) \
   IMAGE_ADD(box, image, TET_IMG_PATH"bg.png"); \
   elm_entry_markup_filter_append(entry, elm_entry_filter_accept_set, &accept_color); \
   elm_entry_markup_filter_append(entry, elm_entry_filter_limit_size, &limit_color); \
   snprintf(buff, sizeof(buff), "%i %i %i %i", r, g, b, a); \
   elm_entry_entry_set(entry, buff); \
   evas_object_data_set(entry, OBJ_DATA, group); \
   elm_object_tooltip_text_set(entry, tooltip); \
   evas_object_smart_callback_add(entry, "activated", \
                                  __on_state_##value##_change, part); \
   evas_object_color_set(image, r*a/255, g*a/255, b*a/255, a); \
   elm_box_pack_end(box, entry); \
   elm_box_pack_end(box, image); \
   elm_object_part_content_set(item, "elm.swallow.content", box); \
   return item; \
}

#define ITEM_IM_BORDER_STATE(text, value) \
Evas_Object * \
prop_item_state_##value##_add(Evas_Object *parent, \
                              Group *group, \
                              Part *part, \
                              const char *tooltip) \
{ \
   Evas_Object *item, *entry; \
   char buff[BUFF_MAX]; \
   int l, r, t, b; \
   edje_edit_state_##value##_get(group->obj, part->name, \
                                 part->curr_state, part->curr_state_value, \
                                 &l, &r, &t, &b); \
   ITEM_ADD(parent, item, text) \
   ENTRY_ADD(item, entry, EINA_TRUE) \
   elm_entry_markup_filter_append(entry, elm_entry_filter_accept_set, &accept_color); \
   snprintf(buff, sizeof(buff), "%i %i %i %i", l, r, t, b); \
   elm_entry_entry_set(entry, buff); \
   evas_object_data_set(entry, OBJ_DATA, group); \
   elm_object_tooltip_text_set(entry, tooltip); \
   evas_object_smart_callback_add(entry, "activated", \
                                  __on_state_##value##_change, part); \
   elm_object_part_content_set(item, "elm.swallow.content", entry); \
   return item; \
}

#define ITEM_1INT_STATE_CREATE(text, value) \
   ITEM_INT_STATE_CALLBACK(value) \
   ITEM_1SPINNER_STATE(text, value)

#define ITEM_1DOUBLE_STATE_CREATE(text, value) \
   ITEM_DOUBLE_STATE_CALLBACK(value) \
   ITEM_1SPINNER_STATE(text, value)

#define ITEM_1BOOL_STATE_CREATE(text, value) \
   ITEM_BOOL_STATE_CALLBACK(value) \
   ITEM_1CHEACK_STATE(text, value)

#define ITEM_2INT_STATE_CREATE(text, value1, value2) \
   ITEM_INT_STATE_CALLBACK(value1) \
   ITEM_INT_STATE_CALLBACK(value2) \
   ITEM_2SPINNER_STATE(text, value1, value2)

#define ITEM_2DOUBLE_STATE_CREATE(text, value1, value2) \
   ITEM_DOUBLE_STATE_CALLBACK(value1) \
   ITEM_DOUBLE_STATE_CALLBACK(value2) \
   ITEM_2SPINNER_STATE(text, value1, value2)

#define ITEM_2BOOL_STATE_CREATE(text, value1, value2) \
   ITEM_BOOL_STATE_CALLBACK(value1) \
   ITEM_BOOL_STATE_CALLBACK(value2) \
   ITEM_2CHEACK_STATE(text, value1, value2)

#define ITEM_COLOR_STATE_CREATE(text, value) \
   ITEM_COLOR_STATE_CALLBACK(value) \
   ITEM_COLOR_STATE(text, value)

#define ITEM_1STRING_STATE_CREATE(text, value) \
   ITEM_STRING_STATE_CALLBACK(value) \
   ITEM_1ENTRY_STATE(text, value)

#define ITEM_2STRING_STATE_CREATE(text, value1, value2) \
   ITEM_STRING_STATE_CALLBACK(value1) \
   ITEM_STRING_STATE_CALLBACK(value2) \
   ITEM_2ENTRY_STATE(text, value1, value2)

#define ITEM_IM_BORDER_STATE_CREATE(text, value) \
   ITEM_IM_BORDER_STATE_CALLBACK(value) \
   ITEM_IM_BORDER_STATE(text, value)


