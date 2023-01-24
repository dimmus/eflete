#include "test_ewe_ruler.h"

/**
 * @addtogroup ewe_ruler_test
 * @{
 * @addtogroup ewe_ruler_horizontal_set
 * @{
 * ewe_ruler
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_ruler_horizontal_set
 * @{
 * <tr>
 * <td>ewe_ruler_horizontal_set</td>
 * <td>ewe_ruler_horizontal_set_test_p1</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 parent window create
 * @step 3 ewe_ruler added
 *
 * @procedure
 * @step 1 call ewe_ruler_horizontal_set
 * @step 2 check returned value
 * </td>
 * <td>(Evas_Object *) ruler, EINA_TRUE</td>
 * <td>EINA_FALSE</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFL_START_TEST(ewe_ruler_horizontal_set_test_p1)
{
   
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *ruler = ewe_ruler_add(win);

   ck_assert_msg(ewe_ruler_horizontal_set(ruler, EINA_TRUE) == EINA_FALSE,
                 "Resetted default value");

   evas_object_del(win);
}
EFL_END_TEST

/**
 * @addtogroup ewe_ruler_horizontal_set
 * @{
 * <tr>
 * <td>ewe_ruler_horizontal_set</td>
 * <td>ewe_ruler_horizontal_set_test_p2</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 parent window create
 * @step 3 ewe_ruler added
 *
 * @procedure
 * @step 1 call ewe_ruler_horizontal_set
 * @step 2 check returned value
 * @step 3 compare setted and getted horizontal values
 * </td>
 * <td>(Evas_Object *) ruler, EINA_FALSE</td>
 * <td>All check passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFL_START_TEST(ewe_ruler_horizontal_set_test_p2)
{
   
   Eina_Bool get_horizontal;
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *ruler = ewe_ruler_add(win);

   ck_assert_msg(ewe_ruler_horizontal_set(ruler, EINA_FALSE) == EINA_TRUE,
                 "Cannot set vertical orientation of the ruler");
   get_horizontal = ewe_ruler_horizontal_get(ruler);
   ck_assert_msg(get_horizontal == EINA_FALSE,
                 "Getted and setted value are not equal");

   evas_object_del(win);
}
EFL_END_TEST

/**
 * @addtogroup ewe_ruler_horizontal_set
 * @{
 * <tr>
 * <td>ewe_ruler_horizontal_set</td>
 * <td>ewe_ruler_horizontal_set_test_n</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 don't create ruler object
 *
 * @procedure
 * @step 1 call ewe_ruler_horizontal_set
 * @step 2 check returned value
 * </td>
 * <td>NULL, EINA_FALSE</td>
 * <td>EINA_FALSE returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFL_START_TEST(ewe_ruler_horizontal_set_test_n)
{
   

   ck_assert_msg(ewe_ruler_horizontal_set(NULL, EINA_FALSE) == EINA_FALSE,
                 "Setted horizontal orientation to the NULL ruler object");

}
EFL_END_TEST
/**
 * @addtogroup ewe_ruler_horizontal_set
 * @{
 * </TABLE>
 * @}
 * @}
 */

void ewe_ruler_horizontal_set_test(TCase *tc)
{
   tcase_add_test(tc, ewe_ruler_horizontal_set_test_p1);
   tcase_add_test(tc, ewe_ruler_horizontal_set_test_p2);
   tcase_add_test(tc, ewe_ruler_horizontal_set_test_n);
}