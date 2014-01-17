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

#ifndef LOGGER_H
#define LOGGER_H

/**
 * @defgroup Logger Logger
 *
 * The application logger
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* include config.h */

#include <Eina.h>

extern int _eflete_lod_dom;

/**
 * Logs a message on the specified domain, level and format.
 *
 * @ingroup Logger
 */
#define EFLETE_LOG(DOM, LEVEL, fmt, ...) \
  { \
    if (HAVE_EFLETE_DEBUG) EINA_LOG(DOM, LEVEL, fmt, ## __VA_ARGS__); \
    else logger_message_print(LEVEL, fmt, ## __VA_ARGS__); \
  }

/**
 * Logs a message with level CRITICAL.
 *
 * @ingroup Logger
 */
#define CRIT(fmt, ...) \
   EFLETE_LOG(_eflete_lod_dom, EINA_LOG_LEVEL_CRITICAL, fmt, ## __VA_ARGS__)

/**
 * Logs a message with level ERROR.
 *
 * @ingroup Logger
 */
#define ERR(fmt, ...) \
   EFLETE_LOG(_eflete_lod_dom, EINA_LOG_LEVEL_ERR, fmt, ## __VA_ARGS__)

/**
 * Logs a message with level WARNING.
 *
 * @ingroup Logger
 */
#define WARN(fmt, ...) \
   EFLETE_LOG(_eflete_lod_dom, EINA_LOG_LEVEL_WARN, fmt, ## __VA_ARGS__)

/**
 * Logs a message with level INFO.
 *
 * @ingroup Logger
 */
#define INFO(fmt, ...) \
   EFLETE_LOG(_eflete_lod_dom, EINA_LOG_LEVEL_INFO, fmt, ## __VA_ARGS__)

/**
 * Logs a message with level DEBUG.
 *
 * @ingroup Logger
 */
#define DBG(fmt, ...) \
   EFLETE_LOG(_eflete_lod_dom, EINA_LOG_LEVEL_DBG, fmt, ## __VA_ARGS__)

/**
 * Init logger
 *
 * @ingroup Logger
 */
Eina_Bool
logger_init(void);

/**
 * Shutdown logger
 *
 * @ingroup Logger
 */
void
logger_shutdown(void);

/**
 * Print the log message to strerr. Used only in case when debug is disable.
 *
 * @ingroup Logger
 */
void
logger_message_print(Eina_Log_Level level, const char *fmt, ...);

#endif /* LOGGER_H */
