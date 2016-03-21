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

/*========================================================*/
/*               SHORTCUTS CB FUNCTION                    */
/*========================================================*/

static Eina_Bool
_save_cb(void)
{
   if (!ap.project) return false;
   project_save();
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
_object_area_show_switch_cb(void)
{
   Evas_Object *workspace = tabs_current_workspace_get();
   Eina_Bool flag;

   SKIP_IN_ENVENTOR_MODE
   if (!workspace) return false;

   flag = workspace_object_area_visible_get(workspace);
   workspace_object_area_visible_set(workspace, !flag);

   flag = workspace_highlight_align_visible_get(workspace);
   workspace_highlight_align_visible_set(workspace, !flag);
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
   Evas_Object *workspace = tabs_current_workspace_get();

   SKIP_IN_ENVENTOR_MODE
   if (!workspace) return false;

   evas_object_smart_callback_call(ap.win, SIGNAL_SHORTCUT_UNDO, NULL);
   return true;
}

static Eina_Bool
_redo_cb(void)
{
   Evas_Object *workspace = tabs_current_workspace_get();

   SKIP_IN_ENVENTOR_MODE
   if (!workspace) return false;

   evas_object_smart_callback_call(ap.win, SIGNAL_SHORTCUT_REDO, NULL);
   return true;
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
     {"save", _save_cb},
     {"zoom.in", _zoom_in_cb},
     {"zoom.out", _zoom_out_cb},
     {"quit", _quit_cb},
     {"close", _close_tab_cb},
     {"undo", _undo_cb},
     {"redo", _redo_cb},
     {"object_area.show", _object_area_show_switch_cb},
     {NULL, NULL}
};
static Eina_Hash *_sc_functions = NULL;

static Eina_Bool
_key_press_event_cb(void *data __UNUSED__, int type __UNUSED__, void *event)
{
   Ecore_Event_Key *ev = (Ecore_Event_Key *)event;
   Shortcut_Function *sc_func;
   Key_Pair *key = mem_malloc(sizeof(Key_Pair));


   if (!ap.popup)
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


   if (!ap.popup)
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
