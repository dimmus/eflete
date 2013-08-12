#ifndef IMAGE_EDIT_API_H
#define IMAGE_EDIT_API_H

/**
 * @defgroup ImageEditApi Image Edit API
 *
 * Funcitons for image manipulations in the project.
 */

#include "efl_tet.h"
#include "edit_private.h"

/**
 * Get the list of all the images in the given project.
 *
 * @param project Project being editing.
 * @return List of strings, each being the name for an image.
 *
 * @ingroup ImageEditApi
 */
Eina_List *
image_edit_images_list_get(Project *project);

/**
 * Get the image id by name
 *
 * @param project Project being editing.
 * @param image_name Filename of the image.
 * @return Id of the image.
 *
 * @ingroup ImageEditApi
 */
EAPI int
image_edit_image_id_get (Project *project, const char *image_name);

/**
 * Get the commpression type of the image.
 *
 * @param project Project being editing.
 * @param image_name Filename of the image.
 * @return Compression type of the image.
 *
 * @ingroup ImageEditApi
 */
EAPI Edje_Edit_Image_Comp
image_edit_image_compression_type_get (Project *project, const char *image);

/**
 * Get the image compression rate
 *
 * @param project Project being editing.
 * @param image_name Filename of the image.
 * @return Image compression rate.
 *
 * @ingroup ImageEditApi
 */
EAPI int
image_edit_image_compression_rate_get(Project *project, const char *image);

#endif /* IMAGE_EDIT_API_H */
