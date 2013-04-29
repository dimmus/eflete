#include <stdio.h>
#include <stdlib.h>
#include <Eina.h>
#include "ui_main_window.h"
#include <ui.h>

EAPI_MAIN int
elm_main()
{
	UI_Data *ud =  NULL;

	ud = ui_create();
	ui_main_window_add(ud);

	elm_run();
	elm_shutdown();

	return 0;
}
ELM_MAIN();
