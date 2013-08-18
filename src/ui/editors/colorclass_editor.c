#include "colorclass_editor.h"
struct _Colorclass_Item
{
   const char *name;
   int r1, g1, b1, a1;
   int r2, g2, b2, a2;
   int r3, g3, b3, a3;
};
typedef struct _Colorclass_Item Item;
typedef struct _Colorclass_Item Colorclass_Item;

struct _Colorclasses_Editor
{
   Project *pr;
   Evas_Object *mwin;
   Evas_Object *genlist;
   Colorclass_Item old_ccl,*current_ccl, *next_ccl;
   Evas_Object *rect_color1, *obj_r, *obj_g, *obj_b, *obj_a;
   Evas_Object *rect_color2, *out_r, *out_g, *out_b, *out_a;
   Evas_Object *rect_color3, *sdw_r, *sdw_g, *sdw_b, *sdw_a;
   Evas_Object *label;
};

typedef struct _Colorclasses_Editor Colorclasses_Editor;

static Colorclasses_Editor window;
static Evas_Object *ccl_entry;

void
_ccl_set(Colorclass_Item *ccl_it);

static Elm_Genlist_Item_Class *_itc_ccl = NULL;

static void
__on_ccl_editor_close(void *data __UNUSED__,
                        Evas *e __UNUSED__,
                        Evas_Object *obj __UNUSED__,
                        void *event_info __UNUSED__)
{
   evas_object_del(window.rect_color1);
   evas_object_del(window.rect_color2);
   evas_object_del(window.rect_color3);
   window.current_ccl=NULL;
}

/*colorclass popup buttons callbacks*/
static void
_on_apply_popup_btn_yes(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *ei __UNUSED__)
{
   Evas_Object *popup = (Evas_Object *)data;

   colorclasses_edit_colors_set(window.pr,window.current_ccl->name,
                    window.current_ccl->r1,window.current_ccl->b1,
                    window.current_ccl->b1,window.current_ccl->a1,
                    window.current_ccl->r2,window.current_ccl->g2,
                    window.current_ccl->b2,window.current_ccl->a2,
                    window.current_ccl->r3,window.current_ccl->g3,
                    window.current_ccl->b3,window.current_ccl->a3);

   _ccl_set(window.next_ccl);

   evas_object_del(popup);
}


static void
_on_apply_popup_btn_no(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *ei __UNUSED__)
{
   Evas_Object *popup = (Evas_Object *)data;
   (*window.current_ccl)=window.old_ccl;
   _ccl_set(window.next_ccl);

   evas_object_del(popup);
}

static void
_on_add_popup_btn_add(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *ei __UNUSED__)
{
   Evas_Object *popup = (Evas_Object *)data;
   Item *it=(Item *) calloc(1,sizeof(Item));
   it->name = elm_entry_entry_get(ccl_entry);
   Elm_Object_Item *glit_ccl;

   if (!it->name)
     {
        NOTIFY_WARNING("Color class name can not be empty!");
        return;
     }
   if(!colorclass_edit_add(window.pr, it->name))
     {
        NOTIFY_WARNING("Color class name must be unique!");
        return;
     }

   glit_ccl=elm_genlist_item_append(window.genlist, _itc_ccl, it, NULL,
                           ELM_GENLIST_ITEM_NONE, NULL, NULL);
   elm_genlist_item_selected_set(glit_ccl, EINA_TRUE);
   evas_object_del(popup);
}

static void
_on_add_popup_btn_cancel(void *data,
                 Evas_Object *obj __UNUSED__,
                 void *ei __UNUSED__)
{
   Evas_Object *popup = (Evas_Object *)data;
   evas_object_del(popup);
}


/*Colorclass editor buttons callbacks*/
static void
_on_btn_cancel(void *data, Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   Evas_Object *mwin = (Evas_Object *)data;
   evas_object_del(mwin);
}


static void
_on_btn_apply(void *data __UNUSED__,
                 Evas_Object *obj __UNUSED__,
                 void *ei __UNUSED__)
{
   colorclasses_edit_colors_set(window.pr,window.current_ccl->name,
                    window.current_ccl->r1,window.current_ccl->b1,
                    window.current_ccl->b1,window.current_ccl->a1,
                    window.current_ccl->r2,window.current_ccl->g2,
                    window.current_ccl->b2,window.current_ccl->a2,
                    window.current_ccl->r3,window.current_ccl->g3,
                    window.current_ccl->b3,window.current_ccl->a3);
   window.old_ccl=*window.current_ccl;
}

static void
_on_btn_add(void *data __UNUSED__, Evas_Object *obj __UNUSED__,
                       void *event_info __UNUSED__)
{
   Evas_Object *popup, *box, *bt_yes, *bt_no;
   Evas_Object *ccl_box, *ccl_label;
   popup = elm_popup_add(window.mwin);
   elm_object_part_text_set(popup, "title,text", "Add color class:");

   box = elm_box_add(popup);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(box, EVAS_HINT_FILL, EVAS_HINT_FILL);

   ccl_box = elm_box_add(box);
   elm_box_horizontal_set(ccl_box, EINA_TRUE);
   evas_object_size_hint_weight_set(ccl_box, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(ccl_box, EVAS_HINT_FILL, 0.0);
   evas_object_show(ccl_box);

   ccl_label = elm_label_add(ccl_box);
   elm_object_text_set(ccl_label, "Color class name: ");
   evas_object_show(ccl_label);
   elm_box_pack_end(ccl_box, ccl_label);

   ccl_entry = elm_entry_add(ccl_box);
   elm_entry_single_line_set(ccl_entry, EINA_TRUE);
   elm_entry_scrollable_set(ccl_entry, EINA_TRUE);
   elm_entry_scrollbar_policy_set(ccl_entry, ELM_SCROLLER_POLICY_OFF,
                                                    ELM_SCROLLER_POLICY_OFF);
   evas_object_size_hint_weight_set(ccl_entry, EVAS_HINT_EXPAND,
                                                            EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(ccl_entry, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(ccl_entry);
   elm_box_pack_end(ccl_box, ccl_entry);

   elm_box_pack_end(box, ccl_box);
   elm_object_content_set(popup, box);
   evas_object_show(box);

   bt_yes = elm_button_add(popup);
   elm_object_text_set(bt_yes, "Add");
   evas_object_smart_callback_add(bt_yes, "clicked", _on_add_popup_btn_add,
                                                                         popup);
   elm_object_part_content_set(popup, "button1", bt_yes);
   evas_object_show(bt_yes);

   bt_no = elm_button_add(popup);
   elm_object_text_set(bt_no, "Cancel");
   evas_object_smart_callback_add(bt_no, "clicked", _on_add_popup_btn_cancel,
                                                                        popup);
   elm_object_part_content_set(popup, "button2", bt_no);
   evas_object_show(bt_no);

   evas_object_show(popup);
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
_on_ccl_selected(void *data __UNUSED__, Evas_Object *obj __UNUSED__,
                       void *event_info)
{
   Elm_Object_Item *glit = (Elm_Object_Item *)event_info;
   Colorclass_Item *ccl_it = elm_object_item_data_get(glit);
   if(!window.current_ccl||(window.old_ccl.r1 == window.current_ccl->r1
        && window.old_ccl.r2 == window.current_ccl->r2
        && window.old_ccl.r3 == window.current_ccl->r3
        && window.old_ccl.g1 == window.current_ccl->g1
        && window.old_ccl.g2 == window.current_ccl->g2
        && window.old_ccl.g3 == window.current_ccl->g3
        && window.old_ccl.b1 == window.current_ccl->b1
        && window.old_ccl.b2 == window.current_ccl->b2
        && window.old_ccl.b3 == window.current_ccl->b3
        && window.old_ccl.a1 == window.current_ccl->a1
        && window.old_ccl.a2 == window.current_ccl->a2
        && window.old_ccl.a3 == window.current_ccl->a3))
   {
      _ccl_set(ccl_it);
      return;
   }
   Evas_Object *popup, *bt_yes, *bt_no;
   popup = elm_popup_add(window.mwin);
   elm_object_part_text_set(popup, "title,text", "Apply changes?");

   bt_yes = elm_button_add(popup);
   elm_object_text_set(bt_yes, "Yes");
   evas_object_smart_callback_add(bt_yes, "clicked", _on_apply_popup_btn_yes,
                                                                         popup);
   elm_object_part_content_set(popup, "button1", bt_yes);
   evas_object_show(bt_yes);

   bt_no = elm_button_add(popup);
   elm_object_text_set(bt_no, "No");
   evas_object_smart_callback_add(bt_no, "clicked", _on_apply_popup_btn_no,
                                                                        popup);
   elm_object_part_content_set(popup, "button2", bt_no);
   evas_object_show(bt_no);

   window.next_ccl=ccl_it;
   evas_object_show(popup);
}


static void
_on_btn_del(void *data __UNUSED__, Evas_Object *obj __UNUSED__,
                       void *event_info __UNUSED__)
{
   if(!window.current_ccl)
      return;
   colorclass_edit_del(window.pr,window.current_ccl->name);
   Elm_Object_Item *it=elm_genlist_selected_item_get(window.genlist);
   Elm_Object_Item *next=elm_genlist_item_next_get(it);
   if(!next) next = elm_genlist_item_prev_get(it);
   if(next)
      elm_genlist_item_selected_set(next,EINA_TRUE);
   else
      window.current_ccl=NULL;
   elm_object_item_del(it);
}
/*button callbacks end*/

static void
_on_spinner_value_changed(void *data __UNUSED__, Evas_Object *obj __UNUSED__,
                       void *event_info __UNUSED__)
{
   if(!window.current_ccl)
      return;
   window.current_ccl->r1 = elm_spinner_value_get(window.obj_r);
   window.current_ccl->g1 = elm_spinner_value_get(window.obj_g);
   window.current_ccl->b1 = elm_spinner_value_get(window.obj_b);
   window.current_ccl->a1 = elm_spinner_value_get(window.obj_a);

   window.current_ccl->r2 = elm_spinner_value_get(window.out_r);
   window.current_ccl->g2 = elm_spinner_value_get(window.out_g);
   window.current_ccl->b2 = elm_spinner_value_get(window.out_b);
   window.current_ccl->a2 = elm_spinner_value_get(window.out_a);

   window.current_ccl->r3 = elm_spinner_value_get(window.sdw_r);
   window.current_ccl->g3 = elm_spinner_value_get(window.sdw_g);
   window.current_ccl->b3 = elm_spinner_value_get(window.sdw_b);
   window.current_ccl->a3 = elm_spinner_value_get(window.sdw_a);

   evas_object_color_set(window.rect_color1,
           window.current_ccl->r1*window.current_ccl->a1/255,
           window.current_ccl->g1*window.current_ccl->a1/255,
           window.current_ccl->b1*window.current_ccl->a1/255,
           window.current_ccl->a1);
   evas_object_color_set(window.rect_color2,
           window.current_ccl->r2*window.current_ccl->a2/255,
           window.current_ccl->g2*window.current_ccl->a2/255,
           window.current_ccl->b2*window.current_ccl->a2/255,
           window.current_ccl->a2);
   evas_object_color_set(window.rect_color3,
           window.current_ccl->r3*window.current_ccl->a3/255,
           window.current_ccl->g3*window.current_ccl->a3/255,
           window.current_ccl->b3*window.current_ccl->a3/255,
           window.current_ccl->a3);
   edje_object_color_class_set(window.label,
                              "colorclass_editor/text_example_colorclass",
          window.current_ccl->r1,window.current_ccl->g1,window.current_ccl->b1,
          window.current_ccl->a1,window.current_ccl->r2,window.current_ccl->g2,
          window.current_ccl->b2,window.current_ccl->a2,window.current_ccl->r3,
          window.current_ccl->g3,window.current_ccl->b3,window.current_ccl->a3);
}

void
_ccl_set(Colorclass_Item *ccl_it)
{
   window.old_ccl=*ccl_it;
   window.current_ccl=ccl_it;

   elm_spinner_value_set(window.obj_r,ccl_it->r1);
   elm_spinner_value_set(window.obj_g,ccl_it->g1);
   elm_spinner_value_set(window.obj_b,ccl_it->b1);
   elm_spinner_value_set(window.obj_a,ccl_it->a1);

   elm_spinner_value_set(window.out_r,ccl_it->r2);
   elm_spinner_value_set(window.out_g,ccl_it->g2);
   elm_spinner_value_set(window.out_b,ccl_it->b2);
   elm_spinner_value_set(window.out_a,ccl_it->a2);

   elm_spinner_value_set(window.sdw_r,ccl_it->r3);
   elm_spinner_value_set(window.sdw_g,ccl_it->g3);
   elm_spinner_value_set(window.sdw_b,ccl_it->b3);
   elm_spinner_value_set(window.sdw_a,ccl_it->a3);

   _on_spinner_value_changed(NULL,NULL,NULL);
}

Evas_Object *
colorclass_viewer_add(Evas_Object *parent)
{
   Evas_Object *mwin;
   Evas_Object *button,  *panes;
   Evas_Object *label, *color;
   Evas_Object *box, *bottom_box, *right_box;

   mwin = mw_add(parent);
   mw_title_set(mwin, "Color class editor");
   evas_object_event_callback_add(mwin, EVAS_CALLBACK_FREE,
                                  __on_ccl_editor_close, NULL);

   box = elm_box_add(mwin);
   evas_object_size_hint_weight_set(box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_inwin_content_set(mwin, box);
   evas_object_show(box);

   panes = elm_panes_add(box);
   elm_box_pack_end(box,panes);
   evas_object_size_hint_weight_set(panes, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panes, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_panes_content_left_size_set(panes, 0.6);
   evas_object_show(panes);

   window.genlist = elm_genlist_add(mwin);
   evas_object_size_hint_weight_set(window.genlist, EVAS_HINT_EXPAND,
                                                            EVAS_HINT_EXPAND);
   elm_object_part_content_set(panes, "left", window.genlist);
   evas_object_show(window.genlist);
   evas_object_smart_callback_add(window.genlist, "selected", _on_ccl_selected,
                                                                        mwin);

   if (!_itc_ccl)
     {
        _itc_ccl = elm_genlist_item_class_new();
        _itc_ccl->item_style = "default";
        _itc_ccl->func.text_get = _item_ccl_label_get;
        _itc_ccl->func.content_get = NULL;
        _itc_ccl->func.state_get = NULL;
        _itc_ccl->func.del = NULL;
     }

   bottom_box = elm_box_add(box);
   elm_box_pack_end(box,bottom_box);
   elm_box_horizontal_set(bottom_box,EINA_TRUE);
   evas_object_size_hint_weight_set(bottom_box, EVAS_HINT_EXPAND, 0.0);
   evas_object_size_hint_align_set(bottom_box, 0,0);
   evas_object_show(bottom_box);

   button = elm_button_add(mwin);
   elm_object_text_set(button, "Add");
   evas_object_smart_callback_add(button, "clicked", _on_btn_add,
                                   mwin);
   elm_box_pack_end(bottom_box,button);
   evas_object_show(button);

   button = elm_button_add(mwin);
   elm_object_text_set(button, "Delete");
   evas_object_smart_callback_add(button, "clicked", _on_btn_del,
                                   mwin);
   elm_box_pack_end(bottom_box,button);
   evas_object_show(button);

   button = elm_button_add(mwin);
   elm_object_text_set(button, "Apply");
   evas_object_smart_callback_add(button, "clicked", _on_btn_apply,
                                   mwin);
   elm_box_pack_end(bottom_box,button);
   evas_object_show(button);

   button = elm_button_add(mwin);
   elm_object_text_set(button, "Close");
   evas_object_smart_callback_add(button, "clicked",
                     _on_btn_cancel, mwin);
   elm_box_pack_end(bottom_box,button);
   evas_object_show(button);

   right_box = elm_box_add(box);
   elm_object_part_content_set(panes, "right", right_box);
   evas_object_size_hint_weight_set(right_box, EVAS_HINT_EXPAND,
                                                            EVAS_HINT_EXPAND);
   evas_object_size_hint_min_set(right_box, 200,100);
   evas_object_size_hint_align_set(right_box, -1.0,0);
   elm_box_homogeneous_set(right_box,EINA_TRUE);
   evas_object_show(right_box);

#define SPINNER_ADD(spinner,format)\
   spinner = elm_spinner_add(mwin);\
   elm_spinner_min_max_set(spinner, 0,255);\
   elm_spinner_interval_set(spinner,0.4);\
   elm_spinner_label_format_set(spinner,format);\
   elm_spinner_editable_set(spinner, EINA_FALSE);\
   evas_object_size_hint_min_set(spinner, 150, 35);\
   evas_object_size_hint_max_set(spinner, 150, 35);\
   evas_object_size_hint_align_set(spinner, -1.0, -1.0);\
   evas_object_smart_callback_add(spinner, "changed",\
                           _on_spinner_value_changed, spinner);\
   elm_box_pack_end(right_box, spinner);\
   evas_object_show(spinner);

#define COLOR_ADD(rect,title)\
   label = elm_label_add(mwin);\
   elm_object_text_set(label,title);\
   elm_box_pack_end(right_box, label);\
   evas_object_show(label);\
   color = edje_object_add (evas_object_evas_get(mwin));\
   rect = evas_object_rectangle_add(evas_object_evas_get(mwin));\
   edje_object_file_set(color,TET_EDJ,"base/colorclass_editor/color_example");\
   edje_object_part_swallow(color,"color_example",rect);\
   evas_object_size_hint_weight_set(color, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);\
   evas_object_size_hint_min_set(color, 150, 35);\
   evas_object_size_hint_max_set(color, 150, 35);\
   evas_object_size_hint_align_set(color, -1.0, -1.0);\
   elm_box_pack_end(right_box, color);\
   evas_object_show(rect);\
   evas_object_show(color);

   COLOR_ADD(window.rect_color1,"Object color")
   SPINNER_ADD(window.obj_r,"R: %4.0f")
   SPINNER_ADD(window.obj_g,"G: %4.0f")
   SPINNER_ADD(window.obj_b,"B: %4.0f")
   SPINNER_ADD(window.obj_a,"A: %4.0f")
   COLOR_ADD(window.rect_color2,"Outline color")
   SPINNER_ADD(window.out_r,"R: %4.0f")
   SPINNER_ADD(window.out_g,"G: %4.0f")
   SPINNER_ADD(window.out_b,"B: %4.0f")
   SPINNER_ADD(window.out_a,"A: %4.0f")
   COLOR_ADD(window.rect_color3,"Shadow color")
   SPINNER_ADD(window.sdw_r,"R: %4.0f")
   SPINNER_ADD(window.sdw_g,"G: %4.0f")
   SPINNER_ADD(window.sdw_b,"B: %4.0f")
   SPINNER_ADD(window.sdw_a,"A: %4.0f")

#undef SPINNER_ADD
#undef COLOR_ADD
   window.label = edje_object_add(evas_object_evas_get(mwin));
   edje_object_file_set(window.label,TET_EDJ,
                                        "base/colorclass_editor/text_example");
   edje_object_part_text_set(window.label, "text_example", "EXAMPLE");
   elm_box_pack_start(right_box,window.label);
   evas_object_size_hint_weight_set(window.label, EVAS_HINT_EXPAND,
                                                            EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(window.label, -1.0, -1.0);
   evas_object_show(window.label);
   window.mwin=mwin;
   return mwin;
}

void
colorclass_viewer_init(Evas_Object *ccl_view __UNUSED__,
                       Project *project __UNUSED__)
{
   int r1, r2, r3, g1, g2, g3, b1, b2, b3, a1, a2, a3;
   if (!project)
     {
        NOTIFY_ERROR ("EDJ/EDC file is not loaded");
        evas_object_del(ccl_view);
        return;
     }
   window.pr=project;
   evas_object_show(ccl_view);
   const char* ccname;
   Eina_List *cclist, *l;
   cclist = colorclasses_edit_list_get(window.pr);

   EINA_LIST_FOREACH(cclist,l,ccname)
   {
      Item *it=(Item *) calloc(1,sizeof(Item));
      colorclasses_edit_colors_get(window.pr, ccname,
         &r1, &g1, &b1, &a1, &r2, &g2, &b2, &a2, &r3, &g3, &b3, &a3);
     it->r1 = r1; it->g1 = g1;
     it->b1 = b1; it->a1 = a1;
     it->r2 = r2; it->g2 = g2;
     it->b2 = b2; it->a2 = a2;
     it->r3 = r3; it->g3 = g3;
     it->b3 = b3; it->a3 = a3;

     it->name = strdup(ccname);
     elm_genlist_item_append(window.genlist, _itc_ccl, it, NULL,
       ELM_GENLIST_ITEM_NONE, NULL, NULL);
     }
}
