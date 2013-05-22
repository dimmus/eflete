#include "ui_block.h"

Evas_Object *
ui_block_add(Evas_Object *parent)
{
	Evas_Object *block;

	if(!parent)
	{
		ERR("Could not create 'ui_block', because parent object is NULL.");
		return NULL;
	}

	block = elm_layout_add(parent);
	elm_layout_file_set(block, TET_EDJ, "tet/block/layout");

	return block;
}

void
ui_block_title_visible(Evas_Object *block, Eina_Bool vis)
{
	if(!block)
	{
		ERR("Could not show/hide a title, because a block is NULL.");
		return;
	}

	if(vis)
		elm_object_signal_emit(block, "title,show", "");
	else
		elm_object_signal_emit(block, "title,hide", "");
}

void
ui_block_title_text_set(Evas_Object *block, const char *title)
{
	if(!block)
	{
		ERR("Could not set title text, because a block is NULL.");
		return;
	}

	elm_object_part_text_set(block, "text.header", title);
	ui_block_title_visible(block, EINA_TRUE);
}

const char *
ui_block_title_text_get(Evas_Object *block)
{
	if(!block)
	{
		ERR("Could not nget title text, becouse a block is NULL");
		return NULL;
	}

	return elm_object_part_text_get(block, "text.header");
}

void
ui_block_content_set (Evas_Object *block, Evas_Object *content)
{
	if (!block || !content)
		return;
	elm_object_part_content_set (block, "elm.block.swallow", content);
}

Evas_Object *
ui_block_content_get (Evas_Object *block)
{
	if (!block)
		return NULL;
	return elm_object_part_content_get (block, "elm.block.swallow");
}
