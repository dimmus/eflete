#ifdef HAVE_CONFIG_H
   #include "eflete_config.h"
#endif /* include eflete_config.h */

#include "main_window.h"
#include "history_ui.h"
#include "project_navigator.h"
#include "tabs.h"
#include "shortcuts.h"
#include "cursor.h"
#include "config.h"
#include "property.h"

size_t strlen_safe(const char *str)
{
    return (str != NULL) ? strlen(str) : 0;
}

static void
_project_navigator_group_open(void *data __UNUSED__,
                      Evas_Object *obj __UNUSED__,
                      void *event_info)
{
   tabs_tab_add((Group2 *)event_info);
}

static void
_close_request(void *data __UNUSED__,
               Evas_Object *obj __UNUSED__,
               void *event_info __UNUSED__)
{
   ui_main_window_del();
}

static void
_help(void *data __UNUSED__,
      Evas_Object *obj __UNUSED__,
      void *event_info __UNUSED__)
{
   shortcuts_window_add();
}

static void
_image_manager(void *data __UNUSED__,
               Evas_Object *obj __UNUSED__,
               void *event_info __UNUSED__)
{
   if (ap.project)
     image_manager_add();
}

static void
_sound_manager(void *data __UNUSED__,
               Evas_Object *obj __UNUSED__,
               void *event_info __UNUSED__)
{
   if (ap.project)
     sound_manager_add();
}

static void
_style_manager(void *data __UNUSED__,
               Evas_Object *obj __UNUSED__,
               void *event_info __UNUSED__)
{
   if (ap.project)
     style_manager_add();
}

static void
_color_class_manager(void *data __UNUSED__,
               Evas_Object *obj __UNUSED__,
               void *event_info __UNUSED__)
{
   if (ap.project)
     colorclass_manager_add();
}

static void
_script_manager(void *data __UNUSED__,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   Group2 *group = tabs_current_group_get();

   if (group == NULL) return;

   if (group->current_selected)
     {
        if ((group->current_selected->common.type == RESOURCE2_TYPE_PROGRAM) &&
            (((Program2 *)group->current_selected)->type == EDJE_ACTION_TYPE_SCRIPT))

           script_manager_add(group->current_selected);
     }
   else
      script_manager_add((Resource2 *)group);
}

static void
_after_popup_close(void *data __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *event_info)
{
   Popup_Button pbtn = (Popup_Button) event_info;

   if (BTN_CANCEL == pbtn) return;

#ifdef HAVE_ENVENTOR
   code_edit_mode_switch(false);
#endif

   free(ap.menu);
   ap.menu = NULL;
   cursor_main_free();

   config_save();
   evas_object_del(ap.property.group);
   INFO("%s %s - Finished...", PACKAGE_NAME, VERSION);
   elm_exit();
}

Eina_Bool
ui_main_window_del(void)
{
   ap.exit_in_progress = true;
   if (ap.project)
     if (!project_close(_after_popup_close, NULL))
       return false;

   _after_popup_close(NULL, NULL, (void *)BTN_OK);
   return true;
}

Eina_Bool
ui_main_window_add(void)
{
   Config *config;
   Evas_Object *bg, *project_navigator, *tabs, *icon;
   Eina_Stringshare *icon_path;

   config = config_get();

   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   ap.win = elm_win_add(NULL, "eflete", ELM_WIN_BASIC);

   assert(ap.win != NULL);

   icon = evas_object_image_add(evas_object_evas_get(ap.win));
#ifdef _WIN32
   icon_path = eina_stringshare_printf("%seflete.ico", ap.path.image_path);
#else
   icon_path = eina_stringshare_printf("%seflete.svg", ap.path.image_path);
#endif
   evas_object_image_file_set(icon, icon_path, NULL);
   eina_stringshare_del(icon_path);
   elm_win_icon_object_set(ap.win, icon);
   evas_object_show(icon);

   evas_object_resize(ap.win, config->window.w, config->window.h);
   evas_object_move(ap.win, config->window.x, config->window.y);

#ifdef HAVE_TIZEN
   elm_win_title_set(ap.win, "Component Designer");
#else
   elm_win_title_set(ap.win, "EFL Edje Theme Editor");
#endif

   evas_object_smart_callback_add(ap.win, signals.elm.win.delete_request, _close_request, NULL);
   evas_object_smart_callback_add(ap.win, signals.shortcut.quit, _close_request, NULL);
   evas_object_smart_callback_add(ap.win, signals.shortcut.help, _help, NULL);
   evas_object_smart_callback_add(ap.win, signals.shortcut.manager.image, _image_manager, NULL);
   evas_object_smart_callback_add(ap.win, signals.shortcut.manager.sound, _sound_manager, NULL);
   evas_object_smart_callback_add(ap.win, signals.shortcut.manager.color_class, _color_class_manager, NULL);
   evas_object_smart_callback_add(ap.win, signals.shortcut.manager.style, _style_manager, NULL);
   evas_object_smart_callback_add(ap.win, signals.shortcut.manager.script, _script_manager, NULL);

#if 0 // turn off the eflete main cursor, while not used elementary combobox, and not fixed bug with double cursors
   if (!cursor_main_set(ap.win, CURSOR_ARROW))
     {
        ERR("Main cursor not setted.");
        abort();
     }
#endif /* if 0 */

   elm_object_theme_set(ap.win, ap.theme);

   bg = elm_bg_add(ap.win);
   elm_win_resize_object_add(ap.win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_focus_highlight_enabled_set(ap.win, false);
   evas_object_show(bg);

   ap.win_layout = elm_layout_add(ap.win);
   elm_layout_theme_set(ap.win_layout, "layout", "window", "main");
   evas_object_size_hint_weight_set(ap.win_layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(ap.win, ap.win_layout);
   elm_layout_text_set(ap.win_layout, "eflete.project.time", _("Last saved: none"));
   elm_layout_text_set(ap.win_layout, "eflete.project.part", _("Project path: none"));
   evas_object_show(ap.win_layout);

   /* add panes to main window */
   ap.panes.left = elm_panes_add(ap.win);
   elm_object_part_content_set(ap.win_layout, "eflete.swallow.panes", ap.panes.left);

   ap.panes.right = elm_panes_add(ap.win);
   elm_object_part_content_set(ap.panes.left, "right", ap.panes.right);

   ap.panes.left_ver = elm_panes_add(ap.win);
   elm_panes_horizontal_set(ap.panes.left_ver, true);
   elm_object_part_content_set(ap.panes.left, "left", ap.panes.left_ver);

   /* apply the panes size from config */
   elm_panes_content_left_size_set(ap.panes.left, config->panes.left);
   elm_panes_content_left_size_set(ap.panes.right, config->panes.right);
   elm_panes_content_left_size_set(ap.panes.left_ver, config->panes.left_ver);

   project_navigator = project_navigator_add();
   evas_object_smart_callback_add(project_navigator, SIGNAL_GROUP_OPEN, _project_navigator_group_open, NULL);
   elm_object_part_content_set(ap.panes.left_ver, "left", project_navigator);

   tabs = tabs_add();
   elm_object_part_content_set(ap.panes.right, "left", tabs);

   ap.property.group = property_add(ap.win, PROPERTY_MODE_GROUP);
   elm_object_disabled_set(ap.property.group, true);
   ap.property.image_manager = property_add(ap.win, PROPERTY_MODE_IMAGE);
   ap.property.sound_manager = property_add(ap.win, PROPERTY_MODE_SOUND);
   ap.property.style_manager = property_add(ap.win, PROPERTY_MODE_STYLE);
   ap.property.color_manager = property_add(ap.win, PROPERTY_MODE_COLOR_CLASS);
   ap.property.demo = property_add(ap.win, PROPERTY_MODE_DEMO);
   elm_object_part_content_set(ap.panes.right, "right", ap.property.group);

   if (!shortcuts_init())
     {
        CRIT("Can't initialize the shortcut module");
        return false;
     }

   ap.menu = ui_menu_add();

   #ifdef HAVE_ENVENTOR
     ap.enventor= enventor_object_init(ap.win);
   #endif /* HAVE_ENVENTOR */

   elm_config_window_auto_focus_enable_set(false);

#if HAVE_TIZEN
   elm_config_tooltip_delay_set(0.2);
#endif
    return true;
}

#if !HAVE_TIZEN
Evas_Object *
_about_window_content_get(void *data, Evas_Object *popup, Evas_Object **to_focus __UNUSED__)
{
   Evas_Object *label = (Evas_Object *) data;
   Eina_Strbuf *authors_file_path = NULL;
   Eina_Strbuf *authors = NULL;
   FILE *authors_file  = NULL;

   authors_file_path = eina_strbuf_new();
   eina_strbuf_prepend_printf(authors_file_path, "%s/AUTHORS", elm_app_data_dir_get());

   authors_file = fopen(eina_strbuf_string_get(authors_file_path), "r");

   authors = eina_strbuf_new();
   eina_strbuf_append_printf(authors,
                             "<color=#b6b6b6>"
                             "<b><align=center>"PACKAGE_NAME" v."VERSION" (build time "BUILD_TIME")</align></b><br>"
                             "This application was written for Enlightenment project.<br>"
                             "It is designed to create and modify styles of Elementary widgets.<br>"
                             "<br>"
                             "Copyright (C) 2013 - 2017 Samsung Electronics.<br>"
                             "Copyright (C) 2022 - 2023 Dmitri \"dimmus\" Chudinov.<br>"
                             "<br>"
                             "<align=center><b>Authors:</b><br>");

   char line [BUFF_MAX];
   while (fgets(line, sizeof(line), authors_file ) != NULL)
     eina_strbuf_append_printf(authors, "%s<br>", line);

   eina_strbuf_append_printf(authors, "</align>");

   elm_object_text_set(label, eina_strbuf_string_get(authors));
   elm_object_style_set(label, "slide_about");
   elm_layout_signal_emit(label, "elm,state,slide,start", "elm");
   eina_strbuf_free(authors_file_path);
   eina_strbuf_free(authors);
   fclose(authors_file);

   elm_object_style_set(popup, "shortcuts");

   return label;
}

Evas_Object *
about_window_add(void)
{
   Evas_Object *content = elm_label_add(ap.win);
   popup_add(_("About"), NULL, BTN_CANCEL, _about_window_content_get, content);
   return NULL;
}

#else
Evas_Object *
_about_window_content_get(void *data, Evas_Object *popup __UNUSED__, Evas_Object **to_focus __UNUSED__)
{
  Evas_Object *layout = (Evas_Object *)data;
  elm_layout_theme_set(layout, "layout", "about", "default");
  elm_object_part_text_set(layout, "ver.text", VERSION);
  elm_object_part_text_set(layout, "build.text", BUILD_TIME);
  evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  evas_object_size_hint_align_set(layout, EVAS_HINT_FILL, EVAS_HINT_FILL);
  return layout;
}

Evas_Object *
about_window_add(void)
{
   Evas_Object *content = elm_layout_add(ap.win);
   popup_add(_("About Component Designer"), NULL, BTN_OK, _about_window_content_get, content);
   return NULL;
}

#endif

#if 0

static Evas_Object *
_shortcuts_window_content_get(void *data, Evas_Object *popup __UNUSED__, Evas_Object **to_focus __UNUSED__)
{
   Evas_Object *box = data;
   Evas_Object *scroller = elm_scroller_add(ap.win);
   Evas_Object *label = elm_label_add(scroller);
   elm_object_text_set(label, _(
                       "Global:<br>"
                       "<b>F1</b> - show shortcuts list (this dialog)<br>"
                       "<b>F2</b> - normal mode<br>"
                       "<b>F3</b> - code mode<br>"
                       "<b>F4</b> - demo mode<br>"
                       "<b>F7</b> - open image manager<br>"
                       "<b>F8</b> - open sound manager<br>"
                       "<b>F9</b> - open style manager<br>"
                       "<b>F10</b> - open colorclass manager<br>"
                       "<b>F11</b> - open script manager<br>"
                       "<b>ctrl + q</b> - quit<br>"
                       "<br>"
                       "Workspace:<br>"
                       "<b>ctrl + s</b> - save<br>"
                       "<b>ctrl + z</b> - undo<br>"
                       "<b>ctrl + y</b> - redo<br>"
                       "<b>ctrl + n</b> - add new group<br>"
                       "<b>q</b> - add new part<br>"
                       "<b>w</b> - add new state<br>"
                       "<b>e</b> - add new item<br>"
                       "<b>r</b> - add new program<br>"
                       "<b>t</b> - add new data item<br>"
                       "<b>del</b> - delete selected part/state/item/program/data item<br>"
                       "<b>s</b> - select next state of active part<br>"
                       "<b>z</b> - select prev part<br>"
                       "<b>x</b> - select next part<br>"
                       "<b>ESC</b> - unselect<br>"
                       "<b>o</b> - show/hide object area<br>"
                       "<b>ctrl + wheel_up</b> / <b>KP_ADD</b> - zoom in<br>"
                       "<b>ctrl + wheel_down</b> / <b>KP_SUB</b> - zoom out<br>"
                       "<b>KP_DIV</b> - set zoom to 100%<br>"
                       "<br>"
                       "Tabs:<br>"
                       "<b>ctrl + w</b> - close tab<br>"
                       "<b>ctrl + num</b> - switch to tab 1-10<br>"
                       "<b>tab</b> / <b>ctrl + pg_down</b> - switch to next tab<br>"
                       "<b>shift + tab</b> / <b>ctrl + pg_up</b> - switch to prev tab<br>"
                       "<br>"
                       "Popups:<br>"
                       "<b>Enter</b> - OK<br>"
                       "<b>ESC</b> - cancel<br>"
                       ));

   elm_object_style_set(label, "help");
   elm_object_content_set(scroller, label);
   evas_object_size_hint_weight_set(scroller, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(scroller, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(scroller);
   elm_box_pack_end(box, scroller);
   return box;
}

Evas_Object *
shortcuts_window_add(void)
{
   Evas_Object *content = elm_box_add(ap.win);

   evas_object_size_hint_min_set(content, 0, 300);

   popup_add(_("Help: shortcuts"), NULL, BTN_OK, _shortcuts_window_content_get, content);
   return NULL;
}
#else

typedef struct {
     const char *combination;
     const char *description;
     Shortcut_Type type;
} Shortcut_Data;

static void
_shortcut_change_request(void *data,
                         Evas *e __UNUSED__,
                         Evas_Object *obj,
                         void *event)
{
   Evas_Event_Key_Up *ev = (Evas_Event_Key_Up *)event;
   Shortcut_Data *sc = (Shortcut_Data *)data;
   Eina_Bool result = false;

   result = shortcuts_shortcut_new_set(sc->type, ev);
   if (result)
     {
       elm_layout_signal_emit(obj, "hover,hide", "eflete");
     }
   else
     {
        elm_layout_text_set(obj, "elm.text", _("Sequence already binded."));
        elm_layout_signal_emit(obj, "hover,wrong", "eflete");
     }
   Evas_Object *genlist = elm_layout_content_get(obj, "elm.swallow.content");
   elm_genlist_realized_items_update(genlist);
   evas_object_event_callback_del(obj, EVAS_CALLBACK_KEY_UP, _shortcut_change_request);
   shortcuts_disabled_set(false);
}

static void
_change_shortcut(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Shortcut_Data *sc = (Shortcut_Data *)data;
   Evas_Object *layout =  evas_object_data_get(obj, "layout");

   shortcuts_disabled_set(true);
   elm_layout_signal_emit(layout, "hover,show", "eflete");
   elm_layout_text_set(layout, "elm.text", _("Please press key sequence"));
   evas_object_event_callback_add(layout, EVAS_CALLBACK_KEY_UP, _shortcut_change_request, sc);

}

static void
_reset_shortcut(void *data __UNUSED__, Evas_Object *obj __UNUSED__, void *event_info __UNUSED__)
{
   Popup_Button btn_res = (Popup_Button) event_info;
   if (btn_res == BTN_RESET)
     {
       shortcuts_shortcut_reset();
     }
}

static char *
_label_get(void *data, Evas_Object *obj __UNUSED__, const char *pr __UNUSED__)
{
   if (!pr) return strdup(" ");
   if (!strcmp(pr, "combination.text"))
     {
        Shortcut_Data *sc = (Shortcut_Data *)data;
        const char *text = shortcuts_shortcut_combination_get(sc->type);
        return  text != NULL ? strdup(text) : strdup("NONE");
     }
   else if (!strcmp(pr, "description.text"))
     {
        Shortcut_Data *sc = (Shortcut_Data *)data;
        return strdup(sc->description);
        return  sc->description != NULL ? strdup(sc->description) : strdup("NONE");
     }
   else if (!strcmp(pr, "elm.text"))
     {
        return  data != NULL ? strdup((char *)data) : strdup(" ");
     }
   return strdup(" ");
}

static Evas_Object *
_content_get(void *data __UNUSED__, Evas_Object *obj, const char *pr __UNUSED__)
{
   if (!pr) return NULL;

   if (!strcmp(pr, "change.button"))
     {
        Evas_Object *change_button = elm_button_add(obj);
        Evas_Object *layout = elm_object_parent_widget_get(obj);
        evas_object_data_set(change_button, "layout", layout);
        elm_object_style_set(change_button, "edit");
        evas_object_smart_callback_add(change_button, "clicked", _change_shortcut, data);
        return change_button;
     }
   return NULL;
}

static const Shortcut_Data global_hotkeys[] = {
       {NULL, "Show shortcuts list", SHORTCUT_TYPE_HELP},
       {NULL, "Normal mode",         SHORTCUT_TYPE_MODE_NORMAL},
       {NULL, "Code mode",           SHORTCUT_TYPE_MODE_CODE},
       {NULL, "Demo mode",           SHORTCUT_TYPE_MODE_DEMO},
       {NULL, "Open image manager",  SHORTCUT_TYPE_TAB_IMAGE_MANAGER},
       {NULL, "Open sound manager",  SHORTCUT_TYPE_TAB_SOUND_MANAGER},
       {NULL, "Open style manager",  SHORTCUT_TYPE_TAB_STYLE_MANAGER},
       {NULL, "Open colorclass manager", SHORTCUT_TYPE_TAB_COLOR_CLASS_MANAGER},
       {NULL, "Open script manager", SHORTCUT_TYPE_TAB_SCRIPT_MANAGER},
       {NULL, "Quit",                SHORTCUT_TYPE_QUIT},
       {NULL, NULL,  SHORTCUT_TYPE_NONE}
};
static const Shortcut_Data workspace_hotkeys[] = {
       {NULL, "Save",         SHORTCUT_TYPE_SAVE},
       {NULL, "Undo",         SHORTCUT_TYPE_UNDO},
       {NULL, "Redo",         SHORTCUT_TYPE_REDO},
       {NULL, "Add new group",SHORTCUT_TYPE_ADD_GROUP},
       {NULL, "Add new part", SHORTCUT_TYPE_ADD_PART},
       {NULL, "Add new state",SHORTCUT_TYPE_ADD_STATE},
       {NULL, "Add new item", SHORTCUT_TYPE_ADD_ITEM},
       {NULL, "Add new program", SHORTCUT_TYPE_ADD_PROGRAM},
       {NULL, "Add new data item",SHORTCUT_TYPE_ADD_DATA_ITEM},
       {NULL, "Delete selected part/state/item/etc",SHORTCUT_TYPE_DEL},
       {NULL, "Select next state of active part", SHORTCUT_TYPE_STATE_NEXT},
       {NULL, "Select prev part",SHORTCUT_TYPE_PART_PREV},
       {NULL, "Select next part", SHORTCUT_TYPE_PART_NEXT},
       {NULL, "Unselect", SHORTCUT_TYPE_CANCEL},
       {NULL, "Show/hide object area", SHORTCUT_TYPE_OBJECT_AREA},
       {NULL, "Zoom in", SHORTCUT_TYPE_ZOOM_IN},
       {NULL, "Zoom out", SHORTCUT_TYPE_ZOOM_OUT},
       {NULL, "Set zoom to 100%", SHORTCUT_TYPE_ZOOM_RESET},
       {NULL, NULL,  SHORTCUT_TYPE_NONE}
};

static const Shortcut_Data tabs_hotkeys[] = {
       {NULL,"Close tab",          SHORTCUT_TYPE_TAB_CLOSE},
       {NULL,"Switch to tab 1 ",   SHORTCUT_TYPE_TAB_NUM1 },
       {NULL,"Switch to tab 2 ",   SHORTCUT_TYPE_TAB_NUM2 },
       {NULL,"Switch to tab 3 ",   SHORTCUT_TYPE_TAB_NUM3 },
       {NULL,"Switch to tab 4 ",   SHORTCUT_TYPE_TAB_NUM4 },
       {NULL,"Switch to tab 5 ",   SHORTCUT_TYPE_TAB_NUM5 },
       {NULL,"Switch to tab 6 ",   SHORTCUT_TYPE_TAB_NUM6 },
       {NULL,"Switch to tab 7 ",   SHORTCUT_TYPE_TAB_NUM7 },
       {NULL,"Switch to tab 8 ",   SHORTCUT_TYPE_TAB_NUM8 },
       {NULL,"Switch to tab 9 ",   SHORTCUT_TYPE_TAB_NUM9 },
       {NULL,"Switch to tab 10",   SHORTCUT_TYPE_TAB_NUM10},
       {NULL,"Switch to next tab", SHORTCUT_TYPE_TAB_NEXT},
       {NULL,"Switch to prev tab", SHORTCUT_TYPE_TAB_PREV},
       {NULL, NULL,  SHORTCUT_TYPE_NONE}
};

static const Shortcut_Data popup_hotkeys[] = {
       {NULL,"Ok",          SHORTCUT_TYPE_DONE},
       {NULL,"Cancel",      SHORTCUT_TYPE_CANCEL },
       {NULL, NULL,  SHORTCUT_TYPE_NONE}
};

static Evas_Object *
_shortcuts_window_content_get(void *data, Evas_Object *popup, Evas_Object **to_focus __UNUSED__)
{
   Evas_Object *box = data;
   Evas_Object *genlist = elm_genlist_add(ap.win);
   Elm_Genlist_Item_Class *group_itc = elm_genlist_item_class_new();
   group_itc->item_style = "group_index";
   group_itc->func.text_get = _label_get;

   Elm_Genlist_Item_Class *shortcut_itc = elm_genlist_item_class_new();
   shortcut_itc->item_style = "shortcuts";
   shortcut_itc->func.text_get = _label_get;
   shortcut_itc->func.content_get = _content_get;

   Elm_Genlist_Item_Class *empty_itc = elm_genlist_item_class_new();
   empty_itc->item_style = "empty";

   int i = 0;
   Elm_Object_Item *global_group = elm_genlist_item_append(genlist, group_itc, strdup("Global"),
                                                           NULL, ELM_GENLIST_ITEM_GROUP, NULL, NULL);
   for (i = 0; global_hotkeys[i].type != SHORTCUT_TYPE_NONE; i++)
     {
        elm_genlist_item_append(genlist, shortcut_itc, &global_hotkeys[i],
                                global_group , ELM_GENLIST_ITEM_NONE, NULL, NULL);
     }
   elm_genlist_item_append(genlist, empty_itc, NULL,
                           global_group , ELM_GENLIST_ITEM_NONE, NULL, NULL);

   Elm_Object_Item *workspace_group = elm_genlist_item_append(genlist, group_itc, strdup("Workspace"),
                                                              NULL, ELM_GENLIST_ITEM_GROUP, NULL, NULL);
   for (i = 0; workspace_hotkeys[i].type != SHORTCUT_TYPE_NONE; i++)
     {
        elm_genlist_item_append(genlist, shortcut_itc, &workspace_hotkeys[i],
                                workspace_group, ELM_GENLIST_ITEM_NONE, NULL, NULL);
     }
   elm_genlist_item_append(genlist, empty_itc, NULL,
                           workspace_group , ELM_GENLIST_ITEM_NONE, NULL, NULL);


   Elm_Object_Item *tabs_group = elm_genlist_item_append(genlist, group_itc, strdup("Tabs"),
                                                         NULL, ELM_GENLIST_ITEM_GROUP, NULL, NULL);
   for (i = 0; tabs_hotkeys[i].type != SHORTCUT_TYPE_NONE; i++)
     {
        elm_genlist_item_append(genlist, shortcut_itc, &tabs_hotkeys[i],
                                tabs_group, ELM_GENLIST_ITEM_NONE, NULL, NULL);
     }
   elm_genlist_item_append(genlist, empty_itc, NULL,
                           tabs_group , ELM_GENLIST_ITEM_NONE, NULL, NULL);


   Elm_Object_Item *popup_group = elm_genlist_item_append(genlist, group_itc, strdup("Popup"),
                                                          NULL, ELM_GENLIST_ITEM_GROUP, NULL, NULL);
   for (i = 0; popup_hotkeys[i].type != SHORTCUT_TYPE_NONE; i++)
     {
        elm_genlist_item_append(genlist, shortcut_itc, &popup_hotkeys[i],
                                popup_group, ELM_GENLIST_ITEM_NONE, NULL, NULL);
     }
   elm_genlist_item_append(genlist, empty_itc, NULL,
                           popup_group , ELM_GENLIST_ITEM_NONE, NULL, NULL);

   evas_object_size_hint_weight_set(genlist, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(genlist, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(genlist);

   Evas_Object *layout = elm_layout_add(ap.win);
   elm_layout_theme_set(layout, "layout", "shortcuts", "default");
   elm_layout_content_set(layout, "elm.swallow.content", genlist);
   evas_object_show(layout);
   elm_box_pack_end(box, layout);

   elm_object_style_set(popup, "shortcuts");
   return box;
}

Evas_Object *
shortcuts_window_add(void)
{
   Evas_Object *content = elm_box_add(ap.win);
   Evas_Object *popup = NULL;

   evas_object_size_hint_min_set(content, 400, 460);

   popup = popup_add(_("Help: shortcuts"), NULL,  BTN_OK | BTN_RESET, _shortcuts_window_content_get, content);
   evas_object_smart_callback_add(popup, POPUP_CLOSE_CB, _reset_shortcut, NULL);
   return NULL;
}
#endif
