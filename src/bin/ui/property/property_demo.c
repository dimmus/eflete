#include "property.h"
#include "property_private.h"
#include "demo_group.h"
#include "project_manager2.h"
#include "tabs.h"

#define DEMO_MASK(TYPE) (1u << TYPE)
#define DEMO_SIGNAL     DEMO_MASK(EDJE_PART_TYPE_NONE) /* in case of SIGNAL things are different */
#define DEMO_TEXT       DEMO_MASK(EDJE_PART_TYPE_TEXT)
#define DEMO_TEXTBLOCK  DEMO_MASK(EDJE_PART_TYPE_TEXTBLOCK)
#define DEMO_SWALLOW    DEMO_MASK(EDJE_PART_TYPE_SWALLOW)

#define DEMO_SWALLOW_NONE DEMO_MASK(CONTENT_NONE)
#define DEMO_SWALLOW_RECT DEMO_MASK(CONTENT_RECTANGLE)
#define DEMO_SWALLOW_IMAGE DEMO_MASK(CONTENT_IMAGE)
#define DEMO_SWALLOW_WIDGET DEMO_MASK(CONTENT_WIDGET)

struct _Property_Demo_Data {
   Demo_Part *part;
   Demo_Signal *signal;
   Property_Attribute items[PROPERTY_DEMO_ITEM_LAST];
};
typedef struct _Property_Demo_Data Property_Demo_Data;
static Property_Demo_Data demo_pd;

struct _Property_Demo_Update_Info {
   Property_Attribute *pa;
   Property_Action *action;
};
typedef struct _Property_Demo_Update_Info Property_Demo_Update_Info;

static const char *program_actions[] = {
     "NONE",
     "STATE_SET",
     "ACTION_STOP",
     "SIGNAL_EMIT",
     "DRAG_VAL_SET",
     "DRAG_VAL_STEP",
     "DRAG_VAL_PAGE",
     "SCRIPT",
     "FOCUS_SET",
     "[NOT SUPPORTED] RESERVED00",
     "FOCUS_OBJECT",
     "[NOT SUPPORTED] PARAM_COPY",
     "[NOT SUPPORTED] PARAM_SET",
     "SOUND_SAMPLE",
     "SOUND_TONE",
     "[NOT SUPPORTED] PHYSICS_IMPULSE",
     "[NOT SUPPORTED] PHYSICS_TORQUE_IMPULSE",
     "[NOT SUPPORTED] PHYSICS_FORCE",
     "[NOT SUPPORTED] PHYSICS_TORQUE",
     "[NOT SUPPORTED] PHYSICS_FORCES_CLEAR",
     "[NOT SUPPORTED] PHYSICS_VEL_SET",
     "[NOT SUPPORTED] PHYSICS_ANG_VEL_SET",
     "[NOT SUPPORTED] PHYSICS_STOP",
     "[NOT SUPPORTED] PHYSICS_ROT_SET",
     "[NOT SUPPORTED] VIBRATION_SAMPLE"
};


static const char *swallow_content_type[] = {
     "None",
     "Rectangle",
     "Picture",
     "Widget",
     NULL};

static const char *widget_type[] = { "layout",
                                     "button",
                                     "check",
                                     "slider",
                                     "entry",
                                     "progressbar",
                                     "radio",
                                     "spinner",
                                     "label",
                                     "calendar",
                                     "icon",
                                     "colorselector",
                                     "fileselector",
                                     "dayselector",
                                     "datetime",
                                     "frame",
                                     "panel",
                                     "segment_control",
                                     "clock",
                                     "panes",
                                     "list",
                                     "grid",
                                     "table",
                                     "box",
                                     "naviframe",
                                     "genlist",
                                     "gengrid",
                                     "scroller",
                                     "toolbar",
                                     "map",
                                     NULL};

/* array to find item by Attribute */
static Property_Demo_Update_Info attribute_map[ATTRIBUTE_DEMO_ITEM_LAST];

/* local callbacks */
static Eina_List *
_subitems_get(Property_Attribute *pa)
{
   Eina_List *items = NULL;

   assert(pa != NULL);
#define APPEND(TYPE) items = eina_list_append(items, &demo_pd.items[TYPE]);
   switch (pa->type.demo_item)
     {
      case PROPERTY_DEMO_ITEM_TEXT_TITLE:
         APPEND(PROPERTY_DEMO_ITEM_TEXT_NAME);
         APPEND(PROPERTY_DEMO_ITEM_TEXT_CONTENT);
         break;
      case PROPERTY_DEMO_ITEM_SWALLOW_TITLE:
         APPEND(PROPERTY_DEMO_ITEM_SWALLOW_NAME);
         APPEND(PROPERTY_DEMO_ITEM_SWALLOW_CONTENT);
         APPEND(PROPERTY_DEMO_ITEM_SWALLOW_PICTURE);
         APPEND(PROPERTY_DEMO_ITEM_SWALLOW_WIDGET);
         APPEND(PROPERTY_DEMO_ITEM_SWALLOW_STYLE);
         APPEND(PROPERTY_DEMO_ITEM_SWALLOW_LAYOUT_GROUP);
         APPEND(PROPERTY_DEMO_ITEM_SWALLOW_COLOR);
         APPEND(PROPERTY_DEMO_ITEM_SWALLOW_MIN);
         APPEND(PROPERTY_DEMO_ITEM_SWALLOW_MAX);
         break;
      case PROPERTY_DEMO_ITEM_PROGRAM_TITLE:
         APPEND(PROPERTY_DEMO_ITEM_PROGRAM_SIGNAL);
         APPEND(PROPERTY_DEMO_ITEM_PROGRAM_SOURCE);
         APPEND(PROPERTY_DEMO_ITEM_PROGRAM_ACTION);
         APPEND(PROPERTY_DEMO_ITEM_PROGRAM_EMIT);
         APPEND(PROPERTY_DEMO_ITEM_PROGRAM_EMITTER);
         break;
      default:
         CRIT("items callback not found for %s", pa->name);
         abort();
     }

   return items;
#undef APPEND
}

static void
_fill_combobox_with_enum(Evas_Object *control, const char **array)
{
   unsigned int i = 0;
   Combobox_Item *combobox_item;
   Elm_Genlist_Item_Class *itc;

   assert(control != NULL);
   assert(array != NULL);

   itc = evas_object_data_get(control, "COMMON_ITC");

   while (array[i] != NULL)
     {
        combobox_item = mem_malloc(sizeof(Combobox_Item));
        combobox_item->index = i;
        combobox_item->data = eina_stringshare_add(array[i]);
        elm_genlist_item_append(control, itc,
                                combobox_item, NULL,
                                ELM_GENLIST_ITEM_NONE, NULL, NULL);
        ++i;
     }
}

static void
_fill_combobox_group(Evas_Object *control)
{
   unsigned int i = 0;
   Combobox_Item *combobox_item;
   Elm_Genlist_Item_Class *itc;
   Eina_List *l;
   Group2 *group;
   Group2 *current_group = tabs_current_group_get();

   assert(control != NULL);

   itc = evas_object_data_get(control, "COMMON_ITC");

   elm_genlist_clear(control);

   combobox_item = mem_malloc(sizeof(Combobox_Item));
   combobox_item->index = i++;
   combobox_item->data = eina_stringshare_add("None");
   elm_genlist_item_append(control, itc,
                           combobox_item, NULL,
                           ELM_GENLIST_ITEM_NONE, NULL, NULL);

   EINA_LIST_FOREACH(ap.project->RM.groups, l, group)
     {
        if (group == current_group) continue;
        combobox_item = mem_malloc(sizeof(Combobox_Item));
        combobox_item->index = i++;
        combobox_item->data = eina_stringshare_add(group->common.name);
        elm_genlist_item_append(control, itc,
                                combobox_item, NULL,
                                ELM_GENLIST_ITEM_NONE, NULL, NULL);
     }
}

static void
_change_cb(Property_Attribute *pa, Property_Action *action)
{
   Eina_Stringshare *str_val1 = NULL;
   Combobox_Item *cb_item_combo = NULL;
   double double_val1 = 0.0;
   int r = -1, g = -1, b = -1, a = -1;

   assert(pa != NULL);
   assert(action != NULL);
   assert(action->control != NULL);

   PROPERTY_DATA_GET(action->control);

   switch (action->control_type)
     {
      case PROPERTY_CONTROL_SPINNER:
         double_val1 = elm_spinner_value_get(action->control);
         break;
      case PROPERTY_CONTROL_IMAGE_SELECTOR:
      case PROPERTY_CONTROL_ENTRY:
         str_val1 = property_entry_get(action->control);
         break;
      case PROPERTY_CONTROL_COMBOBOX:
         cb_item_combo = evas_object_data_get(action->control, "CURRENT_DATA");
         if (!cb_item_combo) return;
         break;
      case PROPERTY_CONTROL_COLOR:
         property_color_control_color_get(action->control, &r, &g, &b, &a);
         break;
      default:
         break;
     }

   switch (action->type.attribute_demo)
     {
      case ATTRIBUTE_DEMO_ITEM_TEXT_CONTENT:
         if (demo_pd.part->text_content)
           eina_stringshare_del(demo_pd.part->text_content);
         demo_pd.part->text_content = eina_stringshare_add(str_val1);
         demo_pd.part->change = true;
         evas_object_smart_callback_call(ap.win, SIGNAL_DEMO_TEXT_SET, demo_pd.part);
         break;
      case ATTRIBUTE_DEMO_ITEM_SWALLOW_CONTENT:
         assert(cb_item_combo != NULL);
         demo_pd.part->swallow_content = cb_item_combo->index;
         demo_pd.part->change = true;
         GENLIST_FILTER_APPLY(pd->genlist);
         elm_object_text_set(action->control, swallow_content_type[demo_pd.part->swallow_content]);
         evas_object_smart_callback_call(ap.win, SIGNAL_DEMO_SWALLOW_SET, demo_pd.part);
         break;

      case ATTRIBUTE_DEMO_ITEM_SWALLOW_WIDGET:
         assert(cb_item_combo != NULL);
         demo_pd.part->widget = cb_item_combo->index;
         demo_pd.part->change = true;
         GENLIST_FILTER_APPLY(pd->genlist);
         elm_object_text_set(action->control, widget_type[demo_pd.part->widget]);
         evas_object_smart_callback_call(ap.win, SIGNAL_DEMO_SWALLOW_SET, demo_pd.part);
         break;
      case ATTRIBUTE_DEMO_ITEM_SWALLOW_STYLE:
         if (demo_pd.part->content_style)
           eina_stringshare_del(demo_pd.part->content_style);
         demo_pd.part->content_style = eina_stringshare_add(str_val1);
         demo_pd.part->change = true;
         evas_object_smart_callback_call(ap.win, SIGNAL_DEMO_SWALLOW_SET, demo_pd.part);
         break;
      case ATTRIBUTE_DEMO_ITEM_SWALLOW_LAYOUT_GROUP:
         assert(cb_item_combo != NULL);
         if (demo_pd.part->content_style)
           eina_stringshare_del(demo_pd.part->content_style);
         demo_pd.part->content_style = eina_stringshare_add(cb_item_combo->data);
         demo_pd.part->widget = 0;
         demo_pd.part->change = true;
         elm_object_text_set(action->control, cb_item_combo->data);
         evas_object_smart_callback_call(ap.win, SIGNAL_DEMO_SWALLOW_SET, demo_pd.part);
         break;
      case ATTRIBUTE_DEMO_ITEM_SWALLOW_PICTURE:
         if (demo_pd.part->image_path)
           eina_stringshare_del(demo_pd.part->image_path);
         demo_pd.part->image_path = eina_stringshare_add(str_val1);
         demo_pd.part->change = true;
         evas_object_smart_callback_call(ap.win, SIGNAL_DEMO_SWALLOW_SET, demo_pd.part);
         break;
      case ATTRIBUTE_DEMO_ITEM_SWALLOW_COLOR:
         demo_pd.part->r = r;
         demo_pd.part->g = g;
         demo_pd.part->b = b;
         demo_pd.part->a = a;
         evas_object_smart_callback_call(ap.win, SIGNAL_DEMO_SWALLOW_SET, demo_pd.part);
         break;
      case ATTRIBUTE_DEMO_ITEM_SWALLOW_MIN_W:
         demo_pd.part->min_w = double_val1;
         evas_object_smart_callback_call(ap.win, SIGNAL_DEMO_SWALLOW_SET, demo_pd.part);
         break;
      case ATTRIBUTE_DEMO_ITEM_SWALLOW_MIN_H:
         demo_pd.part->min_h = double_val1;
         evas_object_smart_callback_call(ap.win, SIGNAL_DEMO_SWALLOW_SET, demo_pd.part);
         break;
      case ATTRIBUTE_DEMO_ITEM_SWALLOW_MAX_W:
         demo_pd.part->max_w = double_val1;
         evas_object_smart_callback_call(ap.win, SIGNAL_DEMO_SWALLOW_SET, demo_pd.part);
         break;
      case ATTRIBUTE_DEMO_ITEM_SWALLOW_MAX_H:
         demo_pd.part->max_h = double_val1;
         evas_object_smart_callback_call(ap.win, SIGNAL_DEMO_SWALLOW_SET, demo_pd.part);
         break;
      default:
         TODO("remove default case after all attributes will be added");
         CRIT("change callback not found for %s (%s)", pa->name, action->name ? action->name : "unnamed");
         abort();
         break;
     }
}

static Eina_Bool
_update_cb(Property_Attribute *pa, Property_Action *action)
{
   assert(pa != NULL);
   assert(action != NULL);
   assert(action->control != NULL);

   switch (action->type.attribute_demo)
     {
      case ATTRIBUTE_DEMO_ITEM_TEXT_NAME:
         elm_layout_text_set(action->control, NULL, demo_pd.part->name);
         break;
      case ATTRIBUTE_DEMO_ITEM_TEXT_CONTENT:
         property_entry_set(action->control, demo_pd.part->text_content);
         break;

      case ATTRIBUTE_DEMO_ITEM_SWALLOW_NAME:
         elm_layout_text_set(action->control, NULL, demo_pd.part->name);
         break;
      case ATTRIBUTE_DEMO_ITEM_SWALLOW_CONTENT:
         elm_object_text_set(action->control, swallow_content_type[demo_pd.part->swallow_content]);
         break;
      case ATTRIBUTE_DEMO_ITEM_SWALLOW_PICTURE:
         property_entry_set(action->control, demo_pd.part->image_path);
         break;
      case ATTRIBUTE_DEMO_ITEM_SWALLOW_WIDGET:
         elm_object_text_set(action->control, widget_type[demo_pd.part->widget]);
         break;
      case ATTRIBUTE_DEMO_ITEM_SWALLOW_STYLE:
         property_entry_set(action->control, demo_pd.part->content_style);
         break;
      case ATTRIBUTE_DEMO_ITEM_SWALLOW_LAYOUT_GROUP:
         _fill_combobox_group(action->control);
         elm_object_text_set(action->control, demo_pd.part->content_style);
         break;
      case ATTRIBUTE_DEMO_ITEM_SWALLOW_COLOR:
         property_color_control_color_set(action->control,
                                          demo_pd.part->r,
                                          demo_pd.part->g,
                                          demo_pd.part->b,
                                          demo_pd.part->a);
         break;
      case ATTRIBUTE_DEMO_ITEM_SWALLOW_MIN_W:
         elm_spinner_value_set(action->control, demo_pd.part->min_w);
         break;
      case ATTRIBUTE_DEMO_ITEM_SWALLOW_MIN_H:
         elm_spinner_value_set(action->control, demo_pd.part->min_h);
         break;
      case ATTRIBUTE_DEMO_ITEM_SWALLOW_MAX_W:
         elm_spinner_value_set(action->control, demo_pd.part->max_w);
         break;
      case ATTRIBUTE_DEMO_ITEM_SWALLOW_MAX_H:
         elm_spinner_value_set(action->control, demo_pd.part->max_h);
         break;

      case ATTRIBUTE_DEMO_ITEM_PROGRAM_SIGNAL:
         elm_layout_text_set(action->control, NULL, demo_pd.signal->sig_name);
         break;
      case ATTRIBUTE_DEMO_ITEM_PROGRAM_SOURCE:
         elm_layout_text_set(action->control, NULL, demo_pd.signal->source_name);
         break;
      case ATTRIBUTE_DEMO_ITEM_PROGRAM_ACTION:
         elm_layout_text_set(action->control, NULL, program_actions[demo_pd.signal->action]);
         break;
      case ATTRIBUTE_DEMO_ITEM_PROGRAM_EMIT:
         elm_layout_text_set(action->control, NULL, demo_pd.signal->emit_signal);
         break;
      case ATTRIBUTE_DEMO_ITEM_PROGRAM_EMITTER:
         elm_layout_text_set(action->control, NULL, demo_pd.signal->emitter);
         break;
      default:
         TODO("remove default case after all attributes will be added");
         CRIT("update callback not found for %s (%s)", pa->name, action->name ? action->name : "unnamed");
         abort();
         break;
     }
   return true;
}

static void
_init_cb(Property_Attribute *pa, Property_Action *action)
{
   assert(pa != NULL);
   assert(action != NULL);
   assert(action->control != NULL);

   switch (action->type.attribute_demo)
     {
      case ATTRIBUTE_DEMO_ITEM_SWALLOW_LAYOUT_GROUP:
         elm_object_text_set(action->control, "None");
      break;
      case ATTRIBUTE_DEMO_ITEM_SWALLOW_PICTURE:
      case ATTRIBUTE_DEMO_ITEM_TEXT_NAME:
      case ATTRIBUTE_DEMO_ITEM_TEXT_CONTENT:
      case ATTRIBUTE_DEMO_ITEM_SWALLOW_NAME:
      case ATTRIBUTE_DEMO_ITEM_SWALLOW_STYLE:
      case ATTRIBUTE_DEMO_ITEM_SWALLOW_COLOR:
      case ATTRIBUTE_DEMO_ITEM_PROGRAM_SIGNAL:
      case ATTRIBUTE_DEMO_ITEM_PROGRAM_SOURCE:
      case ATTRIBUTE_DEMO_ITEM_PROGRAM_ACTION:
      case ATTRIBUTE_DEMO_ITEM_PROGRAM_EMIT:
      case ATTRIBUTE_DEMO_ITEM_PROGRAM_EMITTER:
         break;
      case ATTRIBUTE_DEMO_ITEM_SWALLOW_CONTENT:
         _fill_combobox_with_enum(action->control, swallow_content_type);
         break;
      case ATTRIBUTE_DEMO_ITEM_SWALLOW_WIDGET:
         _fill_combobox_with_enum(action->control, widget_type);
         break;
      case ATTRIBUTE_DEMO_ITEM_SWALLOW_MIN_W:
      case ATTRIBUTE_DEMO_ITEM_SWALLOW_MIN_H:
      case ATTRIBUTE_DEMO_ITEM_SWALLOW_MAX_W:
      case ATTRIBUTE_DEMO_ITEM_SWALLOW_MAX_H:
         elm_spinner_min_max_set(action->control, 0, 9999);
         break;
      default:
         TODO("remove default case after all attributes will be added");
         CRIT("init callback not found for %s (%s)", pa->name, action->name ? action->name : "unnamed");
         abort();
         break;
     }
}

/* blocks */
static inline void
_action_internal(Property_Action *action, const char *name, const char *units,
                 Property_Control control_type, Attribute_Demo_Item attribute)
{
   /* check that we haven't inited this action already */
   assert(action->update_cb == NULL);

   action->name = name;
   action->units = units;
   action->control_type = control_type;
   action->type.attribute_demo = attribute;
   action->init_cb = _init_cb;
   action->update_cb = _update_cb;
   action->change_cb = _change_cb;
}

static inline void
_action1(Property_Attribute *pa, const char *name, const char *units,
         Property_Control control_type, Attribute_Demo_Item attribute)
{
   _action_internal(&pa->action1, name, units, control_type, attribute);

   assert(attribute_map[attribute].pa == NULL);
   assert(attribute_map[attribute].action == NULL);

   attribute_map[attribute].pa = pa;
   attribute_map[attribute].action = &pa->action1;
}

static inline void
_action2(Property_Attribute *pa, const char *name, const char *units,
         Property_Control control_type, Attribute_Demo_Item attribute)
{
   _action_internal(&pa->action2, name, units, control_type, attribute);

   assert(attribute_map[attribute].pa == NULL);
   assert(attribute_map[attribute].action == NULL);

   attribute_map[attribute].pa = pa;
   attribute_map[attribute].action = &pa->action2;
}

static Eina_Bool
_filter_cb(Property_Attribute *pa)
{
   assert(pa != NULL);

   if (demo_pd.part)
     return !!(pa->filter_data.demo_types & DEMO_MASK(demo_pd.part->type));
   else if (demo_pd.signal)
     return !!(pa->filter_data.demo_types & DEMO_MASK(EDJE_PART_TYPE_NONE));
   else
     return false;
}

static Eina_Bool
_filter_swallow_cb(Property_Attribute *pa)
{
   assert(pa != NULL);
   assert(demo_pd.part != NULL);
   assert(demo_pd.part->type == EDJE_PART_TYPE_SWALLOW);

   if (!!(pa->filter_data.demo_types & DEMO_MASK(demo_pd.part->swallow_content)))
     {
        if (PROPERTY_DEMO_ITEM_SWALLOW_STYLE == pa->type.demo_item)
          return demo_pd.part->widget != 0;
        else if (PROPERTY_DEMO_ITEM_SWALLOW_LAYOUT_GROUP == pa->type.demo_item)
          return demo_pd.part->widget == 0;
        else
          return true;
     }
   return false;
}

static void
_init_items()
{
   Property_Demo_Item it;
#define IT demo_pd.items[it]
   for (it = 0 /* first element of enum */; it < PROPERTY_DEMO_ITEM_LAST; it++)
     {
        IT.type.demo_item = it;
        IT.filter_cb = _filter_cb;

        /* default value: show for all types */
        IT.filter_data.demo_types = ~0u;
        switch(it)
          {
           case PROPERTY_DEMO_ITEM_TEXT_TITLE:
              IT.name = "Text";
              IT.expandable = true;
              IT.expanded = true;
              IT.expand_cb = _subitems_get;
              IT.filter_data.demo_types = DEMO_TEXT | DEMO_TEXTBLOCK;
              break;
           case PROPERTY_DEMO_ITEM_TEXT_NAME:
              IT.name = "Name";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_LABEL, ATTRIBUTE_DEMO_ITEM_TEXT_NAME);
              break;
           case PROPERTY_DEMO_ITEM_TEXT_CONTENT:
              IT.name = "Content";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_ENTRY, ATTRIBUTE_DEMO_ITEM_TEXT_CONTENT);
              break;

           case PROPERTY_DEMO_ITEM_SWALLOW_TITLE:
              IT.name = "Swallow";
              IT.expandable = true;
              IT.expanded = true;
              IT.expand_cb = _subitems_get;
              IT.filter_data.demo_types = DEMO_SWALLOW;
              break;
           case PROPERTY_DEMO_ITEM_SWALLOW_NAME:
              IT.name = "Name";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_LABEL, ATTRIBUTE_DEMO_ITEM_SWALLOW_NAME);
              break;
           case PROPERTY_DEMO_ITEM_SWALLOW_CONTENT:
              IT.name = "Content";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_COMBOBOX, ATTRIBUTE_DEMO_ITEM_SWALLOW_CONTENT);
              break;
           case PROPERTY_DEMO_ITEM_SWALLOW_PICTURE:
              IT.name = "Image";
              IT.filter_cb = _filter_swallow_cb;
              IT.filter_data.demo_types = DEMO_SWALLOW_IMAGE;
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_IMAGE_SELECTOR, ATTRIBUTE_DEMO_ITEM_SWALLOW_PICTURE);
              break;
           case PROPERTY_DEMO_ITEM_SWALLOW_WIDGET:
              IT.name = "Widget";
              IT.filter_cb = _filter_swallow_cb;
              IT.filter_data.demo_types = DEMO_SWALLOW_WIDGET;
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_COMBOBOX, ATTRIBUTE_DEMO_ITEM_SWALLOW_WIDGET);
              break;
           case PROPERTY_DEMO_ITEM_SWALLOW_STYLE:
              IT.name = "Widget style";
              IT.filter_cb = _filter_swallow_cb;
              IT.filter_data.demo_types = DEMO_SWALLOW_WIDGET;
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_ENTRY, ATTRIBUTE_DEMO_ITEM_SWALLOW_STYLE);
              break;
           case PROPERTY_DEMO_ITEM_SWALLOW_LAYOUT_GROUP:
              IT.name = "Layout group";
              IT.filter_cb = _filter_swallow_cb;
              IT.filter_data.demo_types = DEMO_SWALLOW_WIDGET;
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_COMBOBOX, ATTRIBUTE_DEMO_ITEM_SWALLOW_LAYOUT_GROUP);
              break;
           case PROPERTY_DEMO_ITEM_SWALLOW_COLOR:
              IT.name = "Color";
              IT.filter_cb = _filter_swallow_cb;
              IT.filter_data.demo_types = ~DEMO_SWALLOW_NONE;
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_COLOR, ATTRIBUTE_DEMO_ITEM_SWALLOW_COLOR);
              break;
           case PROPERTY_DEMO_ITEM_SWALLOW_MIN:
              IT.name = "Min";
              IT.filter_cb = _filter_swallow_cb;
              IT.filter_data.demo_types = ~DEMO_SWALLOW_NONE;
              _action1(&IT, "w:", "px", PROPERTY_CONTROL_SPINNER, ATTRIBUTE_DEMO_ITEM_SWALLOW_MIN_W);
              _action2(&IT, "h:", "px", PROPERTY_CONTROL_SPINNER, ATTRIBUTE_DEMO_ITEM_SWALLOW_MIN_H);
              break;
           case PROPERTY_DEMO_ITEM_SWALLOW_MAX:
              IT.name = "Max";
              IT.filter_cb = _filter_swallow_cb;
              IT.filter_data.demo_types = ~DEMO_SWALLOW_NONE;
              _action1(&IT, "w:", "px", PROPERTY_CONTROL_SPINNER, ATTRIBUTE_DEMO_ITEM_SWALLOW_MAX_W);
              _action2(&IT, "h:", "px", PROPERTY_CONTROL_SPINNER, ATTRIBUTE_DEMO_ITEM_SWALLOW_MAX_H);
              break;

           case PROPERTY_DEMO_ITEM_PROGRAM_TITLE:
              IT.name = "Program";
              IT.expandable = true;
              IT.expanded = true;
              IT.expand_cb = _subitems_get;
              IT.filter_data.demo_types = DEMO_SIGNAL;
              break;
           case PROPERTY_DEMO_ITEM_PROGRAM_SIGNAL:
              IT.name = "Signal";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_LABEL, ATTRIBUTE_DEMO_ITEM_PROGRAM_SIGNAL);
              break;
           case PROPERTY_DEMO_ITEM_PROGRAM_SOURCE:
              IT.name = "Source";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_LABEL, ATTRIBUTE_DEMO_ITEM_PROGRAM_SOURCE);
              break;
           case PROPERTY_DEMO_ITEM_PROGRAM_ACTION:
              IT.name = "Action";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_LABEL, ATTRIBUTE_DEMO_ITEM_PROGRAM_ACTION);
              break;
           case PROPERTY_DEMO_ITEM_PROGRAM_EMIT:
              IT.name = "Emit signal";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_LABEL, ATTRIBUTE_DEMO_ITEM_PROGRAM_EMIT);
              break;
           case PROPERTY_DEMO_ITEM_PROGRAM_EMITTER:
              IT.name = "Emitter";
              _action1(&IT, NULL, NULL, PROPERTY_CONTROL_LABEL, ATTRIBUTE_DEMO_ITEM_PROGRAM_EMITTER);
              break;

           case PROPERTY_DEMO_ITEM_LAST:
              break;
          }
     }
#undef IT
}

static void
_on_demo_part_clicked(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *event_info)
{
   Property_Data *pd = data;
   demo_pd.part = event_info;
   demo_pd.signal = NULL;

   GENLIST_FILTER_APPLY(pd->genlist);

   if (demo_pd.part)
     {
        property_item_update_recursively(&demo_pd.items[PROPERTY_DEMO_ITEM_TEXT_TITLE]);
        property_item_update_recursively(&demo_pd.items[PROPERTY_DEMO_ITEM_SWALLOW_TITLE]);
     }
}
static void
_on_demo_program_clicked(void *data,
                         Evas_Object *obj __UNUSED__,
                         void *event_info)
{
   Property_Data *pd = data;
   demo_pd.part = NULL;
   demo_pd.signal = event_info;

   GENLIST_FILTER_APPLY(pd->genlist);

   if (demo_pd.signal)
     property_item_update_recursively(&demo_pd.items[PROPERTY_DEMO_ITEM_PROGRAM_TITLE]);
}
static void
_on_unclicked(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   Property_Data *pd = data;
   demo_pd.part = NULL;
   demo_pd.signal = NULL;

   GENLIST_FILTER_APPLY(pd->genlist);
}

/* public */
void
property_demo_init(Property_Data *pd)
{
   assert(pd != NULL);

   _init_items();

   /* register global callbacks */
   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_TEXT_PART_CLICKED, _on_demo_part_clicked, pd);
   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_SWALLOW_PART_CLICKED, _on_demo_part_clicked, pd);
   evas_object_smart_callback_add(ap.win, SIGNAL_DEMO_PROGRAM_PART_CLICKED, _on_demo_program_clicked, pd);
   evas_object_smart_callback_add(ap.win, SIGNAL_DIFFERENT_TAB_CLICKED, _on_unclicked, pd);
}

Eina_List *
property_demo_items_get()
{
   Eina_List *items = NULL;

   items = eina_list_append(items, &demo_pd.items[PROPERTY_DEMO_ITEM_TEXT_TITLE]);
   items = eina_list_append(items, &demo_pd.items[PROPERTY_DEMO_ITEM_SWALLOW_TITLE]);
   items = eina_list_append(items, &demo_pd.items[PROPERTY_DEMO_ITEM_PROGRAM_TITLE]);

   return items;
}
