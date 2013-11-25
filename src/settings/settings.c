/* Edje Theme Editor
* Copyright (C) 2013 Samsung Electronics.
*
* This file is part of Edje Theme Editor.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; If not, see .
*/

#include "settings.h"

typedef struct _UI_Settings UI_Settings;
typedef struct _Panes_Settings Panes_Settings;
typedef struct _Window_Settings Window_Settings;

struct _Window_Settings
{
   int x;
   int y;
   int width;
   int height;
};

struct _Panes_Settings
{
   double left;
   double right;
};


struct _UI_Settings
{
   Eina_List *panes;
   Eina_List *window;
};

static Eet_Data_Descriptor *_ui_settings_descriptor;
static Eet_Data_Descriptor *_panes_descriptor;
static Eet_Data_Descriptor *_window_descriptor;
extern UI_Elements_Settings *us;

static void
_ui_settings_descriptors_init(void)
{
   Eet_Data_Descriptor_Class eddc;

   EET_EINA_FILE_DATA_DESCRIPTOR_CLASS_SET(&eddc, UI_Settings);
   _ui_settings_descriptor = eet_data_descriptor_file_new (&eddc);

   EET_EINA_FILE_DATA_DESCRIPTOR_CLASS_SET(&eddc, Panes_Settings);
   _panes_descriptor = eet_data_descriptor_file_new (&eddc);

   EET_EINA_FILE_DATA_DESCRIPTOR_CLASS_SET(&eddc, Window_Settings);
   _window_descriptor = eet_data_descriptor_file_new (&eddc);


   #define TET_CONF_ADD_BASIC(member)				\
   EET_DATA_DESCRIPTOR_ADD_BASIC				\
   (_panes_descriptor,						\
    Panes_Settings,# member, member,EET_T_DOUBLE)
   TET_CONF_ADD_BASIC(left);
   TET_CONF_ADD_BASIC(right);
   #undef TET_CONF_ADD_BASIC

   #define TET_CONF_ADD_BASIC(member)				\
   EET_DATA_DESCRIPTOR_ADD_BASIC				\
   (_window_descriptor,					\
    Window_Settings,# member, member,EET_T_INT)
   TET_CONF_ADD_BASIC(x);
   TET_CONF_ADD_BASIC(y);
   TET_CONF_ADD_BASIC(width);
   TET_CONF_ADD_BASIC(height);
   #undef TET_CONF_ADD_BASIC


   EET_DATA_DESCRIPTOR_ADD_LIST(
      _ui_settings_descriptor, UI_Settings,"panes_list",
      panes, _panes_descriptor);
   EET_DATA_DESCRIPTOR_ADD_LIST(
      _ui_settings_descriptor, UI_Settings, "window_list",
      window, _window_descriptor);
}

static void
_ui_settings_descriptors_shutdown(void)
{
   eet_data_descriptor_free(_panes_descriptor);
   eet_data_descriptor_free(_window_descriptor);
   eet_data_descriptor_free(_ui_settings_descriptor);
}

static Panes_Settings *
_ui_settings_panes_add(Evas_Object *panes)
{
   Panes_Settings *_panes_sett = mem_malloc(sizeof(Panes_Settings));
   _panes_sett->left = elm_panes_content_left_size_get(panes);
   _panes_sett->right = elm_panes_content_right_size_get(panes);
   return _panes_sett;
}

/**
 * Prepare data from elements for save.
 */
static UI_Settings *
_ui_settings_prepare()
{
   UI_Settings *_ui_settings = mem_malloc(sizeof(UI_Settings));
   Panes_Settings *_panes_set;
   Eina_List *_settings_to_save = NULL;
   Eina_List *_window_list = NULL;
   Window_Settings *_window_sett = calloc(1, sizeof(Window_Settings));
   if (!us) return NULL;
   _panes_set = _ui_settings_panes_add(us->panes_left);
   _settings_to_save = eina_list_append(_settings_to_save, _panes_set);

   _panes_set = _ui_settings_panes_add(us->panes_left_hor);
   _settings_to_save = eina_list_append(_settings_to_save, _panes_set);

   _panes_set = _ui_settings_panes_add(us->panes_right);
   _settings_to_save = eina_list_append(_settings_to_save, _panes_set);

   _panes_set = _ui_settings_panes_add(us->panes_right_hor);
   _settings_to_save = eina_list_append(_settings_to_save, _panes_set);

   _panes_set = _ui_settings_panes_add(us->panes_center);
   _settings_to_save = eina_list_append(_settings_to_save,
                                        _panes_set);

   _panes_set = _ui_settings_panes_add(us->panes_center_down);
   _settings_to_save = eina_list_append(_settings_to_save, _panes_set);

   _ui_settings->panes = _settings_to_save;

   evas_object_geometry_get(us->window, NULL, NULL,
                             &_window_sett->width,
                             &_window_sett->height);
   elm_win_screen_position_get(us->window,
                                &_window_sett->x, &_window_sett->y);
   _window_list = eina_list_append(_window_list, _window_sett);
   _ui_settings->window = _window_list;

   return _ui_settings;
}

/**
 * Apply loaded settings to elements.
 */
static void
_ui_settings_apply(UI_Settings *ui_set)
{
   Eina_List *_panes_list = ui_set->panes;
   Eina_List *_window_list = ui_set->window;
   Panes_Settings *_panes_sett;
   Window_Settings *_window_sett;

   _window_sett = (Window_Settings *)_window_list->data;
   evas_object_resize(us->window, _window_sett->width,
                       _window_sett->height);
   evas_object_move(us->window, _window_sett->x,
                     _window_sett->y);

   _panes_sett =(Panes_Settings *)_panes_list->data;
   elm_panes_content_left_size_set(us->panes_left, _panes_sett->left);

   _panes_list = eina_list_next(_panes_list);
   _panes_sett =(Panes_Settings *)_panes_list->data;
   elm_panes_content_left_size_set(us->panes_left_hor, _panes_sett->left);

   _panes_list = eina_list_next(_panes_list);
   _panes_sett =(Panes_Settings *)_panes_list->data;
   elm_panes_content_left_size_set(us->panes_right, _panes_sett->left);

   _panes_list = eina_list_next(_panes_list);
   _panes_sett =(Panes_Settings *)_panes_list->data;
   elm_panes_content_left_size_set(us->panes_right_hor, _panes_sett->left);

   _panes_list = eina_list_next(_panes_list);
   _panes_sett =(Panes_Settings *)_panes_list->data;
   elm_panes_content_left_size_set(us->panes_center, _panes_sett->left);

   _panes_list = eina_list_next(_panes_list);
   _panes_sett =(Panes_Settings *)_panes_list->data;
   elm_panes_content_left_size_set(us->panes_center_down, _panes_sett->left);
}

Eina_Bool
ui_settings_save()
{
   Eet_File *file_settings;
   int state_write;
   UI_Settings *_ui_settings;

   _ui_settings_descriptors_init();

   _ui_settings = _ui_settings_prepare();
   if (!_ui_settings)
     {
        ERR ("Could not prepare UI_Settings");
        return EINA_FALSE;
     }

   file_settings = eet_open(SETTINGSFILE, EET_FILE_MODE_WRITE);
   if (!file_settings)
     {
        WARN("Unable to open configs file for write");
        free(_ui_settings);
        return EINA_FALSE;
    }

   state_write = eet_data_write(file_settings, _ui_settings_descriptor,
                                "settings", _ui_settings, EINA_FALSE);

   if (!state_write)
     WARN ("Unable to save data into file");
   eet_close(file_settings);

   _ui_settings_descriptors_shutdown();

   return EINA_TRUE;
}

Eina_Bool
ui_settings_load()
{
   UI_Settings *_ui_settings;
   Eet_File *file_settings;

   _ui_settings_descriptors_init();
   file_settings = eet_open(SETTINGSFILE, EET_FILE_MODE_READ);
   if (!file_settings)
     {
        WARN("Unable open configs file for load. Try load defaults settings");
        return EINA_FALSE;
     }

   _ui_settings = eet_data_read(file_settings,
                                 _ui_settings_descriptor, "settings");
   if (!_ui_settings)
     {
        ERR("Unable to load ui setings. Try load defaults settings");
        return EINA_FALSE;
    }

   _ui_settings_apply(_ui_settings);
   _ui_settings_descriptors_shutdown();
   eet_close(file_settings);

   return EINA_TRUE;
}


UI_Elements_Settings *
ui_element_settings_init(void)
{
   return mem_calloc (1, sizeof (UI_Elements_Settings));
}
