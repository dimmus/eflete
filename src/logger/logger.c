#include <stdio.h>
#include <time.h>
#include "logger.h"

static FILE *log_file;
static char *log_file_name;

char *
log_file_name_get(void)
{
	char *log_file_name;
	char prefix[] = "tet_log_";
	time_t timer;

	time(&timer);
	log_file_name = calloc(1, sizeof(prefix) + 16);
	sprintf(log_file_name, "%s%lld", prefix, (long long)timer);

	return log_file_name;
}

void
tet_log_cb(const Eina_Log_Domain *domain,
				Eina_Log_Level level,
				const char *file,
				const char *fnc,
				int line,
				const char *fmt,
				void *data,
				va_list args)
{
	FILE *output;

	if(data)
	{
		output = data;
		fprintf(output, "%s:%s:%s (%d): ",
				domain->name, file, fnc, line);
		vfprintf(output, fmt, args);
		putc('\n', output);
	}

	if(level > EINA_LOG_LEVEL_ERR)
		output = stdout;
	else
		output = stderr;

	fprintf(output, "%s:%s:%s (%d): ",
			domain->domain_str, file, fnc, line);
	vfprintf(output, fmt, args);
	putc('\n', output);
}

Eina_Bool
logger_init(void)
{

	if(!eina_init())
		return EINA_FALSE;

	if(_tet_log_dom_crit < 0)
	{
		_tet_log_dom_crit = eina_log_domain_register("TET CRIT", EINA_COLOR_RED);
		if(_tet_log_dom_crit < 0)
		{
			EINA_LOG_CRIT("Could not register log domain 'TET CRIT'");
			return EINA_FALSE;
		}
	}
	if(_tet_log_dom_err < 0)
	{
		_tet_log_dom_err = eina_log_domain_register("TET ERR", EINA_COLOR_LIGHTRED);
		if(_tet_log_dom_err < 0)
		{
			EINA_LOG_CRIT("Could not register log domain 'TET ERR'");
			return EINA_FALSE;
		}
	}
	if(_tet_log_dom_warn < 0)
	{
		_tet_log_dom_warn = eina_log_domain_register("TET WARN", EINA_COLOR_YELLOW);
		if(_tet_log_dom_warn < 0)
		{
			EINA_LOG_CRIT("Could not register log domain 'TET WARN'");
			return EINA_FALSE;
		}
	}
	if(_tet_log_dom_info < 0)
	{
		_tet_log_dom_info = eina_log_domain_register("TET INFO", EINA_COLOR_GREEN);
		if(_tet_log_dom_info < 0)
		{
			EINA_LOG_CRIT("Could not register log domain 'TET INFO'");
			return EINA_FALSE;
		}
	}
	if(_tet_log_dom_dbg < 0)
	{
		_tet_log_dom_dbg = eina_log_domain_register("TET DBG", EINA_COLOR_LIGHTBLUE);
		if(_tet_log_dom_dbg < 0)
		{
			EINA_LOG_CRIT("Could not register log domain 'TET DBG'");
			return EINA_FALSE;
		}
	}

	log_file_name = log_file_name_get();
	log_file = fopen(log_file_name, "a+");
	if(!log_file)
		EINA_LOG_CRIT("Could not create log file %s", log_file_name);

	eina_log_level_set(TET_LOG_DEFAULT_LEVEL);
	eina_log_print_cb_set(tet_log_cb, log_file);

	return EINA_TRUE;
}

void
logger_shutdown(void)
{
	eina_log_print_cb_set(NULL, NULL);
	fclose(log_file);
	free(log_file_name);

	if(_tet_log_dom_crit >= 0)
	{
		eina_log_domain_unregister(_tet_log_dom_crit);
		_tet_log_dom_crit = -1;
	}
	if(_tet_log_dom_err  >= 0)
	{
		eina_log_domain_unregister(_tet_log_dom_err);
		_tet_log_dom_err = -1;
	}
	if(_tet_log_dom_warn >= 0)
	{
		eina_log_domain_unregister(_tet_log_dom_warn);
		_tet_log_dom_warn = -1;
	}
	if(_tet_log_dom_info >= 0)
	{
		eina_log_domain_unregister(_tet_log_dom_info);
		_tet_log_dom_info = -1;
	}
	if(_tet_log_dom_dbg  >= 0)
	{
		eina_log_domain_unregister(_tet_log_dom_dbg);
		_tet_log_dom_dbg = -1;
	}
}
