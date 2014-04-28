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

#include "utc_common.h"
#include "project_manager.h"
#include "eflete.h"

/**
 * @defgroup test_project_manager
 */

void pm_open_project_edj_test_p(int);
void pm_open_project_edj_test_n1(int);
void pm_open_project_edj_test_n2(int);

void pm_project_close_test_p1(int);
void pm_project_close_test_p2(int);
void pm_project_close_test_n(int);

void pm_save_project_edj_test_p(int);
void pm_save_project_edj_test_n(int);

void pm_save_project_to_swap_test_p1(int);
void pm_save_project_to_swap_test_p2(int);
void pm_save_project_to_swap_test_n(int);

void pm_export_to_edc_test_p1(int);
void pm_export_to_edc_test_p2(int);
void pm_export_to_edc_test_n(int);
