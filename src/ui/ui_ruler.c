#include "ui_ruler.h"
#include "efl_tet.h"

struct _UI_Ruler_Data
{
	Orient			orient;
	Eina_Bool		relative : 1;
	Eina_Bool		absolute : 1;
	unsigned int	step ;
	int				start;

	Eina_List		*dashes;
	Eina_List		*marks;
	int				dash_counter;

	int				pointer_pos;
	Eina_Bool		pointer_visible;
};
typedef struct _UI_Ruler_Data UI_Ruler_Data;

char *_itoa(long n)
{
    int len = n==0 ? 1 : floor(log10l(abs(n)))+1;
    if (n<0) len++;
    char    *buf = (char*) calloc(sizeof(char), len+1);
	snprintf(buf, len, "%ld", n);
    return   buf;
}


int
_del_dashes (Evas_Object *obj, int count)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);
	Eina_List *l=eina_list_last(_ruler_data->dashes);
	Evas_Object *_line;
	int i=0;

	for (_line = eina_list_data_get(l); i<count; i++)
	{
		_line = (Evas_Object*) eina_list_data_get(l);
		if (!_line) break;
		l = eina_list_remove (l, _line);
		l = eina_list_prev(l);
		evas_object_del (_line);
	}
	return i;
}

void
_del_marks (Evas_Object *obj, int count)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);
	Eina_List *l=eina_list_last(_ruler_data->marks);
	Evas_Object *_text;
	int i=0;

	for (_text = eina_list_data_get(l); i<count; i++)
	{
		_text = (Evas_Object*) eina_list_data_get(l);
		if (!_text) break;
		l = eina_list_remove (l, _text);
		l = eina_list_prev(l);
		evas_object_del (_text);
	}
}

void
_clear_all_dashes_and_marks (Evas_Object *obj)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);

	_del_marks (obj,eina_list_count (_ruler_data->marks));
	_del_dashes (obj, eina_list_count (_ruler_data->dashes));
	_ruler_data->dash_counter=0;
	_ruler_data->marks=NULL;
	_ruler_data->dashes=NULL;
}

int
_add_dashes (Evas_Object *obj, int count, int from)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);
	int i = _ruler_data->dash_counter;
	int x, y, w, h, k;
	int iDash_size = 3;
	int iDash_from = from;
	int iPos_text_shift=20;
	Evas_Object *_line, *_text;

	Evas *_canvas = evas_object_evas_get (obj);
	evas_object_geometry_get (obj, &x, &y, &w, &h);
	for (k=0; k<=count; i++, k++)
	{
		if (!(i%5) && (i%10))
			iDash_size=7;
		else
			if (!(i%10) || !i)
			{
				iDash_size=12;
				_text = evas_object_text_add (_canvas);
				evas_object_text_font_set (_text, "Calibri",10);
				evas_object_color_set (_text, 64, 64, 64, 255);
				evas_object_text_text_set (_text, _itoa(i*100));
				if (_ruler_data->orient == HORIZONTAL)
				{
					if (i>=100) iPos_text_shift=25;
					else iPos_text_shift=20;
					evas_object_move(_text,x+i*_ruler_data->step-iPos_text_shift,
						iDash_from - iDash_size-7);
				}
				else
					evas_object_move (_text, iDash_from - iDash_size-12,
						y+i*_ruler_data->step-13);
				_ruler_data->marks = eina_list_append(_ruler_data->marks,
					_text);

				evas_object_show (_text);
			}
		_line = evas_object_line_add (_canvas);
		if (_ruler_data->orient == HORIZONTAL)
			evas_object_line_xy_set (_line,x+i*_ruler_data->step,
				iDash_from, x+i*_ruler_data->step, iDash_from - iDash_size);
		else
			evas_object_line_xy_set (_line, iDash_from, y+i*_ruler_data->step,
				iDash_from-iDash_size, y+i*_ruler_data->step);
		evas_object_color_set (_line, 64, 64, 64, 255);
		evas_object_show (_line);
		_ruler_data->dashes = eina_list_append(_ruler_data->dashes, _line);
		iDash_size=3;
	}
	return k;
}

void
_display_scale (Evas_Object *obj)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);
	Eina_List *l=_ruler_data->dashes;
	Evas_Object *_line = (Evas_Object*) eina_list_data_get(l);

	int x, y, w, h;
	int lx1, ly1, lx2, ly2;
	int iDash_counter;
	int iDash_from;
	int iTemp_dash=0;
	int iMarks_count;

	evas_object_geometry_get (obj, &x, &y, &w, &h);
	evas_object_line_xy_get (_line, &lx1, &ly1, &lx2, &ly2);

	if (_ruler_data->orient == HORIZONTAL)
	{
		iDash_counter = (int)(w/_ruler_data->step);
		iDash_from = y+h/2+3;
	}
	else
	{
		iDash_counter = (int)(h/_ruler_data->step);
		iDash_from = x+w;
	}
	if (iDash_from < 0)
		return;

	iTemp_dash=_ruler_data->dash_counter-iDash_counter;
	if (((_ruler_data->orient == HORIZONTAL) && (x!=lx1)) ||
		((_ruler_data->orient == VERTICAL) && (iDash_from!=lx1)))
		{
			_clear_all_dashes_and_marks(obj);
			iTemp_dash = -1;
		}

	if (iTemp_dash<0)
	{
		iDash_counter =iDash_counter-_ruler_data->dash_counter;
		_ruler_data->dash_counter+=_add_dashes(obj,iDash_counter,iDash_from);
	}
	else
	{
		iDash_counter =	_ruler_data->dash_counter-iDash_counter;
		_ruler_data->dash_counter -=_del_dashes(obj,iDash_counter);
		iMarks_count = eina_list_count (_ruler_data->marks);
		_del_marks (obj,(int)(iMarks_count-(_ruler_data->dash_counter/10)-1));
	}
}

void
_pointer_show (Evas_Object *obj __UNUSED__)
{
}

static void
_ruler_resize_cb (void *data __UNUSED__,
				Evas *e __UNUSED__,
				Evas_Object *obj,
				void *event_info __UNUSED__)
{
	_display_scale (obj);
}

static void
_ruler_move_cb (void *data __UNUSED__,
				Evas *e __UNUSED__,
				Evas_Object *obj,
				void *event_info __UNUSED__)
{
	_display_scale (obj);
}


void
ui_ruler_pointer_pos_set (Evas_Object *obj)
{
	double x,y, w, h;
	edje_object_part_drag_value_get (obj,"pointer_hor",&x,&y );
	edje_object_part_drag_size_get(obj,"pointer_hor",&w, &h);
//	fprintf (stdout, "x[%f], y[%f], w[%f] h[%f]   ", x,y,w,h);
}

void
ui_ruler_orient_set (Evas_Object *obj, Orient orient)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);
	_ruler_data->orient = orient;
	if (_ruler_data->orient == HORIZONTAL)
		elm_layout_file_set (obj, TET_EDJ_PATH"ui_ruler.edj",
			"ui/ruler/horizontal/default");
	else
	{
		elm_layout_file_set (obj, TET_EDJ_PATH"ui_ruler.edj",
			"ui/ruler/vertical/default");
		evas_object_event_callback_add  (obj, EVAS_CALLBACK_MOVE,
			_ruler_move_cb, _ruler_data);
	}
}

Orient
ui_ruler_orient_get (Evas_Object *obj)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);
	return	_ruler_data->orient;
}

int
ui_ruler_pointer_pos_get (Evas_Object *obj)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);
	return	_ruler_data->pointer_pos;
}

void
ui_ruler_pointer_visible_set (Evas_Object *obj, Eina_Bool visible)
{
	UI_Ruler_Data *_ruler_data=evas_object_data_get(obj, RULERDATAKEY);
	_ruler_data->pointer_visible = visible;
	_pointer_show (obj);
}

Eina_Bool
ui_ruler_pointer_visible_get (Evas_Object *obj)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);
	return	_ruler_data->pointer_visible;

}

void
ui_ruler_step_scale_set (Evas_Object *obj,unsigned int step)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj, RULERDATAKEY);
	_ruler_data->step = step;
	_display_scale (obj);
}

unsigned int
ui_ruler_step_scale_get (Evas_Object *obj)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);
	return	_ruler_data->step;
}

void
ui_ruler_type_relative_set (Evas_Object *obj, Eina_Bool enable)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);
	_ruler_data->relative = enable;
	_display_scale (obj);
}

void
ui_ruler_type_absolute_set (Evas_Object *obj, Eina_Bool enable)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);
	_ruler_data->absolute = enable;
	_display_scale (obj) ;
}

UI_Ruler_Data *
_ruler_data_init (void)
{
	UI_Ruler_Data *_ruler_data =
		(UI_Ruler_Data *)calloc (1,sizeof(UI_Ruler_Data));
	if (!_ruler_data)
		return NULL;

	_ruler_data->orient = HORIZONTAL;
	_ruler_data->relative = EINA_TRUE;
	_ruler_data->absolute = EINA_TRUE;
	_ruler_data->step = 10;
	_ruler_data->start = 0;
	_ruler_data->dash_counter = 0;
	_ruler_data->dashes = NULL;
	_ruler_data->marks = NULL;
	_ruler_data->pointer_pos = 0;
	_ruler_data->pointer_visible = EINA_TRUE;

	return _ruler_data;
}


Evas_Object *
ui_ruler_add (Evas_Object *parent)
{
	Evas_Object *ruler;
	UI_Ruler_Data *ruler_data;

	ruler_data = _ruler_data_init();
	if (!ruler_data)
	{
		ERR ("Unable initialize ruler data structure");
		return NULL;
	}

	ruler = elm_layout_add(parent);
	if (!ruler)
	{
		ERR ("Unable to create ruler");
		return NULL;
	}
	evas_object_data_set (ruler, RULERDATAKEY, ruler_data);
	evas_object_event_callback_add  (ruler, EVAS_CALLBACK_RESIZE,
		_ruler_resize_cb, ruler_data);

	ui_ruler_orient_set (ruler, HORIZONTAL);

	return ruler;
}

