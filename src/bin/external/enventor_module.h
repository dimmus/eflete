#ifndef ENVENTOR_MODULE_H
#define ENVENTOR_MODULE_H

TODO("check this define")
#ifdef HAVE_ENVENTOR
   #define ENVENTOR_BETA_API_SUPPORT
   #include "Enventor.h"
#endif /* HAVE_ENVENTOR */

#include "eflete.h"

/**
 * @defgroup Enventor Enventor library support
 *
 * Function of this submodule provide ability to use enventor object
 * in eflete.
 */

/**
 * @struct _Enventor_Data
 *
 * Structure, which store data, that needed for normal work enventor module.
 *
 * @ingroup Enventor
 */
struct _Enventor_Data
{
    Eina_Stringshare *file; /**< exported edc file, which uses for enventor mode*/
    Eina_Stringshare *path; /**< path to exported style project*/
};

/**
 * Function initialize enventor object. Register callbacks for
 * manipulate with enventor object.
 *
 * @param parent The evas object.
 *
 * @return Pointer to created enventor object if successful, or NULL
 * in otherwise.
 *
 * @ingroup Enventor
 */
Evas_Object *
enventor_object_init(Evas_Object *parent);

/**
 * Create temporary edc project, that load into enventor object. Use
 * current loaded style for create edc file. This file contain currently open
 * style.
 *
 * @param enventor The enventor object.
 * @param project Loaded project.
 *
 * @return true if temporary project successfuly created and loaded into
 * enventor object, or false in otherwise.
 *
 * @ingroup Enventor
 */
Eina_Bool
enventor_object_project_load(Evas_Object *enventor, Project *project);

/**
 * Free data and directories, which was created as temporary project for enventor
 * object.
 *
 * @param project Loaded project.
 *
 * @return true if temporary project freed successfuly, or false in otherwise.
 *
 * @ingroup Enventor
 */
Eina_Bool
enventor_object_project_unload(Project *project);

/**
 * Load enventor settings from given config profile.
 *
 * @param enventor The enventor object.
 * @param profile The filled profile structure.
 *
 * @return true if successful, or false in otherwise.
 *
 * @ingroup Enventor
 */
Eina_Bool
enventor_object_profile_load(Evas_Object *enventor, Profile *profile);


/**
 * If loaded edc code does not contain the data item  "version", then into edc
 * file will be added string: "version" "version value". Version value is passed
 * as argument.
 *
 * @param enventor The enventor object.
 * @param project Loaded project.
 * @param key The version value. Should be like "110".
 *
 * @return true if successful, or false in otherwise.
 *
 * @ingroup Enventor
 */
Eina_Bool
enventor_object_file_version_update(Evas_Object *enventor, Project *project, const char *key);

#endif /* ENVENTOR_MODULE_H */
