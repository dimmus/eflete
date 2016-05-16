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
typedef struct _Shortcuts Shortcuts;
typedef struct _ColorClassData ColorClassData;

#include "common_macro.h"
TODO("delete it, and remake all strings to eina_stringshare or eina_strbuff")
#ifndef PATH_MAX
   #define PATH_MAX 4096
#endif
#define BUFF_MAX 512

#include <Ewe.h>

/* do not allow unsafe sprintf. use snprintf instead */
#pragma GCC poison sprintf

#include <Elementary.h>
#include "logger.h"
#include "string_common.h"
#include "editor.h"
#include "signals.h"

#define EFLETE_INTERNAL_GROUP_NAME "___eflete_internal_group___"
#define EFLETE_DUMMY_IMAGE_NAME "___eflete_dummy_image___.png"

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
   } property;

   struct {
      Evas_Object *history;
      Elm_Object_Item *item_history, *item_property;
   } block;
   Project *project;
   Shortcut_Module *shortcuts; /**< Structure with data from shortcuts module */
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

/**
 * Start nested not blocked ecore main loop.
 *
 * @note Master Raster add to ecore_main_loop_begin the hard check to nested
 * main loops. Now have no time to remake our popup behavior, and add this
 * crutch. In further need to delete it.
 *
 * @ingroup Eflete
 */
void
eflete_main_loop_begin(void);

/**
 * End nested main loop.
 *
 * @ingroup Eflete
 */
void
eflete_main_loop_quit(void);

#define GET_IMAGE(IMG, PARENT, NAME) \
{ \
   IMG = edje_object_add(PARENT); \
   if (!edje_object_file_set(IMG, EFLETE_RESOURCES, NAME)) \
     ERR("Image with name \"%s\" was not found in resources\n", NAME); \
}

#include "alloc.h"

#endif /* EFLETE_H */
