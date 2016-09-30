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

#include "diff.h"
#include "change.h"

typedef Eina_Bool (* function_type_signalsblock) (Change*);
typedef Eina_Bool (* function_type_int) (Evas_Object *, Change*, Eina_Bool, Eina_Bool, int);
typedef Eina_Bool (* function_type_string_string_double_double) (Evas_Object *, Change*, Eina_Bool, Eina_Bool,
                                                                 const char *, const char *, double, double);
typedef Eina_Bool (* function_type_string_string_double_int) (Evas_Object *, Change*, Eina_Bool, Eina_Bool,
                                                              const char *, const char *, double, int);
typedef Eina_Bool (* function_type_string_string_double_bool) (Evas_Object *, Change*, Eina_Bool, Eina_Bool,
                                                               const char *, const char *, double, Eina_Bool);
typedef Eina_Bool (* function_type_string_string_double_string) (Evas_Object *, Change*, Eina_Bool, Eina_Bool,
                                                                 const char *, const char *, double, const char *);
typedef Eina_Bool (* function_type_string_string_double_string_double) (Evas_Object *, Change*, Eina_Bool, Eina_Bool,
                                                                        const char *, const char *, double, const char *, double);
typedef Eina_Bool (* function_type_string_string_double_int_int_int_int) (Evas_Object *, Change*, Eina_Bool, Eina_Bool,
                                                                          const char *, const char *, double, int, int, int, int);
typedef Eina_Bool (* function_type_string_string_double_int_int_int_int_int) (Evas_Object *, Change*, Eina_Bool, Eina_Bool,
                                                                              const char *, const char *, double, int, int, int, int, int);
typedef Eina_Bool (* function_type_string_string_double_uchar) (Evas_Object *, Change*, Eina_Bool, Eina_Bool,
                                                                const char *, const char *, double, unsigned char);
typedef Eina_Bool (* function_type_string_edjetexteffect) (Evas_Object *, Change*, Eina_Bool, Eina_Bool,
                                                           const char *, Edje_Text_Effect);
typedef Eina_Bool (* function_type_string_evaseventflags) (Evas_Object *, Change*, Eina_Bool, Eina_Bool,
                                                           const char *, Evas_Event_Flags);
typedef Eina_Bool (* function_type_string_bool) (Evas_Object *, Change*, Eina_Bool, Eina_Bool,
                                                 const char *, Eina_Bool);
typedef Eina_Bool (* function_type_string_int) (Evas_Object *, Change*, Eina_Bool, Eina_Bool,
                                                const char *, int);
typedef Eina_Bool (* function_type_string_string) (Evas_Object *, Change*, Eina_Bool, Eina_Bool,
                                                   const char *, const char *);
typedef Eina_Bool (* function_type_string_uint) (Evas_Object *, Change*, Eina_Bool, Eina_Bool,
                                                   const char *, unsigned int);
typedef Eina_Bool (* function_type_string) (Evas_Object *, Change*, Eina_Bool, Eina_Bool,
                                            const char *);
typedef Eina_Bool (* function_type_string_string_edjeaspectcontrol) (Evas_Object *, Change*, Eina_Bool, Eina_Bool,
                                                                     const char *, const char *, Edje_Aspect_Control);
typedef Eina_Bool (* function_type_string_uint_edjeaspectcontrol) (Evas_Object *, Change*, Eina_Bool, Eina_Bool,
                                                                     const char *, unsigned int, Edje_Aspect_Control);
typedef Eina_Bool (* function_type_string_edjeparttype) (Evas_Object *, Change*, Eina_Bool, Eina_Bool,
                                                         const char *, Edje_Part_Type);
typedef Eina_Bool (* function_type_string_edjepartcopy) (Evas_Object *, Change*, Eina_Bool, Eina_Bool,
                                                         const char *, const char *);
typedef Eina_Bool (* function_type_string_string_double) (Evas_Object *, Change*, Eina_Bool, Eina_Bool,
                                                          const char *, const char *, double);
typedef Eina_Bool (* function_type_string_uint_double) (Evas_Object *, Change*, Eina_Bool, Eina_Bool,
                                                        const char *, unsigned int, double);
typedef Eina_Bool (* function_type_string_string_int) (Evas_Object *, Change*, Eina_Bool, Eina_Bool,
                                                       const char *, const char *, int);
typedef Eina_Bool (* function_type_string_uint_int) (Evas_Object *, Change*, Eina_Bool, Eina_Bool,
                                                     const char *, unsigned int, int);
typedef Eina_Bool (* function_type_string_string_ushort) (Evas_Object *, Change*, Eina_Bool, Eina_Bool,
                                                          const char *, const char *, unsigned short);
typedef Eina_Bool (* function_type_string_uint_ushort) (Evas_Object *, Change*, Eina_Bool, Eina_Bool,
                                                        const char *, unsigned int, unsigned short);
typedef Eina_Bool (* function_type_string_string_string) (Evas_Object *, Change*, Eina_Bool, Eina_Bool,
                                                          const char *, const char *, const char *);
typedef Eina_Bool (* function_type_string_uint_uint) (Evas_Object *, Change*, Eina_Bool, Eina_Bool,
                                                          const char *, unsigned int, unsigned int);
typedef Eina_Bool (* function_type_string_uint_string) (Evas_Object *, Change*, Eina_Bool, Eina_Bool,
                                                        const char *, unsigned int, const char *);
typedef Eina_Bool (* function_type_string_string_int_int_int_int) (Evas_Object *, Change*, Eina_Bool, Eina_Bool,
                                                                   const char *, const char *, int, int, int, int);
typedef Eina_Bool (* function_type_string_edjeeditselectmode) (Evas_Object *, Change*, Eina_Bool, Eina_Bool,
                                                               const char *, Edje_Edit_Select_Mode);
typedef Eina_Bool (* function_type_string_edjeeditentrymode) (Evas_Object *, Change*, Eina_Bool, Eina_Bool,
                                                              const char *, Edje_Edit_Entry_Mode);
typedef Eina_Bool (* function_type_string_evasobjectpointermode) (Evas_Object *, Change*, Eina_Bool, Eina_Bool,
                                                                  const char *, Evas_Object_Pointer_Mode);
typedef Eina_Bool (* function_type_string_uchar) (Evas_Object *, Change*, Eina_Bool, Eina_Bool,
                                                  const char *, unsigned char);
typedef Eina_Bool (* function_type_string_edjeactiontype) (Evas_Object *, Change*, Eina_Bool, Eina_Bool,
                                                           const char *, Edje_Action_Type);
typedef Eina_Bool (* function_type_string_edjechannel) (Evas_Object *, Change*, Eina_Bool, Eina_Bool,
                                                        const char *, Edje_Channel);
typedef Eina_Bool (* function_type_string_edjetweenmode) (Evas_Object *, Change*, Eina_Bool, Eina_Bool,
                                                          const char *, Edje_Tween_Mode);
typedef Eina_Bool (* function_type_string_double) (Evas_Object *, Change*, Eina_Bool, Eina_Bool,
                                                   const char *, double);

static Eina_Bool
_apply(Evas_Object *obj, Function_Info *fi)
{
   switch (fi->type)
     {
      case FUNCTION_TYPE_NONE:
         return true;
      case FUNCTION_TYPE_SIGNALSBLOCK:
         return ((function_type_signalsblock)fi->function)(NULL);
      case FUNCTION_TYPE_INT:
         return ((function_type_int)fi->function)(obj, NULL, false, true, fi->args.type_i.i1);
      case FUNCTION_TYPE_STRING_STRING_DOUBLE_DOUBLE:
         return ((function_type_string_string_double_double)fi->function)(obj, NULL, false, true,
                  fi->args.type_ssdd.s1, fi->args.type_ssdd.s2, fi->args.type_ssdd.d3, fi->args.type_ssdd.d4);
      case FUNCTION_TYPE_STRING_STRING_DOUBLE_INT:
         return ((function_type_string_string_double_int)fi->function)(obj, NULL, false, true,
                  fi->args.type_ssdi.s1, fi->args.type_ssdi.s2, fi->args.type_ssdi.d3, fi->args.type_ssdi.i4);
      case FUNCTION_TYPE_STRING_STRING_DOUBLE_BOOL:
         return ((function_type_string_string_double_bool)fi->function)(obj, NULL, false, true,
                  fi->args.type_ssdb.s1, fi->args.type_ssdb.s2, fi->args.type_ssdb.d3, fi->args.type_ssdb.b4);
      case FUNCTION_TYPE_STRING_STRING_DOUBLE_STRING:
         return ((function_type_string_string_double_string)fi->function)(obj, NULL, false, true,
                  fi->args.type_ssds.s1, fi->args.type_ssds.s2, fi->args.type_ssds.d3, fi->args.type_ssds.s4);
      case FUNCTION_TYPE_STRING_STRING_DOUBLE_STRING_DOUBLE:
         return ((function_type_string_string_double_string_double)fi->function)(obj, NULL, false, true,
                  fi->args.type_ssdsd.s1, fi->args.type_ssdsd.s2, fi->args.type_ssdsd.d3, fi->args.type_ssdsd.s4, fi->args.type_ssdsd.d5);
      case FUNCTION_TYPE_STRING_STRING_DOUBLE_INT_INT_INT_INT:
         return ((function_type_string_string_double_int_int_int_int)fi->function)(obj, NULL, false, true,
                  fi->args.type_ssdiiii.s1, fi->args.type_ssdiiii.s2, fi->args.type_ssdiiii.d3, fi->args.type_ssdiiii.i4, fi->args.type_ssdiiii.i5, fi->args.type_ssdiiii.i6, fi->args.type_ssdiiii.i7);
      case FUNCTION_TYPE_STRING_STRING_DOUBLE_UCHAR:
         return ((function_type_string_string_double_uchar)fi->function)(obj, NULL, false, true,
                  fi->args.type_ssduc.s1, fi->args.type_ssduc.s2, fi->args.type_ssduc.d3, fi->args.type_ssduc.uc4);
      case FUNCTION_TYPE_STRING_EDJEPARTTYPE:
         return ((function_type_string_edjeparttype)fi->function)(obj, NULL, false, true,
                  fi->args.type_sept.s1, fi->args.type_sept.ept2);
      case FUNCTION_TYPE_STRING_EDJEPARTCOPY:
         return ((function_type_string_edjepartcopy)fi->function)(obj, NULL, false, true,
                  fi->args.type_sepc.s1, fi->args.type_sepc.s2);
      case FUNCTION_TYPE_STRING_EDJETEXTEFFECT:
         return ((function_type_string_edjetexteffect)fi->function)(obj, NULL, false, true,
                  fi->args.type_sete.s1, fi->args.type_sete.ete2);
      case FUNCTION_TYPE_STRING_EVASEVENTFLAGS:
         return ((function_type_string_evaseventflags)fi->function)(obj, NULL, false, true,
                  fi->args.type_seef.s1, fi->args.type_seef.eef2);
      case FUNCTION_TYPE_STRING_BOOL:
         return ((function_type_string_bool)fi->function)(obj, NULL, false, true,
                  fi->args.type_sb.s1, fi->args.type_sb.b2);
      case FUNCTION_TYPE_STRING_INT:
         return ((function_type_string_int)fi->function)(obj, NULL, false, true,
                  fi->args.type_si.s1, fi->args.type_si.i2);
      case FUNCTION_TYPE_STRING_STRING:
      case FUNCTION_TYPE_STRING_STRING_RENAME:
         return ((function_type_string_string)fi->function)(obj, NULL, false, true,
                  fi->args.type_ss.s1, fi->args.type_ss.s2);
      case FUNCTION_TYPE_STRING_UINT:
         return ((function_type_string_uint)fi->function)(obj, NULL, false, true,
                  fi->args.type_sui.s1, fi->args.type_sui.ui2);
      case FUNCTION_TYPE_STRING:
         return ((function_type_string)fi->function)(obj, NULL, false, true,
                  fi->args.type_s.s1);
      case FUNCTION_TYPE_STRING_STRING_EDJEASPECTCONTROL:
         return ((function_type_string_string_edjeaspectcontrol)fi->function)(obj, NULL, false, true,
                  fi->args.type_sseac.s1, fi->args.type_sseac.s2, fi->args.type_sseac.eac3);
      case FUNCTION_TYPE_STRING_UINT_EDJEASPECTCONTROL:
         return ((function_type_string_uint_edjeaspectcontrol)fi->function)(obj, NULL, false, true,
                  fi->args.type_suieac.s1, fi->args.type_suieac.ui2, fi->args.type_suieac.eac3);
      case FUNCTION_TYPE_STRING_STRING_DOUBLE:
         return ((function_type_string_string_double)fi->function)(obj, NULL, false, true,
                  fi->args.type_ssd.s1, fi->args.type_ssd.s2, fi->args.type_ssd.d3);
      case FUNCTION_TYPE_STRING_UINT_DOUBLE:
         return ((function_type_string_uint_double)fi->function)(obj, NULL, false, true,
                  fi->args.type_suid.s1, fi->args.type_suid.ui2, fi->args.type_suid.d3);
      case FUNCTION_TYPE_STRING_STRING_INT:
         return ((function_type_string_string_int)fi->function)(obj, NULL, false, true,
                  fi->args.type_ssi.s1, fi->args.type_ssi.s2, fi->args.type_ssi.i3);
      case FUNCTION_TYPE_STRING_UINT_INT:
         return ((function_type_string_uint_int)fi->function)(obj, NULL, false, true,
                  fi->args.type_suii.s1, fi->args.type_suii.ui2, fi->args.type_suii.i3);
      case FUNCTION_TYPE_STRING_STRING_USHORT:
         return ((function_type_string_string_ushort)fi->function)(obj, NULL, false, true,
                  fi->args.type_ssus.s1, fi->args.type_ssus.s2, fi->args.type_ssus.us3);
      case FUNCTION_TYPE_STRING_UINT_USHORT:
         return ((function_type_string_uint_ushort)fi->function)(obj, NULL, false, true,
                  fi->args.type_suius.s1, fi->args.type_suius.ui2, fi->args.type_suius.us3);
      case FUNCTION_TYPE_STRING_STRING_STRING:
         return ((function_type_string_string_string)fi->function)(obj, NULL, false, true,
                  fi->args.type_sss.s1, fi->args.type_sss.s2, fi->args.type_sss.s3);
      case FUNCTION_TYPE_STRING_UINT_UINT:
         return ((function_type_string_uint_uint)fi->function)(obj, NULL, false, true,
                  fi->args.type_suiui.s1, fi->args.type_suiui.ui2, fi->args.type_suiui.ui3);
      case FUNCTION_TYPE_STRING_UINT_STRING:
         return ((function_type_string_uint_string)fi->function)(obj, NULL, false, true,
                  fi->args.type_suis.s1, fi->args.type_suis.ui2, fi->args.type_suis.s3);
      case FUNCTION_TYPE_STRING_STRING_INT_INT_INT_INT:
         return ((function_type_string_string_int_int_int_int)fi->function)(obj, NULL, false, true,
                  fi->args.type_ssiiii.s1, fi->args.type_ssiiii.s2, fi->args.type_ssiiii.i3, fi->args.type_ssiiii.i4, fi->args.type_ssiiii.i5, fi->args.type_ssiiii.i6);
      case FUNCTION_TYPE_STRING_EDJEEDITSELECTMODE:
         return ((function_type_string_edjeeditselectmode)fi->function)(obj, NULL, false, true,
                  fi->args.type_seesm.s1, fi->args.type_seesm.eesm2);
      case FUNCTION_TYPE_STRING_EDJEEDITENTRYMODE:
         return ((function_type_string_edjeeditentrymode)fi->function)(obj, NULL, false, true,
                  fi->args.type_seeem.s1, fi->args.type_seeem.eeem2);
      case FUNCTION_TYPE_STRING_EVASOBJECTPOINTERMODE:
         return ((function_type_string_evasobjectpointermode)fi->function)(obj, NULL, false, true,
                  fi->args.type_seopm.s1, fi->args.type_seopm.eopm2);
      case FUNCTION_TYPE_STRING_UCHAR:
         return ((function_type_string_uchar)fi->function)(obj, NULL, false, true,
                  fi->args.type_suc.s1, fi->args.type_suc.uc2);
      case FUNCTION_TYPE_STRING_EDJEACTIONTYPE:
         return ((function_type_string_edjeactiontype)fi->function)(obj, NULL, false, true,
                  fi->args.type_seat.s1, fi->args.type_seat.eat2);
      case FUNCTION_TYPE_STRING_EDJECHANNEL:
         return ((function_type_string_edjechannel)fi->function)(obj, NULL, false, true,
                  fi->args.type_sec.s1, fi->args.type_sec.ec2);
      case FUNCTION_TYPE_STRING_EDJETWEENMODE:
         return ((function_type_string_edjetweenmode)fi->function)(obj, NULL, false, true,
                  fi->args.type_setm.s1, fi->args.type_setm.etm2);
      case FUNCTION_TYPE_STRING_DOUBLE:
         return ((function_type_string_double)fi->function)(obj, NULL, false, true,
                  fi->args.type_sd.s1, fi->args.type_sd.d2);

         /* Don't add 'case default:'. Compiler should warn about new values in enum */
     }
   return false;
}

Eina_Bool
diff_undo(Evas_Object *obj, Diff *diff)
{
   assert(obj != NULL);
   assert(diff != NULL);

   Eina_Bool res = _apply(obj, &diff->undo);
   assert(res);
   return res;
}

Eina_Bool
diff_redo(Evas_Object *obj, Diff *diff)
{
   assert(obj != NULL);
   assert(diff != NULL);

   Eina_Bool res = _apply(obj, &diff->redo);
   assert(res);
   return res;
}

void
diff_update(Diff *diff, Diff *new_diff)
{
   assert(diff != NULL);
   assert(new_diff != NULL);
   assert(diff->undo.function == new_diff->undo.function);
   assert(diff->redo.function == new_diff->redo.function);

   switch (diff->redo.type)
     {
      case FUNCTION_TYPE_NONE:
      case FUNCTION_TYPE_INT:
      case FUNCTION_TYPE_SIGNALSBLOCK:
         break;
      case FUNCTION_TYPE_STRING_STRING_DOUBLE_DOUBLE:
         eina_stringshare_del(diff->redo.args.type_ssdd.s1);
         eina_stringshare_del(diff->redo.args.type_ssdd.s2);
         eina_stringshare_ref(new_diff->redo.args.type_ssdd.s1);
         eina_stringshare_ref(new_diff->redo.args.type_ssdd.s2);
         break;
      case FUNCTION_TYPE_STRING_STRING_DOUBLE_INT:
         eina_stringshare_del(diff->redo.args.type_ssdi.s1);
         eina_stringshare_del(diff->redo.args.type_ssdi.s2);
         eina_stringshare_ref(new_diff->redo.args.type_ssdi.s1);
         eina_stringshare_ref(new_diff->redo.args.type_ssdi.s2);
         break;
      case FUNCTION_TYPE_STRING_STRING_DOUBLE_BOOL:
         eina_stringshare_del(diff->redo.args.type_ssdb.s1);
         eina_stringshare_del(diff->redo.args.type_ssdb.s2);
         eina_stringshare_ref(new_diff->redo.args.type_ssdb.s1);
         eina_stringshare_ref(new_diff->redo.args.type_ssdb.s2);
         break;
      case FUNCTION_TYPE_STRING_STRING_DOUBLE_STRING:
         eina_stringshare_del(diff->redo.args.type_ssds.s1);
         eina_stringshare_del(diff->redo.args.type_ssds.s2);
         eina_stringshare_del(diff->redo.args.type_ssds.s4);
         eina_stringshare_ref(new_diff->redo.args.type_ssds.s1);
         eina_stringshare_ref(new_diff->redo.args.type_ssds.s2);
         eina_stringshare_ref(new_diff->redo.args.type_ssds.s4);
         break;
      case FUNCTION_TYPE_STRING_STRING_DOUBLE_STRING_DOUBLE:
         eina_stringshare_del(diff->redo.args.type_ssdsd.s1);
         eina_stringshare_del(diff->redo.args.type_ssdsd.s2);
         eina_stringshare_del(diff->redo.args.type_ssdsd.s4);
         eina_stringshare_ref(new_diff->redo.args.type_ssdsd.s1);
         eina_stringshare_ref(new_diff->redo.args.type_ssdsd.s2);
         eina_stringshare_ref(new_diff->redo.args.type_ssdsd.s4);
         break;
      case FUNCTION_TYPE_STRING_STRING_DOUBLE_INT_INT_INT_INT:
         eina_stringshare_del(diff->redo.args.type_ssdiiii.s1);
         eina_stringshare_del(diff->redo.args.type_ssdiiii.s2);
         eina_stringshare_ref(new_diff->redo.args.type_ssdiiii.s1);
         eina_stringshare_ref(new_diff->redo.args.type_ssdiiii.s2);
         break;
      case FUNCTION_TYPE_STRING_STRING_DOUBLE_UCHAR:
         eina_stringshare_del(diff->redo.args.type_ssduc.s1);
         eina_stringshare_del(diff->redo.args.type_ssduc.s2);
         eina_stringshare_ref(new_diff->redo.args.type_ssduc.s1);
         eina_stringshare_ref(new_diff->redo.args.type_ssduc.s2);
         break;
      case FUNCTION_TYPE_STRING_EDJEPARTTYPE:
         eina_stringshare_del(diff->redo.args.type_sept.s1);
         eina_stringshare_ref(new_diff->redo.args.type_sept.s1);
         break;
      case FUNCTION_TYPE_STRING_EDJEPARTCOPY:
         eina_stringshare_del(diff->redo.args.type_sepc.s1);
         eina_stringshare_del(diff->redo.args.type_sepc.s2);
         eina_stringshare_ref(new_diff->redo.args.type_sepc.s1);
         eina_stringshare_ref(new_diff->redo.args.type_sepc.s2);
         break;
      case FUNCTION_TYPE_STRING_EDJETEXTEFFECT:
         eina_stringshare_del(diff->redo.args.type_sete.s1);
         eina_stringshare_ref(new_diff->redo.args.type_sete.s1);
         break;
      case FUNCTION_TYPE_STRING_EVASEVENTFLAGS:
         eina_stringshare_del(diff->redo.args.type_seef.s1);
         eina_stringshare_ref(new_diff->redo.args.type_seef.s1);
         break;
      case FUNCTION_TYPE_STRING_BOOL:
         eina_stringshare_del(diff->redo.args.type_sb.s1);
         eina_stringshare_ref(new_diff->redo.args.type_sb.s1);
         break;
      case FUNCTION_TYPE_STRING_INT:
         eina_stringshare_del(diff->redo.args.type_si.s1);
         eina_stringshare_ref(new_diff->redo.args.type_si.s1);
         break;
      case FUNCTION_TYPE_STRING_STRING:
         eina_stringshare_del(diff->redo.args.type_ss.s1);
         eina_stringshare_del(diff->redo.args.type_ss.s2);
         eina_stringshare_ref(new_diff->redo.args.type_ss.s1);
         eina_stringshare_ref(new_diff->redo.args.type_ss.s2);
         break;
      case FUNCTION_TYPE_STRING_UINT:
         eina_stringshare_del(diff->redo.args.type_sui.s1);
         eina_stringshare_ref(new_diff->redo.args.type_sui.s1);
         break;
      case FUNCTION_TYPE_STRING_STRING_RENAME:
         /* rename needs special update */
         assert(diff->redo.type == diff->undo.type);

         eina_stringshare_del(diff->undo.args.type_ss.s1);
         diff->undo.args.type_ss.s1 = eina_stringshare_ref(new_diff->undo.args.type_ss.s1);
         eina_stringshare_del(diff->redo.args.type_ss.s2);
         diff->redo.args.type_ss.s2 = eina_stringshare_ref(new_diff->redo.args.type_ss.s2);
         diff_free(new_diff);
         /* using return because we don't want to replace redo field */
         return;
      case FUNCTION_TYPE_STRING:
         eina_stringshare_del(diff->redo.args.type_s.s1);
         eina_stringshare_ref(new_diff->redo.args.type_s.s1);
         break;
      case FUNCTION_TYPE_STRING_STRING_EDJEASPECTCONTROL:
         eina_stringshare_del(diff->redo.args.type_sseac.s1);
         eina_stringshare_del(diff->redo.args.type_sseac.s2);
         eina_stringshare_ref(new_diff->redo.args.type_sseac.s1);
         eina_stringshare_ref(new_diff->redo.args.type_sseac.s2);
         break;
      case FUNCTION_TYPE_STRING_UINT_EDJEASPECTCONTROL:
         eina_stringshare_del(diff->redo.args.type_suieac.s1);
         eina_stringshare_ref(new_diff->redo.args.type_suieac.s1);
         break;
      case FUNCTION_TYPE_STRING_STRING_DOUBLE:
         eina_stringshare_del(diff->redo.args.type_ssd.s1);
         eina_stringshare_del(diff->redo.args.type_ssd.s2);
         eina_stringshare_ref(new_diff->redo.args.type_ssd.s1);
         eina_stringshare_ref(new_diff->redo.args.type_ssd.s2);
         break;
      case FUNCTION_TYPE_STRING_UINT_DOUBLE:
         eina_stringshare_del(diff->redo.args.type_suid.s1);
         eina_stringshare_ref(new_diff->redo.args.type_suid.s1);
         break;
      case FUNCTION_TYPE_STRING_STRING_INT:
         eina_stringshare_del(diff->redo.args.type_ssi.s1);
         eina_stringshare_del(diff->redo.args.type_ssi.s2);
         eina_stringshare_ref(new_diff->redo.args.type_ssi.s1);
         eina_stringshare_ref(new_diff->redo.args.type_ssi.s2);
         break;
      case FUNCTION_TYPE_STRING_UINT_INT:
         eina_stringshare_del(diff->redo.args.type_suii.s1);
         eina_stringshare_ref(new_diff->redo.args.type_suii.s1);
         break;
      case FUNCTION_TYPE_STRING_STRING_USHORT:
         eina_stringshare_del(diff->redo.args.type_ssus.s1);
         eina_stringshare_del(diff->redo.args.type_ssus.s2);
         eina_stringshare_ref(new_diff->redo.args.type_ssus.s1);
         eina_stringshare_ref(new_diff->redo.args.type_ssus.s2);
         break;
      case FUNCTION_TYPE_STRING_UINT_USHORT:
         eina_stringshare_del(diff->redo.args.type_suius.s1);
         eina_stringshare_ref(new_diff->redo.args.type_suius.s1);
         break;
      case FUNCTION_TYPE_STRING_STRING_STRING:
         eina_stringshare_del(diff->redo.args.type_sss.s1);
         eina_stringshare_del(diff->redo.args.type_sss.s2);
         eina_stringshare_del(diff->redo.args.type_sss.s3);
         eina_stringshare_ref(new_diff->redo.args.type_sss.s1);
         eina_stringshare_ref(new_diff->redo.args.type_sss.s2);
         eina_stringshare_ref(new_diff->redo.args.type_sss.s3);
         break;
      case FUNCTION_TYPE_STRING_UINT_UINT:
         eina_stringshare_del(diff->redo.args.type_suiui.s1);
         eina_stringshare_ref(new_diff->redo.args.type_suiui.s1);
         break;
      case FUNCTION_TYPE_STRING_UINT_STRING:
         eina_stringshare_del(diff->redo.args.type_suis.s1);
         eina_stringshare_del(diff->redo.args.type_suis.s3);
         eina_stringshare_ref(new_diff->redo.args.type_suis.s1);
         eina_stringshare_ref(new_diff->redo.args.type_suis.s3);
         break;
      case FUNCTION_TYPE_STRING_STRING_INT_INT_INT_INT:
         eina_stringshare_del(diff->redo.args.type_ssiiii.s1);
         eina_stringshare_del(diff->redo.args.type_ssiiii.s2);
         eina_stringshare_ref(new_diff->redo.args.type_ssiiii.s1);
         eina_stringshare_ref(new_diff->redo.args.type_ssiiii.s2);
         break;
      case FUNCTION_TYPE_STRING_EDJEEDITSELECTMODE:
         eina_stringshare_del(diff->redo.args.type_seesm.s1);
         eina_stringshare_ref(new_diff->redo.args.type_seesm.s1);
         break;
      case FUNCTION_TYPE_STRING_EDJEEDITENTRYMODE:
         eina_stringshare_del(diff->redo.args.type_seeem.s1);
         eina_stringshare_ref(new_diff->redo.args.type_seeem.s1);
         break;
      case FUNCTION_TYPE_STRING_EVASOBJECTPOINTERMODE:
         eina_stringshare_del(diff->redo.args.type_seopm.s1);
         eina_stringshare_ref(new_diff->redo.args.type_seopm.s1);
         break;
      case FUNCTION_TYPE_STRING_UCHAR:
         eina_stringshare_del(diff->redo.args.type_suc.s1);
         eina_stringshare_ref(new_diff->redo.args.type_suc.s1);
         break;
      case FUNCTION_TYPE_STRING_EDJEACTIONTYPE:
         eina_stringshare_del(diff->redo.args.type_seat.s1);
         eina_stringshare_ref(new_diff->redo.args.type_seat.s1);
         break;
      case FUNCTION_TYPE_STRING_EDJECHANNEL:
         eina_stringshare_del(diff->redo.args.type_sec.s1);
         eina_stringshare_ref(new_diff->redo.args.type_sec.s1);
         break;
      case FUNCTION_TYPE_STRING_EDJETWEENMODE:
         eina_stringshare_del(diff->redo.args.type_setm.s1);
         eina_stringshare_ref(new_diff->redo.args.type_setm.s1);
         break;
      case FUNCTION_TYPE_STRING_DOUBLE:
         eina_stringshare_del(diff->redo.args.type_sd.s1);
         eina_stringshare_ref(new_diff->redo.args.type_sd.s1);
         break;
         /* Do not forget to replace previous stringshares in existing_diff.redo
            if needed. */
         /* Don't add 'case default:'. Compiler should warn about new values in enum */
     }

   diff->redo = new_diff->redo;
   diff_free(new_diff);
}

void
diff_free(Diff *diff)
{
   assert(diff != NULL);

   switch (diff->redo.type)
     {
      case FUNCTION_TYPE_NONE:
      case FUNCTION_TYPE_INT:
      case FUNCTION_TYPE_SIGNALSBLOCK:
         break;
      case FUNCTION_TYPE_STRING_STRING_DOUBLE_DOUBLE:
         eina_stringshare_del(diff->redo.args.type_ssdd.s1);
         eina_stringshare_del(diff->redo.args.type_ssdd.s2);
         break;
      case FUNCTION_TYPE_STRING_STRING_DOUBLE_INT:
         eina_stringshare_del(diff->redo.args.type_ssdi.s1);
         eina_stringshare_del(diff->redo.args.type_ssdi.s2);
         break;
      case FUNCTION_TYPE_STRING_STRING_DOUBLE_BOOL:
         eina_stringshare_del(diff->redo.args.type_ssdb.s1);
         eina_stringshare_del(diff->redo.args.type_ssdb.s2);
         break;
      case FUNCTION_TYPE_STRING_STRING_DOUBLE_STRING:
         eina_stringshare_del(diff->redo.args.type_ssds.s1);
         eina_stringshare_del(diff->redo.args.type_ssds.s2);
         eina_stringshare_del(diff->redo.args.type_ssds.s4);
         break;
      case FUNCTION_TYPE_STRING_STRING_DOUBLE_STRING_DOUBLE:
         eina_stringshare_del(diff->redo.args.type_ssdsd.s1);
         eina_stringshare_del(diff->redo.args.type_ssdsd.s2);
         eina_stringshare_del(diff->redo.args.type_ssdsd.s4);
         break;
      case FUNCTION_TYPE_STRING_STRING_DOUBLE_INT_INT_INT_INT:
         eina_stringshare_del(diff->redo.args.type_ssdiiii.s1);
         eina_stringshare_del(diff->redo.args.type_ssdiiii.s2);
         break;
      case FUNCTION_TYPE_STRING_STRING_DOUBLE_UCHAR:
         eina_stringshare_del(diff->redo.args.type_ssduc.s1);
         eina_stringshare_del(diff->redo.args.type_ssduc.s2);
         break;
      case FUNCTION_TYPE_STRING_EDJEPARTTYPE:
         eina_stringshare_del(diff->redo.args.type_sept.s1);
         break;
      case FUNCTION_TYPE_STRING_EDJEPARTCOPY:
         eina_stringshare_del(diff->redo.args.type_sepc.s1);
         eina_stringshare_del(diff->redo.args.type_sepc.s2);
         break;
      case FUNCTION_TYPE_STRING_EDJETEXTEFFECT:
         eina_stringshare_del(diff->redo.args.type_sete.s1);
         break;
      case FUNCTION_TYPE_STRING_EVASEVENTFLAGS:
         eina_stringshare_del(diff->redo.args.type_seef.s1);
         break;
      case FUNCTION_TYPE_STRING_BOOL:
         eina_stringshare_del(diff->redo.args.type_sb.s1);
         break;
      case FUNCTION_TYPE_STRING_INT:
         eina_stringshare_del(diff->redo.args.type_si.s1);
         break;
      case FUNCTION_TYPE_STRING_STRING:
      case FUNCTION_TYPE_STRING_STRING_RENAME:
         eina_stringshare_del(diff->redo.args.type_ss.s1);
         eina_stringshare_del(diff->redo.args.type_ss.s2);
         break;
      case FUNCTION_TYPE_STRING_UINT:
         eina_stringshare_del(diff->redo.args.type_sui.s1);
         break;
      case FUNCTION_TYPE_STRING:
         eina_stringshare_del(diff->redo.args.type_s.s1);
         break;
      case FUNCTION_TYPE_STRING_STRING_EDJEASPECTCONTROL:
         eina_stringshare_del(diff->redo.args.type_sseac.s1);
         eina_stringshare_del(diff->redo.args.type_sseac.s2);
         break;
      case FUNCTION_TYPE_STRING_UINT_EDJEASPECTCONTROL:
         eina_stringshare_del(diff->redo.args.type_suieac.s1);
         break;
      case FUNCTION_TYPE_STRING_STRING_DOUBLE:
         eina_stringshare_del(diff->redo.args.type_ssd.s1);
         eina_stringshare_del(diff->redo.args.type_ssd.s2);
         break;
      case FUNCTION_TYPE_STRING_UINT_DOUBLE:
         eina_stringshare_del(diff->redo.args.type_suid.s1);
         break;
      case FUNCTION_TYPE_STRING_STRING_INT:
         eina_stringshare_del(diff->redo.args.type_ssi.s1);
         eina_stringshare_del(diff->redo.args.type_ssi.s2);
         break;
      case FUNCTION_TYPE_STRING_UINT_INT:
         eina_stringshare_del(diff->redo.args.type_suii.s1);
         break;
      case FUNCTION_TYPE_STRING_STRING_USHORT:
         eina_stringshare_del(diff->redo.args.type_ssus.s1);
         eina_stringshare_del(diff->redo.args.type_ssus.s2);
         break;
      case FUNCTION_TYPE_STRING_UINT_USHORT:
         eina_stringshare_del(diff->redo.args.type_suius.s1);
         break;
      case FUNCTION_TYPE_STRING_STRING_STRING:
         eina_stringshare_del(diff->redo.args.type_sss.s1);
         eina_stringshare_del(diff->redo.args.type_sss.s2);
         eina_stringshare_del(diff->redo.args.type_sss.s3);
         break;
      case FUNCTION_TYPE_STRING_UINT_UINT:
         eina_stringshare_del(diff->redo.args.type_suiui.s1);
         break;
      case FUNCTION_TYPE_STRING_UINT_STRING:
         eina_stringshare_del(diff->redo.args.type_suis.s1);
         eina_stringshare_del(diff->redo.args.type_suis.s3);
         break;
      case FUNCTION_TYPE_STRING_STRING_INT_INT_INT_INT:
         eina_stringshare_del(diff->redo.args.type_ssiiii.s1);
         eina_stringshare_del(diff->redo.args.type_ssiiii.s2);
         break;
      case FUNCTION_TYPE_STRING_EDJEEDITSELECTMODE:
         eina_stringshare_del(diff->redo.args.type_seesm.s1);
         break;
      case FUNCTION_TYPE_STRING_EDJEEDITENTRYMODE:
         eina_stringshare_del(diff->redo.args.type_seeem.s1);
         break;
      case FUNCTION_TYPE_STRING_EVASOBJECTPOINTERMODE:
         eina_stringshare_del(diff->redo.args.type_seopm.s1);
         break;
      case FUNCTION_TYPE_STRING_UCHAR:
         eina_stringshare_del(diff->redo.args.type_suc.s1);
         break;
      case FUNCTION_TYPE_STRING_EDJEACTIONTYPE:
         eina_stringshare_del(diff->redo.args.type_seat.s1);
         break;
      case FUNCTION_TYPE_STRING_EDJECHANNEL:
         eina_stringshare_del(diff->redo.args.type_sec.s1);
         break;
      case FUNCTION_TYPE_STRING_EDJETWEENMODE:
         eina_stringshare_del(diff->redo.args.type_setm.s1);
         break;
      case FUNCTION_TYPE_STRING_DOUBLE:
         eina_stringshare_del(diff->redo.args.type_sd.s1);
         break;
         /* Do not forget to clean stringshares */
         /* Don't add 'case default:'. Compiler should warn about new values in enum */
     }
   free(diff);
}
