
#include "test_ewe_ruler.h"

/**
 * @addtogroup ewe_ruler_test
 * @{
 * @addtogroup ewe_ruler_marker_absolute_get
 * @{
 * ewe_ruler
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_ruler_marker_absolute_get
 * @{
 * <tr>
 * <td>ewe_ruler_marker_absolute_get</td>
 * <td>ewe_ruler_marker_absolute_get_test_p</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 create window
 * @step 3 add ewe_ruler
 * @step 4 add marker to ruler
 *
 * @procedure
 * @step 1 call ewe_ruler_marker_absolute_get
 * @step 2 check returned value
 * </td>
 * <td>Evas_Object *ruler, Ewe_Ruler_Marker* marker</td>
 * <td>0 returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFL_START_TEST(ewe_ruler_marker_absolute_get_test_p)
{
   logger_init();
   elm_init(0, 0);
   app_init();
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *ruler = ewe_ruler_add(win);
   Ewe_Ruler_Marker *marker = ewe_ruler_marker_add(ruler, NULL);

   ck_assert_msg(ewe_ruler_marker_absolute_get(ruler, marker) == 0, "Wrong default abs position value returned");

   evas_object_del(win);
   app_shutdown();
   elm_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup ewe_ruler_marker_absolute_get
 * @{
 * <tr>
 * <td>ewe_ruler_marker_absolute_get</td>
 * <td>ewe_ruler_marker_absolute_get_test_n</td>
 * <td>
 * @precondition
 * @step 1 init ewe
 * @step 2 create window
 * @step 3 add ewe_ruler
 * @step 4 add marker to ruler
 *
 * @procedure
 * @step 1 call ewe_ruler_marker_absolute_get(NULL, marker)
 * @step 2 check returned value
 * @step 3 call ewe_ruler_marker_absolute_get(ruler, NULL)
 * @step 4 check returned value
 * @step 5 call ewe_ruler_marker_absolute_get(NULL, NULL)
 * @step 6 check returned value
 * </td>
 * <td>NULL, NULL</td>
 * <td>All check passed</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFL_START_TEST(ewe_ruler_marker_absolute_get_test_n)
{
   logger_init();
   elm_init(0, 0);
   app_init();
   Evas_Object *win = elm_win_util_standard_add("test", "test");
   Evas_Object *ruler = ewe_ruler_add(win);
   Ewe_Ruler_Marker *marker = ewe_ruler_marker_add(ruler, NULL);

   ck_assert_msg(ewe_ruler_marker_absolute_get(NULL, marker) == 0, "Getted abs position from NULL ruler object");
   ck_assert_msg(ewe_ruler_marker_absolute_get(ruler, NULL) == 0, "Getted abs position for NULL marker");
   ck_assert_msg(ewe_ruler_marker_absolute_get(NULL, NULL) == 0, "Getted abs position from NULL ruler object for NULL marker");

   app_shutdown();
   elm_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup ewe_ruler_marker_absolute_get
 * @{
 * </TABLE>
 * @}
 * @}
 */
