/*
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
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

#include "animator.h"
#include "animator_private.h"
#include "main_window.h"
#include "tabs.h"

TODO("After all is done, cleanup this structure from non-important fields")
struct _Animator
{
   Evas_Object *mwin;
   Live_View *live; /* delete */
   Style *style; /* delete */
   const char *program;
   Evas_Object *program_area_layout;
   Evas_Object *prop_scroller;
   Evas_Object *gl_progs;
   Elm_Object_Item *sel;
   Eina_List *afters;

   Evas_Object *program_editor;
   Evas_Object *program_sequence;

   struct {
        Evas_Object *popup;
        Elm_Validator_Regexp *name_validator;
        Evas_Object *entry;
        Evas_Object *btn_add;
   } popup;
   struct {
        Evas_Object *play;
        Evas_Object *reset;
        Evas_Object *cycle;
   } program_controls;
   Eina_Bool is_cycled : 1;
   Eina_Bool sequence_mode : 1;

   Project *project;
   Group *group;
   Evas_Object *live_view;
};

typedef struct _Animator Animator;

static Elm_Genlist_Item_Class *_itc_prog, *_itc_prog_after;

/* onload callback for live object. needed to interrupt onload/onshow programs */

/*
static void
_on_object_load(void *data,
                Evas_Object *obj __UNUSED__,
                const char *em __UNUSED__,
                const char *src __UNUSED__)
{
   Animator *animator = data;

   assert(animator != NULL);

   program_editor_program_reset(animator->program_editor);
}
*/

/********************* callbacks for interaction with submodule ***************/
static void
_on_program_pause_cb(void *data,
                     Evas_Object *obj __UNUSED__,
                     void *event_info __UNUSED__)
{
   /* changing button name on submodules request */
   Animator *animator = data;

   assert(animator != NULL);

   elm_object_text_set(animator->program_controls.play, _("Play"));
}

static void
_on_program_play_cb(void *data,
                    Evas_Object *obj __UNUSED__,
                    void *event_info __UNUSED__)
{
   /* changing button name on submodules request */
   Animator *animator = data;

   assert(animator != NULL);

   elm_object_text_set(animator->program_controls.play, _("Pause"));
}

static void
_on_program_name_change_cb(void *data,
                           Evas_Object *obj __UNUSED__,
                           void *event_info)
{
   /* changing name of program in genlist on submodules request */
   Animator *animator = data;
   const char *new_name = event_info;

   assert(animator != NULL);

   Eina_Stringshare *item_data = elm_object_item_data_get(animator->sel);
   eina_stringshare_replace(&item_data, new_name);
   elm_object_item_data_set(animator->sel, (void *)item_data);
   elm_genlist_item_update(animator->sel);
}

static void
_on_program_scroll_cb(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *event_info)
{
   Animator *animator = data;
   Evas_Coord x = * (int *) event_info;
   Evas_Coord y, w, h;

   assert(animator != NULL);

   elm_scroller_region_get(animator->prop_scroller, NULL, &y, &w, &h);
   elm_scroller_region_show(animator->prop_scroller, x, y, w, h);
}

/********************* ui callbacks *******************************************/
static void
_on_program_reset(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   Animator *animator = data;

   assert(animator != NULL);

   if (animator->sequence_mode)
     prog_sequence_program_reset(animator->program_sequence);
   else
     program_editor_program_reset(animator->program_editor);
}

static void
_on_program_play(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info __UNUSED__)
{
   Animator *animator = data;

   assert(animator != NULL);

   if (animator->sequence_mode)
     prog_sequence_program_play(animator->program_sequence);
   else
     program_editor_program_play(animator->program_editor);
}

static void
_on_program_cycle(void *data,
                  Evas_Object *obj __UNUSED__,
                  void *event_info __UNUSED__)
{
   Animator *animator = data;

   assert(animator != NULL);

   animator->is_cycled = !animator->is_cycled;
   if (animator->is_cycled)
     elm_object_text_set(animator->program_controls.cycle, _("Cycled"));
   else
     elm_object_text_set(animator->program_controls.cycle, _("Not cycled"));

   program_editor_cycled_set(animator->program_editor, animator->is_cycled);
   prog_sequence_cycled_set(animator->program_sequence, animator->is_cycled);
}

static void
_on_animator_save(void *data __UNUSED__,
                  Evas_Object* obj __UNUSED__,
                  void *ei __UNUSED__)
{
   //Style *style = ap.project->current_style;
   TODO("this comment should be deleted after signal block functionality will be moved")
   /*ui_signal_list_data_unset(ap.block.signal_list);
   ui_signal_list_data_set(ap.block.signal_list, style);*/

//   live_view_widget_style_set(ap.live_view, ap.project, style);
   //project_changed(true);
}

static void
_on_animator_close(void *data,
                   Evas *e __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void *event_info __UNUSED__)
{
   Animator *animator = (Animator*)data;

   assert(animator != NULL);

   TODO("change this after discarding changes would be possible")
//   _on_animator_save(NULL, NULL, NULL);

   program_editor_free(animator->program_editor);

   evas_object_del(animator->live_view);

   animator->afters = eina_list_free(animator->afters);

   free(animator);
}

static void
_on_animator_ok(void *data,
                Evas_Object *obj __UNUSED__,
                void *ei __UNUSED__)
{
   Animator *animator = (Animator *)data;

   assert(animator != NULL);

   mw_del(animator->mwin);
}

static void
_on_add_popup_bt_add(void *data,
                     Evas_Object *obj __UNUSED__,
                     void *ei __UNUSED__)
{
   Elm_Object_Item *glit_prog = NULL;
   Animator *animator = (Animator*)data;
   Eina_Stringshare *name = elm_entry_entry_get(animator->popup.entry);

   assert(animator != NULL);

   if ((!name) || (strcmp(name, "") == 0))
     {
        NOTIFY_WARNING(_("Program's 'name can not be empty!"));
        return;
     }
   if (!edje_edit_program_add(animator->group->edit_object, name))
     {
        NOTIFY_WARNING(_("Program's name must be unique!"));
        return;
     }

   glit_prog = elm_genlist_item_append(animator->gl_progs, _itc_prog,
                                       eina_stringshare_add(name),
                                       NULL, ELM_GENLIST_ITEM_NONE, NULL, NULL);
   elm_genlist_item_selected_set(glit_prog, true);
   evas_object_del(animator->popup.popup);
   animator->popup.popup = NULL;
   animator->popup.entry = NULL;

   /* move to "Apply"?
   live_view_widget_style_set(animator->live_view, ap.project, animator->style);
   edje_object_signal_callback_add(animator->live_view, "show", "",
                                   _on_object_load, animator);
   */
}

static void
_on_add_popup_bt_cancel(void *data,
                        Evas_Object *obj __UNUSED__,
                        void *ei __UNUSED__)
{
   Animator *animator = (Animator*)data;

   assert(animator != NULL);
   assert(animator->popup.name_validator != NULL);

   elm_validator_regexp_free(animator->popup.name_validator);
   animator->popup.name_validator = NULL;

   evas_object_del(animator->popup.popup);
   animator->popup.popup = NULL;
   animator->popup.entry = NULL;
}

static void
_on_bt_prog_del(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   Animator *animator = (Animator*)data;

   assert(animator != NULL);

   Elm_Object_Item *glit = elm_genlist_selected_item_get(animator->gl_progs);
   if (!glit) return;
   const char *program_name = NULL;

   program_name = elm_object_item_part_text_get(glit, "elm.text");
   if (!edje_edit_program_del(animator->group->edit_object, program_name))
     {
        ERR("Can't del program");
        abort();
     }
   else
     {
        if (elm_genlist_items_count(animator->gl_progs) == 1)
          {
             animator->program = NULL;
             elm_layout_signal_emit(animator->program_area_layout, "eflete,content,hide", "eflete");
          }
        else
          {
             Elm_Object_Item *next = elm_genlist_item_next_get(glit);
             if (!next) next = elm_genlist_item_prev_get(glit);
             elm_genlist_item_selected_set(next, true);
          }
        elm_object_item_del(glit);
     }
}

static void
_validation(void *data,
            Evas_Object *obj __UNUSED__,
            void *event_info __UNUSED__)
{
   Animator *animator = (Animator*)data;
   
   assert(animator != NULL);

   if (elm_validator_regexp_status_get(animator->popup.name_validator) != ELM_REG_NOERROR)
     elm_object_disabled_set(animator->popup.btn_add, true);
   else
     elm_object_disabled_set(animator->popup.btn_add, false);
}

static void
_on_bt_prog_add(void *data,
                Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   Evas_Object *button = NULL;
   Evas_Object *item;

   Animator *animator = (Animator*)data;

   assert(animator != NULL);
   assert(animator->popup.name_validator == NULL);

   animator->popup.popup = elm_popup_add(animator->mwin);
   elm_object_part_text_set(animator->popup.popup, "title,text",
                            _("New program"));

   animator->popup.name_validator = elm_validator_regexp_new(NAME_REGEX, NULL);
   LAYOUT_PROP_ADD(animator->popup.popup, _("Frequency:"), "property", "1swallow")
   ENTRY_ADD(item, animator->popup.entry, true)
   eo_event_callback_add(animator->popup.entry, ELM_ENTRY_EVENT_VALIDATE, elm_validator_regexp_helper, animator->popup.name_validator);
   evas_object_smart_callback_add(animator->popup.entry, "changed", _validation, animator);
   elm_object_part_content_set(item, "elm.swallow.content", animator->popup.entry);

   elm_object_content_set(animator->popup.popup, item);

   BUTTON_ADD(animator->popup.popup, animator->popup.btn_add, _("Ok"));
   evas_object_smart_callback_add(animator->popup.btn_add, "clicked", _on_add_popup_bt_add, animator);
   elm_object_part_content_set(animator->popup.popup, "button1", animator->popup.btn_add);
   elm_object_disabled_set(animator->popup.btn_add, true);

   BUTTON_ADD(animator->popup.popup, button, _("Cancel"));
   evas_object_smart_callback_add(button, "clicked", _on_add_popup_bt_cancel, animator);
   elm_object_part_content_set(animator->popup.popup, "button2", button);

   evas_object_show(animator->popup.popup);
   elm_object_focus_set(animator->popup.entry, true);
}

static void
_on_bt_mode_change(void *data,
                   Evas_Object *bt,
                   void *event_info __UNUSED__)
{
   Evas_Object *icon;
   Animator *animator = (Animator*)data;

   assert(animator != NULL);

   if (!animator->program) return;

   animator->sequence_mode = !animator->sequence_mode;

   elm_object_content_unset(animator->prop_scroller);
   if (animator->sequence_mode)
     {
        IMAGE_ADD_NEW(bt, icon, "animator", "arrow-left");
        elm_layout_content_set(bt, "icon", icon);
        program_editor_program_stop(animator->program_editor);
        evas_object_hide(animator->program_editor);
        evas_object_show(animator->program_sequence);
        elm_object_content_set(animator->prop_scroller, animator->program_sequence);
        prog_sequence_program_set(animator->program_sequence, animator->program, animator->afters);
     }
   else
     {
        IMAGE_ADD_NEW(bt, icon, "animator", "arrow-right");
        elm_layout_content_set(bt, "icon", icon);
        prog_sequence_program_stop(animator->program_sequence);
        evas_object_hide(animator->program_sequence);
        evas_object_show(animator->program_editor);
        elm_object_content_set(animator->prop_scroller, animator->program_editor);
        prog_editor_program_set(animator->program_editor, animator->program);
     }
}

static void
_on_mwin_del(void * data __UNUSED__,
             Evas *e __UNUSED__,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{

   ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_MAIN, false);
   ap.modal_editor--;
}

/********************* genlist callbacks **************************************/
static void
_item_prog_del(void *data,
               Evas_Object *obj __UNUSED__)
{
   eina_stringshare_del(data);
}

static void __UNUSED__
_on_gen_prog_sel(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *ei)
{
   Animator *animator = (Animator*)data;
   Elm_Object_Item *glit = (Elm_Object_Item *)ei, *it;
   Eina_List *queue = NULL;
   Eina_List *prog_afters = NULL, *l;
   Eina_Stringshare *program;

   assert(animator != NULL);

   elm_layout_signal_emit(animator->program_area_layout, "eflete,content,show", "eflete");
   animator->sel = glit;
   program = elm_object_item_data_get(glit);
   animator->program = program;

   /* highlighting afters */
   EINA_LIST_FREE(animator->afters, program)
     eina_stringshare_del(program);

   queue = eina_list_append(queue, eina_stringshare_add(animator->program));
   while (queue)
     {
        program = eina_list_data_get(queue);
        queue = eina_list_remove_list(queue, queue);

        if (!eina_list_data_find_list(animator->afters, program))
          {
             animator->afters = eina_list_append(animator->afters,
                                                 eina_stringshare_add(program));

             prog_afters = edje_edit_program_afters_get(animator->group->edit_object,
                                                        program);
             EINA_LIST_FOREACH(prog_afters, l, program)
               queue = eina_list_append(queue, eina_stringshare_add(program));
             edje_edit_string_list_free(prog_afters);
          }
        else
          eina_stringshare_del(program);
     }

   it = elm_genlist_first_item_get(animator->gl_progs);
   while (it)
     {
        program = eina_stringshare_add(elm_object_item_data_get(it));
        if (!!(l = eina_list_data_find_list(animator->afters, program)))
          {
             animator->afters = eina_list_remove_list(animator->afters, l);
             animator->afters = eina_list_append(animator->afters, program);
             elm_genlist_item_item_class_update(it, _itc_prog_after);
          }
        else
          elm_genlist_item_item_class_update(it, _itc_prog);
        eina_stringshare_del(program);
        it = elm_genlist_item_next_get(it);
     }

   /* updating submodule view */
   if (animator->sequence_mode)
     prog_sequence_program_set(animator->program_sequence, animator->program, animator->afters);
   else
     prog_editor_program_set(animator->program_editor, animator->program);
}

static char *
_item_prog_label_get(void *data,
                     Evas_Object *obj __UNUSED__,
                     const char *part __UNUSED__)
{
   Eina_Stringshare *prog_label = (Eina_Stringshare *)data;
   return strdup(prog_label);
}

/********************* object creation ****************************************/
static Evas_Object *
_gl_progs_add(Animator *animator)
{
   assert(animator != NULL);

   Evas_Object *gl_progs = NULL;;
   Eina_List *progs_list = NULL;
   Eina_List *l = NULL;

   Resource *res;
   Eina_Stringshare *item_data = NULL;
   Elm_Object_Item *glit = NULL;

   if (!_itc_prog)
     {
        _itc_prog = elm_genlist_item_class_new();
        _itc_prog->item_style = "default";
        _itc_prog->func.text_get = _item_prog_label_get;
        _itc_prog->func.content_get = NULL;
        _itc_prog->func.state_get = NULL;
        _itc_prog->func.del = _item_prog_del;
     }

   if (!_itc_prog_after)
     {
        _itc_prog_after = elm_genlist_item_class_new();
        _itc_prog_after->item_style = "program_after";
        _itc_prog_after->func.text_get = _item_prog_label_get;
        _itc_prog_after->func.content_get = NULL;
        _itc_prog_after->func.state_get = NULL;
        _itc_prog_after->func.del = _item_prog_del;
     }

   gl_progs = elm_genlist_add(animator->mwin);
   evas_object_size_hint_align_set(gl_progs, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(gl_progs, EVAS_HINT_EXPAND,
                                    EVAS_HINT_EXPAND);
   elm_scroller_policy_set(gl_progs, ELM_SCROLLER_POLICY_OFF,
                           ELM_SCROLLER_POLICY_OFF);
   evas_object_show(gl_progs);

   progs_list = animator->group->programs;
   EINA_LIST_FOREACH(progs_list, l, res)
     {
        if (!res->name) continue; /* if the list has NULL item */
        item_data = eina_stringshare_add(res->name);
        glit = elm_genlist_item_append(gl_progs, _itc_prog, item_data,
                                       NULL, ELM_GENLIST_ITEM_NONE,
                                       NULL, NULL);
        elm_object_item_data_set(glit, (void *)item_data);
     }

   evas_object_smart_callback_add(gl_progs, "selected", _on_gen_prog_sel,
                                  animator);
   elm_scroller_policy_set(gl_progs, ELM_SCROLLER_POLICY_AUTO,
                           ELM_SCROLLER_POLICY_AUTO);
   return gl_progs;
}

Evas_Object *
animator_window_add(Project *project)
{
   Evas_Object *top_layout;
   Evas_Object *panes;
   Evas_Object *bottom_panes;
   Evas_Object *scroller;
   Evas_Object *ic;
   Evas_Object *bt, *program_list_box;
   Animator *animator = NULL;

   assert(project != NULL);
   animator = (Animator *)mem_calloc(1, sizeof(Animator));
   animator->mwin = mw_add("dialog", _on_animator_ok, animator);
   animator->project = project;
   animator->group = tabs_current_group_get();

   assert(animator->mwin != NULL);

   animator->is_cycled = true;

   mw_title_set(animator->mwin, _("Program editor"));
   ic = elm_icon_add(animator->mwin);
   elm_icon_standard_set(ic, "animator");
   mw_icon_set(animator->mwin, ic);
   evas_object_event_callback_add(animator->mwin, EVAS_CALLBACK_DEL,
                                  _on_animator_close, animator);

   panes = elm_panes_add(animator->mwin);
   evas_object_size_hint_weight_set(panes, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panes, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_panes_horizontal_set(panes, true);
   evas_object_show(panes);

   animator->live_view = live_view_add(animator->mwin, true, animator->group);
   TODO("SPANK SPANK SPANK, forgetfull programmer! Where is comment about why do we need that object load?")
//   edje_object_signal_callback_add(animator->live->object, "show", "",
//                                   _on_object_load, animator);

   top_layout = elm_layout_add(animator->mwin);
   elm_layout_theme_set(top_layout, "layout", "animator", "live_view");
   elm_layout_content_set(top_layout, "swallow.content", animator->live_view);
   evas_object_show(top_layout);

   BUTTON_ADD(top_layout, bt, _("Play"));
   evas_object_size_hint_weight_set(bt, 0.0, 0.0);
   evas_object_smart_callback_add(bt, "clicked", _on_program_play, animator);
   elm_layout_content_set(top_layout, "swallow.button.play", bt);
   animator->program_controls.play = bt;

   BUTTON_ADD(top_layout, bt, _("Reset"));
   evas_object_size_hint_weight_set(bt, 0.0, 0.0);
   evas_object_smart_callback_add(bt, "clicked", _on_program_reset, animator);
   elm_layout_content_set(top_layout, "swallow.button.reset", bt);
   animator->program_controls.reset = bt;

   BUTTON_ADD(top_layout, bt, _("Cycled"));
   evas_object_size_hint_weight_set(bt, 0.0, 0.0);
   evas_object_smart_callback_add(bt, "clicked", _on_program_cycle, animator);
   elm_layout_content_set(top_layout, "swallow.button.cycled", bt);
   animator->program_controls.cycle = bt;

   bottom_panes = elm_panes_add(animator->mwin);
   evas_object_size_hint_weight_set(bottom_panes, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(bottom_panes, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_panes_content_left_size_set(bottom_panes, 0.2);
   evas_object_show(bottom_panes);

   BOX_ADD(animator->mwin, program_list_box, false, false);

   animator->gl_progs = _gl_progs_add(animator);
   elm_box_pack_end(program_list_box, animator->gl_progs);
   evas_object_show(animator->gl_progs);

   evas_object_size_hint_align_set(animator->gl_progs, EVAS_HINT_FILL,
                                   EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(animator->gl_progs, EVAS_HINT_EXPAND,
                                    EVAS_HINT_EXPAND);

   BUTTON_ADD(program_list_box, bt, _("New program"));
   evas_object_size_hint_weight_set(bt, 0.0, 0.0);
   evas_object_smart_callback_add(bt, "clicked", _on_bt_prog_add, animator);
   elm_box_pack_end(program_list_box, bt);
   TODO("Disable for few commits, making animator back alive")
   elm_object_disabled_set(bt, true);

   BUTTON_ADD(program_list_box, bt, _("Delete"));
   evas_object_size_hint_weight_set(bt, 0.0, 0.0);
   evas_object_smart_callback_add(bt, "clicked", _on_bt_prog_del, animator);
   elm_box_pack_end(program_list_box, bt);
   elm_object_part_content_set(bottom_panes, "left", program_list_box);
   TODO("Disable for few commits, making animator back alive")
   elm_object_disabled_set(bt, true);

   animator->program_area_layout = elm_layout_add(bottom_panes);
   elm_layout_theme_set(animator->program_area_layout, "layout", "animator", "program_area");
   elm_object_part_content_set(bottom_panes, "right", animator->program_area_layout);
   evas_object_show(animator->program_area_layout);
   elm_layout_signal_emit(animator->program_area_layout, "eflete,content,hide", "eflete");

   BUTTON_ADD(animator->program_area_layout, bt, "");
   elm_object_part_content_set(animator->program_area_layout, "swallow.button", bt);
   evas_object_smart_callback_add(bt, "clicked", _on_bt_mode_change, animator);
   IMAGE_ADD_NEW(bt, ic, "animator", "arrow-right");
   elm_layout_content_set(bt, "icon", ic);
   elm_object_style_set(bt, "anchor");

   SCROLLER_ADD(animator->program_area_layout, scroller);
   elm_scroller_policy_set(scroller, ELM_SCROLLER_POLICY_AUTO, ELM_SCROLLER_POLICY_AUTO);
   elm_object_part_content_set(animator->program_area_layout, "swallow.content",
                               scroller);
   animator->prop_scroller = scroller;

   TODO("Fix it furtherly.. need to make compilable for now")
   animator->program_editor = program_editor_add(animator->mwin,
                                                 animator->group,
                                                 animator->live_view);
   elm_object_content_set(animator->prop_scroller, animator->program_editor);
   evas_object_size_hint_weight_set(animator->program_editor, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(animator->program_editor, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(animator->program_editor);

   TODO("Fix it furtherly.. need to make compilable for now")
      animator->program_sequence = prog_sequence_add(animator->prop_scroller,
                                                     animator->group,
                                                     animator->live_view);
   evas_object_size_hint_weight_set(animator->program_sequence, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(animator->program_sequence, EVAS_HINT_FILL, EVAS_HINT_FILL);

   animator->sequence_mode = false;

   evas_object_smart_callback_add(animator->program_editor, PLAY_CB,
                                  _on_program_play_cb, animator);
   evas_object_smart_callback_add(animator->program_editor, PAUSE_CB,
                                  _on_program_pause_cb, animator);
   evas_object_smart_callback_add(animator->program_editor, NAME_CHANGED_CB,
                                  _on_program_name_change_cb, animator);

   evas_object_smart_callback_add(animator->program_sequence, PLAY_CB,
                                  _on_program_play_cb, animator);
   evas_object_smart_callback_add(animator->program_sequence, PAUSE_CB,
                                  _on_program_pause_cb, animator);
   evas_object_smart_callback_add(animator->program_sequence, SCROLL_CB,
                                  _on_program_scroll_cb, animator);

   BUTTON_ADD(animator->mwin, bt, _("Apply"));
   evas_object_smart_callback_add(bt, "clicked", _on_animator_save, NULL);
   elm_object_part_content_set(animator->mwin, "eflete.swallow.btn_ok", bt);
   TODO("Disable for few commits, making animator back alive")
   elm_object_disabled_set(bt, true);

   BUTTON_ADD(animator->mwin, bt, _("Ok"));
   evas_object_smart_callback_add(bt, "clicked", _on_animator_ok, animator);
   elm_object_part_content_set(animator->mwin, "eflete.swallow.btn_close", bt);

   elm_object_part_content_set(panes, "top", top_layout);
   elm_object_part_content_set(panes, "bottom", bottom_panes);
   elm_object_part_content_set(animator->mwin, NULL, panes);

   ui_menu_items_list_disable_set(ap.menu, MENU_ITEMS_LIST_MAIN, true);
   evas_object_event_callback_add(animator->mwin, EVAS_CALLBACK_DEL, _on_mwin_del, NULL);

   evas_object_show(animator->mwin);

   ap.modal_editor++;
   return animator->mwin;
}

#undef CALLBACK_UPDATE
#undef TRANS_ENTRIES_DEFAULT_SET
#undef TRANS_VAL_UPDATE
#undef REGEX_SET
