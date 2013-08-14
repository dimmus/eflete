#ifndef WIDGET_MANAGER_H
#define WIDGET_MANAGER_H

/**
 * @defgroup WidgetManager Widget Manager
 * @ingroup ProjectManager
 *
 * The module 'Widget Manager' designed to create, storage and manipulation the
 * data of a widget theme. A module create a list, for a list are using
 * Eina_Inlist, of widgets which consist in a theme file. List has the following
 * stucture:
 * - widget (#_Widget)
 *   - style (#_Style)
 *     - group (#_Group)
 *       - part (#_Part)
 *       - [part]
 *       - ...
 *     - [group]
 *     - ...
 *   - [style]
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
 * @enum _type
 * @ingroup WidgetManager
 */
enum _type
{
	WIDGET = 0,
	STYLE,
	GROUP,
	PART
};

/**
 * @typedef type
 * @ingroup WidgetManager
 */
typedef enum _type type;

/**
 * @struct _Part
 * This struct designed to storage a data of part of group.
 * In a theme file (.edc) this data presented as block 'part'
 *
 * @ingroup WidgetManager
 */
struct _Part
{
	EINA_INLIST;
	Eina_Stringshare *name;
	Evas_Object *obj;
	Eina_Bool show;
	int type;
	type __type;
};

/**
 * @typedef Part
 * @ingroup WidgetManager
 */
typedef struct _Part Part;

/**
 * @struct _Group
 * This struct designed to storage a data of 'group' - a widget layout.
 * In a theme file (.edc) this data presented as block 'group'.
 *
 * @ingroup WidgetManager
 */
struct _Group
{
	EINA_INLIST;
   /** Member 'group_name' consist a name of a group. **/
   Eina_Stringshare *group_name;
   /** Member 'full_group_name' consist a name of a block 'group'. **/
	Eina_Stringshare *full_group_name;
   /** Member 'obj' - edje edit object.**/
	Evas_Object *obj;
   /** Show/Hide a object on a workspace. **/
	Eina_Bool show;
	int current_w, current_h;
   /** Member 'parts' saved a list of a group parts data. **/
	Eina_Inlist *parts;
	type __type;
};

/**
 * @typedef Group
 * @ingroup WidgetManager
 */
typedef struct _Group Group;

/**
 * @struct _Style
 * This struct designed to storage a data of style.
 *
 * @ingroup WidgetManager
 */
struct _Style
{
	EINA_INLIST;
   /** Member 'style_name' consist a name of a style. **/
	Eina_Stringshare *style_name;
   /** Member 'groups' saved a list of groups that make up the style. **/
	Eina_Inlist *groups;
	type __type;
};

/**
 * @typedef Style
 * @ingroup WidgetManager
 */
typedef struct _Style Style;

/**
 * @struct _Widget
 * This struct designed to storage a widget style data.
 *
 * @ingroup WidgetManager
 */
struct _Widget
{
	EINA_INLIST;
   /** Member 'widget_name' consist a name of a widget. **/
	Eina_Stringshare *widget_name;
   /** Member 'styles' consist a list of a widget styles. **/
	Eina_Inlist *styles;
	type __type;
};

/**
 * @typedef Widget
 * @ingroup WidgetManager
 */
typedef struct _Widget Widget;

/**
 * Create a new Part object
 *
 * @param obj The edje edit object with a 'group' data.
 * @param part The name of a part.
 * @return A new Part object.
 *
 * @ingroup WidgetManager
 */
Part *
wm_part_add(Evas_Object *obj, const char *part);

/**
 *
 */
/*TODO: Add 'wm_part_new', for new part which will be add user */

/**
 * Free a Part object
 *
 * @param part A Part object to free.
 * @return EINA_TRUE if a object deleted, else EINA_FALSE.
 *
 * @ingroup WidgetManager
 */
Eina_Bool
wm_part_free(Part *part);

/**
 * Get a list of signals which using in a 'group' programs
 *
 * @param programs A list of programs.
 * @return A list of signals
 *
 * @ingroup WidgetManager
 */
Eina_List *
wm_program_signals_list_get(Group *group);

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
 * Create a new Group object.
 *
 * @param group_name A name of a group.
 * @param full_group_name A full name of group, a name of block 'group' in a
 *        edc-file.
 * @return A new Group object.
 *
 * @ingroup WidgetManager
 */
Group *
wm_group_add(const char *group_name, const char *full_group_name);

/**
 * Free a Group object.
 *
 * @param group A Group object to free.
 * @return EINA_TRUE if a object is deleted, else EINA_FALSE.
 *
 * @ingroup WidgetManager
 */
Eina_Bool
wm_group_free(Group *group);

/**
 * Create a new Style object.
 *
 * @param style A name of a style.
 * @param groups A list of groups in a current style.
 *
 * @ingroup WidgetManager
 */
Style *
wm_style_add(const char *style, Eina_List *groups);

/**
 * Free a Style object.
 *
 * @param style A Style object to free.
 * @return EINA_TRUE if a object is deleted.
 *
 * @ingroup WidgetManager
 */
Eina_Bool
wm_style_free(Style *style);

/**
 * Create a new Widget object.
 *
 * @param widget A name of a widget.
 * @param groups A list of 'group' styles in a current widget.
 * @return A new Widget object.
 *
 * @ingroup WidgetManager
 */
Widget *
wm_widget_add(const char *widget, Eina_List *groups);

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
wm_widget_list_new(const char *file);

/**
 * Free a generic Eina_Inlist of widgets, allocated by \ref wm_widget_list_new.
 *
 * @param widget_list A list of widgets to free.
 * @return EINA_TRUE if a list os deleted.
 *
 * @ingroup WidgetManager
 */
Eina_Bool
wm_widget_list_free(Eina_Inlist *widget_list);

/**
 * Create edje edit object and load data from edj-file.
 *
 * @param widget_list A list of widget to load data.
 * @param e A Evas, parent object.
 * @param path Path to theme file (.edj).
 *
 * @ingroup WidgetManager
 */
void
wm_widget_list_objects_load(Eina_Inlist *widget_list,
                            Evas *e,
                            const char *path);

/**
 * Find group object in widget list. Use full name of group.
 *
 * @param widget_list the Eina_Inlist list that contain wdgets structures.
 * @param group_full_name the string full name of group.(with widget/class/style)
 * @return the group object.
 *
 * @ingroup WidgetManager
 */
Evas_Object *
wm_group_object_find(Eina_Inlist *widget_list, const char *group_full_name);

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

#endif /* WIDGET_MANAGER_H */
