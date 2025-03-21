/* Elementary Widgets Extension
 * Copyright (C) 2014 Samsung Electronics.
 *
 * This file is part of Elementary Widgets Extension.
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
#ifndef EWE_PRIVATE_H
#define EWE_PRIVATE_H

#ifdef EAPI
# undef EAPI
#endif

#ifdef _WIN32
# ifdef EFL_BUILD
#  ifdef DLL_EXPORT
#   define EAPI __declspec(dllexport)
#  else
#   define EAPI
#  endif
# else
#  define EAPI __declspec(dllimport)
# endif
#else
# ifdef __GNUC__
#  if __GNUC__ >= 4
#   define EAPI __attribute__ ((visibility("default")))
#  else
#   define EAPI
#  endif
# else
#  define EAPI
# endif
#endif

#ifdef __cplusplus
extern "C" {
#endif


#define EFL_CANVAS_OBJECT_PROTECTED
#define EFL_CANVAS_GROUP_PROTECTED
#define EFL_CANVAS_GROUP_BETA

// #ifdef HAVE_CONFIG_H
// #include "eflete_config.h"
// #endif

#include <math.h>
#include <stdlib.h>

#include <Eo.h>
#include <Evas.h>
#include <Efl_Ui.h>
#include <Elementary.h>

// #include <elm_widget.h>
// #include <elm_widget_layout.h>
// #include <elm_interface_scrollable.h>

#include "Ewe.h"
#include "logger.h"

/* do not allow unsafe sprintf. use snprintf instead */
#pragma GCC poison sprintf

#ifdef SHOW_TODO
   #define DO_PRAGMA(x) _Pragma (#x)
   #define TODO(x) DO_PRAGMA(message ("TODO - " #x))
#else
   #define TODO(x)
#endif

#define EWE_SAFE_FREE(_h, _fn) do { _fn((void*)_h); _h = NULL; } while (0)

// for group add/del functions
EOAPI void efl_canvas_group_add(Eo *obj);
EOAPI void efl_canvas_group_del(Eo *obj);

#define EFL_CANVAS_GROUP_DEL_OPS(kls) EFL_OBJECT_OP_FUNC(efl_canvas_group_del, _##kls##_efl_canvas_group_group_del)
#define EFL_CANVAS_GROUP_ADD_OPS(kls) EFL_OBJECT_OP_FUNC(efl_canvas_group_add, _##kls##_efl_canvas_group_group_add)
#define EFL_CANVAS_GROUP_ADD_DEL_OPS(kls) EFL_CANVAS_GROUP_ADD_OPS(kls), EFL_CANVAS_GROUP_DEL_OPS(kls)

#ifdef __cplusplus
}
#endif

#undef EAPI
#define EAPI

#endif /* EWE_PRIVATE_H */
