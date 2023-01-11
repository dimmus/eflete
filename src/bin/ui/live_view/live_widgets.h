#ifndef LIVE_WIDGETS_H
#define LIVE_WIDGETS_H

#include "eflete.h"
/*****************************************************
 ************** COMMON MACRO AND FUNCTIONS************
 *****************************************************/

#define ITEM "item"
#define EDJE_EDIT_OBJ "edje_edit_obj"
#define SIGNAL_NAME "signal_data_name"
#define SIGNAL_SOURCE "signal_data_source"

#define TEXT_FUNC "text_func"
#define SWALLOW_FUNC "swallow_func"
#define SWALLOW_CLEAN_FUNC "swallow_clean_func"
#define SIGNAL_FUNC "signal_func"
#define SWALLOW_LIST "swallow_list"
#define TEXT_LIST "part_list"

#define RECT_COLOR 136, 24, 242, 255

#define COLOR_BLUE_LIGHT 57, 102, 147, 255
#define COLOR_BLUE_DARK 58, 92, 126, 255
#define ELEMENTS_SMALL_COUNT 5
#define ELEMENTS_MID_COUNT   25
#define ELEMENTS_BIG_COUNT   50
#define ELEMENT_BIG_SIZE     2000

typedef void (* Swallow_Clean_Func)(const char *, Evas_Object *);

Eina_Bool
standard_widget_name_parse(const char *full_group_name, Eina_Stringshare **widget, Eina_Stringshare **class, Eina_Stringshare **style_name);

/**
 * Special "widget" for program editor. Privat API
 */
Evas_Object *
layout_prog_edit_create(Evas_Object *parent);
#endif
