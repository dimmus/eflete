#include <ui_main_window.h>
#include <ui_settings.h>

#define SETTINGSFILE "panes.set"

struct _UI_Settings
{
	double left_panes_left_size;
	double left_panes_right_size;
	short int left_panes_fixed;

	double left_hor_panes_left_size;
	double left_hor_panes_right_size;
	short int left_hor_panes_fixed;

	double right_panes_left_size;
	double right_panes_right_size;
	short int right_panes_fixed;

	double center_panes_left_size;
	double center_panes_right_size;
	short int center_panes_fixed;

	double center_down_panes_left_size;
	double center_down_panes_right_size;
	short int center_down_panes_fixed;

	double right_hor_panes_left_size;
	double right_hor_panes_right_size;
	short int right_hor_panes_fixed;
};

typedef struct _UI_Settings UI_Settings;

static Eet_Data_Descriptor *_panes_settings_descriptor;

static void
_panes_settings_descriptor_init (void)
{
	Eet_Data_Descriptor_Class eddc;

	EET_EINA_STREAM_DATA_DESCRIPTOR_CLASS_SET (&eddc, UI_Settings);
	_panes_settings_descriptor = eet_data_descriptor_stream_new (&eddc);

	#define MY_CONF_ADD_BASIC(member, eet_type) 	\
  		EET_DATA_DESCRIPTOR_ADD_BASIC				\
    		(_panes_settings_descriptor, UI_Settings,# member, member, eet_type)
	
		MY_CONF_ADD_BASIC(left_panes_left_size, EET_T_DOUBLE);
		MY_CONF_ADD_BASIC(left_panes_right_size, EET_T_DOUBLE);
		MY_CONF_ADD_BASIC(left_panes_fixed, EET_T_SHORT);

		MY_CONF_ADD_BASIC(left_hor_panes_left_size, EET_T_DOUBLE);
		MY_CONF_ADD_BASIC(left_hor_panes_right_size, EET_T_DOUBLE);
		MY_CONF_ADD_BASIC(left_hor_panes_fixed, EET_T_SHORT);

		MY_CONF_ADD_BASIC(right_panes_left_size, EET_T_DOUBLE);
		MY_CONF_ADD_BASIC(right_panes_right_size, EET_T_DOUBLE);
		MY_CONF_ADD_BASIC(right_panes_fixed, EET_T_SHORT);

		MY_CONF_ADD_BASIC(center_panes_left_size, EET_T_DOUBLE);
		MY_CONF_ADD_BASIC(center_panes_right_size, EET_T_DOUBLE);
		MY_CONF_ADD_BASIC(center_panes_fixed, EET_T_SHORT);

		MY_CONF_ADD_BASIC(center_down_panes_left_size, EET_T_DOUBLE);
		MY_CONF_ADD_BASIC(center_down_panes_right_size, EET_T_DOUBLE);
		MY_CONF_ADD_BASIC(center_down_panes_fixed, EET_T_SHORT);

		MY_CONF_ADD_BASIC(right_hor_panes_left_size, EET_T_DOUBLE);
		MY_CONF_ADD_BASIC(right_hor_panes_right_size, EET_T_DOUBLE);
		MY_CONF_ADD_BASIC(right_hor_panes_fixed, EET_T_SHORT);

	#undef MY_CONF_ADD_BASIC

}

static void
_panes_settings_descriptor_shutdown (void)
{
	eet_data_descriptor_free (_panes_settings_descriptor);
}

static UI_Settings *
_from_list_to_ui_settings (Eina_List *ui_list_panes)
{
	Eina_List *l = ui_list_panes;
	UI_Settings *panes_settings;
	
	panes_settings->left_panes_left_size = elm_panes_content_left_size_get (
		(Evas_Object *)l->data);
	panes_settings->left_panes_right_size = elm_panes_content_right_size_get (
		(Evas_Object *)l->data);
	panes_settings->left_panes_fixed = (short) elm_panes_fixed_get (
		(Evas_Object *)l->data);

	l = eina_list_next (l);
	panes_settings->left_hor_panes_left_size = elm_panes_content_left_size_get (
		(Evas_Object *)l->data);
	panes_settings->left_hor_panes_right_size = elm_panes_content_right_size_get (
		(Evas_Object *)l->data);
	panes_settings->left_hor_panes_fixed = (short) elm_panes_fixed_get (
		(Evas_Object *)l->data);

	l = eina_list_next (l);
	panes_settings->right_panes_left_size = elm_panes_content_left_size_get (
		(Evas_Object *)l->data);
	panes_settings->right_panes_right_size = elm_panes_content_right_size_get (
		(Evas_Object *)l->data);
	panes_settings->right_panes_fixed = (short) elm_panes_fixed_get (
		(Evas_Object *)l->data);

	l = eina_list_next (l);
	panes_settings->center_panes_left_size = elm_panes_content_left_size_get (
		(Evas_Object *)l->data);
	panes_settings->center_panes_right_size = elm_panes_content_right_size_get (
		(Evas_Object *)l->data);
	panes_settings->center_panes_fixed = (short) elm_panes_fixed_get (
		(Evas_Object *)l->data);

	l = eina_list_next (l);
	panes_settings->center_down_panes_left_size = elm_panes_content_left_size_get (
		(Evas_Object *)l->data);
	panes_settings->center_down_panes_right_size = elm_panes_content_right_size_get (
		(Evas_Object *)l->data);
	panes_settings->center_down_panes_fixed = (short) elm_panes_fixed_get (
		(Evas_Object *)l->data);

	l = eina_list_next (l);
	panes_settings->right_hor_panes_left_size = elm_panes_content_left_size_get (
		(Evas_Object *)l->data);
	panes_settings->right_hor_panes_right_size = elm_panes_content_right_size_get (
		(Evas_Object *)l->data);
	panes_settings->right_hor_panes_fixed = (short) elm_panes_fixed_get (
		(Evas_Object *)l->data);

	eina_list_free (l);

	return panes_settings;
}

void
_from_ui_settings_to_list (UI_Settings *panes_settings, Eina_List *ui_list_panes)
{
	Eina_List *l = ui_list_panes;

	elm_panes_content_left_size_set (
		(Evas_Object *)l->data,	panes_settings->left_panes_left_size);
	elm_panes_content_right_size_set (
		(Evas_Object *)l->data,	panes_settings->left_panes_right_size);
	elm_panes_fixed_set (
		(Evas_Object *)l->data, (Eina_Bool)	panes_settings->left_panes_fixed) ;

	l = eina_list_next (l);
	elm_panes_content_left_size_set (
		(Evas_Object *)l->data,	panes_settings->left_hor_panes_left_size);
	elm_panes_content_right_size_set (
		(Evas_Object *)l->data,	panes_settings->left_hor_panes_right_size);
	elm_panes_fixed_set (
		(Evas_Object *)l->data, (Eina_Bool)	panes_settings->left_hor_panes_fixed) ;

	l = eina_list_next (l);
	elm_panes_content_left_size_set (
		(Evas_Object *)l->data,	panes_settings->right_panes_left_size);
	elm_panes_content_right_size_set (
		(Evas_Object *)l->data,	panes_settings->right_panes_right_size);
	elm_panes_fixed_set (
		(Evas_Object *)l->data, (Eina_Bool)	panes_settings->right_panes_fixed) ;

	l = eina_list_next (l);
	elm_panes_content_left_size_set (
		(Evas_Object *)l->data,	panes_settings->center_panes_left_size);
	elm_panes_content_right_size_set (
		(Evas_Object *)l->data,	panes_settings->center_panes_right_size);
	elm_panes_fixed_set (
		(Evas_Object *)l->data, (Eina_Bool)	panes_settings->center_panes_fixed) ;

	l = eina_list_next (l);
	elm_panes_content_left_size_set (
		(Evas_Object *)l->data,	panes_settings->center_down_panes_left_size);
	elm_panes_content_right_size_set (
		(Evas_Object *)l->data,	panes_settings->center_down_panes_right_size);
	elm_panes_fixed_set (
		(Evas_Object *)l->data, (Eina_Bool)	panes_settings->center_down_panes_fixed) ;

	l = eina_list_next (l);
	elm_panes_content_left_size_set (
		(Evas_Object *)l->data,	panes_settings->right_hor_panes_left_size);
	elm_panes_content_right_size_set (
		(Evas_Object *)l->data,	panes_settings->right_hor_panes_right_size);
	elm_panes_fixed_set (
		(Evas_Object *)l->data, (Eina_Bool)	panes_settings->right_hor_panes_fixed) ;

	eina_list_free (l);

}

Eina_Bool
_panes_settings_save (Eina_List *ui_list_panes)
{
	Eina_Bool state_errors = EINA_FALSE;
	Eet_File *file_settings;	
	UI_Settings *panes_settings;

	_panes_settings_descriptor_init ();

	panes_settings = _from_list_to_ui_settings (ui_list_panes);


	file_settings = eet_open (SETTINGSFILE, EET_FILE_MODE_WRITE);
	if (!file_settings) {
		fprintf (stdout, "Failrue open configs file for write panes settings");
		state_errors = EINA_TRUE;
	}

	eet_data_write (file_settings, _panes_settings_descriptor, "settings", 
		panes_settings, EINA_TRUE);
	eet_close (file_settings);

	_panes_settings_descriptor_shutdown ();

	return ~state_errors;
}

Eina_Bool
_panes_settings_load (Eina_List *ui_list_panes)
{
	Eina_Bool state_errors = EINA_FALSE;
	UI_Settings *panes_settings;
	Eet_File *file_settings;

	_panes_settings_descriptor_init ();

	file_settings = eet_open (SETTINGSFILE, EET_FILE_MODE_READ);
	if (!file_settings) {
		fprintf (stdout, "Failrue open configs file for load panes settings");
		state_errors = EINA_TRUE;
	}

	panes_settings = eet_data_read (file_settings, _panes_settings_descriptor, "settings");
	if (!panes_settings) {
		fprintf (stdout, "Failrue load panes setings.");
		state_errors = EINA_TRUE;
	}
	_from_ui_settings_to_list (panes_settings, ui_list_panes);

	_panes_settings_descriptor_shutdown ();
	eet_close (file_settings);

	return ~state_errors;
}
