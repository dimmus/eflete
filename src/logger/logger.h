#ifndef LOGGER_H
#define LOGGER_H

#include <Eina.h>

static int _tet_log_dom_crit = -1;
static int _tet_log_dom_err  = -1;
static int _tet_log_dom_warn = -1;
static int _tet_log_dom_info = -1;
static int _tet_log_dom_dbg  = -1;

#define CRI(...) EINA_LOG_DOM_CRIT(_tet_log_dom_crit, __VA_ARGS__)
#define ERR(...) EINA_LOG_DOM_ERR (_tet_log_dom_err,  __VA_ARGS__)
#define WAR(...) EINA_LOG_DOM_WARN(_tet_log_dom_warn, __VA_ARGS__)
#define INF(...) EINA_LOG_DOM_INFO(_tet_log_dom_info, __VA_ARGS__)
#define DBG(...) EINA_LOG_DOM_DBG (_tet_log_dom_dbg,  __VA_ARGS__)

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

#endif /* LOGGER_H */
