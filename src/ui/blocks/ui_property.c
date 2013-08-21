#include "ui_property.h"
#include "widget_manager.h"

struct _Prop_View_Data
{
   Evas_Object *group_prop;
   Evas_Object *part_prop;
};

typedef struct _Prop_View_Data Prop_View_Data;
#define PROP_VIEW_DATA "prop_view_data"

struct _Prop_Part_View_Data
{
   Evas_Object *box;
   Evas_Object *base;
   struct {
        Evas_Object *state;
        Evas_Object *text;
        Evas_Object *image;
   } state;
   Evas_Object *dragable;
};

typedef struct _Prop_Part_View_Data Prop_Part_View_Data;
#define PROP_PART_VIEW_DATA "prop_part_view_data"

void
_on_group_view_del(void *data __UNUSED__,
                   Evas *e __UNUSED__,
                   Evas_Object *obj,
                   void *event_info __UNUSED__)
{
   free(evas_object_data_get(obj, PROP_VIEW_DATA));
}

void
_on_part_view_del(void *data __UNUSED__,
                  Evas *e __UNUSED__,
                  Evas_Object *obj,
                  void *event_info __UNUSED__)
{
   free(evas_object_data_get(obj, PROP_PART_VIEW_DATA));
}

Evas_Object *
ui_property_view_new(Evas_Object *parent)
{
   Evas_Object *box, *scroller;
   Prop_View_Data *pvd;

   if (!parent)
     return NULL;

   pvd = mem_malloc(sizeof(Prop_View_Data));
   pvd->group_prop = NULL;
   pvd->part_prop = NULL;

   scroller = elm_scroller_add(parent);
   evas_object_size_hint_weight_set(scroller, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(scroller, EVAS_HINT_FILL, EVAS_HINT_FILL);

   box = elm_box_add(scroller);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(box, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_align_set(box, 0.5, 0.0);
   evas_object_data_set(scroller, PROP_VIEW_DATA, pvd);

   evas_object_event_callback_add(scroller, EVAS_CALLBACK_DEL,
                                  _on_group_view_del, NULL);

   elm_object_content_set(scroller, box);

   return scroller;
}

void
ui_property_group_view_set(Evas_Object *prop_view, Evas_Object *group_view)
{
   Prop_View_Data *pvd;

   if ((!prop_view) || (!group_view))
     return;

   pvd = evas_object_data_get(prop_view, PROP_VIEW_DATA);
   if (pvd->group_prop)
     evas_object_del(pvd->group_prop);
   pvd->group_prop = group_view;
   elm_box_pack_start(elm_object_content_get(prop_view), group_view);
}

void
ui_property_part_view_set(Evas_Object *prop_view, Evas_Object *part_view)
{
   Prop_View_Data *pvd;

   if ((!prop_view) || (!part_view))
     return;

   pvd = evas_object_data_get(prop_view, PROP_VIEW_DATA);
   if (pvd->part_prop)
     evas_object_del(pvd->part_prop);
   pvd->part_prop = part_view;
   elm_box_pack_end(elm_object_content_get(prop_view), part_view);
}

Evas_Object *
ui_property_group_view_get(Evas_Object *prop_view)
{
   Prop_View_Data *pvd;

   if (!prop_view)
     return NULL;

   pvd = evas_object_data_get(prop_view, PROP_VIEW_DATA);
   return pvd->group_prop;
}

Evas_Object *
ui_property_part_view_get(Evas_Object *prop_view)
{
   Prop_View_Data *pvd;

   if (!prop_view)
     return NULL;

   pvd = evas_object_data_get(prop_view, PROP_VIEW_DATA);
   return pvd->part_prop;
}

void
ui_prop_group_info_view_update(Evas_Object *prop_view, Group *group)
{
   Evas_Object *group_view;

   if ((!prop_view) || (!group))
     return;

   group_view = ui_prop_group_info_view_add(prop_view, group);
   evas_object_show(group_view);
   ui_property_group_view_set(prop_view, group_view);
}

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

ITEM_2INT_GROUP_CREATE("min:", group_min_w, group_min_h)
ITEM_2INT_GROUP_CREATE("max:", group_max_w, group_max_h)

Evas_Object *
ui_prop_group_info_view_add(Evas_Object *prop_view, Group *group __UNUSED__)
{
   Evas_Object *group_view, *box, *item;

   if (!prop_view)
     return NULL;

   group_view = elm_frame_add(prop_view);
   elm_frame_autocollapse_set(group_view, EINA_TRUE);
   evas_object_size_hint_fill_set(group_view, EVAS_HINT_FILL, 0.0);
   elm_object_text_set(group_view, "Group property");

   box = elm_box_add(group_view);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(box, EVAS_HINT_FILL, 0.0);
   elm_box_align_set(box, 0.5, 0.0);

   item = prop_item_group_min_w_group_min_h_add(box, group,
                                                0.0, 9999.0, 1.0,
                                                "Minimum group width in pixels.",
                                                "Minimum group height in pixels.");
   elm_box_pack_end(box, item);
   item = prop_item_group_max_w_group_max_h_add(box, group,
                                                -1.0, 9999.0, 1.0,
                                                "Maximum group width in pixels.",
                                                "Maximum group height in pixels.");
   elm_box_pack_end(box, item);

/*   item = ui_prop_view_item_two_edit_int_add(box, "current",
                                             &group->current_w, "Current group width in pixels.",
                                             &group->current_h, "Current group height in pixels.");
   evas_object_show(item);*/

   evas_object_show(box);
   elm_object_content_set(group_view, box);

   return group_view;
}


Evas_Object *
prop_item_label_add(Evas_Object *parent,
                    const char *lab_text,
                    const char *text)
{
   Evas_Object *item, *label;
   ITEM_ADD(parent, item, lab_text)
   LABEL_ADD(parent, label, text)
   elm_object_part_content_set(item, "elm.swallow.content", label);
   return item;
}

#define OBJ_DATA "group_data"

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

#define ITEM_1CHEACK_PART(text, value) \
Evas_Object * \
prop_item_##value##_add(Evas_Object *parent, \
                        Group *group, \
                        Part *part, \
                        const char *tooltip) \
{ \
   Evas_Object *item, *check; \
   ITEM_ADD(parent, item, text) \
   CHECK_ADD(parent, check, "toggle") \
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

ITEM_1BOOL_PART_CREATE("scale:", part_scale)
ITEM_1BOOL_PART_CREATE("mouse:", part_mouse_events)
ITEM_1BOOL_PART_CREATE("repeat:", part_repeat_events)

Evas_Object *
ui_prop_part_info_view_add(Evas_Object *prop_view,
                           Group *group __UNUSED__,
                           Part *part __UNUSED__)
{
   Evas_Object *part_view, *box, *item;
   Evas_Object *part_view_base, *box_base;
   Prop_Part_View_Data *ppvd;

   if (!prop_view)
     return NULL;

   part_view = elm_frame_add(prop_view);
   elm_frame_autocollapse_set(part_view, EINA_TRUE);
   evas_object_size_hint_fill_set(part_view, EVAS_HINT_FILL, 0.0);
   elm_object_text_set(part_view, "Part property");

   box = elm_box_add(part_view);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(box, EVAS_HINT_FILL, 0.0);
   elm_box_align_set(box, 0.5, 0.0);
   evas_object_show(box);

   ppvd = mem_malloc(sizeof(Prop_Part_View_Data));

   part_view_base = elm_frame_add(box);
   elm_frame_autocollapse_set(part_view_base, EINA_TRUE);
   evas_object_size_hint_fill_set(part_view_base, EVAS_HINT_FILL, 0.0);
   elm_object_style_set(part_view_base, "pad_small");
   elm_box_pack_end(box, part_view_base);
   evas_object_show(part_view_base);

   box_base = elm_box_add(part_view_base);
   evas_object_size_hint_weight_set(box_base, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(box_base, EVAS_HINT_FILL, 0.0);
   elm_box_align_set(box_base, 0.5, 0.0);
   elm_object_content_set(part_view_base, box_base);
   evas_object_show(box_base);

   ppvd->box = box;
   ppvd->base = part_view_base;
   ppvd->state.state = NULL;
   ppvd->state.text = NULL;
   ppvd->state.image = NULL;
   ppvd->dragable = NULL;

   evas_object_data_set(part_view, PROP_PART_VIEW_DATA, ppvd);
   evas_object_event_callback_add(box, EVAS_CALLBACK_DEL,
                                  _on_part_view_del, NULL);

   elm_object_content_set(part_view, box);

   item = prop_item_label_add(box_base, "name:", part->name);
   elm_box_pack_end(box_base, item);
   Edje_Part_Type type = edje_edit_part_type_get(group->obj, part->name);
   item = prop_item_label_add(box_base, "type:", wm_part_type_get(type));
   elm_box_pack_end(box_base, item);
   item = prop_item_part_scale_add(box_base, group, part,
                                   "Specifies whether the part will scale "
                                   "its size with an edje scaling factor.");
   elm_box_pack_end(box_base, item);
   item = prop_item_part_mouse_events_add(box_base, group, part,
                                          "Enable mouse events in this part.");
   elm_box_pack_end(box_base, item);
   item = prop_item_part_repeat_events_add(box_base, group, part,
                                           "Enable repeat mouse events to the parts below.");
   elm_box_pack_end(box_base, item);

/*
   #define EDJE_EDIT_FUNC_PARAM group->obj, part->name
   item = ui_prop_view_item_one_edit_string_add(box_base, "name",
                                                part->name, "Unique name of part.");
   evas_object_show(item);

   item = ui_prop_view_item_one_edit_string_add(box_base, "clip to",
                                                edje_edit_part_clip_to_get(EDJE_EDIT_FUNC_PARAM),
                                                "Reference clipper part.");
   evas_object_show(item);

   item = ui_prop_view_item_one_edit_string_add(box_base, "source",
                                                edje_edit_part_source_get(EDJE_EDIT_FUNC_PARAM),
                                                "Source group selection.");
   evas_object_show(item);

   evas_object_data_set(part_view, PROP_PART_VIEW_DATA, ppvd);

   #undef EDJE_EDIT_FUNC_PARAM
   */
   return part_view;
}

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
   CHECK_ADD(parent, check, "toggle") \
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
   CHECK_ADD(box, check1, "toggle") \
   elm_check_state_set(check1, edje_edit_state_##value1##_get(group->obj, \
                                                      part->name, \
                                                      part->curr_state, \
                                                      part->curr_state_value)); \
   evas_object_data_set(check1, OBJ_DATA, group); \
   elm_object_tooltip_text_set(check1, tooltip1); \
   elm_box_pack_end(box, check1); \
   evas_object_smart_callback_add(check1, "changed", \
                                  __on_state_##value1##_change, part); \
   CHECK_ADD(box, check2, "toggle") \
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
                                          const char *tooltip1, \
                                          const char *tooltip2) \
{ \
   Evas_Object *item, *box, *spinner1, *spinner2; \
   ITEM_ADD(parent, item, text) \
   BOX_ADD(parent, box, EINA_TRUE, EINA_TRUE) \
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

static Elm_Entry_Filter_Accept_Set accept_color = {
     .accepted = "0123456789 ",
     .rejected = NULL
};
static Elm_Entry_Filter_Limit_Size limit_color = {
     .max_char_count = 15,
     .max_byte_count = 0
};

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

#define ITEM_IM_BORDER_STATE_CREATE(text, value) \
   ITEM_IM_BORDER_STATE_CALLBACK(value) \
   ITEM_IM_BORDER_STATE(text, value)

ITEM_1BOOL_STATE_CREATE("visible:", visible)
ITEM_2INT_STATE_CREATE("min:", min_w, min_h)
ITEM_2INT_STATE_CREATE("max:", max_w, max_h)
ITEM_2DOUBLE_STATE_CREATE("align:", align_x, align_y)
ITEM_2DOUBLE_STATE_CREATE("aspect:", aspect_min, aspect_max)
ITEM_COLOR_STATE_CREATE("color:", color)
ITEM_COLOR_STATE_CREATE("color2:", color2)
ITEM_COLOR_STATE_CREATE("color3:", color3)
ITEM_2DOUBLE_STATE_CREATE("relative:", rel1_relative_x, rel1_relative_y)
ITEM_2INT_STATE_CREATE("offset:", rel1_offset_x, rel1_offset_y)
ITEM_2DOUBLE_STATE_CREATE("relative:", rel2_relative_x, rel2_relative_y)
ITEM_2INT_STATE_CREATE("offset:", rel2_offset_x, rel2_offset_y)
ITEM_1STRING_STATE_CREATE("image:", image)
ITEM_IM_BORDER_STATE_CREATE("border:", image_border)
ITEM_1STRING_STATE_CREATE("text: ", text)
ITEM_1STRING_STATE_CREATE("font:", font)
ITEM_1INT_STATE_CREATE("size:", text_size)
ITEM_2DOUBLE_STATE_CREATE("align:", text_align_x, text_align_y)
ITEM_1DOUBLE_STATE_CREATE("elipsis:", text_elipsis)
ITEM_2BOOL_STATE_CREATE("fit:", text_fit_x, text_fit_y)

Evas_Object *
ui_prop_part_info_state_view_add(Evas_Object *part_view,
                                 Group *group,
                                 Part *part)
{
   Evas_Object *part_view_state, *box_state;
   Evas_Object *item;
   Evas_Object *rel1_frame, *rel1_box, *rel2_frame, *rel2_box;
   Evas_Object *text_frame, *text_box, *image_frame, *image_box;
   Prop_Part_View_Data *ppvd;
   Edje_Part_Type type;

   if ((!part_view) || (!group) || (!part)) return NULL;

   ppvd = evas_object_data_get(part_view, PROP_PART_VIEW_DATA);
   if (!ppvd)
     return NULL;

   part_view_state = elm_frame_add(ppvd->box);
   elm_frame_autocollapse_set(part_view_state, EINA_TRUE);
   evas_object_size_hint_fill_set(part_view_state, EVAS_HINT_FILL, 0.0);
   elm_object_style_set(part_view_state, "pad_small");
   evas_object_show(part_view_state);

   box_state = elm_box_add(part_view_state);
   evas_object_size_hint_weight_set(box_state, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(box_state, EVAS_HINT_FILL, 0.0);
   elm_box_align_set(box_state, 0.5, 0.0);
   elm_object_content_set(part_view_state, box_state);
   evas_object_show(box_state);

   type = edje_edit_part_type_get(group->obj, part->name);

   item = prop_item_state_visible_add(box_state, group, part,
                                      "Change part's visibility.");
   elm_box_pack_end(box_state, item);
   item = prop_item_state_min_w_min_h_add(box_state, group, part,
                                          0.0, 9999.0, 1.0, "%.0f",
                                          "Minimum part width in pixels.",
                                          "Minimum part height in pixels.");
   elm_box_pack_end(box_state, item);
   item = prop_item_state_max_w_max_h_add(box_state, group, part,
                                          -1.0, 9999.0, 1.0, "%.0f",
                                          "Maximum part width in pixels.",
                                          "Maximum part height in pixels.");
   elm_box_pack_end(box_state, item);
   item = prop_item_state_align_x_align_y_add(box_state, group, part,
                                              0.0, 1.0, 0.1, "%1.2f",
                                              "Part horizontal align: 0.0 = left  1.0 = right",
                                              "Part vertical align: 0.0 = top  1.0 = bottom");
   elm_box_pack_end(box_state, item);
   item = prop_item_state_aspect_min_aspect_max_add(box_state, group, part,
                                                    0.0, 1.0, 0.1, "%1.2f",
                                                    "Normally width and height can be "
                                                    "resized to any values independently",
                                                    "Normally width and height can be "
                                                    "resized to any values independently");
   elm_box_pack_end(box_state, item);
   /* addaspect perf */
   item = prop_item_state_color_add(box_state, group, part,
                                    "Part main color.");
   elm_box_pack_end(box_state, item);
   if (type == EDJE_PART_TYPE_TEXT)
     {
         item = prop_item_state_color2_add(box_state, group, part,
                                           "Text shadow color.");
         elm_box_pack_end(box_state, item);
         item = prop_item_state_color3_add(box_state, group, part,
                                           "Text outline color.");
         elm_box_pack_end(box_state, item);
     }

   rel1_frame = elm_frame_add(box_state);
   elm_frame_autocollapse_set(rel1_frame, EINA_FALSE);
   evas_object_size_hint_fill_set(rel1_frame, EVAS_HINT_FILL, 0.0);
   elm_object_text_set(rel1_frame, "rel1: top-left");
   evas_object_show(rel1_frame);

   rel1_box = elm_box_add(rel1_frame);
   evas_object_size_hint_weight_set(rel1_box, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(rel1_box, EVAS_HINT_FILL, 0.0);
   elm_box_align_set(rel1_box, 0.5, 0.0);
   elm_object_content_set(rel1_frame, rel1_box);
   evas_object_show(rel1_box);

   elm_box_pack_end(box_state, rel1_frame);
   item = prop_item_state_rel1_relative_x_rel1_relative_y_add(rel1_box, group, part,
                                                              -5.0, 5.0, 0.1, "%1.2f",
                                                              "Relative 1 x",
                                                              "Relative 1 y");
   elm_box_pack_end(rel1_box, item);
   item = prop_item_state_rel1_offset_x_rel1_offset_y_add(rel1_box, group, part,
                                                        -9999.0, 9999.0, 1.0, "%.0f",
                                                        "Left offset from relative position in pixels",
                                                        "Top offset from relative position in pixels");
   elm_box_pack_end(rel1_box, item);

   rel2_frame = elm_frame_add(box_state);
   elm_frame_autocollapse_set(rel2_frame, EINA_FALSE);
   evas_object_size_hint_fill_set(rel2_frame, EVAS_HINT_FILL, 0.0);
   elm_object_text_set(rel2_frame, "rel2: bottom-right");
   evas_object_show(rel2_frame);

   rel2_box = elm_box_add(rel2_frame);
   evas_object_size_hint_weight_set(rel2_box, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(rel2_box, EVAS_HINT_FILL, 0.0);
   elm_box_align_set(rel2_box, 0.5, 0.0);
   elm_object_content_set(rel2_frame, rel2_box);
   evas_object_show(rel2_box);
   elm_box_pack_end(box_state, rel2_frame);

   item = prop_item_state_rel2_relative_x_rel2_relative_y_add(rel2_box, group, part,
                                                              -5.0, 5.0, 0.1, "%1.2f",
                                                              "Relative 2 x",
                                                              "Relative 2 y");
   elm_box_pack_end(rel2_box, item);
   item = prop_item_state_rel2_offset_x_rel2_offset_y_add(rel2_box, group, part,
                                                     -9999.0, 9999.0, 1.0, "%.0f",
                                                     "Right offset from relative position in pixels",
                                                     "Bottom offset from relative position in pixels");
   elm_box_pack_end(rel2_box, item);

   if (type == EDJE_PART_TYPE_TEXT)
     {
        text_frame = elm_frame_add(box_state);
        elm_frame_autocollapse_set(text_frame, EINA_FALSE);
        evas_object_size_hint_fill_set(text_frame, EVAS_HINT_FILL, 0.0);
        elm_object_text_set(text_frame, "text");
        evas_object_show(text_frame);
        elm_box_pack_end(box_state, text_frame);

        text_box = elm_box_add(text_frame);
        evas_object_size_hint_weight_set(text_box, EVAS_HINT_EXPAND, 0.0);
        evas_object_size_hint_align_set(text_box, EVAS_HINT_FILL, 0.0);
        elm_box_align_set(text_box, 0.5, 0.0);
        elm_object_content_set(text_frame, text_box);
        evas_object_show(text_box);

        item = prop_item_state_text_add(text_box, group, part,
                                        "Set the text of part.");
        elm_box_pack_end(text_box, item);
        item = prop_item_state_font_add(text_box, group, part,
                                        "Change the text's font.");
        elm_box_pack_end(text_box, item);
        item = prop_item_state_text_size_add(text_box, group, part,
                                             0.0, 128.0, 1.0, "%.0f",
                                             "Change text font's size.");
        elm_box_pack_end(text_box, item);
        item = prop_item_state_text_align_x_text_align_y_add(text_box, group, part,
                                                             0.0, 1.0, 0.1, "%1.11f",
                                                             "Text horizontal align. "
                                                             "0.0 = left  1.0 = right",
                                                             "Text vertical align. "
                                                             "0.0 = top  1.0 = bottom");
        elm_box_pack_end(text_box, item);
        item = prop_item_state_text_elipsis_add(text_box, group, part,
                                                0.0, 1.0, 0.1, "%1.11f",
                                                "Cut text if biggest then part's area"
                                                "0.0 = fix the left side  1.0 = right side");
        elm_box_pack_end(text_box, item);
        item = prop_item_state_text_fit_x_text_fit_y_add(text_box, group, part,
                                                         "", "");
        elm_box_pack_end(text_box, item);
     }
   if (type == EDJE_PART_TYPE_IMAGE)
     {
        image_frame = elm_frame_add(box_state);
        elm_frame_autocollapse_set(image_frame, EINA_FALSE);
        evas_object_size_hint_fill_set(image_frame, EVAS_HINT_FILL, 0.0);
        elm_object_text_set(image_frame, "image");
        evas_object_show(image_frame);

        image_box = elm_box_add(image_frame);
        evas_object_size_hint_weight_set(image_box, EVAS_HINT_EXPAND, 0.0);
        evas_object_size_hint_align_set(image_box, EVAS_HINT_FILL, 0.0);
        elm_box_align_set(image_box, 0.5, 0.0);
        elm_object_content_set(image_frame, image_box);
        evas_object_show(image_box);
        elm_box_pack_end(box_state, image_frame);

        item = prop_item_state_image_add(image_box, group, part,
                                         "Set the image of part.");
        elm_box_pack_end(image_box, item);
        item = prop_item_state_image_border_add(image_box, group, part,
                                                "Lenght of left border in pixels");
        elm_box_pack_end(image_box, item);

     }
 /*
   item = ui_prop_view_item_one_edit_string_add(box_state, "state",
                                                buffer, "Unique state name for part.");
   item = ui_prop_view_item_two_edit_int_add(rel1_box, "offset",
                                             edje_edit_state_rel1_offset_x_get(EDJE_EDIT_FUNC_PARAMS),
                                             "Left offset from relative position in pixels",
                                             edje_edit_state_rel1_offset_y_get(EDJE_EDIT_FUNC_PARAMS),
                                             "Top offset from relative position in pixels");
   item = ui_prop_view_item_one_edit_string_add(rel1_box, "to_x",
                                                edje_edit_state_rel1_to_x_get(EDJE_EDIT_FUNC_PARAMS),
                                                "Left reference part.");
   item = ui_prop_view_item_one_edit_string_add(rel1_box, "to_y",
                                                edje_edit_state_rel1_to_y_get(EDJE_EDIT_FUNC_PARAMS),
                                                "Top reference part.");
   item = ui_prop_view_item_one_edit_string_add(rel2_box, "to_x",
                                                edje_edit_state_rel2_to_x_get(EDJE_EDIT_FUNC_PARAMS),
                                                "Left reference part.");
   item = ui_prop_view_item_one_edit_string_add(rel2_box, "to_y",
                                                edje_edit_state_rel2_to_y_get(EDJE_EDIT_FUNC_PARAMS),
                                                "Top reference part.");
   */
   return part_view_state;
}

void
ui_prop_part_info_state_set(Evas_Object *part_view, Evas_Object *state_view)
{
   Prop_Part_View_Data *ppvd;
   Evas_Object *box_part;

   if ((!part_view) || (!state_view))
     return;

   ppvd = evas_object_data_get(part_view, PROP_PART_VIEW_DATA);
   if (!ppvd)
     return;

   box_part = elm_object_content_get(part_view);
   if (ppvd->state.state)
     evas_object_del(ppvd->state.state);

   elm_box_pack_end(box_part, state_view);
   ppvd->state.state = state_view;
}

Evas_Object *
ui_prop_part_info_state_view_get(Evas_Object *part_view)
{
   Prop_Part_View_Data *ppvd;

   if (!part_view)
     return NULL;

   ppvd = evas_object_data_get(part_view, PROP_PART_VIEW_DATA);
   if (!ppvd)
     return NULL;

   return ppvd->state.state;
}

#undef OBJ_DATA
#undef PROP_PART_VIEW_DATA
#undef PROP_VIEW_DATA
