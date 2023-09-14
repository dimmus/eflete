#include "main_window.h"
#include "tabs.h"
#include "workspace.h"
#include "shortcuts.h"
#include "project_manager2.h"

static int _menu_delayed_event = 0;

int MENU_ITEMS_LIST_BASE[] = {
   MENU_FILE_SAVE,
   MENU_FILE_EXPORT_EDC,
   MENU_FILE_EXPORT_EDC_PROJECT,
   MENU_FILE_EXPORT,
   MENU_FILE_EXPORT_DEVELOP,
   MENU_FILE_EXPORT_RELEASE,
   MENU_FILE_CLOSE_PROJECT,
   MENU_WINDOW_MANAGER_IMAGE,
   MENU_WINDOW_MANAGER_SOUND,
   MENU_WINDOW_MANAGER_COLORCLASS,
   MENU_WINDOW_MANAGER_TEXT_STYLE,
   MENU_EDIT_GROUP_ADD,

   MENU_NULL
};

int MENU_ITEMS_LIST_STYLE_ONLY[] = {
   MENU_EDIT_UNDO,
   MENU_EDIT_REDO,
   MENU_VIEW_WORKSPACE_ZOOM_IN,
   MENU_VIEW_WORKSPACE_ZOOM_OUT,
   MENU_VIEW_WORKSPACE_ZOOM_RESET,
   MENU_VIEW_WORKSPACE_FIT,
   MENU_VIEW_WORKSPACE_FILL,
   MENU_VIEW_WORKSPACE_OBJECT_AREA,
   MENU_VIEW_RULERS_SHOW,
   MENU_FILE_EXPORT_EDC_GROUP,
   MENU_EDIT_PART_ADD,
   MENU_EDIT_STATE_ADD,
   MENU_EDIT_ITEM_ADD,
   MENU_EDIT_PROGRAM_ADD,
   MENU_EDIT_DATA_ITEM_ADD,

   MENU_NULL
};

int MENU_ITEMS_LIST_MAIN[] = {
   MENU_FILE,
   MENU_VIEW,
   MENU_EDIT,
   MENU_WINDOW,
   MENU_HELP,

   MENU_NULL
};

static int sad_callback_data[MENU_ITEMS_COUNT];

struct _Menu
{
   Elm_Object_Item *items[MENU_ITEMS_COUNT];
   Evas_Object *window_menu;
};

struct _menu_event
{
   int mid;
};
typedef struct _menu_event Menu_Event;

static Eina_Bool
_menu_cb(void *data EINA_UNUSED,
         int type EINA_UNUSED,
         void *event)
{
   Menu_Event *menu_event = (Menu_Event *)event;

   switch (menu_event->mid)
     {
      case MENU_FILE:
         // TOP LEVEL: nothing to do. Need this to avoid 'unknown menu id' error.
         break;
      case MENU_FILE_NEW_PROJECT:
         tabs_home_tab_add(TAB_HOME_NEW_PROJECT);
         break;
      case MENU_FILE_OPEN_PROJECT:
         tabs_home_tab_add(TAB_HOME_OPEN_PROJECT);
         break;
      case MENU_FILE_IMPORT_EDJ:
         tabs_home_tab_add(TAB_HOME_IMPORT_EDJ);
         break;
      case MENU_FILE_IMPORT_EDC:
         tabs_home_tab_add(TAB_HOME_IMPORT_EDC);
         break;
      case MENU_FILE_SAVE:
         project_save();
         break;
      case MENU_FILE_EXPORT_EDC_GROUP:
         project_export_edc_group();
         break;
      case MENU_FILE_EXPORT_EDC_PROJECT:
         project_export_edc_project(ap.project->groups);
         break;
      case MENU_FILE_EXPORT_DEVELOP:
         project_export_develop();
         break;
      case MENU_FILE_EXPORT_RELEASE:
         project_export_release();
         break;
      case MENU_FILE_CLOSE_PROJECT:
           {
              if (!project_close(NULL, NULL)) break;
           }
         break;
      case MENU_FILE_EXIT:
         ui_main_window_del();
         break;
      case MENU_EDIT:
         // TOP LEVEL: nothing to do. Need this to avoid 'unknown menu id' error.
         break;
      case MENU_EDIT_UNDO:
         shortcuts_shortcut_send(SHORTCUT_TYPE_UNDO);
         break;
      case MENU_EDIT_REDO:
         shortcuts_shortcut_send(SHORTCUT_TYPE_REDO);
         break;
      case MENU_EDIT_GROUP_ADD:
         shortcuts_shortcut_send(SHORTCUT_TYPE_ADD_GROUP);
         break;
      case MENU_EDIT_PART_ADD:
         shortcuts_shortcut_send(SHORTCUT_TYPE_ADD_PART);
         break;
      case MENU_EDIT_STATE_ADD:
         shortcuts_shortcut_send(SHORTCUT_TYPE_ADD_STATE);
         break;
      case MENU_EDIT_ITEM_ADD:
         shortcuts_shortcut_send(SHORTCUT_TYPE_ADD_ITEM);
         break;
      case MENU_EDIT_PROGRAM_ADD:
         shortcuts_shortcut_send(SHORTCUT_TYPE_ADD_PROGRAM);
         break;
      case MENU_EDIT_DATA_ITEM_ADD:
         shortcuts_shortcut_send(SHORTCUT_TYPE_ADD_DATA_ITEM);
         break;
      case MENU_EDIT_PREFERENCE:
         /* preferences_window_add(ap.project); */
         break;
      case MENU_VIEW:
         // TOP LEVEL: nothing to do. Need this to avoid 'unknown menu id' error.
         break;
      case MENU_VIEW_WORKSPACE_ZOOM_IN:
         shortcuts_shortcut_send(SHORTCUT_TYPE_ZOOM_IN);
         break;
      case MENU_VIEW_WORKSPACE_ZOOM_OUT:
         shortcuts_shortcut_send(SHORTCUT_TYPE_ZOOM_OUT);
        break;
      case MENU_VIEW_WORKSPACE_ZOOM_RESET:
         shortcuts_shortcut_send(SHORTCUT_TYPE_ZOOM_RESET);
         break;
      case MENU_VIEW_WORKSPACE_FIT:
         shortcuts_shortcut_send(SHORTCUT_TYPE_FIT);
         break;
      case MENU_VIEW_WORKSPACE_FILL:
         shortcuts_shortcut_send(SHORTCUT_TYPE_FILL);
         break;
      case MENU_VIEW_WORKSPACE_OBJECT_AREA:
         shortcuts_shortcut_send(SHORTCUT_TYPE_OBJECT_AREA);
         break;
      case MENU_VIEW_RULERS_SHOW:
         shortcuts_shortcut_send(SHORTCUT_TYPE_RULERS_SHOW);
         break;
      case MENU_WINDOW:
         // TOP LEVEL: nothing to do. Need this to avoid 'unknown menu id' error.
         break;
      case MENU_WINDOW_TAB_HOME:
         tabs_home_tab_add(TAB_HOME_OPEN_PROJECT);
         break;
      case MENU_WINDOW_MANAGER_IMAGE:
         shortcuts_shortcut_send(SHORTCUT_TYPE_TAB_IMAGE_MANAGER);
         break;
      case MENU_WINDOW_MANAGER_SOUND:
         shortcuts_shortcut_send(SHORTCUT_TYPE_TAB_SOUND_MANAGER);
         break;
      case MENU_WINDOW_MANAGER_COLORCLASS:
         shortcuts_shortcut_send(SHORTCUT_TYPE_TAB_COLOR_CLASS_MANAGER);
         break;
      case MENU_WINDOW_MANAGER_TEXT_STYLE:
         shortcuts_shortcut_send(SHORTCUT_TYPE_TAB_STYLE_MANAGER);
         break;
      case MENU_WINDOW_MANAGER_SCRIPT:
         shortcuts_shortcut_send(SHORTCUT_TYPE_TAB_SCRIPT_MANAGER);
         break;
      case MENU_HELP:
         // TOP LEVEL: nothing to do. Need this to avoid 'unknown menu id' error.
         break;
      case MENU_HELP_ABOUT:
         about_window_add();
         break;
      case MENU_HELP_SHORTCUTS:
         shortcuts_window_add();
         break;
      default:
         ERR("unknown menu id: %d", menu_event->mid);
         break;
     }
   return ECORE_CALLBACK_DONE;
}

static void
_delay_menu_cb(void *data,
               Evas_Object *obj EINA_UNUSED,
               void *event_info EINA_UNUSED)
{
   Menu_Event *menu_event = mem_malloc(sizeof(Menu_Event));

   assert(data != NULL);

   menu_event->mid = * (int *)data;
   ecore_event_add(_menu_delayed_event, menu_event, NULL, NULL);
}

static void
_project_changed(void *data EINA_UNUSED,
                 Evas_Object *obj EINA_UNUSED,
                 void *event_info EINA_UNUSED)
{
   ui_menu_disable_set(ap.menu, MENU_FILE_SAVE, false);
}

Menu *
ui_menu_add(void)
{
   Evas_Object *window_menu;
   Menu *menu;
   int i = 0;
   char buf[PATH_MAX];

   assert(ap.win != NULL);
   assert(ap.win_layout != NULL);

   _menu_delayed_event = ecore_event_type_new();

   ecore_event_handler_add(_menu_delayed_event, _menu_cb, NULL);

   menu = mem_calloc(1, sizeof(Menu));

   /* this array is needed to pass integers as data to callback */
   for (i = 0; i < MENU_ITEMS_COUNT; i++)
     sad_callback_data[i] = i;

   window_menu = elm_win_main_menu_get(ap.win);


#define ITEM_MENU_ADD(PARENT_ID, ID, ICON, LABEL, SHORTCUT) \
   do \
     { \
        menu->items[ID] = elm_menu_item_add(window_menu, menu->items[PARENT_ID], ICON, LABEL, _delay_menu_cb, &sad_callback_data[ID]); \
        if (SHORTCUT) \
          { \
             Evas_Object *item_obj = elm_menu_item_object_get(menu->items[ID]);\
             elm_object_part_text_set(item_obj, "elm.shortcut", SHORTCUT); \
          } \
     } \
   while (0);

#define ___(PARENT_ID) \
   elm_menu_item_separator_add(window_menu, menu->items[PARENT_ID]);

   snprintf(buf, sizeof(buf), "%sicon-save.png", ap.path.image_path);

   ITEM_MENU_ADD(MENU_NULL, MENU_FILE, NULL, _("File"), NULL)
#ifndef HAVE_TIZEN
      ITEM_MENU_ADD(MENU_FILE, MENU_FILE_NEW_PROJECT, "file", _("New project"), NULL)
      ITEM_MENU_ADD(MENU_FILE, MENU_FILE_OPEN_PROJECT, "folder", _("Open project"), "Ctrl-O")
      ITEM_MENU_ADD(MENU_FILE, MENU_FILE_IMPORT_EDJ, NULL, _("Import edj-file"), NULL)
      ITEM_MENU_ADD(MENU_FILE, MENU_FILE_IMPORT_EDC, NULL, _("Import edc-file"), NULL)
      ___(MENU_FILE);
#endif /* if !HAVE_TIZEN */
      ITEM_MENU_ADD(MENU_FILE, MENU_FILE_SAVE, buf, _("Save"), "Ctrl-S")
#ifndef HAVE_TIZEN
      ITEM_MENU_ADD(MENU_FILE, MENU_FILE_EXPORT_EDC, NULL, _("Export as edc"), NULL)
         ITEM_MENU_ADD(MENU_FILE_EXPORT_EDC, MENU_FILE_EXPORT_EDC_GROUP, NULL, _("Group"), NULL)
         ITEM_MENU_ADD(MENU_FILE_EXPORT_EDC, MENU_FILE_EXPORT_EDC_PROJECT, NULL, _("Project"), NULL)
      ITEM_MENU_ADD(MENU_FILE, MENU_FILE_EXPORT, NULL, _("Export as edj"), NULL)
         ITEM_MENU_ADD(MENU_FILE_EXPORT, MENU_FILE_EXPORT_DEVELOP, NULL, _("Develop"), NULL)
         ITEM_MENU_ADD(MENU_FILE_EXPORT, MENU_FILE_EXPORT_RELEASE, NULL, _("Release"), NULL)
      ___(MENU_FILE);
      ITEM_MENU_ADD(MENU_FILE, MENU_FILE_CLOSE_PROJECT, NULL, _("Close project"), NULL)
      ___(MENU_FILE);
#endif /* if !HAVE_TIZEN */
      ITEM_MENU_ADD(MENU_FILE, MENU_FILE_EXIT, NULL, _("Quit"), "Ctrl-Q")

   ITEM_MENU_ADD(MENU_NULL, MENU_EDIT, NULL, _("Edit"), NULL)
      ITEM_MENU_ADD(MENU_EDIT, MENU_EDIT_UNDO, NULL, _("Undo"), "Ctrl-Z")
      ITEM_MENU_ADD(MENU_EDIT, MENU_EDIT_REDO, NULL, _("Redo"), "Ctrl-Y")
      ___(MENU_EDIT);
      ITEM_MENU_ADD(MENU_EDIT, MENU_EDIT_GROUP_ADD, NULL, _("Add group"), "Ctrl-N")
      ___(MENU_EDIT);
      ITEM_MENU_ADD(MENU_EDIT, MENU_EDIT_PART_ADD, NULL, _("Add part"), "q")
      ITEM_MENU_ADD(MENU_EDIT, MENU_EDIT_STATE_ADD, NULL, _("Add state"), "w")
      ITEM_MENU_ADD(MENU_EDIT, MENU_EDIT_ITEM_ADD, NULL, _("Add item"), "e")
      ITEM_MENU_ADD(MENU_EDIT, MENU_EDIT_PROGRAM_ADD, NULL, _("Add program"), "r")
      ITEM_MENU_ADD(MENU_EDIT, MENU_EDIT_DATA_ITEM_ADD, NULL, _("Add data item"), "t")
      /* ITEM_MENU_ADD(MENU_EDIT, MENU_EDIT_PREFERENCE, NULL, _("Preference..."))*/

   ITEM_MENU_ADD(MENU_NULL, MENU_VIEW, NULL, _("View"), NULL)
      ITEM_MENU_ADD(MENU_VIEW, MENU_VIEW_WORKSPACE_ZOOM_IN, NULL, _("Zoom in"), "+")
      ITEM_MENU_ADD(MENU_VIEW, MENU_VIEW_WORKSPACE_ZOOM_OUT, NULL, _("Zoom out"), "-")
      ITEM_MENU_ADD(MENU_VIEW, MENU_VIEW_WORKSPACE_ZOOM_RESET, NULL, _("Reset zoom"), "/")
      ITEM_MENU_ADD(MENU_VIEW, MENU_VIEW_WORKSPACE_FIT, NULL, _("Zoom to fit in Workspace"), NULL)
      ITEM_MENU_ADD(MENU_VIEW, MENU_VIEW_WORKSPACE_FILL, NULL, _("Resize to fill workspace"), NULL)

      ___(MENU_VIEW);
      ITEM_MENU_ADD(MENU_VIEW, MENU_VIEW_WORKSPACE_OBJECT_AREA, NULL, _("Show object area"), "o")
      ___(MENU_VIEW);
      ITEM_MENU_ADD(MENU_VIEW, MENU_VIEW_RULERS_SHOW, NULL, _("Show rulers"), NULL)

   ITEM_MENU_ADD(MENU_NULL, MENU_WINDOW, NULL, _("Window"), NULL)
#ifndef HAVE_TIZEN
      ITEM_MENU_ADD(MENU_WINDOW, MENU_WINDOW_TAB_HOME, NULL, _("Home"), NULL)
      ___(MENU_WINDOW);
#endif /* if !HAVE_TIZEN */
      ITEM_MENU_ADD(MENU_WINDOW, MENU_WINDOW_MANAGER_IMAGE, "image2", _("Image manager"), "F7")
      ITEM_MENU_ADD(MENU_WINDOW, MENU_WINDOW_MANAGER_SOUND, "sound2", _("Sound manager"), "F8")
      ITEM_MENU_ADD(MENU_WINDOW, MENU_WINDOW_MANAGER_TEXT_STYLE, "text2", _("Textblock styles manager"), "F9")
      ITEM_MENU_ADD(MENU_WINDOW, MENU_WINDOW_MANAGER_COLORCLASS, "color", _("Color class manager"), "F10")
      ITEM_MENU_ADD(MENU_WINDOW, MENU_WINDOW_MANAGER_SCRIPT, "code2", _("Script manager"), "F11")

   ITEM_MENU_ADD(MENU_NULL, MENU_HELP, NULL, _("Help"), NULL)
      ITEM_MENU_ADD(MENU_HELP, MENU_HELP_SHORTCUTS, NULL, _("Shortcuts"), "F1")
      ITEM_MENU_ADD(MENU_HELP, MENU_HELP_ABOUT, NULL, _("About"), NULL)

   elm_menu_item_separator_add(window_menu, menu->items[MENU_FILE_IMPORT_EDC]);

   ui_menu_items_list_disable_set(menu, MENU_ITEMS_LIST_BASE, true);
   ui_menu_items_list_disable_set(menu, MENU_ITEMS_LIST_STYLE_ONLY, true);
   ui_menu_disable_set(menu, MENU_WINDOW_MANAGER_SCRIPT, true);

   menu->window_menu = window_menu;

   evas_object_smart_callback_add(ap.win, SIGNAL_PROJECT_CHANGED, _project_changed, NULL);

   return menu;
}

Eina_Bool
ui_menu_disable_set(Menu *menu, int mid, Eina_Bool flag)
{
   assert((mid > MENU_NULL) && (mid < MENU_ITEMS_COUNT));
   /* TODO
    * I will comment this assert because after rework popup from sync we change
    * the popup workflow, and now posible situation when popup is closed but
    * after him we do same job. As example - project is changed, user try close
    * Eflete. Popup about close project is shown with question what to do with
    * unsaved changes. User chose save/unsave and started ecore_exe or file copy
    * with splash. As its async jobs, after popup close main window is marked
    * for del and all data is clear. But when a splash is finished menu treid to
    * enable, but menu is NULL... Because all data is freed.
    *
    * So for now I'm make menu check not so agressive.
    * Realy need to think how to make popups and splash for avoid simular
    * situations.
    * assert(menu != NULL);
    */
   if (menu == NULL) return false;

   elm_object_item_disabled_set(menu->items[mid], flag);

   return true;
}

Eina_Bool
ui_menu_items_list_disable_set(Menu *menu, int *list, Eina_Bool flag)
{
   assert(list != NULL);
   if (menu == NULL) return false;

   Eina_Bool result = true;
   int i = 0;
   while (list[i] != 0)
     {
        result &= ui_menu_disable_set(menu, list[i], flag);
        i++;
     }
   return result;
}
