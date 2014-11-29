#include "main_window.h"

#define FILESELCTOR_WINDOW(FUNC, TITLE, FOLDER_ONLY, FILTER, FILTER_CB, FIELD) \
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
   if ((selected) && (strcmp(selected, ""))) \
     elm_entry_entry_set(wiew->FIELD, selected); \
\
   _fs_close(NULL, wiew->fs, NULL); \
} \
\
static void \
FUNC(void *data, \
     Evas_Object *obj __UNUSED__, \
     void *event_info __UNUSED__) \
{ \
   Evas_Object *bg, *fs; \
   Wizard_Import_Edj_Win *wiew; \
   const char *path; \
\
   wiew = (Wizard_Import_Edj_Win *)data; \
\
   MODAL_WINDOW_ADD(wiew->fs, main_window_get(), TITLE, _fs_close, NULL); \
   bg = elm_bg_add(wiew->fs); \
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_show(bg); \
   elm_win_resize_object_add(wiew->fs, bg); \
   FILESELECTOR_ADD(fs, wiew->fs, FUNC##_done, data); \
   elm_fileselector_folder_only_set(fs, FOLDER_ONLY); \
   path = elm_entry_entry_get(wiew->FIELD); \
   if ((path) && (ecore_file_is_dir(path))) elm_fileselector_path_set(fs, path); \
   if (FILTER) \
     elm_fileselector_custom_filter_append(fs, FILTER_CB, NULL, "Edje Files"); \
   elm_win_resize_object_add(wiew->fs, fs); \
}

struct _Wizard_Import_Edj_Win
{
   Evas_Object *layout;

   Evas_Object *win;
   Evas_Object *splash;
   Evas_Object *name;
   Evas_Object *path;
   Evas_Object *edj;
   Evas_Object *meta_version;
   Evas_Object *meta_authors;
   Evas_Object *meta_licenses;
   Evas_Object *meta_comment;
   Evas_Object *fs;
   Project_Thread *thread;
   Splash_Cb splash_setup_func;
};

typedef struct _Wizard_Import_Edj_Win Wizard_Import_Edj_Win;

void
_fs_close(void *data, Evas_Object *obj, void *event_info);

Eina_Bool
_progress_print(void *data, Eina_Stringshare *progress_string);

void
_progress_end(void *data, PM_Project_Result result);

void
project_path_item_add(Wizard_Import_Edj_Win *wiew, const char *label, Evas_Smart_Cb cb_func);

Wizard_Import_Edj_Win *
wizard_import_common_add(const char *layout_name);

