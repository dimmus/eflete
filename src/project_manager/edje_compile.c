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
   //INFO("Start compile project: %s.", edc);

   edjecc = mem_malloc(sizeof(*edjecc));
   edjecc->messages = NULL;
   size = strlen(edc) + strlen(edj) +
      strlen(image_directory) + strlen(font_directory) +
      strlen(sound_directory) + BUFF_MAX;
   edjecc->cmd = (char *)malloc(size);
   sprintf(edjecc->cmd, "edje_cc -id %s -fd %s -sd %s %s %s",
           image_directory, font_directory, sound_directory,
           edc, edj);
   //INFO("Run command: %s", edjecc->cmd);

   ecore_event_handler_add(ECORE_EXE_EVENT_DEL, exe_exit, NULL);

   ecore_event_handler_add(ECORE_EXE_EVENT_DATA, exe_data, edjecc->messages);
   ecore_event_handler_add(ECORE_EXE_EVENT_ERROR, exe_data, edjecc->messages);

   edjecc->exe = ecore_exe_pipe_run(edjecc->cmd, flags, NULL);
   ecore_main_loop_begin();

   return edjecc;
}

Edje_DeCC *
decompile(char *edj, char *edc)
{
   Edje_DeCC *edjedecc = NULL;
   int size;
   Ecore_Exe_Flags flags  = ECORE_EXE_PIPE_READ |
      ECORE_EXE_PIPE_READ_LINE_BUFFERED |
      ECORE_EXE_PIPE_ERROR |
      ECORE_EXE_PIPE_ERROR_LINE_BUFFERED;

   edjedecc = mem_malloc(sizeof(*edjedecc));
   edjedecc->messages = NULL;
   if (!edc)
     {
        size = strlen(edj) + BUFF_MAX;
        edjedecc->cmd = (char *)malloc(size);
        sprintf(edjedecc->cmd, "edje_decc %s -no-build-sh -current-dir", edj);
     }
   else
     {
        size = strlen(edj) + strlen(edc) + BUFF_MAX;
        edjedecc->cmd = (char *)malloc(size);
        sprintf(edjedecc->cmd,
                "edje_decc %s -main-out %s -no-build-sh -current-dir",
                edj, edc);
     }

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

   //if (edje_cc->exe) ecore_exe_free(edje_cc->exe);
   free(edje_cc->cmd);
   compiler_message_clear(edje_cc->messages);
   free(edje_cc);
}
