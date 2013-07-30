#ifndef STYLE_VIEWER_DIALOG_HEADER_H
#define STYLE_VIEWER_DIALOG_HEADER_H

/**
 * @defgroup TextStyle TextStyle
 *
 * This widget emits the following signals:
 *
 * @li "ts,res,add": add new text style resource
 * @li "ts,res,rm": remove text style resource.
 * @li "ts,res,cp": copy text style resource into new one.
 * @li "ts,res,modify,tag,add": add tag to the text style resource.
 * @li "ts,res,modify,tag,rm": remove tag from the resource.
 * @li "ts,res,modify,tag,change": change tag's data from resource.
 *
 * TextStyle is a Inwin that contain information
 * of the widgets text styles.
 */

#include <Eina.h>
#include "efl_tet.h"
#include "modal_window.h"

enum _Type_Style
{
   TAG,
   TEXTSTYLE
};

typedef enum _Type_Style Type_Style;

struct _Tag
{
   EINA_INLIST;
   const char *tag_name;
   const char *tag_value;
   Type_Style __type;
};

typedef struct _Tag Tag;

struct _Text_Styles
{
   EINA_INLIST;
   const char *style_name;
   Eina_Inlist *tags;
   Type_Style __type;
};

typedef struct _Text_Styles Text_Styles;

struct _Style_Window
{
   Evas_Object *entry_style;
   Evas_Object *entry_tag;
   Evas_Object *entry_prop;
};

typedef struct _Style_Window Style_Window;

/**
 * Show the whole inwin window by using some data
 * about loaded project (edj file)
 *
 * @param parent - the object that will be the parent of this dialog.
 * @param project -currently opened project
 *
 * @return the inwin object that contain style viewer
 *
 * @ingroup TextStyle
 */
Evas_Object *
style_viewer_add (Evas_Object *parent, Project *project);

#endif  /* STYLE_VIEWER_DIALOG_HEADER_H */
