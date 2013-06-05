#ifndef LOGGER_H
#define LOGGER_H

/**
 * @defgroup Logger Logger
 *
 * The application logger
 */

#include <Eina.h>

extern int _tet_log_dom;

/**
 * @def TET_LOG(DOM, LEVEL, fmt, ...))
 *
 * Logs a message on the specified domain, level and format.
 *
 * @ingroup Logger
 */
#define TET_LOG(DOM, LEVEL, fmt, ...) EINA_LOG(DOM, LEVEL, fmt, ## __VA_ARGS__)

/**
 * @def Logs a message with level CRITICAL.
 *
 * @ingroup Logger
 */
#define CRIT(fmt, ...) \
   TET_LOG(_tet_log_dom, EINA_LOG_LEVEL_CRITICAL, fmt, ## __VA_ARGS__)

/**
 * @def Logs a message with level ERROR.
 *
 * @ingroup Logger
 */
#define ERR(fmt, ...) \
   TET_LOG(_tet_log_dom, EINA_LOG_LEVEL_ERR, fmt, ## __VA_ARGS__)

/**
 * Logs a message with level WARNING.
 *
 * @ingroup Logger
 */
#define WARN(fmt, ...) \
   TET_LOG(_tet_log_dom, EINA_LOG_LEVEL_WARN, fmt, ## __VA_ARGS__)

/**
 * Logs a message with level INFO.
 *
 * @ingroup Logger
 */
#define INFO(fmt, ...) \
   TET_LOG(_tet_log_dom, EINA_LOG_LEVEL_INFO, fmt, ## __VA_ARGS__)

/**
 * Logs a message with level DEBUG.
 *
 * @ingroup Logger
 */
#define DBG(fmt, ...) \
   TET_LOG(_tet_log_dom, EINA_LOG_LEVEL_DBG, fmt, ## __VA_ARGS__)

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

#endif /* LOGGER_H */
