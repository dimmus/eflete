/*
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
 *
 * This file is part of Edje Theme Editor.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; If not, see www.gnu.org/licenses/lgpl.html.
 */

#ifndef LOGGER_H
#define LOGGER_H

/**
 * @defgroup Logger Logger
 * @ingroup Eflete
 *
 * The application logger.
 */

extern int _eflete_lod_dom;

/**
 * Logs a message with level CRITICAL.
 *
 * @ingroup Logger
 */
#define CRIT(fmt, ...) \
   EINA_LOG(_eflete_lod_dom, EINA_LOG_LEVEL_CRITICAL, fmt, ## __VA_ARGS__);

/**
 * Logs a message with level ERROR.
 *
 * @ingroup Logger
 */
#define ERR(fmt, ...) \
   EINA_LOG(_eflete_lod_dom, EINA_LOG_LEVEL_ERR, fmt, ## __VA_ARGS__);

/**
 * Logs a message with level WARNING.
 *
 * @ingroup Logger
 */
#define WARN(fmt, ...) \
   EINA_LOG(_eflete_lod_dom, EINA_LOG_LEVEL_WARN, fmt, ## __VA_ARGS__);

/**
 * Logs a message with level INFO.
 *
 * @ingroup Logger
 */
#define INFO(fmt, ...) \
   EINA_LOG(_eflete_lod_dom, EINA_LOG_LEVEL_INFO, fmt, ## __VA_ARGS__);

/**
 * Logs a message with level DEBUG.
 *
 * @ingroup Logger
 */
#define DBG(fmt, ...) \
   EINA_LOG(_eflete_lod_dom, EINA_LOG_LEVEL_DBG, fmt, ## __VA_ARGS__);

/**
 * Initialize logger
 *
 * @return EINA_TRUE, on success or EINA_FALSE, on errors.
 *
 * @ingroup Logger
 */
Eina_Bool
logger_init(void);

/**
 * Shutdown logger
 *
 * @return -1 when module are completely shutdown, 0 or greater otherwise.
 *
 * @ingroup Logger
 *
 */
int
logger_shutdown(void);

#endif /* LOGGER_H */
