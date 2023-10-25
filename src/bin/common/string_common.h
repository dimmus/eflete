#ifndef STRING_MACRO_H
#define STRING_MACRO_H

#include <stddef.h>
#include <Eina.h>
#include <Edje.h>

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

#define LAYOUT_NAME_REGEX "^[a-zA-Z0-9_\\.\\/-]+$"
#define NAME_REGEX "^[a-zA-Z0-9_]+$"
#define FILE_NAME_REGEX "^[a-zA-Z0-9_\\.-]+$"
#define STATE_VALUE_REGEX "^((0?(\\.[0-9]+)?|1(\\.0+)?))?$"
#define PART_NAME_REGEX "^[a-zA-Z0-9_\\.]+$"
#define DATA_NAME_REGEX ".*"
#define FONT_STYLE_REGEX "^((\\w|[- ])+(:(style|slant|weight|width|spacing|lang)=\\w+)?)?$"
#define IMAGE_BORDER_REGEX "^([0-9]+( [0-9]+){3}){0,1}?$"
#define FREQUENCY_REGEX "^([2-9][0-9]|[1-9][0-9]{2,3}|1[0-9]{4}|20000)$"

#define BANNED_SYMBOLS ":;,.'\"~!?&^%$#@()[]=+*{} |/\\№"
#define STYLE_NAME_BANNED_SYMBOLS ":;,.'`\"~!?&^%$#@()[]=+*{} |\\№"
#define EDITORS_BANNED_SYMBOLS ":;'\"~!?&^%$@()[]*{}|\\№"
#define PART_NAME_BANNED_SYMBOLS ";,'\"~!?&^%$#@()[]=+*{} |/\\№"

#define EINA_LIST_STRINGSHARE_FREE(EINA_LIST) \
   Eina_Stringshare *_data_; \
   EINA_LIST_FREE(EINA_LIST, _data_) \
   eina_stringshare_del(_data_);

int
strlen_safe(const char *str);

char *
strcpy_safe(char *dest, const char *src);

int
sort_cb(const void *data1, const void *data2);

void
state_name_split(Eina_Stringshare *name, Eina_Stringshare **name_out, double *val_out);

void
string_char_replace(char *str, char src, char rep);

char *
string_cat(const char *str1, const char *str2);

const char *
string_rstr(const char *str1, const char *str2);

char *
string_backslash_insert(const char *str, char src);

const char *
part_type_text_get(Edje_Part_Type part_type);

#endif /* STRING_MACRO_H */
