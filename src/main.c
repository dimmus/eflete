#include <stdio.h>
#include <stdlib.h>
#include <Eina.h>
#include "ui_main_window.h"

EAPI_MAIN int
elm_main()
{
	Evas_Object *win;

	win = ui_main_window_add();

	elm_run();
	elm_shutdown();

	return 0;
}
ELM_MAIN();
