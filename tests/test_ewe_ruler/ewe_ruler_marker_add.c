
#include "test_ewe_ruler.h"

/**
 * @addtogroup ewe_ruler_test
 * @{
 * @addtogroup ewe_ruler_marker_add
 * @{
 * ewe_ruler
 * <TABLE>
 * @}
 */

/**
 * @addtogroup ewe_ruler_marker_add
 * @{
 * <tr>
 * <td>ewe_ruler_marker_add</td>
 * <td>ewe_ruler_marker_add_test_p</td>
 * <td>
 * @precondition
 * @step 1 initialize ewe library
 * @step 2 create a parent window
 * @step 3 create ruler
 *
 * @procedure
 * @step 1 create marker
 * @step 2 check returned value
 * </td>
 * <td>Evas_Object * ruler, NULL</td>
 * <td>Not NULL object returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */

EFL_START_TEST(ewe_ruler_marker_add_test_p)
{
   logger_init();
   elm_init(0, 0);
   app_init();
   Evas_Object *ruler, *parent;
   Ewe_Ruler_Marker *result;
   parent = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   ruler = ewe_ruler_add(parent);

   result = ewe_ruler_marker_add(ruler, NULL);
   ck_assert_msg(result != NULL, "Marker not returned");

   evas_object_del(parent);
   app_shutdown();
   elm_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup ewe_ruler_marker_add
 * @{
 * <tr>
 * <td>ewe_ruler_marker_add</td>
 * <td>ewe_ruler_marker_add_test_n</td>
 * <td>
 * @precondition
 * @step 1 initialize ewe library
 * @step 2 don't create ruler
 *
 * @procedure
 * @step 1 call ewe_ruler_marker_add
 * @step 2 check returned value from function
 * </td>
 * <td> NULL</td>
 * <td>NULL returned</td>
 * <td>_REAL_RESULT_</td>
 * <td>_PASSED_</td>
 * </tr>
 * @}
 */
EFL_START_TEST(ewe_ruler_marker_add_test_n)
{
   logger_init();
   elm_init(0, 0);
   app_init();
   Ewe_Ruler_Marker *result;

   result = ewe_ruler_marker_add(NULL, NULL);
   ck_assert_msg(result == NULL, "Not NULL returned");

   app_shutdown();
   elm_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup ewe_ruler_marker_add
 * @{
 * </TABLE>
 * @}
 * @}
 */
