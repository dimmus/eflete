
#include "test_logger.h"

/**
 * @addtogroup logger_test
 * @{
 * @addtogroup logger_init
 * @{
 * logger
 * <TABLE>
 * @}
 */

/**
 * @addtogroup logger_init
 * @{
 * <tr>
 * <td>logger_init</td>
 * <td>logger_init_test_p</td>
 * <td>
 * @precondition
 * @step 1 NONE
 *
 * @procedure
 * @step 1 Call function logger_init
 * @step 2 Check returned value.
 * </td>
 * <td>void</td>
 * <td>Returned EINA_TRUE</td>
 * </tr>
 * @}
 */
EFL_START_TEST(logger_init_test_p)
{
   Eina_Bool ret = EINA_FALSE;

   ret = logger_init();
   ck_assert_msg(ret == EINA_TRUE, "Failed initialize logger");

}
EFL_END_TEST

/**
 * @addtogroup logger_init
 * @{
 * </TABLE>
 * @}
 * @}
 */

void logger_init_test(TCase *tc)
{
   tcase_add_test(tc, logger_init_test_p);
}