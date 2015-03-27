
#ifndef EWE_ENTRY_LEGACY_H
#define EWE_ENTRY_LEGACY_H

/**
 * This adds an entry to @p parent object.
 *
 * By default, entries are:
 * @li not scrolled
 * @li multi-line
 * @li word wrapped
 * @li autosave is enabled
 *
 * @param parent The parent object
 * @return The new object or NULL if it cannot be created
 *
 * @ingroup Entry
 */
EAPI Evas_Object       *ewe_entry_add(Evas_Object *parent);

/**
 * Set regular expresion to be compiled and used.
 *
 * @param obj The entry object
 * @param regex_str The text of regular expresion
 * @param flags Compile flags for regex
 * @return EINA_TRUE on success, otherwise returns EINA_FALSE.
 *
 * @ingroup Entry
 */
EAPI Eina_Bool          ewe_entry_regex_set(Evas_Object *obj, const char *regex_str, int flags);

/**
 * Unset previoustly set regular expresion.
 *
 * @param obj The entry object
 * @return EINA_TRUE on success, otherwise returns EINA_FALSE.
 *
 * @ingroup Entry
 */
EAPI Eina_Bool          ewe_entry_regex_unset(Evas_Object *obj);

/**
 * Check current text of Entry with previoustly set regular expresion.
 *
 * @param obj The entry object
 * @return EINA_TRUE on success, otherwise returns EINA_FALSE.
 *
 * @ingroup Entry
 */
EAPI Eina_Bool          ewe_entry_regex_check(Evas_Object *obj);

/**
 * Set if regex error signals for EDJE should be emitted.
 *
 * @param obj The entry object
 * @param glow Boolean to set signals emition on/off
 * @return EINA_TRUE on success, otherwise returns EINA_FALSE.
 *
 * @ingroup Entry
 */
EAPI Eina_Bool          ewe_entry_regex_glow_set(Evas_Object *obj, Eina_Bool glow);

/**
 * Returns boolean to check if redex error-signals emition is on/off
 *
 * @param obj The entry object.
 * @return EINA_TRUE if signals glowing is on, otherwise, returns EINA_FALSE.
 *
 * @ingroup Entry
 */
EAPI Eina_Bool          ewe_entry_regex_glow_get(const Evas_Object *obj);

/**
 * Returns last regex error
 *
 * @param obj The entry object.
 * @return The last regex error.
 *
 * @ingroup Entry
 */
EAPI int                ewe_entry_regex_error_get(const Evas_Object *obj);

/**
 * Returns the string that contains last regex error.
 * This message updates on each regex setting and checking.
 * The string is deleted on Entry object deleteon.
 *
 * @param obj The entry object.
 * @return The last regex error. If regex is not set yet NULL would be returned.
 *
 * @ingroup Entry
 */
EAPI const char *       ewe_entry_regex_error_text_get(const Evas_Object *obj);

/**
 * Set automatical regex check on "entry,changed".
 * Signals will be processed only for focused entry.
 *
 * @param obj The entry object
 * @param emit Boolean to set autocheck on/off
 * @return EINA_TRUE on success, otherwise returns EINA_FALSE.
 *
 * @ingroup Entry
 */
EAPI Eina_Bool          ewe_entry_regex_autocheck_set(Evas_Object *obj, Eina_Bool autocheck);
/**
 * Returns boolean to check if redex autocheck is on/off
 *
 * @param obj The entry object.
 * @return EINA_TRUE if autocheck is on, otherwise, returns EINA_FALSE.
 *
 * @ingroup Entry
 */
EAPI Eina_Bool          ewe_entry_regex_autocheck_get(const Evas_Object *obj);
/**
 * This sets the text displayed within the entry to @p entry.
 *
 * @param obj The entry object
 * @param entry The text to be displayed
 *
 * @note Using this function bypasses text filters
 *
 * @ingroup Entry
 */
EAPI void               ewe_entry_entry_set(Evas_Object *obj, const char *text);

#endif /* EWE_ENTRY_LEGACY_H */
