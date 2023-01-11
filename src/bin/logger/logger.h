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
