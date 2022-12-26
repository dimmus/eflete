/**
 * Edje Theme Editor
 * Copyright (C) 2013-2014 Samsung Electronics.
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

#include <check.h>

#ifdef HAVE_CONFIG_H
   #include "eflete_config.h"
#endif /* include eflete_config.h */

#ifdef HAVE_ENVENTOR
#define ENVENTOR_BETA_API_SUPPORT
#include "Enventor.h"
#endif

// #define EFLETE_TEST(__testname) \
// void __testname (int _i CK_ATTRIBUTE_UNUSED) \
// { \
//   printf("\033[7;32m============================= "#__testname" ==================================================================\033[0m\n"); \
//   tcase_fn_start (""# __testname, __FILE__, __LINE__);

// #ifdef END_TEST
// #undef END_TEST
// #endif

// #define END_TEST \
//   printf("\n\n"); \
// }

# define EFLETE_TEST(TEST_NAME) START_TEST(TEST_NAME)
// # define EFLETE_END_TEST END_TEST

#define M_ mark_point();
