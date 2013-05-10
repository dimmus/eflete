#include "efl_tet.h"
#include "ui_main_window.h"

App_Data *
app_create (void)
{
	App_Data *_ap = calloc (1, sizeof (App_Data));
	_ap->win = NULL;
	_ap->win_layout = NULL;
	_ap->block_left_top = NULL;
	_ap->block_left_bottom = NULL;
	_ap->block_bottom_left = NULL;
	_ap->block_bottom_right = NULL;
	_ap->block_right_top = NULL;
	_ap->block_right_bottom = NULL;
	_ap->main_menu = NULL;
	_ap->block_canvas = NULL;

	return _ap;
}

EAPI_MAIN int
elm_main()
{
	eina_init();
	efreet_init();
	if(!logger_init())
		EINA_LOG_CRIT("Could not start logger!");
	#ifdef HAVE_CONFIG_H
		INFO("%s: %s - Started...", PACKAGE_NAME, VERSION);
	#else
		CRIT("Could not find 'config.h'");
	#endif

	App_Data *ap =  NULL;

	ap = app_create();
	ui_main_window_add(ap);

	elm_run();
	elm_shutdown();

	return 0;
}
ELM_MAIN();
