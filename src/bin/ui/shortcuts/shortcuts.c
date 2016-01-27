/*
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
 *
 * This file is part of Edje Theme Editor.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; If not, see www.gnu.org/licenses/lgpl.html.
 */

#include "shortcuts.h"
#include "main_window.h"
#include "style_editor.h"
#include "sound_editor.h"
#include "animator.h"
#include "tabs.h"
#include "workspace.h"
#include "config.h"

#ifdef HAVE_ENVENTOR
   #define SKIP_IN_ENVENTOR_MODE \
      if (ap.enventor_mode) \
        return false;
#else
   #define SKIP_IN_ENVENTOR_MODE
#endif

static inline unsigned int
_keycode_convert(unsigned int keycode)
{
#ifdef __APPLE__
#define KEY(mac, lin) case mac: return lin;
   switch (keycode)
   {
      KEY(26, 10) /* 1 */
      KEY(27, 11)
      KEY(28, 12)
      KEY(29, 13)
      KEY(31, 14)
      KEY(30, 15)
      KEY(34, 16)
      KEY(36, 17)
      KEY(33, 18)
      KEY(37, 19)
      KEY(35, 20) /* - */
      KEY(32, 21) /* = */

      KEY(20, 24) /* q */
      KEY(21, 25)
      KEY(22, 26)
      KEY(23, 27)
      KEY(25, 28)
      KEY(24, 29)
      KEY(40, 30)
      KEY(42, 31)
      KEY(39, 32)
      KEY(43, 33) /* p */
      KEY(41, 34) /* [ */
      KEY(38, 35) /* ] */
      KEY(50, 51) /* \ */

      KEY(8, 38) /* a */
      KEY(9, 39)
      KEY(10, 40)
      KEY(11, 41)
      KEY(13, 42)
      KEY(12, 43)
      KEY(46, 44)
      KEY(48, 45)
      KEY(45, 46)
      KEY(49, 47) /* ; */
      KEY(47, 48) /* ' */

      KEY(14, 52) /* z */
      KEY(15, 53)
      KEY(16, 54)
      KEY(17, 55)
      KEY(19, 56)
      KEY(53, 57)
      KEY(54, 58)
      KEY(51, 59)
      KEY(55, 60)
      KEY(52, 61) /* / */

      KEY(56, 23) /* tab */

      KEY(66, 64) /* Alt_L */

      KEY(131, 113) /* left */
      KEY(132, 114) /* right */
   }
   DBG("notconverted mac keycode: %d", keycode);
#endif
   return keycode;
}

struct _Shortcut_Module
{
   Ecore_Event_Handler *shortcuts_handler; /**< handler for catching key presses\
                                                for shortcuts */
   Ecore_Event_Handler *shortcuts_handler_unpress; /**< handler for catching key
                                                        unpressing for
                                                        shortcuts */
   Eina_Hash *shortcut_functions; /**< list of user's shortcuts */
   Eina_List *holded_functions; /**< list of functions that is being held */
   Eina_List *keys;             /* list of pointer to hash keys to be freed */
};

/*
static void
_random_name_generate(char *part_name, unsigned int length)
{
   unsigned int i = 0;

   static const char CHARS[] =
      "0123456789"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz";

   assert(part_name != NULL);

   for (i = 0; i < length - 1; ++i)
     part_name[i] = CHARS[rand() % (sizeof(CHARS) - 1)];

   part_name[length - 1] = 0;
}
*/
/*========================================================*/
/*               SHORTCUTS CB FUNCTION                    */
/*========================================================*/

#define PART_FUNCTIONALITY(TYPE, DATA) \
   SKIP_IN_ENVENTOR_MODE \
   Evas_Object *workspace = ap.workspace; \
   Evas_Object *widget_list = ui_block_widget_list_get(); \
   Style *style = NULL; \
   assert(style != NULL); \
   char name[9]; \
   _random_name_generate(name, 9); \
   if (workspace_edit_object_part_add(workspace, name, TYPE, DATA)) \
     ui_widget_list_part_add(widget_list, style, name); \
   history_diff_add(style->obj, PART_TARGET, ADD, name); \
   /* live_view_part_add(ap.live_view, ui_widget_list_selected_part_get(widget_list)); */ \
   project_changed(true);

#define PART_ADD(TYPE, FUNC) \
static Eina_Bool \
_##FUNC##_part_add_cb(void) \
{ \
   PART_FUNCTIONALITY(TYPE, NULL) \
   return true; \
}

/* Adding New Parts */
/*
PART_ADD(EDJE_PART_TYPE_SWALLOW, swallow)
PART_ADD(EDJE_PART_TYPE_TEXTBLOCK, textblock)
PART_ADD(EDJE_PART_TYPE_SPACER, spacer)
PART_ADD(EDJE_PART_TYPE_TEXT, text)
PART_ADD(EDJE_PART_TYPE_RECTANGLE, rectangle)
PART_ADD(EDJE_PART_TYPE_PROXY, proxy)
PART_ADD(EDJE_PART_TYPE_GROUP, group)
PART_ADD(EDJE_PART_TYPE_BOX, box)
*/
/* different adding is for image */
/*
static void
_on_image_editor_done(void *data __UNUSED__,
                      Evas_Object *obj __UNUSED__,
                      void *event_info)
{
   char *selected = (char *)event_info;


   if (!selected) return;
   PART_FUNCTIONALITY(EDJE_PART_TYPE_IMAGE, selected);
}

Eina_Bool
_image_part_choose_cb(void)
{
   Evas_Object *img_edit;


   img_edit = image_editor_window_add(ap.project, SINGLE);
   evas_object_smart_callback_add(img_edit, SIG_IMAGE_SELECTED, _on_image_editor_done, NULL);
   return true;
}
*/
TODO("move this code or some of it's part to Connector")
TODO("Change this shortcut function since there are not tabs inb widget_list anymore")
Eina_Bool
_item_delete_cb(void)
{
/*   SKIP_IN_ENVENTOR_MODE
   Elm_Object_Item *glit = NULL;
   Style *_style = NULL;
   Evas_Object *nf = NULL;
   Ewe_Tabs_Item *selected = NULL;

   COMMENT: if widget list is in focus. Delete part, style or layout.
   nf = ui_block_widget_list_get();
   if ((nf) && (elm_object_focus_get(nf)))
     {
        selected = ewe_tabs_active_item_get(nf);

        COMMENT: try to delete layout
        if ((evas_object_data_get(nf, "layouts_tab") == selected) &&
            ((!ap.project->current_style) ||
             (ap.project->current_style->__type != LAYOUT)))
          {
             evas_object_smart_callback_call(ap.block.left_top, "wl,layout,del", nf);
             return true;
          }

        COMMENT: try to delete style
        else if ((evas_object_data_get(nf, "widgets_tab") == selected) &&
            ((!ap.project->current_style) ||
             (ap.project->current_style->__type != STYLE)))
          {
             nf = evas_object_data_get(nf, "nf_widgets");
             nf = elm_object_item_part_content_get(elm_naviframe_top_item_get(nf),
                                                   "elm.swallow.content");
             glit = elm_genlist_selected_item_get(nf);
             if (glit)
               {
                  _style = elm_object_item_data_get(glit);
                  if (_style->__type != WIDGET)
                    {
                       evas_object_smart_callback_call(ap.block.left_top, "wl,style,del", NULL);
                       return true;
                    }
               }
          }
     }

   COMMENT: if state list is in focus
   if (elm_object_focus_get(ap.block.state_list))
     {
        evas_object_smart_callback_call(ap.block.state_list, "stl,state,del", NULL);
        return true;
     }

   COMMENT: try to delete part
   evas_object_smart_callback_call(ui_block_widget_list_get(), "wl,part,del", NULL);
*/
   return true;
}

static Eina_Bool
_separate_mode_change_cb(void)
{
   Evas_Object *workspace = tabs_current_workspace_get();

   SKIP_IN_ENVENTOR_MODE

   if (!workspace) return false;

   double factor = workspace_zoom_factor_get(workspace);
   if (fabs(factor - 1.0) > 0.001)
     return false;

   Eina_Bool sep = workspace_separate_mode_get(workspace);
   workspace_separate_mode_set(workspace, !sep);

   return true;
}

TODO("Please remake this shortcut since there are no TABS anymore")
static Eina_Bool
_new_style_create_cb(void)
{
/*
   SKIP_IN_ENVENTOR_MODE
   Elm_Object_Item *glit = NULL;
   Style *_style = NULL;
   Evas_Object *nf = ui_block_widget_list_get();
   Ewe_Tabs_Item *selected = ewe_tabs_active_item_get(nf);

   if (!elm_object_focus_get(nf)) return false;
   if (!ap.project) return false;

   if ((evas_object_data_get(nf, "layouts_tab") == selected) &&
       ((!ap.project->current_style) ||
        (ap.project->current_style->__type != LAYOUT)))
     {
        evas_object_smart_callback_call(ap.block.left_top, "wl,layout,add", nf);
     }
   else if ((evas_object_data_get(nf, "widgets_tab") == selected) &&
       ((!ap.project->current_style) ||
        (ap.project->current_style->__type != STYLE)))
     {
        nf = evas_object_data_get(nf, "nf_widgets");
        nf = elm_object_item_part_content_get(elm_naviframe_top_item_get(nf),
                                              "elm.swallow.content");
        glit = elm_genlist_selected_item_get(nf);
        if (glit)
          {
             _style = elm_object_item_data_get(glit);
             if (_style->__type != WIDGET)
               evas_object_smart_callback_call(ap.block.left_top, "wl,style,add", NULL);
          }
     }
*/
   return true;
}

TODO("Please remake this shortcut since there are no TABS anymore")
/*
static Eina_Bool
_style_delete_cb(void)
{
   SKIP_IN_ENVENTOR_MODE
   Elm_Object_Item *glit = NULL;
   Style *_style = NULL;
   Evas_Object *nf = ui_block_widget_list_get();
   Ewe_Tabs_Item *selected = ewe_tabs_active_item_get(nf);

   if (!elm_object_focus_get(nf)) return false;
   if (!ap.project) return false;

   if ((evas_object_data_get(nf, "layouts_tab") == selected) &&
       ((!ap.project->current_style) ||
        (ap.project->current_style->__type != LAYOUT)))
     {
        evas_object_smart_callback_call(ap.block.left_top, "wl,layout,del", nf);
     }
   else if ((evas_object_data_get(nf, "widgets_tab") == selected) &&
       ((!ap.project->current_style) ||
        (ap.project->current_style->__type != STYLE)))
     {
        nf = evas_object_data_get(nf, "nf_widgets");
        nf = elm_object_item_part_content_get(elm_naviframe_top_item_get(nf),
                                              "elm.swallow.content");
        glit = elm_genlist_selected_item_get(nf);
        _style = elm_object_item_data_get(glit);
        if (_style->__type != WIDGET)
          evas_object_smart_callback_call(ap.block.left_top, "wl,style,del", NULL);
     }
   return true;
}
*/

static Eina_Bool
_new_theme_cb(void)
{
   if (ap.colorsel)
     evas_object_smart_callback_call(ap.colorsel, "dismissed", NULL);
   tabs_menu_tab_open(TAB_HOME_NEW_PROJECT);
   return true;
}

/*
Eina_Bool
_open_edc_cb(void)
{
   if (!project_close())
     return false;
   compile_dialog();
   return true;
}
*/

static Eina_Bool
_open_project_cb(void)
{
   if (ap.colorsel)
     evas_object_smart_callback_call(ap.colorsel, "dismissed", NULL);
   tabs_menu_tab_open(TAB_HOME_OPEN_PROJECT);
   return true;
}

static Eina_Bool
_import_edj_cb(void)
{
   if (ap.colorsel)
     evas_object_smart_callback_call(ap.colorsel, "dismissed", NULL);
   tabs_menu_tab_open(TAB_HOME_IMPORT_EDJ);
   return true;
}

static Eina_Bool
_save_cb(void)
{
   project_save();
   return true;
}

static Eina_Bool
_save_as_cb(void)
{
   if (!ap.project) return false;
   project_export_develop();
   return true;
}

static Eina_Bool
_export_cb(void)
{
   if (!ap.project) return false;
   project_export_edc_project();
   return true;
}

TODO("Remove? Or modify since we don't have EWE_TABS anymore?")
static Eina_Bool
_visual_tab_cb(void)
{
/*
   const Eina_List *tabs;
   Evas_Object *nf;

   nf = ui_block_property_get();
   tabs = ewe_tabs_items_list_get(nf);
   ewe_tabs_active_item_set(nf, eina_list_data_get(tabs));
*/
   return true;
}

TODO("Remove? Or modify since we don't have EWE_TABS anymore?")
static Eina_Bool
_code_tab_cb(void)
{
/*
   const Eina_List *tabs;
   Evas_Object *nf;

   nf = ui_block_property_get();
   tabs = ewe_tabs_items_list_get(nf);
   tabs = eina_list_next(tabs);
   ewe_tabs_active_item_set(nf, eina_list_data_get(tabs));
*/
   return true;
}

static Eina_Bool
_quit_cb(void)
{
   ui_main_window_del();
   return true;
}
/* close currently opened group tab */
static Eina_Bool
_close_tab_cb(void)
{
   if (!ap.project) return false;
   tabs_current_tab_close();
   return true;
}

static Eina_Bool
_style_editor_open_cb(void)
{
   if (ap.colorsel)
     evas_object_smart_callback_call(ap.colorsel, "dismissed", NULL);
   if (ap.project)
     tabs_menu_tab_open(TAB_STYLE_EDITOR);
   return true;
}

static Eina_Bool
_image_editor_open_cb(void)
{
   if (ap.colorsel)
     evas_object_smart_callback_call(ap.colorsel, "dismissed", NULL);
   if (ap.project)
     tabs_menu_tab_open(TAB_IMAGE_EDITOR);
   return true;
}

static Eina_Bool
_sound_editor_open_cb(void)
{
   if (ap.colorsel)
     evas_object_smart_callback_call(ap.colorsel, "dismissed", NULL);
   if (ap.project)
     tabs_menu_tab_open(TAB_SOUND_EDITOR);
   return true;
}

static Eina_Bool
_colorclass_manager_open_cb(void)
{
   if (ap.colorsel)
     evas_object_smart_callback_call(ap.colorsel, "dismissed", NULL);
   if (ap.project)
     tabs_menu_tab_open(TAB_COLORCLASS_EDITOR);
   return true;
}

static Eina_Bool
_animator_open_cb(void)
{
/*   if ((ap.project) && (tabs_current_group_get()))
     animator_window_add(ap.project);*/
   return true;
}

static Eina_Bool
_highlight_align_show_switch_cb(void)
{
   Evas_Object *workspace = tabs_current_workspace_get();

   if (!workspace) return false;

   SKIP_IN_ENVENTOR_MODE
   Eina_Bool flag = workspace_highlight_align_visible_get(workspace);
   workspace_highlight_align_visible_set(workspace, !flag);
   workspace_object_area_visible_set(workspace, !flag);
   return true;
}

static Eina_Bool
_object_area_show_switch_cb(void)
{
   Evas_Object *workspace = tabs_current_workspace_get();

   SKIP_IN_ENVENTOR_MODE
   if (!workspace) return false;
   Eina_Bool flag = workspace_object_area_visible_get(workspace);
   workspace_object_area_visible_set(workspace, !flag);
   return true;
}

static Eina_Bool
_zoom_in_cb(void)
{
   Evas_Object *workspace = tabs_current_workspace_get();

   SKIP_IN_ENVENTOR_MODE
   if (!workspace) return false;

   double current_factor = workspace_zoom_factor_get(workspace);
   workspace_zoom_factor_set(workspace, current_factor + 0.1);
   return true;
}

static Eina_Bool
_zoom_out_cb(void)
{
   Evas_Object *workspace = tabs_current_workspace_get();

   SKIP_IN_ENVENTOR_MODE
   if (!workspace) return false;

   double current_factor = workspace_zoom_factor_get(workspace);
   workspace_zoom_factor_set(workspace, current_factor - 0.1);
   return true;
}

static Eina_Bool
_undo_cb(void)
{
   SKIP_IN_ENVENTOR_MODE
/*   if ((ap.project) && (ap.project->current_style))
     history_undo(ap.project->current_style->obj, 1);
   return true;*/
   return false;
   TODO("Implement undo shortcut");
}

static Eina_Bool
_redo_cb(void)
{
   SKIP_IN_ENVENTOR_MODE
/*   if ((ap.project) && (ap.project->current_style))
     history_redo(ap.project->current_style->obj, 1);
   return true;*/
   return false;
   TODO("Implement redo shortcut");
}

/*========================================================*/
/*                 HELPFULL STRUCTURES                    */
/*========================================================*/

/*
 * Pointer to a function that will be called after clicking on some shortcut.
 */
typedef Eina_Bool (*Shortcut_Function_Cb)(void);
struct _Shortcut_Function
{
   const char           *keyname;
   unsigned int          keycode;
   unsigned int          modifiers;
   Eina_Bool             holdable;
   Eina_Bool             held;
   const char           *description;
   Shortcut_Function_Cb  function;
};
typedef struct _Shortcut_Function Shortcut_Function;

/*
 * Depending on keycode AND modifiers there could be different behaviour.
 * for example Ctrl + N (modifier = 2 and keycode = 57) is different from
 * Ctrl + Shift + N (modifier = 3 and keycode = 57).
 * So different function should be returned.
 * So this is actually Key to eina_hash_find.
 */
struct _Key_Pair
{
   unsigned int modifiers;
   unsigned int keycode;
};
typedef struct _Key_Pair Key_Pair;

/*
 * Private structure, exists for creating hash of
 * "shortcut description" <=> "shortcut callback function"
 */
struct _Function_Set
{
   const char           *descr;
   Shortcut_Function_Cb  func;
};
typedef struct _Function_Set Function_Set;

/*
 * Set of functions attached to it's definition.
 * When shortcut_init() is being called it generated
 */
static Function_Set _sc_func_set_init[] =
{
     {"new_theme", _new_theme_cb},
//     {"open_edc", _open_edc_cb},
     {"open_edj", _open_project_cb},
     {"import_edj", _import_edj_cb},
     {"save", _save_cb},
     {"save_as", _save_as_cb},
     {"export", _export_cb},
     {"property.visual_tab", _visual_tab_cb},
     {"property.code_tab", _code_tab_cb},
/*     {"part.add.swallow", _swallow_part_add_cb},
     {"part.add.textblock", _textblock_part_add_cb},
     {"part.add.text", _text_part_add_cb},
     {"part.add.rectangle", _rectangle_part_add_cb},
     {"part.add.image", _image_part_choose_cb},
     {"part.add.proxy", _proxy_part_add_cb},
     {"part.add.spacer", _spacer_part_add_cb},
     {"part.add.group", _group_part_add_cb},
     {"part.add.box", _box_part_add_cb},*/
/*   {"item.delete", _item_delete_cb}, this callback works unpredictable because of focus */
     {"separate_mode", _separate_mode_change_cb},
     {"style.create", _new_style_create_cb},
     {"highlight.align.show", _highlight_align_show_switch_cb},
     {"object_area.show", _object_area_show_switch_cb},
     {"zoom.in", _zoom_in_cb},
     {"zoom.out", _zoom_out_cb},
     {"quit", _quit_cb},
     {"close", _close_tab_cb},
     {"undo", _undo_cb},
     {"redo", _redo_cb},
     {"tab.style_editor", _style_editor_open_cb},
     {"tab.image_editor", _image_editor_open_cb},
     {"tab.sound_editor", _sound_editor_open_cb},
     {"tab.colorclass_viewer", _colorclass_manager_open_cb},
     {"animator", _animator_open_cb},
     {NULL, NULL}
};
static Eina_Hash *_sc_functions = NULL;

static Eina_Bool
_key_press_event_cb(void *data __UNUSED__, int type __UNUSED__, void *event)
{
   Ecore_Event_Key *ev = (Ecore_Event_Key *)event;
   Shortcut_Function *sc_func;
   Key_Pair *key = mem_malloc(sizeof(Key_Pair));


   if ((!ap.popup) && (!ap.modal_editor))
     {
        /*
         *  (ev->modifiers && 255) because modifiers contain both locks and modifs,
         *  so if Caps Lock is clicked, then with SHIFT it will return not 1, but 257.
         *  So we need to set a mask for real modifiers (Ctrl, Shift, Alt etc)
         */
        key->modifiers = ev->modifiers & 255;
        key->keycode = _keycode_convert(ev->keycode);

        sc_func = eina_hash_find(ap.shortcuts->shortcut_functions, key);
        if ((sc_func) && (!sc_func->holdable))
          sc_func->function();
        else if ((sc_func) && (sc_func->holdable) && (!sc_func->held))
          {
             sc_func->held = true;
             ap.shortcuts->holded_functions = eina_list_append(ap.shortcuts->holded_functions, sc_func);
             sc_func->function();
          }
     }

   free(key);
   return ECORE_CALLBACK_PASS_ON;
}

static Eina_Bool
_key_unpress_event_cb(void *data __UNUSED__, int type __UNUSED__, void *event)
{
   Ecore_Event_Key *ev = (Ecore_Event_Key *)event;
   Shortcut_Function *sc_func;
   Eina_List *l;


   if ((!ap.popup) && (!ap.modal_editor))
     {
        EINA_LIST_FOREACH(ap.shortcuts->holded_functions, l, sc_func)
          {
             if (ev->keycode == sc_func->keycode)
               {
                  sc_func->held = false;
                  ap.shortcuts->holded_functions = eina_list_remove(ap.shortcuts->holded_functions, sc_func);
                  sc_func->function();
               }
          }
     }

   return ECORE_CALLBACK_PASS_ON;
}

/* key_length for custom hash */
static unsigned int
_eina_int_key_length(const void *key __UNUSED__)
{
   return 0;
}
/* key_cmp for custom hash */
static int
_eina_int_key_cmp(const void *key1, int key1_length __UNUSED__,
                  const void *key2, int key2_length __UNUSED__)
{
   Key_Pair *keys1 = (Key_Pair *)key1;
   Key_Pair *keys2 = (Key_Pair *)key2;

   unsigned int key_cmp = keys1->keycode - keys2->keycode;

   if (key_cmp == 0)
      return keys1->modifiers - keys2->modifiers;

   return key_cmp;
}
/* key_free for custom hash */
static void
_eina_hash_free(void *data)
{
   Shortcut_Function *sc_func = data;
   if (!sc_func) return;
   free(sc_func);
}
/*=============================================*/
/*               PUBLIC API                    */
/*=============================================*/

#undef PART_ADD
#undef PART_FUNCTIONALITY

static Eina_Bool
_shortcuts_main_add(void)
{

   if (ap.shortcuts->shortcuts_handler)
     return false;

   ap.shortcuts->shortcuts_handler = ecore_event_handler_add(ECORE_EVENT_KEY_DOWN,
                                                   _key_press_event_cb,
                                                   NULL);
   ap.shortcuts->shortcuts_handler_unpress = ecore_event_handler_add(ECORE_EVENT_KEY_UP,
                                                                      _key_unpress_event_cb,
                                                                      NULL);
   return true;
}

static Eina_Bool
_shortcuts_main_del(void)
{

   assert(ap.shortcuts->shortcuts_handler != NULL);

   ecore_event_handler_del(ap.shortcuts->shortcuts_handler);
   ap.shortcuts->shortcuts_handler = NULL;
   ecore_event_handler_del(ap.shortcuts->shortcuts_handler_unpress);
   ap.shortcuts->shortcuts_handler_unpress = NULL;
   eina_hash_free(ap.shortcuts->shortcut_functions);
   ap.shortcuts->shortcut_functions = NULL;

   return true;
}

Eina_Bool
shortcuts_profile_load(Profile *profile)
{
   Shortcuts *sc;
   Shortcut_Function *sc_func;
   Eina_List *l;
   Key_Pair *key;

   assert(profile != NULL);
   assert(profile->shortcuts != NULL);
   assert(ap.shortcuts != NULL);

   if (ap.shortcuts->shortcut_functions)
     eina_hash_free(ap.shortcuts->shortcut_functions);
   ap.shortcuts->shortcut_functions = eina_hash_new(EINA_KEY_LENGTH(_eina_int_key_length),
                                                     EINA_KEY_CMP(_eina_int_key_cmp),
                                                     EINA_KEY_HASH(eina_hash_int32),
                                                     _eina_hash_free,
                                                     8);

   EINA_LIST_FOREACH(profile->shortcuts, l, sc)
     {
        key = mem_malloc(sizeof(Key_Pair));
        key->keycode = sc->keycode;
        key->modifiers = sc->modifiers;

        sc_func = mem_calloc(1, sizeof(Shortcut_Function));
        sc_func->keyname = sc->keyname;
        sc_func->keycode = sc->keycode;
        sc_func->modifiers = sc->modifiers;
        sc_func->holdable = sc->holdable;
        sc_func->held = false;
        sc_func->description = sc->description;
        sc_func->function = eina_hash_find(_sc_functions, sc->description);
        if (!sc_func->function)
          {
             free(sc_func);
             free(key);
             continue;
          }
        if (eina_hash_find(ap.shortcuts->shortcut_functions, key) ||
            (!eina_hash_direct_add(ap.shortcuts->shortcut_functions, key, sc_func)))
          {
             free(sc_func);
             free(key);
             return false;
          }
        ap.shortcuts->keys = eina_list_append(ap.shortcuts->keys, key);
     }

   return true;
}

Eina_Bool
shortcuts_init(void)
{
   assert(_sc_functions == NULL);
   assert(ap.shortcuts == NULL);

   ap.shortcuts = mem_calloc(1, sizeof(Shortcut_Module));

   Function_Set *_sc_func_set = _sc_func_set_init;
   _sc_functions = eina_hash_string_superfast_new(NULL);
   while (_sc_func_set->descr)
     {
        eina_hash_direct_add(_sc_functions, _sc_func_set->descr, _sc_func_set->func);
        _sc_func_set++;
     }
   _shortcuts_main_add();

   return true;
}

Eina_Bool
shortcuts_shutdown(void)
{
   assert(_sc_functions != NULL);
   assert(ap.shortcuts != NULL);

   Key_Pair *key;

   _shortcuts_main_del();

   eina_hash_free(_sc_functions);
   _sc_functions = NULL;

   EINA_LIST_FREE(ap.shortcuts->keys, key)
     free(key);

   free(ap.shortcuts);
   ap.shortcuts = NULL;

   return true;
}
