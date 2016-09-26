/*
 * Copyright (C) 2013-2015 ChunEon Park and various contributors. Enventor.
 * That code is public domain and can be freely used or copied.
 * https://git.enlightenment.org/tools/enventor.git
 */

#ifndef SYNTAX_COLOR_H
#define SYNTAX_COLOR_H

#include "eflete.h"

#define QUOT "&quot;"
#define QUOT_C '\"'
#define QUOT_LEN 6
#define EOL "<br/>"
#define EOL_LEN 5
#define TAB "<tab/>"
#define TAB_LEN 6

typedef struct syntax_color_s color_data;

color_data *
color_init(Eina_Strbuf *strbuf);

void
color_term(color_data *cd);

const char *
color_cancel(color_data *cd, const char *str, int length, int from_pos,
             int to_pos, char **from, char **to);

char *
color_apply(color_data *cd, const char *str, int length, char *from, char *to);

Eina_Bool
color_ready(color_data *cd);

#endif /* SYNTAX_COLOR_H */
