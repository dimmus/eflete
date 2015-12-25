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

struct _Dir_Data
{
   Evas_Object *item;
   Evas_Object *entry;
   Evas_Object *btn_del;
};
typedef struct _Dir_Data Dir_Data;

struct _Tab_Home_Edc
{
   Evas_Object *layout;
   Evas_Object *btn_create;
   Evas_Object *box;
   Elm_Validator_Regexp *name_validator;

   Evas_Object *name;
   Evas_Object *path;
   Evas_Object *edc;
   Eina_List *img_dirs;
   Eina_List *fnt_dirs;
   Eina_List *snd_dirs;
   /* Eina_List *vbr_dirs; uncoment when vibration will be suported in the edje */
   Eina_List *data_dirs;
   Meta_Data_Controls meta;

   Eina_Strbuf *log;
};
typedef struct _Tab_Home_Edc Tab_Home_Edc;

static Tab_Home_Edc tab_edc;

static void
_img_dir_add(void *data, Evas_Object *obj, void *event_info);

static void
_fnt_dir_add(void *data, Evas_Object *obj, void *event_info);

static void
_snd_dir_add(void *data, Evas_Object *obj, void *event_info);

/*
static void
_vbr_dir_add(void *data, Evas_Object *obj, void *event_info);
*/

static void
_data_dir_add(void *data, Evas_Object *obj, void *event_info);

static void
_img_dir_del(void *data, Evas_Object *obj, void *event_info);

static void
_fnt_dir_del(void *data, Evas_Object *obj, void *event_info);

static void
_snd_dir_del(void *data, Evas_Object *obj, void *event_info);

/*
static void
_vbr_dir_del(void *data, Evas_Object *obj, void *event_info);
*/

static void
_data_dir_del(void *data, Evas_Object *obj, void *event_info);

static void
_validate(void *data __UNUSED__,
          Evas_Object *obj __UNUSED__,
          void *event_info __UNUSED__)
{
   if ((elm_validator_regexp_status_get(tab_edc.name_validator) != ELM_REG_NOERROR) ||
       !eina_str_has_extension(elm_entry_entry_get(tab_edc.edc), ".edc"))
     elm_object_disabled_set(tab_edc.btn_create, true);
   else
     elm_object_disabled_set(tab_edc.btn_create, false);
}

static void
_elipsis(void *data,
         Evas_Object *obj __UNUSED__,
         void *event_info __UNUSED__)
{
   popup_fileselector_folder_helper(NULL,
                                    (Evas_Object *)data,
                                    elm_entry_entry_get(tab_edc.path),
                                    entry_path_set,
                                    (Evas_Object *)data,
                                    false,
                                    false);
}

static void
_elipsis_edc(void *data __UNUSED__,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   popup_fileselector_edc_helper(NULL,
                                 tab_edc.edc,
                                 NULL,
                                 entry_path_set,
                                 tab_edc.edc,
                                 false,
                                 false);
}

static void
_btn_add_add(Evas_Object *item, Evas_Smart_Cb add_func)
{
   Evas_Object *btn, *ic;

   btn = elm_button_add(item);
   ic = elm_icon_add(btn);
   elm_icon_standard_set(ic, "plus");
   elm_object_part_content_set(btn, NULL, ic);
   elm_layout_content_set(item, "swallow.button_add", btn);
   evas_object_smart_callback_add(btn, "clicked", add_func, NULL);
}

static void
_dir_del(Eina_List **dirs_list, Dir_Data *dir_data,
         const char *label, Evas_Smart_Cb add_func)
{
   Dir_Data *tmp;
   Eina_List *l;
   /* check the first item, if deleted object the first in the list, we need to
    * set the label to next item and move btn_add */
   if (dir_data == eina_list_data_get(*dirs_list))
     {
        tmp = eina_list_data_get(eina_list_next(*dirs_list));
        elm_layout_text_set(tmp->item, NULL, label);
        _btn_add_add(tmp->item, add_func);
     }

   l = eina_list_data_find_list(*dirs_list, dir_data);
   *dirs_list = eina_list_remove_list(*dirs_list, l);
   elm_box_unpack(tab_edc.box, dir_data->item);
   evas_object_del(dir_data->item);
   free(dir_data);
   if (eina_list_count(*dirs_list) == 1)
     {
        tmp = eina_list_data_get(*dirs_list);
        elm_object_disabled_set(tmp->btn_del, true);
     }
}

static void
_img_dir_del(void *data,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   _dir_del(&tab_edc.img_dirs, (Dir_Data *)data, _("Images directoies:"), _img_dir_add);
}

static void
_fnt_dir_del(void *data,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   _dir_del(&tab_edc.fnt_dirs, (Dir_Data *)data, _("Fonts directoies:"), _fnt_dir_add);
}

static void
_snd_dir_del(void *data,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   _dir_del(&tab_edc.snd_dirs, (Dir_Data *)data, _("Sounds directoies:"), _snd_dir_add);
}

/*
static void
_vbr_dir_del(void *data,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   _dir_del(&tab_edc.vbr_dirs, (Dir_Data *)data, _("Vibrations directoies:"), _vbr_dir_add);
}
*/

static void
_data_dir_del(void *data,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   _dir_del(&tab_edc.data_dirs, (Dir_Data *)data, _("Data directoies:"), _data_dir_add);
}

static Dir_Data *
_dir_item_add(Evas_Smart_Cb del_func)
{
   Dir_Data *dir_data;
   Evas_Object *item, *ic;

   dir_data = mem_malloc(sizeof(Dir_Data));
   LAYOUT_PROP_ADD(tab_edc.box, NULL, "tab_home", "item")
   dir_data->item = item;
   ENTRY_ADD(dir_data->item, dir_data->entry, true)
   elm_layout_content_set(dir_data->item, NULL, dir_data->entry);
   elipsis_btn_add(dir_data->entry, _elipsis, dir_data->entry);

   dir_data->btn_del = elm_button_add(item);
   ic = elm_icon_add(dir_data->btn_del);
   elm_icon_standard_set(ic, "minus");
   elm_object_part_content_set(dir_data->btn_del, NULL, ic);
   evas_object_smart_callback_add(dir_data->btn_del, "clicked", del_func, dir_data);
   elm_layout_content_set(dir_data->item, "swallow.button_del", dir_data->btn_del);

   return dir_data;
}

static void
_dir_add(Eina_List *dirs_list, Evas_Smart_Cb del_func)
{
   Dir_Data *dir_data, *tmp;

   dir_data = _dir_item_add(del_func);
   if (eina_list_count(dirs_list) == 1)
     {
        /* enable the 'del' button of first item, make posible to delete the
         * first item */
        tmp = eina_list_data_get(dirs_list);
        elm_object_disabled_set(tmp->btn_del, false);
     }
   tmp = eina_list_data_get(eina_list_last(dirs_list));
   elm_box_pack_after(tab_edc.box, dir_data->item, tmp->item);
   dirs_list = eina_list_append(dirs_list, dir_data);
}

static void
_img_dir_add(void *data __UNUSED__,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   _dir_add(tab_edc.img_dirs, _img_dir_del);
}

static void
_fnt_dir_add(void *data __UNUSED__,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   _dir_add(tab_edc.fnt_dirs, _fnt_dir_del);
}

static void
_snd_dir_add(void *data __UNUSED__,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   _dir_add(tab_edc.snd_dirs, _snd_dir_del);
}

/*
static void
_vbr_dir_add(void *data __UNUSED__,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   _dir_add(tab_edc.vbr_dirs, _vbr_dir_del);
}
*/

static void
_data_dir_add(void *data __UNUSED__,
              Evas_Object *obj __UNUSED__,
              void *event_info __UNUSED__)
{
   _dir_add(tab_edc.data_dirs, _data_dir_del);
}

static Eina_Strbuf * /* need free by user side */
_edje_cc_opt_build(void)
{
   Eina_Strbuf *buf;
   Eina_List *l;
   Dir_Data *dir_data;

   buf = eina_strbuf_new();
   EINA_LIST_FOREACH(tab_edc.img_dirs, l, dir_data)
     {
        if (elm_entry_is_empty(dir_data->entry)) continue;
        eina_strbuf_append_printf(buf, "-id \"%s\"", elm_entry_entry_get(dir_data->entry));
     }
   EINA_LIST_FOREACH(tab_edc.fnt_dirs, l, dir_data)
     {
        if (elm_entry_is_empty(dir_data->entry)) continue;
        eina_strbuf_append_printf(buf, "-fd \"%s\"", elm_entry_entry_get(dir_data->entry));
     }
   EINA_LIST_FOREACH(tab_edc.snd_dirs, l, dir_data)
     {
        if (elm_entry_is_empty(dir_data->entry)) continue;
        eina_strbuf_append_printf(buf, "-sd \"%s\"", elm_entry_entry_get(dir_data->entry));
     }
   /*
   EINA_LIST_FOREACH(tab_edc.vbr_dirs, l, dir_data)
     {
        if (elm_entry_is_empty(dir_data->entry)) continue;
        eina_strbuf_append_printf(buf, "-vd \"%s\"", elm_entry_entry_get(dir_data->entry));
     }
   */
   EINA_LIST_FOREACH(tab_edc.data_dirs, l, dir_data)
     {
        if (elm_entry_is_empty(dir_data->entry)) continue;
        eina_strbuf_append_printf(buf, "-dd \"%s\"", elm_entry_entry_get(dir_data->entry));
     }

   return buf;
}

static void
_dirs_cleanup(Eina_List *list, Evas_Smart_Cb del_func)
{
   Dir_Data *data;

   while (1 != eina_list_count(list))
     {
        data = eina_list_data_get(eina_list_last(list));
        del_func(data, NULL, NULL);
     }
}

static Eina_Bool
_progress_print(void *data, Eina_Stringshare *progress_string)
{
   eina_strbuf_append_printf(tab_edc.log, "%s<br>", progress_string);
   return progress_print(data, progress_string);
}

static void
_progress_end(void *data, PM_Project_Result result)
{
   if (PM_PROJECT_ERROR == result)
     popup_log_message_helper(eina_strbuf_string_get(tab_edc.log));
   if (PM_PROJECT_SUCCESS == result)
     {
        elm_entry_entry_set(tab_edc.name, NULL);
        elm_entry_entry_set(tab_edc.path, profile_get()->general.projects_folder);
        elm_entry_entry_set(tab_edc.edc, NULL);
        _dirs_cleanup(tab_edc.img_dirs, _img_dir_del);
        _dirs_cleanup(tab_edc.fnt_dirs, _fnt_dir_del);
        _dirs_cleanup(tab_edc.snd_dirs, _snd_dir_del);
        /* _dirs_cleanup(tab_edc.vbr_dirs, _vbr_dir_del); */
        _dirs_cleanup(tab_edc.data_dirs, _data_dir_del);
        elm_entry_entry_set(tab_edc.meta.version, NULL);
        elm_entry_entry_set(tab_edc.meta.authors, NULL);
        elm_entry_entry_set(tab_edc.meta.licenses, NULL);
        elm_entry_entry_set(tab_edc.meta.comment, N_("Created with Eflete!"));
     }
   _tabs_progress_end(data, result);
}

static Eina_Bool
_setup_open_splash(void *data __UNUSED__, Splash_Status status __UNUSED__)
{
   Eina_Strbuf *flags = _edje_cc_opt_build();

   eina_strbuf_reset(tab_edc.log);
   pm_project_import_edc(elm_entry_entry_get(tab_edc.name),
                         elm_entry_entry_get(tab_edc.path),
                         elm_entry_entry_get(tab_edc.edc),
                         eina_strbuf_string_get(flags),
                         _progress_print,
                         _progress_end,
                         &tab_edc.meta);

   eina_strbuf_free(flags);
   return true;
}

static Eina_Bool
_teardown_open_splash(void *data __UNUSED__, Splash_Status status __UNUSED__)
{
   pm_project_thread_free();
   return true;
}

static Eina_Bool
_cancel_open_splash(void *data __UNUSED__, Splash_Status status __UNUSED__)
{
   pm_project_thread_cancel();
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
   eina_strbuf_append_printf(buf,
                            _("<font_size=16>A project folder named '%s' already exist."
                              "Do you want to replace it?</font_size><br>"
                              "The project folder '%s' already exist in '%s'. Replacing it will overwrite"
                              "<b>all</b> contents."),
                            elm_entry_entry_get(tab_edc.name),
                            elm_entry_entry_get(tab_edc.name),
                            elm_entry_entry_get(tab_edc.path));

   if (!exist_permission_check(elm_entry_entry_get(tab_edc.path),
                               elm_entry_entry_get(tab_edc.name),
                               _("Import edc-file"), eina_strbuf_string_get(buf)))
     return;
   eina_strbuf_free(buf);
   ap.splash = splash_add(ap.win,
                          _setup_open_splash,
                          _teardown_open_splash,
                          _cancel_open_splash,
                          NULL);
   elm_object_focus_set(ap.splash, true);
   evas_object_show(ap.splash);
}

Evas_Object *
_tab_import_edc_add(void)
{
   Evas_Object *sc, *item, *separ;
   Dir_Data *dir_data;

   tab_edc.name_validator = elm_validator_regexp_new(NAME_REGEX, NULL);

   tab_edc.layout = elm_layout_add(ap.win);
   elm_layout_theme_set(tab_edc.layout, "layout", "tab_home", "import_edc");

   BUTTON_ADD(tab_edc.layout, tab_edc.btn_create, _("Import"))
   elm_object_part_content_set(tab_edc.layout, "elm.swallow.btn_create", tab_edc.btn_create);
   evas_object_smart_callback_add(tab_edc.btn_create, "clicked", _import, NULL);
   elm_object_disabled_set(tab_edc.btn_create, true);

   SCROLLER_ADD(tab_edc.layout, sc);
   BOX_ADD(tab_edc.layout, tab_edc.box, false, false);
   elm_box_padding_set(tab_edc.box, 0, 6);
   elm_object_content_set(sc, tab_edc.box);
   elm_object_part_content_set(tab_edc.layout, "swallow.project_info", sc);

   /* label.name */
   LAYOUT_PROP_ADD(tab_edc.box, _("Project name:"), "tab_home", "item")
   ENTRY_ADD(item, tab_edc.name, true)
   eo_do(tab_edc.name, eo_event_callback_add(ELM_ENTRY_EVENT_VALIDATE, elm_validator_regexp_helper, tab_edc.name_validator));
   evas_object_smart_callback_add(tab_edc.name, "changed", _validate, NULL);
   elm_layout_content_set(item, NULL, tab_edc.name);
   elm_box_pack_end(tab_edc.box, item);
   /* label.path */
   LAYOUT_PROP_ADD(tab_edc.box, _("Path to project:"), "tab_home", "item")
   ENTRY_ADD(item, tab_edc.path, true)
   elm_entry_entry_set(tab_edc.path, profile_get()->general.projects_folder);
   elm_layout_content_set(item, NULL, tab_edc.path);
   elm_box_pack_end(tab_edc.box, item);
   elipsis_btn_add(tab_edc.path, _elipsis, tab_edc.path);
   /* label.path */
   LAYOUT_PROP_ADD(tab_edc.box, _("Path to edc-file:"), "tab_home", "item")
   ENTRY_ADD(item, tab_edc.edc, true)
   elm_layout_content_set(item, NULL, tab_edc.edc);
   evas_object_smart_callback_add(tab_edc.edc, "changed", _validate, NULL);
   elm_box_pack_end(tab_edc.box, item);
   elipsis_btn_add(tab_edc.edc, _elipsis_edc, NULL);
   /* separator 1 */
   separ = elm_separator_add(tab_edc.box);
   elm_separator_horizontal_set(separ, true);
   evas_object_show(separ);
   elm_box_pack_end(tab_edc.box, separ);

   /* first item for image dirs list */
   dir_data = _dir_item_add(_img_dir_del);
   elm_layout_text_set(dir_data->item, NULL, _("Images directoies:"));
   tab_edc.img_dirs = eina_list_append(tab_edc.img_dirs, dir_data);
   elm_object_disabled_set(dir_data->btn_del, true);
   _btn_add_add(dir_data->item, _img_dir_add);
   elm_box_pack_end(tab_edc.box, dir_data->item);

   /* separator 2 */
   separ = elm_separator_add(tab_edc.box);
   elm_separator_horizontal_set(separ, true);
   evas_object_show(separ);
   elm_box_pack_end(tab_edc.box, separ);

   /* first item for font dirs list */
   dir_data = _dir_item_add(_fnt_dir_del);
   elm_layout_text_set(dir_data->item, NULL, _("Font directoies:"));
   tab_edc.fnt_dirs = eina_list_append(tab_edc.fnt_dirs, dir_data);
   elm_object_disabled_set(dir_data->btn_del, true);
   _btn_add_add(dir_data->item, _fnt_dir_add);
   elm_box_pack_end(tab_edc.box, dir_data->item);

   /* separator 3 */
   separ = elm_separator_add(tab_edc.box);
   elm_separator_horizontal_set(separ, true);
   evas_object_show(separ);
   elm_box_pack_end(tab_edc.box, separ);

   /* first item for sound dirs list */
   dir_data = _dir_item_add(_snd_dir_del);
   elm_layout_text_set(dir_data->item, NULL, _("Sound directoies:"));
   tab_edc.snd_dirs = eina_list_append(tab_edc.snd_dirs, dir_data);
   elm_object_disabled_set(dir_data->btn_del, true);
   _btn_add_add(dir_data->item, _snd_dir_add);
   elm_box_pack_end(tab_edc.box, dir_data->item);

   /* separator 4 */
   separ = elm_separator_add(tab_edc.box);
   elm_separator_horizontal_set(separ, true);
   evas_object_show(separ);
   elm_box_pack_end(tab_edc.box, separ);

   /* first item for vibration dirs list */
   /* UNCOMMENT it when vibration will be supported
   dir_data = _dir_item_add(_vbr_dir_del);
   elm_layout_text_set(dir_data->item, NULL, _("Virbration directoies:"));
   tab_edc.vbr_dirs = eina_list_append(tab_edc.vbr_dirs, dir_data);
   elm_object_disabled_set(dir_data->btn_del, true);
   _btn_add_add(dir_data->item, _vbr_dir_add);
   elm_box_pack_end(tab_edc.box, dir_data->item);
   */

   /* separator 5 */
   /* UNCOMMENT it when vibration will be supported
   separ = elm_separator_add(tab_edc.box);
   elm_separator_horizontal_set(separ, true);
   evas_object_show(separ);
   elm_box_pack_end(tab_edc.box, separ);
   */

   /* first item for data dirs list */
   dir_data = _dir_item_add(_data_dir_del);
   elm_layout_text_set(dir_data->item, NULL, _("Data directoies:"));
   tab_edc.data_dirs = eina_list_append(tab_edc.data_dirs, dir_data);
   elm_object_disabled_set(dir_data->btn_del, true);
   _btn_add_add(dir_data->item, _data_dir_add);
   elm_box_pack_end(tab_edc.box, dir_data->item);

   meta_controls_add(tab_edc.layout, &tab_edc.meta);

   tab_edc.log = eina_strbuf_new();

   return tab_edc.layout;
}
