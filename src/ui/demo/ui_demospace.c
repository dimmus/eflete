#include "ui_demospace.h"
//#include "ui_groupspace.h"
#include "notify.h"

#define ZOOM_ON_CLICK(type, scale) \
   static void \
   _zoom_##type##_on_click(void *data, \
                           Evas_Object *obj __UNUSED__, \
                           void *event_info __UNUSED__) \
   { \
      Demospace *demo = (Demospace *)data; \
      demo->current_scale = scale; \
      elm_object_scale_set(demo->object, demo->current_scale); \
   }
#define ELEMENTS_COUNT 16

ZOOM_ON_CLICK(half, 0.5)
ZOOM_ON_CLICK(one, 1.0)
ZOOM_ON_CLICK(two, 2.0)

/*
static Elm_Bubble_Pos
_bubble_pos_get(const char *class)
{
   Elm_Bubble_Pos position = ELM_BUBBLE_POS_TOP_LEFT;

   if (strcmp(class, "top_left") == 0)
     position = ELM_BUBBLE_POS_TOP_LEFT;
   else if (strcmp(class, "top_right") == 0)
     position = ELM_BUBBLE_POS_TOP_RIGHT;
   else if (strcmp(class, "bottom_left") == 0)
     position = ELM_BUBBLE_POS_BOTTOM_LEFT;
   else if (strcmp(class, "bottom_right") == 0)
     position = ELM_BUBBLE_POS_BOTTOM_RIGHT;

   return position;
}

static Elm_Panel_Orient
_panel_orient_get(const char *class)
{
   Elm_Panel_Orient orient = ELM_PANEL_ORIENT_LEFT;

   if (strcmp(class, "top") == 0)
     orient = ELM_PANEL_ORIENT_TOP;
   else if (strcmp(class, "bottom") == 0)
     orient = ELM_PANEL_ORIENT_BOTTOM;
   else if (strcmp(class, "left") == 0)
     orient = ELM_PANEL_ORIENT_LEFT;
   else if (strcmp(class, "right") == 0)
     orient = ELM_PANEL_ORIENT_RIGHT;

   return orient;
}

static Eina_Bool
_panes_orient_get(const char *class)
{
   Eina_Bool horizontal = true;

   if (strcmp(class, "horizontal") == 0)
     horizontal = true;
   else if (strcmp(class, "vertical") == 0)
     horizontal = false;

   return horizontal;
}
*/

/*
static Evas_Object *
_elm_widget_create(const char *widget, const char *class, Evas_Object *parent)
{
   Evas_Object *object = NULL;

   if (strcmp(widget, "bubble") == 0)
     {
        object = elm_bubble_add(parent);
        if (strcmp(class, "base") != 0)
          elm_bubble_pos_set(object, _bubble_pos_get(class));
        elm_object_part_text_set(object, "info", "Info");
        elm_object_text_set(object, "Text example");
     }
   else if (strcmp(widget, "button") == 0)
     {
        object = elm_button_add(parent);
        elm_object_text_set(object, "Text example");
     }
   else if (strcmp(widget, "check") == 0)
     {
        object = elm_check_add(parent);
        elm_object_text_set(object, "Text example");
     }
   else if (strcmp(widget, "panes") == 0)
     {
        Evas_Object *test_right = elm_button_add(parent);
        Evas_Object *test_left = elm_button_add(parent);
        elm_object_text_set(test_right, "Right side");
        elm_object_text_set(test_left, "Left side");

        object = elm_panes_add(parent);
        elm_panes_horizontal_set(object, _panes_orient_get(class));
        elm_object_part_content_set(object, "right", test_right);
        elm_object_part_content_set(object, "left", test_left);
     }
   else if (strcmp(widget, "panel") == 0)
     {
        object = elm_panel_add(parent);
        elm_panel_orient_set(object, _panel_orient_get(class));
     }
   else if (strcmp(widget, "label") == 0)
     {
        LABEL_ADD(parent, object, "Some long text for our label, that is long but"
                            "not too long.");
        elm_label_slide_duration_set(object, 0.5);
        elm_label_slide_mode_set(object, ELM_LABEL_SLIDE_MODE_ALWAYS);
     }
   else if (strcmp(widget, "scroller") == 0)
     {
        if (strcmp(class, "entry") == 0)
          {
             object = elm_entry_add(parent);
             elm_entry_scrollable_set(object, true);
             elm_scroller_policy_set(object, ELM_SCROLLER_POLICY_ON,
                                            ELM_SCROLLER_POLICY_ON);
          }
        else
          {
             Evas_Object *tb = elm_table_add(parent);
             Evas_Object *bt;
             object = elm_scroller_add(parent);
             int i, j;

             for (j = 0; j < ELEMENTS_COUNT; j++)
               {
                  for (i = 0; i < ELEMENTS_COUNT; i++)
                    {
                       bt = elm_button_add(tb);
                       elm_object_text_set(bt, "Both");
                       elm_table_pack(tb, bt, i, j, 1, 1);
                       evas_object_show(bt);
                    }
               }
             elm_object_content_set(object, tb);
             evas_object_show(tb);
          }
     }

   return object;
}
*/

static Demospace *
_demo_init(void)
{
   return mem_calloc(1, sizeof(Demospace));
}

Demospace *
ui_demospace_add(Evas_Object *parent)
{
   Demospace *demo;
   Evas_Object *_button, *_layout;

   demo = _demo_init();
   demo->current_scale = 1.0;

   _layout = elm_layout_add(parent);
   demo->layout = _layout;
   elm_layout_file_set(_layout, TET_EDJ, "base/demospace");

   _button = elm_button_add(parent);
   elm_object_text_set(_button, "0.5");
   elm_object_style_set(_button, "eflete/default");
   elm_object_part_content_set(_layout, "base/demospace/button_zoom_half",
                               _button);
   evas_object_smart_callback_add(_button, "clicked", _zoom_half_on_click, demo);

   _button = elm_button_add(parent);
   elm_object_text_set(_button, "1.0");
   elm_object_style_set(_button, "eflete/default");
   elm_object_part_content_set(_layout, "base/demospace/button_zoom_one",
                               _button);
   evas_object_smart_callback_add(_button, "clicked", _zoom_one_on_click, demo);

   _button = elm_button_add(parent);
   elm_object_text_set(_button, "2.0");
   elm_object_style_set(_button, "eflete/default");
   elm_object_part_content_set(_layout, "base/demospace/button_zoom_two",
                               _button);
   evas_object_smart_callback_add(_button, "clicked", _zoom_two_on_click, demo);

   //demo->groupspace = ui_groupspace_add(_layout);
   //elm_object_part_content_set(_layout, "base/demospace/groupspace",
   //                            demo->groupspace);
   //elm_layout_signal_emit(demo->groupspace, "groupspace,hide", "");

   return demo;
}

Eina_Bool
ui_demospace_set(Demospace *demo, Project *project, Group *group)
{
   if (group)
     {
        char **c = eina_str_split(group->full_group_name, "/", 4);
        const char /* *widget = c[1],  *type = c[2], */ *style = c[3];

        /*
        if (!demo->object)
          {
             demo->object = _elm_widget_create(widget, type, demo->groupspace);
             elm_object_part_content_set(demo->groupspace, "groupspace", demo->object);
             evas_object_show(demo->groupspace);
          }
          */

        if (!demo->object)
          {
             NOTIFY_INFO(3, "Widget isn't implemented yet or isn't exist");
             free(c[0]);
             free(c);
             return false;
          }

        if (!demo->th)
          demo->th = elm_theme_new();
        else
          elm_theme_flush(demo->th);
        elm_theme_set(demo->th, project->swapfile);
        elm_object_theme_set(demo->object, demo->th);
        elm_object_style_set(demo->object, style);
        elm_object_scale_set(demo->object, demo->current_scale);
        evas_object_show(demo->object);

        free(c[0]);
        free(c);
     }
   else
     {
        WARN("Edje edit group object was deleted. Could'nt set it into groupspace");
        return false;
     }
   //elm_layout_signal_emit (demo->groupspace, "groupspace,show", "");
   return true;
}

Eina_Bool
ui_demospace_unset(Demospace *demo)
{
   if (!demo) return false;
   //elm_layout_signal_emit(demo->groupspace, "groupspace,hide", "");
   //elm_object_part_content_unset(demo->groupspace, "groupspace");
   evas_object_del(demo->object);
   demo->object = NULL;
   return true;
}

Eina_Bool
ui_demospace_update(Demospace *demo)
{
   if (!demo) return false;

   //const Evas_Object *part_bottom = edje_object_part_object_get(
   //   elm_layout_edje_get(demo->groupspace), "bottom_pad");
   //const Evas_Object *part_top = edje_object_part_object_get(
   //   elm_layout_edje_get(demo->groupspace), "top_pad");

   //elm_config_cursor_engine_only_set(false);
   //elm_object_cursor_set((Evas_Object *)part_top, "top_left_corner");
   //elm_object_cursor_set((Evas_Object *)part_bottom, "bottom_right_corner");
   //elm_config_cursor_engine_only_set(true);

   //int x, y, gw, gh, w, h;
   //double dx, dy;
   //evas_object_geometry_get(demo->groupspace, NULL, NULL, &w, &h);
   //evas_object_geometry_get(elm_object_parent_widget_get(demo->groupspace), NULL, NULL, &gw, &gh);
   //x = (int)((w - gw) / 2);
   //y = (int)((h - gh) / 2);
   /*
    *  value 0.01 needed for groupspace offset from top left and bottom right
    *  corners workspace area
    */
   //dx = (double)x / w + 0.01;
   //dy = (double)y / h + 0.01;
   //edje_object_part_drag_value_set(elm_layout_edje_get(demo->groupspace),
   //                                "top_pad", dx, dy);
   //dx = (double)(x + gw) / w - 0.01;
   //dy = (double)(y + gh) / h - 0.01;
   //edje_object_part_drag_value_set(elm_layout_edje_get(demo->groupspace),
   //                                "bottom_pad", dx, dy);
   return true;
}

void
demo_free(Demospace *demo)
{
   if (demo) ui_demospace_unset(demo);
   else return;

   if (demo->th)
     elm_theme_free(demo->th);
   free(demo);
}
