/* Edje Theme Editor
* Copyright (C) 2013 Samsung Electronics.
*
* This file is part of Edje Theme Editor.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; If not, see www.gnu.org/licenses/gpl-2.0.html.
*/

#ifndef EDJE_COMPILE_H
#define EDJE_COMPILE_H

/**
 * @defgroup EdjeCompile Edje Compile
 *
 * It module designed to compile and decompile edc edj file respectively.
 *
 * @ingroup ProjectManager
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* include config.h */

#include <Eina.h>
#include <Ecore.h>
#include "logger.h"

typedef void (*Edje_Compile_Log_Cb)(time_t, Eina_Stringshare*, int type);

/**
 * Compile a edc file.
 *
 * @param edc Path to input edc file. returns -1 if param is NULL or ""
 * @param edj Path to output edj file. returns -2 if param is NULL or ""
 * @param image_directory Path to a image directory of a project.
 * @param font_directory Path to a font directory of a project.
 * @param sound_direcotory Path to a sound directory of a project.
 * @return edje_cc exit code
 *
 * @ingroup EdjeCompile
 */
int
compile(const char *edc,
        const char *edj,
        const char *image_directory,
        const char *font_directory,
        const char *sound_directory,
        Edje_Compile_Log_Cb);

/**
 * Decompile a edj file.
 *
 * @param edj Path to input edj file. returns -1 if param is NULL or ""
 * @param edc Path to output folder. returns -2 if param is NULL or ""
 * @return edje_cc exit code
 *
 * @ingroup EdjeCompile
 */
int
decompile(const char *edj,
          const char *edc,
          Edje_Compile_Log_Cb);

#endif /* EDJE_COMPILE_H */
