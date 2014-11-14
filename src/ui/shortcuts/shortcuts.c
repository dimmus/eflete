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
#include "compile_dialog.h"
#include "open_file_dialog.h"
#include "save_file_dialog.h"
#include "style_editor.h"
#include "image_editor.h"
#include "sound_editor.h"
#include "program_editor.h"

struct _Shortcut_Module
{
   Ecore_Event_Handler *shortcuts_handler; /**< handler for catching key presses\
                                                for shortcuts */
   Ecore_Event_Handler *shortcuts_handler_unpress; /**< handler for catching key
                                                        unpressing for
                                                        shortcuts */
   Eina_Hash *shortcut_functions; /**< list of user's shortcuts */
   Eina_List *holded_functions; /**< list of functions that is being held */
};

static void
_random_name_generate(char *part_name, unsigned int length)
{
   unsigned int i = 0;

   static const char CHARS[] =
      "0123456789"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz";

   for (i = 0; i < length; ++i)
     part_name[i] = CHARS[rand() % (sizeof(CHARS) - 1)];

   part_name[length] = 0;
}

/*========================================================*/
/*               SHORTCUTS CB FUNCTION                    */
/*========================================================*/
#define PART_ADD(TYPE, FUNC) \
Eina_Bool \
_##FUNC##_part_add_cb(App_Data *ap) \
{ \
   Evas_Object *workspace = ap->workspace; \
   Evas_Object *widget_list = ui_block_widget_list_get(ap); \
   Style *style = workspace_edit_object_get(workspace); \
   char name[9]; \
   _random_name_generate(name, 9); \
   if (workspace_edit_object_part_add(workspace, name, TYPE, NULL)) \
     { \
       ui_widget_list_part_add(widget_list, style, name); \
       style->isModify = true; \
     } \
   live_view_widget_style_set(ap->live_view, ap->project, style); \
   return true; \
}

/* Adding New Parts */
PART_ADD(EDJE_PART_TYPE_SWALLOW, swallow)
PART_ADD(EDJE_PART_TYPE_TEXTBLOCK, textblock)
PART_ADD(EDJE_PART_TYPE_SPACER, spacer)
PART_ADD(EDJE_PART_TYPE_TEXT, text)
PART_ADD(EDJE_PART_TYPE_RECTANGLE, rectangle)
PART_ADD(EDJE_PART_TYPE_IMAGE, image)

/* this one will delete part or style or layout or state */
Eina_Bool
_item_delete_cb(App_Data *app)
{
   Elm_Object_Item *glit = NULL;
   Style *_style = NULL;
   Evas_Object *nf = NULL;
   Ewe_Tabs_Item *selected = NULL;
   /* if widget list is in focus. Delete part, style or layout. */
   nf = ui_block_widget_list_get(app);
   if ((nf) && (elm_object_focus_get(nf)))
     {
        selected = ewe_tabs_active_item_get(nf);
        /* try to delete part */
        evas_object_smart_callback_call(nf, "wl,part,del", NULL);
        /* try to delete layout */
        if ((evas_object_data_get(nf, "layouts_tab") == selected) &&
            ((!app->project->current_style) ||
             (app->project->current_style->__type != LAYOUT)))
          {
             evas_object_smart_callback_call(app->block.left_top, "wl,layout,del", nf);
          }
        /* try to delete style */
        else if ((evas_object_data_get(nf, "widgets_tab") == selected) &&
            ((!app->project->current_style) ||
             (app->project->current_style->__type != STYLE)))
          {
             nf = evas_object_data_get(nf, "nf_widgets");
             nf = elm_object_item_part_content_get(elm_naviframe_top_item_get(nf),
                                                   "elm.swallow.content");
             glit = elm_genlist_selected_item_get(nf);
             _style = elm_object_item_data_get(glit);
             if (_style->__type != WIDGET)
               evas_object_smart_callback_call(app->block.left_top, "wl,style,del", NULL);
          }
     }

   /* if state list is in focus */
   nf = ui_block_state_list_get(app);
   if ((nf) && (elm_object_focus_get(nf)))
     evas_object_smart_callback_call(nf, "stl,state,del", NULL);

   return true;
}

Eina_Bool
_widget_manager_layout_switch_cb(App_Data *app)
{
   const Eina_List *tabs;
   Evas_Object *nf = ui_block_widget_list_get(app);
   tabs = ewe_tabs_items_list_get(nf);
   tabs = eina_list_next(tabs);
   ewe_tabs_active_item_set(nf, eina_list_data_get(tabs));
   return true;
}

Eina_Bool
_widget_manager_style_switch_cb(App_Data *app)
{
   const Eina_List *tabs;
   Evas_Object *nf = ui_block_widget_list_get(app);
   tabs = ewe_tabs_items_list_get(nf);
   ewe_tabs_active_item_set(nf, eina_list_data_get(tabs));
   return true;
}

Eina_Bool
_separate_mode_change_cb(App_Data *app)
{

   Eina_Bool sep = workspace_separate_mode_get(app->workspace);
   workspace_separate_mode_set(app->workspace, !sep);
   return true;
}

Eina_Bool
_new_style_create_cb(App_Data *app)
{
   Elm_Object_Item *glit = NULL;
   Style *_style = NULL;
   Evas_Object *nf = ui_block_widget_list_get(app);
   Ewe_Tabs_Item *selected = ewe_tabs_active_item_get(nf);

   if (!elm_object_focus_get(nf)) return false;
   if (!app->project) return false;

   if ((evas_object_data_get(nf, "layouts_tab") == selected) &&
       ((!app->project->current_style) ||
        (app->project->current_style->__type != LAYOUT)))
     {
        evas_object_smart_callback_call(app->block.left_top, "wl,layout,add", nf);
     }
   else if ((evas_object_data_get(nf, "widgets_tab") == selected) &&
       ((!app->project->current_style) ||
        (app->project->current_style->__type != STYLE)))
     {
        nf = evas_object_data_get(nf, "nf_widgets");
        nf = elm_object_item_part_content_get(elm_naviframe_top_item_get(nf),
                                              "elm.swallow.content");
        glit = elm_genlist_selected_item_get(nf);
        _style = elm_object_item_data_get(glit);
        if (_style->__type != WIDGET)
          evas_object_smart_callback_call(app->block.left_top, "wl,style,add", NULL);
     }
   return true;
}

Eina_Bool
_style_delete_cb(App_Data *app)
{
   Elm_Object_Item *glit = NULL;
   Style *_style = NULL;
   Evas_Object *nf = ui_block_widget_list_get(app);
   Ewe_Tabs_Item *selected = ewe_tabs_active_item_get(nf);

   if (!elm_object_focus_get(nf)) return false;
   if (!app->project) return false;

   if ((evas_object_data_get(nf, "layouts_tab") == selected) &&
       ((!app->project->current_style) ||
        (app->project->current_style->__type != LAYOUT)))
     {
        evas_object_smart_callback_call(app->block.left_top, "wl,layout,del", nf);
     }
   else if ((evas_object_data_get(nf, "widgets_tab") == selected) &&
       ((!app->project->current_style) ||
        (app->project->current_style->__type != STYLE)))
     {
        nf = evas_object_data_get(nf, "nf_widgets");
        nf = elm_object_item_part_content_get(elm_naviframe_top_item_get(nf),
                                              "elm.swallow.content");
        glit = elm_genlist_selected_item_get(nf);
        _style = elm_object_item_data_get(glit);
        if (_style->__type != WIDGET)
          evas_object_smart_callback_call(app->block.left_top, "wl,style,del", NULL);
     }
   return true;
}

Eina_Bool
_new_theme_cb(App_Data *app)
{
   new_theme_create(app);
   return true;
}

Eina_Bool
_open_edc_cb(App_Data *app)
{
   compile_dialog(app);
   return true;
}

Eina_Bool
_open_edj_cb(App_Data *app)
{
   open_edj_file(app);
   return true;
}

Eina_Bool
_save_cb(App_Data *app)
{
   Evas_Object *nf;
   if (save_edj_file(app))
     {
        nf = ui_block_widget_list_get(app);
        ui_widget_list_title_set(nf, app->project->name);
        STATUSBAR_PROJECT_PATH(app, app->project->edj);
     }
   return true;
}

Eina_Bool
_save_as_cb(App_Data *app)
{
   Evas_Object *nf;
   if (save_as_edj_file(app))
     {
        nf = ui_block_widget_list_get(app);
        ui_widget_list_title_set(nf, app->project->name);
        STATUSBAR_PROJECT_PATH(app, app->project->edj);
     }
   return true;
}

Eina_Bool
_export_cb(App_Data *app)
{
   save_as_edc_file(app);
   return true;
}

Eina_Bool
_visual_tab_cb(App_Data *app)
{
   const Eina_List *tabs;
   Evas_Object *nf;

   nf = ui_block_property_get(app);
   tabs = ewe_tabs_items_list_get(nf);
   ewe_tabs_active_item_set(nf, eina_list_data_get(tabs));

   return true;
}

Eina_Bool
_code_tab_cb(App_Data *app)
{
   const Eina_List *tabs;
   Evas_Object *nf;

   nf = ui_block_property_get(app);
   tabs = ewe_tabs_items_list_get(nf);
   tabs = eina_list_next(tabs);
   ewe_tabs_active_item_set(nf, eina_list_data_get(tabs));

   return true;
}

Eina_Bool
_quit_cb(App_Data *app)
{
   ui_main_window_del(app);
   return true;
}

Eina_Bool
_style_editor_open_cb(App_Data *app)
{
   style_editor_window_add(app->project);
   return true;
}

Eina_Bool
_image_editor_open_cb(App_Data *app)
{
   image_editor_window_add(app->project, MULTIPLE);
   return true;
}

Eina_Bool
_sound_editor_open_cb(App_Data *app)
{
   sound_editor_window_add(app->project, SOUND_EDITOR_SINGLE);
   return true;
}

Eina_Bool
_colorclass_viewer_open_cb(App_Data *app)
{
   colorclass_viewer_add(app->project);
   return true;
}

Eina_Bool
_program_editor_open_cb(App_Data *app)
{
   Style *style = NULL;

   if (app->project)
     style = app->project->current_style;

   program_editor_window_add(style);

   return true;
}

Eina_Bool
_highlight_align_show_switch_cb(App_Data *app)
{
   Eina_Bool flag = workspace_highlight_align_visible_get(app->workspace);
   workspace_highlight_align_visible_set(app->workspace, !flag);
   workspace_object_area_visible_set(app->workspace, !flag);
   return true;
}

Eina_Bool
_object_area_show_switch_cb(App_Data *app)
{
   Eina_Bool flag = workspace_object_area_visible_get(app->workspace);
   workspace_object_area_visible_set(app->workspace, !flag);
   return true;
}

Eina_Bool
_zoom_in_cb(App_Data *app)
{
   double current_factor = workspace_zoom_factor_get(app->workspace);
   workspace_zoom_factor_set(app->workspace, current_factor + 0.1);
   return true;
}

Eina_Bool
_zoom_out_cb(App_Data *app)
{
   double current_factor = workspace_zoom_factor_get(app->workspace);
   workspace_zoom_factor_set(app->workspace, current_factor - 0.1);
   return true;
}

/*========================================================*/
/*                 HELPFULL STRUCTURES                    */
/*========================================================*/

/*
 * Pointer to a function that will be called after clicking on some shortcut.
 */
typedef Eina_Bool (*Shortcut_Function_Cb)(App_Data *app);
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
     {"open_edc", _open_edc_cb},
     {"open_edj", _open_edj_cb},
     {"save", _save_cb},
     {"save_as", _save_as_cb},
     {"export", _export_cb},
     {"property.visual_tab", _visual_tab_cb},
     {"property.code_tab", _code_tab_cb},
     {"part.add.swallow", _swallow_part_add_cb},
     {"part.add.textblock", _textblock_part_add_cb},
     {"part.add.text", _text_part_add_cb},
     {"part.add.rectangle", _rectangle_part_add_cb},
     {"part.add.image", _image_part_add_cb},
     {"part.add.spacer", _spacer_part_add_cb},
     {"item.delete", _item_delete_cb},
     {"separate_mode", _separate_mode_change_cb},
     {"style.create", _new_style_create_cb},
     {"style_editor", _style_editor_open_cb},
     {"image_editor", _image_editor_open_cb},
     {"sound_editor", _sound_editor_open_cb},
     {"colorclass_viewer", _colorclass_viewer_open_cb},
     {"program_editor", _program_editor_open_cb},
     {"widget_manager.layout", _widget_manager_layout_switch_cb},
     {"widget_manager.style", _widget_manager_style_switch_cb},
     {"highlight.align.show", _highlight_align_show_switch_cb},
     {"object_area.show", _object_area_show_switch_cb},
     {"zoom.in", _zoom_in_cb},
     {"zoom.out", _zoom_out_cb},
     {"quit", _quit_cb},
     {NULL, NULL}
};
static Eina_Hash *_sc_functions = NULL;

static Eina_Bool
_key_press_event_cb(void *data, int type __UNUSED__, void *event)
{
   Ecore_Event_Key *ev = (Ecore_Event_Key *)event;
   App_Data *ap = (App_Data *)data;
   Shortcut_Function *sc_func;
   Key_Pair *key = malloc(sizeof(Key_Pair));

   if ((!ap->popup) && (!ap->modal_editor))
     {
        /*
         *  (ev->modifiers && 255) because modifiers contain both locks and modifs,
         *  so if Caps Lock is clicked, then with SHIFT it will return not 1, but 257.
         *  So we need to set a mask for real modifiers (Ctrl, Shift, Alt etc)
         */
        key->modifiers = ev->modifiers & 255;
        key->keycode = ev->keycode;

        sc_func = eina_hash_find(ap->shortcuts->shortcut_functions, key);
        if ((sc_func) && (!sc_func->holdable))
          sc_func->function(ap);
        else if ((sc_func) && (sc_func->holdable) && (!sc_func->held))
          {
             sc_func->held = true;
             ap->shortcuts->holded_functions = eina_list_append(ap->shortcuts->holded_functions, sc_func);
             sc_func->function(ap);
          }
     }

   return ECORE_CALLBACK_PASS_ON;
}

static Eina_Bool
_key_unpress_event_cb(void *data, int type __UNUSED__, void *event)
{
   Ecore_Event_Key *ev = (Ecore_Event_Key *)event;
   App_Data *ap = (App_Data *)data;
   Shortcut_Function *sc_func;
   Eina_List *l;

   if ((!ap->popup) && (!ap->modal_editor))
     {
        EINA_LIST_FOREACH(ap->shortcuts->holded_functions, l, sc_func)
          {
             if (ev->keycode == sc_func->keycode)
               {
                  sc_func->held = false;
                  ap->shortcuts->holded_functions = eina_list_remove(ap->shortcuts->holded_functions, sc_func);
                  sc_func->function(ap);
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

Eina_Bool
shortcuts_main_add(App_Data *ap)
{
   if ((!ap) || (ap->shortcuts->shortcuts_handler))
     return false;

   ap->shortcuts->shortcuts_handler = ecore_event_handler_add(ECORE_EVENT_KEY_DOWN,
                                                   _key_press_event_cb,
                                                   ap);
   ap->shortcuts->shortcuts_handler_unpress = ecore_event_handler_add(ECORE_EVENT_KEY_UP,
                                                                      _key_unpress_event_cb,
                                                                      ap);
   return true;
}

Eina_Bool
shortcuts_main_del(App_Data *ap)
{
   if ((!ap) || (!ap->shortcuts->shortcuts_handler))
     return false;

   ecore_event_handler_del(ap->shortcuts->shortcuts_handler);
   ap->shortcuts->shortcuts_handler = NULL;
   ecore_event_handler_del(ap->shortcuts->shortcuts_handler_unpress);
   ap->shortcuts->shortcuts_handler_unpress = NULL;
   eina_hash_free(ap->shortcuts->shortcut_functions);
   ap->shortcuts->shortcut_functions = NULL;

   return true;
}

Eina_Bool
shortcuts_profile_load(App_Data *ap, Profile *profile)
{
   Shortcuts *sc;
   Shortcut_Function *sc_func;
   Eina_List *l;
   Key_Pair *key;
   if ((!ap) || (!profile) || (!profile->shortcuts) || (!ap->shortcuts))
     return false;

   if (ap->shortcuts->shortcut_functions)
     eina_hash_free(ap->shortcuts->shortcut_functions);
   ap->shortcuts->shortcut_functions = eina_hash_new(EINA_KEY_LENGTH(_eina_int_key_length),
                                                     EINA_KEY_CMP(_eina_int_key_cmp),
                                                     EINA_KEY_HASH(eina_hash_int32),
                                                     _eina_hash_free,
                                                     8);

   EINA_LIST_FOREACH(profile->shortcuts, l, sc)
     {
        key = malloc(sizeof(Key_Pair));
        key->keycode = sc->keycode;
        key->modifiers = sc->modifiers;

        sc_func = malloc(sizeof(Shortcut_Function));
        sc_func->keyname = sc->keyname;
        sc_func->keycode = sc->keycode;
        sc_func->modifiers = sc->modifiers;
        sc_func->holdable = sc->holdable;
        sc_func->held = false;
        sc_func->description = sc->description;
        sc_func->function = eina_hash_find(_sc_functions, sc->description);
        if (eina_hash_find(ap->shortcuts->shortcut_functions, key) ||
            (!eina_hash_direct_add(ap->shortcuts->shortcut_functions, key, sc_func)))
          {
             free(sc_func);
             return false;
          }
     }

   return true;
}

Eina_Bool
shortcuts_init(App_Data *ap)
{
   if ((_sc_functions) || (!ap) || (ap->shortcuts))
     return false;

   ap->shortcuts = mem_calloc(1, sizeof(Shortcut_Module));

   Function_Set *_sc_func_set = _sc_func_set_init;
   _sc_functions = eina_hash_string_superfast_new(NULL);
   while (_sc_func_set->descr)
     {
        eina_hash_direct_add(_sc_functions, _sc_func_set->descr, _sc_func_set->func);
        _sc_func_set++;
     }

   return true;
}

Eina_Bool
shortcuts_shutdown(App_Data *ap)
{
   if ((!_sc_functions) || (!ap) || (!ap->shortcuts))
     return false;

   free(ap->shortcuts);
   ap->shortcuts = NULL;

   eina_hash_free(_sc_functions);
   _sc_functions = NULL;

   return true;
}
