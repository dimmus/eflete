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

#ifndef EFLETE_H
#define EFLETE_H

/**
 * @defgroup Eflete EFL Edje Theme Editor
 *
 * The begging point of the programm
 * (contain starting function like main and
 * some functions that free/init base required components)
 */

#ifndef EFL_BETA_API_SUPPORT
   #define EFL_BETA_API_SUPPORT 1
#endif

#ifdef HAVE_CONFIG_H
   #include "eflete_config.h"
#endif /* include eflete_config.h */

#include <assert.h>

/* localization */
#ifdef HAVE_GETTEXT
   #include <libintl.h>
   #define _(string) gettext(string)
   #define N_(string) string
   #define NGETTEXT(single, plur, n) ngettext(single, plur, n)
#else
   #define _(string) string
   #define N_(string) string
   #define NGETTEXT(single, plur, n) (((n)==1)? (single):(plur))
#endif /* localization */

typedef struct _Shortcut_Module Shortcut_Module;
typedef struct _Menu Menu;
typedef struct _Live_View Live_View;
typedef struct _Project Project;
typedef struct _Profile Profile;
typedef struct _Resource Resource;
typedef struct _Change Change;
typedef struct _Diff_ Diff;
typedef struct _Group Group;
typedef struct _History History;
typedef struct _State State;
typedef struct _Part Part;
typedef struct _Part_Item Part_Item;
typedef struct _Program Program;
typedef struct _Shortcuts Shortcuts;
typedef struct _ColorClassData ColorClassData;
typedef struct _Image_Item Image_Item;
typedef struct _Colorclass_Item Colorclass_Item;

#include "common_macro.h"
TODO("delete it, and remake all strings to eina_stringshare or eina_strbuff")
#ifndef PATH_MAX
   #define PATH_MAX 4096
#endif
#define BUFF_MAX 512

#include <Ewe.h>

/* do not allow unsafe sprintf. use snprintf instead */
#pragma GCC poison sprintf
/* because this function works only with realized items,
 * we can't use this function with entire genlist list.
 * for working with items use elm_object_item_data_get only!
 */
#undef elm_object_item_text_get
#define elm_object_item_text_get 0_NO_DO_NOT_USE_POISONED
#pragma GCC poison elm_object_item_part_text_get

#include <Elementary.h>
#include "logger.h"
#include "string_common.h"
#include "editor.h"
#include "signals.h"

#define EFLETE_INTERNAL_GROUP_NAME "___eflete_internal_group___"
#define EFLETE_DUMMY_IMAGE_NAME "___eflete_dummy_image___.png"
#define EFLETE_DUMMY_SAMPLE_NAME "___eflete_dummy_sample___.wav"

struct _App_Data
{
   Elm_Theme *theme;
   Evas_Object *win;
   Evas_Object *win_layout;
   Menu *menu;
   TODO("Burn popup variable");
   Evas_Object *popup;
   Eina_Stringshare *last_path;
   Evas_Object *splash;
   Evas_Object *statusbar; /**< The statusbar object, which contain some items */
   Evas_Object *tabs;
   struct {
      Evas_Object *left;
      Evas_Object *right;
      Evas_Object *left_ver;
   } panes;
   struct {
      Evas_Object *group;
      Evas_Object *image_manager;
      Evas_Object *sound_manager;
      Evas_Object *style_manager;
      Evas_Object *color_manager;
      Evas_Object *demo;
   } property;

   struct {
      Evas_Object *history;
      Elm_Object_Item *item_history, *item_property;
   } block;
   struct {
      Eina_Stringshare *theme_edj;
      Eina_Stringshare *layout_edj;
      Eina_Stringshare *edj_path;
      Eina_Stringshare *image_path;
      Eina_Stringshare *sound_path;
      Eina_Stringshare *export_edj;
      struct {
         Eina_Stringshare *path;
         Eina_Stringshare *folder;
         Eina_Stringshare *file;
      } export_edc;
   } path;
   Project *project;
   Shortcut_Module *shortcuts; /**< Structure with data from shortcuts module */
   Eina_Bool exit_in_progress : 1; /**< is set to true when ui_main_window_del() is called.
                                       This is needed to continue closing application after
                                       clicking in save/don't save buttons in project close
                                       popup */
#ifdef HAVE_ENVENTOR
   Evas_Object *enventor;
   Eina_Bool enventor_mode : 1;
#else
#endif /* HAVE_ENVENTOR */
};

/**
 *
 */
typedef struct _App_Data App_Data;
EAPI_MAIN int elm_main();

/* The global Eflete_Data */
extern App_Data ap;

/**
 * This function inits all libraries required by this programm.
 * Also it will add extension to all edc files required by this programm.
 *
 * @return EINA_TRUE all libraries initialized succesfully.
 * EINA_FALSE one of the libraries was failed to inti.
 *
 * @ingroup Eflete
 */
Eina_Bool
app_init(void);

/**
 * Shutdown of the program. It'll free App_Data, remove extension theme
 * (custom EDC needed for this program) and shutdown all launched
 * and inited libraries.
 *
 * @return EINA_TRUE all libraries initialized succesfully.
 * EINA_FALSE one of the libraries was failed to inti.
 *
 * @ingroup Eflete
 */
Eina_Bool
app_shutdown(void);

#define GET_IMAGE(IMG, PARENT, NAME) \
{ \
   IMG = edje_object_add(PARENT); \
   if (!edje_object_file_set(IMG, EFLETE_RESOURCES, NAME)) \
     ERR("Image with name \"%s\" was not found in resources\n", NAME); \
}

#include "alloc.h"


#ifdef HAVE_TIZEN
/* hack functions prototypes (same as original functions) */
Evas_Object *
tizen_hack_spinner_add(Evas_Object *parent);
void
tizen_hack_spinner_value_set(Evas_Object *spinner, double val);
/* saving function pointers to use later in tizen_hack* functions to avoid infonote recursion on call */
static void (* _elm_spinner_value_set)(Evas_Object *, double) __UNUSED__ = elm_spinner_value_set;
static Evas_Object * (* _elm_spinner_add)(Evas_Object *)  __UNUSED__ = elm_spinner_add;
/* replacing functions with hack-version */
#define elm_spinner_value_set    tizen_hack_spinner_value_set
#define elm_spinner_add          tizen_hack_spinner_add
#endif

#endif /* EFLETE_H */
