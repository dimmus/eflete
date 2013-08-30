#include "widget_manager.h"
#include "alloc.h"

static char **arr;
static char tmp[PATH_MAX];
static int size;
static char empty = '\0';
/**
 * ref http://docs.enlightenment.org/auto/edje/group__Edje__Object__Part.html
 */
static char *part_types[] = {
     "NONE",
     "RECTANGLE",
     "TEXT",
     "IMAGE",
     "SWALLOW",
     "TEXTBLOCK",
     "GRADIENT",
     "GROUP",
     "BOX",
     "TABLE",
     "EXTERNAL",
     "PROXY",
     "SPACER"
};

#define WM_WIDGET_NAME_GET(widget, group) \
   arr = eina_str_split(group, "/", 3); \
   widget = strdup(arr[1]); \
   free(arr[0]); \
   free(arr);

#define WM_STYLE_NAME_GET(style, group) \
   arr = eina_str_split(group, "/", 0); \
   for (size = 0; arr[size]; size++) \
     ; \
   style = strdup(arr[size - 1]);\
   free(arr[0]); \
   free(arr);

#define WM_GROUP_NAME_GET(group_name, style, group) \
   arr = eina_str_split(group, "/", 0); \
   strcpy(tmp, arr[2]); \
   for (size = 3; arr[size]; size++) \
     { \
        if (strcmp(arr[size],style)) \
          { \
             eina_strlcat(tmp, "/", PATH_MAX); \
             eina_strlcat(tmp, arr[size], PATH_MAX); \
          } \
     } \
   group_name = strdup(tmp); \
   free(arr[0]); \
   free(arr);

void
wm_group_data_load(Group *group, Evas *e, const char *edj)
{
   Evas_Object *edje_edit_obj;
   Eina_List *parts_list, *l;
   char *name;
   Part *part;

   if (!group || !e) return;

   edje_edit_obj = edje_edit_object_add(e);

   if (!edje_object_file_set(edje_edit_obj, edj, group->full_group_name))
     {
        evas_object_del(edje_edit_obj);
        return;
     }
   group->obj = edje_edit_obj;
   group->current_w = -1;
   group->current_h = -1;

   parts_list = edje_edit_parts_list_get(group->obj);
   EINA_LIST_FOREACH(parts_list, l, name)
     {
        part = wm_part_add(group->obj, name);
        group->parts = eina_inlist_append(group->parts,
                                          EINA_INLIST_GET(part));
     }
   edje_edit_string_list_free(parts_list);
}

Part *
wm_part_add(Evas_Object *obj, const char *part)
{
   Part *result;
   double value;

   if (!part || !obj) return NULL;

   result = mem_malloc(sizeof(Part));
   result->__type = PART;

   result->name = eina_stringshare_add(part);
   result->type = edje_edit_part_type_get(obj, part);
   result->obj = NULL;
   result->curr_state = edje_edit_part_selected_state_get(obj, part, &value);
   result->curr_state_value = value;
   result->show = EINA_TRUE;

   return result;
}

Eina_Bool
wm_part_free(Part *part)
{
   if (!part) return EINA_FALSE;

   eina_stringshare_del(part->name);
   eina_stringshare_del(part->curr_state);
   if (part->obj) evas_object_del(part->obj);

   free(part);
   part = NULL;

   return EINA_TRUE;
}

Eina_Bool
wm_part_current_state_set(Part *part, const char *state)
{
   char **split;

   if ((!part) || (!state)) return EINA_FALSE;

   split = eina_str_split(state, " ", 2);
   eina_stringshare_del(part->curr_state);
   part->curr_state = eina_stringshare_add(split[0]);
   part->curr_state_value = atof(split[1]);
   free(split[0]);
   free(split);

   return EINA_TRUE;
}

Eina_List *
wm_program_signals_list_get(Group *group)
{
   Eina_List *result = NULL;
   Eina_List *progs, *l;
   Eina_Stringshare *prog_name, *sig_name;

   progs = edje_edit_programs_list_get(group->obj);
   EINA_LIST_FOREACH(progs, l, prog_name)
     {
        sig_name = edje_edit_program_signal_get(group->obj, prog_name);
        if (sig_name)
          result = eina_list_append(result, sig_name);
     }
   edje_edit_string_list_free(progs);

  return result;
}

Eina_Bool
wm_program_signals_list_free(Eina_List *signals)
{
   Eina_Stringshare *sig;
   if (!signals) return EINA_FALSE;

   EINA_LIST_FREE(signals, sig)
      edje_edit_string_free(sig);

   eina_list_free(signals);
   signals = NULL;

   return EINA_TRUE;
}

Group *
wm_group_add(const char* group_name, const char* full_group_name)
{
   Group *group_edje;

   if (!full_group_name || !group_name) return NULL;

   group_edje = mem_malloc(sizeof(Group));
   group_edje->group_name = eina_stringshare_add(group_name);
   group_edje->full_group_name = eina_stringshare_add(full_group_name);
   group_edje->obj = NULL;
   group_edje->parts = NULL;
   group_edje->__type = GROUP;

   return group_edje;
}

Eina_Bool
wm_group_free(Group *group)
{
   Part *part;

   if (!group) return EINA_FALSE;

   if (!group->obj) evas_object_del(group->obj);

   while (group->parts)
     {
        part = EINA_INLIST_CONTAINER_GET(group->parts, Part);
        group->parts = eina_inlist_remove(group->parts, group->parts);
        wm_part_free(part);
     }

   if (!group->group_name)
     eina_stringshare_del(group->group_name);

   eina_stringshare_del(group->full_group_name);

   free(group);
   group = NULL;

   return EINA_TRUE;
}

Style *
wm_style_add(const char *style, Eina_List *groups)
{
   Style *style_edje;
   Eina_List *l;
   Group *group_edje;
   char *group_name, *group_name_full;

   if (!style || !groups) return NULL;

   style_edje = mem_malloc(sizeof(*style_edje));
   style_edje->style_name = eina_stringshare_add(style);
   style_edje->groups = NULL;
   style_edje->__type = STYLE;
   EINA_LIST_FOREACH(groups, l, group_name_full)
     {
        WM_GROUP_NAME_GET(group_name, style_edje->style_name, group_name_full);
        group_edje = wm_group_add(group_name, group_name_full);
        style_edje->groups = eina_inlist_append(style_edje->groups,
                                                EINA_INLIST_GET(group_edje));
     }

   free(group_name_full);
   free(group_name);

   return style_edje;
}

Eina_Bool
wm_style_free(Style *style)
{
   Group *group_edje;
   Eina_Inlist *list;

   if (!style) return EINA_FALSE;

   list = style->groups;

   if (list)
     {
        while (list)
          {
             group_edje = EINA_INLIST_CONTAINER_GET(list, Group);
             list = eina_inlist_remove(list, list);
             wm_group_free(group_edje);
          }
     }

   eina_stringshare_del(style->style_name);
   free(style);

   return EINA_TRUE;
}

int
_sort_style_cb(const void *data1, const void *data2)
{
   const char *str1 = eina_stringshare_add(data1);
   const char *str2 = eina_stringshare_add(data2);
   char *data1_style;
   char *data2_style = NULL;

   if (!str1) return 1;
   if (!str2) return -1;

   WM_STYLE_NAME_GET(data1_style, str1);
   WM_STYLE_NAME_GET(data2_style, str2);

   int cmp = (strcmp(data1_style, data2_style));
   free(data1_style);
   free(data2_style);
   eina_stringshare_del(str1);
   eina_stringshare_del(str2);
   return cmp;
}

Widget *
wm_widget_add(const char *widget, Eina_List *groups)
{
   Widget *_widget = NULL;
   Eina_List *l, *l_next;
   Eina_List *widget_groups = NULL;
   Style *style;
   char *style_name = NULL, *style_name_next = NULL;
   char *group, *group_next;

   if (!widget) return NULL;

   _widget = mem_malloc(sizeof(*_widget));
   _widget->widget_name = eina_stringshare_add(widget);
   _widget->styles = NULL;
   _widget->__type = WIDGET;

   groups = eina_list_sort(groups,
                           eina_list_count(groups),
                           _sort_style_cb);

   EINA_LIST_FOREACH_SAFE(groups, l, l_next, group)
     {
        free(style_name_next);
        WM_STYLE_NAME_GET(style_name, group);
        if (l_next)
          {
             group_next = eina_list_data_get(l_next);
             WM_STYLE_NAME_GET(style_name_next, group_next);
          }
        else
          style_name_next = &empty;

        widget_groups = eina_list_append(widget_groups, group);

        if (strcmp(style_name, style_name_next) != 0)
          {
             style = wm_style_add(style_name, widget_groups);
             _widget->styles = eina_inlist_append(_widget->styles,
                                                  EINA_INLIST_GET(style));
             widget_groups = eina_list_free(widget_groups);
             widget_groups = NULL;
          }
        free(style_name);
     }

   return _widget;
}

Eina_Bool
wm_widget_free(Widget *widget)
{
   Style *style;
   Eina_Inlist  *list;

   if (!widget) return EINA_FALSE;

   list = widget->styles;

   if (list)
     {
        while (list)
          {
             style = EINA_INLIST_CONTAINER_GET(list, Style);
             list = eina_inlist_remove(list, list);
             wm_style_free(style);
          }
     }
   eina_stringshare_del(widget->widget_name);
   free(widget);

   return EINA_TRUE;
}

int sort_collection_cb(const void *data1, const void *data2)
{
   const char *str1 = eina_stringshare_add(data1);
   const char *str2 = eina_stringshare_add(data2);

   if (!str1) return 1;
   if (!str2) return -1;

   int cmp = (strcmp(str1, str2));
   eina_stringshare_del(str1);
   eina_stringshare_del(str2);
   return cmp;
}

Eina_Inlist *
wm_widget_list_new(const char *file)
{
   Widget *widget;
   Eina_List *collection, *l, *l_next;
   Eina_List *widget_styles = NULL;
   Eina_Inlist *widget_list = NULL;
   char *widget_name = NULL, *widget_name_next = NULL;
   char *group, *group_next;
   const char prefix[] = "elm/";
   Eina_Error error;

   if (!file) return NULL;

   DBG("Start to parse the edje group collection.");
   collection = edje_file_collection_list(file);
   if (!collection)
     {
        error = eina_error_get();
        ERR("%s", eina_error_msg_get(error));
     }

   collection = eina_list_sort(collection,
                               eina_list_count(collection),
                               sort_collection_cb);

   EINA_LIST_FOREACH_SAFE(collection, l, l_next, group)
     {
        if (!eina_str_has_prefix(group, prefix))
          ERR("Invalid style name in group: %s", group);
        else
          {
             free(widget_name_next);
             WM_WIDGET_NAME_GET(widget_name, group);
             if (l_next)
               {
                  group_next = eina_list_data_get(l_next);
                  WM_WIDGET_NAME_GET(widget_name_next, group_next);
               }
             else
               widget_name_next = &empty;

             widget_styles = eina_list_append(widget_styles, group);

             if (strcmp(widget_name, widget_name_next) != 0)
               {
                  widget = wm_widget_add(widget_name, widget_styles);
                  widget_list = eina_inlist_append(widget_list,
                                                   EINA_INLIST_GET(widget));
                  widget_styles = eina_list_free(widget_styles);
                  widget_styles = NULL;
               }
             free(widget_name);
          }
     }
   edje_file_collection_list_free(collection);
   DBG("Parse the edje group collection id finished! The list of widget - created.");
   INFO("The theme consist a styles for %i widgets.",
        eina_inlist_count(widget_list));

   return widget_list;
}

Eina_Bool
wm_widget_list_free(Eina_Inlist *widget_list)
{
   Widget *widget;

   if (!widget_list) return EINA_FALSE;

   while (widget_list)
     {
        widget = EINA_INLIST_CONTAINER_GET(widget_list, Widget);
        widget_list = eina_inlist_remove(widget_list, widget_list);
        wm_widget_free(widget);
     }

   return EINA_TRUE;
}

Evas_Object *
wm_group_object_find(Eina_Inlist *widget_list, const char *group_full_name)
{
   char *widget_name = NULL;
   char *style_name = NULL;
   char *group_name = NULL;
   Group *_group = NULL;
   Widget *_widget = NULL;
   Style *_style = NULL;

   WM_WIDGET_NAME_GET(widget_name, group_full_name);
   if (widget_name [0] <= 'm')
     EINA_INLIST_FOREACH(widget_list, _widget)
       {
          if (!strcmp(_widget->widget_name, widget_name))
            break;
       }
   else
     EINA_INLIST_REVERSE_FOREACH(widget_list, _widget)
       {
          if (!strcmp(_widget->widget_name, widget_name))
            break;
       }
   if (!_widget) return NULL;

   WM_STYLE_NAME_GET(style_name, group_full_name);

   if (style_name [0] <= 'm')
     EINA_INLIST_FOREACH(_widget->styles, _style)
       {
          if (!strcmp(_style->style_name, style_name))
            break;
       }
   else
     EINA_INLIST_REVERSE_FOREACH(_widget->styles, _style)
       {
          if (!strcmp(_style->style_name, style_name))
            break;
       }
   if (!_style) return NULL;

   WM_GROUP_NAME_GET(group_name, style_name, group_full_name)
   if (group_name [0] <= 'm')
     EINA_INLIST_FOREACH(_style->groups, _group)
       {
          if (!strcmp(_group->group_name, group_name))
            break;
       }
   else
     EINA_INLIST_REVERSE_FOREACH(_style->groups, _group)
       {
          if (!strcmp(_group->group_name, group_name))
            break;
       }

   free(widget_name);
   free(style_name);
   free(group_name);

   return _group->obj;
}

void
wm_widget_list_objects_load(Eina_Inlist *widget_list,
                            Evas *e,
                            const char *path)
{
   Widget *widget;
   Style *style;
   Group *group;

   if ((!widget_list) || (!e) || (!path)) return;

   EINA_INLIST_FOREACH(widget_list, widget)
     {
        EINA_INLIST_FOREACH(widget->styles, style)
          {
             EINA_INLIST_FOREACH(style->groups, group)
               {
                  wm_group_data_load(group, e, path);
               }
          }
     }
}

const char *
wm_part_type_get(Edje_Part_Type type)
{
   return part_types[type];
}

#undef WM_WIDGET_NAME_GET
#undef WM_STYLE_NAME_GET
#undef WM_GROUP_NAME_GET
