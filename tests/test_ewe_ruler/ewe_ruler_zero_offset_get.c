#include "test_ewe_ruler.h"

/**
 * @addtogroup ewe_ruler_test
 * @{
 * @addtogroup ewe_ruler_zero_offset_get
 * @{
 * ewe_ruler
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_ruler_zero_offset_get
 * @{
 * <tr>
 * <td>ewe_ruler_zero_offset_get</td>
 * <td>ewe_ruler_zero_offset_get_test_p</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 parent window create
 * @step 3 ewe_ruler added
 *
 * @procedure
 * @step 1 call ewe_ruler_zero_offset_get
 * @step 2 check returned value
 * </td>
 * <td>Evas_Object *ruler</td>
 * <td> default step returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFL_START_TEST(ewe_ruler_zero_offset_get_test_p)
{
   
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *ruler = ewe_ruler_add(win);

   ck_assert_msg(ewe_ruler_zero_offset_get(ruler, NULL) == 0, "Cannot get default zero offset of the ruler");

   evas_object_del(win);
}
EFL_END_TEST

/**
 * @addtogroup ewe_ruler_zero_offset_get
 * @{
 * <tr>
 * <td>ewe_ruler_zero_offset_get</td>
 * <td>ewe_ruler_zero_offset_get_test_n</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 don't create ruler object
 *
 * @procedure
 * @step 1 call ewe_ruler_zero_offset_get
 * @step 2 check returned value
 * </td>
 * <td>NULL</td>
 * <td> -1 returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFL_START_TEST(ewe_ruler_zero_offset_get_test_n)
{
   

   ck_assert_msg(ewe_ruler_zero_offset_get(NULL, NULL) == 0, "Getted zero offset of the NULL ruler object");

}
EFL_END_TEST

/**
 * @addtogroup ewe_ruler_zero_offset_get
 * @{
 * </TABLE>
 * @}
 * @}
 */

void ewe_ruler_zero_offset_get_test(TCase *tc)
{
   tcase_add_test(tc, ewe_ruler_zero_offset_get_test_p);
   tcase_add_test(tc, ewe_ruler_zero_offset_get_test_n);
}