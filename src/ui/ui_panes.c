#include "ui_settings.h"
#include "ui_block.h"

void
_print_current_state(char *);

void
_ui_panes_current_state_set(void);

struct _Panes_Attributes
{
   int left_size;
   int right_size;
};
typedef struct _Panes_Attributes Panes_Attributes;

struct _Window_Attributes
{
   int width;
   int height;
};
typedef struct _Window_Attributes Window_Attributes;

struct _UI_Current_State_Panes
{
   Panes_Attributes panes_left;
   Panes_Attributes panes_left_hor;
   Panes_Attributes panes_right;
   Panes_Attributes panes_right_hor;
   Panes_Attributes panes_center;
   Panes_Attributes panes_center_down;
   Window_Attributes window;
};
typedef struct _UI_Current_State_Panes UI_Current_State_Panes;

UI_Current_State_Panes *ui_csp;
UI_Elements_Settings *us;

/*
   Store main panes in global pointer. This need for have access to all panes.
 */
Evas_Object *panes;

UI_Current_State_Panes *
_ui_panes_current_state_struct_init(void)
{
   UI_Current_State_Panes *csp = calloc(1, sizeof(UI_Current_State_Panes));
   csp->panes_left.left_size = 0;
   csp->panes_left.right_size = 0;

   csp->panes_left_hor.left_size = 0;
   csp->panes_left_hor.right_size = 0;

   csp->panes_right.left_size = 0;
   csp->panes_right.right_size = 0;

   csp->panes_right_hor.left_size = 0;
   csp->panes_right_hor.right_size = 0;

   csp->panes_center.left_size = 0;
   csp->panes_center.right_size = 0;

   csp->panes_center_down.left_size = 0;
   csp->panes_center_down.right_size = 0;

   csp->window.width = 1000;
   csp->window.height = 800;

   return csp;
}

void
_ui_panes_current_state_get(void)
{
   double size_get = 0.0;

   Evas_Object *_panes;
   Evas_Object *_panes_temp;

   _panes = panes;
   _panes_temp = NULL;

   Eina_Bool _need_resize = EINA_FALSE;

   size_get = elm_panes_content_left_size_get(_panes);
   ui_csp->panes_left.left_size = (int)(ui_csp->window.width * size_get);
   size_get = elm_panes_content_right_size_get(_panes);
   ui_csp->panes_left.right_size = (int)(ui_csp->window.width * size_get);

   _panes = elm_object_part_content_get(panes, "left");
   size_get = elm_panes_content_left_size_get(_panes);
   ui_csp->panes_left_hor.left_size =
      (int)((ui_csp->window.height - 20) * size_get);
   size_get = elm_panes_content_right_size_get(_panes);
   ui_csp->panes_left_hor.right_size =
      (int)((ui_csp->window.height - 20) * size_get);

   _panes = elm_object_part_content_get(panes, "right");
   size_get = elm_panes_content_left_size_get(_panes);
   ui_csp->panes_right.left_size =
      (int)((ui_csp->window.width - ui_csp->panes_left.left_size)* size_get);
   size_get = 1 - elm_panes_content_left_size_get(_panes);
   ui_csp->panes_right.right_size =
      (int)((ui_csp->window.width-ui_csp->panes_left.left_size)* size_get);

   _panes_temp = elm_object_part_content_get(_panes, "right");
   size_get = elm_panes_content_left_size_get(_panes_temp);
   ui_csp->panes_right_hor.left_size =
      (int)((ui_csp->window.height - 20)*size_get);
   size_get = elm_panes_content_right_size_get(_panes_temp);
   ui_csp->panes_right_hor.right_size =
      (int)((ui_csp->window.height - 20) * size_get);

   _panes_temp = elm_object_part_content_get(_panes, "left");
   size_get = elm_panes_content_left_size_get(_panes_temp);
   ui_csp->panes_center.left_size =
      (int)((ui_csp->window.height - 20)* size_get);
   size_get = elm_panes_content_right_size_get(_panes_temp);
   ui_csp->panes_center.right_size =
      (int)((ui_csp->window.height - 20)* size_get);

   /*	Check for minimal size to workspace. 300x300px. */
   if (ui_csp->panes_center.left_size < 300)
     {
        int _shift_size = 0;
        _shift_size = 300 - ui_csp->panes_center.left_size;
        ui_csp->panes_center.left_size = 300;
        ui_csp->panes_center.right_size -= _shift_size;
        _need_resize = EINA_TRUE;
        _shift_size = 0;
     }

   /*	Check for minimal size to workspace. 300x300px. */
   if (ui_csp->panes_right.left_size < 300)
     {
        int _shift_size = 0;
        _shift_size = 300 - ui_csp->panes_right.left_size;
        ui_csp->panes_right.left_size = 300;
        ui_csp->panes_right.right_size -= _shift_size;
        _need_resize = EINA_TRUE;
        _shift_size = 0;
     }

   _panes = elm_object_part_content_get(_panes_temp, "right");
   size_get = elm_panes_content_left_size_get(_panes);
   ui_csp->panes_center_down.left_size=
      (int)(ui_csp->panes_right.left_size*size_get);
   size_get =elm_panes_content_right_size_get(_panes);
   ui_csp->panes_center_down.right_size=
      (int)(ui_csp->panes_right.left_size*size_get);

   if (_need_resize)
     _ui_panes_current_state_set();
}

void
_ui_panes_current_state_set(void)
{
   double size_set = 0.0;
   Evas_Object *_panes;

   _panes = panes;
   size_set = (double)(ui_csp->panes_left.left_size) / (ui_csp->window.width);
   elm_panes_content_left_size_set(_panes, size_set);

   _panes = elm_object_part_content_get(panes, "right");
   size_set = (double)(ui_csp->panes_right.right_size) /
      (ui_csp->window.width - ui_csp->panes_left.left_size);
   elm_panes_content_right_size_set(_panes, size_set);

   _panes = elm_object_part_content_get(_panes, "left");
   size_set = (double)(ui_csp->panes_center.right_size) /
      (ui_csp->window.height - 20);
   elm_panes_content_right_size_set(_panes, size_set);
}


void
ui_resize_pans(int w, int h)
{
   ui_csp->window.height = h;
   ui_csp->window.width = w;
   _ui_panes_current_state_set();
}

static void
_unpress_cb(void *data __UNUSED__,
            Evas_Object *obj __UNUSED__,
            void *event_info __UNUSED__)
{
   _ui_panes_current_state_get();
}

static void
_double_click_up_cb(void * data __UNUSED__,
                     Evas_Object *obj,
                     void *event_info __UNUSED__)
{
   static double _size = 0.0;
   if (elm_panes_content_left_size_get(obj) > 0)
     {
        _size = elm_panes_content_left_size_get(obj);
        elm_panes_content_left_size_set(obj, 0.0);
     }
   else
     {
        if (!_size) _size = 0.3;
        elm_panes_content_left_size_set(obj, _size);
     }

}

static void
_double_click_down_cb(void * data,
                       Evas_Object *obj,
                       void *event_info __UNUSED__)
{
   static double _size[2] = {0.0 , 0.0};
   int *_number = (int*)data;
   if (elm_panes_content_right_size_get(obj) > 0)
     {
        _size[*_number]=elm_panes_content_right_size_get(obj);
        elm_panes_content_right_size_set(obj, 0.0);
     }
   else
     {
        if (!_size[*_number])
          {
             if (!*_number)
               _size[*_number] = 0.2;
             else
               _size[*_number] = 0.4;
          }
        elm_panes_content_right_size_set(obj, _size[*_number]);
     }
}

void
ui_panes_settings_save()
{
   _ui_panes_current_state_set();
   ui_settings_save();
}

void
ui_panes_settings_load()
{
   ui_settings_load();
   _ui_panes_current_state_get ();
}

Eina_Bool
ui_panes_add(App_Data *ap)
{
   Evas_Object *block;
   Evas_Object *panes_left, *panes_left_hor, *panes_right;
   Evas_Object *panes_center, *panes_center_down, *panes_right_hor;

   /*
      _panes_id_center and panes_id_left needs for identify pans in callback
      for double click mouse button. For center panes id == 0, and for left
      panes id == 1.
    */

   int *_panes_id_center = calloc(1, sizeof(int));
   int *_panes_id_left = calloc(1, sizeof(int));
   *_panes_id_center = 0;
   *_panes_id_left = 1;

   us = ui_element_settings_init();
   if (!us){
        NOTIFY_ERROR (ap->win, "ERROR: unable initialize settings module");
        free(_panes_id_center);
        free(_panes_id_left);
        return EINA_FALSE;
   }
   ui_csp =_ui_panes_current_state_struct_init();

   panes_left = elm_panes_add(ap->win_layout);
   if (panes_left == NULL)
     {
        free(_panes_id_center);
        free(_panes_id_left);
        return EINA_FALSE;
     }
   evas_object_size_hint_weight_set(panes_left,
                                    EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panes_left,
                                   EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_panes_content_left_size_set(panes_left, 0.2);
   panes = panes_left;
   elm_object_part_content_set(ap->win_layout, "swallow/panes", panes_left);

   panes_left_hor = elm_panes_add(ap->win_layout);
   if (panes_left_hor == NULL)
     {
        free(_panes_id_center);
        free(_panes_id_left);
        return EINA_FALSE;
     }
   elm_panes_horizontal_set(panes_left_hor, EINA_TRUE);
   evas_object_size_hint_weight_set(panes_left_hor,
                                    EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panes_left_hor,
                                   EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_part_content_set(panes_left, "left", panes_left_hor);
   elm_panes_content_left_size_set(panes_left_hor, 0.45);

   panes_right = elm_panes_add(ap->win_layout);
   if (panes_right == NULL)
     {
        free(_panes_id_center);
        free(_panes_id_left);
        return EINA_FALSE;
     }
   evas_object_size_hint_weight_set(panes_right,
                                    EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panes_right,
                                   EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_part_content_set(panes_left, "right", panes_right);
   elm_panes_content_left_size_set(panes_right, 0.75);

   panes_right_hor = elm_panes_add(ap->win_layout);
   if (panes_right_hor == NULL)
     {
        free(_panes_id_center);
        free(_panes_id_left);
        return EINA_FALSE;
     }
   elm_panes_horizontal_set(panes_right_hor, EINA_TRUE);
   evas_object_size_hint_weight_set(panes_right_hor,
                                    EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panes_right_hor,
                                   EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_part_content_set(panes_right, "right", panes_right_hor);
   elm_panes_content_left_size_set(panes_right_hor, 0.45);

   panes_center = elm_panes_add(ap->win_layout);
   if (panes_center == NULL)
     {
        free(_panes_id_center);
        free(_panes_id_left);
        return EINA_FALSE;
     }
   elm_panes_horizontal_set(panes_center, EINA_TRUE);
   evas_object_size_hint_weight_set(panes_center,
                                    EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panes_center,
                                   EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_part_content_set(panes_right, "left", panes_center);
   elm_panes_content_left_size_set(panes_center, 0.65);

   panes_center_down = elm_panes_add(ap->win_layout);
   if (panes_center_down == NULL)
     {
        free(_panes_id_center);
        free(_panes_id_left);
        return EINA_FALSE;
     }
   evas_object_size_hint_weight_set(panes_center_down,
                                    EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(panes_center_down,
                                   EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_object_part_content_set(panes_center, "right", panes_center_down);

   us->panes_left = panes_left;
   us->panes_right = panes_right;
   us->panes_left_hor = panes_left_hor;
   us->panes_right_hor = panes_right_hor;
   us->panes_center = panes_center;
   us->panes_center_down = panes_center_down;
   us->window = ap->win;

   evas_object_smart_callback_add(panes_left,"unpress",_unpress_cb, NULL);
   evas_object_smart_callback_add(panes_left_hor,"unpress",_unpress_cb, NULL);
   evas_object_smart_callback_add(panes_right,"unpress",_unpress_cb, NULL);
   evas_object_smart_callback_add(panes_right_hor,"unpress",_unpress_cb, NULL);
   evas_object_smart_callback_add(panes_center,"unpress",_unpress_cb, NULL);
   evas_object_smart_callback_add(panes_center_down,"unpress",
                                  _unpress_cb, NULL);
   evas_object_smart_callback_add(panes_right_hor, "clicked,double",
                                  _double_click_up_cb, NULL);
   evas_object_smart_callback_add(panes_left_hor, "clicked,double",
                                  _double_click_down_cb, _panes_id_left);
   evas_object_smart_callback_add(panes_center, "clicked,double",
                                  _double_click_down_cb, _panes_id_center);


   block = ui_block_add(ap->win_layout);
   ui_block_title_text_set(block, "Part States");
   elm_object_part_content_set(panes_center_down, "left", block);
   evas_object_show(block);
   ap->block.bottom_left = block;

   block = ui_block_add(ap->win_layout);
   elm_object_part_content_set(panes_center_down, "right", block);
   evas_object_show(block);
   ap->block.bottom_right = block;

   block = ui_block_add(ap->win_layout);
   elm_object_part_content_set(panes_center, "left", block);
   evas_object_show(block);
   ap->block.canvas = block;

   block = ui_block_add(ap->win_layout);
   elm_object_part_content_set(panes_left_hor, "left", block);
   evas_object_show(block);
   ap->block.left_top = block;

   block = ui_block_add(ap->win_layout);
   ui_block_title_text_set(block, "Using Signals");
   elm_object_part_content_set(panes_left_hor, "right", block);
   evas_object_show(block);
   ap->block.left_bottom = block;

   block = ui_block_add(ap->win_layout);
   elm_object_part_content_set(panes_right_hor, "left", block);
   evas_object_show(block);
   ap->block.right_top = block;

   block = ui_block_add(ap->win_layout);
   ui_block_title_text_set(block, "Property");
   elm_object_part_content_set(panes_right_hor, "right", block);
   evas_object_show(block);
   ap->block.right_bottom = block;

   free(_panes_id_center);
   free(_panes_id_left);

   return EINA_TRUE;
}

void
ui_panes_show(App_Data *ap)
{
   elm_object_signal_emit(ap->win_layout, "window,panes,show", "");
}

void
ui_panes_hide(App_Data *ap)
{
   elm_object_signal_emit(ap->win_layout, "window,panes,hide", "");
}
