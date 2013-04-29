#ifndef __UI_HEADER__
#define __UI_HEADER__
#include <Evas.h>

struct _UI_Data
{
	Evas_Object *win;
	Evas_Object *win_layout;
	Evas_Object *block_left_top;
	Evas_Object *block_left_bottom;
	Evas_Object *block_bottom_left;
	Evas_Object *block_bottom_right;
	Evas_Object *block_right_top;
	Evas_Object *block_right_bottom;
	Evas_Object *main_menu;
	Evas_Object *block_canvas;
};

typedef struct _UI_Data UI_Data;

UI_Data *
ui_create (void);
#endif // __UI_HEADER__


