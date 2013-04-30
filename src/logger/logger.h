#ifndef LOGGER_H
#define LOGGER_H

#include <Eina.h>

#define CRIT(fmt, ...) EINA_LOG_DOM_CRIT(dom_crit_get(), fmt, ## __VA_ARGS__)
#define ERR(fmt, ...) EINA_LOG_DOM_ERR(dom_err_get(), fmt, ## __VA_ARGS__)
#define WARN(fmt, ...) EINA_LOG_DOM_WARN(dom_warn_get(), fmt, ## __VA_ARGS__)
#define INFO(fmt, ...) EINA_LOG_DOM_INFO(dom_info_get(), fmt, ## __VA_ARGS__)
#define DBG(fmt, ...) EINA_LOG_DOM_DBG(dom_dbg_get(), fmt, ## __VA_ARGS__)

#define TET_LOG_DEFAULT_LEVEL EINA_LOG_LEVEL_DBG

/**
 *
 */
Eina_Bool
logger_init(void);

/**
 *
 */
void
logger_shutdown(void);

int
dom_crit_get(void);

int
dom_err_get(void);

int
dom_warn_get(void);

int
dom_info_get(void);

int
dom_dbg_get(void);

#endif /* LOGGER_H */
