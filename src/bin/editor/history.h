#ifndef NEW_HISTORY_H
#define NEW_HISTORY_H
#include "eflete.h"
#include "resource_manager2.h"

struct _History {
   Group2 *group;
   Eina_List *changes;
   Eina_List *current_change;
};
typedef struct _History History;

History *
history_add(Group2 *group);

void
history_del(History *history);

void
history_change_add(History *history, Change *change);

Eina_Bool
history_undo(History *history);

Eina_Bool
history_undo_all(History *history);

Eina_Bool
history_redo(History *history);

#endif /* NEW_HISTORY_H */
