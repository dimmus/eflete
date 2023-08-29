#ifndef PROJECT_COMMON_H
#define PROJECT_COMMON_H

Eina_Bool
exist_permission_check(const char *path, const char *name, const char *title, const char *msg, File_Save option, Ecore_Cb func, const void *data);

Eina_Bool
progress_print(void *data, Eina_Stringshare *progress_string);

void
progress_end(void *data, PM_Project_Result result, Project *project);

#endif /* PROJECT_COMMON_H */
