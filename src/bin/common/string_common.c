#include "eflete.h"
#include <string.h>
#include <Eina.h>
#include <assert.h>

static int max_len = 4096;

static int 
strlcpy_safe(char *dst, const char *src, int dst_size)
{
    int i;

    /* Copy up to dst_size - 1 characters from src to dst. */
    for (i = 0; i < dst_size - 1 && src[i] != '\0'; i++) {
        dst[i] = src[i];
    }

    /* Null-terminate the destination string. */
    dst[i] = '\0';

    /* Return the number of characters in src. */
    while (src[i] != '\0') {
        i++;
    }
    return i;
}

int 
strlen_safe(const char *str)
{
   assert(str != NULL);

   char *buffer = mem_malloc(max_len + 1);
   memset(buffer, 0, max_len + 1);
   if (!buffer)
       ERR("Failed to allocate memory for a string: %s", str);

   int len = strlcpy_safe(buffer, str, max_len + 1);
   free(buffer);

   return len;
}

char * 
strcpy_safe(char *dest, const char *src) {
   assert(dest != NULL);
   assert(src != NULL);

   int dest_size = strnlen(src, max_len) + 1;
   if (dest_size == max_len + 1) {
      ERR("Source string is too large: %s", src); 
      return NULL;
   }

   if (strlcpy_safe(dest, src, dest_size) >= dest_size) {
      ERR("Destination buffer overflow occurred: %s", src);
      return NULL;
   }

   return dest;
}

int
sort_cb(const void *data1, const void *data2)
{
   if (!data1) return 1;
   if (!data2) return -1;

   if (data1 == data2)
     return 0;
   else
     return strcmp(data1, data2);
}

void
state_name_split(Eina_Stringshare *name, Eina_Stringshare **name_out, double *val_out)
{
   char **state_split;

   assert(name != NULL);
   assert(name_out != NULL || val_out != NULL);

   state_split = eina_str_split(name, " ", 2);
   if (name_out) *name_out = eina_stringshare_add(state_split[0]);
   if (val_out) *val_out = atof(state_split[1]);
   free(state_split[0]);
   free(state_split);
}

void
string_char_replace(char *str, char src, char rep)
{
   assert(str != NULL);

   while (*str != '\0')
     {
        if (*str == src) *str = rep;
        str++;
     }
}

char *
string_cat(const char *str1, const char *str2)
{
   char *string;
   string = mem_calloc(strlen_safe(str1) + strlen_safe(str2) + 1, sizeof(char));
   strcpy_safe(string, str1);
   strcat(string, str2);

   return string;
}

const char *
string_rstr(const char *str1, const char *str2)
{
   int str2len = 0;
   int i = 0, j = 0;

   str2len = strlen_safe(str2) - 1;

   for (i = strlen_safe(str1) - 1; i != 0; i--)
     {
        if (str1[i] == str2[str2len])
          {
             if (str2len == 0)
               return &str1[i];
             for (j = 1; (j < strlen_safe(str2)) & (i > j); j++)
               {
                  if (str1[i - j] != str2[str2len - j])
                    break;
                  if (j + 1 == strlen_safe(str2))
                    return &str1[i - j];
               }
          }
     }
   return NULL;
}

char *
string_backslash_insert(const char *str, char src)
{
   assert(str != NULL);
   char *dst;
   unsigned int i = 0, count = 1;
   size_t str_len = strnlen(str, max_len);

   for (i = 0; i < str_len; i++)
     if (str[i] == src)
       count++;

   dst = mem_calloc(str_len + count, sizeof(char));

   i = 0;
   while (*str != '\0')
     {
        if (*str != src)
          dst[i] = *str;
        else
          {
             dst[i++] = '\\';
             dst[i] = src;
          }
        str++;
        i++;
     }
   dst[i] = '\0';

   return dst;
}

/**
 * ref http://docs.enlightenment.org/auto/edje/group__Edje__Object__Part.html
 */
static char *part_types[] = {
     "None",
     "Rectangle",
     "Text",
     "Image",
     "Swallow",
     "Textblock",
     "Gradient",
     "Group",
     "Box",
     "Table",
     "External",
     "Proxy",
     "Spacer",
     "Mesh node",
     "Light",
     "Camera",
     "Snapshot",
     "Vector"
};
static unsigned int part_types_count = 17;

const char *
part_type_text_get(Edje_Part_Type part_type)
{
   assert(part_type <= part_types_count);

   return part_types[part_type];
}
