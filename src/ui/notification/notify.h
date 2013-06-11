#ifndef UI_NOTIFY_HEADER_H
#define UI_NOTIFY_HEADER_H

/**
 * @defgroup Notify Notify
 *
 * Notify API for showing some user-oriented information
 * like warning or error's.
 */

#include "efl_tet.h"


#define BUFFSIZE 1024

/**
 * @def NOTIFY_ERROR(obj, fmt, args ...)
 *
 * Show Error Notification with formated message
 *
 * @ingroup Notify
 */
#define NOTIFY_ERROR(obj, fmt, ...) \
{ \
	char *msg = calloc(BUFFSIZE, sizeof(char)); \
	snprintf(msg, BUFFSIZE, fmt, ##__VA_ARGS__); \
	noti_error_show(obj, msg); \
	free(msg); \
}

/**
 * @def NOTIFY_WARNING(obj, fmt, args ...)
 *
 * Show Warning Notification with formated message
 *
 * @ingroup Notify
 */
#define NOTIFY_WARNING(obj, fmt, ...) \
{ \
	char *msg = calloc(BUFFSIZE, sizeof(char)); \
	snprintf(msg, BUFFSIZE, fmt, ##__VA_ARGS__); \
	noti_warning_show(obj, msg); \
	free(msg); \
}

/**
 * @def NOTIFY_INFO(obj, fmt, args ...)
 *
 * Show Information Notification with formated message
 *
 * @ingroup Notify
 */
#define NOTIFY_INFO(obj, fmt, time, ...) \
{ \
	char *msg = calloc(BUFFSIZE, sizeof(char)); \
	snprintf(msg, BUFFSIZE, fmt, ##__VA_ARGS__); \
	noti_info_show(obj, msg, time); \
	free(msg); \
}

/**
 * Show notification with some 'error' related message.
 *
 * @param obj object, on which notify will be appeared.
 * @param message error message.
 *
 * @ingroup Notify
 */
void
noti_error_show(Evas_Object *obj, const char *message);

/**
 * Show notification with 'warning' related message.
 *
 * @param obj object, on which notify will be appeared.
 * @param message warning message.
 *
 * @ingroup Notify
 */
void
noti_warning_show(Evas_Object *obj, const char *message);

/**
 * Show notification with some information.
 *
 * @param obj object, on which notify will be appeared.
 * @param message information message.
 * @param time the information message will be shown until given time,
 * and after that time it will disappear.
 *
 * @ingroup Notify
 */
void
noti_info_show(Evas_Object *obj, const char *message, double time);

#endif /* UI_NOTIFY_HEADER_H */
