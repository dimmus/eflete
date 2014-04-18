/**
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
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

#include "edje_compile.h"
#include "alloc.h"

static Eina_Bool
_exe_data(void *data,
         int type,
         void *event)
{
   Ecore_Exe_Event_Data *ev = event;
   Edje_Compile_Log_Cb msg_cb = (Edje_Compile_Log_Cb)data;
   if (ev->lines)
     {
        int i;
        for (i = 0; ev->lines[i].line; i++)
          {
             msg_cb(time(NULL), eina_stringshare_add(ev->lines[i].line), type);
          }
     }

   return ECORE_CALLBACK_PASS_ON;
}

static Eina_Bool
_exe_exit(void *data,
         int type __UNUSED__,
         void *event)
{
   Ecore_Exe_Event_Del *e = (Ecore_Exe_Event_Del *)event;
   int * exit_code = (int *) data;
   *exit_code = e->exit_code;
   ecore_main_loop_quit();

   return EINA_TRUE;
}

static int
_exec(Eina_Stringshare *cmd, Edje_Compile_Log_Cb log_cb)
{
   int exit_code = 0;
   Ecore_Event_Handler *cb_exit, *cb_msg_stdout, *cb_msg_stderr;
   Ecore_Exe_Flags flags  = ECORE_EXE_PIPE_READ |
                            ECORE_EXE_PIPE_READ_LINE_BUFFERED |
                            ECORE_EXE_PIPE_ERROR |
                            ECORE_EXE_PIPE_ERROR_LINE_BUFFERED;

   cb_exit = ecore_event_handler_add(ECORE_EXE_EVENT_DEL, _exe_exit, &exit_code);
   if (log_cb)
     {
        cb_msg_stdout = ecore_event_handler_add(ECORE_EXE_EVENT_DATA, _exe_data, log_cb);
        cb_msg_stderr = ecore_event_handler_add(ECORE_EXE_EVENT_ERROR, _exe_data, log_cb);
     }

   ecore_exe_pipe_run(cmd, flags, NULL);
   ecore_main_loop_begin();

   ecore_event_handler_del(cb_exit);
   if (log_cb)
     {
        ecore_event_handler_del(cb_msg_stdout);
        ecore_event_handler_del(cb_msg_stderr);
     }
   return exit_code;
}

int
compile(const char *edc,
        const char *edj,
        const char *image_directory,
        const char *font_directory,
        const char *sound_directory,
        Edje_Compile_Log_Cb log_cb)
{
   if ((!edc) || (!strcmp(edc, ""))) return -1;
   if ((!edj) || (!strcmp(edj, ""))) return -2;

   Eina_Stringshare *cmd = eina_stringshare_printf("edje_cc -v %s%s %s%s %s%s %s %s",
           ((image_directory) && strcmp(image_directory, "")) ? "-id " : "",
           ((image_directory) && strcmp(image_directory, "")) ? image_directory : "",
           ((font_directory) && strcmp(font_directory, "")) ? "-fd " : "",
           ((font_directory) && strcmp(font_directory, "")) ? font_directory : "",
           ((sound_directory) && strcmp(sound_directory, "")) ? "-sd " : "",
           ((sound_directory) && strcmp(sound_directory, "")) ? sound_directory : "",
           edc, edj);
   INFO("Run command: %s", cmd);

   int exit_code = _exec(cmd, log_cb);
   eina_stringshare_del(cmd);
   return exit_code;
}

int
decompile(const char *edj,
          const char *dest_folder,
          Edje_Compile_Log_Cb log_cb)
{
   if ((!dest_folder) || (!strcmp(dest_folder, ""))) return -1;
   if ((!edj) || (!strcmp(edj, ""))) return -2;

   Eina_Stringshare *cmd = eina_stringshare_printf("edje_decc %s -no-build-sh -o %s",
                                                   edj, dest_folder);
   INFO("Run command: %s", cmd);

   int exit_code = _exec(cmd, log_cb);
   eina_stringshare_del(cmd);
   return exit_code;
}
