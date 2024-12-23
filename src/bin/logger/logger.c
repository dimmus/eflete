#include "eflete.h"

int _eflete_lod_dom = -1;

Eina_Bool
logger_init(void)
{
   if (!eina_init()) return false;

#ifdef HAVE_EFLETE_DEBUG
   eina_log_level_set(EINA_LOG_LEVEL_DBG);
#else
   eina_log_level_set(EINA_LOG_LEVEL_ERR);
#endif

   if(_eflete_lod_dom < 0)
     {
        _eflete_lod_dom = eina_log_domain_register(PACKAGE_NAME, EINA_COLOR_LIGHTBLUE);
        if(_eflete_lod_dom < 0)
          {
             EINA_LOG_CRIT("Could not register log domain "PACKAGE_NAME);
             return false;
          }
     }
   return true;
}

int
logger_shutdown(void)
{
   if(_eflete_lod_dom >= 0)
     {
        eina_log_domain_unregister(_eflete_lod_dom);
        _eflete_lod_dom = -1;
     }
   return _eflete_lod_dom;
}
