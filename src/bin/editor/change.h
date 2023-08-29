#ifndef CHANGE_H
#define CHANGE_H
#include "eflete.h"

struct _Change {
   Eina_Stringshare *description;
   Eina_List *diffs;
   Eina_Bool reverted : 1;
};

Change *
change_add(const char *description);

void
change_description_set(Change *change, const char *description);

void
change_free(Change *change);

void
change_diff_add(Change *change, Diff *diff);

void
change_diff_merge_add(Change *change, Diff *diff);

Eina_Bool
change_undo(Evas_Object *edit_object, Change *change);

Eina_Bool
change_redo(Evas_Object *edit_object, Change *change);
#endif /* CHANGE_H */
