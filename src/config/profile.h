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
* along with this program; If not, see www.gnu.org/licenses/gpl-2.0.html.
*/

#ifndef CONFIG_H
#define CONFIG_H

struct _Config_Color
{
   unsigned char r, g, b;
}
typedef struct _Config_Color Config_Color;

struct _BG
{
   Config_Color color;
   const char *image;
}
typedef struct _BG BG;

struct _Config
{
   const char     *version;
   struct {
      const char     *home_folder;
      const char     *swap_folder;
      Eina_Bool      save_ui : 1;
      Eina_Bool      save_win_pos : 1;
      struct {
         Eina_Bool      autosave : 1;
         unsigned int   period; /* autosave period in seconds */
      } autosave;
   } general;
   struct {
      BG bg;
      struct {
         Config_color   border_color;
         unsigned int   handler_size;
      } groupedit;
      //int rulers;
   } workspace;
   struct {
      BG bg;
      Eina_Bool auto_fill_text : 1;
      Eina_Bool auto_fill_content : 1;
   } liveview;
}
typedef _Config Config;

#endif /* CONFIG_H */
