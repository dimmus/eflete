#ifndef NAVIGATOR_H
#define NAVIGATOR_H

/**
 * @defgroup Navigator
 * @ingroup Block
 *
 * This widget is used to navigate in groups tree.
 *
 * This widget emits the following signals:
 * @li "project_navigator,group,selected": the user double clicked on the group name;
 */
#include "eflete.h"

/**
 * Add project_navigator object. This object is a layout with two buttons: "plus" and
 * "minus", for add and delete layout from project_navigator. Also layout contaions a
 * genlist for view and navigate by layouts in given project.
 *
 * @return The project_navigator object
 * @ingroup Navigator
 */
Evas_Object *
project_navigator_add(void);

/**
 * Set the project to project_navigator. By project data project_navigator build a layouts
 * (styles) tree. Navigator used the global data (App_Data) for take a opened
 * project.
 *
 * @warning If project not avaliable assert is work.
 *
 * @ingroup Navigator
 */
void
project_navigator_project_set(void);

/**
 * Unset the given project. Navigator delete all data from genlist and disable
 * manipulation buttons ("plus" and "delete").
 *
 * @ingroup Navigator
 */
void
project_navigator_project_unset(void);

#endif /* NAVIGATOR_H */
