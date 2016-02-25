#include "ewe_private.h"
#include "ewe_widget_combobox.h"

EAPI Eo_Op EWE_OBJ_COMBOBOX_BASE_ID = EO_NOOP;

#define MY_CLASS EWE_COMBOBOX_CLASS
#define MY_CLASS_NAME "Ewe_Combobox"
#define MY_CLASS_NAME_LEGACY "ewe_combobox"
#define MIN_ITEMS 5
#define ITEM_H 20


static void _focus_out_cb(void *, Evas_Object *, void *);

static const Evas_Smart_Cb_Description _smart_callbacks[] = {
   {NULL, NULL}
};

/*----------------------internal helpers functions----------------------------*/


static void
_item_data_free(Ewe_Combobox_Item *item)
{
   if (!item) return;
   eina_stringshare_del(item->title);
   eina_stringshare_del(item->style);
   free(item);
}

static void
_win_del(void *data)
{
   Ewe_Combobox_Smart_Data  *sd = (Ewe_Combobox_Smart_Data  *) data;
   if (sd->win)
     {
        evas_object_smart_callback_del(sd->win, "unfocused", _focus_out_cb);
        evas_object_del(sd->win);
        sd->win = NULL;
     }
}

static void
_focus_out_cb(void *data,
          Evas_Object *obj EINA_UNUSED,
          void *einfo EINA_UNUSED)
{
   Ewe_Combobox_Smart_Data  *sd = (Ewe_Combobox_Smart_Data  *) data;
   if (!sd->win) return;

   sd->expanded = EINA_FALSE;
   edje_object_signal_emit(sd->combobox, "ewe,state,default", "ewe");
   edje_object_signal_emit(sd->combobox, "btn,show", "ewe");
   elm_object_scroll_freeze_pop(sd->obj);
   evas_object_smart_callback_call(sd->obj, "collapsed", NULL);
   ecore_job_add(_win_del, sd);
}

static void
_selected_cb(void *data,
             Evas_Object *obj EINA_UNUSED,
             const char *emission EINA_UNUSED,
             const char *source EINA_UNUSED)
{
   Ewe_Combobox_Item *item = (Ewe_Combobox_Item *)data;
   EWE_COMBOBOX_DATA_GET(item->owner, sd);

   edje_object_part_text_set(sd->combobox, "ewe.text", item->title);
   sd->selected = item;
   _focus_out_cb(sd, NULL, NULL);
   evas_object_smart_callback_call(sd->obj, "selected", item);
}

static void
_combobox_geometry_calc(Evas *evas,
                        Ewe_Combobox_Smart_Data *sd,
                        int *width,
                        int *height)
{
   int b_w, b_h, scr_h;
   int top_widget_pos_x, top_widget_pos_y, sc_y_offset, list_count;
   Evas_Coord combo_x, combo_y, combo_width, combo_height;
   Eina_List *l;
   Ewe_Combobox_Item *item;
   int holds_item;

   edje_object_size_min_calc(sd->combobox, NULL, &combo_height);
   evas_object_geometry_get(sd->combobox, &combo_x, &combo_y, &combo_width, NULL);

   evas_object_geometry_get(sd->top_win, &top_widget_pos_x, &top_widget_pos_y, NULL, NULL);
   elm_win_screen_size_get(sd->top_win, NULL, &sc_y_offset, NULL, &scr_h);

   scr_h += sc_y_offset;
   combo_x += top_widget_pos_x;
   combo_y += combo_height + top_widget_pos_y;

   EINA_LIST_FOREACH(sd->items, l, item)
     {
        item->content = edje_object_add(evas);
        if (item->style)
          edje_object_file_set(item->content, EWE_THEME, item->style);
        else
          edje_object_file_set(item->content, EWE_THEME,
                               "ewe/combobox/item/default");
        evas_object_size_hint_min_set(item->content, combo_width, ITEM_H);
        evas_object_size_hint_weight_set(item->content, EVAS_HINT_EXPAND, 0);
        evas_object_size_hint_align_set(item->content, EVAS_HINT_FILL, 0.5);
        edje_object_part_text_set(item->content, "ewe.text", item->title);
        evas_object_show(item->content);
        edje_object_size_min_calc(item->content, &b_w, NULL);
        if ((*width) < b_w)
          *width = b_w;
     }

   list_count = eina_list_count(sd->items);
   *height = b_h = list_count * ITEM_H + ITEM_H / 4;

   if (b_h > scr_h - combo_y)
     {
        holds_item = (scr_h - combo_y) / ITEM_H;
        *height = holds_item * ITEM_H;
        if (holds_item < MIN_ITEMS)
          {
             *height = b_h;
             int tmp = combo_y;
             combo_y -= combo_height + b_h;
             if (combo_y < 0)
               {
                  combo_y = 0;
                  *height = tmp - combo_height;
               }
          }
     }

   if ((*width) < combo_width)
     (*width) = combo_width;

   evas_object_geometry_set(sd->win, combo_x, combo_y, *width, *height);
}

static void
_exp_window_create(Evas **evas, Ewe_Combobox_Smart_Data *sd)
{
   Evas_Object *win;
   Eina_List *l;
   Ewe_Combobox_Item *item;
   int height, width = 0;

   if (sd->win)
     {
        evas_object_smart_callback_del(sd->win, "unfocused", _focus_out_cb);
        evas_object_del(sd->win);
     }
   win = elm_win_add(sd->top_win, "expand_window", ELM_WIN_DROPDOWN_MENU);
   elm_win_autodel_set(win, EINA_TRUE);
   elm_win_borderless_set(win, EINA_TRUE);
   elm_win_focus_highlight_enabled_set(win, EINA_TRUE);
   elm_win_activate(win);
   elm_win_shaped_set(win, EINA_TRUE);
   sd->win = win;
   evas_object_smart_callback_add(sd->win, "unfocused", _focus_out_cb, sd);
   *evas = evas_object_evas_get(win);
   _combobox_geometry_calc(*evas, sd, &width, &height);
   evas_object_show(win);

   sd->expander = edje_object_add(*evas);
   edje_object_file_set(sd->expander, EWE_THEME, "ewe/combobox/expander/default");
   evas_object_resize(sd->expander, width, height);
   evas_object_show(sd->expander);

   sd->scroller = elm_scroller_add(win);
   elm_object_style_set(sd->scroller, "ewe/default");
   edje_object_part_swallow(sd->expander, "swallow.part", sd->scroller);
   evas_object_size_hint_weight_set(sd->scroller, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_scroller_policy_set(sd->scroller, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_AUTO);
   elm_scroller_content_min_limit(sd->scroller, EINA_TRUE, EINA_FALSE);
   evas_object_focus_set(sd->scroller, EINA_TRUE);
   evas_object_show(sd->scroller);

   sd->items_box = evas_object_box_add(*evas);
   elm_object_content_set(sd->scroller, sd->items_box);
   evas_object_box_align_set(sd->items_box, 0.0, 0.0);
   evas_object_box_layout_set(sd->items_box, evas_object_box_layout_vertical, NULL, NULL);
   evas_object_show(sd->items_box);

   EINA_LIST_FOREACH(sd->items, l, item)
     {
        evas_object_box_append(sd->items_box, item->content);
        edje_object_signal_callback_add(item->content, "select", "ewe",
                                        _selected_cb, item);
     }
}

static void
_ewe_combobox_expand(Evas_Object *obj)
{
   Evas *evas = NULL;

   EWE_COMBOBOX_DATA_GET(obj, sd);

   sd->expanded = EINA_TRUE;
   edje_object_signal_emit(sd->combobox, "ewe,state,expanded", "ewe");
   edje_object_signal_emit(sd->combobox, "btn,hide", "ewe");

   _exp_window_create(&evas, sd);

   elm_object_scroll_freeze_push(sd->obj);
   evas_object_smart_callback_call(sd->obj, "expanded", evas);
}

static void
_on_clicked(void *data,
            Evas_Object *obj EINA_UNUSED,
            const char *emission EINA_UNUSED,
            const char *source EINA_UNUSED)
{
   Ewe_Combobox_Smart_Data  *sd = (Ewe_Combobox_Smart_Data  *) data;
   if (!eina_list_count(sd->items))
     {
        ERR("Combobox items list is empty");
        return;
     }
   _ewe_combobox_expand(sd->obj);
}

/*----------------------internal realisation API------------------------------*/

EOLIAN static Eina_Bool
_ewe_combobox_expanded_get(Eo *obj EINA_UNUSED,
                           Ewe_Combobox_Smart_Data  *sd)
{
   return sd->expanded;
}

EOLIAN static Eina_Bool
_ewe_combobox_text_set(Eo *obj EINA_UNUSED,
                       Ewe_Combobox_Smart_Data  *sd,
                       Eina_Stringshare         *title)
{
   if (!title) title = "";
   edje_object_part_text_set(sd->combobox, "ewe.text", title);
   return EINA_TRUE;
}

EOLIAN static Ewe_Combobox_Item *
_ewe_combobox_item_add(Eo                       *obj,
                       Ewe_Combobox_Smart_Data  *sd,
                       Eina_Stringshare         *title)
{
   Ewe_Combobox_Item *ret = NULL;

   ret = malloc(sizeof(Ewe_Combobox_Item));
   ret->owner = obj;
   if (!title) title = "";
   ret->title = eina_stringshare_add(title);

   ret->index = eina_list_count(sd->items);
   ret->style = NULL;
   sd->items = eina_list_append(sd->items, ret);

   return ret;
}


EOLIAN static Eina_Bool
_ewe_combobox_item_del(Eo                       *obj,
                       Ewe_Combobox_Smart_Data  *sd,
                       Ewe_Combobox_Item        *item)
{
   if ((!item) || (item->owner != obj)) return EINA_FALSE;
   Eina_List *l;
   int selindex;
   Ewe_Combobox_Item *data;

   if (sd->selected)
     {
        if (item == sd->selected)
          {
             selindex = item->index - 1;
             if (selindex >= 0)
               ewe_combobox_select_item_set(obj, selindex);
             else
               ewe_combobox_select_item_set(obj, selindex + 2);
          }
     }
   sd->items = eina_list_remove(sd->items, item);

   l = eina_list_nth_list(sd->items, item->index);
   for (data = eina_list_data_get(l); l ;
        l = eina_list_next(l), data = eina_list_data_get(l))
     data->index--;

   _item_data_free(item);

   return EINA_TRUE;
}


EOLIAN const Eina_List *
_ewe_combobox_items_list_get(Eo *obj EINA_UNUSED,
                             Ewe_Combobox_Smart_Data *sd)
{
   return sd->items;
}

EOLIAN Eina_Bool
_ewe_combobox_items_list_free(Eo *obj EINA_UNUSED,
                              Ewe_Combobox_Smart_Data *sd,
                              Eina_Bool empty)
{
   Ewe_Combobox_Item *it;
   if (eina_list_count(sd->items) == 0) return EINA_FALSE;
   EINA_LIST_FREE(sd->items, it)
     {
        _item_data_free(it);
     }
   if (empty)
     edje_object_part_text_set(sd->combobox, "ewe.text", "");
   return EINA_TRUE;
}

EOLIAN static Eina_Bool
_ewe_combobox_item_title_set(Eo *obj EINA_UNUSED,
                             Ewe_Combobox_Smart_Data   *sd,
                             int                       index,
                             Eina_Stringshare          *title)
{
   if (((int)eina_list_count(sd->items) < (index + 1)) || (index < 0))
     return EINA_FALSE;
   Ewe_Combobox_Item *item;
   item = eina_list_nth(sd->items, index);
   eina_stringshare_del(item->title);
   if (!title) title = "";
   item->title = eina_stringshare_add(title);
   if (item == sd->selected)
     edje_object_part_text_set(sd->combobox, "ewe.text", item->title);

   return EINA_TRUE;
}

EOLIAN static Eina_Stringshare *
_ewe_combobox_item_title_get(Eo *obj EINA_UNUSED,
                             Ewe_Combobox_Smart_Data  *sd,
                             const int                index)
{
   if (((int)eina_list_count(sd->items) < (index + 1)) || (index < 0))
     return NULL;
   Ewe_Combobox_Item *item;
   item = eina_list_nth(sd->items, index);
   return item->title;
}

EOLIAN static Eina_Bool
_ewe_combobox_select_item_set(Eo *obj EINA_UNUSED,
                              Ewe_Combobox_Smart_Data  *sd,
                              int                      index)
{
   if (((int)eina_list_count(sd->items) < (index + 1)) || (index < 0))
     return EINA_FALSE;
   Ewe_Combobox_Item *item;
   item = eina_list_nth(sd->items, index);
   sd->selected = item;
   return edje_object_part_text_set(sd->combobox, "ewe.text", item->title);
}

EOLIAN static Ewe_Combobox_Item *
_ewe_combobox_select_item_get(Eo *obj EINA_UNUSED,
                              Ewe_Combobox_Smart_Data  *sd)
{
   if (!sd->selected) return NULL;
   return sd->selected;
}

EOLIAN static Eina_Bool
_ewe_combobox_elm_widget_disable(Eo *obj,
                                 Ewe_Combobox_Smart_Data *sd)
{
   if (elm_object_disabled_get(obj))
     {
        edje_object_signal_emit(sd->combobox, "ewe,state,disabled", "ewe");
        edje_object_signal_emit(sd->combobox, "btn,hide", "ewe");
        edje_object_signal_emit(sd->combobox, "ewe,disable,active", "ewe");
     }
   else
     {
        edje_object_signal_emit(sd->combobox, "ewe,state,default", "ewe");
        edje_object_signal_emit(sd->combobox, "btn,show", "ewe");
        edje_object_signal_emit(sd->combobox, "ewe,disable,unactive", "ewe");
     }

   return EINA_TRUE;
}

EOLIAN static Eina_Bool
_ewe_combobox_item_style_set(Eo *obj EINA_UNUSED,
                             Ewe_Combobox_Smart_Data *sd EINA_UNUSED,
                             Ewe_Combobox_Item *item,
                             Eina_Stringshare *style)
{
   if (!style)
     return EINA_FALSE;
   item->style = eina_stringshare_printf("ewe/combobox/item/%s", style);
   if (!item->style)
     return EINA_FALSE;
   return EINA_TRUE;
}

EOLIAN static Eina_Bool
_ewe_combobox_style_set(Eo *obj EINA_UNUSED,
                        Ewe_Combobox_Smart_Data *sd EINA_UNUSED,
                        Eina_Stringshare *style)
{
   Eina_Bool res;
   Eina_Stringshare *st;
   if (!style)
     return EINA_FALSE;

   st = eina_stringshare_printf("ewe/combobox/base/%s", style);
   res = edje_object_file_set(sd->combobox, EWE_THEME, st);

   eina_stringshare_del(st);
   return res;
}

EOLIAN static Eina_Bool
_ewe_combobox_content_set(Eo *obj EINA_UNUSED,
                          Ewe_Combobox_Smart_Data *sd,
                          Eina_Stringshare *part,
                          Evas_Object *content)
{
   return edje_object_part_swallow(sd->combobox, part, content);
}

/*------------------------Smart functions ----------------------------*/

EOLIAN static void
_ewe_combobox_evas_object_smart_add(Eo *obj, Ewe_Combobox_Smart_Data *sd)
{
   ELM_WIDGET_DATA_GET_OR_RETURN(obj, wd);

   eo_do_super(obj, MY_CLASS, evas_obj_smart_add());
   elm_widget_sub_object_parent_add(obj);

   elm_widget_can_focus_set(obj, EINA_TRUE);
   sd->obj = obj;
   sd->expanded = EINA_FALSE;

   sd->top_win = elm_object_top_widget_get(obj);
   sd->combobox = edje_object_add(evas_object_evas_get(obj));
   evas_object_smart_member_add(sd->combobox, obj);
   edje_object_file_set(sd->combobox, EWE_THEME, "ewe/combobox/base/default");
   edje_object_signal_callback_add(sd->combobox, "clicked", "ewe", _on_clicked, sd);
   sd->items = NULL;
}

EOLIAN static void
_ewe_combobox_evas_object_smart_del(Eo *obj EINA_UNUSED,
                                    Ewe_Combobox_Smart_Data *sd)
{
   Ewe_Combobox_Item *it;
   EINA_LIST_FREE(sd->items, it)
     _item_data_free(it);
   evas_object_del(sd->win);
   evas_object_smart_member_del(sd->combobox);
   evas_object_del(sd->combobox);
}

EOLIAN static void
_ewe_combobox_evas_object_smart_show(Eo *obj EINA_UNUSED,
                                     Ewe_Combobox_Smart_Data *sd)
{
   eo_do_super(obj, MY_CLASS, evas_obj_smart_show());
   evas_object_show(sd->combobox);
}

EOLIAN static void
_ewe_combobox_evas_object_smart_hide(Eo *obj EINA_UNUSED,
                                     Ewe_Combobox_Smart_Data *sd)
{
   eo_do_super(obj, MY_CLASS, evas_obj_smart_hide());
   evas_object_hide(sd->combobox);
}

EAPI Evas_Object *
ewe_combobox_add(Evas_Object *parent)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(parent, NULL);
   Evas_Object *obj = eo_add(MY_CLASS, parent);
   return obj;
}

EOLIAN static void
_ewe_combobox_evas_object_smart_move(Eo                      *obj,
                                     Ewe_Combobox_Smart_Data *sd,
                                     Evas_Coord              x,
                                     Evas_Coord              y)
{
   eo_do_super(obj, MY_CLASS, evas_obj_smart_move(x, y));
   evas_object_move(sd->combobox, x, y);
}

EOLIAN static void
_ewe_combobox_evas_object_smart_resize(Eo                      *obj,
                                       Ewe_Combobox_Smart_Data *sd,
                                       Evas_Coord              w,
                                       Evas_Coord              h EINA_UNUSED)
{
   int height = 0;
   edje_object_size_max_get(sd->combobox, NULL, &height);
   eo_do_super(obj, MY_CLASS, evas_obj_smart_resize(w, height));
   evas_object_resize(sd->combobox, w, height);
}

/* -----------------------------EO operations-------------------------------- */

EOLIAN static Eo*
_ewe_combobox_eo_base_constructor(Eo *obj, Ewe_Combobox_Smart_Data *sd)
{
   obj = eo_do_super_ret(obj, MY_CLASS, obj, eo_constructor());
   sd->obj = obj;
   eo_do(obj,
         evas_obj_type_set(MY_CLASS_NAME_LEGACY),
         evas_obj_smart_callbacks_descriptions_set(_smart_callbacks));
   return obj;
}

EOLIAN static void
_ewe_combobox_class_constructor(Eo_Class *klass)
{
   evas_smart_legacy_type_register(MY_CLASS_NAME_LEGACY, klass);
}

#include "ewe_combobox.eo.c"
#undef MIN_ITEMS
