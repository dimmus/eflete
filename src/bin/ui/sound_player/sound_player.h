
#ifndef SOUND_PLAYER_H
#define SOUND_PLAYER_H

#include "eflete.h"
#include "project_manager2.h"
#include "main_window.h"

void
sound_player_sound_set(Sound_Data *sound);

void
sound_player_sound_unset(void);

Evas_Object *
sound_player_add(Evas_Object *parent);

#endif /* SOUND_PLAYER_H */
