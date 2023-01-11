
#ifndef HISTORY_UI_H
#define HISTORY_UI_H

#include "eflete.h"

Evas_Object *
history_ui_add(Evas_Object *parent, History *history);

void
history_ui_undo(Evas_Object *obj);

void
history_ui_redo(Evas_Object *obj);

void
history_ui_update(Evas_Object *obj);

#endif /* HISTORY_UI_H */
