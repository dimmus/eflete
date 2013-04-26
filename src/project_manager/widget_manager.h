#ifndef WIDGET_MANAGER_H
#define WIDGET_MANAGER_H

#include <Eina.h>
#include <Edje.h>
#include <string.h>
#include "logger.h"

struct _Part
{
	EINA_INLIST;
	char *part_name;
	Evas_Object *obj;
	Eina_Bool show;
	/*TODO: add part details */
};

typedef struct _Part Part;

struct _Group
{
	EINA_INLIST;
	char *group_name;
	char *full_group_name;
	Evas_Object *obj;
	Eina_Bool show;
	Eina_Inlist *parts;
};

typedef struct _Group Group;

struct _WClass
{
	EINA_INLIST;
	char *class_name;
	Eina_Inlist *groups;
};

typedef struct _WClass WClass;

struct _Widget
{
	EINA_INLIST;
	char *widget_name;
	Eina_Inlist *classes;
};

typedef struct _Widget Widget;

/**
 *
 */
Part *
wm_part_add(const Evas_Object *obj, const char *part);

/**
 *
 */
//Part *
//wm_part_empty_add(Evas_Object *obj, const char *part, Edje_Part_Type type);

/**
 *
 */
Eina_Bool
wm_part_free(Part *part);

/**
 *
 */
Group *
wm_group_add(const char *group, const char *full_name_group);

/**
 *
 */
Eina_Bool
wm_group_free(Group *group);

/**
 *
 */
WClass *
wm_wclass_add(const char *wclass, Eina_List *groups);

/**
 *
 */
Eina_Bool
wm_wclass_free(WClass *wclass);

/**
 *
 */
Widget *
wm_widget_add(const char *widget, Eina_List *widget_styles);

/**
 *
 */
Eina_Bool
wm_widget_free(Widget *widget);

/**
 *
 */
Eina_Inlist *
wm_widget_list_new(const char *file);

/**
 *
 */
Eina_Bool
wm_widget_list_free(Eina_Inlist *widget_list);

#endif /* WIDGET_MANAGER_H */
