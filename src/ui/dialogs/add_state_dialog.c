#include "add_state_dialog.h"

#define STADD_LIST_KEY "state_list_key"

static Evas_Object *entry_name;
static Evas_Object *entry_value;

static void
_ok_clicked(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Evas_Object *groupspace = (Evas_Object *)data;
   Evas_Object *glist = evas_object_data_del(obj, STADD_LIST_KEY);
   Part *part = ui_state_list_part_get(glist);
   Group *group = ui_groupspace_group_get(groupspace);
   double value = 0;
   char state_name[BUFF_MAX];
   snprintf(state_name, sizeof(state_name), "%s", elm_entry_entry_get(entry_name));

   const char *str_value = elm_entry_entry_get(entry_value);

   value = atof(str_value);
   if(ui_groupspace_part_state_add(groupspace, part, state_name, value))
     ui_states_list_data_set(glist, group, part);
}

static void
_cancel_clicked(void *data,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Evas_Object *mwin = (Evas_Object *)data;
   evas_object_del(mwin);
}

Evas_Object *
new_state_dialog_add(App_Data *ap)
{
   Evas_Object *mwin;
   Evas_Object *label, *button;
   Evas_Object *main_box = NULL;
   Evas_Object *box_label, *box_entry, *box_button;
   Evas_Object *groupspace = ap->ws->groupspace;
   Evas_Object *glist = NULL;

   glist = ui_block_state_list_get(ap);
   if (!ui_state_list_part_get(glist))
     {
        NOTIFY_INFO(3, "Please select part");
        return NULL;
     }

   mwin = mw_add(ap->win);
   mw_title_set(mwin, "Add new state.");

   BOX_ADD(mwin, main_box, EINA_FALSE, EINA_TRUE);
   BOX_ADD(main_box, box_label, EINA_TRUE, EINA_FALSE);
   BOX_ADD(main_box, box_entry, EINA_TRUE, EINA_FALSE);
   BOX_ADD(main_box, box_button, EINA_TRUE, EINA_TRUE);
   elm_win_inwin_content_set(mwin, main_box);

   LABEL_ADD(box_label, label, "State name");
   elm_box_pack_end(box_label, label);
   LABEL_ADD(box_label, label, "State value");
   elm_box_pack_end(box_label, label);
   elm_box_pack_end(main_box, box_label);

   ENTRY_ADD(box_entry, entry_name, EINA_TRUE);
   elm_entry_entry_set(entry_name, "new_state");
   elm_box_pack_end(box_entry, entry_name);
   ENTRY_ADD(box_entry, entry_value, EINA_TRUE);
   elm_entry_entry_set(entry_value, "0.0");
   elm_box_pack_end(box_entry, entry_value);
   elm_box_pack_end(main_box, box_entry);


   button = elm_button_add(box_button);
   elm_object_text_set(button, "Ok");
   evas_object_data_set(button, STADD_LIST_KEY, glist);

   evas_object_smart_callback_add (button, "pressed", _ok_clicked, groupspace);
   evas_object_smart_callback_add (button, "unpressed", _cancel_clicked, mwin);
   elm_box_pack_end(box_button, button);
   evas_object_show(button);
   button = elm_button_add(box_button);
   elm_object_text_set(button, "Cancel");
   elm_box_pack_end(box_button, button);
   evas_object_smart_callback_add (button, "clicked", _cancel_clicked, mwin);
   evas_object_show(button);
   elm_box_pack_end(main_box, box_button);

   evas_object_show(main_box);

   evas_object_show(mwin);


   return mwin;
}

