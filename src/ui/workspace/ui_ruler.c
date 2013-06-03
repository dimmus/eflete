#include "ui_ruler.h"
#include "efl_tet.h"

struct _Scale
{
	Eina_List	*marks;
	Eina_List	*dashes;
	int			dash_counter;
	double		step;
	Eina_Bool	visible;
	int			start;
	int			end;
};
typedef struct _Scale Scale;

struct _UI_Ruler_Data
{
	Orient			orient;
	Eina_Bool		visible;

	Scale			*abs_scale;
	Scale			*rel_scale;

	Evas_Object		*pointer;
	double			pointer_pos;
	Eina_Bool		pointer_visible;
};
typedef struct _UI_Ruler_Data UI_Ruler_Data;

char *_itoa(int n)
{
    int len = n==0 ? 1 : floor(log10l(abs(n)))+1;
    if (n<0)
	{
		len++;
	}
    char    *buf = (char*) calloc(sizeof(char), len+1);
	snprintf(buf, len, "%d", n);
    return   buf;
}

int
_del_dashes_absolute (Evas_Object *obj, int count)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);
	Eina_List *l=eina_list_last(_ruler_data->abs_scale->dashes);
	Evas_Object *_line;
	int i=0;

	for (_line = eina_list_data_get(l); i<count; i++)
	{
		_line = (Evas_Object*) eina_list_data_get(l);
		if (!_line)
		{
			return 0;
		}
		l = eina_list_remove (l, _line);
		l = eina_list_prev(l);
		evas_object_del (_line);
	}
	return i;
}

void
_del_marks_absolute (Evas_Object *obj, int count)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);
	Eina_List *l=eina_list_last(_ruler_data->abs_scale->marks);
	Evas_Object *_text;
	int i=0;

	for (_text = eina_list_data_get(l); i<count; i++)
	{
		_text = (Evas_Object*) eina_list_data_get(l);
		if (!_text)
		{
			return;
		}
		l = eina_list_remove (l, _text);
		l = eina_list_prev(l);
		evas_object_del (_text);
	}
}

void
_del_marks_relative (Evas_Object *obj)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);
	if (!_ruler_data->rel_scale->marks)
	{
		return;
	}
	Eina_List *l= NULL;
	Evas_Object *_mark;

	EINA_LIST_FOREACH (_ruler_data->rel_scale->dashes, l, _mark)
	{
		evas_object_del (_mark);
	}
	_ruler_data->rel_scale->dashes = NULL;
	l=NULL;
	EINA_LIST_FOREACH (_ruler_data->rel_scale->marks, l, _mark)
	{
		evas_object_del (_mark);
	}
	_ruler_data->rel_scale->marks = NULL;
}

void
_clear_all_dashes_and_marks (Evas_Object *obj)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);

	_del_marks_absolute (obj,eina_list_count  (_ruler_data->abs_scale->marks));
	_del_dashes_absolute (obj, eina_list_count(_ruler_data->abs_scale->dashes));
	_ruler_data->abs_scale->dash_counter=0;
	_ruler_data->abs_scale->marks=NULL;
	_ruler_data->abs_scale->dashes=NULL;
}

int
_add_relative_marks (Evas_Object *obj)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);
	Evas_Object *_text = NULL;
	Evas_Object *_line = NULL;
	double value = 0.0;
	double step = _ruler_data->rel_scale->step;
	int step_px=0;
	char buf[4];
	int x, y, w, h, k;

	_del_marks_relative (obj);
	if ((!_ruler_data->rel_scale->visible) || (!_ruler_data->visible))
	{
		return 0;
	}
	Evas *_canvas = evas_object_evas_get (obj);
	evas_object_geometry_get (obj, &x, &y, &w, &h);

	if ((_ruler_data->rel_scale->start!=_ruler_data->rel_scale->end) &&
		(_ruler_data->rel_scale->start<_ruler_data->rel_scale->end))
	{
		if (_ruler_data->orient == HORIZONTAL)
		{
			x = _ruler_data->rel_scale->start;
			w = _ruler_data->rel_scale->end -_ruler_data->rel_scale->start;
			step_px =(int)(w*step);
		}
		else
		{
			y = _ruler_data->rel_scale->start;
			h = _ruler_data->rel_scale->end -_ruler_data->rel_scale->start;
			step_px = (int)(h*step);
		}
	}
	else
	{
		if (_ruler_data->orient == HORIZONTAL)
		{
			step_px = (int)(w*step);
		}
		else
		{
			step_px = (int)(h*step);
		}
	}

	for (k=0; value<=1-step; value+=step, k++)
	{
		sprintf (buf, "%2.1f", value);
		_text = evas_object_text_add (_canvas);
		evas_object_text_font_set (_text, "Calibri",10);
		evas_object_color_set (_text, 53, 136, 32, 255);
		evas_object_text_text_set (_text, buf);

		if (_ruler_data->orient == HORIZONTAL)
		{
			evas_object_move(_text, x + step_px * k - 7, y - 4);
		}
		else
		{
			evas_object_move(_text, x + w - 25, y + step_px * k - 15);
		}

		_ruler_data->rel_scale->marks=
			eina_list_append(_ruler_data->rel_scale->marks,_text);
		evas_object_show (_text);

		_line = evas_object_line_add (_canvas);
		if (_ruler_data->orient == HORIZONTAL)
		{
			evas_object_line_xy_set (_line, x + k * step_px, y + h,
					x + k * step_px, y + h - 15);
		}
		else
		{
			evas_object_line_xy_set (_line, x + w, y + k * step_px,
					x + w - 15, y + k * step_px);
		}
		evas_object_color_set (_line, 53, 136, 32, 255);
		evas_object_show (_line);
		_ruler_data->rel_scale->dashes =
			eina_list_append(_ruler_data->rel_scale->dashes, _line);

	}

	_text = evas_object_text_add (_canvas);
	evas_object_text_font_set (_text, "Calibri",10);
	evas_object_color_set (_text, 53, 136, 32, 255);
	evas_object_text_text_set (_text, "1.0");

	if (_ruler_data->orient == HORIZONTAL)
	{
		evas_object_move(_text, x + step_px * k - 15, y - 3);
	}
	else
	{
		evas_object_move(_text, x + w - 25, y + step_px * k - 15);
	}

	_ruler_data->rel_scale->marks=
		eina_list_append(_ruler_data->rel_scale->marks,_text);
	evas_object_show (_text);

	_line = evas_object_line_add (_canvas);
	if (_ruler_data->orient == HORIZONTAL)
	{
		evas_object_line_xy_set (_line, x + k * step_px, y + h,
				x + k * step_px, y + h - 15);
	}
	else
	{
		evas_object_line_xy_set (_line, x + w, y + k * step_px,
				x + w - 15, y + k *step_px);
	}
	evas_object_color_set (_line, 53, 136, 32, 255);
	evas_object_show (_line);
	_ruler_data->rel_scale->dashes =
		eina_list_append(_ruler_data->rel_scale->dashes, _line);

	return k;
}

int
_add_dashes (Evas_Object *obj, int count, int from)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);
	int i = _ruler_data->abs_scale->dash_counter;
	int x, y, w, h, k;
	int _dash_size = 3;
	int _dash_from = from;
	int _pos_text_shift=10;
	Evas_Object *_line, *_text;

	Evas *_canvas = evas_object_evas_get (obj);
	evas_object_geometry_get (obj, &x, &y, &w, &h);

/*	if (_ruler_data->abs_scale->start!=0)
		if (_ruler_data->orient == HORIZONTAL)
		{
			x = _ruler_data->rel_scale->start;
			w = _ruler_data->rel_scale->end -_ruler_data->rel_scale->start;
			step_px =(int)(w*step);
		}
		else
		{
			y = _ruler_data->rel_scale->start;
			h = _ruler_data->rel_scale->end -_ruler_data->rel_scale->start;
			step_px = (int)(h*step);
		}
*/
	for (k=0; k<=count; i++, k++)
	{
		if (!(i%5) && (i%10) && i)
		{
			_dash_size=7;
		}
		else
		{
			if (!(i%10) || !i)
			{
				_dash_size=12;
				_text = evas_object_text_add (_canvas);
				evas_object_text_font_set (_text, "Calibri",10);
				evas_object_color_set (_text, 64, 64, 64, 255);
				evas_object_text_text_set (_text, _itoa(i*100));
				if (_ruler_data->orient == HORIZONTAL)
				{
					if (i >= 100)
					{
						_pos_text_shift = 26;
					}
					else
					{
						_pos_text_shift = 20;
					}
					evas_object_move(_text,
						x + i * (int)_ruler_data->abs_scale->step-_pos_text_shift,
						_dash_from - _dash_size - 5);
					if (i == 0)
					{
						evas_object_text_text_set (_text, "0");
						evas_object_move(_text, x + 2,
								_dash_from - _dash_size - 5);
					}
				}
				else
					if (i == 0)
					{
						evas_object_text_text_set (_text, "0");
						evas_object_move(_text, _dash_from - _dash_size, y - 2);
					}
					else
					{
						evas_object_move (_text, _dash_from - _dash_size - 12,
							y + i * (int)_ruler_data->abs_scale->step - 13);
					}

				_ruler_data->abs_scale->marks =
					eina_list_append(_ruler_data->abs_scale->marks,_text);

				evas_object_show (_text);
			}
		}
		_line = evas_object_line_add (_canvas);
		if (_ruler_data->orient == HORIZONTAL)
		{
			evas_object_line_xy_set (_line,
				x + i * (int)_ruler_data->abs_scale->step, _dash_from,
				x + i * (int)_ruler_data->abs_scale->step, _dash_from-_dash_size);
		}
		else
		{
			evas_object_line_xy_set (_line,
				_dash_from,	y + i * (int)_ruler_data->abs_scale->step,
				_dash_from - _dash_size, y + i * (int)_ruler_data->abs_scale->step);
		}
		evas_object_color_set (_line, 64, 64, 64, 255);
		evas_object_show (_line);
		_ruler_data->abs_scale->dashes =
			eina_list_append(_ruler_data->abs_scale->dashes, _line);
		_dash_size = 3;
	}
	return k;
}

void
_display_scale (Evas_Object *obj)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);
	Eina_List *l=_ruler_data->abs_scale->dashes;
	Evas_Object *_line = (Evas_Object*) eina_list_data_get(l);

	int x, y, w, h;
	int lx1, ly1, lx2, ly2;
	int dash_counter;
	int dash_from;
	int temp_dash=0;

	evas_object_geometry_get (obj, &x, &y, &w, &h);
	evas_object_line_xy_get (_line, &lx1, &ly1, &lx2, &ly2);

	if (_ruler_data->orient == HORIZONTAL)
	{
		dash_counter = (int)(w/_ruler_data->abs_scale->step);
		dash_from = y+h;
	}
	else
	{
		dash_counter = (int)(h/_ruler_data->abs_scale->step);
		dash_from = x+w;
	}
	if (dash_from < 0)
	{
		return;
	}
	_add_relative_marks(obj);
	if ((!_ruler_data->abs_scale->visible) || (!_ruler_data->visible))
	{
		_clear_all_dashes_and_marks(obj);
		return;
	}


	temp_dash=_ruler_data->abs_scale->dash_counter-dash_counter;
	if (((_ruler_data->orient == HORIZONTAL) && (x!=lx1)) ||
		((_ruler_data->orient == VERTICAL) && (dash_from!=lx1)))
		{
			_clear_all_dashes_and_marks(obj);
			temp_dash = -1;
		}

	if (temp_dash<0)
	{
		dash_counter =dash_counter-_ruler_data->abs_scale->dash_counter;
		_ruler_data->abs_scale->dash_counter+=_add_dashes(obj,dash_counter,
			dash_from);
	}
	else
	{
		int marks_count;
		dash_counter =	_ruler_data->abs_scale->dash_counter-dash_counter;
		_ruler_data->abs_scale->dash_counter-=
			_del_dashes_absolute (obj,dash_counter);
		marks_count = eina_list_count (_ruler_data->abs_scale->marks);
		_del_marks_absolute (obj,
			(int)(marks_count-(_ruler_data->abs_scale->dash_counter/10)-1));
	}
}

UI_Ruler_Data *
_ruler_data_init (void)
{
	UI_Ruler_Data *_ruler_data =
		(UI_Ruler_Data *)calloc (1,sizeof(UI_Ruler_Data));

	if (!_ruler_data)
	{
		return NULL;
	}
	_ruler_data->orient = HORIZONTAL;
	_ruler_data->visible = EINA_TRUE;

	_ruler_data->abs_scale = (Scale*)calloc (1,sizeof(Scale));
	_ruler_data->abs_scale->step = 10.0;
	_ruler_data->abs_scale->dash_counter = 0;
	_ruler_data->abs_scale->dashes = NULL;
	_ruler_data->abs_scale->marks = NULL;
	_ruler_data->abs_scale->visible = EINA_TRUE;
	_ruler_data->abs_scale->start = 0;
	_ruler_data->abs_scale->end = 0;

	_ruler_data->rel_scale = (Scale*)calloc (1,sizeof(Scale));
	_ruler_data->rel_scale->step = 0.5;
	_ruler_data->rel_scale->dash_counter = 0;
	_ruler_data->rel_scale->dashes = NULL;
	_ruler_data->rel_scale->marks = NULL;
	_ruler_data->rel_scale->visible = EINA_FALSE;
	_ruler_data->rel_scale->start = 0;
	_ruler_data->rel_scale->end = 0;

	_ruler_data->pointer = NULL;
	_ruler_data->pointer_pos = 0.0;
	_ruler_data->pointer_visible = EINA_TRUE;
	return _ruler_data;
}

/* TODO: make show method for pointer*/
void
_pointer_show (Evas_Object *obj __UNUSED__)
{
}

/* TODO: make hide method for pointer*/
void
_pointer_hide (Evas_Object *obj __UNUSED__)
{
}


static void
_ruler_resize_cb (void *data __UNUSED__,
				Evas *e __UNUSED__,
				Evas_Object *obj,
				void *event_info __UNUSED__)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);
	if (_ruler_data->visible)
	{
		_display_scale (obj);
	}
}

static void
_ruler_move_cb (void *data __UNUSED__,
				Evas *e __UNUSED__,
				Evas_Object *obj,
				void *event_info __UNUSED__)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);
	if (_ruler_data->visible)
	{
		_display_scale (obj);
	}
}

/* TODO: make correct return value at absolute coordinates*/
int
ui_ruler_pointer_pos_get (Evas_Object *obj)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);
	return	_ruler_data->pointer_pos;
}

void
ui_ruler_pointer_pos_set (Evas_Object *obj, int pos)
{
	int x,y,w,h;
	double dx,dy;
	dx=dy=0.0;
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);
	evas_object_geometry_get (obj, &x, &y, &w, &h);

	if (_ruler_data->orient == HORIZONTAL)
	{
		dx =(double) (pos - x)/w;
		_ruler_data->pointer_pos=dx;
	}
	else
	{
		dy = (double) (pos - y)/h;
		_ruler_data->pointer_pos=dy;
	}
	edje_object_part_drag_value_set (elm_layout_edje_get(obj),"pointer",dx,dy);
}

Orient
ui_ruler_orient_get (Evas_Object *obj)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);
	return	_ruler_data->orient;
}

void
ui_ruler_orient_set (Evas_Object *obj, Orient orient)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);
	_ruler_data->orient = orient;
	if (_ruler_data->orient == HORIZONTAL)
	{
		elm_layout_file_set (obj, TET_EDJ,
			"ui/ruler/horizontal/default");
			edje_object_file_set (obj, TET_EDJ, "ui/ruler/horizontal/default");
	}
	else
	{
		elm_layout_file_set (obj, TET_EDJ,	"ui/ruler/vertical/default");
		evas_object_event_callback_add  (obj, EVAS_CALLBACK_MOVE,
			_ruler_move_cb, _ruler_data);
	}
}

void
ui_ruler_pointer_visible_set (Evas_Object *obj, Eina_Bool visible)
{
	UI_Ruler_Data *_ruler_data=evas_object_data_get(obj, RULERDATAKEY);
	_ruler_data->pointer_visible = visible;
	if (visible)
	{
		_pointer_show (obj);
	}
	else
	{
		_pointer_hide (obj);
	}
}

Eina_Bool
ui_ruler_pointer_visible_get (Evas_Object *obj)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);
	return	_ruler_data->pointer_visible;
}

void
ui_ruler_step_scale_absolute_set (Evas_Object *obj,int step)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj, RULERDATAKEY);
	_ruler_data->abs_scale->step = (double)step;
	_display_scale (obj);
}

unsigned int
ui_ruler_step_absolute_get (Evas_Object *obj)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);
	return	(int)_ruler_data->abs_scale->step;
}

void
ui_ruler_step_relative_set (Evas_Object *obj,double step)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj, RULERDATAKEY);
	_ruler_data->rel_scale->step = step;
	_display_scale (obj);
}

double
ui_ruler_step_relative_get (Evas_Object *obj)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);
	return	_ruler_data->rel_scale->step;
}

void
ui_ruler_scale_relative_visible_set (Evas_Object *obj, Eina_Bool enable)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);
	_ruler_data->rel_scale->visible = enable;
	_display_scale (obj);
}

Eina_Bool
ui_ruler_scale_relative_visible_get (Evas_Object *obj)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);
	 return _ruler_data->rel_scale->visible;
}

void
ui_ruler_scale_absolute_visible_set (Evas_Object *obj, Eina_Bool enable)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);
	_ruler_data->abs_scale->visible = enable;
	_display_scale (obj) ;
}

Eina_Bool
ui_ruler_scale_absolute_visible_get (Evas_Object *obj)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);
	return _ruler_data->abs_scale->visible;
}

void
ui_ruler_scale_relative_position_set (Evas_Object *obj, int start, int end)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);
	_ruler_data->rel_scale->start = start;
	_ruler_data->rel_scale->end = end;
//	fprintf (stdout, "DEBAG: position start [%d] end[%d]\n", start, end);
	_display_scale(obj);
}

void
ui_ruler_scale_relative_position_get (Evas_Object *obj, int *start, int *end)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);
	*start = _ruler_data->rel_scale->start;
	*end = _ruler_data->rel_scale->end;
}

void
ui_ruler_scale_absolute_position_zero_set (Evas_Object *obj, int pos)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);
	_ruler_data->abs_scale->start = pos;
	//fprintf (stdout, "DEBAG: position zero [%d]\n", pos);
	_display_scale(obj);
}

int
ui_ruler_scale_absolute_position_zero_get (Evas_Object *obj)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);
	return (int)_ruler_data->abs_scale->start;
}

void
ui_ruler_hide (Evas_Object *obj)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);
	_ruler_data->visible = EINA_FALSE;
	_clear_all_dashes_and_marks(obj);
	evas_object_hide (obj);
}

void
ui_ruler_show (Evas_Object *obj)
{
	UI_Ruler_Data *_ruler_data = evas_object_data_get(obj,RULERDATAKEY);
	_ruler_data->visible = EINA_TRUE;
	_display_scale(obj);
	evas_object_show (obj);
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
		free(ruler_data);
		return NULL;
	}
	evas_object_data_set (ruler, RULERDATAKEY, ruler_data);
	evas_object_event_callback_add  (ruler, EVAS_CALLBACK_RESIZE,
		_ruler_resize_cb, ruler_data);
	ui_ruler_orient_set (ruler, HORIZONTAL);

	return ruler;
}
