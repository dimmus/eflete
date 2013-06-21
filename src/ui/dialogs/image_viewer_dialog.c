#include "efl_tet.h"
#include "image_viewer_dialog.h"

typedef struct _Item
{
   int id;
   char *prj;
} Item;

static Elm_Gengrid_Item_Class *gic = NULL;

static char *
_grid_label_get(void        *data __UNUSED__,
                Evas_Object *obj __UNUSED__,
                const char  *part __UNUSED__)
{
   return NULL;
}

/* icon fetching callback */
static Evas_Object *
_grid_content_get(void *data,
               Evas_Object *obj,
               const char  *part __UNUSED__)
{
   const Item *it = data;
   Evas_Object *grid = (Evas_Object *)obj;

   if (!strcmp(part, "elm.swallow.icon"))
     {
        Evas_Object *image = elm_image_add(grid);
        char buf[4096];
        snprintf(buf, sizeof(buf), "edje/images/%i", it->id);
        elm_image_file_set(image, it->prj, buf);
        evas_object_size_hint_aspect_set(image, EVAS_ASPECT_CONTROL_VERTICAL, 1,
                                         1);
        evas_object_show(image);
        return image;
     }
   return NULL;
}

/* state fetching callback */
static Eina_Bool
_grid_state_get(void        *data __UNUSED__,
                Evas_Object *obj __UNUSED__,
                const char  *part __UNUSED__)
{
   return EINA_FALSE;
}

/* deletion callback */
static void
_grid_del(void        *data,
          Evas_Object *obj __UNUSED__)
{
   Item *it = data;
   free(it);
}
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

Evas_Object *
image_viewer_add(Evas_Object *parent)
{
   Evas_Object *inwin;
   Evas_Object *button, *gengrid, *layout;

   inwin = elm_win_inwin_add(parent);

   layout = elm_layout_add(inwin);
   evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_layout_file_set(layout, TET_EDJ, "base/image_viewer");
   elm_win_inwin_content_set(inwin, layout);
   evas_object_show(layout);

   gengrid = elm_gengrid_add(inwin);
   elm_gengrid_item_size_set(gengrid, 100, 100);
   evas_object_size_hint_weight_set(gengrid, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_object_part_content_set(layout, "swallow/images_grid", gengrid);
   elm_gengrid_align_set(gengrid, 0, 0);
   evas_object_show(gengrid);

   button = elm_button_add(layout);
   elm_object_text_set(button, "Add image");
   elm_object_part_content_set(layout, "button_add_image", button);
   evas_object_resize(button, 40, 20);
   evas_object_show(button);

   button = elm_button_add(layout);
   elm_object_text_set(button, "Delete image");
   elm_object_part_content_set(layout, "button_del_image", button);
   evas_object_resize(button, 40, 20);
   evas_object_show(button);

   button = elm_button_add(layout);
   elm_object_text_set(button, "Ok");
   elm_object_part_content_set(layout, "button_ok", button);
   evas_object_smart_callback_add(button, "clicked", _on_button_ok_clicked_cb,
                                   inwin);
   evas_object_resize(button, 40, 20);
   evas_object_show(button);

   button = elm_button_add(layout);
   elm_object_text_set(button, "Cancel");
   elm_object_part_content_set(layout, "button_cancel", button);
   evas_object_smart_callback_add(button, "clicked", _on_button_cancel_clicked_cb,
                                   inwin);
   evas_object_resize(button, 40, 20);
   evas_object_show(button);

   if (!gic)
     {
        gic = elm_gengrid_item_class_new();
        gic->item_style = "default";
        gic->func.text_get = _grid_label_get;
        gic->func.content_get = _grid_content_get;
        gic->func.state_get = _grid_state_get;
        gic->func.del = _grid_del;
     } // we only create the first time its needed. we dont unref/free
   evas_object_show(inwin);
   return inwin;
}

int
_project_images_count_get(Project *project)
{
   int max_value = 0;
   Widget *_widget = NULL;
   Style *_style = NULL;
   Group *_group = NULL;
   Part *_part = NULL;
   Part_State *_state = NULL;

   EINA_INLIST_FOREACH(project->widgets, _widget)
     {
        EINA_INLIST_FOREACH(_widget->styles, _style)
          {
             EINA_INLIST_FOREACH(_style->groups, _group)
               {
                  EINA_INLIST_FOREACH(_group->parts, _part)
                    {
                       if (_part->type == EDJE_PART_TYPE_IMAGE)
                         {
                           EINA_INLIST_FOREACH(_part->states, _state)
                             {
                                if(_state->image->id > max_value)
                                  max_value = _state->image->id;
                             }
                         }
                    }
               }
          }
     }
   return max_value;
}

void
image_viewer_init (Evas_Object *img_view, Project *project)
{
   Evas_Object *gengrid;
   int i = 0, count_images = 0;
   if (!project)
     {
        NOTIFY_ERROR (elm_object_parent_widget_get(img_view),
                      "EDJ/EDC file is not loaded");
        evas_object_del(img_view);
        return;
     }
   gengrid = elm_object_part_content_get(elm_win_inwin_content_get(img_view),
                                          "swallow/images_grid");
   count_images = _project_images_count_get(project);
   for (i = 0; i < count_images; i++)
     {
        Item *it;
        it = (Item *)mem_calloc(1, sizeof(*it));
        it->prj =  project->swapfile;
        it->id = i;
        elm_gengrid_item_append(gengrid, gic, it, NULL, NULL);
     }
}
