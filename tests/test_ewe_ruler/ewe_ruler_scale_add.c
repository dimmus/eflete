#include "test_ewe_ruler.h"

/**
 * @addtogroup ewe_ruler_test
 * @{
 * @addtogroup ewe_ruler_scale_add
 * @{
 * ewe_ruler
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_ruler_scale_add
 * @{
 * <tr>
 * <td>ewe_ruler_scale_add</td>
 * <td>ewe_ruler_scale_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 create window
 * @step 3 add ewe_ruler
 *
 * @procedure
 * @step 1 call ewe_ruler_scale_add
 * @step 2 check returned value
 * </td>
 * <td>Evas_Object *ruler, const char* style</td>
 * <td>not NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFL_START_TEST(ewe_ruler_scale_add_test_p)
{
   
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *ruler = ewe_ruler_add(win);

   ck_assert_msg(ewe_ruler_scale_add(ruler, NULL) != NULL, "Cannot add scale to the ruler");

   evas_object_del(win);
}
EFL_END_TEST

/**
 * @addtogroup ewe_ruler_scale_add
 * @{
 * <tr>
 * <td>ewe_ruler_scale_add</td>
 * <td>ewe_ruler_scale_add_test_n</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 don't create ruler object
 *
 * @procedure
 * @step 1 call ewe_ruler_scale_add
 * @step 2 check returned value
 * </td>
 * <td>NULL, NULL</td>
 * <td>NULL pointer returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFL_START_TEST(ewe_ruler_scale_add_test_n)
{
   

   ck_assert_msg(ewe_ruler_scale_add(NULL, NULL) == NULL, "Added scale to NULL ruler object");

}
EFL_END_TEST

/**
 * @addtogroup ewe_ruler_scale_add
 * @{
 * </TABLE>
 * @}
 * @}
 */

void ewe_ruler_scale_add_test(TCase *tc)
{
   tcase_add_test(tc, ewe_ruler_scale_add_test_p);
   tcase_add_test(tc, ewe_ruler_scale_add_test_n);
}