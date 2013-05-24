#ifndef UI_STYLE_VIEWER_WINDOW_HEADER_H
#define UI_STYLE_VIEWER_WINDOW_HEADER_H

#include <Eina.h>
#include "efl_tet.h"

/*
   I think that I should move this structs and
   some functions from C part to 
   the widget_manager.
   So: TODO is - ask for move and move it.
*/

enum _type_style
{
	TAG,
	TEXTSTYLE
};

typedef enum _type_style type_style;

struct _Tag
{
	EINA_INLIST;
	char *tag_name;
	char *tag_value;
	type_style __type;
};

typedef struct _Tag Tag;

struct _TextStyles
{
	EINA_INLIST;
	char *style_name;
	Eina_Inlist *tags;
	type_style __type;
};

typedef struct _TextStyles TextStyles;


/* Can I have global variables like that? */
struct _Style_Window
{
	Evas_Object *entry_style;
	Evas_Object *entry_tag;
	Evas_Object *entry_prop;
};

typedef struct _Style_Window Style_Window;

void
style_viewer_init (App_Data *ap);

/* What functions should I add here? */

#endif	/* UI_STYLE_VIEWER_WINDOW_HEADER_H */

