#include "live_view.h"
#include "notify.h"

#define ELEMENTS_COUNT 16
#define COLOR_BLUE_LIGHT 57, 102, 147, 255
#define COLOR_BLUE_DARK 58, 92, 126, 255

static const char *imgs[] =
{
   "insanely_huge_test_image.jpg",
   "mystrale.jpg",
   "mystrale_2.jpg",
   "twofish.jpg",
   "sky_01.jpg",
   "sky_02.jpg",
   "sky_03.jpg",
   "sky_04.jpg",
};

typedef struct _TestItem
{
   Eina_Stringshare *path;
   int checked;
} TestItem;

static void
_grid_del(void         *data,
          Evas_Object  *obj __UNUSED__)
{
   TestItem *ti = data;
   eina_stringshare_del(ti->path);
   free(data);
}

char *
_grid_text_get(void        *data,
              Evas_Object  *obj __UNUSED__,
              const char   *part __UNUSED__)
{
   const TestItem *ti = data;
   char buf[256];
   snprintf(buf, sizeof(buf), _("Photo %s"), ti->path);
   return strdup(buf);
}

static Evas_Object *
_grid_content_get(void        *data,
                  Evas_Object *obj,
                  const char  *part)
{
   const TestItem *ti = data;
   Evas_Object *content;
   if (!strcmp(part, "elm.swallow.icon"))
     {
        content = elm_bg_add(obj);
        elm_bg_file_set(content, ti->path, NULL);
        evas_object_size_hint_aspect_set(content, EVAS_ASPECT_CONTROL_VERTICAL, 1, 1);
        evas_object_show(content);
        return content;
     }
   else if (!strcmp(part, "elm.swallow.end"))
     {
        content = elm_check_add(obj);
        evas_object_propagate_events_set(content, 0);
        elm_check_state_set(content, ti->checked);
        evas_object_show(content);
        return content;
     }
   return NULL;
}

static Evas_Object *
_create_gengrid(Evas_Object *obj, const char *style)
{
   Elm_Gengrid_Item_Class *ic = NULL;
   TestItem *ti;
   int i;
   Evas_Object *grid = NULL;
   double scale = elm_config_scale_get();
   if (!obj) return NULL;
   grid = elm_gengrid_add(obj);
   elm_gengrid_item_size_set(grid, scale * 100, scale * 100);
   evas_object_size_hint_align_set(grid, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(grid, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

   if (!ic)
     {
        ic = elm_gengrid_item_class_new();
        ic->item_style = strdup(style);
        ic->func.text_get = _grid_text_get;
        ic->func.content_get = _grid_content_get;
        ic->func.state_get = NULL;
        ic->func.del = _grid_del;
     }

   for (i = 0; i < 40; i++)
     {
        ti = mem_calloc(1, sizeof(TestItem));
        ti->path = eina_stringshare_printf(EFLETE_IMG_PATH"%s", imgs[rand() %
                                          (sizeof(imgs) / (sizeof(imgs[0])))]);
        elm_gengrid_item_append(grid, ic, ti, NULL, NULL);
     }
   elm_gengrid_item_class_free(ic);
   return grid;
}

static void
_on_zoom_change(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   Live_View *live = (Live_View *)data;
   live->current_scale = elm_spinner_value_get(obj);
   if (live->object)
      elm_object_scale_set(live->object, live->current_scale);
}

static void
_on_click(void *data,
          Evas_Object *obj __UNUSED__,
          void *event_info __UNUSED__)
{
   Evas_Object *notify = data;
   evas_object_show(notify);
}

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

static void
_notify_orient_get(const char *class, double *horizontal, double *vertical)
{
   *horizontal = 0.5;
   *vertical = 0.0;
   if (strcmp(class, "top") == 0)
     {
        *horizontal = 0.5;
        *vertical = 0.0;
     }
   else if (strcmp(class, "center") == 0)
     {
        *horizontal = 0.5;
        *vertical = 0.5;
     }
   else if (strcmp(class, "bottom") == 0)
     {
        *horizontal = 0.5;
        *vertical = 1.0;
     }
   else if (strcmp(class, "left") == 0)
     {
        *horizontal = 0.0;
        *vertical = 0.5;
     }
   else if (strcmp(class, "right") == 0)
     {
        *horizontal = 1.0;
        *vertical = 0.5;
     }
   else if (strcmp(class, "top_left") == 0)
     {
        *horizontal = 0.0;
        *vertical = 0.0;
     }
   else if (strcmp(class, "top_right") == 0)
     {
        *horizontal = 1.0;
        *vertical = 0.0;
     }
   else if (strcmp(class, "bottom_left") == 0)
     {
        *horizontal = 0.0;
        *vertical = 1.0;
     }
   else if (strcmp(class, "bottom_right") == 0)
     {
        *horizontal = 1.0;
        *vertical = 1.0;
     }
}

static Evas_Object *
_icon_create(const char *image_path, Evas_Object *parent)
{
   Evas_Object *icon;

   icon = elm_icon_add(parent);
   elm_image_file_set(icon, image_path, NULL);
   elm_image_resizable_set(icon, false, false);

   return icon;
}

static Evas_Object *
_elm_widget_create(const char  *widget,
                   const char  *class,
                   const char  *style,
                   Evas_Object *parent)
{
   Evas_Object *object = NULL;

   if (strcmp(widget, "bubble") == 0)
     {
        object = elm_bubble_add(parent);
        if (strcmp(class, "base") != 0)
          elm_bubble_pos_set(object, _bubble_pos_get(class));
        elm_object_part_text_set(object, "info", _("Info"));
        elm_object_text_set(object, _("Text example"));
     }
   else if (strcmp(widget, "button") == 0)
     {
        object = elm_button_add(parent);
        elm_object_text_set(object, _("Text example"));
     }
   else if (strcmp(widget, "check") == 0)
     {
        object = elm_check_add(parent);
        elm_object_text_set(object, _("Text example"));
     }
   else if (strcmp(widget, "panes") == 0)
     {
        Evas *e = evas_object_evas_get(parent);
        Evas_Object *test_right = evas_object_rectangle_add(e);
        Evas_Object *test_left = evas_object_rectangle_add(e);

        evas_object_color_set(test_right,
                              255 * 255 / 255,
                              0,
                              0,
                              255);
        evas_object_color_set(test_left,
                              0,
                              0,
                              255 * 255 / 255,
                              255);

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
        LABEL_ADD(parent, object, _("Some long text for our label, that is"
                                    "long but not too long."));
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
                       elm_object_text_set(bt, _("Both"));
                       elm_table_pack(tb, bt, i, j, 1, 1);
                       evas_object_show(bt);
                    }
               }
             elm_object_content_set(object, tb);
             evas_object_show(tb);
          }
     }
   else  if (strcmp(widget, "spinner") == 0)
     {
        object = elm_spinner_add(parent);
        elm_spinner_label_format_set(object, _("%1.1f units"));
        elm_spinner_step_set(object, 1.5);
        elm_spinner_wrap_set(object, true);
        elm_spinner_min_max_set(object, -50.0, 250.0);
     }
   else  if (strcmp(widget, "actionslider") == 0)
     {
        object = elm_actionslider_add(parent);
        elm_object_part_text_set(object, "left", _("Left"));
        elm_object_part_text_set(object, "right", _("Right"));
        elm_object_part_text_set(object, "center", _("Center"));
        elm_actionslider_magnet_pos_set(object, ELM_ACTIONSLIDER_ALL);
     }
   else  if (strcmp(widget, "calendar") == 0)
     {
        object = elm_calendar_add(parent);
        /* Structure containing a calendar date and time
           broken down into its components (see "time.h").
           {sec, min, hour, day of the month, month, year since 1900,
              days since Sunday, days since January 1, Daylight Saving Time flag} */
        struct tm saturday = {0, 0, 0, 0, 1, 114, 6, -1, 0, 0, NULL};
        elm_calendar_mark_add(object, "checked", &saturday,
                                ELM_CALENDAR_WEEKLY);
     }
   else if (strcmp(widget, "bg") == 0)
     {
        object = elm_bg_add(parent);
     }
   else if (strcmp(widget, "list") == 0)
     {
        int i = 0;
        Evas_Object *start, *end;

        object = elm_list_add(parent);

        for (i = 0; i < (ELEMENTS_COUNT / 4); i++)
             elm_list_item_append(object, _("No icons"), NULL, NULL, NULL, NULL);
        for (i = 0; i < (ELEMENTS_COUNT / 4); i++)
          {
             start = _icon_create(EFLETE_IMG_PATH"mw_button_info.png", parent);
             end = _icon_create(EFLETE_IMG_PATH"mw_button_close.png", parent);
             elm_list_item_append(object, _("Two icons"), start, end, NULL, NULL);
          }
        for (i = 0; i < (ELEMENTS_COUNT / 4); i++)
          {
             end = _icon_create(EFLETE_IMG_PATH"mw_button_close.png", parent);
             elm_list_item_append(object, _("End icon"), NULL, end, NULL, NULL);
          }
        for (i = 0; i < (ELEMENTS_COUNT / 4); i++)
          {
             start = _icon_create(EFLETE_IMG_PATH"mw_button_info.png", parent);
             elm_list_item_append(object, _("Start icon"), start,
                                    NULL, NULL, NULL);
          }

        if (strstr(class, "h_") == class)
           elm_list_horizontal_set(object, true);

        if (strstr(class, "compress"))
           elm_list_mode_set(object, ELM_LIST_COMPRESS);
        else
           elm_list_mode_set(object, ELM_LIST_SCROLL);
     }
   else  if (strcmp(widget, "radio") == 0)
     {
        Evas_Object *rd, *rdg;

        object = elm_box_add(parent);
        evas_object_size_hint_weight_set(object, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

        RADIO_ADD(parent, rd, 1, _("Radio 1 Text Example"));
        elm_box_pack_end(object, rd);
        rdg = rd;
        RADIO_ADD(parent, rd, 2, _("Radio 2 Text Example"));
        elm_radio_group_add(rd, rdg);
        elm_box_pack_end(object, rd);
        RADIO_ADD(parent, rd, 3, _("Radio 3 Text Example"));
        elm_radio_group_add(rd, rdg);
        elm_box_pack_end(object, rd);

        elm_radio_value_set(rdg, 2);
     }
   else if (strcmp(widget, "notify") == 0)
     {
        Evas_Object *content, *noti, *bx, *btn;
        double horizontal, vertical;
        BUTTON_ADD(parent, btn, _("Press to show Notify"));

        noti = elm_notify_add(btn);
        _notify_orient_get(class, &horizontal, &vertical);
        elm_notify_align_set(noti, horizontal, vertical);
        elm_notify_timeout_set(noti, 5);
        BOX_ADD(parent, bx, false, false);
        elm_object_content_set(noti,bx);
        LABEL_ADD(parent, content, _("Text example. Timeout 5 sec"));
        elm_box_pack_end(bx, content);

        evas_object_smart_callback_add(btn, "clicked", _on_click, noti);

        object = elm_box_add(parent);
        elm_box_pack_end(object, noti);
        elm_box_pack_end(object, btn);
     }
   else if (strcmp(widget, "separator") == 0)
     {
        Evas_Object *rect;
        Evas_Object *separator;
        Eina_Bool is_horizontal;

        object = elm_box_add(parent);
        separator = elm_separator_add(parent);

        is_horizontal = !strcmp(class, "horizontal");
        elm_box_horizontal_set(object, !is_horizontal);
        elm_separator_horizontal_set(separator, is_horizontal);

#define _ADD_RECT(COLOR_BLUE) \
        rect = evas_object_rectangle_add(evas_object_evas_get(parent)); \
        evas_object_color_set(rect, COLOR_BLUE); \
        evas_object_size_hint_weight_set(rect, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
        evas_object_size_hint_align_set(rect, EVAS_HINT_FILL, EVAS_HINT_FILL); \
        elm_box_pack_end(object, rect); \
        evas_object_show(rect);

        _ADD_RECT(COLOR_BLUE_LIGHT);
        elm_box_pack_end(object, separator);
        evas_object_show(separator);
        _ADD_RECT(COLOR_BLUE_DARK);

#undef _ADD_RECT
     }
   else  if (strcmp(widget, "clock") == 0)
     {
        object = elm_clock_add(parent);
        unsigned int digit_edit;
        if (strcmp(class, "flipdigit") == 0)
          elm_clock_edit_set(object, true);
  /****************************************************************************
   * Enum  Elm_Clock_Edit_Mode are identifiers for which clock digits should
   * be editable, when a clock widget is in edition mode. For "flipampm"
   * mode variable ELM_CLOCK_EDIT_HOUR_DECIMAL is responsible for user editing
   * time. In this case time can be increased or decreased with step 12 hours.
   ***************************************************************************/
        else if (strcmp(class, "flipampm") == 0)
          {
             elm_clock_show_am_pm_set(object, true);
             elm_clock_edit_set(object, true);
             digit_edit = ELM_CLOCK_EDIT_HOUR_DECIMAL;
             elm_clock_edit_mode_set(object, digit_edit);
          }
        else if (strcmp(class, "base-seconds") == 0)
          elm_clock_show_seconds_set(object, true);
        else if (strcmp(class, "base-am_pm") == 0)
          elm_clock_show_am_pm_set(object, true);
        else if (strcmp(class, "base-all") == 0)
          {
             elm_clock_show_seconds_set(object, true);
             elm_clock_show_am_pm_set(object, true);
          }
   }
   else if (strcmp(widget, "gengrid") == 0)
     {
        if (strcmp(class, "base") == 0)
          {
             object = _create_gengrid(parent, "default");
             evas_object_show(object);
          }
        if (strcmp(class, "item") == 0)
          {
             object = _create_gengrid(parent, style);
             evas_object_show(object);
          }
     }
   return object;
}

static Live_View *
_live_view_init(void)
{
   return mem_calloc(1, sizeof(Live_View));
}

Live_View *
live_view_add(Evas_Object *parent)
{
   Live_View *live;
   Evas_Object *spinner, *_layout;

   live = _live_view_init();

   _layout = elm_layout_add(parent);
   live->layout = _layout;
   elm_layout_file_set(_layout, EFLETE_EDJ, "eflete/live_view/toolbar/default");

   SPINNER_ADD(parent, spinner, 0.01, 5.0, 0.01, true, "eflete/live_view");
   elm_spinner_label_format_set(spinner, "%1.2f");
   evas_object_smart_callback_add(spinner, "changed", _on_zoom_change, live);
   elm_spinner_value_set(spinner, 1.0);
   elm_object_part_content_set(live->layout, "zoom_spinner",
                               spinner);
   live->scale_spinner = spinner;

   live->live_view = elm_layout_add(parent);
   elm_layout_file_set(live->live_view, EFLETE_EDJ, "eflete/live_view/base/default");
   elm_object_part_content_set(live->layout, "live_view",
                               live->live_view);
   elm_layout_signal_emit(live->live_view, "live_view,hide", "eflete");
   elm_layout_signal_emit(live->layout, "live_view,hide", "eflete");

   return live;
}

Eina_Bool
live_view_widget_style_set(Live_View *live, Project *project, Style *style)
{
   if ((!live) || (!project)) return false;
   if (style)
     {
        char **c = NULL;
        const char *widget = NULL, *type = NULL, *style_name = NULL,
                   *custom_name = NULL;
        live->current_scale = 1.0;
        elm_spinner_value_set(live->scale_spinner, 1.0);

        if (style->__type != LAYOUT)
          {
             if (strstr(style->full_group_name, "gengrid"))
               {
                  c = eina_str_split(style->full_group_name, "/", 5);
                  custom_name = c[4];
               }
             else
               c = eina_str_split(style->full_group_name, "/", 4);
             widget = c[1];
             type = c[2];
             style_name = c[3];

             if (!live->object)
               {
                  live->object = _elm_widget_create(widget, type, style_name,
                                                    live->live_view);
                  elm_object_part_content_set(live->live_view, "live_object",
                                              live->object);
                  evas_object_show(live->live_view);
               }

             if (!live->object)
               {
                  NOTIFY_INFO(3, _("Widget isn't implemented yet or isn't exist"));
                  free(c[0]);
                  free(c);
                  return false;
               }

             live_view_theme_update(live, project);
             if ((!strcmp(type, "item")) && (custom_name))
               elm_object_style_set(live->object, custom_name);
             else
               elm_object_style_set(live->object, style_name);
             elm_object_scale_set(live->object, live->current_scale);
             evas_object_show(live->object);

             free(c[0]);
             free(c);
          }
        else
          {
             live->object = elm_layout_add(live->live_view);
             elm_layout_file_set(live->object, project->swapfile,
                                 style->full_group_name);
             elm_object_part_content_set(live->live_view, "live_object",
                                         live->object);
             evas_object_show(live->live_view);
             live_view_theme_update(live, project);
             elm_object_style_set(live->object, style->full_group_name);
             elm_object_scale_set(live->object, live->current_scale);

             evas_object_show(live->object);
          }
     }
   else
     {
        WARN("Edje edit style object was deleted. Could'nt set it into groupspace");
        return false;
     }

   elm_layout_signal_emit(live->live_view, "live_view,show", "eflete");
   elm_layout_signal_emit(live->layout, "live_view,show", "eflete");

   /* updating handlers for fitting block in Live_View
      after opening another widget. */
   const Evas_Object *part_bottom = edje_object_part_object_get(
      elm_layout_edje_get(live->live_view), "bottom_pad");
   const Evas_Object *part_top = edje_object_part_object_get(
      elm_layout_edje_get(live->live_view), "top_pad");

   elm_config_cursor_engine_only_set(false);
   elm_object_cursor_set((Evas_Object *)part_top, "top_left_corner");
   elm_object_cursor_set((Evas_Object *)part_bottom, "bottom_right_corner");
   elm_config_cursor_engine_only_set(true);

   Evas_Coord x, y;
   evas_object_geometry_get(live->live_view, NULL, NULL, &x, &y);
   edje_object_part_drag_value_set(elm_layout_edje_get(live->live_view),
                                   "bottom_pad", x, y);

   return true;
}

Eina_Bool
live_view_widget_style_unset(Live_View *live)
{
   if (!live) return false;
   elm_layout_signal_emit(live->live_view, "live_view,hide", "eflete");
   elm_layout_signal_emit(live->layout, "live_view,hide", "eflete");
   elm_object_part_content_unset(live->live_view, "live");
   evas_object_del(live->object);
   live->object = NULL;
   return true;
}

Eina_Bool
live_view_theme_update(Live_View *live, Project *project)
{
   if ((!live) || (!project) || (!live->object)) return false;
   if ((project->current_style) && (project->current_style->__type == LAYOUT))
     elm_layout_file_set(live->object, project->swapfile,
                           project->current_style->full_group_name);
   Elm_Theme *theme = elm_theme_new();
   elm_theme_set(theme, project->swapfile);
   elm_object_theme_set(live->object, theme);
   elm_object_scale_set(live->object, live->current_scale);
   elm_theme_free(theme);

   return true;
}

void
live_view_free(Live_View *live)
{
   if (live) live_view_widget_style_unset(live);
   else return;

   free(live);
}

#undef ELEMENTS_COUNT
#undef COLOR_BLUE_LIGHT
#undef COLOR_BLUE_DARK
