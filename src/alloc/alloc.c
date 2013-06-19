#include "alloc.h"

#define TERM_MESSAGE "Could not allocate memory! Maybe not enough a memory. Application will be terminated!"

void *
mem_malloc(size_t size)
{
   void *mem_block;
   mem_block = malloc(size);
   if (!mem_block)
     {
        CRIT(TERM_MESSAGE);
        app_shutdown();
     }
   return mem_block;
}

void *
mem_calloc(size_t num, size_t size)
{
   void *mem_block;
   mem_block = calloc(num, size);
   if (!mem_block)
     {
        CRIT(TERM_MESSAGE);
        app_shutdown();
     }
   return mem_block;
}
