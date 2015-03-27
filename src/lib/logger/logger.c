/* Copyright (C) 2013 Samsung Electronics. Edje Theme Editor.
 * That code is public domain and can be freely used or copied.
 */

#include "logger.h"

int _ewe_log_dom = -1;

Eina_Bool
logger_init(void)
{
   if (!eina_init()) return EINA_FALSE;

#ifdef HAVE_EWE_DEBUG
   eina_log_level_set(EINA_LOG_LEVEL_DBG);
#else
   eina_log_level_set(EINA_LOG_LEVEL_ERR);
#endif


   if(_ewe_log_dom < 0)
     {
        _ewe_log_dom = eina_log_domain_register("ewe", EINA_COLOR_CYAN);
        if(_ewe_log_dom < 0)
          {
             EINA_LOG_CRIT("Could not register log domain ewe");
             return EINA_FALSE;
          }
     }

   return EINA_TRUE;
}

int
logger_shutdown(void)
{
   if(_ewe_log_dom >= 0)
     {
        eina_log_domain_unregister(_ewe_log_dom);
        _ewe_log_dom = -1;
     }
   return _ewe_log_dom;
}
