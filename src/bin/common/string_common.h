/*
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
 *
 * This file is part of Edje Theme Editor.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; If not, see www.gnu.org/licenses/lgpl.html.
 */

#ifndef STRING_MACRO_H
#define STRING_MACRO_H

#define GET_NAME_FROM_PATH(NAME, PATH) \
{ \
   char **arr; \
   int i; \
   arr = eina_str_split(PATH, "/", 0); \
   for (i = 0; arr[i]; i++) ; \
   NAME = strdup(arr[i-1]); \
   free(arr[0]); \
   free(arr); \
}

#define BANNED_SYMBOLS ":;,.'\"~!?&^%$#@()[]=+*{} |/\\№"
#define STYLE_NAME_BANNED_SYMBOLS ":;,.'`\"~!?&^%$#@()[]=+*{} |\\№"
#define EDITORS_BANNED_SYMBOLS ":;'\"~!?&^%$@()[]*{}|\\№"
#define PART_NAME_BANNED_SYMBOLS ";,'\"~!?&^%$#@()[]=+*{} |/\\№"

#define EINA_LIST_STRINGSHARE_FREE(EINA_LIST) \
   Eina_Stringshare *_data_; \
   EINA_LIST_FREE(EINA_LIST, _data_) \
   eina_stringshare_del(_data_);

int
sort_cb(const void *data1, const void *data2);

#endif /* STRING_MACRO_H */
