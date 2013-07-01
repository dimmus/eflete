#include <ui_main_window.h>
#include "style_viewer_dialog.h"

static Elm_Genlist_Item_Class *_itc_style = NULL;
static Elm_Genlist_Item_Class *_itc_tags = NULL;

static Style_Window *window;

Tag*
_edj_tag_get(Evas_Object *obj, const char *style_name,
            const char *tag_name)
{
   Tag *tag;
   const char* value;

   tag = calloc(1, sizeof(Tag));
   tag->tag_name = eina_stringshare_add(tag_name);
   value = edje_edit_style_tag_value_get(obj, style_name,
                                         tag_name);
   tag->tag_value = eina_stringshare_add(value);
   tag->__type = TAG;

   return tag;
}

Eina_Inlist*
_edj_tags_list_get(Evas_Object *obj, const char *style_name)
{
   Eina_Inlist *tags = NULL;
   Eina_List *tag_names, *l;
   Tag *tag;
   char *tag_name_text;

   tag_names = edje_edit_style_tags_list_get(obj, style_name);

   EINA_LIST_FOREACH (tag_names, l, tag_name_text)
     {
        tag = _edj_tag_get(obj, style_name, tag_name_text);
        tags = eina_inlist_append(tags, EINA_INLIST_GET(tag));
     }

   return tags;
}

Text_Styles*
_edj_text_style_get(Evas_Object *obj, const char *style_name)
{
   Eina_Inlist *tags;
   Text_Styles *style;

   tags = _edj_tags_list_get(obj, style_name);
   style = mem_malloc(sizeof(Text_Styles));
   style->style_name = eina_stringshare_add(style_name);
   style->tags = tags;
   style->__type = TEXTSTYLE;

   return style;
}

Eina_Inlist*
_style_list_get(Evas_Object* obj, Project *project)
{
   Eina_Inlist *styles, *groups, *widgets, *text_styles = NULL;
   Eina_List *f, *text_styles_name;
   char *text_style;
   Evas_Object *edje_object;
   Widget *_widget;
   Style *_style;
   Group *_group;
   Text_Styles *tstyles;

   widgets = project->widgets;
   if (!widgets)
     {
        NOTIFY_ERROR (obj,
                         "It seems that there is no widgets in project. \n");
        return NULL;
     }
   _widget = EINA_INLIST_CONTAINER_GET(widgets, Widget);
   styles = _widget->styles;
   if (!styles)
     {
        NOTIFY_ERROR (obj,
                         "It seems that there is no styles in project. \n");
        return NULL;
     }
   _style = EINA_INLIST_CONTAINER_GET(styles, Style);
   groups = _style->groups;
   if (!groups)
     {
        NOTIFY_ERROR (obj,
                         "It seems that there is no groups in project. \n");
        return NULL;
     }

   _group = EINA_INLIST_CONTAINER_GET(groups, Group);
   edje_object = _group->obj;
   text_styles_name = edje_edit_styles_list_get(edje_object);
   EINA_LIST_FOREACH (text_styles_name, f, text_style)
     {
        tstyles = _edj_text_style_get(edje_object, text_style);
        text_styles = eina_inlist_append(text_styles,
                                         EINA_INLIST_GET(tstyles));
     }
   return text_styles;
}

/**
 * For GenList, getting the content for showing. Tag Names.
 */
static char *
_item_tags_label_get(void *data,
                     Evas_Object *obj __UNUSED__,
                     const char *part __UNUSED__)
{
   Tag *t = (Tag *)data;
   if (!t->tag_name)
     {
        ERR("It impossible, but it is occurred, tag's name is missing!");
        return NULL;
     }
   return strdup(t->tag_name);
}

/**
 * For Style names.
 */
static char *
_item_style_label_get(void *data,
                      Evas_Object *obj __UNUSED__,
                      const char *part __UNUSED__)
{
   Text_Styles *s = (Text_Styles *)data;
   if (!s->style_name)
     {
        ERR("It impossible, but it is occurred, style's name is missing!");
        return NULL;
     }
   return strdup(s->style_name);
}

/**
 * For showing tags value and name in appropriate entry.
 */
static void
_on_tag_clicked_double(void *data __UNUSED__,
                       Evas_Object *obj __UNUSED__,
                       void *event_info)
{
   Tag *_tag;
   Text_Styles *_style;
   void *event_obj;
   Eina_Inlist *tags_list;

   char style[BUFF_MAX] = "DEFAULT='";

   Elm_Object_Item *glit = (Elm_Object_Item *)event_info;

   event_obj = elm_object_item_data_get(glit);
   _tag = (Tag*)event_obj;
   _style = (Text_Styles*)event_obj;

   if (_style->__type == TEXTSTYLE)
     {
        char style_tags[BUFF_MAX] = "";
        tags_list = _style->tags;

        EINA_INLIST_FOREACH (tags_list, _tag)
          {
             strcat(style, _tag->tag_value);
             strcat(style_tags, _tag->tag_value);
             strcat(style, " ");
             strcat(style_tags, " ");
          }
        strcat(style, "'");
        elm_entry_text_style_user_push(window->entry_style, style);
        elm_object_text_set(window->entry_prop, style_tags);
        elm_object_text_set(window->entry_tag, "");
     }
   else if (_tag->__type == TAG)
     {
        elm_object_text_set(window->entry_tag, _tag->tag_name);
        elm_object_text_set(window->entry_prop, _tag->tag_value);
        strcat(style, _tag->tag_value);
        strcat(style, "'");
        elm_entry_text_style_user_push(window->entry_style, style);
     }
   evas_object_size_hint_max_set(window->entry_style, EVAS_HINT_FILL, EVAS_HINT_FILL);
}

static void
_on_viewer_exit(void *data, Evas_Object *obj __UNUSED__,
                void *event_info __UNUSED__)
{
   Evas_Object *inwin = (Evas_Object *) data;

   evas_object_del(inwin);
}

/**
 * On hide we need to free everything!!!
 */
static void
_item_style_del(void *data, Evas_Object *obj __UNUSED__)
{
   Text_Styles *_style = (Text_Styles *) data;
   eina_stringshare_del(_style->style_name);
}

/**
 * On hide we need to free everything!!!
 */
static void
_item_tags_del(void *data, Evas_Object *obj __UNUSED__)
{
   Tag *_tag = (Tag *) data;
   eina_stringshare_del(_tag->tag_name);
   eina_stringshare_del(_tag->tag_value);
}

/**
 * Creating the view of the inwin!!!
 */
Evas_Object*
_form_left_side(Evas_Object *obj, Project *project) {
     Elm_Object_Item *glit_style, *glit_tag;
     Evas_Object *layout, *list, *btn;
     Eina_Inlist *style_list, *tags_list;
     Text_Styles *_style;
     Tag *_tag;

     layout = elm_layout_add(obj);
     evas_object_size_hint_weight_set(layout,
                                      EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
     elm_layout_file_set(layout, TET_EDJ, "ui/style_viewer_window/list");
     evas_object_show(layout);

     style_list = _style_list_get(obj, project);

     if (!_itc_style)
       {
          _itc_style = elm_genlist_item_class_new();
          _itc_style->item_style = "tree_effect";
          _itc_style->func.text_get = _item_style_label_get;
          _itc_style->func.content_get = NULL;
          _itc_style->func.state_get = NULL;
          _itc_style->func.del = _item_style_del;
       }
     if (!_itc_tags)
       {
          _itc_tags= elm_genlist_item_class_new();
          _itc_tags->item_style = "tree_effect";
          _itc_tags->func.text_get = _item_tags_label_get;
          _itc_tags->func.content_get = NULL;
          _itc_tags->func.state_get = NULL;
          _itc_tags->func.del = _item_tags_del;
       }

     list = elm_genlist_add(obj);
     elm_object_part_content_set (layout, "swallow/genlist", list);
     evas_object_show(list);

     evas_object_size_hint_align_set(list,
                                     EVAS_HINT_FILL,
                                     EVAS_HINT_FILL);
     evas_object_size_hint_weight_set(list,
                                      EVAS_HINT_EXPAND,
                                      EVAS_HINT_EXPAND);

     EINA_INLIST_FOREACH (style_list, _style)
       {
          glit_style = elm_genlist_item_append(list, _itc_style,
                                               _style,
                                               NULL, ELM_GENLIST_ITEM_NONE,
                                               NULL, NULL);
          tags_list = _style->tags;

          EINA_INLIST_FOREACH (tags_list, _tag)
            {
               glit_tag = elm_genlist_item_append(list, _itc_tags,
                                                  _tag,
                                                  glit_style, ELM_GENLIST_ITEM_NONE,
                                                  NULL, NULL);
               elm_object_item_data_set(glit_tag, _tag);
            }
       }

     evas_object_smart_callback_add(list, "selected",
                                    _on_tag_clicked_double, NULL);

     btn = elm_button_add(obj);
     elm_object_text_set(btn, "New Style");
     elm_object_part_content_set (layout, "swallow/button_add_style", btn);
     evas_object_show(btn);

     btn = elm_button_add(obj);
     elm_object_text_set(btn, "Delete Style");
     elm_object_part_content_set (layout, "swallow/button_rm_style", btn);
     evas_object_show(btn);

     btn = elm_button_add(obj);
     elm_object_text_set(btn, "New Tag");
     elm_object_part_content_set (layout, "swallow/button_add_tag", btn);
     evas_object_show(btn);

     btn = elm_button_add(obj);
     elm_object_text_set(btn, "Delete Tag");
     elm_object_part_content_set (layout, "swallow/button_rm_tag", btn);
     evas_object_show(btn);

     return layout;
}

Evas_Object*
_form_right_side(Evas_Object *obj)
{
   Evas_Object *layout, *btn;

   layout = elm_layout_add(obj);
   evas_object_size_hint_weight_set(layout,
                                    EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_layout_file_set(layout, TET_EDJ, "ui/style_viewer_window/property");
   evas_object_show(layout);

   window->entry_tag = elm_entry_add(obj);
   elm_entry_single_line_set(window->entry_tag, EINA_TRUE);
   elm_object_part_content_set (layout, "swallow/tag_entry", window->entry_tag);
   elm_entry_scrollable_set(window->entry_tag, EINA_TRUE);
   evas_object_show(window->entry_tag);

   window->entry_prop = elm_entry_add(obj);
   elm_entry_single_line_set(window->entry_prop, EINA_TRUE);
   elm_object_part_content_set (layout, "swallow/prop_entry", window->entry_prop);
   elm_entry_scrollable_set(window->entry_prop, EINA_TRUE);
   evas_object_show(window->entry_prop);

   btn = elm_button_add(obj);
   elm_object_text_set(btn, "Close Viewer");
   elm_object_part_content_set (layout, "swallow/button_close", btn);
   evas_object_smart_callback_add(btn, "clicked", _on_viewer_exit, obj);

   evas_object_show(btn);

   return layout;
}

Evas_Object *
style_viewer_add (Evas_Object *parent, Project *project)
{
   Evas_Object *inwin = NULL;
   Evas_Object *panes, *panes_h;
   Evas_Object *layout_left, *layout_right;

   inwin = elm_win_inwin_add(parent);

   window = mem_malloc(sizeof(Style_Window));

   panes = elm_panes_add(inwin);
   evas_object_size_hint_weight_set(panes, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panes, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_panes_content_left_size_set(panes, 0.2);
   elm_win_inwin_content_set(inwin, panes);
   evas_object_show(panes);

   layout_left = _form_left_side(inwin, project);
   elm_object_part_content_set(panes, "left", layout_left);
   evas_object_show(layout_left);

   panes_h = elm_panes_add(inwin);
   evas_object_size_hint_weight_set(panes_h, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panes_h, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_panes_horizontal_set(panes_h, EINA_TRUE);
   elm_object_part_content_set(panes, "right", panes_h);
   evas_object_show(panes_h);

   elm_theme_extension_add(NULL, TET_EDJ);

   window->entry_style = elm_entry_add(inwin);
   evas_object_size_hint_weight_set(window->entry_style, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(window->entry_style, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_entry_scrollable_set(window->entry_style, EINA_TRUE);
   elm_entry_single_line_set(window->entry_style, EINA_TRUE);
   elm_object_text_set(window->entry_style, "The quick brown fox jumps over the lazy dog");
   elm_object_part_content_set(panes_h, "left", window->entry_style);
   elm_entry_editable_set(window->entry_style, EINA_FALSE);
   elm_entry_text_style_user_push(window->entry_style, "DEFAULT='align=center'");
   evas_object_size_hint_max_set(window->entry_style, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(window->entry_style);

   layout_right = _form_right_side(inwin);
   elm_object_part_content_set(panes_h, "right", layout_right);
   evas_object_show(layout_right);

   evas_object_show(inwin);

   return inwin;
}

void
style_viewer_list_callback_add(Evas_Object *object,
                               const char *event,
                               Edje_Signal_Cb func,
                               void *data)
{
   if (!object) return;
   elm_object_signal_callback_add(object, event, "", func, data);
}
