#include "main_window.h"
#include "project_manager2.h"

typedef struct {
   Eina_Strbuf *buf, *buf_msg;
   char *name;
   const char *title;
   char *path;
   Ecore_Cb func;
   const void *data;
} Permission_Check_Data;

static void _permision_popup_job(void *data)
{
   Permission_Check_Data *pcd = data;
   Eina_Strbuf *buf_msg;

   buf_msg = eina_strbuf_new();
   eina_strbuf_append_printf(buf_msg, _("Haven't permision to overwrite '%s' in '%s'"), pcd->name, pcd->path);
   popup_add(pcd->title, eina_strbuf_string_get(buf_msg), BTN_OK, NULL, NULL);

   eina_strbuf_free(buf_msg);
   eina_strbuf_free(pcd->buf);
   free(pcd->name);
   free(pcd);
}

static void
_exist_permission_popup_close_cb(void *data, Evas_Object *obj __UNUSED__, void *event_info)
{
   Popup_Button btn_res = (Popup_Button) event_info;
   Permission_Check_Data *pcd = data;

   if (btn_res == BTN_CANCEL) goto end;
   if (!ecore_file_can_write(eina_strbuf_string_get(pcd->buf)))
     {
        ecore_job_add(_permision_popup_job, pcd);
        return;
     }
   if (btn_res == BTN_REPLACE)
     ecore_file_recursive_rm(eina_strbuf_string_get(pcd->buf));
   if (pcd->func)
      pcd->func((void *)pcd->data);

end:
   eina_strbuf_free(pcd->buf);
   free(pcd->name);
   free(pcd);
}

Eina_Bool
exist_permission_check(const char *path, const char *name,
                       const char *title, const char *msg, File_Save option,
                       Ecore_Cb func,
                       const void *data)
{
   Evas_Object *popup;
   Eina_Strbuf *buf_msg;
   Permission_Check_Data *pcd = mem_calloc(1, sizeof(Permission_Check_Data));
   Eina_Bool ret = true;

   assert(path != NULL);
   assert(name != NULL);
   assert(title != NULL);

   pcd->name = strdup(name);
   pcd->title = title;
   pcd->path = strdup(path);
   pcd->func = func;
   pcd->data = data;
   /* we alwayes imported and exported project to folder by given path, means
    * that we alwayes create a new folder for project or exported source.
    * So need to check there is the folder "path/name" */
   if (!ecore_file_can_write(path))
     {
        buf_msg = eina_strbuf_new();
        eina_strbuf_append_printf(buf_msg, _("Haven't permision to write '%s'"), path);
        popup_add(title, eina_strbuf_string_get(buf_msg), BTN_OK, NULL, NULL);
        eina_strbuf_free(buf_msg);
        ret = false;
        goto exit;
     }
   pcd->buf = eina_strbuf_new();
   eina_strbuf_append_printf(pcd->buf, "%s/%s", path, name);
   if (!ecore_file_exists(eina_strbuf_string_get(pcd->buf)))
     {
        if (pcd->func)
          pcd->func((void *)pcd->data);
        eina_strbuf_free(pcd->buf);
        goto exit;
     }

   if (option == FILE_SAVE_ASK)
     {
        popup = popup_add(title, msg, BTN_REPLACE | BTN_CANCEL, NULL, NULL);
        evas_object_smart_callback_add(popup, POPUP_CLOSE_CB, _exist_permission_popup_close_cb, pcd);
        return ret;
     }
   else
     {
        if (!ecore_file_can_write(eina_strbuf_string_get(pcd->buf)))
          {
             ecore_job_add(_permision_popup_job, pcd);
             ret = false;
             goto exit;
          }

        if (option == FILE_SAVE_REPLACE)
             ecore_file_recursive_rm(eina_strbuf_string_get(pcd->buf));
        if (pcd->func)
          pcd->func((void *)pcd->data);
     }

exit:
   free(pcd->name);
   free(pcd->path);
   free(pcd);
   return ret;
}

Eina_Bool
progress_print(void *data __UNUSED__, Eina_Stringshare *progress_string)
{
   elm_object_part_text_set(ap.splash, "label.info", progress_string);
   return true;
}

void
progress_end(void *data __UNUSED__, PM_Project_Result result, Project *project __UNUSED__)
{
   char buf[PATH_MAX];
   switch (result)
     {
      case PM_PROJECT_SUCCESS:
        {
           INFO(_("Project '%s' is opened."), ap.project->name);
           STATUSBAR_PROJECT_PATH(ap.project->pro_path);
           STATUSBAR_PROJECT_SAVE_TIME_UPDATE();
           break;
        }
      case PM_PROJECT_CANCEL:
      case PM_PROJECT_ERROR:
      case PM_PROJECT_LOCKED:
      case PM_PROJECT_CREATE_PRO_FAILED:
      case PM_PROJECT_OPEN_PRO_FAILED:
      case PM_PROJECT_READ_PRO_FAILED:
      case PM_PROJECT_WRITE_PRO_FAILED:
      case PM_PROJECT_ADD_SPEC_GROUP_FAILED:
      case PM_PROJECT_ADD_SPEC_IMAGE_FAILED:
      case PM_PROJECT_ADD_SPEC_SAMPLE_FAILED:
      case PM_PROJECT_LOAD_INTERNAL_OBJ_FAILED:
      case PM_PROJECT_LOAD_GROUP_FAILED:
      case PM_PROJECT_COPY_GROUP_FAILED:
      case PM_PROJECT_COPY_FILE_FAILED:
      case PM_PROJECT_EXPORT_CREATE_PATH_FAILED:
      case PM_PROJECT_EXPORT_COPY_FILE_FAILED:
      case PM_PROJECT_EXPORT_SAVE_IMAGE_FAILED:
      case PM_PROJECT_EXPORT_WRONG_IMAGE_ID:
      case PM_PROJECT_EXPORT_CREATE_FILE_FAILED:
      case PM_PROJECT_EXPORT_SAVE_SAMPLE_FAILED:
      case PM_PROJECT_EXPORT_READ_EDJ_FONT_FAILED:
      case PM_PROJECT_EXPORT_SAVE_FONT_FAILED:
      case PM_PROJECT_EXPORT_CREATE_OBJECT_FAILED:
      case PM_PROJECT_EXPORT_DEVELOP_EDJ_FAILED:
      case PM_PROJECT_EXPORT_RELEASE_EDJ_FAILED:
      case PM_PROJECT_BUILD_SOURCE_EDC_FAILED:
      case PM_PROJECT_LAST:
      default:
        {
           snprintf(buf, sizeof(buf), "Warning! %s", pm_project_result_string_get(result));
           popup_add(_("Project progress"), buf, BTN_CANCEL, NULL, NULL);
        }
     }
TODO("Don't forgot about Enventor")
#ifdef HAVE_ENVENTOR
#endif /* HAVE_ENVENTOR */

   splash_del(ap.splash);
   ap.splash = NULL;
}
