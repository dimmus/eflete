
#include "test_cursor.h"
// #include "config.h"

/**
 * @addtogroup cursor_test
 * @{
 * @addtogroup cursor_main_set
 * @{
 * Cursor
 * <TABLE>
 * @}
 */

/**
 * @addtogroup cursor_main_set
 * @{
 * <tr>
 * <td>cursor_main_set</td>
 * <td>cursor_main_set_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create window
 *
 * @procedure
 * @step 1 call cursor_main_set
 * </td>
 * <td>(Evas_Object *) win, (Cursor_Type) CURSOR_ARROW</td>
 * <td>EINA_TRUE</td>
 * </tr>
 * @}
 */
EFL_START_TEST(cursor_main_set_test_p1)
{
   Evas_Object *win;

   // elm_init(0,0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   ck_assert_msg(cursor_main_set(win, CURSOR_ARROW), "Main cursor is not setted");

   evas_object_del(win);
   // elm_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup cursor_main_set
 * @{
 * <tr>
 * <td>cursor_main_set</td>
 * <td>cursor_main_set_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create window
 *
 * @procedure
 * @step 1 call cursor_main_set
 * </td>
 * <td>NULL, (Cursor_Type) CURSOR_SYSTEM</td>
 * <td>EINA_TRUE</td>
 * </tr>
 * @}
 */
EFL_START_TEST(cursor_main_set_test_p2)
{
   Evas_Object *win;

   // elm_init(0,0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   ck_assert_msg(cursor_main_set(win, CURSOR_SYSTEM), "Main cursor is not setted");

   // elm_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup cursor_main_set
 * @{
 * </TABLE>
 * @}
 * @}
 */

void cursor_main_set_test(TCase *tc)
{
   tcase_add_test(tc, cursor_main_set_test_p1);
   tcase_add_test(tc, cursor_main_set_test_p2);
}