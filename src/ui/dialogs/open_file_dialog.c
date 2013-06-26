#include "choose_file_dialog.h"

struct _fs_entries
{
   char *project_name;
   Evas_Object *edc;
   Evas_Object *id;
   Evas_Object *fd;
   Evas_Object *sd;
};

typedef struct _fs_entries fs_entries;

static fs_entries *fs_ent = NULL;

static void
_on_part_back(void *data, Evas_Object *obj __UNUSED__, void *event_data __UNUSED__)
{
   App_Data *ap = (App_Data *)data;

   if (ap->ws->groupspace)
     {
        ui_groupspace_unset(ap->ws->groupspace);
        ui_object_highlight_hide (ap->ws);
     }
  else
     WARN ("Groupspace object always delete");

  elm_genlist_clear(ui_block_state_list_get(ap));
}

static void
_on_state_selected(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info)
{
   Elm_Object_Item *glit = (Elm_Object_Item *)event_info;
   App_Data *ap = (App_Data *)data;
   Part_State *state;
   Part *part = NULL;
   Evas_Object *prop_view, *part_view, *state_view;

   state = elm_object_item_data_get(glit);

   prop_view = ui_block_property_get(ap);
   part_view = ui_property_part_view_get(prop_view);
   state_view = ui_prop_part_info_state_view_add(part_view, state);
   ui_prop_part_info_state_set(part_view, state_view);
   part = ui_state_list_part_get(obj);
   ui_groupspace_part_state_update(part, state, ap->ws->groupspace);
   ui_object_highlight_set(ap->ws, part->obj);
   evas_object_show(state_view);
}

static void
_on_part_selected(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_data)
{

   Evas_Object *prop, *part_prop, *gl_states;
   App_Data *ap = (App_Data *)data;
   Part *_part = (Part *) event_data;

   ap = (App_Data *)data;
   if (!_part)
     {
        ERR("Coud not get acess to part object");
        return;
     }
   prop = ui_block_property_get(ap);
   if (!prop)
     {
        ERR("Property view is missing!");
        return;
     }

   part_prop = ui_prop_part_info_view_add(prop, _part);
   ui_property_part_view_set(prop, part_prop);
   evas_object_show(part_prop);

   gl_states = ui_states_list_add(ap->win);
   ui_states_list_data_set(gl_states, _part);
   ui_block_state_list_set(ap, gl_states);
   evas_object_show(gl_states);
   evas_object_smart_callback_add(gl_states, "st,state,select",
                                  _on_state_selected, ap);

   /* FIXME: it bad */
   elm_genlist_item_selected_set(elm_genlist_first_item_get(gl_states), EINA_TRUE);

   ui_object_highlight_set(ap->ws, _part->obj);
}
static void
_on_group_clicked(void *data,
                         Evas_Object *obj __UNUSED__,
                         void *event_data)
{
   App_Data *ap = (App_Data *)data;
   Group *_group = (Group *)event_data;
   Evas_Object *gl_signals = NULL;
   Evas_Object *prop = NULL;
   Evas_Object *group_prop = NULL;
   Eina_List *signals = NULL;

   /* Get signals list of a group and show them */
   signals = wm_program_signals_list_get(_group->programs);
   gl_signals = ui_signal_list_add(ap->win);
   ui_signal_list_data_set(gl_signals, signals);
   wm_program_signals_list_free(signals);
   ui_block_signal_list_set(ap, gl_signals);

   /* group properties */
   prop = ui_block_property_get(ap);
   if (prop)
     {
        ui_prop_group_info_view_update(prop, _group);
        evas_object_show(prop);
     }
   else
     {
        prop = ui_property_view_new(ap->win);
        ui_block_property_set(ap, prop);
        evas_object_show(prop);

        group_prop = ui_prop_group_info_view_add(prop, _group);
        ui_property_group_view_set(prop, group_prop);
        evas_object_show(group_prop);
     }

   ui_groupspace_set (ap->ws, ap->project, _group);
   ui_groupspace_update (ap->ws->groupspace);
}

static void
_on_edj_done(void *data, Evas_Object *obj, void *event_info)
{
   Evas_Object *wd_list;
   App_Data *ap;
   const char *selected = event_info;
   ap = (App_Data *)data;

   if (selected)
     {
        if (eina_str_has_suffix(selected, ".edj"))
          {
             INFO("Select file: %s", selected);
             ap->project = pm_open_project_edj(selected, selected);
             wd_list = ui_widget_list_add(ap->win);
             ui_widget_list_title_set(wd_list, ap->project->name);
             ui_widget_list_data_set(wd_list, ap->project);
             ui_block_widget_list_set(ap, wd_list);

             evas_object_smart_callback_add(wd_list, "wl,group,select",
                                            _on_group_clicked, ap);

             evas_object_smart_callback_add(wd_list, "wl,part,select",
                                            _on_part_selected, ap);

             evas_object_smart_callback_add(wd_list, "wl,part,back",
                                            _on_part_back, ap);

             evas_object_show(wd_list);
             ui_panes_show(ap);
          }
        else
          /*TODO: add notify about a wrong file extension */
          ERR("The file must have a extension '.edj'");
     }
   else
     ui_panes_hide(ap);

   evas_object_hide(elm_object_parent_widget_get(obj));
   evas_object_del(obj);
}

Eina_Bool
open_edj_file(App_Data *ap)
{
   Evas_Object *fs;

   if ((!ap) || (!ap->win)) return EINA_FALSE;

   if (!ap->inwin)
     ap->inwin = elm_win_inwin_add(ap->win);

   fs = elm_fileselector_add(ap->inwin);

   evas_object_size_hint_weight_set(fs, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(fs, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_fileselector_path_set(fs, getenv("HOME"));
   elm_fileselector_buttons_ok_cancel_set(fs, EINA_TRUE);
   elm_fileselector_expandable_set(fs, EINA_FALSE);
   elm_fileselector_mode_set(fs, ELM_FILESELECTOR_LIST);
   evas_object_smart_callback_add(fs, "done", _on_edj_done, ap);

   elm_win_inwin_content_set(ap->inwin, fs);

   evas_object_show(fs);
   elm_win_inwin_activate(ap->inwin);

   return EINA_TRUE;
}

static void
_on_ok_cb(void *data,
          Evas_Object *obj __UNUSED__,
          void *event_info __UNUSED__)
{
   Evas_Object *wd_list;
   App_Data *ap;
   const char *path_edc = elm_fileselector_entry_selected_get(fs_ent->edc);
   const char *path_id = elm_fileselector_entry_selected_get(fs_ent->id);
   const char *path_sd = elm_fileselector_entry_selected_get(fs_ent->sd);
   const char *path_fd = elm_fileselector_entry_selected_get(fs_ent->fd);

   ap = (App_Data *)data;

   if (eina_str_has_suffix(path_edc, ".edc"))
     {
        INFO("Select file: %s", path_edc);
        ap->project = pm_open_project_edc(fs_ent->project_name,
                                          path_edc,
                                          path_id,
                                          path_sd,
                                          path_fd);
        wd_list = ui_widget_list_add(ap->win);
        ui_widget_list_title_set(wd_list, ap->project->name);
        ui_widget_list_data_set(wd_list, ap->project);
        ui_block_widget_list_set(ap, wd_list);
        evas_object_show(wd_list);
        ui_panes_show(ap);
     }
   else
     /*TODO: add notify about a wrong file extension */
     ERR("The file must have a extension '.edc'");

   evas_object_del(elm_object_content_get(ap->inwin));
   evas_object_hide(ap->inwin);

   free(fs_ent->project_name);
   free(fs_ent);
}

static void
_on_cancel_cb(void *data,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   Evas_Object *inwin = (Evas_Object *)data;

   evas_object_del(elm_object_content_get(inwin));
   evas_object_hide(inwin);
}

static void
_on_fs_edc_done(void *data __UNUSED__,
                  Evas_Object *obj __UNUSED__,
                  void *event_info)
{
   char *path = (char *)event_info;
   char *images, *sounds, *fonts;
   char tmp_path[PATH_MAX];
   Eina_Array *array;
   int i, j;

   array = eina_file_split(strdup(path));
   i = eina_array_count(array);
   eina_strlcpy(tmp_path, "/", sizeof("/"));
   for (j = 0; j < i - 1; j++)
     {
        eina_strlcat(tmp_path, eina_array_data_get(array, j), PATH_MAX);
        eina_strlcat(tmp_path, "/", PATH_MAX);
     }
   fs_ent->project_name = strdup(eina_array_data_get(array, i - 1));
   eina_array_free(array);

#define _create_path(target, prefix, prefix_size, suffix, suffix_size) \
   size_t size_##target = prefix_size + prefix_size + 1; \
   target = mem_malloc(size_##target * sizeof(char)); \
   eina_strlcat(target, prefix, size_##target); \
   eina_strlcat(target, suffix, size_##target);

   _create_path(images, tmp_path, strlen(tmp_path), "images/", strlen("images/"));
   elm_fileselector_entry_path_set(fs_ent->id, images);

   _create_path(sounds, tmp_path, strlen(tmp_path), "sounds/", strlen("sounds/"));
   elm_fileselector_entry_path_set(fs_ent->sd, sounds);

   _create_path(fonts, tmp_path, strlen(tmp_path), "fonts/", strlen("fonts/"));
   elm_fileselector_entry_path_set(fs_ent->fd, fonts);
#undef _create_path
}

Eina_Bool
open_edc_file(App_Data *ap)
{
   Evas_Object *box, *box_item;
   Evas_Object *label, *fs_entry;
   Evas_Object *bt;

   if ((!ap) || (!ap->win)) return EINA_FALSE;

   if (!ap->inwin)
     ap->inwin = elm_win_inwin_add(ap->win);

   if (!fs_ent)
     fs_ent = mem_malloc(sizeof(fs_entries));

   box = elm_box_add(ap->inwin);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(box);

   #define _item_add(box, label_text, button_text, isFolder, _fs) \
      box_item = elm_box_add(box); \
      evas_object_size_hint_weight_set(box_item, EVAS_HINT_EXPAND, 0.0); \
      evas_object_size_hint_align_set(box_item, EVAS_HINT_FILL, EVAS_HINT_FILL); \
      elm_box_horizontal_set(box_item, EINA_TRUE); \
      evas_object_show(box_item); \
      label = elm_label_add(box_item); \
      elm_object_text_set(label, label_text); \
      evas_object_show(label); \
      elm_box_pack_end(box_item, label); \
      fs_entry = elm_fileselector_entry_add(box_item); \
      elm_fileselector_entry_path_set(fs_entry, getenv("HOME")); \
      elm_fileselector_entry_inwin_mode_set(fs_entry, EINA_TRUE); \
      elm_fileselector_entry_folder_only_set(fs_entry, isFolder); \
      elm_object_text_set(fs_entry, button_text); \
      evas_object_size_hint_weight_set(fs_entry, EVAS_HINT_EXPAND, 0.0); \
      evas_object_size_hint_align_set(fs_entry, EVAS_HINT_FILL, EVAS_HINT_FILL); \
      elm_fileselector_entry_expandable_set(fs_entry, EINA_FALSE); \
      _fs = fs_entry; \
      evas_object_show(fs_entry); \
      elm_box_pack_end(box_item, fs_entry); \
      elm_box_pack_end(box, box_item);

   _item_add(box, "EDC:", "Select edc", EINA_FALSE, fs_ent->edc);
   evas_object_smart_callback_add(fs_entry, "file,chosen", _on_fs_edc_done, fs_ent);
   _item_add(box, "Image directory:", "Select", EINA_TRUE, fs_ent->id);
   _item_add(box, "Sound directory:", "Select", EINA_TRUE, fs_ent->sd);
   _item_add(box, "Font directory:", "Select", EINA_TRUE, fs_ent->fd);
   #undef _item_add

   box_item = elm_box_add(box);
   evas_object_size_hint_weight_set(box_item, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(box_item, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_horizontal_set(box_item, EINA_TRUE);
   evas_object_show(box_item);

   #define _button_add(box, text, func, data) \
      bt = elm_button_add(box); \
      elm_object_text_set(bt, text); \
      evas_object_smart_callback_add(bt, "clicked", func, data); \
      evas_object_show(bt); \
      elm_box_pack_end(box, bt);

   _button_add(box_item, "Ok", _on_ok_cb, ap);
   _button_add(box_item, "Cansel", _on_cancel_cb, ap->inwin);
   #undef _button_add
   elm_box_pack_end(box, box_item);

   elm_win_inwin_content_set(ap->inwin, box);
   elm_win_inwin_activate(ap->inwin);

   return EINA_TRUE;
}
