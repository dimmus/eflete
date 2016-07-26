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

#include "tabs_private.h"
#include "tabs.h"
#include "main_window.h"
#include "project_common.h"
#include "widget_list.h"
#include "config.h"

Eina_List *widget_list = NULL;
const char *prev_edj_path = NULL;

struct _Tab_Home_Edj
{
   Evas_Object *layout;
   Evas_Object *btn_create;
   Elm_Validator_Regexp *name_validator;

   Evas_Object *name;
   Evas_Object *path;
   Evas_Object *edj;
   Meta_Data_Controls meta;

   Evas_Object *ch_all;
   Evas_Object *themes;
   Elm_Genlist_Item_Class *itc;
   Evas_Object *genlist;
};

struct _Node
{
   Eina_Stringshare *name;
   Eina_Bool check;
   Eina_List *list; /* if list is NULL then it is group in the tree */
};

typedef struct _Tab_Home_Edj Tab_Home_Edj;
typedef struct _Node Node;

static Tab_Home_Edj tab_edj;
static Elm_Genlist_Item_Class *itc = NULL;

static char *
_genlist_label_get(void *data, Evas_Object *obj, const char  *part);

static Evas_Object *
_genlist_content_get(void *data, Evas_Object *obj, const char *part);

static void
_tree_nodes_get(Eina_List *groups_list, Node *node);

static Eina_Bool
_checked_get(void)
{
   Tree_Item_Data *widget = NULL, *style = NULL;
   End_Item_Data *item_style;
   Eina_List *l, *ll, *lll;

   EINA_LIST_FOREACH(widget_list, l, widget)
     {
        if (widget->check)
          return EINA_TRUE;

        EINA_LIST_FOREACH(widget->list, ll, style)
          {
             if (style->check)
               return EINA_TRUE;

             EINA_LIST_FOREACH(style->list, lll, item_style)
               {
                  if (item_style->check)
                    return EINA_TRUE;
               }
          }
     }
   return EINA_FALSE;
}

static void
_validate()
{
   if ((elm_validator_regexp_status_get(tab_edj.name_validator) != ELM_REG_NOERROR) ||
       !eina_str_has_extension(elm_entry_entry_get(tab_edj.edj), ".edj") ||
       !ecore_file_exists(elm_entry_entry_get(tab_edj.edj)) ||
       (widget_list && !_checked_get()))
     elm_object_disabled_set(tab_edj.btn_create, true);
   else
     elm_object_disabled_set(tab_edj.btn_create, false);
}

static void
_name_changed_cb(void *data __UNUSED__,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   _validate();
}

static void
_tree_nodes_get(Eina_List *groups_list, Node *node)
{
   Eina_List *folders = NULL, *groups = NULL;
   Eina_Stringshare *prefix;
   Node *n;
   Group *group;

   widget_tree_items_get(groups_list, node->name, &folders, &groups);
   EINA_LIST_FREE(folders, prefix)
     {
        n = mem_calloc(1, sizeof(Node));
        n->name = eina_stringshare_add(prefix);
        node->list = eina_list_append(node->list, n);
        _tree_nodes_get(groups_list, n);
     }
   EINA_LIST_FREE(groups, group)
     {
        n = mem_calloc(1, sizeof(Node));
        n->name = eina_stringshare_add(group->name);
        node->list = eina_list_append(node->list, n);
     }
}

static void
_edj_changed_cb(void *data __UNUSED__,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   Eina_List *collections, *l, *groups_list = NULL;
   Eina_List *folders = NULL, *groups = NULL;
   Eina_Stringshare *group_name, *prefix;
   Group *group;
   Node *node;

   if (!eina_str_has_extension(elm_entry_entry_get(tab_edj.edj), ".edj") &&
       !ecore_file_exists(elm_entry_entry_get(tab_edj.edj)))
     return;

   if (prev_edj_path && !strcmp(prev_edj_path, elm_entry_entry_get(tab_edj.edj)))
     return;
   prev_edj_path = elm_entry_entry_get(tab_edj.edj);

   elm_genlist_clear(tab_edj.genlist);
   EINA_LIST_FREE(widget_list, group_name)
     {
        eina_stringshare_del(group_name);
     }
   widget_list = NULL;

   collections = edje_file_collection_list(elm_entry_entry_get(tab_edj.edj));
   if (!collections) return;

   collections = eina_list_sort(collections, eina_list_count(collections), (Eina_Compare_Cb) strcmp);
   EINA_LIST_FOREACH(collections, l, group_name)
     {
        group = mem_calloc(1, sizeof(Group));
        group->name = eina_stringshare_ref(group_name);
        groups_list = eina_list_append(groups_list, group);
     }
   edje_file_collection_list_free(collections);

   widget_tree_items_get(groups_list, "", &folders, &groups);
   EINA_LIST_FREE(folders, prefix)
     {
        node = mem_calloc(1, sizeof(Node));
        node->name = eina_stringshare_ref(prefix);
        _tree_nodes_get(groups_list, node);
        elm_genlist_item_append(tab_edj.genlist,
                                itc,
                                node,
                                NULL,
                                ELM_GENLIST_ITEM_TREE,
                                NULL,
                                NULL);
     }
   EINA_LIST_FREE(groups, group)
     {
        node = mem_calloc(1, sizeof(Node));
        node->name = eina_stringshare_ref(group->name);
        elm_genlist_item_append(tab_edj.genlist,
                                itc,
                                node,
                                NULL,
                                ELM_GENLIST_ITEM_NONE,
                                NULL,
                                NULL);
     }
   edje_file_cache_flush();
   EINA_LIST_FREE(groups_list, group)
     {
        eina_stringshare_del(group->name);
        free(group);
     }

   _validate();
}

/*  GENLIST  */
static void
_widget_list_group_set(Node *node)
{
   if (node->check)
     {
        if (!node->list)
          widget_list = eina_list_append(widget_list, node->name);
     }
   else
     {
        if (!node->list)
          widget_list = eina_list_remove(widget_list, node->name);
     }
}

static void
_node_selected_set(Node *node)
{
   Eina_List *l;
   Node *n;

   EINA_LIST_FOREACH(node->list, l, n)
     {
        n->check = node->check;
        _widget_list_group_set(n);
        _node_selected_set(n);
     }
}

static void
_check_widget(void *data,
              Evas_Object *obj,
              void *event_info __UNUSED__)
{
   Node *node = data;

   assert(node != NULL);

   node->check = elm_check_state_get(obj);
   _widget_list_group_set(node);
   _node_selected_set(node);
   elm_genlist_realized_items_update(tab_edj.genlist);

   _validate();
}

static void
_on_check_all(void *data __UNUSED__,
              Evas_Object *obj,
              void *event_info __UNUSED__)
{
   Elm_Object_Item *item;
   Node *node;

   item = elm_genlist_first_item_get(tab_edj.genlist);
   do
     {
        node = elm_object_item_data_get(item);
        node->check = elm_check_state_get(obj);
        _widget_list_group_set(node);
        _node_selected_set(node);
        item = elm_genlist_item_next_get(item);
     }
   while(item);

   elm_genlist_realized_items_update(tab_edj.genlist);
   _validate();
}

static char *
_genlist_label_get(void *data,
                   Evas_Object *obj __UNUSED__,
                   const char  *part __UNUSED__)
{
   Node *node = data;
   const char *pos;
   char buf[BUFF_MAX];
   int symbols;

   assert (node != NULL);
   if (node->list)
     {
        symbols = strlen(node->name) - 1;
        strncpy(buf, node->name, symbols);
        buf[symbols] = '\0';
        pos = strrchr(buf, '/');
        if (pos) return strdup(pos + 1);
        else return strdup(buf);

     }
   else
     {
        pos = strrchr(node->name, '/');
        if (pos) return strdup(pos + 1);
        else return strdup(node->name);
     }

   return strdup(node->name);
}

static Evas_Object *
_genlist_content_get(void *data,
                     Evas_Object *obj,
                     const char *part)
{
   Evas_Object *check;
   Node *node = data;
   if (strcmp(part, "elm.swallow.icon")) return NULL;

   CHECK_ADD(obj, check);
   elm_object_focus_allow_set(check, false);
   elm_check_state_set(check, node->check);
   evas_object_smart_callback_add(check, "changed", _check_widget, node);
   return check;
}

static void
_on_item_activated(void *data __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *event_info)
{
   Elm_Object_Item *it = (Elm_Object_Item *)event_info;
   Tree_Item_Data *widget_data = elm_object_item_data_get(it);

   assert(widget_data != NULL);

   widget_data->check = !widget_data->check;
   elm_genlist_item_update(it);
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
             Evas_Object *obj __UNUSED__,
             void *event_info)
{
   Elm_Object_Item *glit = event_info;
   Node *node = elm_object_item_data_get(glit);
   Eina_List *l;
   Node *n;

   EINA_LIST_FOREACH(node->list, l, n)
     {
        if (n->list)
          elm_genlist_item_append(tab_edj.genlist,
                                  itc,
                                  n,
                                  glit,
                                  ELM_GENLIST_ITEM_TREE,
                                  NULL,
                                  NULL);
        else
          elm_genlist_item_append(tab_edj.genlist,
                                  itc,
                                  n,
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
_progress_end(void *data, PM_Project_Result result)
{
   if (PM_PROJECT_SUCCESS == result)
     {
        elm_entry_entry_set(tab_edj.name, NULL);
        elm_entry_entry_set(tab_edj.path, profile_get()->general.projects_folder);
        elm_entry_entry_set(tab_edj.edj, NULL);
        elm_entry_entry_set(tab_edj.meta.version, NULL);
        elm_entry_entry_set(tab_edj.meta.authors, NULL);
        elm_entry_entry_set(tab_edj.meta.licenses, NULL);
        elm_entry_entry_set(tab_edj.meta.comment, N_("Created with Eflete!"));
     }

   _tabs_progress_end(data, result);
}

static Eina_Bool
_setup_open_splash(void *data __UNUSED__, Splash_Status status __UNUSED__)
{
   pm_project_import_edj(elm_entry_entry_get(tab_edj.name),
                         elm_entry_entry_get(tab_edj.path),
                         elm_entry_entry_get(tab_edj.edj),
                         widget_list,
                         progress_print,
                         _progress_end,
                         &tab_edj.meta);

   return true;
}

static Eina_Bool
_teardown_open_splash(void *data __UNUSED__, Splash_Status status __UNUSED__)
{
   pm_project_thread_free();
   ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_MAIN, false);
   return true;
}

__UNUSED__ static Eina_Bool
_cancel_open_splash(void *data __UNUSED__, Splash_Status status __UNUSED__)
{
   pm_project_thread_cancel();
   ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_MAIN, false);
   return true;
}

static void
_import(void *data __UNUSED__,
        Evas_Object *obj __UNUSED__,
        void *event_info __UNUSED__)
{
   Eina_Strbuf *buf;

   if (ap.project)
     if (!project_close())
       return;

   buf = eina_strbuf_new();
   eina_strbuf_append_printf(buf, "%s/%s.pro",
                             elm_entry_entry_get(tab_edj.path),
                             elm_entry_entry_get(tab_edj.name));
   if (!pm_lock_check(eina_strbuf_string_get(buf)))
     {
       popup_want_action(_("Import EDJ-file"), _("The given file is locked by another application"),
                         NULL, BTN_OK, NULL, NULL);
       return;
     }

   eina_strbuf_reset(buf);
   eina_strbuf_append_printf(buf,
                            _("<wrap=word>A project folder named <b>'%s'</b> already exist in <b>'%s'</b>."
                              "Do you want to replace it?<br>"
                              "Replacing it will overwrite <b>all</b> contents.</wrap>"),
                            elm_entry_entry_get(tab_edj.name),
                            elm_entry_entry_get(tab_edj.path));

   if (!exist_permission_check(elm_entry_entry_get(tab_edj.path),
                               elm_entry_entry_get(tab_edj.name),
                               _("Import edj-file"), eina_strbuf_string_get(buf), EINA_FALSE))
     return;
   eina_strbuf_free(buf);
   ap.splash = splash_add(ap.win,
                          _setup_open_splash,
                          _teardown_open_splash,
                          NULL,
                          NULL);
   elm_object_focus_set(ap.splash, true);
   evas_object_show(ap.splash);
}

static void
_elipsis(void *data __UNUSED__,
         Evas_Object *obj __UNUSED__,
         void *event_info __UNUSED__)
{
   popup_fileselector_folder_helper(NULL,
                                    tab_edj.path,
                                    elm_entry_entry_get(tab_edj.path),
                                    entry_path_set,
                                    tab_edj.path,
                                    false,
                                    false);
}

static void
_elipsis_edj(void *data __UNUSED__,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   popup_fileselector_edj_helper(NULL,
                                 tab_edj.edj,
                                 NULL,
                                 entry_path_set,
                                 tab_edj.edj,
                                 false,
                                 false);
}

void
_tab_import_edj_del(void *data __UNUSED__,
                    Evas *e __UNUSED__,
                    Evas_Object *obj __UNUSED__,
                    void *event_info __UNUSED__)
{
   elm_validator_regexp_free(tab_edj.name_validator);
}

Evas_Object *
_tab_import_edj_add(void)
{
   tab_edj.name_validator = elm_validator_regexp_new(NAME_REGEX, NULL);

   tab_edj.layout = elm_layout_add(ap.win);
   elm_layout_theme_set(tab_edj.layout, "layout", "tab_home", "import_edj");

   BUTTON_ADD(tab_edj.layout, tab_edj.btn_create, _("Create"))
   elm_object_part_content_set(tab_edj.layout, "elm.swallow.btn_create", tab_edj.btn_create);
   evas_object_smart_callback_add(tab_edj.btn_create, "clicked", _import, NULL);
   elm_object_disabled_set(tab_edj.btn_create, true);

   /* label.name */
   elm_object_part_text_set(tab_edj.layout, "label.name", _("Project name:"));
   ENTRY_ADD(tab_edj.layout, tab_edj.name, true)
   eo_event_callback_add(tab_edj.name, ELM_ENTRY_EVENT_VALIDATE, elm_validator_regexp_helper, tab_edj.name_validator);
   evas_object_smart_callback_add(tab_edj.name, "changed", _name_changed_cb, NULL);
   elm_object_part_content_set(tab_edj.layout, "swallow.name", tab_edj.name);
   /* label.path */
   elm_object_part_text_set(tab_edj.layout, "label.path", _("Path to project:"));
   ENTRY_ADD(tab_edj.layout, tab_edj.path, true)
   elm_object_part_content_set(tab_edj.layout, "swallow.path", tab_edj.path);
   elm_entry_entry_set(tab_edj.path, profile_get()->general.projects_folder);
   elipsis_btn_add(tab_edj.path, _elipsis, NULL);

   /* label.path */
   elm_object_part_text_set(tab_edj.layout, "label.edj", _("Path to edj-file:"));
   ENTRY_ADD(tab_edj.layout, tab_edj.edj, true)
   evas_object_smart_callback_add(tab_edj.edj, "changed", _edj_changed_cb, NULL);
   elm_object_part_content_set(tab_edj.layout, "swallow.edj", tab_edj.edj);
   elipsis_btn_add(tab_edj.edj, _elipsis_edj, NULL);

   meta_controls_add(tab_edj.layout, &tab_edj.meta);

   evas_object_event_callback_add(tab_edj.layout, EVAS_CALLBACK_SHOW, _tab_default_focus, tab_edj.name);

   /* check all */
   CHECK_ADD(tab_edj.layout, tab_edj.ch_all);
   evas_object_smart_callback_add(tab_edj.ch_all, "changed", _on_check_all, NULL);
   elm_object_part_content_set(tab_edj.layout, "swallow.all_widgets_check", tab_edj.ch_all);
   elm_object_part_text_set(tab_edj.layout, "label.widgets", _("Widgets:"));

   /* genlist item */
   itc = elm_genlist_item_class_new();
   itc->item_style = "aligned";
   itc->func.text_get = _genlist_label_get;
   itc->func.content_get = _genlist_content_get;

   /* genlist */
   tab_edj.genlist = elm_genlist_add(ap.win);
   evas_object_smart_callback_add(tab_edj.genlist, "activated", _on_item_activated, NULL);
   evas_object_smart_callback_add(tab_edj.genlist, "expand,request", _expand_request_cb, NULL);
   evas_object_smart_callback_add(tab_edj.genlist, "contract,request", _contract_request_cb, NULL);
   evas_object_smart_callback_add(tab_edj.genlist, "expanded", _expanded_cb, NULL);
   evas_object_smart_callback_add(tab_edj.genlist, "contracted", _contracted_cb, NULL);

   elm_object_part_content_set(tab_edj.layout, "swallow.widgets", tab_edj.genlist);
   evas_object_event_callback_add(tab_edj.layout, EVAS_CALLBACK_DEL, _tab_import_edj_del, NULL);

   return tab_edj.layout;
}

static void
_delayed_popup(void *data)
{
   char *msg = data;
   popup_want_action(_("Import edj-file"), msg, NULL, BTN_OK, NULL, NULL);
   free(msg);
}

static void
_genlist_style_selected_set(Node *item, Eina_List *styles, Eina_Bool selected)
{
   Eina_List *l, *l1, *cp_style_list;
   Node *node;
   Eina_Stringshare *name, *name1, *style_name, *tmp;
   const char *pos;
   char cp_style[256];

   assert (item != NULL);

   if (item->list)
     {
        EINA_LIST_FOREACH(item->list, l, node)
          {
             _genlist_style_selected_set(node, styles, selected);
          }
     }
   else
     {
        if (styles)
          {
             EINA_LIST_FOREACH(styles, l, name)
               {
                  style_name = option_style_name_get(name, &cp_style_list);
                  if (!strcmp(style_name, "default"))
                    {
                       pos = string_rstr(item->name, "base/default");
                       if (pos) widget_list = eina_list_append(widget_list, item->name);
                    }
                  else
                    {
                       pos = string_rstr(item->name, style_name);
                       if (pos)
                         {
                            item->check = selected;
                            widget_list = eina_list_append(widget_list, item->name);
                            EINA_LIST_FOREACH(cp_style_list, l1, name1)
                              {
                                 strncpy(cp_style, item->name, pos - item->name - 1);
                                 cp_style[pos - item->name] = '\0';
                                 tmp = eina_stringshare_printf("cp***%s***%s/%s", item->name, cp_style, name1);
                                 widget_list = eina_list_append(widget_list, tmp);
                              }
                         }
                    }
                  eina_stringshare_del(style_name);
                  EINA_LIST_STRINGSHARE_FREE(cp_style_list);
               }
          }
        else
          {
             /* if list of style empty need to select all available widget styles */
             item->check = selected;
             widget_list = eina_list_append(widget_list, item->name);
          }
     }
}

void
_tab_import_edj_data_set(const char *name, const char *path, const char *edj, const Eina_List *widgets)
{
   Eina_List *style_list = NULL;
   const char *str;
   Eina_Stringshare *widget_name;
   Eina_Strbuf *buf = eina_strbuf_new();
   Eina_Bool first_not_found = true;

   Elm_Object_Item *item = NULL;
   const Eina_List *l, *l1;
   Node *node = NULL, *sub;

   assert(tab_edj.layout != NULL);

   elm_entry_entry_set(tab_edj.name, name);

   if (path) elm_entry_entry_set(tab_edj.path, path);
   else elm_entry_entry_set(tab_edj.path, profile_get()->general.projects_folder);

   elm_entry_entry_set(tab_edj.edj, edj);
   edje_message_signal_process();

   EINA_LIST_FOREACH(widgets, l, str)
     {
        widget_name = option_widget_name_get(str, &style_list);

        /* if 'elm' node missing try to find it */
        if (!item)
          {
             item = elm_genlist_first_item_get(tab_edj.genlist);
             do
               {
                  node = elm_object_item_data_get(item);
                  if (!strcmp(node->name, "elm/"))
                    break;
                  item = elm_genlist_item_next_get(item);
               }
             while (item);
          }
        if (node->list)
          {
             EINA_LIST_FOREACH(node->list, l1, sub)
             {
                if (strstr(sub->name, widget_name))
                  _genlist_style_selected_set(sub, style_list, true);
             }
          }
        else
          {
             eina_strbuf_append_printf(buf, first_not_found ? "%s" : ", %s", widget_name);
             first_not_found = false;
          }
        eina_stringshare_del(widget_name);
        EINA_LIST_STRINGSHARE_FREE(style_list);
     }
   elm_genlist_realized_items_update(tab_edj.genlist);
   if (eina_strbuf_length_get(buf))
     {
        eina_strbuf_prepend(buf, _("Following widgets were not found and ignored: "));
        ERR("%s", eina_strbuf_string_get(buf));
        ecore_job_add(_delayed_popup, eina_strbuf_string_steal(buf));
     }
   eina_strbuf_free(buf);

   _validate();
}
