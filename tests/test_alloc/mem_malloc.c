
#include "test_alloc.h"

/**
 * @addtogroup alloc_test
 * @{
 * @addtogroup mem_malloc
 * @{
 * Alloc
 * <TABLE>
 * @}
 */

/**
 * @addtogroup mem_malloc
 * @{
 * <tr>
 * <td>mem_malloc</td>
 * <td>mem_malloc_test_p</td>
 * <td>
 * @precondition
 * @step 1 nothing
 *
 * @procedure
 * @step 1 call mem_malloc for 1024 ints
 * @step 2 check returned pointer
 * @step 3 check for segfault by setting 1023th int into 42.
 * </td>
 * <td>sizeof(int)*1024</td>
 * <td>All checks passed</td>
 * </tr>
 * @}
 */
EFL_START_TEST (mem_malloc_test_p)
{
   int *mm = mem_malloc(sizeof(int)*1024);
   ck_assert_msg(mm != NULL,
                 "failure: cannot return pointer to the memory block allocated");
   mm[1023] = 42;
   free(mm);
}
EFL_END_TEST

/**
 * @addtogroup mem_malloc
 * @{
 * </TABLE>
 * @}
 * @}
 */

void mem_malloc_test(TCase *tc)
{
   tcase_add_test(tc, mem_malloc_test_p);
}