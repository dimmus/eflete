#ifndef UI_NOTIFY_HEADER_H
#define UI_NOTIFY_HEADER_H

/**
 * @defgroup Notify Notify
 *
 * Notify API for showing some user-oriented information
 * like warning or error's.
 */

#include "efl_tet.h"

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
