#ifndef UI_BLOCK_HEADER_H
#define UI_BLOCK_HEADER_H

#include "efl_tet.h"

/**
 *
 */
Evas_Object *
ui_block_add(Evas_Object *parent);

/**
 *
 */
void
ui_block_title_visible(Evas_Object *block, Eina_Bool vis);

/**
 *
 */
void
ui_block_title_text_set(Evas_Object *block, const char *title);

/**
 *
 */
const char *
ui_block_title_text_get(Evas_Object *block);

/**
 *
 */
void
ui_block_content_set (Evas_Object *block, Evas_Object *content);

/**
 *
 */
Evas_Object *
ui_block_content_get (Evas_Object *block);

/**
 *
 */
void
ui_block_content_set (Evas_Object *block, Evas_Object *content);

/**
 *
 */
Evas_Object *
ui_block_content_get (Evas_Object *block);

#endif /* UI_BLOCK_HEADER_H */
