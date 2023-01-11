#include "test_project_manager.h"

/**
 * @addtogroup project_manager
 * @{
 * @addtogroup project_manager_functional
 * @{
 * Project manager
 * <TABLE>
 * @}
 */

/**
 * @addtogroup Project_manager_functional
 * @{
 * <tr>
 * <td>project_manager</td>
 * <td>project mnager functional</td>
 * <td>
 * @precondition
 * @step 1 NONE
 *
 * @procedure
 * </td>
 * <td>NO args</td>
 * <td>All good and nice</td>
 * </tr>
 * @}
 */
EFL_START_TEST(project_manager_functional)
{
}
EFL_END_TEST

/**
 * @addtogroup logger_init
 * @{
 * </TABLE>
 * @}
 * @}
 *
 */

void project_manager_functional_test(TCase *tc)
{
   tcase_add_test(tc, project_manager_functional);
}