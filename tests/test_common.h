
#include "project_manager.h"

static void
_test_end_cb(void *data __UNUSED__,
      PM_Project_Result result __UNUSED__)
{
   ecore_main_loop_quit();
}

static void
teardown(const char *path)
{
   ecore_file_recursive_rm(path);
}

static Project *
setup(const char *name)
{
   Project *pro;

   ecore_file_recursive_rm(name);
   pm_project_import_edj(name, ".", "./edj_build/"TEST_NAME".edj",
         NULL, _test_end_cb, NULL);
   ecore_main_loop_begin();

   /*
   pro = pm_project_thread_project_get();
   if (!pro)
      ck_abort_msg("Setup is failed! Project not imported");
      */

   pm_project_thread_free();
   return pro;
}
