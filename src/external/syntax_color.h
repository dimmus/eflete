/**
 * Copyright notice for Enventor:
 *
 * Copyright (C) 2013-2014 ChunEon Park and various contributors (see AUTHORS)
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SYNTAX_COLOR_H
#define SYNTAX_COLOR_H

#include "eflete.h"

#define QUOT "\""
#define QUOT_C '\"'
#define QUOT_LEN 1
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

const char *
color_apply(color_data *cd, const char *str, int length, char *from, char *to);

Eina_Bool
color_ready(color_data *cd);

#endif /* SYNTAX_COLOR_H */
