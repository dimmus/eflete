#ifndef ALLOC_H
#define ALLOC_H

/**
 * @defgroup Alloc Alloc
 *
 * This header describes functions used to manage dynamic storage in a project
 * PACKAGE
 */

#include "efl_tet.h"

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
void *
mem_malloc(size_t size);

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
void *
mem_calloc(size_t num, size_t size);

#endif /* ALLOC_H */
