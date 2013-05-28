#include "ui_groupspace.h"

void
ui_groupspace_add (App_Data *ap, Group *group __UNUSED__)
{
	Evas_Object *parent = ap->block_canvas;
	Evas_Object *_groupspace;
	Evas_Object *_part_view;

	ui_groupspace_del (ap->ws->groupspace);

	_groupspace = elm_layout_add (parent);

	elm_object_part_content_set (parent,
		"base/workspace/groupspace",_groupspace);
	elm_layout_file_set(_groupspace, TET_EDJ, "base/groupspace");

	_part_view = elm_layout_add(_groupspace);
	elm_object_part_content_set (_groupspace,
		"base/groupspace/groupspace",_part_view);

	ap->ws->groupspace = _groupspace;
}

void
ui_groupspace_del (Evas_Object *obj)
{
	if (obj)
		evas_object_del (obj);
	obj=NULL;
}
