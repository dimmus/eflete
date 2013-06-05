#include "logger.h"

int _tet_log_dom = -1;

Eina_Bool
logger_init(void)
{
   if (!eina_init())
     {
        return EINA_FALSE;
     }

   /*  */
   eina_log_level_set(EINA_LOG_LEVEL_DBG);

   if(_tet_log_dom < 0)
     {
        _tet_log_dom = eina_log_domain_register("Theme Editor Tool",
                                                EINA_COLOR_LIGHTBLUE);
        if(_tet_log_dom < 0)
          {
             EINA_LOG_CRIT("Could not register log domain 'TET'");
             return EINA_FALSE;
          }
     }

   return EINA_TRUE;
}

void
logger_shutdown(void)
{
   if(_tet_log_dom >= 0)
     {
        eina_log_domain_unregister(_tet_log_dom);
        _tet_log_dom = -1;
     }
}
