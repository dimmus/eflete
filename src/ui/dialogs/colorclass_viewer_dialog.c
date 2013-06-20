#include "efl_tet.h"
#include "colorclass_viewer_dialog.h"


struct _Colorclass_Item
{
   const char *name;
   int r1, g1, b1, a1;
   int r2, g2, b2, a2;
   int r3, g3, b3, a3;
};
typedef struct _Colorclass_Item Colorclass_Item;

void
_ccl_set(Colorclass_Item *ccl_it, Evas_Object *ccl_view);

static Elm_Genlist_Item_Class *_itc_ccl = NULL;

static void
_on_button_ok_clicked_cb(void *data, Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   Evas_Object *inwin = (Evas_Object *)data;
   evas_object_del(inwin);
}

static void
_on_button_cancel_clicked_cb(void *data, Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   Evas_Object *inwin = (Evas_Object *)data;
   evas_object_del(inwin);
}

static char *
_item_ccl_label_get(void *data,
                      Evas_Object *obj __UNUSED__,
                      const char *part __UNUSED__)
{
   Colorclass_Item *ccl_it = (Colorclass_Item *)data;
   return strdup(ccl_it->name);
}

static void
_on_ccl_selected(void *data, Evas_Object *obj __UNUSED__, void *event_info)
{
   Elm_Object_Item *glit = (Elm_Object_Item *)event_info;
   Colorclass_Item *ccl_it = elm_object_item_data_get(glit);
   Evas_Object *ccl_view = (Evas_Object *) data;
   _ccl_set(ccl_it, ccl_view);
}

void
_ccl_set(Colorclass_Item *ccl_it, Evas_Object *ccl_view)
{
   Evas_Object *layout = elm_object_part_content_get(
      elm_object_part_content_get(elm_win_inwin_content_get(ccl_view),
         "swallow/panes"), "right");
   Evas_Object *label, *rect;
   char buf[4];

   label = elm_object_part_content_get(layout, "obj_r");
   snprintf(buf, 4, "%i", ccl_it->r1);
   elm_object_text_set(label, buf);

   label = elm_object_part_content_get(layout, "obj_g");
   snprintf(buf, 4, "%i", ccl_it->g1);
   elm_object_text_set(label, buf);

   label = elm_object_part_content_get(layout, "obj_b");
   snprintf(buf, 4, "%i", ccl_it->b1);
   elm_object_text_set(label, buf);

   label = elm_object_part_content_get(layout, "obj_a");
   snprintf(buf, 4, "%i", ccl_it->a1);
   elm_object_text_set(label, buf);

   rect = elm_object_part_content_get(layout, "obj_color");
   evas_object_color_set(rect, ccl_it->r1, ccl_it->g1, ccl_it->b1,
                              ccl_it->a1);

   label = elm_object_part_content_get(layout, "out_r");
   snprintf(buf, 4, "%i", ccl_it->r2);
   elm_object_text_set(label, buf);

   label = elm_object_part_content_get(layout, "out_g");
   snprintf(buf, 4, "%i", ccl_it->g2);
   elm_object_text_set(label, buf);

   label = elm_object_part_content_get(layout, "out_b");
   snprintf(buf, 4, "%i", ccl_it->b2);
   elm_object_text_set(label, buf);

   label = elm_object_part_content_get(layout, "out_a");
   snprintf(buf, 4, "%i", ccl_it->a2);
   elm_object_text_set(label, buf);

   rect = elm_object_part_content_get(layout, "out_color");
   evas_object_color_set(rect, ccl_it->r2, ccl_it->g2, ccl_it->b2,
                              ccl_it->a2);

   label = elm_object_part_content_get(layout, "shdw_r");
   snprintf(buf, 4, "%i", ccl_it->r3);
   elm_object_text_set(label, buf);

   label = elm_object_part_content_get(layout, "shdw_g");
   snprintf(buf, 4, "%i", ccl_it->g3);
   elm_object_text_set(label, buf);

   label = elm_object_part_content_get(layout, "shdw_b");
   snprintf(buf, 4, "%i", ccl_it->b3);
   elm_object_text_set(label, buf);

   label = elm_object_part_content_get(layout, "shdw_a");
   snprintf(buf, 4, "%i", ccl_it->a3);
   elm_object_text_set(label, buf);

   rect = elm_object_part_content_get(layout, "shdw_color");
   evas_object_color_set(rect, ccl_it->r3, ccl_it->g3, ccl_it->b3,
                              ccl_it->a3);

}

Colorclass_Item *
_ccl_it_new ()
{
   Colorclass_Item *ccl_it = (Colorclass_Item *)calloc(1,sizeof(Colorclass_Item));
   ccl_it->name = NULL;
   ccl_it->r1 = ccl_it->g1 = ccl_it->b1 = ccl_it->a1 = 0;
   ccl_it->r2 = ccl_it->g2 =  ccl_it->b2 =  ccl_it->a2 = 0;
   ccl_it->r3 = ccl_it->g3 =  ccl_it->b3 =  ccl_it->a3 = 0;
   return ccl_it;
}

Evas_Object *
colorclass_viewer_add(Evas_Object *parent)
{
   Evas_Object *inwin;
   Evas_Object *button, *genlist, *layout, *panes;
   Evas_Object *rect, *label;

   inwin = elm_win_inwin_add(parent);

   layout = elm_layout_add(inwin);
   evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_layout_file_set(layout, TET_EDJ, "base/colorclass_viewer/default");
   elm_win_inwin_content_set(inwin, layout);
   evas_object_show(layout);

   panes = elm_panes_add(inwin);
   evas_object_size_hint_weight_set(panes, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panes, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_panes_content_left_size_set(panes, 0.6);
   elm_object_part_content_set(layout, "swallow/panes", panes);
   evas_object_show(panes);

   genlist = elm_genlist_add(inwin);
   evas_object_size_hint_weight_set(genlist, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_object_part_content_set(panes, "left", genlist);
   evas_object_show(genlist);
   evas_object_smart_callback_add(genlist, "selected", _on_ccl_selected, inwin);

   if (!_itc_ccl)
     {
        _itc_ccl = elm_genlist_item_class_new();
        _itc_ccl->item_style = "default";
        _itc_ccl->func.text_get = _item_ccl_label_get;
        _itc_ccl->func.content_get = NULL;
        _itc_ccl->func.state_get = NULL;
        _itc_ccl->func.del = NULL;
     }

   button = elm_button_add(inwin);
   elm_object_text_set(button, "Add");
   elm_object_part_content_set(layout, "button_add", button);
   evas_object_resize(button, 40, 20);
   evas_object_show(button);

   button = elm_button_add(inwin);
   elm_object_text_set(button, "Delete");
   elm_object_part_content_set(layout, "button_del", button);
   evas_object_resize(button, 40, 20);
   evas_object_show(button);

   button = elm_button_add(inwin);
   elm_object_text_set(button, "Ok");
   elm_object_part_content_set(layout, "button_ok", button);
   evas_object_smart_callback_add(button, "clicked", _on_button_ok_clicked_cb,
                                   inwin);
   evas_object_resize(button, 40, 20);
   evas_object_show(button);

   button = elm_button_add(inwin);
   elm_object_text_set(button, "Cancel");
   elm_object_part_content_set(layout, "button_cancel", button);
   evas_object_smart_callback_add(button, "clicked", _on_button_cancel_clicked_cb,
                                   inwin);
   evas_object_resize(button, 40, 20);
   evas_object_show(button);

   layout = elm_layout_add(inwin);
   evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_layout_file_set(layout, TET_EDJ, "base/colorclass_viewer/property");
   elm_object_part_content_set(panes, "right", layout);
   evas_object_show(layout);

#define LABEL_ADD(part)\
   label = elm_entry_add(inwin);\
   elm_entry_single_line_set(label, EINA_TRUE);\
   elm_entry_single_line_set(label, EINA_TRUE);\
   elm_entry_scrollable_set(label, EINA_TRUE);\
   elm_entry_scrollbar_policy_set(label, ELM_SCROLLER_POLICY_OFF, ELM_SCROLLER_POLICY_OFF);\
   elm_object_text_set(label, "---");\
   elm_object_part_content_set(layout, part, label);\
   evas_object_show(label);

#define COLOR_RECT_ADD(part)\
   rect = elm_bg_add (inwin);\
   evas_object_size_hint_weight_set(rect, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);\
   elm_bg_color_set(rect, 255, 255, 255);\
   elm_object_part_content_set(layout, part, rect);\
   evas_object_show(rect);

   COLOR_RECT_ADD("obj_color")
   LABEL_ADD("obj_r")
   LABEL_ADD("obj_g")
   LABEL_ADD("obj_b")
   LABEL_ADD("obj_a")
   COLOR_RECT_ADD("out_color")
   LABEL_ADD("out_r")
   LABEL_ADD("out_g")
   LABEL_ADD("out_b")
   LABEL_ADD("out_a")
   COLOR_RECT_ADD("shdw_color")
   LABEL_ADD("shdw_r")
   LABEL_ADD("shdw_g")
   LABEL_ADD("shdw_b")
   LABEL_ADD("shdw_a")
#undef LABEL_ADD
#undef COLOR_RECT_ADD

   evas_object_show(layout);
   return inwin;
}

void
colorclass_viewer_init(Evas_Object *ccl_view, Project *project)
{
   int r1, r2, r3, g1, g2, g3, b1, b2, b3, a1, a2, a3;

   Evas_Object *genlist = elm_object_part_content_get(
      elm_object_part_content_get(elm_win_inwin_content_get(ccl_view),
         "swallow/panes"), "left");
   Widget *_widget = NULL;
   Style *_style = NULL;
   Group *_group = NULL;
   Part *_part = NULL;
   Part_State *_state = NULL;

   char buf[4096];
   Eina_Bool ccl_pr = EINA_FALSE;
   Evas_Object *edje;
   Evas *e = evas_object_evas_get(elm_object_parent_widget_get(ccl_view));

   if (!project)
     {
        NOTIFY_ERROR (elm_object_parent_widget_get(ccl_view),
                      "EDJ/EDC file is not loaded");
        evas_object_del(ccl_view);
        return;
     }
   evas_object_show(ccl_view);

   edje = edje_object_add(e);

   EINA_INLIST_FOREACH(project->widgets, _widget)
     {
        EINA_INLIST_FOREACH(_widget->styles, _style)
          {
             EINA_INLIST_FOREACH(_style->groups, _group)
               {
                  EINA_INLIST_FOREACH(_group->parts, _part)
                    {
                       EINA_INLIST_FOREACH(_part->states, _state)
                         {
                            if (_state->color_class)
                              {
                                 edje_object_file_set(edje, project->swapfile,
                                    _group->full_group_name);
                                  ccl_pr = edje_object_color_class_get(
                                    edje, _state->color_class, &r1, &g1, &b1,
                                    &a1, &r2, &g2, &b2, &a2, &r3, &g3, &b3, &a3);
                                 if (ccl_pr && (strcmp(buf, _state->color_class)))
                                   {
                                      Colorclass_Item *ccl_it = _ccl_it_new();
                                      ccl_it->r1 = r1; ccl_it->g1 = g1;
                                      ccl_it->b1 = b1; ccl_it->a1 = a1;
                                      ccl_it->r2 = r2; ccl_it->g2 = g2;
                                      ccl_it->b2 = b2; ccl_it->a2 = a2;
                                      ccl_it->r3 = r3; ccl_it->g3 = g3;
                                      ccl_it->b3 = b3; ccl_it->a3 = a3;

                                      ccl_it->name = _state->color_class;
                                      strcpy(buf, _state->color_class);
                                      elm_genlist_item_append(genlist,
                                         _itc_ccl, ccl_it, NULL,
                                         ELM_GENLIST_ITEM_NONE, NULL, NULL);
                                   }
                              }
                         }
                    }
               }
          }
     }
   evas_object_del(edje);
}


