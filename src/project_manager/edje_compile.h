#ifndef EDJE_COMPILE_H
#define EDJE_COMPILE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* include config.h */

#include <Eina.h>
#include <Ecore.h>
#include "logger.h"

struct _Compiler_Message
{
   EINA_INLIST;
   time_t time;
   char *text;
};

/**
 *
 */
typedef struct _Compiler_Message Compiler_Message;

struct _Edje_CC
{
   Ecore_Exe *exe;
   char *cmd;
   Eina_Inlist *messages;
};

/**
 *
 */
typedef struct _Edje_CC Edje_CC;

/**
 *
 */
typedef struct _Edje_CC Edje_DeCC;

/**
 *
 */
Edje_CC *
compile(const char *edc,
        const char *edj,
        const char *image_directory,
        const char *font_directory,
        const char *sound_directory);

/**
 *
 */
Edje_DeCC *
decompile(char *edj, char *edc);

/**
 *
 */
#define compiler_free(compiler) \
   edje_cc_free(compiler);

/**
 *
 */
#define decompiler_free(decompiler) \
   edje_cc_free(decompiler);


/**
 *
 */
void
edje_cc_free(struct _Edje_CC *edje_cc);

#endif /* EDJE_COMPILE_H */
