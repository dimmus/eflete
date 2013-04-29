#include <ui.h>

UI_Data *
ui_create (void)
{
	UI_Data *_ui = calloc (1, sizeof (UI_Data));
	_ui->win = NULL;
	_ui->win_layout = NULL;
	_ui->block_left_top = NULL;
	_ui->block_left_bottom = NULL;
	_ui->block_bottom_left = NULL;
	_ui->block_bottom_right = NULL;
	_ui->block_right_top = NULL;
	_ui->block_right_bottom = NULL;
	_ui->main_menu = NULL;
	_ui->block_canvas = NULL;

	return _ui;
}
