#include "ui_demospace.h"
#include "ui_groupspace.h"
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


ZOOM_ON_CLICK(half, 0.5)
ZOOM_ON_CLICK(one, 1.0)
ZOOM_ON_CLICK(two, 2.0)

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
   Eina_Bool horizontal = EINA_TRUE;

   if (strcmp(class, "horizontal") == 0)
     horizontal = EINA_TRUE;
   else if (strcmp(class, "vertical") == 0)
     horizontal = EINA_FALSE;

   return horizontal;
}

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
        /* is it correct? */
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

   return object;
}

static Demospace *
_demo_init(void)
{
   return mem_calloc(1, sizeof(Demospace));
}

Demospace *
ui_demospace_add(Evas_Object *parent)
{
   Demospace *demo;
   Evas_Object *_bg, *_button, *_layout;
   Evas *canvas;

   demo = _demo_init();
   demo->current_scale = 1.0;

   canvas = evas_object_evas_get(parent);
   demo->canvas = canvas;

   _layout = elm_layout_add(parent);
   demo->layout = _layout;
   elm_layout_file_set(_layout, TET_EDJ, "base/demospace");

   _bg = evas_object_image_filled_add(canvas);
   evas_object_image_filled_set(_bg, EINA_TRUE);
   evas_object_image_file_set(_bg, TET_IMG_PATH"styles-preview-bg-current.png",
                              NULL);
   evas_object_image_border_set(_bg, 4, 4, 4, 4);
   elm_object_part_content_set(_layout, "base/demospace/background", _bg);
   evas_object_show(_bg);

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

   demo->groupspace = ui_groupspace_add(_layout);
   elm_object_part_content_set(_layout, "base/demospace/groupspace",
                               demo->groupspace);
   elm_layout_signal_emit(demo->groupspace, "groupspace,hide", "");

   return demo;
}

void
ui_demospace_set(Demospace *demo, Project *project, Group *group)
{
   if (group)
     {
        /* FIXME: that is really terrible decision :C */
        char **c = eina_str_split(group->full_group_name, "/", 4);
        const char *widget = c[1], *type = c[2], *style = c[3];

        Evas_Object *object = _elm_widget_create(widget, type, demo->groupspace);
        if (!object)
          {
             NOTIFY_INFO(3, "Widget isn't implemented yet or isn't exist");
             return;
          }

        Evas_Object *demo_edit_obj;

        demo_edit_obj = edje_edit_object_add(demo->canvas);
        Eina_Stringshare *path = eina_stringshare_add(TET_SETT_PATH"cache/");
        Eina_Stringshare *file_path = eina_stringshare_add(TET_SETT_PATH"cache/demo.edj");
        if (!ecore_file_exists(path))
          {
             if (!ecore_file_mkdir(path))
               {
                  ERR("Could'nt create cache directory");
                  return;
               }
          }

        ecore_file_cp(project->swapfile, file_path);
        if (!edje_object_file_set(demo_edit_obj, file_path,
                                  group->full_group_name))
          return;

        char *demo_group_name = mem_malloc((strlen(group->full_group_name) + 6)
                                           * sizeof(char));
        char *demo_style_name = mem_malloc((strlen(style) + 6) * sizeof(char));
        strcpy(demo_group_name, group->full_group_name);
        strcpy(demo_style_name, style);
        strncat(demo_group_name, "_demo", 5);
        strncat(demo_style_name, "_demo", 5);

        edje_edit_group_name_set(demo_edit_obj, (const char *)demo_group_name);
        edje_edit_save_all(demo_edit_obj);

        elm_theme_extension_add(NULL, file_path);
        elm_object_style_set(object, demo_style_name);
        elm_object_part_content_set(demo->groupspace, "groupspace", object);
        demo->object = object;
        elm_object_scale_set(demo->object, demo->current_scale);
        evas_object_show(object);
        evas_object_show(demo->groupspace);

        eina_stringshare_del(path);
        eina_stringshare_del(file_path);
        free(demo_group_name);
        free(demo_style_name);
        free(c[0]);
        free(c);
     }
   else
     WARN("Edje edit group object was deleted. Could'nt set it into groupspace");
   elm_layout_signal_emit (demo->groupspace, "groupspace,show", "");
}

void
ui_demospace_unset(Demospace *demo, Project *project)
{
   if ((!project) || (!project->edj) || (!demo)) return;

   Eina_Stringshare *file_path = eina_stringshare_add(TET_SETT_PATH"cache/demo.edj");
   if (!ecore_file_exists(file_path))
     {
        WARN("Demo directory is not exist");
        return;
     }
   elm_theme_extension_del(NULL, file_path);
   elm_layout_signal_emit(demo->groupspace, "groupspace,hide", "");
   elm_object_part_content_unset(demo->groupspace, "groupspace");
   evas_object_del(demo->object);
   eina_stringshare_del(file_path);
}

void
ui_demospace_update(Demospace *demo)
{
   if (!demo) return;

   const Evas_Object *part_bottom = edje_object_part_object_get(
      elm_layout_edje_get(demo->groupspace), "bottom_pad");
   const Evas_Object *part_top = edje_object_part_object_get(
      elm_layout_edje_get(demo->groupspace), "top_pad");

   elm_config_cursor_engine_only_set(EINA_FALSE);
   elm_object_cursor_set((Evas_Object *)part_top, "top_left_corner");
   elm_object_cursor_set((Evas_Object *)part_bottom, "bottom_right_corner");
   elm_config_cursor_engine_only_set(EINA_TRUE);

   int x, y, gw, gh, w, h;
   double dx, dy;
   evas_object_geometry_get(demo->groupspace, NULL, NULL, &w, &h);
   evas_object_geometry_get(elm_object_parent_widget_get(demo->groupspace), NULL, NULL, &gw, &gh);
   x = (int)((w - gw) / 2);
   y = (int)((h - gh) / 2);
   /*
    *  value 0.01 needed for groupspace offset from top left and bottom right
    *  corners workspace area
    */
   dx = (double)x / w + 0.01;
   dy = (double)y / h + 0.01;
   edje_object_part_drag_value_set(elm_layout_edje_get(demo->groupspace),
                                   "top_pad", dx, dy);
   dx = (double)(x + gw) / w - 0.01;
   dy = (double)(y + gh) / h - 0.01;
   edje_object_part_drag_value_set(elm_layout_edje_get(demo->groupspace),
                                   "bottom_pad", dx, dy);
}
