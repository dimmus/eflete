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

#ifndef WIDGET_MANAGER_B_H
#define WIDGET_MANAGER_B_H

/**
 * @defgroup WidgetManager Widget Manager
 * @ingroup ProjectManager
 *
 * The module 'Widget Manager' designed to create, storage and manipulation the
 * data of a widget theme. A module create a list, for a list are using
 * Eina_Inlist, of widgets which consist in a theme file. List has the following
 * stucture:
 * - widget (#_Widget)
 *   - class (#_Class)
 *     - style (#_Style)
 *       - part (#_Part)
 *       - [part]
 *       - ...
 *     - [style]
 *     - ...
 *   - [class]
 *   - ...
 * - [widget]
 * - ...
 *
 */

#define EDJE_EDIT_IS_UNSTABLE_AND_I_KNOW_ABOUT_IT

#include <Eina.h>
#include <Ecore_Evas.h>
#include <Edje_Edit.h>
#include "logger.h"

/**
 * @enum _Type
 * @ingroup WidgetManager
 */
enum _Type
{
    WIDGET = 0,
    CLASS,
    STYLE,
    PART,
    LAYOUT
};

/**
 * @typedef Type
 * @ingroup WidgetManager
 */
typedef enum _Type Type;

/**
 * @typedef Part
 * @ingroup WidgetManager
 */
typedef struct _Part Part;

/**
 * @typedef Style
 * @ingroup WidgetManager
 */
typedef struct _Style Style;


/**
 * @typedef Class
 * @ingroup WidgetManager
 */
typedef struct _Class Class;

/**
 * @typedef Widget
 * @ingroup WidgetManager
 */
typedef struct _Widget Widget;

/**
 * @struct _Part
 *
 * @brief This struct designed to storage a data part in style.
 * In a theme file (.edc) this data presented as block 'part'
 *
 * @ingroup WidgetManager
 */
struct _Part
{
    EINA_INLIST;
    Type __type; /**< PART from enum @_type */
    Eina_Stringshare *name; /**< Part name in style */
    Eina_Stringshare *curr_state; /**< Current selected part state name */
    double curr_state_value; /**< Current selected part state value */
    Eina_Bool show; /**< Flag indicate current visibly part evas primitive */
    int type; /**< Id part type. Type value equal @Edje_Part_Type enum */
};

/**
 * @struct _Style
 *
 * @brief This struct designed to storage a data of 'style' - a widget layout.
 * In a theme file (.edc) this data presented as block 'group'.
 *
 * @ingroup WidgetManager
 */
struct _Style
{
    EINA_INLIST;
    Type __type; /**< STYLE from enum @_type */
    Eina_Stringshare *name; /**< The name of style. */
    Eina_Stringshare *full_group_name; /**< The name of a block 'group' in edc. */
    Evas_Object *obj; /** Edje edit object, which loaded form theme file. */
    Eina_Bool isModify;  /**< Is it style modificated. */
    /** is it Group an alias **/
    Eina_Bool isAlias;
    /** link to main group that is being aliased **/
    Style *main_group;
    Eina_Inlist *parts;  /**< The list of a style parts data. */
    Eina_List *aliasses; /**< The list of pointer Style structures which aliased to this style */
    Class *parent; /**< Pointer to class, which contain this style */
};

/**
 * @struct _Class
 *
 * @brief This struct designed to storage a data of class.
 *
 * @ingroup WidgetManager
 */
struct _Class
{
    EINA_INLIST;
    Type __type; /**< CLASS from enum @_type */
    Eina_Stringshare *name; /**< The name of a class. */
    Eina_Inlist *styles;    /**<The list of styles that make up the class. */
    Widget *parent; /**< Pointer to widget, which contain this class*/
};

/**
 * @struct _Widget
 *
 * @brief This struct designed to storage a widget classes data.
 *
 * @ingroup WidgetManager
 */
struct _Widget
{
    EINA_INLIST;
    Type __type; /**< WIDGET from enum @_type */
    Eina_Stringshare *name; /**< The name of a widget. */
    Eina_Inlist *classes; /**< The list of a widget classes. */
};

/**
 * @struct _Signal
 *
 * @brief This struct is designed to store a signal data. I.e. program name
 *        where this signal is used, source name (emitter) of this signal and
 *        pointer to @Style which using this signal.
 *
 * @ingroup WidgetManager
 */
struct _Signal
{
    Eina_Stringshare *name; /**< text name of signal */
    Eina_Stringshare *source; /**< emitter name */
    Eina_Stringshare *program; /**< program, where this signal is used */
    Style *style; /**< pointer to style, where this signal is used */
};
typedef struct _Signal Signal;
/**
 * Delete a Part from the @Style object
 *
 * @param style A @Style object being editing
 * @param part A Part object to be deleted.
 * @return EINA_TRUE if a object deleted, else EINA_FALSE.
 *
 * @ingroup WidgetManager
 */
Eina_Bool
wm_part_del(Style *style, Part *part);

/**
 * Set a current state in Part object.
 *
 * @param part The part to set the state of.
 * @param state Name of the state to set.
 * @return EINA_TRUE if successful, EINA_FALSE otherwise.
 *
 * @note State has format "NAME X.X" we need to extract NAME
 * and double value separately
 *
 * @ingroup WidgetManager
 */
Eina_Bool
wm_part_current_state_set(Part *part, const char *state);

/**
 * Fill current states for all parts structures, which exist in style.
 *
 * @param style A @Style object being editing
 *
 * @return EINA_TRUE if successful, EINA_FALSE if not.
 *
 * @ingroup WidgetManager
 */
Eina_Bool
wm_style_current_state_parts_update(Style *style);

/**
 * Set "default" 0.0 state for all parts, which exist in style.
 *
 * @param style A @Style object being editing
 *
 * @return EINA_TRUE if successful, EINA_FALSE if not.
 *
 * @ingroup WidgetManager
 */
Eina_Bool
wm_style_state_parts_reset(Style *style);

/**
 * Create a new Part object
 *
 * @param style The @Style object.
 * @param part The name of a part.
 * @return A new Part object.
 *
 * @ingroup WidgetManager
 */
Part *
wm_part_add(Style *style, const char *part);

/**
 * Get a list of signals which using in a style programs
 *
 * @param style The @Style object with loaded edje edit object.
 * @return A list of signals
 *
 * @ingroup WidgetManager
 */
Eina_List *
wm_program_signals_list_get(Style *style);

/**
 * Free a signal list.
 *
 * @param signals A list of signals to free.
 * @return EINA_TRUE if a list is deleted, else EINA_FALSE
 *
 * @ingroup WidgetManager
 */
Eina_Bool
wm_program_signals_list_free(Eina_List *signals);

/**
 * Load part data to @Style object, also load base style parametrs and create
 * edje_edit object.
 *
 * @param style A @Style object pointer, in wich must be loaded data.
 * @param e A Evas, parent object.
 * @param edj Path to theme file (.edj).
 *
 * @return EINA_TRUE if successful, EINA_FALSE if not.
 *
 * @ingroup WidgetManager
 */
Eina_Bool
wm_style_data_load(Style *style, Evas *e, const char *edj);

/**
 * Create a new @Style object.
 *
 * @param style_name The style name.
 * @param full_group_name A full name of group, a name of block 'group' in a
 *        edc-file.
 * @param style_type May be STYLE or LAYOUT, in case what type needed to create.
 * @param parent The pointer to Class structure, which contain creating  style.
 *
 * @return A new @Style object.
 *
 * @ingroup WidgetManager
 */
Style *
wm_style_add(const char* style_name, const char* full_group_name,
             Type style_type, Class *parent);

/**
 * Free a @Style object.
 *
 * @param style A @Style object to free.
 *
 * @return EINA_TRUE if a object is deleted, else EINA_FALSE.
 *
 * @ingroup WidgetManager
 */
Eina_Bool
wm_style_free(Style *style);

/**
 * Copy all parms of style from one edje_edit object to another. Before call
 * this function need prepare pointer to edje edit object, which contain
 * source style. Also need pointer to edje_edit object where new style will
 * created.
 * First of all copy main params of style, like min/max size. At second copy
 * all params of states of parts in style. Also copyes programms for style.
 *
 * @param dest_edje The edje_edit object, which provide access to edj file data.
 * in this object will copy new style.
 * @param source_edje The edje_edit object, which provide access to edj file data.
 * from this object will gave params for copying.
 * @param full_name The string with full name of new style.
 * @param dest_file The string path to edj file, where will copyed new style.
 * @param style The new @Style structure object with new style
 *
 * @return EINA_FALSE on failure, EINA_TRUE on success.
 *
 */
Eina_Bool
wm_style_copy(Evas_Object *dest_edje, Evas_Object *source_edje,
              Eina_Stringshare *full_name, Eina_Stringshare *dest_file,
              Style *style);

/**
 * Create a new @Class object.
 *
 * @param class_name A name of a class.
 * @param styles A list of styles in a current class.
 * @param parent A pointer to Widget structure, which contain creating class.
 *
 * @ingroup WidgetManager
 */
Class *
wm_class_add(const char *class_name, Eina_List *styles, Widget *parent);

/**
 * Free a @Class object.
 *
 * @param class A @Class object to free.
 *
 * @return EINA_TRUE if a object is deleted.
 *
 * @ingroup WidgetManager
 */
Eina_Bool
wm_class_free(Class *class_st);

/**
 * Create a new Widget object.
 *
 * @param widget A name of a widget.
 * @param styles A list of styles in a current widget.
 * @return A new Widget object.
 *
 * @ingroup WidgetManager
 */
Widget *
wm_widget_add(const char *widget_name, Eina_List *styles);

/**
 * Free a Widget object.
 *
 * @param widget A Widget object to free.
 * @return EINA_TRUE if a object is deleted.
 *
 * @ingroup WidgetManager
 */
Eina_Bool
wm_widget_free(Widget *widget);

/**
 * Create a new list of Widgets.
 *
 * @param file A edj-file to load data of widgets styles.
 * @return A list of widgets.
 *
 * @ingroup WidgetManager
 */
Eina_Inlist *
wm_widgets_list_new(const char *file);

/**
 * Load list of custom layouts, which stored in file.
 *
 * @param file A edj-file to load data of custom layouts styles.
 * @return A list of custom layouts if successful or NULL if failed.
 *
 * @ingroup WidgetManager
 */
Eina_Inlist *
wm_layouts_list_new(const char *file);

/**
 * Free a generic Eina_Inlist of widgets, allocated by \ref wm_widgets_list_new.
 *
 * @param widget_list A list of widgets to free.
 * @return EINA_TRUE if a list os deleted.
 *
 * @ingroup WidgetManager
 */
Eina_Bool
wm_widgets_list_free(Eina_Inlist *widget_list);

/**
 * Free a generic Eina_Inlist of widgets, allocated by \ref wm_layouts_list_new.
 *
 * @param widget_list A list of widgets to free.
 * @return EINA_TRUE if a list os deleted.
 *
 * @ingroup WidgetManager
 */
Eina_Bool
wm_layouts_list_free(Eina_Inlist *widget_list);

/**
 * Find style object in widget list. Use full name of style.
 *
 * @param widget_list the Eina_Inlist list that contain wdgets structures.
 * @param style_full_name the string full name of style.(with widget/class/style)
 *
 * @return the @Style object.
 *
 * @ingroup WidgetManager
 */
Style *
wm_style_object_find(Eina_Inlist *widget_list, const char *style_full_name);

/**
 * Create edje edit object and load data from edj-file.
 *
 * @param widget_list A list of widget to load data.
 * @param e A Evas, parent object.
 * @param path Path to theme file (.edj).
 * @return EINA_TRUE if a list os deleted.
 *
 * @ingroup WidgetManager
 */
Eina_Bool
wm_widgets_list_objects_load(Eina_Inlist *widget_list,
                            Evas *e,
                            const char *path);

/**
 * Create edje edit object and load data from edj-file for custom layouts only.
 *
 * @param layouts_list A list of layouts, which stored in file.
 * @param e A Evas object.
 * @param path Path to theme file (.edj).
 * @return EINA_TRUE if a list os deleted.
 *
 * @ingroup WidgetManager
 */
Eina_Bool
wm_layouts_list_objects_load(Eina_Inlist *layouts_list,
                             Evas *e, const char *path);

/**
 * Return the string with the name of the part type by the giving id (int).
 * @note the returned string should not be deleted!
 *
 * @param type part type id.
 * @return string with the name of part type.
 *
 * @ingroup WidgetManager
 */
const char *
wm_part_type_get(Edje_Part_Type type);

#endif /* WIDGET_MANAGER_B_H*/

