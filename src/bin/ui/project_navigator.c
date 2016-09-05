/*
 * Edje Theme Editor
 * Copyright (C) 2013-2015 Samsung Electronics.
 *
 * This file is part of Edje Theme Editor.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; If not, see www.gnu.org/licenses/lgpl.html.
 */
#define EO_BETA_API
#define EFL_BETA_API_SUPPORT
#define EFL_EO_API_SUPPORT

#include "project_navigator.h"
#include "main_window.h"
#include "validator.h"
#include "project_manager.h"
#include "widget_list.h"

typedef struct
{
   int index;
   Eina_Stringshare *data;
} Combobox_Item;

typedef struct
{
   Evas_Object *layout;
   Evas_Object *genlist;
   Evas_Object *btn_add;
   Evas_Object *btn_del;
   Elm_Genlist_Item_Class *itc_group;
   Elm_Genlist_Item_Class *itc_folder;
} Project_Navigator;

typedef struct
{
   Evas_Object *box;
   Evas_Object *entry;
   Evas_Object *layout_combo;
   Evas_Object *combobox;
   Evas_Object *check;
   Elm_Genlist_Item_Class *combobox_itc;
   Combobox_Item *selected;
} Layout_Popup;

static Project_Navigator project_navigator;
static Layout_Popup layout_p;
static Resource_Name_Validator *validator = NULL;

static char *
_group_item_label_get(void *data,
                      Evas_Object *obj __UNUSED__,
                      const char *part __UNUSED__)
{
   const char *pos;
   Group *group = data;

   pos = strrchr(group->name, '/');
   if (pos) return strdup(pos + 1);
   else return strdup(group->name);
}

static char *
_folder_item_label_get(void *data,
                       Evas_Object *obj __UNUSED__,
                       const char *part __UNUSED__)
{
   const char *pos;
   char buf[BUFF_MAX];
   int symbols;
   Eina_Stringshare *prefix = data;

   symbols = strlen(prefix) - 1;
   strncpy(buf, prefix, symbols);
   buf[symbols] = '\0';
   pos = strrchr(buf, '/');
   if (pos) return strdup(pos + 1);
   else return strdup(buf);
}

static Evas_Object *
_folder_item_icon_get(void *data __UNUSED__,
                      Evas_Object *obj,
                      const char *part)
{
   Evas_Object *icon = NULL;

   if (!strcmp(part, "elm.swallow.icon"))
     {
        IMAGE_ADD_NEW(obj, icon, "icon", "folder");
     }
#ifdef HAVE_TIZEN
   if (!strcmp(part, "elm.swallow.icon1"))
     {
        IMAGE_ADD_NEW(obj, icon, "icon", "folder1");
     }
   if (!strcmp(part, "elm.swallow.icon2"))
     {
        IMAGE_ADD_NEW(obj, icon, "icon", "folder2");
     }
#endif /* HAVE_TIZEN */

   return icon;
}

static Evas_Object *
_group_item_icon_get(void *data,
                     Evas_Object *obj,
                     const char *part)
{
   Evas_Object *icon = NULL;
   Group *group = data;

   if (!strcmp(part, "elm.swallow.icon"))
     {
        if (group->main_group == NULL)
          {
             IMAGE_ADD_NEW(obj, icon, "icon", "file");
          }
        else
          {
             IMAGE_ADD_NEW(obj, icon, "icon", "alias_link");
          }
     }
#ifdef HAVE_TIZEN
   if (!strcmp(part, "elm.swallow.icon1"))
     {
        if (group->main_group == NULL)
          {
             IMAGE_ADD_NEW(obj, icon, "icon", "file1");
          }
        else
          {
             IMAGE_ADD_NEW(obj, icon, "icon", "alias_link");
          }
     }
   if (!strcmp(part, "elm.swallow.icon2"))
     {
        if (group->main_group == NULL)
          {
             IMAGE_ADD_NEW(obj, icon, "icon", "file2");
          }
        else
          {
             IMAGE_ADD_NEW(obj, icon, "icon", "alias_link");
          }
     }
#endif /* HAVE_TIZEN */

   return icon;
}

static void
_folder_item_del(void *data,
                 Evas_Object *obj __UNUSED__)
{
   Eina_Stringshare *prefix = data;
   eina_stringshare_del(prefix);
}

static void
_expand_request_cb(void *data __UNUSED__,
                   Evas_Object *o __UNUSED__,
                   void *event_info)
{
   Elm_Object_Item *glit = event_info;
   elm_genlist_item_expanded_set(glit, EINA_TRUE);
}

static void
_contract_request_cb(void *data __UNUSED__,
                     Evas_Object *o __UNUSED__,
                     void *event_info)
{
   Elm_Object_Item *glit = event_info;
   elm_genlist_item_expanded_set(glit, EINA_FALSE);
}

static void
_expanded_cb(void *data __UNUSED__,
             Evas_Object *o __UNUSED__,
             void *event_info)
{
   Group *group;
   Eina_List *folders = NULL, *groups = NULL;
   Elm_Object_Item *glit = event_info;
   Eina_Stringshare *prefix = elm_object_item_data_get(glit);

   widget_tree_items_get(ap.project->groups, prefix, &folders, &groups);
   EINA_LIST_FREE(folders, prefix)
     {
        elm_genlist_item_append(project_navigator.genlist,
                                project_navigator.itc_folder,
                                prefix,
                                glit,
                                ELM_GENLIST_ITEM_TREE,
                                NULL,
                                NULL);
     }
   EINA_LIST_FREE(groups, group)
     {
        elm_genlist_item_append(project_navigator.genlist,
                                project_navigator.itc_group,
                                group,
                                glit,
                                ELM_GENLIST_ITEM_NONE,
                                NULL,
                                NULL);
     }
}

static void
_contracted_cb(void *data __UNUSED__,
               Evas_Object *o __UNUSED__,
               void *event_info)
{
   Elm_Object_Item *glit = event_info;
   elm_genlist_item_subitems_clear(glit);
}

static void
_on_clicked_double(void *data __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *event_info)
{
   Elm_Object_Item *glit = (Elm_Object_Item *)event_info;

   if (elm_genlist_item_type_get(glit) != ELM_GENLIST_ITEM_TREE)
     {
        Group *group = (Group *)elm_object_item_data_get(glit);
        evas_object_smart_callback_call(project_navigator.layout, SIGNAL_GROUP_OPEN, group);
     }
}

static Elm_Object_Item *
_find_item(Elm_Object_Item *item, const char *name)
{
   char *item_name;

   while (item)
     {
        if (elm_genlist_item_type_get(item) != ELM_GENLIST_ITEM_TREE)
          item_name = _group_item_label_get(elm_object_item_data_get(item), NULL, NULL);
        else
          item_name = _folder_item_label_get(elm_object_item_data_get(item), NULL, NULL);

        if (!strcmp(item_name, name))
          {
             free(item_name);
             break;
          }
        free(item_name);
        item = elm_genlist_item_next_get(item);
     }
   return item;
}

static int
_items_compare(const void *data1, const void *data2)
{
   const char *str1;
   const char *str2;
   const Elm_Object_Item *it1 = data1;
   const Elm_Object_Item *it2 = data2;

   /* add group */
   if (elm_genlist_item_item_class_get(it1) == project_navigator.itc_group)
     {
        if (elm_genlist_item_item_class_get(it2) != project_navigator.itc_folder)
          str2 = ((Group *)elm_object_item_data_get(it2))->name;
        else return 1;
        str1 = ((Group *)elm_object_item_data_get(it1))->name;
     }
   else /* add folder */
     {
        if (elm_genlist_item_item_class_get(it2) == project_navigator.itc_group)
          str2 = elm_object_item_data_get(it2);
        else return -1;
        str1 = elm_object_item_data_get(it1);
     }
   return strcmp(str1, str2);
}

static void
_group_add(void *data __UNUSED__,
           Evas_Object *obj __UNUSED__,
           void *event_info)
{
   Group *group;
   Elm_Object_Item *item, *parent = NULL;
   char **arr;
   unsigned int count, i;
   Eina_Stringshare *prefix;

   group = (Group *)event_info;
   item = elm_genlist_first_item_get(project_navigator.genlist);
   arr = eina_str_split_full(group->name, "/", 0, &count);

   for (i = 0; i < count; i++)
     {
        parent = elm_genlist_item_parent_get(item);
        item = _find_item(item, arr[i]);
        if (!item) break;
        if (elm_genlist_item_item_class_get(item) != project_navigator.itc_folder) break;
        if (!elm_genlist_item_expanded_get(item)) goto exit;
        item = eina_list_data_get(elm_genlist_item_subitems_get(item));
     }

   if (i != count - 1)
     {
        prefix = widget_prefix_get(group->name, i, NULL);
        elm_genlist_item_sorted_insert(project_navigator.genlist,
                                       project_navigator.itc_folder,
                                       prefix,
                                       parent,
                                       ELM_GENLIST_ITEM_TREE,
                                       _items_compare,
                                       NULL,
                                       NULL);
     }
   else
     elm_genlist_item_sorted_insert(project_navigator.genlist,
                                    project_navigator.itc_group,
                                    group,
                                    parent,
                                    ELM_GENLIST_ITEM_NONE,
                                    _items_compare,
                                    NULL,
                                    NULL);

exit:
   free(arr[0]);
   free(arr);
}

static void
_alias_ch(void *data __UNUSED__,
          Evas_Object *obj __UNUSED__,
          void *event_info __UNUSED__)
{
   if (elm_check_state_get(layout_p.check))
     elm_layout_text_set(layout_p.layout_combo, NULL, _("Alias of"));
   else
     elm_layout_text_set(layout_p.layout_combo, NULL, _("Copy of"));
}

static void
_group_sel(void *data __UNUSED__,
           Evas_Object *obj,
           void *event_info)
{
   Combobox_Item *item = elm_object_item_data_get(event_info);
   elm_object_disabled_set(layout_p.check, (item->index != 0) ? false : true);
   layout_p.selected = item;
   elm_object_text_set(obj, item->data);
   elm_combobox_hover_end(obj);
   elm_entry_cursor_end_set(obj);
}

static void
_group_validate(void *data __UNUSED__,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   if (resource_name_validator_status_get(validator) != ELM_REG_NOERROR)
     {
       popup_buttons_disabled_set(BTN_OK, true);
       elm_object_signal_emit(obj, "validation,default,fail", "elm");
     }
   else
     {
       popup_buttons_disabled_set(BTN_OK, false);
       elm_object_signal_emit(obj, "validation,default,pass", "elm");
     }
}

Evas_Object *
_add_group_content_get(void *data __UNUSED__, Evas_Object **to_focus)
{
   Evas_Object *item;
   Group *group;
   Eina_List *l;
   Elm_Object_Item *glit;
   Combobox_Item *combobox_item;
   unsigned int i = 1;

   BOX_ADD(ap.win, layout_p.box, false, false)
   elm_box_padding_set(layout_p.box, 0, 10);
   /* name: entry */
   LAYOUT_PROP_ADD(layout_p.box, _("Name"), "popup", "1swallow")
   ENTRY_ADD(layout_p.box, layout_p.entry, true)
   evas_object_smart_callback_add(layout_p.entry, "changed", _group_validate, NULL);
   efl_event_callback_add(layout_p.entry, ELM_ENTRY_EVENT_VALIDATE, resource_name_validator_helper, validator);
   elm_layout_content_set(item, NULL, layout_p.entry);
   elm_box_pack_end(layout_p.box, item);
   glit = elm_genlist_selected_item_get(project_navigator.genlist);
   if (glit && (elm_genlist_item_item_class_get(glit) == project_navigator.itc_folder))
     elm_entry_entry_set(layout_p.entry, elm_object_item_data_get(glit));
   /* copy: combobox */
   LAYOUT_PROP_ADD(layout_p.box, _("Copy of"), "popup", "1swallow")
   layout_p.layout_combo = item;
   COMBOBOX_ADD(item, layout_p.combobox)
   evas_object_smart_callback_add(layout_p.combobox, "item,pressed", _group_sel, NULL);
   elm_layout_content_set(item, NULL, layout_p.combobox);
   elm_box_pack_end(layout_p.box, item);
   /* alias: check */
   LAYOUT_PROP_ADD(layout_p.box, _("Alias"), "popup", "1swallow")
   CHECK_ADD(item, layout_p.check)
   evas_object_smart_callback_add(layout_p.check, "changed", _alias_ch, NULL);
   elm_object_disabled_set(layout_p.check, true);
   elm_layout_content_set(item, NULL, layout_p.check);
   elm_box_pack_end(layout_p.box, item);

   /* fill the combobox */
   combobox_item = mem_malloc(sizeof(Combobox_Item));
   combobox_item->index = 0;
   combobox_item->data = eina_stringshare_add(_("None"));
   elm_genlist_item_append(layout_p.combobox, layout_p.combobox_itc,
                           combobox_item, NULL,
                           ELM_GENLIST_ITEM_NONE, NULL, NULL);
   EINA_LIST_FOREACH(ap.project->groups, l, group)
     {
        combobox_item = mem_malloc(sizeof(Combobox_Item));
        combobox_item->index = i++;
        combobox_item->data = eina_stringshare_add(group->name);
        elm_genlist_item_append(layout_p.combobox, layout_p.combobox_itc,
                                combobox_item, NULL,
                                ELM_GENLIST_ITEM_NONE, NULL, NULL);
     }
   elm_object_text_set(layout_p.combobox, _("None"));

   if (to_focus) *to_focus = layout_p.entry;
   popup_buttons_disabled_set(BTN_OK, true);

   return layout_p.box;
}

static void
_add_group_popup_close_cb(void *data __UNUSED__,
                          Evas_Object *obj __UNUSED__,
                          void *event_info)
{
   Popup_Button btn_res = (Popup_Button) event_info;

   if (BTN_OK == btn_res)
     {
        if ((!layout_p.selected) || (layout_p.selected->index == 0))
          CRIT_ON_FAIL(editor_group_add(ap.project->global_object, elm_entry_entry_get(layout_p.entry)));
        else
          {
             if (!elm_check_state_get(layout_p.check))
               CRIT_ON_FAIL(editor_group_copy(ap.project->global_object, layout_p.selected->data, elm_entry_entry_get(layout_p.entry)));
             else
               CRIT_ON_FAIL(editor_group_alias_add(ap.project->global_object, layout_p.selected->data, elm_entry_entry_get(layout_p.entry)));
          }
        gm_group_add(ap.project, elm_entry_entry_get(layout_p.entry), true);
     }

   evas_object_del(layout_p.box);
   resource_name_validator_free(validator);
   validator = NULL;
   layout_p.selected = NULL;
}


static void
_btn_add_group_cb(void *data __UNUSED__,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   Evas_Object *popup;

   if (!ap.project) return; /* when pressing ctrl + n without open project */

   assert(validator == NULL);

   validator = resource_name_validator_new(LAYOUT_NAME_REGEX, NULL);
   resource_name_validator_list_set(validator, &ap.project->groups, false);
   popup = popup_add(_("Create a new layout"), NULL, BTN_OK|BTN_CANCEL, _add_group_content_get, layout_p.entry);
   evas_object_smart_callback_add(popup, POPUP_CLOSE_CB, _add_group_popup_close_cb, NULL);
}

static void
_folder_del(const char *prefix)
{
   Eina_List *folders = NULL, *groups = NULL;
   Eina_Stringshare *tmp, *msg;
   Group *group, *alias;

   widget_tree_items_get(ap.project->groups, prefix, &folders, &groups);
   EINA_LIST_FREE(folders, tmp)
     {
       _folder_del(tmp);
     }

   EINA_LIST_FREE(groups, group)
     {
        if (group->main_group) continue;
        EINA_LIST_FREE(group->aliases, alias)
          {
             tmp = eina_stringshare_add(alias->name);
             if (editor_group_del(ap.project->global_object, tmp))
               gm_group_del(ap.project, alias);
             else
               {
                  msg = eina_stringshare_printf(_("Can't delete alias layout \"%s\""), alias->name);
                  TODO("Check if it's correct to ignore error");
                  popup_add(_("Error"), msg, BTN_OK, NULL, NULL);
                  eina_stringshare_del(msg);
               }
             eina_stringshare_del(tmp);
          }

        tmp = eina_stringshare_add(group->name);
        if (editor_group_del(ap.project->global_object, tmp))
          gm_group_del(ap.project, group);
        else
          {
             msg = eina_stringshare_printf(_("Can't delete layout \"%s\". "
                                            "Please close a tab with given group."), group->name);
             TODO("Check if it's correct to ignore error");
             popup_add(_("Error"), msg, BTN_OK, NULL, NULL);
             eina_stringshare_del(msg);
          }
        eina_stringshare_del(tmp);
     }
}

static int
group_cmp(Resource *res1, const char *name)
{
   return strncmp(res1->name, name, strlen(name));
}

static void
_group_del(void *data __UNUSED__,
           Evas_Object *obj __UNUSED__,
           void *event_info)
{
   Eina_Stringshare *group_name;
   Elm_Object_Item *item;
   char **arr;
   unsigned int depth, i;
   Eina_List *stack = NULL, *l;
   Eina_Strbuf *buf;

   buf = eina_strbuf_new();
   group_name = (Eina_Stringshare *)event_info;
   item = elm_genlist_first_item_get(project_navigator.genlist);
   arr = eina_str_split_full(group_name, "/", 0, &depth);
   for (i = 0; i < depth; i++)
     {
        item  =_find_item(item, arr[i]);
        eina_strbuf_append_printf(buf, "%s", arr[i]);
        if (!item) break;
        if (i != depth - 1)
          {
             eina_strbuf_append(buf, "/");
          }
        else
          {
             if (elm_genlist_item_type_get(item) == ELM_GENLIST_ITEM_TREE)
               item = _find_item(elm_genlist_item_next_get(item), arr[i]);
             stack = eina_list_append(stack, item);
             break;
          }
        if (!elm_genlist_item_expanded_get(item) &&
            NULL != eina_list_search_sorted_list(ap.project->groups, (Eina_Compare_Cb)group_cmp, eina_strbuf_string_get(buf)))
          break;
        stack = eina_list_append(stack, item);
        item = eina_list_data_get(elm_genlist_item_subitems_get(item));
     }
   EINA_LIST_REVERSE_FOREACH(stack, l, item)
     {
        if (0 == elm_genlist_item_subitems_count(item))
          elm_object_item_del(item);
     }

   eina_strbuf_free(buf);
   free(arr[0]);
   free(arr);
}

static void
_folder_del_popup_close_cb(void *data,
                           Evas_Object *obj __UNUSED__,
                           void *event_info)
{
   Elm_Object_Item *glit = data;
   Popup_Button btn_res = (Popup_Button) event_info;

   if (BTN_CANCEL == btn_res) return;

   _folder_del(elm_object_item_data_get(glit));
   elm_object_disabled_set(project_navigator.btn_del, true);
}

static void
_group_del_popup_close_cb(void *data,
                          Evas_Object *obj __UNUSED__,
                          void *event_info)
{
   Eina_Stringshare *tmp, *msg;
   Group *group = data;
   Popup_Button btn_res = (Popup_Button) event_info;

   if (BTN_CANCEL == btn_res) return;

   tmp = eina_stringshare_add(group->name);
   if (editor_group_del(ap.project->global_object, tmp))
     gm_group_del(ap.project, group);
   else
     {
        msg = eina_stringshare_printf(_("Can't delete layout \"%s\""), group->name);
        popup_add(_("Error"), msg, BTN_OK, NULL, NULL);
        eina_stringshare_del(msg);
        return;
     }
   eina_stringshare_del(tmp);

   elm_object_disabled_set(project_navigator.btn_del, true);
}

static void
_btn_del_group_cb(void *data __UNUSED__,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   Group *group;
   Evas_Object *popup;
   Elm_Object_Item *glit;

   glit = elm_genlist_selected_item_get(project_navigator.genlist);
   if (elm_genlist_item_type_get(glit) == ELM_GENLIST_ITEM_TREE)
     {
        popup = popup_add(_("Confirm delete layouts"),
                          _("Are you sure you want to delete the selected layouts?<br>"
                            "All aliases will be delete too."),
                          BTN_OK|BTN_CANCEL, NULL, NULL);
        evas_object_smart_callback_add(popup, POPUP_CLOSE_CB, _folder_del_popup_close_cb, glit);
     }
   else
     {
        group = (Group *)elm_object_item_data_get(glit);
        /* if Group have an object then we cann't close tab with this object
         * and delete it */
        if (group->edit_object)
          {
             popup_add(_("Warning: Delete layout"),
                       _("Cann't delete the opened layout. Please, "
                         "close the layout tab before delete it."),
                       BTN_CANCEL, NULL, NULL);
             return;
          }
        popup = popup_add(_("Confirm delete layout"),
                                    _("Are you sure you want to delete the selected layout?<br>"
                                      "All aliases will be delete too."),
                                    BTN_OK|BTN_CANCEL, NULL, NULL);
        evas_object_smart_callback_add(popup, POPUP_CLOSE_CB, _group_del_popup_close_cb, group);
     }
}

static void
_selected_cb(void *data __UNUSED__,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   elm_object_disabled_set(project_navigator.btn_del, false);
}

static void
_unselected_cb(void *data __UNUSED__,
               Evas_Object *obj __UNUSED__,
               void *event_info __UNUSED__)
{
   elm_object_disabled_set(project_navigator.btn_del, true);
}

static void
_shortcut_save_cb(void *data __UNUSED__,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   if (!ap.project) return; /* when pressing ctrl + s without open project */

   project_save();
}

static char *
_combobox_text_get(void *data, Evas_Object *obj __UNUSED__, const char *part __UNUSED__)
{
   Combobox_Item *item = (Combobox_Item *)data;
   return strdup(item->data);
}

static void
_combobox_item_del(void *data,
                   Evas_Object *obj __UNUSED__)
{
   Combobox_Item *item = (Combobox_Item *)data;
   eina_stringshare_del(item->data);
   free(item);
}

Evas_Object *
project_navigator_add(void)
{
   assert(ap.win != NULL);

   project_navigator.itc_folder = elm_genlist_item_class_new();
   project_navigator.itc_folder->item_style = "aligned";
   project_navigator.itc_folder->func.text_get = _folder_item_label_get;
   project_navigator.itc_folder->func.content_get = _folder_item_icon_get;
   project_navigator.itc_folder->func.state_get = NULL;
   project_navigator.itc_folder->func.del = _folder_item_del;

   project_navigator.itc_group = elm_genlist_item_class_new();
   project_navigator.itc_group->item_style = "aligned";
   project_navigator.itc_group->func.text_get = _group_item_label_get;
   project_navigator.itc_group->func.content_get = _group_item_icon_get;
   project_navigator.itc_group->func.state_get = NULL;
   project_navigator.itc_group->func.del = NULL;

   layout_p.combobox_itc = elm_genlist_item_class_new();
   layout_p.combobox_itc->item_style = "default";
   layout_p.combobox_itc->func.text_get = _combobox_text_get;
   layout_p.combobox_itc->func.del = _combobox_item_del;

   project_navigator.layout = elm_layout_add(ap.win);
   elm_layout_theme_set(project_navigator.layout, "layout", "navigator", "default");

#ifdef HAVE_TIZEN
   Evas_Object *icon;
   IMAGE_ADD_NEW(project_navigator.layout, icon, "navigator", "project");
   elm_object_part_content_set(project_navigator.layout, "elm.swallow.icon", icon);
#endif

   evas_object_show(project_navigator.layout);

   project_navigator.btn_add = elm_button_add(project_navigator.layout);
   evas_object_smart_callback_add(project_navigator.btn_add, "clicked", _btn_add_group_cb, NULL);
   elm_object_style_set(project_navigator.btn_add, "plus");
   elm_object_part_content_set(project_navigator.layout, "elm.swallow.btn1", project_navigator.btn_add);

   project_navigator.btn_del = elm_button_add(project_navigator.layout);
   evas_object_smart_callback_add (project_navigator.btn_del, "clicked", _btn_del_group_cb, NULL);
   elm_object_style_set(project_navigator.btn_del, "minus");
   elm_object_part_content_set(project_navigator.layout, "elm.swallow.btn0", project_navigator.btn_del);
   elm_object_disabled_set(project_navigator.btn_del, true);

   project_navigator.genlist = elm_genlist_add(project_navigator.layout);
   elm_genlist_homogeneous_set(project_navigator.genlist, true);
   evas_object_show(project_navigator.genlist);
   elm_object_content_set(project_navigator.layout, project_navigator.genlist);
   evas_object_smart_callback_add (project_navigator.genlist, "selected", _selected_cb, NULL);
   evas_object_smart_callback_add (project_navigator.genlist, "unselected", _unselected_cb, NULL);
   /*elm_genlist_tree_effect_enabled_set(project_navigator.genlist, EINA_TRUE);*/

   elm_object_text_set(project_navigator.layout, _("None"));
   elm_object_disabled_set(project_navigator.layout, true);

   evas_object_smart_callback_add(project_navigator.genlist, "clicked,double", _on_clicked_double, NULL);
   evas_object_smart_callback_add(project_navigator.genlist, "expand,request", _expand_request_cb, NULL);
   evas_object_smart_callback_add(project_navigator.genlist, "contract,request", _contract_request_cb, NULL);
   evas_object_smart_callback_add(project_navigator.genlist, "expanded", _expanded_cb, NULL);
   evas_object_smart_callback_add(project_navigator.genlist, "contracted", _contracted_cb, NULL);

   evas_object_smart_callback_add(ap.win, SIGNAL_GROUP_ADDED, _group_add, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_GROUP_DELETED, _group_del, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_ADD_GROUP, _btn_add_group_cb, NULL);
   evas_object_smart_callback_add(ap.win, SIGNAL_SHORTCUT_SAVE, _shortcut_save_cb, NULL);

   TODO("Add deletion callback and free resources");

   return project_navigator.layout;
}

void
project_navigator_project_set(void)
{
   Eina_List *folders = NULL, *groups = NULL;
   Eina_Stringshare *prefix;
   Group *group;

   elm_object_text_set(project_navigator.layout, ap.project->name);
   widget_tree_items_get(ap.project->groups, "", &folders, &groups);

   EINA_LIST_FREE(folders, prefix)
     {
        elm_genlist_item_append(project_navigator.genlist,
                                project_navigator.itc_folder,
                                prefix,
                                NULL,
                                ELM_GENLIST_ITEM_TREE,
                                NULL,
                                NULL);
     }
   EINA_LIST_FREE(groups, group)
     {
        elm_genlist_item_append(project_navigator.genlist,
                                project_navigator.itc_group,
                                group,
                                NULL,
                                ELM_GENLIST_ITEM_NONE,
                                NULL,
                                NULL);
     }

   elm_object_disabled_set(project_navigator.layout, false);
}

void
project_navigator_project_unset(void)
{
   elm_object_text_set(project_navigator.layout, _("None"));
   elm_genlist_clear(project_navigator.genlist);
   elm_object_disabled_set(project_navigator.layout, true);
}
