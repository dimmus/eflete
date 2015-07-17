#include "main_window.h"

#define FILESELCTOR_INWIN(FUNC, TITLE, FOLDER_ONLY, FILTER, FILTER_CB, FIELD) \
static void \
FUNC##_done(void *data, \
            Evas_Object *obj __UNUSED__, \
            void *event_info) \
{ \
   Wizard_Import_Edj_Win *wiew; \
   const char *selected; \
\
   selected = event_info; \
   wiew = (Wizard_Import_Edj_Win *)data; \
   assert(wiew != NULL); \
   if ((selected) && (strcmp(selected, ""))) \
     elm_entry_entry_set(wiew->FIELD, selected); \
   mw_del(wiew->fs); \
} \
\
static void \
FUNC(void *data, \
     Evas_Object *obj __UNUSED__, \
     void *event_info __UNUSED__) \
{ \
   Evas_Object *fs; \
   Wizard_Import_Edj_Win *wiew; \
   const char *path; \
\
   wiew = (Wizard_Import_Edj_Win *)data; \
   assert(wiew != NULL); \
\
   wiew->fs = mw_add(NULL, _fs_close, NULL); \
   assert(wiew->fs != NULL); \
   evas_object_show(wiew->fs); \
   FILESELECTOR_ADD(fs, wiew->fs, FUNC##_done, data); \
   elm_fileselector_folder_only_set(fs, FOLDER_ONLY); \
   path = elm_entry_entry_get(wiew->FIELD); \
   if ((path) && (ecore_file_is_dir(path))) elm_fileselector_path_set(fs, path); \
   if (FILTER) \
     { \
        elm_fileselector_custom_filter_append(fs, FILTER_CB, NULL, "Edje Files"); \
        elm_fileselector_mime_types_filter_append(fs, "*", "All Files"); \
     } \
   elm_win_inwin_content_set(wiew->fs, fs); \
}

struct _Wizard_Import_Edj_Win
{
   Evas_Object *layout;

   Evas_Object *win;
   Evas_Object *splash;
   Evas_Object *name;
   Elm_Validator_Regexp *name_validator;
   Evas_Object *path;
   Eina_Stringshare *tmp_dir_path;
   const char *select_file_error_msg;
   Evas_Object *edj;
   Evas_Object *meta_version;
   Evas_Object *meta_authors;
   Evas_Object *meta_licenses;
   Evas_Object *meta_comment;
   Evas_Object *fs;
   Splash_Cb splash_setup_func;

   Eina_Strbuf *progress_log;
};

typedef struct _Wizard_Import_Edj_Win Wizard_Import_Edj_Win;

void
_fs_close(void *data, Evas_Object *obj, void *event_info);

Eina_Bool
_progress_print(void *data, Eina_Stringshare *progress_string);

void
_progress_end(void *data, PM_Project_Result result);

void
project_path_item_add(Wizard_Import_Edj_Win *wiew, const char *label, const char*msg, Evas_Smart_Cb cb_func);

Wizard_Import_Edj_Win *
wizard_import_common_add(const char *layout_name);

