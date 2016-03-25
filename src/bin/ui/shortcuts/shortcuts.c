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
#undef KEY
#endif
   return keycode;
}

struct _Shortcut
{
   unsigned int          keycode;
   unsigned int          modifiers;
   Shortcut_Type         type_press;
   Shortcut_Type         type_unpress;
};
typedef struct _Shortcut Shortcut;

struct _Shortcut_Module
{
   Ecore_Event_Handler *shortcuts_handler; /**< handler for catching key presses\
                                                for shortcuts */
   Ecore_Event_Handler *shortcuts_handler_unpress; /**< handler for catching key
                                                        unpressing for
                                                        shortcuts */
   Eina_List *shortcuts;      /**< list of user's shortcuts */
   Eina_List *held_shortcuts; /**< list of functions that is being held */
   unsigned int last_modifiers;
};

static int
_shortcut_cmp(Shortcut *s1, Shortcut *s2)
{
   if (s1->modifiers == s2->modifiers)
     return s1->keycode - s2->keycode;
   return s1->modifiers - s2->modifiers;
}

static void
_shortcut_handle(Shortcut_Type type)
{
   int num;
#define SHORTCUT(NAME) \
 case SHORTCUT_TYPE_##NAME: \
    evas_object_smart_callback_call(ap.win, SIGNAL_SHORTCUT_##NAME, NULL); \
    break;

#define SHORTCUT_NUM(NAME, SIGNAL, NUM) \
 case SHORTCUT_TYPE_##NAME: \
    num = NUM; \
    evas_object_smart_callback_call(ap.win, SIGNAL, &num); \
    break;

   switch (type)
     {
        SHORTCUT(REDO);
        SHORTCUT(UNDO);
        SHORTCUT(SAVE);
        SHORTCUT(ADD_GROUP);
        SHORTCUT(ADD_PART);
        SHORTCUT(ADD_STATE);
        SHORTCUT(ADD_ITEM);
        SHORTCUT(ADD_PROGRAM);
        SHORTCUT(DEL);
        SHORTCUT_NUM(TAB_NUM1, SIGNAL_SHORTCUT_TAB_NUM, 1);
        SHORTCUT_NUM(TAB_NUM2, SIGNAL_SHORTCUT_TAB_NUM, 2);
        SHORTCUT_NUM(TAB_NUM3, SIGNAL_SHORTCUT_TAB_NUM, 3);
        SHORTCUT_NUM(TAB_NUM4, SIGNAL_SHORTCUT_TAB_NUM, 4);
        SHORTCUT_NUM(TAB_NUM5, SIGNAL_SHORTCUT_TAB_NUM, 5);
        SHORTCUT_NUM(TAB_NUM6, SIGNAL_SHORTCUT_TAB_NUM, 6);
        SHORTCUT_NUM(TAB_NUM7, SIGNAL_SHORTCUT_TAB_NUM, 7);
        SHORTCUT_NUM(TAB_NUM8, SIGNAL_SHORTCUT_TAB_NUM, 8);
        SHORTCUT_NUM(TAB_NUM9, SIGNAL_SHORTCUT_TAB_NUM, 9);
        SHORTCUT_NUM(TAB_NUM10, SIGNAL_SHORTCUT_TAB_NUM, 10);
        SHORTCUT(TAB_NEXT);
        SHORTCUT(TAB_PREV);
        SHORTCUT(TAB_CLOSE);
        SHORTCUT(TAB_IMAGE_MANAGER);
        SHORTCUT(TAB_SOUND_MANAGER);
        SHORTCUT(TAB_COLOR_CLASS_MANAGER);
        SHORTCUT(TAB_STYLE_MANAGER);

      case SHORTCUT_TYPE_NONE:
         break;
      case SHORTCUT_TYPE_LAST:
         CRIT("Incorrect shortcut type");
         abort();
     }
}

static Eina_Bool
_key_press_event_cb(void *data __UNUSED__, int type __UNUSED__, void *event)
{
   Ecore_Event_Key *ev = (Ecore_Event_Key *)event;
   Shortcut sc, *shortcut;

   /*
    *  (ev->modifiers && 255) because modifiers contain both locks and modifs,
    *  so if Caps Lock is clicked, then with MOD_SHIFT it will return not 1, but 257.
    *  So we need to set a mask for real modifiers (Ctrl, Shift, Alt etc)
    */
   sc.modifiers = ev->modifiers & 255;
   sc.keycode = _keycode_convert(ev->keycode);
   switch(sc.keycode)
     {
      case 37: /*MOD_CTRL*/
      case 105: /*MOD_CTRL*/
         sc.modifiers |= MOD_CTRL;
         break;
      case 64: /*MOD_ALT*/
      case 108: /*MOD_ALT*/
         sc.modifiers |= MOD_ALT;
         break;
      case 50: /*MOD_SHIFT*/
      case 62: /*MOD_SHIFT*/
         sc.modifiers |= MOD_SHIFT;
         break;
      case 133: /*MOD_SUPER*/
      case 134: /*MOD_SUPER*/
         sc.modifiers |= MOD_SUPER;
         break;
      default:
         break;
     }

   /* ignore hotkey if */
   if (/* it is without modifier or with shift-only */
      ((sc.modifiers == MOD_NONE) || (sc.modifiers == MOD_SHIFT)) &&
      /* is not F1 - F12 */
      (!(((sc.keycode >= 67 /*F1*/) &&
          (sc.keycode <= 76 /*F10*/)) ||
         (sc.keycode == 95 /*F11*/) ||
         (sc.keycode == 96 /*F12*/)) ) &&
      /* elm_entry is in focus */
      (!strcmp("elm_entry", evas_object_type_get(elm_object_focused_object_get(ap.win)))))
     {
        DBG("entry focused, ignoring hotkeys with MOD_SHIFT or MODE_NONE");
        return ECORE_CALLBACK_PASS_ON;
     }

   DBG("key_down: %s %d, mod: %d", ev->key, ev->keycode, ev->modifiers & 255);

   ap.shortcuts->last_modifiers = sc.modifiers;

   if (!ap.popup)
     {
        /* check if shortcut already pressed */
        shortcut = eina_list_search_sorted(ap.shortcuts->held_shortcuts, (Eina_Compare_Cb)_shortcut_cmp, &sc);
        if (shortcut)
          return ECORE_CALLBACK_PASS_ON;

        shortcut = eina_list_search_sorted(ap.shortcuts->shortcuts, (Eina_Compare_Cb)_shortcut_cmp, &sc);
        if (shortcut)
          {
             _shortcut_handle(shortcut->type_press);
             ap.shortcuts->held_shortcuts = eina_list_sorted_insert(ap.shortcuts->held_shortcuts,
                                                                    (Eina_Compare_Cb)_shortcut_cmp, shortcut);
             if (shortcut->type_press != SHORTCUT_TYPE_NONE)
               return ECORE_CALLBACK_DONE;
          }
     }

   return ECORE_CALLBACK_PASS_ON;
}

static Eina_Bool
_key_unpress_event_cb(void *data __UNUSED__, int type __UNUSED__, void *event)
{
   Ecore_Event_Key *ev = (Ecore_Event_Key *)event;
   Shortcut sc, *shortcut;
   unsigned int unpressed_modifiers;
   Eina_List *l, *l_n;

   /*
    *  (ev->modifiers && 255) because modifiers contain both locks and modifs,
    *  so if Caps Lock is clicked, then with SHIFT it will return not 1, but 257.
    *  So we need to set a mask for real modifiers (Ctrl, Shift, Alt etc)
    */
   sc.modifiers = ev->modifiers & 255;
   sc.keycode = _keycode_convert(ev->keycode);
   switch(sc.keycode)
     {
      case 37: /*MOD_CTRL*/
      case 105: /*MOD_CTRL*/
         sc.modifiers &= ~MOD_CTRL;
         break;
      case 64: /*MOD_ALT*/
      case 108: /*MOD_ALT*/
         sc.modifiers &= ~MOD_ALT;
         break;
      case 50: /*MOD_SHIFT*/
      case 62: /*MOD_SHIFT*/
         sc.modifiers &= ~MOD_SHIFT;
         break;
      case 133: /*MOD_SUPER*/
      case 134: /*MOD_SUPER*/
         sc.modifiers &= ~MOD_SUPER;
         break;
      default:
         break;
     }
   DBG("key_up  : %s %d, mod: %d", ev->key, ev->keycode, ev->modifiers & 255);

   unpressed_modifiers = ap.shortcuts->last_modifiers & (~sc.modifiers);
   EINA_LIST_FOREACH_SAFE(ap.shortcuts->held_shortcuts, l, l_n, shortcut)
     {
        if (shortcut->modifiers & unpressed_modifiers)
          {
             _shortcut_handle(shortcut->type_unpress);
             ap.shortcuts->held_shortcuts = eina_list_remove_list(ap.shortcuts->held_shortcuts, l);
          }
     }
   ap.shortcuts->last_modifiers = sc.modifiers;
   /* check if shortcut already pressed */
   l = eina_list_search_sorted_list(ap.shortcuts->held_shortcuts, (Eina_Compare_Cb)_shortcut_cmp, &sc);
   shortcut = eina_list_data_get(l);
   if (!shortcut)
     return ECORE_CALLBACK_PASS_ON;

   _shortcut_handle(shortcut->type_unpress);
   ap.shortcuts->held_shortcuts = eina_list_remove_list(ap.shortcuts->held_shortcuts, l);
   if (shortcut->type_unpress != SHORTCUT_TYPE_NONE)
     return ECORE_CALLBACK_DONE;

   return ECORE_CALLBACK_PASS_ON;
}

static void
_win_unfocused_cb(void *data __UNUSED__,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   Shortcut *shortcut;

   EINA_LIST_FREE(ap.shortcuts->held_shortcuts, shortcut)
      _shortcut_handle(shortcut->type_unpress);
}

static void
_add_shortcut(Shortcut_Type type_press,
              Shortcut_Type type_unpress,
              unsigned int modifiers,
              unsigned int keycode)
{
   Shortcut *sc;
   assert(ap.shortcuts != NULL);

   sc = mem_calloc(1, sizeof(Shortcut));
   sc->type_press = type_press;
   sc->type_unpress = type_unpress;
   sc->keycode = keycode;
   sc->modifiers = modifiers;

   ap.shortcuts->shortcuts = eina_list_sorted_insert(ap.shortcuts->shortcuts, (Eina_Compare_Cb)_shortcut_cmp, sc);
}

static void
_default_shortcuts_add()
{
   assert(ap.shortcuts != NULL);

   _add_shortcut(SHORTCUT_TYPE_UNDO, SHORTCUT_TYPE_NONE,
                 MOD_CTRL, 52/*z*/);
   _add_shortcut(SHORTCUT_TYPE_REDO, SHORTCUT_TYPE_NONE,
                 MOD_CTRL, 29/*y*/);
   _add_shortcut(SHORTCUT_TYPE_SAVE, SHORTCUT_TYPE_NONE,
                 MOD_CTRL, 39/*s*/);
   _add_shortcut(SHORTCUT_TYPE_ADD_GROUP, SHORTCUT_TYPE_NONE,
                 MOD_CTRL, 57/*n*/);
   _add_shortcut(SHORTCUT_TYPE_ADD_PART, SHORTCUT_TYPE_NONE,
                 MOD_NONE, 24/*q*/);
   _add_shortcut(SHORTCUT_TYPE_ADD_STATE, SHORTCUT_TYPE_NONE,
                 MOD_NONE, 25/*w*/);
   _add_shortcut(SHORTCUT_TYPE_ADD_ITEM, SHORTCUT_TYPE_NONE,
                 MOD_NONE, 26/*e*/);
   _add_shortcut(SHORTCUT_TYPE_ADD_PROGRAM, SHORTCUT_TYPE_NONE,
                 MOD_NONE, 27/*r*/);
   _add_shortcut(SHORTCUT_TYPE_DEL, SHORTCUT_TYPE_NONE,
                 MOD_NONE, 119/*del*/);

   _add_shortcut(SHORTCUT_TYPE_TAB_NUM1, SHORTCUT_TYPE_NONE,
                 MOD_CTRL, 10/*1*/);
   _add_shortcut(SHORTCUT_TYPE_TAB_NUM2, SHORTCUT_TYPE_NONE,
                 MOD_CTRL, 11/*2*/);
   _add_shortcut(SHORTCUT_TYPE_TAB_NUM3, SHORTCUT_TYPE_NONE,
                 MOD_CTRL, 12/*3*/);
   _add_shortcut(SHORTCUT_TYPE_TAB_NUM4, SHORTCUT_TYPE_NONE,
                 MOD_CTRL, 13/*4*/);
   _add_shortcut(SHORTCUT_TYPE_TAB_NUM5, SHORTCUT_TYPE_NONE,
                 MOD_CTRL, 14/*5*/);
   _add_shortcut(SHORTCUT_TYPE_TAB_NUM6, SHORTCUT_TYPE_NONE,
                 MOD_CTRL, 15/*6*/);
   _add_shortcut(SHORTCUT_TYPE_TAB_NUM7, SHORTCUT_TYPE_NONE,
                 MOD_CTRL, 16/*7*/);
   _add_shortcut(SHORTCUT_TYPE_TAB_NUM8, SHORTCUT_TYPE_NONE,
                 MOD_CTRL, 17/*8*/);
   _add_shortcut(SHORTCUT_TYPE_TAB_NUM9, SHORTCUT_TYPE_NONE,
                 MOD_CTRL, 18/*9*/);
   _add_shortcut(SHORTCUT_TYPE_TAB_NUM10, SHORTCUT_TYPE_NONE,
                 MOD_CTRL, 19/*0*/);

   _add_shortcut(SHORTCUT_TYPE_TAB_NEXT, SHORTCUT_TYPE_NONE,
                 MOD_CTRL, 23/*TAB*/);
   _add_shortcut(SHORTCUT_TYPE_TAB_PREV, SHORTCUT_TYPE_NONE,
                 MOD_CTRL|MOD_SHIFT, 23/*TAB*/);
   _add_shortcut(SHORTCUT_TYPE_TAB_NEXT, SHORTCUT_TYPE_NONE,
                 MOD_CTRL, 117/*PG_DOWN*/);
   _add_shortcut(SHORTCUT_TYPE_TAB_PREV, SHORTCUT_TYPE_NONE,
                 MOD_CTRL, 112/*PG_UP*/);

   _add_shortcut(SHORTCUT_TYPE_TAB_CLOSE, SHORTCUT_TYPE_NONE,
                 MOD_CTRL, 25/*w*/);

   _add_shortcut(SHORTCUT_TYPE_TAB_IMAGE_MANAGER, SHORTCUT_TYPE_NONE,
                 MOD_NONE, 73/*F7*/);
   _add_shortcut(SHORTCUT_TYPE_TAB_SOUND_MANAGER, SHORTCUT_TYPE_NONE,
                 MOD_NONE, 74/*F8*/);
   _add_shortcut(SHORTCUT_TYPE_TAB_STYLE_MANAGER, SHORTCUT_TYPE_NONE,
                 MOD_NONE, 75/*F9*/);
   _add_shortcut(SHORTCUT_TYPE_TAB_COLOR_CLASS_MANAGER, SHORTCUT_TYPE_NONE,
                 MOD_NONE, 76/*F10*/);
}

/*=============================================*/
/*               PUBLIC API                    */
/*=============================================*/
Eina_Bool
shortcuts_profile_load(Profile *profile __UNUSED__)
{
   return false;
}

Eina_Bool
shortcuts_init(void)
{
   assert(ap.shortcuts == NULL);
   assert(ap.win != NULL);

   ap.shortcuts = mem_calloc(1, sizeof(Shortcut_Module));

   ap.shortcuts->shortcuts_handler = ecore_event_handler_add(ECORE_EVENT_KEY_DOWN,
                                                   _key_press_event_cb,
                                                   NULL);
   ap.shortcuts->shortcuts_handler_unpress = ecore_event_handler_add(ECORE_EVENT_KEY_UP,
                                                                      _key_unpress_event_cb,
                                                                      NULL);
   evas_object_smart_callback_add(ap.win, "unfocused", _win_unfocused_cb, NULL);

   _default_shortcuts_add();

   return true;
}

Eina_Bool
shortcuts_shutdown(void)
{
   assert(ap.shortcuts != NULL);
   assert(ap.shortcuts->shortcuts_handler != NULL);

   ecore_event_handler_del(ap.shortcuts->shortcuts_handler);
   ap.shortcuts->shortcuts_handler = NULL;
   ecore_event_handler_del(ap.shortcuts->shortcuts_handler_unpress);
   ap.shortcuts->shortcuts_handler_unpress = NULL;
   free(ap.shortcuts);
   ap.shortcuts = NULL;

   return true;
}
