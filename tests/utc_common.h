
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
