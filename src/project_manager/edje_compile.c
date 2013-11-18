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

#include "edje_compile.h"
#include "alloc.h"

static void
compiler_message_clear(Eina_Inlist *list)
{
   if(list)
     {
        while(list)
          {
             Compiler_Message *mes = EINA_INLIST_CONTAINER_GET(list,
                                                               Compiler_Message);
             list = eina_inlist_remove(list, EINA_INLIST_GET(mes));
             free(mes->text);
             free(mes);
          }
     }
}

/***
 *
 */
static Eina_Bool
exe_data(void *data __UNUSED__,
         int type __UNUSED__,
         void *event)
{
   Ecore_Exe_Event_Data *ev;
   Eina_Inlist *messages = NULL;
   Compiler_Message *message = NULL;

   ev = event;
   messages = (Eina_Inlist *)data;

   if (ev->lines)
     {
        int i;
        for (i = 0; ev->lines[i].line; i++)
          {
             message = mem_malloc(sizeof(*message));
             message->time = time(NULL);
             message->text = strdup(ev->lines[i].line);
             messages = eina_inlist_append(messages, EINA_INLIST_GET(message));
             DBG("%s", message->text);
          }
     }

   return ECORE_CALLBACK_PASS_ON;
}

static Eina_Bool
exe_exit(void *data __UNUSED__,
         int type __UNUSED__,
         void *event __UNUSED__)
{
   INFO("End of compile.");

   ecore_main_loop_quit();

   return EINA_TRUE;
}

Edje_CC *
compile(const char *edc,
        const char *edj,
        const char *image_directory,
        const char *font_directory,
        const char *sound_directory)
{
   Edje_CC *edjecc = NULL;
   int size;
   Ecore_Exe_Flags flags  = ECORE_EXE_PIPE_READ |
                            ECORE_EXE_PIPE_READ_LINE_BUFFERED |
                            ECORE_EXE_PIPE_ERROR |
                            ECORE_EXE_PIPE_ERROR_LINE_BUFFERED;

   edjecc = mem_malloc(sizeof(*edjecc));
   edjecc->messages = NULL;
   size = strlen(edc) + strlen(edj) +
      strlen(image_directory) + strlen(font_directory) +
      strlen(sound_directory) + BUFF_MAX;
   edjecc->cmd = (char *)malloc(size);
   sprintf(edjecc->cmd, "edje_cc -id %s -fd %s -sd %s %s %s",
           image_directory, font_directory, sound_directory,
           edc, edj);
   INFO("Run command: %s", edjecc->cmd);

   ecore_event_handler_add(ECORE_EXE_EVENT_DEL, exe_exit, NULL);
   ecore_event_handler_add(ECORE_EXE_EVENT_DATA, exe_data, edjecc->messages);
   ecore_event_handler_add(ECORE_EXE_EVENT_ERROR, exe_data, edjecc->messages);

   edjecc->exe = ecore_exe_pipe_run(edjecc->cmd, flags, NULL);
   ecore_main_loop_begin();

   return edjecc;
}

/*
   TODO: Saving decompiled EDC into another folder.
   edje_decc dont have ability to save EDC into another directory.
   Waiting for edje patch.
 */
Edje_DeCC *
decompile(const char *edj, const char *edc __UNUSED__)
{
   Edje_DeCC *edjedecc = NULL;
   int size;
   Ecore_Exe_Flags flags  = ECORE_EXE_PIPE_READ |
      ECORE_EXE_PIPE_READ_LINE_BUFFERED |
      ECORE_EXE_PIPE_ERROR |
      ECORE_EXE_PIPE_ERROR_LINE_BUFFERED;

   edjedecc = mem_malloc(sizeof(*edjedecc));
   edjedecc->messages = NULL;

   size = strlen(edj) + BUFF_MAX;
   edjedecc->cmd = (char *)malloc(size);
   sprintf(edjedecc->cmd, "edje_decc %s -no-build-sh", edj);

   ecore_event_handler_add(ECORE_EXE_EVENT_DEL, exe_exit, NULL);
   ecore_event_handler_add(ECORE_EXE_EVENT_DATA, exe_data, edjedecc->messages);
   ecore_event_handler_add(ECORE_EXE_EVENT_ERROR, exe_data, edjedecc->messages);

   edjedecc->exe = ecore_exe_pipe_run(edjedecc->cmd, flags, NULL);
   ecore_main_loop_begin();

   return edjedecc;
}

void
edje_cc_free(struct _Edje_CC *edje_cc)
{
   if (!edje_cc) return;

   free(edje_cc->cmd);
   compiler_message_clear(edje_cc->messages);
   free(edje_cc);
}
