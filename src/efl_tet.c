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
	_ap->ws = NULL;
	return _ap;
}

EAPI_MAIN int
elm_main()
{

	if (!efl_tet_init())
		return -1;
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
	efl_tet_shutdown();
	return 0;
}

Eina_Bool
efl_tet_init ()
{
	if (!eina_init())
	{
		CRIT("Cannot initialize the Eina library");
		return EINA_FALSE;
	}

	if (!efreet_init())
	{
		CRIT("Cannot initialize the Efreet system");
		return EINA_FALSE;
	}

	if (!ecore_init())
	{
		CRIT("Cannot initialize the Ecore library");
		return EINA_FALSE;
	}

	if (!edje_init())
	{
		CRIT("Cannot initialize the Edje Library");
		return EINA_FALSE;
	}

	if (!logger_init())
	{
		CRIT("Cannot initialize the logger library");
		return EINA_FALSE;
	}

	if (!ecore_evas_init())
	{
		CRIT("Cannot initialize the Ecore_Evas system");
		return EINA_FALSE;
	}
	return EINA_TRUE;
}

void
efl_tet_shutdown ()
{
	eina_shutdown();
	efreet_shutdown();
	ecore_shutdown();
	edje_shutdown();
	logger_shutdown();
	ecore_evas_shutdown();
}

ELM_MAIN();
