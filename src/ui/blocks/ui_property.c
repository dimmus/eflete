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

/* ===========================
            CALLBACKS
   =========================== */
#define ITEM_INT_GROUP_CALLBACK(Value) \
static void \
__on_##Value##_change(void *data, \
                      Evas_Object *obj, \
                      void *ei __UNUSED__) \
{ \
   Evas_Object *edje_obj = (Evas_Object *)data; \
   int value = (int)elm_spinner_value_get(obj); \
   edje_edit_##Value##_set(edje_obj, value); \
}

/* ===========================
             ITEMS
   =========================== */
#define ITEM_2SPINNER_GROUP(text, value1, value2) \
Evas_Object * \
prop_item_##value1##_##value2##_add(Evas_Object *parent, \
                                    Evas_Object *obj, \
                                    double min, \
                                    double max, \
                                    double step) \
{ \
   Evas_Object *item, *box, *spinner1, *spinner2; \
   ITEM_ADD(parent, item, text) \
   BOX_ADD(parent, box, EINA_TRUE, EINA_TRUE); \
   SPINNER_ADD(box, spinner1, min, max, step, EINA_TRUE, "vertical") \
   elm_spinner_value_set(spinner1, edje_edit_##value1##_get(obj)); \
   elm_box_pack_end(box, spinner1); \
   evas_object_smart_callback_add(spinner1, "changed", \
                                  __on_##value1##_change, obj); \
   SPINNER_ADD(box, spinner2, min, max, step, EINA_TRUE, "vertical") \
   elm_spinner_value_set(spinner2, edje_edit_##value2##_get(obj)); \
   elm_box_pack_end(box, spinner2); \
   evas_object_smart_callback_add(spinner2, "changed", \
                                  __on_##value2##_change, obj); \
   elm_object_part_content_set(item, "elm.swallow.content", box); \
   return item; \
}

#define ITEM_2INT_GROUP_CREATE(text, value1, value2) \
   ITEM_INT_GROUP_CALLBACK(value1) \
   ITEM_INT_GROUP_CALLBACK(value2) \
   ITEM_2SPINNER_GROUP(text, value1, value2)

/* ===================== */

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

   item = prop_item_group_min_w_group_min_h_add(box, group->obj,
                                                0.0, 9999.0, 1.0);
   elm_box_pack_end(box, item);
   item = prop_item_group_max_w_group_max_h_add(box, group->obj,
                                                -1.0, 9999.0, 1.0);
   elm_box_pack_end(box, item);

   /*
   item = ui_prop_view_item_two_edit_int_add(box, "min",
                                             edje_edit_group_min_w_get(group->obj),
                                             "Minimum group width in pixels.",
                                             edje_edit_group_min_h_get(group->obj),
                                             "Minimum group height in pixels.");
   evas_object_show(item);

   item = ui_prop_view_item_two_edit_int_add(box, "max",
                                             edje_edit_group_max_w_get(group->obj),
                                             "Maximum group width in pixels.",
                                             edje_edit_group_max_h_get(group->obj),
                                             "Maximum group height in pixels.");
   evas_object_show(item);
*/
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

/* ==========================
           CALLBACKS
   ========================== */
#define OBJ_DATA "obj_data"

#define ITEM_BOOL_PART_CALLBACK(Value) \
static void \
__on_##Value##_change(void *data, \
                                Evas_Object *obj, \
                                void *ei __UNUSED__) \
{ \
   Part *part = (Part *)data; \
   Evas_Object *edje_obj = evas_object_data_get(obj, OBJ_DATA); \
   Eina_Bool value = elm_check_state_get(obj); \
   edje_edit_##Value##_set(edje_obj, part->name, value); \
}

#define ITEM_1ENTRY_PART(text, value) \
void \
prop_item_##value##_add(Evas_Object *parent_box, \
                        Evas_Object *obj, \
                        Part *part) \
{ \
   Evas_Object *item_box, *entry; \
   ITEM_ADD(parent_box, item_box, text) \
   ENTRY_ADD(parent_box, entry, EINA_TRUE) \
   elm_entry_entry_set(entry, edje_edit_##value##_get(obj, \
                                                      part->name, \
                                                      part->curr_state, \
                                                      part->curr_state_value)); \
   elm_box_pack_end(item_box, entry); \
   evas_object_data_set(entry, OBJ_DATA, obj); \
   evas_object_smart_callback_add(entry, "activated", \
                                  __on_##value##_change, part); \
   elm_box_pack_end(parent_box, item_box); \
}

#define ITEM_1STRING_PART_CREATE(text, value) \
   ITEM_STRING_PART_CALLBACK(value) \
   ITEM_1ENTRY_PART(text, value)

// toggle
#define ITEM_1CHEACK_PART(text, value) \
Evas_Object * \
prop_item_##value##_add(Evas_Object *parent_box, \
                        Evas_Object *obj, \
                        Part *part) \
{ \
   Evas_Object *item, *check; \
   ITEM_ADD(parent_box, item, text) \
   CHECK_ADD(parent_box, check, "toggle") \
   elm_check_state_set(check, edje_edit_##value##_get(obj, \
                                                      part->name)); \
   evas_object_data_set(check, OBJ_DATA, obj); \
   evas_object_smart_callback_add(check, "changed", \
                                  __on_##value##_change, part); \
   elm_object_part_content_set(item, "elm.swallow.content", check); \
   return item; \
}

#define ITEM_1BOOL_PART_CREATE(text, value) \
   ITEM_BOOL_PART_CALLBACK(value) \
   ITEM_1CHEACK_PART(text, value)


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
  // Evas_Object *event_frame, *event_box;
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

   elm_object_content_set(part_view, box);

   item = prop_item_label_add(box_base, "name:", part->name);
   elm_box_pack_end(box_base, item);
   Edje_Part_Type type = edje_edit_part_type_get(group->obj, part->name);
   item = prop_item_label_add(box_base, "type:", wm_part_type_get(type));
   elm_box_pack_end(box_base, item);
   item = prop_item_part_scale_add(box_base, group->obj, part);
   elm_box_pack_end(box_base, item);
   item = prop_item_part_mouse_events_add(box_base, group->obj, part);
   elm_box_pack_end(box_base, item);
   item = prop_item_part_repeat_events_add(box_base, group->obj, part);
   elm_box_pack_end(box_base, item);

/*
   #define EDJE_EDIT_FUNC_PARAM group->obj, part->name
   item = ui_prop_view_item_one_edit_string_add(box_base, "name",
                                                part->name, "Unique name of part.");
   evas_object_show(item);

   item = ui_prop_view_item_label_add(box_base, "type",
                                      wm_part_type_get(edje_edit_part_type_get(EDJE_EDIT_FUNC_PARAM)));
   evas_object_show(item);

   item = ui_prop_view_item_one_edit_string_add(box_base, "clip to",
                                                edje_edit_part_clip_to_get(EDJE_EDIT_FUNC_PARAM),
                                                "Reference clipper part.");
   evas_object_show(item);

   item = ui_prop_view_item_one_edit_string_add(box_base, "source",
                                                edje_edit_part_source_get(EDJE_EDIT_FUNC_PARAM),
                                                "Source group selection.");
   evas_object_show(item);

   event_frame = elm_frame_add(box_base);
   elm_frame_autocollapse_set(event_frame, EINA_FALSE);
   evas_object_size_hint_fill_set(event_frame, EVAS_HINT_FILL, 0.0);
   elm_object_text_set(event_frame, "part events");
   evas_object_show(event_frame);

   event_box = elm_box_add(event_frame);
   evas_object_size_hint_weight_set(event_box, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(event_box, EVAS_HINT_FILL, 0.0);
   elm_box_align_set(event_box, 0.5, 0.0);
   elm_object_content_set(event_frame, event_box);
   evas_object_show(event_box);

   item = ui_prop_view_item_bool_add(event_box, "mouse",
                                     edje_edit_part_mouse_events_get(EDJE_EDIT_FUNC_PARAM),
                                     "Enable mouse events in this part.");
   evas_object_show(item);

   item = ui_prop_view_item_bool_add(event_box, "repeat",
                                     edje_edit_part_repeat_events_get(EDJE_EDIT_FUNC_PARAM),
                                     "Enable repeat mouse events to the parts below.");
   evas_object_show(item);

   elm_box_pack_end(box_base, event_frame);

   elm_object_content_set(event_frame, event_box);

   evas_object_show(box);
   elm_object_content_set(part_view, box);

   evas_object_data_set(part_view, PROP_PART_VIEW_DATA, ppvd);
   evas_object_event_callback_add(box, EVAS_CALLBACK_DEL,
                                  _on_part_view_del, NULL);

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
   Evas_Object *edje_obj = evas_object_data_get(obj, OBJ_DATA); \
   const char *value = elm_entry_entry_get(obj); \
   edje_edit_state_##Value##_set(edje_obj, part->name, \
                                part->curr_state, part->curr_state_value, \
                                value); \
}

#define ITEM_DOUBLE_STATE_CALLBACK(Value) \
static void \
__on_state_##Value##_change(void *data, \
                            Evas_Object *obj, \
                            void *ei __UNUSED__) \
{ \
   Part *part = (Part *)data; \
   Evas_Object *edje_obj = evas_object_data_get(obj, OBJ_DATA); \
   double value = elm_spinner_value_get(obj); \
   edje_edit_state_##Value##_set(edje_obj, part->name, \
                                 part->curr_state, part->curr_state_value, \
                                 value); \
}

#define ITEM_INT_STATE_CALLBACK(Value) \
static void \
__on_state_##Value##_change(void *data, \
                            Evas_Object *obj, \
                            void *ei __UNUSED__) \
{ \
   Part *part = (Part *)data; \
   Evas_Object *edje_obj = evas_object_data_get(obj, OBJ_DATA); \
   int value = (int)elm_spinner_value_get(obj); \
   edje_edit_state_##Value##_set(edje_obj, part->name, \
                                 part->curr_state, part->curr_state_value, \
                                 value); \
}

#define ITEM_BOOL_STATE_CALLBACK(Value) \
static void \
__on_state_##Value##_change(void *data, \
                            Evas_Object *obj, \
                            void *ei __UNUSED__) \
{ \
   Part *part = (Part *)data; \
   Evas_Object *edje_obj = evas_object_data_get(obj, OBJ_DATA); \
   Eina_Bool value = elm_check_state_get(obj); \
   edje_edit_state_##Value##_set(edje_obj, part->name, \
                                 part->curr_state, part->curr_state_value, \
                                 value); \
}

#define ITEM_COLOR_STATE_CALLBACK(Value) \
static void \
__on_state_##Value##_change(void *data, \
                            Evas_Object *obj, \
                            void *ei __UNUSED__) \
{ \
   Part *part = (Part *)data; \
   Evas_Object *edje_obj = evas_object_data_get(obj, OBJ_DATA); \
   const char *value = elm_entry_entry_get(obj); \
   char **c = eina_str_split(value, " ", 4); \
   edje_edit_state_##Value##_set(edje_obj, part->name, \
                                part->curr_state, part->curr_state_value, \
                                atoi(c[0]), atoi(c[1]), atoi(c[2]), atoi(c[3])); \
   free(c[0]); \
   free(c); \
}


#define ITEM_1CHEACK_STATE(text, value) \
Evas_Object * \
prop_item_state_##value##_add(Evas_Object *parent, \
                              Evas_Object *obj, \
                              Part *part) \
{ \
   Evas_Object *item, *check; \
   ITEM_ADD(parent, item, text) \
   CHECK_ADD(parent, check, "toggle") \
   elm_check_state_set(check, edje_edit_state_##value##_get(obj, \
                                                      part->name, \
                                                      part->curr_state, \
                                                      part->curr_state_value)); \
   evas_object_data_set(check, OBJ_DATA, obj); \
   evas_object_smart_callback_add(check, "changed", \
                                  __on_state_##value##_change, part); \
   elm_object_part_content_set(item, "elm.swallow.content", check); \
   return item; \
}

#define ITEM_1BOOL_STATE_CREATE(text, value) \
   ITEM_BOOL_STATE_CALLBACK(value) \
   ITEM_1CHEACK_STATE(text, value)


#define ITEM_2SPINNER_STATE(text, value1, value2) \
Evas_Object * \
prop_item_state_##value1##_##value2##_add(Evas_Object *parent, \
                                          Evas_Object *obj, \
                                          Part *part, \
                                          double min, \
                                          double max, \
                                          double step) \
{ \
   Evas_Object *item, *box, *spinner1, *spinner2; \
   ITEM_ADD(parent, item, text) \
   BOX_ADD(parent, box, EINA_TRUE, EINA_TRUE) \
   SPINNER_ADD(box, spinner1, min, max, step, EINA_TRUE, "vertical") \
   elm_spinner_value_set(spinner1, edje_edit_state_##value1##_get(obj, \
                                                            part->name, \
                                                            part->curr_state, \
                                                            part->curr_state_value)); \
   elm_box_pack_end(box, spinner1); \
   evas_object_data_set(spinner1, OBJ_DATA, obj); \
   evas_object_smart_callback_add(spinner1, "changed", \
                                  __on_state_##value1##_change, part); \
   SPINNER_ADD(box, spinner2, min, max, step, EINA_TRUE, "vertical") \
   elm_spinner_value_set(spinner2, edje_edit_state_##value2##_get(obj, \
                                                            part->name, \
                                                            part->curr_state, \
                                                            part->curr_state_value)); \
   elm_box_pack_end(box, spinner2); \
   evas_object_data_set(spinner2, OBJ_DATA, obj); \
   evas_object_smart_callback_add(spinner2, "changed", \
                                  __on_state_##value2##_change, part); \
   elm_object_part_content_set(item, "elm.swallow.content", box); \
   return item;\
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
                        Evas_Object *obj, \
                        Part *part) \
{ \
   Evas_Object *item, *box, *entry, *image; \
   char buff[BUFF_MAX]; \
   int r, g, b, a; \
   edje_edit_state_##value##_get(obj, part->name, \
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
   evas_object_data_set(entry, OBJ_DATA, obj); \
   evas_object_smart_callback_add(entry, "activated", \
                                  __on_state_##value##_change, part); \
   evas_object_color_set(image, r*a/255, g*a/255, b*a/255, a); \
   elm_box_pack_end(box, entry); \
   elm_box_pack_end(box, image); \
   elm_object_part_content_set(item, "elm.swallow.content", box); \
   return item; \
}

#define ITEM_2INT_STATE_CREATE(text, value1, value2) \
   ITEM_INT_STATE_CALLBACK(value1) \
   ITEM_INT_STATE_CALLBACK(value2) \
   ITEM_2SPINNER_STATE(text, value1, value2)

#define ITEM_2DOUBLE_STATE_CREATE(text, value1, value2) \
   ITEM_DOUBLE_STATE_CALLBACK(value1) \
   ITEM_DOUBLE_STATE_CALLBACK(value2) \
   ITEM_2SPINNER_STATE(text, value1, value2)

#define ITEM_COLOR_STATE_CREATE(text, value) \
   ITEM_COLOR_STATE_CALLBACK(value) \
   ITEM_COLOR_STATE(text, value)

ITEM_1BOOL_STATE_CREATE("visible:", visible)
ITEM_2INT_STATE_CREATE("min:", min_w, min_h)
ITEM_2INT_STATE_CREATE("max:", max_w, max_h)
ITEM_2DOUBLE_STATE_CREATE("align:", align_x, align_y)
ITEM_2DOUBLE_STATE_CREATE("aspect:", aspect_min, aspect_max)
ITEM_COLOR_STATE_CREATE("color:", color)

Evas_Object *
ui_prop_part_info_state_view_add(Evas_Object *part_view,
                                 Group *group,
                                 Part *part)
                                 //const char *state __UNUSED__,
                                 //double value __UNUSED__)
{
   Evas_Object *part_view_state, *box_state;
   Evas_Object *item;
   //Evas_Object *rel1_frame, *rel1_box, *rel2_frame, *rel2_box;
   //Evas_Object *text_frame, *text_box, *image_frame, *image_box;
   //int r, g, b, a;
   //int bl, br, bt, bb;
   Prop_Part_View_Data *ppvd;
   //char buffer[BUFF_MAX];

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


   item = prop_item_state_visible_add(box_state, group->obj, part);
   elm_box_pack_end(box_state, item);
   item = prop_item_state_min_w_min_h_add(box_state, group->obj, part, 0.0, 9999.0, 1.0);
   elm_box_pack_end(box_state, item);
   item = prop_item_state_max_w_max_h_add(box_state, group->obj, part, -1.0, 9999.0, 1.0);
   elm_box_pack_end(box_state, item);
   item = prop_item_state_align_x_align_y_add(box_state, group->obj, part, 0.0, 1.0, 0.1);
   elm_box_pack_end(box_state, item);
   item = prop_item_state_aspect_min_aspect_max_add(box_state, group->obj, part, 0.0, 1.0, 0.1);
   elm_box_pack_end(box_state, item);
   //aspect perf
   item = prop_item_state_color_add(box_state, group->obj, part);
   elm_box_pack_end(box_state, item);


 /*
   sprintf(buffer, "%s %f", state, value);
   item = ui_prop_view_item_one_edit_string_add(box_state, "state",
                                                buffer, "Unique state name for part.");
   evas_object_show(item);

   #define EDJE_EDIT_FUNC_PARAMS group->obj, part->name, state, value

   item = ui_prop_view_item_bool_add(box_state, "visible",
                                     edje_edit_state_visible_get(EDJE_EDIT_FUNC_PARAMS),
                                     "Change part's visibility.'");
   evas_object_show(item);

   item = ui_prop_view_item_two_edit_int_add(box_state, "min",
                                             edje_edit_state_min_w_get(EDJE_EDIT_FUNC_PARAMS),
                                             "Minimum part width in pixels.",
                                             edje_edit_state_min_h_get(EDJE_EDIT_FUNC_PARAMS),
                                             "Minimum part height in pixels.");
   evas_object_show(item);

   item = ui_prop_view_item_two_edit_int_add(box_state, "max",
                                             edje_edit_state_max_w_get(EDJE_EDIT_FUNC_PARAMS),
                                             "Maximum part width in pixels.",
                                             edje_edit_state_max_h_get(EDJE_EDIT_FUNC_PARAMS),
                                             "Maximum part height in pixels.");
   evas_object_show(item);

   edje_edit_state_color_get(EDJE_EDIT_FUNC_PARAMS, &r, &g, &b, &a);
   item = ui_prop_view_item_color_add(box_state, "color",
                                      r, g, b, a,
                                      "Part main color.");
   evas_object_show(item);

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

   item = ui_prop_view_item_two_edit_double_add(rel1_box, "rel1",
                                           edje_edit_state_rel1_relative_x_get(EDJE_EDIT_FUNC_PARAMS),
                                           "Relative 1 x",
                                           edje_edit_state_rel1_relative_y_get(EDJE_EDIT_FUNC_PARAMS),
                                           "Relative 1 y");
   evas_object_show(item);

   //
   //elm.swallow.content
   //field1.swallow
   Evas_Object *con1, *edit1;
   con1 = elm_layout_content_get(item, "elm.swallow.content");
   edit1 = elm_layout_content_get(con1, "field1.swallow");
   evas_object_data_set(edit1, "group", group);
   evas_object_smart_callback_add(edit1, "activated", __rel1_change, part->name);



   item = ui_prop_view_item_two_edit_int_add(rel1_box, "offset",
                                             edje_edit_state_rel1_offset_x_get(EDJE_EDIT_FUNC_PARAMS),
                                             "Left offset from relative position in pixels",
                                             edje_edit_state_rel1_offset_y_get(EDJE_EDIT_FUNC_PARAMS),
                                             "Top offset from relative position in pixels");
   evas_object_show(item);


   item = ui_prop_view_item_one_edit_string_add(rel1_box, "to_x",
                                                edje_edit_state_rel1_to_x_get(EDJE_EDIT_FUNC_PARAMS),
                                                "Left reference part.");
   evas_object_show(item);

   item = ui_prop_view_item_one_edit_string_add(rel1_box, "to_y",
                                                edje_edit_state_rel1_to_y_get(EDJE_EDIT_FUNC_PARAMS),
                                                "Top reference part.");
   evas_object_show(item);

   elm_box_pack_end(box_state, rel1_frame);

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

   item = ui_prop_view_item_two_edit_double_add(rel2_box, "rel2",
                                           edje_edit_state_rel2_relative_x_get(EDJE_EDIT_FUNC_PARAMS),
                                           "Relative 2 x",
                                           edje_edit_state_rel2_relative_y_get(EDJE_EDIT_FUNC_PARAMS),
                                           "Relative 2 y");
   evas_object_show(item);

   item = ui_prop_view_item_two_edit_int_add(rel2_box, "offset",
                                             edje_edit_state_rel2_offset_x_get(EDJE_EDIT_FUNC_PARAMS),
                                             "Left offset from relative position in pixels",
                                             edje_edit_state_rel2_offset_y_get(EDJE_EDIT_FUNC_PARAMS),
                                             "Top offset from relative position in pixels");
   evas_object_show(item);


   item = ui_prop_view_item_one_edit_string_add(rel2_box, "to_x",
                                                edje_edit_state_rel2_to_x_get(EDJE_EDIT_FUNC_PARAMS),
                                                "Left reference part.");
   evas_object_show(item);

   item = ui_prop_view_item_one_edit_string_add(rel2_box, "to_y",
                                                edje_edit_state_rel2_to_y_get(EDJE_EDIT_FUNC_PARAMS),
                                                "Top reference part.");
   evas_object_show(item);

   elm_box_pack_end(box_state, rel2_frame);

   item = ui_prop_view_item_two_edit_double_add(box_state, "align",
                                                edje_edit_state_align_x_get(EDJE_EDIT_FUNC_PARAMS),
                                                "Part horizontal align: 0.0 = left  1.0 = right",
                                                edje_edit_state_align_y_get(EDJE_EDIT_FUNC_PARAMS),
                                                "Part vertical align: 0.0 = left  1.0 = right");
   evas_object_show(item);

   item = ui_prop_view_item_two_edit_double_add(box_state, "aspect",
                                                edje_edit_state_aspect_min_get(EDJE_EDIT_FUNC_PARAMS),
                                                "Normally width and height can be"
                                                " resized to any values independently",
                                                edje_edit_state_aspect_max_get(EDJE_EDIT_FUNC_PARAMS),
                                                "Normally width and height can be"
                                                " resized to any values independently");
   evas_object_show(item);

   if (edje_edit_part_type_get(group->obj, part->name) == EDJE_PART_TYPE_TEXT)
     {
        text_frame = elm_frame_add(box_state);
        elm_frame_autocollapse_set(text_frame, EINA_FALSE);
        evas_object_size_hint_fill_set(text_frame, EVAS_HINT_FILL, 0.0);
        elm_object_text_set(text_frame, "text");
        evas_object_show(text_frame);

        text_box = elm_box_add(text_frame);
        evas_object_size_hint_weight_set(text_box, EVAS_HINT_EXPAND, 0.0);
        evas_object_size_hint_align_set(text_box, EVAS_HINT_FILL, 0.0);
        elm_box_align_set(text_box, 0.5, 0.0);
        elm_object_content_set(text_frame, text_box);
        evas_object_show(box_state);

        item = ui_prop_view_item_one_edit_string_add(text_box, "text",
                                                     edje_edit_state_text_get(EDJE_EDIT_FUNC_PARAMS),
                                                     "Set the text of part.");
        evas_object_show(item);

        item = ui_prop_view_item_one_edit_string_add(text_box, "font",
                                                     edje_edit_state_font_get(EDJE_EDIT_FUNC_PARAMS),
                                                     "Change the text's font.");
        evas_object_show(item);

        item = ui_prop_view_item_one_edit_int_add(text_box, "size",
                                                  edje_edit_state_text_size_get(EDJE_EDIT_FUNC_PARAMS),
                                                  "Change text font's size.");
        evas_object_show(item);

        item = ui_prop_view_item_two_edit_double_add(text_box, "align",
                                            edje_edit_state_text_align_x_get(EDJE_EDIT_FUNC_PARAMS),
                                            "Text horizontal align."
                                            " 0.0 = left  1.0 = right",
                                            edje_edit_state_text_align_y_get(EDJE_EDIT_FUNC_PARAMS),
                                            "Text vertical align."
                                            " 0.0 = top  1.0 = bottom");
        evas_object_show(item);

        item = ui_prop_view_item_one_edit_double_add(text_box, "elipsis",
                                            edje_edit_state_text_elipsis_get(EDJE_EDIT_FUNC_PARAMS),
                                           "Cut text if biggest then part's area"
                                           "0.0 = fix the left side  1.0 = right side");
        evas_object_show(item);

        item = ui_prop_view_item_two_bool_add(text_box, "fit",
                                              edje_edit_state_text_fit_x_get(EDJE_EDIT_FUNC_PARAMS),
                                              "",
                                              edje_edit_state_text_fit_y_get(EDJE_EDIT_FUNC_PARAMS),
                                              "");
        evas_object_show(item);

        edje_edit_state_color2_get(EDJE_EDIT_FUNC_PARAMS, &r, &g, &b, &a);
        item = ui_prop_view_item_color_add(text_box, "color2",
                                           r, g, b, a,
                                           "Text shadow color.");
        evas_object_show(item);

        edje_edit_state_color3_get(EDJE_EDIT_FUNC_PARAMS, &r, &g, &b, &a);
        item = ui_prop_view_item_color_add(text_box, "color3",
                                           r, g, b, a,
                                           "Text outline color.");
        evas_object_show(item);

        elm_box_pack_end(box_state, text_frame);
     }
   if (edje_edit_part_type_get(group->obj, part->name) == EDJE_PART_TYPE_IMAGE)
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

        item = ui_prop_view_item_one_edit_string_add(image_box, "image",
                                                     edje_edit_state_image_get(EDJE_EDIT_FUNC_PARAMS),
                                                     "Set the text of part.");
        evas_object_show(item);

        edje_edit_state_image_border_get(EDJE_EDIT_FUNC_PARAMS, &bl, &br, &bt, &bb);
        item = ui_prop_view_item_four_edit_int_add(image_box, "border",
                                                   bl, br, bt, bb,
                                                   "Lenght of left border in pixels");
        evas_object_show(item);

        elm_box_pack_end(box_state, image_frame);
     }
   #undef EDJE_EDIT_FUNC_PARAM
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
/*
#define ITEM_BASE_CREATE(parent, item, name)\
   item = elm_layout_add(parent); \
elm_layout_file_set(item, TET_EDJ, "property/item/default"); \
evas_object_size_hint_align_set(item, EVAS_HINT_FILL, EVAS_HINT_FILL); \
evas_object_size_hint_weight_set(item, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
elm_object_part_text_set(item, "elm.text", name);

static Evas_Object *
ui_prop_view_item_label_add(Evas_Object *prop, const char *name, const char *label)
{
   Evas_Object *item, *_label;

   ITEM_BASE_CREATE(prop, item, name)

   _label = elm_label_add(item);
   elm_object_text_set(_label, label);
   elm_label_ellipsis_set(_label, 1.0);
   elm_object_part_content_set(item, "elm.swallow.content", _label);
   evas_object_show(_label);
   elm_box_pack_end(prop, item);

   return item;
}

static Evas_Object *
ui_prop_view_item_one_edit_int_add(Evas_Object *prop, const char *name,
                                   int data, const char *tooltip)
{
   Evas_Object *item, *edit;
   char buffer [BUFF_MAX];

   ITEM_BASE_CREATE(prop, item, name)

   edit = elm_entry_add(item);
   elm_entry_single_line_set(edit, EINA_TRUE);
   elm_entry_single_line_set(edit, EINA_TRUE);
   elm_entry_scrollable_set(edit, EINA_TRUE);
   elm_entry_scrollbar_policy_set(edit, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
   sprintf(buffer, "%d", data);
   elm_object_text_set(edit, buffer);
   elm_object_tooltip_text_set(edit, tooltip);
   elm_object_part_content_set(item, "elm.swallow.content", edit);
   evas_object_show(edit);

   elm_box_pack_end(prop, item);
   return item;
}

static Evas_Object *
ui_prop_view_item_two_edit_int_add(Evas_Object *prop, const char *name,
                                   int data1, const char *tooltip1,
                                   int data2, const char *tooltip2)
{
   Evas_Object *item, *content, *edit1, *edit2;
   char buffer [BUFF_MAX];

   ITEM_BASE_CREATE(prop, item, name)

   content = elm_layout_add(item);
   elm_layout_file_set(content, TET_EDJ, "property/item/content/2filed");
   evas_object_size_hint_align_set(content, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(content, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(content);

   edit1 = elm_entry_add(item);
   elm_entry_single_line_set(edit1, EINA_TRUE);
   elm_entry_single_line_set(edit1, EINA_TRUE);
   elm_entry_scrollable_set(edit1, EINA_TRUE);
   elm_entry_scrollbar_policy_set(edit1, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
   sprintf(buffer, "%d", data1);
   elm_object_text_set(edit1, buffer);
   elm_object_tooltip_text_set(edit1, tooltip1);
   elm_object_part_content_set(content, "field1.swallow", edit1);
   evas_object_show(edit1);

   edit2 = elm_entry_add(item);
   elm_entry_single_line_set(edit2, EINA_TRUE);
   elm_entry_single_line_set(edit2, EINA_TRUE);
   elm_entry_scrollable_set(edit2, EINA_TRUE);
   elm_entry_scrollbar_policy_set(edit2, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
   sprintf(buffer, "%d", data2);
   elm_object_text_set(edit2, buffer);
   elm_object_tooltip_text_set(edit2, tooltip2);
   elm_object_part_content_set(content, "field2.swallow", edit2);
   evas_object_show(edit2);

   elm_object_part_content_set(item, "elm.swallow.content", content);

   elm_box_pack_end(prop, item);

   return item;
}

static Evas_Object *
ui_prop_view_item_four_edit_int_add(Evas_Object *prop, const char *name,
                                    int data1, int data2, int data3, int data4,
                                    const char *tooltip)
{
   Evas_Object *item, *content, *content2, *content3;
   Evas_Object *edit1, *edit2, *edit3, *edit4;
   char buffer [BUFF_MAX];

   ITEM_BASE_CREATE(prop, item, name)

   content = elm_layout_add(item);
   elm_layout_file_set(content, TET_EDJ, "property/item/content/2filed");
   evas_object_size_hint_align_set(content, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(content, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(content);

   content2 = elm_layout_add(item);
   elm_layout_file_set(content2, TET_EDJ, "property/item/content/2filed");
   evas_object_size_hint_align_set(content2, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(content2, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_object_part_content_set(content, "field1.swallow", content2);
   evas_object_show(content2);

   content3 = elm_layout_add(item);
   elm_layout_file_set(content3, TET_EDJ, "property/item/content/2filed");
   evas_object_size_hint_align_set(content3, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(content3, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_object_part_content_set(content, "field2.swallow", content3);
   evas_object_show(content3);

   edit1 = elm_entry_add(item);
   elm_entry_single_line_set(edit1, EINA_TRUE);
   elm_entry_single_line_set(edit1, EINA_TRUE);
   elm_entry_scrollable_set(edit1, EINA_TRUE);
   elm_entry_scrollbar_policy_set(edit1, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
   sprintf(buffer, "%d", data1);
   elm_object_text_set(edit1, buffer);
   elm_object_tooltip_text_set(edit1, tooltip);
   elm_object_part_content_set(content2, "field1.swallow", edit1);
   evas_object_show(edit1);

   edit2 = elm_entry_add(item);
   elm_entry_single_line_set(edit2, EINA_TRUE);
   elm_entry_single_line_set(edit2, EINA_TRUE);
   elm_entry_scrollable_set(edit2, EINA_TRUE);
   elm_entry_scrollbar_policy_set(edit2, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
   sprintf(buffer, "%d", data2);
   elm_object_text_set(edit2, buffer);
   elm_object_tooltip_text_set(edit2, tooltip);
   elm_object_part_content_set(content2, "field2.swallow", edit2);
   evas_object_show(edit2);

   edit3 = elm_entry_add(item);
   elm_entry_single_line_set(edit3, EINA_TRUE);
   elm_entry_single_line_set(edit3, EINA_TRUE);
   elm_entry_scrollable_set(edit3, EINA_TRUE);
   elm_entry_scrollbar_policy_set(edit3, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
   sprintf(buffer, "%d", data3);
   elm_object_text_set(edit3, buffer);
   elm_object_tooltip_text_set(edit3, tooltip);
   elm_object_part_content_set(content3, "field1.swallow", edit3);
   evas_object_show(edit3);

   edit4 = elm_entry_add(item);
   elm_entry_single_line_set(edit4, EINA_TRUE);
   elm_entry_single_line_set(edit4, EINA_TRUE);
   elm_entry_scrollable_set(edit4, EINA_TRUE);
   elm_entry_scrollbar_policy_set(edit4, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
   sprintf(buffer, "%d", data4);
   elm_object_text_set(edit4, buffer);
   elm_object_tooltip_text_set(edit4, tooltip);
   elm_object_part_content_set(content3, "field2.swallow", edit4);
   evas_object_show(edit4);

   elm_object_part_content_set(item, "elm.swallow.content", content);

   elm_box_pack_end(prop, item);

   return item;

}

static Evas_Object *
ui_prop_view_item_one_edit_string_add(Evas_Object *prop, const char *name,
                                      const char *data, const char *tooltip)
{
   Evas_Object *item, *edit;

   ITEM_BASE_CREATE(prop, item, name)

   edit = elm_entry_add(item);
   elm_entry_single_line_set(edit, EINA_TRUE);
   elm_entry_scrollable_set(edit, EINA_TRUE);
   elm_entry_scrollbar_policy_set(edit, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
   elm_object_text_set(edit, data);
   elm_object_tooltip_text_set(edit, tooltip);
   evas_object_show(edit);

   elm_object_part_content_set(item, "elm.swallow.content", edit);

   elm_box_pack_end(prop, item);

   return item;
}

static Evas_Object *
ui_prop_view_item_two_edit_string_add(Evas_Object *prop, const char *name,
                                      const char *data1, const char *tooltip1,
                                      const char *data2, const char *tooltip2)
{
   Evas_Object *item, *content, *edit1, *edit2;

   ITEM_BASE_CREATE(prop, item, name)

   content = elm_layout_add(item);
   elm_layout_file_set(content, TET_EDJ, "property/item/content/2filed");
   evas_object_size_hint_align_set(content, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(content, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(content);

   edit1 = elm_entry_add(item);
   elm_entry_single_line_set(edit1, EINA_TRUE);
   elm_entry_single_line_set(edit1, EINA_TRUE);
   elm_entry_scrollable_set(edit1, EINA_TRUE);
   elm_entry_scrollbar_policy_set(edit1, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
   elm_object_text_set(edit1, data1);
   elm_object_tooltip_text_set(edit1, tooltip1);
   elm_object_part_content_set(content, "field1.swallow", edit1);
   evas_object_show(edit1);

   edit2 = elm_entry_add(item);
   elm_entry_single_line_set(edit2, EINA_TRUE);
   elm_entry_single_line_set(edit2, EINA_TRUE);
   elm_entry_scrollable_set(edit2, EINA_TRUE);
   elm_entry_scrollbar_policy_set(edit2, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
   elm_object_text_set(edit2, data2);
   elm_object_tooltip_text_set(edit2, tooltip2);
   elm_object_part_content_set(content, "field2.swallow", edit2);
   evas_object_show(edit2);

   elm_object_part_content_set(item, "elm.swallow.content", content);

   elm_box_pack_end(prop, item);

   // TODO: add callback for modify data

   return item;
}


static Evas_Object *
ui_prop_view_item_bool_add(Evas_Object *prop, const char *name,
                           Eina_Bool check, const char *tooltip)
{
   Evas_Object *item, *toggle;

   ITEM_BASE_CREATE(prop, item, name)

   elm_theme_extension_add(NULL, TET_EDJ);
   toggle = elm_check_add(item);
   elm_object_style_set(toggle, "prop_toggle");
   evas_object_size_hint_align_set(toggle, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(toggle, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_object_tooltip_text_set(toggle, tooltip);
   elm_check_state_set(toggle, check);

   elm_object_part_content_set(item, "elm.swallow.content", toggle);

   elm_box_pack_end(prop, item);

   return item;
}

static Evas_Object *
ui_prop_view_item_two_bool_add(Evas_Object *prop, const char *name,
                               Eina_Bool check1, const char *tooltip1,
                               Eina_Bool check2, const char *tooltip2)
{
   Evas_Object *item, *toggle1, *toggle2, *content;

   ITEM_BASE_CREATE(prop, item, name)

   content = elm_layout_add(item);
   elm_layout_file_set(content, TET_EDJ, "property/item/content/2filed");
   evas_object_size_hint_align_set(content, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(content, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(content);

   toggle1 = elm_check_add(item);
   elm_object_style_set(toggle1, "prop_toggle");
   elm_object_tooltip_text_set(toggle1, tooltip1);
   evas_object_size_hint_align_set(toggle1, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(toggle1, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_check_state_set(toggle1, check1);
   elm_object_part_content_set(content, "field1.swallow", toggle1);
   evas_object_show(toggle1);

   toggle2 = elm_check_add(item);
   elm_object_style_set(toggle2, "prop_toggle");
   elm_object_tooltip_text_set(toggle2, tooltip2);
   evas_object_size_hint_align_set(toggle2, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(toggle2, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_check_state_set(toggle2, check2);
   elm_object_part_content_set(content, "field2.swallow", toggle2);
   evas_object_show(toggle2);

   elm_object_part_content_set(item, "elm.swallow.content", content);

   elm_box_pack_end(prop, item);

   return item;
}

static Evas_Object *
ui_prop_view_item_one_edit_double_add(Evas_Object *prop, const char *name,
                                      double data, const char *tooltip)
{
   Evas_Object *item, *edit;
   char buffer [BUFF_MAX];

   ITEM_BASE_CREATE(prop, item, name)

   edit = elm_entry_add(item);
   elm_entry_single_line_set(edit, EINA_TRUE);
   elm_entry_single_line_set(edit, EINA_TRUE);
   elm_entry_scrollable_set(edit, EINA_TRUE);
   elm_entry_scrollbar_policy_set(edit, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
   sprintf(buffer, "%f", data);
   elm_object_text_set(edit, buffer);
   elm_object_tooltip_text_set(edit, tooltip);
   evas_object_show(edit);

   elm_object_part_content_set(item, "elm.swallow.content", edit);

   elm_box_pack_end(prop, item);

   return item;
}

static Evas_Object *
ui_prop_view_item_two_edit_double_add(Evas_Object *prop, const char *name,
                                      double data1, const char *tooltip1,
                                      double data2, const char *tooltip2)
{
   Evas_Object *item, *content, *edit1, *edit2;
   char buffer [BUFF_MAX];

   ITEM_BASE_CREATE(prop, item, name)

   content = elm_layout_add(item);
   elm_layout_file_set(content, TET_EDJ, "property/item/content/2filed");
   evas_object_size_hint_align_set(content, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(content, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(content);

   edit1 = elm_entry_add(item);
   elm_entry_single_line_set(edit1, EINA_TRUE);
   elm_entry_single_line_set(edit1, EINA_TRUE);
   elm_entry_scrollable_set(edit1, EINA_TRUE);
   elm_entry_scrollbar_policy_set(edit1, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
   sprintf(buffer, "%f", data1);
   elm_object_text_set(edit1, buffer);
   elm_object_tooltip_text_set(edit1, tooltip1);
   elm_object_part_content_set(content, "field1.swallow", edit1);
   evas_object_show(edit1);

   edit2 = elm_entry_add(item);
   elm_entry_single_line_set(edit2, EINA_TRUE);
   elm_entry_single_line_set(edit2, EINA_TRUE);
   elm_entry_scrollable_set(edit2, EINA_TRUE);
   elm_entry_scrollbar_policy_set(edit2, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
   sprintf(buffer, "%f", data2);
   elm_object_text_set(edit2, buffer);
   elm_object_tooltip_text_set(edit2, tooltip2);
   elm_object_part_content_set(content, "field2.swallow", edit2);
   evas_object_show(edit2);

   elm_object_part_content_set(item, "elm.swallow.content", content);

   elm_box_pack_end(prop, item);

   return item;
}

Evas_Object *
ui_prop_view_item_color_add(Evas_Object *prop, const char *name,
                            int r, int g, int b, int a,
                            const char *tooltip)
{
   Evas_Object *item, *content, *content2, *content3;
   Evas_Object *edit1, *edit2, *edit3, *color_rect;
   char buffer [BUFF_MAX];

   ITEM_BASE_CREATE(prop, item, name)

   content = elm_layout_add(item);
   elm_layout_file_set(content, TET_EDJ, "property/item/content/2filed");
   evas_object_size_hint_align_set(content, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(content, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(content);

   content2 = elm_layout_add(item);
   elm_layout_file_set(content2, TET_EDJ, "property/item/content/2filed");
   evas_object_size_hint_align_set(content2, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(content2, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_object_part_content_set(content, "field1.swallow", content2);
   evas_object_show(content2);

   content3 = elm_layout_add(item);
   elm_layout_file_set(content3, TET_EDJ, "property/item/content/2filed");
   evas_object_size_hint_align_set(content3, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(content3, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_object_part_content_set(content, "field2.swallow", content3);
   evas_object_show(content3);

   edit1 = elm_entry_add(item);
   elm_entry_single_line_set(edit1, EINA_TRUE);
   elm_entry_single_line_set(edit1, EINA_TRUE);
   elm_entry_scrollable_set(edit1, EINA_TRUE);
   elm_entry_scrollbar_policy_set(edit1, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
   sprintf(buffer, "%d", r);
   elm_object_text_set(edit1, buffer);
   elm_object_tooltip_text_set(edit1, tooltip);
   elm_object_part_content_set(content2, "field1.swallow", edit1);
   evas_object_show(edit1);

   edit2 = elm_entry_add(item);
   elm_entry_single_line_set(edit2, EINA_TRUE);
   elm_entry_single_line_set(edit2, EINA_TRUE);
   elm_entry_scrollable_set(edit2, EINA_TRUE);
   elm_entry_scrollbar_policy_set(edit2, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
   sprintf(buffer, "%d", g);
   elm_object_text_set(edit2, buffer);
   elm_object_tooltip_text_set(edit2, tooltip);
   elm_object_part_content_set(content2, "field2.swallow", edit2);
   evas_object_show(edit2);

   edit3 = elm_entry_add(item);
   elm_entry_single_line_set(edit3, EINA_TRUE);
   elm_entry_single_line_set(edit3, EINA_TRUE);
   elm_entry_scrollable_set(edit3, EINA_TRUE);
   elm_entry_scrollbar_policy_set(edit3, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);
   sprintf(buffer, "%d", b);
   elm_object_text_set(edit3, buffer);
   elm_object_tooltip_text_set(edit3, tooltip);
   elm_object_part_content_set(content3, "field1.swallow", edit3);
   evas_object_show(edit3);

   color_rect = evas_object_rectangle_add(evas_object_evas_get(item));
   if (a <= 0) evas_object_color_set(color_rect, 0, 0, 0, 0);
   else evas_object_color_set(color_rect, r, g, b, a);
   elm_object_part_content_set(content3, "field2.swallow", color_rect);
   elm_object_tooltip_text_set(color_rect, tooltip);
   evas_object_show(color_rect);

   elm_object_part_content_set(item, "elm.swallow.content", content);

   elm_box_pack_end(prop, item);

   return item;
}
*/

#undef ITEM_BASE_CREATE

#undef PROP_PART_VIEW_DATA
#undef PROP_VIEW_DATA
