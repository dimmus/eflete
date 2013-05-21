#include "edje_compile.h"

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
	int i = 0;

	ev = event;
	messages = (Eina_Inlist *)data;

	if(ev->lines)
	{
		for(i = 0; ev->lines[i].line; i++)
		{
			message = calloc(1, sizeof(*message));
			message->time = time(NULL);
			message->text = strdup(ev->lines[i].line);
			messages = eina_inlist_append(messages, EINA_INLIST_GET(message));
			//fprintf(stdout, "%lld\t%s\n", (long long)message->time,
			//				message->text);
		}
	}

	return ECORE_CALLBACK_PASS_ON;
}

Edje_CC *
compile(const char *edc,
		const char *edj,
		const char *image_directory,
		const char *font_directory,
		const char *sound_directory)
{
	Edje_CC *edjecc = NULL;
	Ecore_Exe_Flags flags  = ECORE_EXE_PIPE_READ |
							ECORE_EXE_PIPE_READ_LINE_BUFFERED |
							ECORE_EXE_PIPE_ERROR |
							ECORE_EXE_PIPE_ERROR_LINE_BUFFERED;
	Compiler_Message *cmes = NULL;
	char compile_mess[] = "Start compile project: %s";

	edjecc = calloc(1, sizeof(*edjecc));
	sprintf(edjecc->cmd, "edje_cc -id=%s -fd=%s -sd=%s %s %s",
			image_directory, font_directory, sound_directory,
			edc, edj);
	cmes = calloc(1, sizeof(*cmes));
	cmes->time = time(NULL);
	cmes->text = malloc(strlen(compile_mess) + strlen(edc) + 32);
	sprintf(cmes->text, compile_mess, edc);
	edjecc->messages = eina_inlist_append(NULL, EINA_INLIST_GET(cmes));

	ecore_event_handler_add(ECORE_EXE_EVENT_DATA, exe_data, edjecc->messages);
	ecore_event_handler_add(ECORE_EXE_EVENT_ERROR, exe_data, edjecc->messages);

	edjecc->exe = ecore_exe_pipe_run(edjecc->cmd, flags, NULL);

	return edjecc;
}

Edje_DeCC *
decompile(char *edj, char *edc)
{
	Edje_DeCC *edjedecc = NULL;
	Ecore_Exe_Flags flags  = ECORE_EXE_PIPE_READ |
							ECORE_EXE_PIPE_READ_LINE_BUFFERED |
							ECORE_EXE_PIPE_ERROR |
							ECORE_EXE_PIPE_ERROR_LINE_BUFFERED;
	Compiler_Message *cmes = NULL;
	char decompile_mess[] = "Start decompile project: %s";

	edjedecc = calloc(1, sizeof(*edjedecc));
	if(!edc)
		sprintf(edjedecc->cmd, "edje_decc %s -no-build-sh -current-dir", edj);
	else
		sprintf(edjedecc->cmd, "edje_decc %s -main-out %s -no-build-sh -current-dir",
				edj, edc);
	cmes = calloc(1, sizeof(*cmes));
	cmes->time = time(NULL);
	cmes->text = malloc(strlen(decompile_mess) + strlen(edj) + 32);
	sprintf(cmes->text, decompile_mess, edj);
	edjedecc->messages = eina_inlist_append(NULL, EINA_INLIST_GET(cmes));

	ecore_event_handler_add(ECORE_EXE_EVENT_DATA, exe_data, edjedecc->messages);
	ecore_event_handler_add(ECORE_EXE_EVENT_ERROR, exe_data, edjedecc->messages);

	edjedecc->exe = ecore_exe_pipe_run(edjedecc->cmd, flags, NULL);

	return edjedecc;
}

void
compiler_free(Edje_CC *compiler)
{
	if(!compiler)
		return;

	ecore_exe_free(compiler->exe);
	free(compiler->cmd);
	compiler_message_clear(compiler->messages);
	free(compiler);
}

void
decompiler_free(Edje_DeCC *decompiler)
{
	if(!decompiler)
		return;

	ecore_exe_free(decompiler->exe);
	free(decompiler->cmd);
	compiler_message_clear(decompiler->messages);
	free(decompiler);
}
