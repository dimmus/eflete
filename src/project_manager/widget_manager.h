#ifndef WIDGET_MANAGER_H
#define WIDGET_MANAGER_H

#define EDJE_EDIT_IS_UNSTABLE_AND_I_KNOW_ABOUT_IT

#include <Eina.h>
#include <Ecore_Evas.h>
#include <Edje_Edit.h>
#include <string.h>
#include "logger.h"


enum _type
{
	WIDGET = 0,
	STYLE,
	GROUP,
	PART
};

typedef enum _type type;

struct _Part_State_Image
{
	const char *normal_image;
	int id;
	Edje_Edit_Image_Comp compression_type;
	int compression_rate;
	int left;
	int right;
	int top;
	int bottom;
	unsigned int border_fill;
	double fill_origin_relative_x;
	double fill_origin_relative_y;
	double fill_origin_offset_x;
	double fill_origin_offset_y;
	double fill_size_relative_x;
	double fill_size_relative_y;
	double fill_size_offset_x;
	double fill_size_offset_y;
	Eina_List *tweens_images;

};

typedef struct _Part_State_Image Part_State_Image;

struct _Part_State_Text
{
	const char *text;
	const char *font;
	int size;
	double align_x;
	double align_y;
	double elipsis;
	Eina_Bool fit_x;
	Eina_Bool fit_y;
	Eina_List *fonts_list;
};

typedef struct _Part_State_Text Part_State_Text;

struct _Part_State
{
	EINA_INLIST;
	const char *name;
	double value;
	double rel1_relative_x;
	double rel1_relative_y;
	double rel2_relative_x;
	double rel2_relative_y;
	int rel1_offset_x;
	int rel1_offset_y;
	int rel2_offset_x;
	int rel2_offset_y;
	const char *rel1_to_x_name;
	const char *rel1_to_y_name;
	const char *rel2_to_x_name;
	const char *rel2_to_y_name;
	int color[4]; // array of r,g,b,a
	int color2[4];
	int color3[4];
	double align_x;
	double align_y;
	int min_w;
	int min_h;
	int max_w;
	int max_h;
	/* 'fixed' not implemented in edje_edit */
	double aspect_min;
	double aspect_max;
	unsigned char aspect_pref;
	Eina_Bool visible;
	const char *color_class;
	union // Can be used only one
	{
		Part_State_Text *text;
		Part_State_Image *image;
	};
};

typedef struct _Part_State Part_State;

struct _Part
{
	EINA_INLIST;
	const char *name;
	Eina_Bool show;
	const char *api_name;
	const char *description;
	Edje_Part_Type type;
	const char *clip_to;
	const char *source;
	Edje_Text_Effect effect;
	const char *selected_state_name;
	double selected_state;
	Eina_Bool mouse_events;
	Eina_Bool repeat_events;
	Evas_Event_Flags ignore_flags;
	Eina_Bool scale;
	int drag_state_x;
	int drag_state_y;
	int drag_step_x;
	int drag_step_y;
	int drag_count_x;
	int drag_count_y;
	const char *drag_confine;
	const char *drag_event;
	Eina_Inlist *states;
	type __type;
};

typedef struct _Part Part;

struct _Program
{
	EINA_INLIST;
	const char *name;
	const char *source;
	const char *signal;
	double in_form;
	double in_range;
	Edje_Action_Type action;
	Eina_List *targets;
	Eina_List *afters;
	const char *state;
	const char *state2;
	const char *api_name;
	const char *api_description;
	double value;
	double value2;
	Edje_Tween_Mode transition;
	double transition_time;
	const char *filter_part;
};

typedef struct _Program Program;

struct _Group
{
	EINA_INLIST;
	char *group_name;
	char *full_group_name;
	Evas_Object *obj;
	Eina_Bool show;
	int min_w, min_h;
	int max_w, max_h;
	int current_w, current_h;
	Eina_Inlist *parts;
	Eina_Inlist *programs;
	type __type;
};

typedef struct _Group Group;

struct _Style
{
	EINA_INLIST;
	char *style_name;
	Eina_Inlist *groups;
	type __type;
};

typedef struct _Style Style;

struct _Widget
{
	EINA_INLIST;
	char *widget_name;
	Eina_Inlist *styles;
	type __type;
};

typedef struct _Widget Widget;

/**
 *
 */
Part *
wm_part_add(Evas_Object *obj, const char *part);

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
/*
Eina_Bool
wm_part_update(Evas_Object *obj,
				const Part *part,
				const char *part_name,
				const Part_Update *part_update);
*/

/**
 *
 */
Program *
wm_program_add(Evas_Object* obj, const char* program_name);

/**
 *
 */
Eina_Bool
wm_program_free(Program *program);

/**
 *
 */
/*
Eina_Bool
wm_program_update(Evas_Object *obj,
				const Program *program,
				const char *program_name,
				const Program_Update *program_update);
*/

/**
 *
 */
Eina_List *
wm_program_signals_list_get(const Eina_Inlist *programs);

/**
 *
 */
Eina_Bool
wm_program_signals_list_free(Eina_List *signals);

/**
 *
 */
Group *
wm_group_add(const char *group_name, const char *full_group_name);

/**
 *
 */
void
wm_group_data_load(Group *group, Evas *e, const char *edj);

/**
 *
 */
Eina_Bool
wm_group_free(Group *group);

/**
 *
 */
Style *
wm_style_add(const char *style, Eina_List *groups);

/**
 *
 */
Eina_Bool
wm_style_free(Style *style);

/**
 *
 */
Widget *
wm_widget_add(const char *widget, Eina_List *groups);

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
