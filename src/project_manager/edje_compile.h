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

/**
 * @struct _Compiler_Message
 * The struct designed to saved compile messages.
 *
 * @ingroup EdjeCompile
 */
struct _Compiler_Message
{
   EINA_INLIST;
   /** Member 'time' consist a time of a compile message */
   time_t time;
   /** Member 'text' consist a text of a compile message */
   char *text;
};

/**
 * @typedef Compiler_Message
 * @ingroup EdjeCompile
 */
typedef struct _Compiler_Message Compiler_Message;

/**
 * @struct _Edje_CC
 * @ingroup EdjeCompile
 */
struct _Edje_CC
{
   /** Member 'exe' consist a Ecore_Exe heandler */
   Ecore_Exe *exe;
   /** Member 'cmd' consist a executing command */
   char *cmd;
   /** Member 'messages' consist a compile messages */
   Eina_Inlist *messages;
};

/**
 * @typedef Edje_CC
 * @ingroup EdjeCompile
 */
typedef struct _Edje_CC Edje_CC;

/**
 * @typedef Edje_DeCC
 * @ingroup EdjeCompile
 */
typedef struct _Edje_CC Edje_DeCC;

/**
 * Compile a edc file.
 *
 * @param edc Path to input edc file.
 * @param edj Path to output edj file.
 * @param image_directory Path to a image directory of a project.
 * @param font_directory Path to a font directory of a project.
 * @param sound_direcotory Path to a sound directory of a project.
 * @return A Edje_CC object.
 *
 * @ingroup EdjeCompile
 */
Edje_CC *
compile(const char *edc,
        const char *edj,
        const char *image_directory,
        const char *font_directory,
        const char *sound_directory);

/**
 * Decompile a edj file.
 *
 * @param edj Path to input edj file.
 * @param edc Path to output edc file.(TODO:NOT IMPLEMENTED)
 *
 * @ingroup EdjeCompile
 */
Edje_DeCC *
decompile(char *edj, char *edc);

/**
 * Delete a Edje_CC object.
 *
 * @param compiler A Edje_CC object to free.
 *
 * @ingroup EdjeCompile
 */
#define compiler_free(compiler) \
   edje_cc_free(compiler);

/**
 * Delete a Edje_DeCC object.
 *
 * @param decompiler A Edje_DeCC object to free.
 *
 * @ingroup EdjeCompile
 */
#define decompiler_free(decompiler) \
   edje_cc_free(decompiler);


/**
 *
 */
void
edje_cc_free(struct _Edje_CC *edje_cc);

#endif /* EDJE_COMPILE_H */
