#include "program_editor.h"

struct _Program_Editor
{
   Group *group;
   Evas_Object *mwin;
   Evas_Object *gl_progs;
};

typedef struct _Program_Editor Program_Editor;
static Program_Editor window;
static Elm_Genlist_Item_Class *_itc_prog;

static char *
_item_prog_label_get(void *data,
                     Evas_Object *obj __UNUSED__,
                     const char *part __UNUSED__)
{
   char *prog_label = (char *)data;
   /* char *formated_style_name; */
   if (!prog_label)
     {
        ERR("It impossible, but it is occurred, style's name is missing!");
        return NULL;
     }

   return strdup(prog_label);
}
static Evas_Object *
_gl_progs_add(Evas_Object *parent, Group *group)
{
   Evas_Object *gl_progs;
   Eina_List *progs_list, *l;
   const char *prog_name;

   if (!_itc_prog)
     {
        _itc_prog = elm_genlist_item_class_new();
        _itc_prog->item_style = "default";
        _itc_prog->func.text_get = _item_prog_label_get;
        _itc_prog->func.content_get = NULL;
        _itc_prog->func.state_get = NULL;
        _itc_prog->func.del = NULL;
     }

   gl_progs = elm_genlist_add(parent);
   evas_object_size_hint_align_set(gl_progs,
                                   EVAS_HINT_FILL,
                                   EVAS_HINT_FILL);
   evas_object_size_hint_weight_set(gl_progs,
                                    EVAS_HINT_EXPAND,
                                    EVAS_HINT_EXPAND);
   evas_object_show(gl_progs);
   progs_list = edje_edit_programs_list_get(group->obj);
   EINA_LIST_FOREACH(progs_list, l, prog_name)
     {
        elm_genlist_item_append(gl_progs, _itc_prog, prog_name,
                                NULL, ELM_GENLIST_ITEM_NONE,
                                NULL, NULL);
     }
   return gl_progs;
}

static void
__on_program_editor_close(void *data __UNUSED__,
                        Evas *e __UNUSED__,
                        Evas_Object *obj __UNUSED__,
                        void *event_info __UNUSED__)
{
   /* clear stringshare */
}

Evas_Object *
program_editor_window_add(Evas_Object *parent, Group *group)
{
   Evas_Object *mw_box, *pans;
   Evas_Object *glist;
   Evas_Object *bt_box, *bt;

   window.group = group;
   window.mwin = mw_add(parent);
   mw_title_set(window.mwin, "Program editor");
   evas_object_event_callback_add(window.mwin, EVAS_CALLBACK_FREE,
                                  __on_program_editor_close, NULL);

   mw_box = elm_box_add(window.mwin);
   evas_object_size_hint_weight_set(mw_box, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(mw_box, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(mw_box);

   pans = elm_panes_add(mw_box);
   evas_object_size_hint_weight_set(pans, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(pans, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_panes_content_left_size_set(pans, 0.2);
   evas_object_show(pans);

   glist = _gl_progs_add(window.mwin, group);
   elm_object_part_content_set(pans, "left", glist);

   bt_box = elm_box_add(mw_box);
   evas_object_size_hint_weight_set(bt_box, EVAS_HINT_EXPAND, 0.0);
   //evas_object_size_hint_align_set(bt_box, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_horizontal_set(bt_box, EINA_TRUE);
   elm_box_align_set(bt_box, 1.0, 1.0);
   evas_object_show(bt_box);

   bt = elm_button_add(bt_box);
   elm_object_text_set(bt, "Ok");
   evas_object_show(bt);
   elm_box_pack_end(bt_box, bt);

   bt = elm_button_add(bt_box);
   elm_object_text_set(bt, "Cansel");
   evas_object_show(bt);
   elm_box_pack_end(bt_box, bt);

   elm_box_pack_end(mw_box, pans);
   elm_box_pack_end(mw_box, bt_box);

   elm_win_inwin_content_set(window.mwin, mw_box);
   //
   evas_object_show(window.mwin);
   return window.mwin;
}
