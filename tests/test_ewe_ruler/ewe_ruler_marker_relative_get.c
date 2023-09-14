
#include "test_ewe_ruler.h"

/**
 * @addtogroup ewe_ruler_test
 * @{
 * @addtogroup ewe_ruler_marker_relative_get
 * @{
 * ewe_ruler
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_ruler_marker_relative_get
 * @{
 * <tr>
 * <td>ewe_ruler_marker_relative_get</td>
 * <td>ewe_ruler_marker_relative_get_test_p</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 create window
 * @step 3 add ewe_ruler
 * @step 4 add marker to ruler
 *
 * @procedure
 * @step 1 call ewe_ruler_marker_relative_get
 * @step 2 check returned value
 * </td>
 * <td>Evas_Object *ruler, Ewe_Ruler_Marker* marker</td>
 * <td>0 returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFL_START_TEST(ewe_ruler_marker_relative_get_test_p)
{
   
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *ruler = ewe_ruler_add(win);
   Ewe_Ruler_Marker *marker = ewe_ruler_marker_add(ruler, NULL);

   ck_assert_msg(EINA_DBL_EQ(ewe_ruler_marker_relative_get(ruler, marker, NULL), 0), "Wrong default rel position value returned");

   evas_object_del(win);
}
EFL_END_TEST

/**
 * @addtogroup ewe_ruler_marker_relative_get
 * @{
 * <tr>
 * <td>ewe_ruler_marker_relative_get</td>
 * <td>ewe_ruler_marker_relative_get_test_n</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 create window
 * @step 3 add ewe_ruler
 * @step 4 add marker to ruler
 *
 * @procedure
 * @step 1 call ewe_ruler_marker_relative_get(NULL, marker, NULL)
 * @step 2 check returned value
 * @step 3 call ewe_ruler_marker_relative_get(ruler, NULL, NULL)
 * @step 4 check returned value
 * @step 5 call ewe_ruler_marker_relative_get(NULL, NULL, NULL)
 * @step 6 check returned value
 * </td>
 * <td>NULL, NULL</td>
 * <td>All check passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFL_START_TEST(ewe_ruler_marker_relative_get_test_n)
{
   
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *ruler = ewe_ruler_add(win);
   Ewe_Ruler_Marker *marker = ewe_ruler_marker_add(ruler, NULL);

   ck_assert_msg(EINA_DBL_EQ(ewe_ruler_marker_relative_get(NULL, marker, NULL), 0), "Getted rel position from NULL ruler object");
   ck_assert_msg(EINA_DBL_EQ(ewe_ruler_marker_relative_get(ruler, NULL, NULL), 0), "Getted rel position for NULL marker");
   ck_assert_msg(EINA_DBL_EQ(ewe_ruler_marker_relative_get(NULL, NULL, NULL), 0), "Getted rel position from NULL ruler object for NULL marker");

}
EFL_END_TEST

/**
 * @addtogroup ewe_ruler_marker_relative_get
 * @{
 * </TABLE>
 * @}
 * @}
 */

void ewe_ruler_marker_relative_get_test(TCase *tc)
{
   tcase_add_test(tc, ewe_ruler_marker_relative_get_test_p);
   tcase_add_test(tc, ewe_ruler_marker_relative_get_test_n);
}