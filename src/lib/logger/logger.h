/* Copyright (C) 2013 Samsung Electronics. Edje Theme Editor.
 * That code is public domain and can be freely used or copied.
 */

#ifndef LOGGER_H
#define LOGGER_H

/**
 * @defgroup Logger Logger
 *
 * The application logger
 */

#ifdef HAVE_CONFIG_H
#include "eflete_config.h"
#endif /* include ewe_config.h */

#include <Eina.h>

extern int _ewe_log_dom;

/**
 * Logs a message with level CRITICAL.
 *
 * @ingroup Logger
 */
#define CRI(fmt, ...) \
   EINA_LOG(_ewe_log_dom, EINA_LOG_LEVEL_CRITICAL, fmt, ## __VA_ARGS__);

/**
 * Logs a message with level ERROR.
 *
 * @ingroup Logger
 */
#define ERR(fmt, ...) \
   EINA_LOG(_ewe_log_dom, EINA_LOG_LEVEL_ERR, fmt, ## __VA_ARGS__);

/**
 * Logs a message with level WARNING.
 *
 * @ingroup Logger
 */
#define WARN(fmt, ...) \
   EINA_LOG(_ewe_log_dom, EINA_LOG_LEVEL_WARN, fmt, ## __VA_ARGS__);

/**
 * Logs a message with level INFO.
 *
 * @ingroup Logger
 */
#define INFO(fmt, ...) \
   EINA_LOG(_ewe_log_dom, EINA_LOG_LEVEL_INFO, fmt, ## __VA_ARGS__);

/**
 * Logs a message with level DEBUG.
 *
 * @ingroup Logger
 */
#define DBG(fmt, ...) \
   EINA_LOG(_ewe_log_dom, EINA_LOG_LEVEL_DBG, fmt, ## __VA_ARGS__);

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
 */
int
logger_shutdown(void);

#endif /* LOGGER_H */
