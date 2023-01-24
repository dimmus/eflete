#include "test_ewe_ruler.h"

/**
 * @addtogroup ewe_ruler_test
 * @{
 * @addtogroup ewe_ruler_format_get
 * @{
 * ewe_ruler
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_ruler_format_get
 * @{
 * <tr>
 * <td>ewe_ruler_format_get</td>
 * <td>ewe_ruler_format_get_test_p</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 create window
 * @step 3 add ewe_ruler
 * @step 4 add scale to ruler
 *
 * @procedure
 * @step 1 call ewe_ruler_format_get
 * @step 2 check returned value
 * </td>
 * <td>Evas_Object *ruler, Ewe_Ruler_Scale* scale</td>
 * <td>Default value("%.0f") returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFL_START_TEST(ewe_ruler_format_get_test_p)
{
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *ruler = ewe_ruler_add(win);
   Ewe_Ruler_Scale *scale = ewe_ruler_scale_add(ruler, NULL);

   ck_assert_str_eq(ewe_ruler_format_get(ruler, scale), "%.0f");

   evas_object_del(win);
}
EFL_END_TEST

/**
 * @addtogroup ewe_ruler_format_get
 * @{
 * <tr>
 * <td>ewe_ruler_format_get</td>
 * <td>ewe_ruler_format_get_test_n</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 don't create ruler object
 *
 * @procedure
 * @step 1 call ewe_ruler_format_get
 * @step 2 check returned value
 * </td>
 * <td>NULL, NULL</td>
 * <td>NULL</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFL_START_TEST(ewe_ruler_format_get_test_n)
{
   ck_assert_msg(ewe_ruler_format_get(NULL, NULL) == NULL, "Getted format from NULL ruler object");
}
EFL_END_TEST

/**
 * @addtogroup ewe_ruler_format_get
 * @{
 * </TABLE>
 * @}
 * @}
 */

void ewe_ruler_format_get_test(TCase *tc)
{
   tcase_add_test(tc, ewe_ruler_format_get_test_p);
   tcase_add_test(tc, ewe_ruler_format_get_test_n);
}