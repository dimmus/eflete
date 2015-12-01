/*
 * Edje Theme Editor
 * Copyright (C) 2013-2015 Samsung Electronics.
 *
 * This file is part of Edje Theme Editor.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; If not, see www.gnu.org/licenses/lgpl.html.
 */

#ifndef DIFF_H
#define DIFF_H
#include "eflete.h"

typedef enum {
   FUNCTION_TYPE_NONE = 0,
   FUNCTION_TYPE_SIGNALSBLOCK,
   FUNCTION_TYPE_INT,
   FUNCTION_TYPE_STRING_STRING_DOUBLE_DOUBLE,
   FUNCTION_TYPE_STRING_STRING_DOUBLE_INT,
   FUNCTION_TYPE_STRING_STRING_DOUBLE_BOOL,
   FUNCTION_TYPE_STRING_STRING_DOUBLE_STRING,
   FUNCTION_TYPE_STRING_STRING_DOUBLE_STRING_DOUBLE,
   FUNCTION_TYPE_STRING_STRING_DOUBLE_INT_INT_INT_INT,
   FUNCTION_TYPE_STRING_STRING_DOUBLE_UCHAR,
   FUNCTION_TYPE_STRING_EDJEPARTTYPE,
   FUNCTION_TYPE_STRING_EDJETEXTEFFECT,
   FUNCTION_TYPE_STRING_EVASEVENTFLAGS,
   FUNCTION_TYPE_STRING_BOOL,
   FUNCTION_TYPE_STRING_INT,
   FUNCTION_TYPE_STRING_STRING,
   FUNCTION_TYPE_STRING,
   FUNCTION_TYPE_STRING_STRING_EDJEASPECTCONTROL,
   FUNCTION_TYPE_STRING_STRING_DOUBLE,
   FUNCTION_TYPE_STRING_STRING_INT,
   FUNCTION_TYPE_STRING_STRING_USHORT,
   FUNCTION_TYPE_STRING_STRING_STRING,
   FUNCTION_TYPE_STRING_STRING_INT_INT_INT_INT,
   FUNCTION_TYPE_STRING_EDJEEDITSELECTMODE,
   FUNCTION_TYPE_STRING_EDJEEDITENTRYMODE,
   FUNCTION_TYPE_STRING_EVASOBJECTPOINTERMODE,
   FUNCTION_TYPE_STRING_UCHAR,
} Function_Type;

struct _Function_Info {
   Function_Type type;  /**< function type */
   void *function;      /**< pointer to function */
   union {
      /* One struct for each function type */
      struct {
         int i1;
      } type_i;
      struct {
         Eina_Stringshare *s1;
         Eina_Stringshare *s2;
         double d3;
         double d4;
      } type_ssdd;
      struct {
         Eina_Stringshare *s1;
         Eina_Stringshare *s2;
         double d3;
         int i4;
      } type_ssdi;
      struct {
         Eina_Stringshare *s1;
         Eina_Stringshare *s2;
         double d3;
         Eina_Bool b4;
      } type_ssdb;
      struct {
         Eina_Stringshare *s1;
         Eina_Stringshare *s2;
         double d3;
         Eina_Stringshare *s4;
      } type_ssds;
      struct {
         Eina_Stringshare *s1;
         Eina_Stringshare *s2;
         double d3;
         Eina_Stringshare *s4;
         double d5;
      } type_ssdsd;
      struct {
         Eina_Stringshare *s1;
         Eina_Stringshare *s2;
         double d3;
         int i4;
         int i5;
         int i6;
         int i7;
      } type_ssdiiii;
      struct {
         Eina_Stringshare *s1;
         Eina_Stringshare *s2;
         double d3;
         unsigned char uc4;
      } type_ssduc;
      struct {
         Eina_Stringshare *s1;
         Edje_Part_Type ept2;
      } type_sept;
      struct {
         Eina_Stringshare *s1;
         Edje_Text_Effect ete2;
      } type_sete;
      struct {
         Eina_Stringshare *s1;
         Evas_Event_Flags eef2;
      } type_seef;
      struct {
         Eina_Stringshare *s1;
         Eina_Bool b2;
      } type_sb;
      struct {
         Eina_Stringshare *s1;
         int i2;
      } type_si;
      struct {
         Eina_Stringshare *s1;
         Eina_Stringshare *s2;
      } type_ss;
      struct {
         Eina_Stringshare *s1;
      } type_s;
      struct {
         Eina_Stringshare *s1;
         Eina_Stringshare *s2;
         Edje_Aspect_Control eac3;
      } type_sseac;
      struct {
         Eina_Stringshare *s1;
         Eina_Stringshare *s2;
         double d3;
      } type_ssd;
      struct {
         Eina_Stringshare *s1;
         Eina_Stringshare *s2;
         int i3;
      } type_ssi;
      struct {
         Eina_Stringshare *s1;
         Eina_Stringshare *s2;
         unsigned short us3;
      } type_ssus;
      struct {
         Eina_Stringshare *s1;
         Eina_Stringshare *s2;
         Eina_Stringshare *s3;
      } type_sss;
      struct {
         Eina_Stringshare *s1;
         Eina_Stringshare *s2;
         int i3;
         int i4;
         int i5;
         int i6;
      } type_ssiiii;
      struct {
         Eina_Stringshare *s1;
         Edje_Edit_Select_Mode eesm2;
      } type_seesm;
      struct {
         Eina_Stringshare *s1;
         Edje_Edit_Entry_Mode eeem2;
      } type_seeem;
      struct {
         Eina_Stringshare *s1;
         Evas_Object_Pointer_Mode eopm2;
      } type_seopm;
      struct {
         Eina_Stringshare *s1;
         unsigned char uc2;
      } type_suc;
   } args;              /**< function args not including Evas_Object * */
};

typedef struct _Function_Info Function_Info;

struct _Diff_ {
   Function_Info undo;
   Function_Info redo;
};

typedef struct _Diff_ Diff;

Eina_Bool
diff_undo(Evas_Object *obj, Diff *diff);

Eina_Bool
diff_redo(Evas_Object *obj, Diff *diff);

void
diff_update(Diff *diff, Diff *new_diff);

void
diff_free(Diff *diff);

#endif /* DIFF_H */
