
#include "test_cursor.h"
// #include "config.h"

/**
 * @addtogroup cursor_test
 * @{
 * @addtogroup cursor_main_get
 * @{
 * Cursor
 * <TABLE>
 * @}
 */

/**
 * @addtogroup cursor_main_get
 * @{
 * <tr>
 * <td>cursor_main_get</td>
 * <td>cursor_main_get_test_p1</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create window
 * @step 3 set the CURSOR_ARROW as main
 *
 * @procedure
 * @step 1 call cursor_main_get
 * </td>
 * <td>(Evas_Object *) win</td>
 * <td>(Cursor_Type) CURSOR_ARROW</td>
 * </tr>
 * @}
 */
EFL_START_TEST(cursor_main_get_test_p1)
{
   Evas_Object *win;

   // elm_init(0,0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);
   cursor_main_set(win, CURSOR_ARROW);

   ck_assert_msg(cursor_main_get(win) == CURSOR_ARROW,
                 "Main cursor return not valid cursor.");

   evas_object_del(win);
   // elm_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup cursor_main_get
 * @{
 * <tr>
 * <td>cursor_main_get</td>
 * <td>cursor_main_get_test_p2</td>
 * <td>
 * @precondition
 * @step 1 initialize elementary library
 * @step 2 create window
 *
 * @procedure
 * @step 1 call cursor_main_get
 * </td>
 * <td>(Evas_Object *) win</td>
 * <td>CURSOR_SYSTEM</td>
 * </tr>
 * @}
 */
EFL_START_TEST(cursor_main_get_test_p2)
{
   Evas_Object *win;

   // elm_init(0,0);
   win = elm_win_add(NULL, "test", ELM_WIN_BASIC);

   ck_assert_msg(cursor_main_get(win) == CURSOR_SYSTEM,
                 "Main cursor return not valid cursor.");

   evas_object_del(win);
   // elm_shutdown();
}
EFL_END_TEST

/**
 * @addtogroup cursor_main_get
 * @{
 * </TABLE>
 * @}
 * @}
 */

void cursor_main_get_test(TCase *tc)
{
   tcase_add_test(tc, cursor_main_get_test_p1);
   tcase_add_test(tc, cursor_main_get_test_p2);
}
