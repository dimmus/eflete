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

#ifndef UI_NOTIFY_HEADER_H
#define UI_NOTIFY_HEADER_H

/**
 * @defgroup Notify Notify
 *
 * Notify API for showing some user-oriented information
 * like warning or error's.
 */

#include "efl_ete.h"
#include "widget_macro.h"

/**
 * Show Error Notification with formated message
 *
 * @ingroup Notify
 */
#define NOTIFY_ERROR(fmt, ...) \
{ \
	char *msg = mem_malloc(BUFF_MAX * sizeof(char)); \
	snprintf(msg, BUFF_MAX, fmt, ##__VA_ARGS__); \
	noti_error_show(win_layout_get(), msg); \
	free(msg); \
}

/**
 * Show Warning Notification with formated message
 *
 * @ingroup Notify
 */
#define NOTIFY_WARNING(fmt, ...) \
{ \
	char *msg = mem_malloc(BUFF_MAX * sizeof(char)); \
	snprintf(msg, BUFF_MAX, fmt, ##__VA_ARGS__); \
	noti_warning_show(win_layout_get(), msg); \
	free(msg); \
}

/**
 * Show Information Notification with formated message
 *
 * @ingroup Notify
 */
#define NOTIFY_INFO(time, fmt, ...) \
{ \
	char *msg = mem_malloc(BUFF_MAX * sizeof(char)); \
	snprintf(msg, BUFF_MAX, fmt, ##__VA_ARGS__); \
	noti_info_show(win_layout_get(), msg, time); \
	free(msg); \
}

/**
 * Show notification with some 'error' related message.
 *
 * @param obj object, on which notify will be appeared.
 * @param message error message.
 *
 * @ingroup Notify
 */
void
noti_error_show(Evas_Object *obj, const char *message);

/**
 * Show notification with 'warning' related message.
 *
 * @param obj object, on which notify will be appeared.
 * @param message warning message.
 *
 * @ingroup Notify
 */
void
noti_warning_show(Evas_Object *obj, const char *message);

/**
 * Show notification with some information.
 *
 * @param obj object, on which notify will be appeared.
 * @param message information message.
 * @param time the information message will be shown until given time,
 * and after that time it will disappear.
 *
 * @ingroup Notify
 */
void
noti_info_show(Evas_Object *obj, const char *message, double time);

#endif /* UI_NOTIFY_HEADER_H */
