
#ifndef EWE_ENTRY_H
#define EWE_ENTRY_H

/**
 * @defgroup Entry Entry
 * @ingroup Ewe
 *
 * An entry is a convenience widget which shows a box that the user can
 * enter text into. Entries by default don't scroll, so they grow to
 * accommodate the entire text, resizing the parent window as needed. This
 * can be changed with the ewe_entry_scrollable_set() function.
 *
 * They can also be single line or multi line (the default).
 *
 * This widget inherits from the Elementary Entry, so that all the
 * functions acting on it also work for entry objects.
 *
 */

#include "ewe_entry_common.h"
#ifdef EFL_EO_API_SUPPORT
#include "ewe_entry_eo.h"
#endif
#ifndef EFL_NOLEGACY_API_SUPPORT
#include "ewe_entry_legacy.h"
#endif

#endif /* EWE_ENTRY_H */
