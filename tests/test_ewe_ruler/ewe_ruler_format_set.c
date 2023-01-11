#include "test_ewe_ruler.h"

/**
 * @addtogroup ewe_ruler_test
 * @{
 * @addtogroup ewe_ruler_format_set
 * @{
 * ewe_ruler
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_ruler_format_set
 * @{
 * <tr>
 * <td>ewe_ruler_format_set</td>
 * <td>ewe_ruler_format_set_test_p</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 create window
 * @step 3 add ewe_ruler
 * @step 4 add scale to ruler
 *
 * @procedure
 * @step 1 call ewe_ruler_format_set(ruler, scale, "%f")
 * @step 2 check that returned value is EINA_TRUE
 * @step 3 check that value is setted
 * @step 4 call ewe_ruler_format_set(ruler, scale, "%.2f%%")
 * @step 5 check that returned value is EINA_TRUE
 * @step 6 check that value is setted
 * </td>
 * <td>Evas_Object *ruler, Ewe_Ruler_Scale* scale</td>
 * <td>All checks passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFL_START_TEST(ewe_ruler_format_set_test_p)
{
   logger_init();
   elm_init(0, 0);
   app_init();
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *ruler = ewe_ruler_add(win);
   Ewe_Ruler_Scale *scale = ewe_ruler_scale_add(ruler, NULL);

   ck_assert_msg(ewe_ruler_format_set(ruler, scale, "%f") == EINA_TRUE, "EINA_FALSE returned");
   ck_assert_str_eq(ewe_ruler_format_get(ruler, scale), "%f");
   ck_assert_msg(ewe_ruler_format_set(ruler, scale, "%.2f%%") == EINA_TRUE, "EINA_FALSE returned");
   ck_assert_str_eq(ewe_ruler_format_get(ruler, scale), "%.2f%%");

   evas_object_del(win);
   app_shutdown();
   elm_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup ewe_ruler_format_set
 * @{
 * <tr>
 * <td>ewe_ruler_format_set</td>
 * <td>ewe_ruler_format_set_test_n</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 don't create ruler object
 *
 * @procedure
 * @step 1 call ewe_ruler_format_set
 * @step 2 check returned value
 * </td>
 * <td>NULL, NULL</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFL_START_TEST(ewe_ruler_format_set_test_n)
{
   logger_init();
   elm_init(0, 0);
   app_init();

   ck_assert_msg(ewe_ruler_format_set(NULL, NULL, "%f") == EINA_FALSE, "Setted format for NULL ruler object");

   app_shutdown();
   elm_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup ewe_ruler_format_set
 * @{
 * </TABLE>
 * @}
 * @}
 */
