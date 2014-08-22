/*I{
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
 *}
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

/* localization */
#ifdef HAVE_GETTEXT
   #include <libintl.h>
   #define _(string) gettext(string)
   #define N_(string) string
#else
   #define _(string) string
   #define N_(string) string
#endif /* localization */

/* TODO: delete it, and remake all strings to eina_stringshare or eina_strbuff */
#define PATH_MAX 4096
#define BUFF_MAX 512

#include <Eina.h>
#include <Ecore.h>
#include <Evas.h>
#include <Ecore_File.h>
#include <Eet.h>
#include <Efreet.h>
#include <Elementary.h>
#include <Ewe.h>
#include "alloc.h"
#include "logger.h"
#include "project_manager.h"
#include "ui_workspace.h"
#include "live_view.h"
#include "notify.h"

struct _App_Data
{
   Evas_Object *win;
   Evas_Object *win_layout;
   Evas_Object *main_menu;
   Eina_Hash *menu_hash;
   Evas_Object *popup;
   Evas_Object *colorsel; /**< global colorselector. the one colorselector for
                            application. */
   Evas_Object *statusbar; /**< The statusbar object, which contain some items */
   struct {
      Evas_Object *left;
      Evas_Object *right;
      Evas_Object *left_hor;
      Evas_Object *right_hor;
      Evas_Object *center;
      Evas_Object *center_down;
   } panes;

   struct {
      Evas_Object *left_top;
      Evas_Object *left_bottom;
      Evas_Object *bottom_left;
      Evas_Object *bottom_right;
      Evas_Object *right_top;
      Evas_Object *right_bottom;
      Evas_Object *canvas;
   } block;
   Evas_Object *workspace;
   Live_View *live_view;
   Project *project;
};

/**
 *
 */
typedef struct _App_Data App_Data;
EAPI_MAIN int elm_main();

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
 * This function will return win_layout (if App_Data and win_layout exists).
 * If they are not exist, then this function will return NULL.
 * This function is usable for notification.
 *
 * @return win_layout if exist, NULL if not exist.
 *
 * @ingroup Eflete
 */
Evas_Object *
win_layout_get(void);

/**
 * Allocate memory for creating App_Data.
 *
 * @return link to created empty App_Data.
 *
 * @ingroup Eflete
 */
App_Data *
app_data_get(void);

/**
 * This function will free allocated memory used for App_Data.
 *
 * @return EINA_TRUE application data freed succesfully.
 * EINA_FALSE failed to free application data.
 *
 * @ingroup Eflete
 */
Eina_Bool
app_free();

/**
 * This function get pointer to main window of application.
 *
 * @return pointer to main window Evas_Object.
 * @ingroup Eflete
 */
Evas_Object *
main_window_get(void);

/**
 * Get the colorselector object pointer.
 * The colorselector one for the entire application.
 *
 * @return pointer to colorselector.
 * @ingroup Eflete
 */
Evas_Object *
colorselector_get(void);


#define GET_IMAGE(IMG, PARENT, NAME) \
{ \
   IMG = edje_object_add(PARENT); \
   if (!edje_object_file_set(IMG, EFLETE_RESOURCES, NAME)) \
     ERR("Image with name \"%s\" was not found in resources\n", NAME); \
}

#endif /* EFLETE_H */
