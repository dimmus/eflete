#ifndef PROPERTY_H
#define PROPERTY_H

#include "eflete.h"

typedef enum {
   PROPERTY_MODE_NONE,
   PROPERTY_MODE_GROUP,
   PROPERTY_MODE_COLOR_CLASS,
   PROPERTY_MODE_STYLE,
   PROPERTY_MODE_SOUND,
   PROPERTY_MODE_IMAGE,
   PROPERTY_MODE_DEMO
} Property_Mode;

Evas_Object *
property_add(Evas_Object *parent, Property_Mode mode);

#endif /* PROPERTY_H */
