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

#include "colorclass_editor.h"
#include "main_window.h"

typedef struct _Colorclass_Item Colorclass_Item;
typedef struct _Colorclasses_Editor Colorclasses_Editor;

struct _Colorclass_Item
{
   Eina_Stringshare *name;
   int r1, g1, b1, a1;
   int r2, g2, b2, a2;
   int r3, g3, b3, a3;
};

struct _Colorclasses_Editor
{
   Project *pr;
   Evas_Object *mwin;
   Evas_Object *genlist;
   Colorclass_Item *old_ccl,*current_ccl, *next_ccl;
   Evas_Object *rect_color1, *obj_r, *obj_g, *obj_b, *obj_a;
   Evas_Object *rect_color2, *out_r, *out_g, *out_b, *out_a;
   Evas_Object *rect_color3, *sdw_r, *sdw_g, *sdw_b, *sdw_a;
   Evas_Object *label;
   Evas_Object *popup;
   Evas_Object *entry;
   Evas_Object *edit_obj;
   Eina_Bool changed;
};

static Elm_Entry_Filter_Accept_Set accept_name = {
   .accepted = NULL,
   .rejected = BANNED_SYMBOLS
};

static Elm_Genlist_Item_Class *_itc_ccl = NULL;

static void
_on_spinner_value_changed(void *data,
                          Evas_Object *obj __UNUSED__,
                          void *event_info __UNUSED__)
{
   Colorclasses_Editor *ccl_edit = (Colorclasses_Editor *)data;
   if (!ccl_edit->current_ccl) return;

   ccl_edit->current_ccl->r1 = elm_spinner_value_get(ccl_edit->obj_r);
   ccl_edit->current_ccl->g1 = elm_spinner_value_get(ccl_edit->obj_g);
   ccl_edit->current_ccl->b1 = elm_spinner_value_get(ccl_edit->obj_b);
   ccl_edit->current_ccl->a1 = elm_spinner_value_get(ccl_edit->obj_a);

   ccl_edit->current_ccl->r2 = elm_spinner_value_get(ccl_edit->out_r);
   ccl_edit->current_ccl->g2 = elm_spinner_value_get(ccl_edit->out_g);
   ccl_edit->current_ccl->b2 = elm_spinner_value_get(ccl_edit->out_b);
   ccl_edit->current_ccl->a2 = elm_spinner_value_get(ccl_edit->out_a);

   ccl_edit->current_ccl->r3 = elm_spinner_value_get(ccl_edit->sdw_r);
   ccl_edit->current_ccl->g3 = elm_spinner_value_get(ccl_edit->sdw_g);
   ccl_edit->current_ccl->b3 = elm_spinner_value_get(ccl_edit->sdw_b);
   ccl_edit->current_ccl->a3 = elm_spinner_value_get(ccl_edit->sdw_a);

   evas_object_color_set(ccl_edit->rect_color1,
                    ccl_edit->current_ccl->r1 * ccl_edit->current_ccl->a1 / 255,
                    ccl_edit->current_ccl->g1 * ccl_edit->current_ccl->a1 / 255,
                    ccl_edit->current_ccl->b1 * ccl_edit->current_ccl->a1 / 255,
                    ccl_edit->current_ccl->a1);
   evas_object_color_set(ccl_edit->rect_color2,
                    ccl_edit->current_ccl->r2 * ccl_edit->current_ccl->a2 / 255,
                    ccl_edit->current_ccl->g2 * ccl_edit->current_ccl->a2 / 255,
                    ccl_edit->current_ccl->b2 * ccl_edit->current_ccl->a2 / 255,
                    ccl_edit->current_ccl->a2);
   evas_object_color_set(ccl_edit->rect_color3,
                    ccl_edit->current_ccl->r3 * ccl_edit->current_ccl->a3 / 255,
                    ccl_edit->current_ccl->g3 * ccl_edit->current_ccl->a3 / 255,
                    ccl_edit->current_ccl->b3 * ccl_edit->current_ccl->a3 / 255,
                    ccl_edit->current_ccl->a3);
   edje_object_color_class_set(ccl_edit->label,
                          "colorclass_editor/text_example_colorclass",
                          ccl_edit->current_ccl->r1, ccl_edit->current_ccl->g1,
                          ccl_edit->current_ccl->b1, ccl_edit->current_ccl->a1,
                          ccl_edit->current_ccl->r2, ccl_edit->current_ccl->g2,
                          ccl_edit->current_ccl->b2, ccl_edit->current_ccl->a2,
                          ccl_edit->current_ccl->r3, ccl_edit->current_ccl->g3,
                          ccl_edit->current_ccl->b3, ccl_edit->current_ccl->a3);
   edje_edit_color_class_colors_set(ccl_edit->edit_obj, ccl_edit->current_ccl->name,
                          ccl_edit->current_ccl->r1, ccl_edit->current_ccl->g1,
                          ccl_edit->current_ccl->b1, ccl_edit->current_ccl->a1,
                          ccl_edit->current_ccl->r2, ccl_edit->current_ccl->g2,
                          ccl_edit->current_ccl->b2, ccl_edit->current_ccl->a2,
                          ccl_edit->current_ccl->r3, ccl_edit->current_ccl->g3,
                          ccl_edit->current_ccl->b3, ccl_edit->current_ccl->a3);
   ccl_edit->changed = true;

}

void _disable(Eina_Bool disabled, Colorclasses_Editor *ccl_edit)
{
   elm_object_disabled_set(ccl_edit->obj_r, disabled);
   elm_object_disabled_set(ccl_edit->obj_g, disabled);
   elm_object_disabled_set(ccl_edit->obj_b, disabled);
   elm_object_disabled_set(ccl_edit->obj_a, disabled);
   elm_object_disabled_set(ccl_edit->out_r, disabled);
   elm_object_disabled_set(ccl_edit->out_g, disabled);
   elm_object_disabled_set(ccl_edit->out_b, disabled);
   elm_object_disabled_set(ccl_edit->out_a, disabled);
   elm_object_disabled_set(ccl_edit->sdw_r, disabled);
   elm_object_disabled_set(ccl_edit->sdw_g, disabled);
   elm_object_disabled_set(ccl_edit->sdw_b, disabled);
   elm_object_disabled_set(ccl_edit->sdw_a, disabled);
}

void
_ccl_set(Colorclasses_Editor *ccl_edit, Colorclass_Item *ccl_it)
{
   ccl_edit->old_ccl = ccl_it;
   ccl_edit->current_ccl = ccl_it;

   elm_spinner_value_set(ccl_edit->obj_r, ccl_it->r1);
   elm_spinner_value_set(ccl_edit->obj_g, ccl_it->g1);
   elm_spinner_value_set(ccl_edit->obj_b, ccl_it->b1);
   elm_spinner_value_set(ccl_edit->obj_a, ccl_it->a1);

   elm_spinner_value_set(ccl_edit->out_r, ccl_it->r2);
   elm_spinner_value_set(ccl_edit->out_g, ccl_it->g2);
   elm_spinner_value_set(ccl_edit->out_b, ccl_it->b2);
   elm_spinner_value_set(ccl_edit->out_a, ccl_it->a2);

   elm_spinner_value_set(ccl_edit->sdw_r, ccl_it->r3);
   elm_spinner_value_set(ccl_edit->sdw_g, ccl_it->g3);
   elm_spinner_value_set(ccl_edit->sdw_b, ccl_it->b3);
   elm_spinner_value_set(ccl_edit->sdw_a, ccl_it->a3);

   _on_spinner_value_changed(ccl_edit, NULL, NULL);
}

static void
_on_ccl_editor_close(void *data,
                      Evas *e __UNUSED__,
                      Evas_Object *obj __UNUSED__,
                      void *event_info __UNUSED__)
{
   Colorclasses_Editor *ccl_edit = (Colorclasses_Editor *)data;
   Style *style = NULL;
   if (ccl_edit->changed)
     {
        GET_STYLE(ccl_edit->pr, style);
        if (style) style->isModify = true;
        project_changed();
     }

   evas_object_del(ccl_edit->rect_color1);
   evas_object_del(ccl_edit->rect_color2);
   evas_object_del(ccl_edit->rect_color3);
   ccl_edit->current_ccl = NULL;
   ccl_edit->pr = NULL;
   free(ccl_edit->old_ccl);
   free(ccl_edit->current_ccl);
   free(ccl_edit->next_ccl);
   free(ccl_edit);
}

/*colorclass popup buttons callbacks*/
static void
_on_apply_popup_btn_yes(void *data,
                        Evas_Object *obj __UNUSED__,
                        void *ei __UNUSED__)
{
   Colorclasses_Editor *ccl_edit = (Colorclasses_Editor *)data;
   Evas_Object *edje_edit_obj = NULL;
   GET_OBJ(ccl_edit->pr, edje_edit_obj);
   edje_edit_color_class_colors_set(edje_edit_obj, ccl_edit->current_ccl->name,
                          ccl_edit->current_ccl->r1, ccl_edit->current_ccl->g1,
                          ccl_edit->current_ccl->b1, ccl_edit->current_ccl->a1,
                          ccl_edit->current_ccl->r2, ccl_edit->current_ccl->g2,
                          ccl_edit->current_ccl->b2, ccl_edit->current_ccl->a2,
                          ccl_edit->current_ccl->r3, ccl_edit->current_ccl->g3,
                          ccl_edit->current_ccl->b3, ccl_edit->current_ccl->a3);

   _ccl_set(ccl_edit, ccl_edit->next_ccl);
   evas_object_del(ccl_edit->popup);
   ccl_edit->popup = NULL;
}


static void
_on_apply_popup_btn_no(void *data,
                       Evas_Object *obj __UNUSED__,
                       void *ei __UNUSED__)
{
   Colorclasses_Editor *ccl_edit = (Colorclasses_Editor *)data;
   ccl_edit->current_ccl = ccl_edit->old_ccl;
   _ccl_set(ccl_edit, ccl_edit->next_ccl);

   evas_object_del(ccl_edit->popup);
   ccl_edit->popup = NULL;
}

static void
_on_add_popup_btn_add(void *data,
                      Evas_Object *obj __UNUSED__,
                      void *ei __UNUSED__)
{
   Colorclasses_Editor *ccl_edit = (Colorclasses_Editor *)data;
   Colorclass_Item *it = NULL;
   Elm_Object_Item *glit_ccl = NULL;
   Evas_Object *edje_edit_obj = NULL;
   App_Data *ap = app_data_get();

   it = (Colorclass_Item *)mem_calloc(1, sizeof(Colorclass_Item));
   it->name = elm_entry_entry_get(ccl_edit->entry);

   if ((!it->name) || (!strcmp(it->name, "")))
     {
        NOTIFY_WARNING(_("Color class name can not be empty!"));
        free(it);
        return;
     }

   GET_OBJ(ccl_edit->pr, edje_edit_obj);
   if (!edje_edit_color_class_add(edje_edit_obj, eina_stringshare_add(it->name)))
     {
        NOTIFY_WARNING(_("Color class name must be unique!"));
        free(it);
        return;
     }

   glit_ccl = elm_genlist_item_append(ccl_edit->genlist, _itc_ccl, it, NULL,
                                    ELM_GENLIST_ITEM_NONE, NULL, NULL);
   elm_genlist_item_selected_set(glit_ccl, EINA_TRUE);

   Part *part = ui_widget_list_selected_part_get(ui_block_widget_list_get(ap));
   ui_property_state_unset(ui_block_property_get(ap));
   ui_property_state_set(ui_block_property_get(ap), part);
   evas_object_del(ccl_edit->popup);
   ccl_edit->popup = NULL;
   _disable(EINA_FALSE, ccl_edit);
   ccl_edit->changed = true;
}

static void
_on_add_popup_btn_cancel(void *data,
                         Evas_Object *obj __UNUSED__,
                         void *ei __UNUSED__)
{
   Colorclasses_Editor *ccl_edit = (Colorclasses_Editor *)data;
   evas_object_del(ccl_edit->popup);
   ccl_edit->popup = NULL;
}

/*Colorclass editor buttons callbacks*/
static void
_on_btn_cancel(void *data,
               Evas_Object *obj __UNUSED__,
               void *event_info __UNUSED__)
{
   Evas_Object *mwin = (Evas_Object *)data;
   evas_object_del(mwin);
}

static void
_on_btn_add(void *data,
            Evas_Object *obj __UNUSED__,
            void *event_info __UNUSED__)
{
   Colorclasses_Editor *ccl_edit = (Colorclasses_Editor *)data;

   Evas_Object *box, *bt_yes, *bt_no;
   Evas_Object *ccl_box, *ccl_label;

   ccl_edit->popup = elm_popup_add(ccl_edit->mwin);
   elm_object_style_set(ccl_edit->popup, "eflete");
   elm_object_part_text_set(ccl_edit->popup, "title,text", _("Add color class:"));

   box = elm_box_add(ccl_edit->popup);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(box, EVAS_HINT_FILL, EVAS_HINT_FILL);

   ccl_box = elm_box_add(box);
   elm_box_horizontal_set(ccl_box, EINA_TRUE);
   evas_object_size_hint_weight_set(ccl_box, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(ccl_box, EVAS_HINT_FILL, 0.0);
   evas_object_show(ccl_box);

   LABEL_ADD(ccl_box, ccl_label, _("Color class name: "))
   elm_box_pack_end(ccl_box, ccl_label);

   EWE_ENTRY_ADD(ccl_box, ccl_edit->entry, true, DEFAULT_STYLE);
   elm_entry_markup_filter_append(ccl_edit->entry, elm_entry_filter_accept_set,
                                  &accept_name);
   elm_object_part_text_set(ccl_edit->entry, "guide",
                            _("Type new color class name here."));
   elm_box_pack_end(ccl_box, ccl_edit->entry);

   elm_box_pack_end(box, ccl_box);
   elm_object_content_set(ccl_edit->popup, box);
   evas_object_show(box);

   BUTTON_ADD(ccl_edit->popup, bt_yes, _("Ok"));
   evas_object_smart_callback_add(bt_yes, "clicked", _on_add_popup_btn_add,
                                  ccl_edit);
   elm_object_part_content_set(ccl_edit->popup, "button1", bt_yes);

   BUTTON_ADD(ccl_edit->popup, bt_no, _("Cancel"));
   evas_object_smart_callback_add(bt_no, "clicked", _on_add_popup_btn_cancel,
                                  ccl_edit);
   elm_object_part_content_set(ccl_edit->popup, "button2", bt_no);

   evas_object_show(ccl_edit->popup);
}

static char *
_item_ccl_label_get(void *data,
                    Evas_Object *obj __UNUSED__,
                    const char *part __UNUSED__)
{
   Colorclass_Item *ccl_it = (Colorclass_Item *)data;
   return  strdup(ccl_it->name);
}

static void
_item_ccl_del(void *data,
                    Evas_Object *obj __UNUSED__)
{
   Colorclass_Item *ccl_it = (Colorclass_Item *)data;
   eina_stringshare_del(ccl_it->name);
}


static void
_on_ccl_selected(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *event_info)
{
   Colorclasses_Editor *ccl_edit = (Colorclasses_Editor *)data;
   Elm_Object_Item *glit = (Elm_Object_Item *)event_info;
   Colorclass_Item *ccl_it = elm_object_item_data_get(glit);
   Evas_Object *button;

   if ((!ccl_edit->current_ccl)
        ||(ccl_edit->old_ccl->r1 == ccl_edit->current_ccl->r1
           && ccl_edit->old_ccl->r2 == ccl_edit->current_ccl->r2
           && ccl_edit->old_ccl->r3 == ccl_edit->current_ccl->r3
           && ccl_edit->old_ccl->g1 == ccl_edit->current_ccl->g1
           && ccl_edit->old_ccl->g2 == ccl_edit->current_ccl->g2
           && ccl_edit->old_ccl->g3 == ccl_edit->current_ccl->g3
           && ccl_edit->old_ccl->b1 == ccl_edit->current_ccl->b1
           && ccl_edit->old_ccl->b2 == ccl_edit->current_ccl->b2
           && ccl_edit->old_ccl->b3 == ccl_edit->current_ccl->b3
           && ccl_edit->old_ccl->a1 == ccl_edit->current_ccl->a1
           && ccl_edit->old_ccl->a2 == ccl_edit->current_ccl->a2
           && ccl_edit->old_ccl->a3 == ccl_edit->current_ccl->a3))
     {
        _ccl_set(ccl_edit, ccl_it);
        return;
     }
   ccl_edit->popup = elm_popup_add(ccl_edit->mwin);
   elm_object_style_set(ccl_edit->popup, "eflete");
   elm_object_part_text_set(ccl_edit->popup, "title,text", _("Apply changes?"));

   BUTTON_ADD(ccl_edit->popup, button, _("Yes"));
   evas_object_smart_callback_add(button, "clicked", _on_apply_popup_btn_yes,
                                  ccl_edit);
   elm_object_part_content_set(ccl_edit->popup, "button1", button);

   BUTTON_ADD(ccl_edit->popup, button, _("No"));
   evas_object_smart_callback_add(button, "clicked", _on_apply_popup_btn_no,
                                  ccl_edit);
   elm_object_part_content_set(ccl_edit->popup, "button2", button);

   ccl_edit->next_ccl = ccl_it;
   evas_object_show(ccl_edit->popup);
}

static void
_on_btn_del(void *data,
            Evas_Object *obj __UNUSED__,
            void *event_info __UNUSED__)
{
   Colorclasses_Editor *ccl_edit = (Colorclasses_Editor *)data;
   Evas_Object *edje_edit_obj;
   App_Data *ap = app_data_get();
   if (!ccl_edit->current_ccl) return;
   GET_OBJ(ccl_edit->pr, edje_edit_obj);
   edje_edit_color_class_del(edje_edit_obj, ccl_edit->current_ccl->name);
   Elm_Object_Item *it = elm_genlist_selected_item_get(ccl_edit->genlist);
   Elm_Object_Item *next = elm_genlist_item_next_get(it);

   Part *part = ui_widget_list_selected_part_get(ui_block_widget_list_get(ap));
   ui_property_state_unset(ui_block_property_get(ap));
   ui_property_state_set(ui_block_property_get(ap), part);

   if (!next) next = elm_genlist_item_prev_get(it);
   if (next)
      elm_genlist_item_selected_set(next, EINA_TRUE);
   else
      {
         ccl_edit->current_ccl = NULL;
         _disable(EINA_TRUE, ccl_edit);
      }
   elm_object_item_del(it);
}
/*button callbacks end*/

Eina_Bool
_colorclass_viewer_init(Colorclasses_Editor *ccl_edit)
{
   int r1, r2, r3, g1, g2, g3, b1, b2, b3, a1, a2, a3;
   const char *ccname = NULL;
   Eina_List *cclist = NULL;
   Eina_List *l = NULL;
   Colorclass_Item *it = NULL;
   Evas_Object *edje_edit_obj = NULL;

   GET_OBJ(ccl_edit->pr, edje_edit_obj);
   ccl_edit->edit_obj = edje_edit_obj;
   cclist = edje_edit_color_classes_list_get(edje_edit_obj);

   EINA_LIST_FOREACH(cclist, l, ccname)
     {
        it = (Colorclass_Item *)mem_calloc(1, sizeof(Colorclass_Item));
        if (!edje_edit_color_class_colors_get(edje_edit_obj, ccname,
                                     &r1, &g1, &b1, &a1,
                                     &r2, &g2, &b2, &a2,
                                     &r3, &g3, &b3, &a3))
          {
             free(it);
             ERR("Coud'nt get color's from colorclass %s.", ccname);
             continue;
          }
        it->r1 = r1; it->g1 = g1;
        it->b1 = b1; it->a1 = a1;
        it->r2 = r2; it->g2 = g2;
        it->b2 = b2; it->a2 = a2;
        it->r3 = r3; it->g3 = g3;
        it->b3 = b3; it->a3 = a3;

        it->name = eina_stringshare_add(ccname);
        elm_genlist_item_append(ccl_edit->genlist, _itc_ccl, it, NULL,
                                ELM_GENLIST_ITEM_NONE, NULL, NULL);
     }

   if (elm_genlist_items_count(ccl_edit->genlist) == 0)
      _disable(EINA_TRUE, ccl_edit);
   else
      elm_genlist_item_selected_set(elm_genlist_first_item_get(ccl_edit->genlist),
                                      EINA_TRUE);
   eina_list_free(cclist);
   return true;
}

static void
_on_mwin_del(void * data,
             Evas *e __UNUSED__,
             Evas_Object *obj __UNUSED__,
             void *event_info __UNUSED__)
{
   App_Data *ap = (App_Data *)data;
   ui_menu_items_list_disable_set(ap->menu, MENU_ITEMS_LIST_MAIN, false);
   ap->modal_editor--;
}

Evas_Object *
colorclass_viewer_add(Project *project)
{
   Evas_Object *button,  *panes;
   Evas_Object *label, *color;
   Evas_Object *box, *bottom_box, *param_box, *scr_box;
   Evas_Object *scroller = NULL;
   Colorclasses_Editor *ccl_edit = NULL;
   /* temporary solution, while it not moved to modal window */
   App_Data *ap = app_data_get();

   if (!project)
     {
        ERR("Project isn't opened");
        return NULL;
     }

   ccl_edit = (Colorclasses_Editor *)mem_calloc(1, sizeof(Colorclasses_Editor));
   ccl_edit->changed = false;
   ccl_edit->pr = project;
   ccl_edit->mwin = mw_add(NULL, NULL);
   mw_title_set(ccl_edit->mwin, _("Color class editor"));
   evas_object_event_callback_add(ccl_edit->mwin, EVAS_CALLBACK_FREE,
                                  _on_ccl_editor_close, ccl_edit);


   panes = elm_panes_add(ccl_edit->mwin);
   elm_object_style_set(panes, DEFAULT_STYLE);
   evas_object_size_hint_weight_set(panes, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panes, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_panes_content_right_size_set(panes, 0.3);
   evas_object_show(panes);
   elm_win_inwin_content_set(ccl_edit->mwin, panes);

   BOX_ADD(panes, box, false, false);

   ccl_edit->genlist = elm_genlist_add(box);
   elm_box_pack_end(box, ccl_edit->genlist);
   elm_object_style_set(ccl_edit->genlist, DEFAULT_STYLE);
   evas_object_size_hint_weight_set(ccl_edit->genlist, EVAS_HINT_EXPAND,
                                                            EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(ccl_edit->genlist, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_part_content_set(panes, "left", box);
   evas_object_show(ccl_edit->genlist);
   evas_object_smart_callback_add(ccl_edit->genlist, "selected", _on_ccl_selected,
                                                                        ccl_edit);

   if (!_itc_ccl)
     {
        _itc_ccl = elm_genlist_item_class_new();
        _itc_ccl->item_style = DEFAULT_STYLE;
        _itc_ccl->func.text_get = _item_ccl_label_get;
        _itc_ccl->func.content_get = NULL;
        _itc_ccl->func.state_get = NULL;
        _itc_ccl->func.del = _item_ccl_del;
     }

   BOX_ADD(panes, bottom_box, true, false)
   elm_box_pack_end(box, bottom_box);

   BUTTON_ADD(ccl_edit->mwin, button, _("Add"));
   evas_object_smart_callback_add(button, "clicked", _on_btn_add, ccl_edit);
   elm_box_pack_end(bottom_box, button);

   BUTTON_ADD(ccl_edit->mwin, button, _("Delete"));
   evas_object_smart_callback_add(button, "clicked", _on_btn_del, ccl_edit);
   elm_box_pack_end(bottom_box, button);

   BUTTON_ADD(ccl_edit->mwin, button, _("Close"));
   evas_object_smart_callback_add(button, "clicked", _on_btn_cancel, ccl_edit->mwin);
   elm_box_pack_end(bottom_box, button);

   SCROLLER_ADD(panes, scroller);
   evas_object_size_hint_align_set(scroller, 0.5, 1.0);
   elm_object_part_content_set(panes, "right", scroller);


   BOX_ADD(scroller, scr_box, false, false);
   elm_object_content_set(scroller, scr_box);

   ccl_edit->label = edje_object_add(evas_object_evas_get(ccl_edit->mwin));
   if (!edje_object_file_set(ccl_edit->label,
                             EFLETE_EDJ,
                             "base/colorclass_editor/text_example"))
     ERR("Couldn't load layout for text example field!");
   edje_object_part_text_set(ccl_edit->label, "text_example", _("EXAMPLE"));
   evas_object_size_hint_align_set(ccl_edit->label, -1, -1);
   evas_object_show(ccl_edit->label);

#define _SPINNER_ADD(spinner, format, box) \
   spinner = elm_spinner_add(box); \
   elm_object_style_set(spinner, DEFAULT_STYLE); \
   elm_spinner_min_max_set(spinner, 0, 255); \
   elm_spinner_interval_set(spinner, 0.4); \
   elm_spinner_label_format_set(spinner, format); \
   evas_object_size_hint_min_set(spinner, 100, 0); \
   evas_object_size_hint_align_set(spinner, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   evas_object_smart_callback_add(spinner, "changed", \
                           _on_spinner_value_changed, ccl_edit); \
   elm_box_pack_end(box, spinner); \
   evas_object_show(spinner);

#define _COLOR_ADD(rect, title, box) \
   LABEL_ADD(box, label, title) \
   elm_box_pack_end(box, label); \
   color = edje_object_add(evas_object_evas_get(ccl_edit->mwin)); \
   rect = evas_object_rectangle_add(evas_object_evas_get(ccl_edit->mwin)); \
   if (!edje_object_file_set(color, EFLETE_EDJ, "base/colorclass_editor/color_example")) \
     ERR("Could not set style for color example!"); \
   edje_object_part_swallow(color, "color_example", rect); \
   evas_object_size_hint_weight_set(color, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND); \
   evas_object_size_hint_min_set(color, 150, 35); \
   evas_object_size_hint_align_set(color, EVAS_HINT_FILL, EVAS_HINT_FILL); \
   elm_box_pack_end(box, color); \
   evas_object_show(rect); \
   evas_object_show(color);


   BOX_ADD(scr_box, param_box, false, true);

   elm_box_pack_start(param_box, ccl_edit->label);
   _COLOR_ADD(ccl_edit->rect_color1,_("Object color"), param_box)
   _SPINNER_ADD(ccl_edit->obj_r,_("R:%3.0f"), param_box)
   _SPINNER_ADD(ccl_edit->obj_g,_("G:%3.0f"), param_box)
   _SPINNER_ADD(ccl_edit->obj_b,_("B:%3.0f"), param_box)
   _SPINNER_ADD(ccl_edit->obj_a,_("A:%3.0f"), param_box)
   elm_box_pack_end(scr_box, param_box);

   BOX_ADD(scr_box, param_box, false, true);
   _COLOR_ADD(ccl_edit->rect_color2,_("Outline color"), param_box)
   _SPINNER_ADD(ccl_edit->out_r,_("R:%3.0f"), param_box)
   _SPINNER_ADD(ccl_edit->out_g,_("G:%3.0f"), param_box)
   _SPINNER_ADD(ccl_edit->out_b,_("B:%3.0f"), param_box)
   _SPINNER_ADD(ccl_edit->out_a,_("A:%3.0f"), param_box)
   elm_box_pack_end(scr_box, param_box);

   BOX_ADD(scr_box, param_box, false, true);
   _COLOR_ADD(ccl_edit->rect_color3,_("Shadow color"), param_box)
   _SPINNER_ADD(ccl_edit->sdw_r,_("R:%3.0f"), param_box)
   _SPINNER_ADD(ccl_edit->sdw_g,_("G:%3.0f"), param_box)
   _SPINNER_ADD(ccl_edit->sdw_b,_("B:%3.0f"), param_box)
   _SPINNER_ADD(ccl_edit->sdw_a,_("A:%3.0f"), param_box)
   elm_box_pack_end(scr_box, param_box);

#undef _SPINNER_ADD
#undef _COLOR_ADD

   if (!_colorclass_viewer_init(ccl_edit))
     {
        NOTIFY_ERROR(_("Failed initialize colorclasses editor"));
        free(ccl_edit);
        return NULL;
     }

   ui_menu_items_list_disable_set(ap->menu, MENU_ITEMS_LIST_MAIN, true);
   evas_object_event_callback_add(ccl_edit->mwin, EVAS_CALLBACK_DEL, _on_mwin_del, ap);

   evas_object_show(ccl_edit->mwin);

   ap->modal_editor++;

   return ccl_edit->mwin;
}
