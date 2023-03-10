#ifndef ALLOC_H
#define ALLOC_H

/**
 * @defgroup Alloc Alloc
 * @ingroup Eflete
 *
 * @brief
 * This module provide a memory allocate managment.
 *
 * If alloc cannot allocate the memory then that module finished the
 * application.
 */

#define TERM_MESSAGE "Could not allocate memory! Maybe not enough a memory. Application will be terminated!"
/**
 * Allocates a block of size bytes of memory, returning a pointer to the
 * beginning of the block.
 *
 * @param size Size of the memory block, in bytes. size_t is an unsigned
 * integral type.
 * @return On success, a pointer to the memory block allocated by the function.
 * If the function failed to allocate the requested block of memory, application
 * will be terminated.
 *
 * @ingroup Alloc
 */
static inline void *
mem_malloc(size_t size)
{
   void *mem_block;
   mem_block = malloc(size);
   if (!mem_block)
     {
        CRIT(TERM_MESSAGE);
        app_shutdown();
        exit(EXIT_FAILURE);
     }
   return mem_block;
}

/**
 * Allocates a block of memory for an array of num elements, each of them size
 * bytes long, and initializes all its bits to zero.
 *
 * @param num Number of elements to allocate.
 * @param size Size of each element.
 * @return On success, a pointer to the memory block allocated by the function.
 * If the function failed to allocate the requested block of memory, application
 * will be terminated.
 *
 * @ingroup Alloc
 */
static inline void *
mem_calloc(size_t num, size_t size)
{
   void *mem_block;
   mem_block = calloc(num, size);
   if (!mem_block)
     {
        CRIT(TERM_MESSAGE);
        app_shutdown();
        exit(EXIT_FAILURE);
     }
   return mem_block;
}
#undef TERM_MESSAGE
#pragma GCC poison malloc
#pragma GCC poison calloc

#endif /* ALLOC_H */
