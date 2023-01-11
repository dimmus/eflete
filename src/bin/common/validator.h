#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "eflete.h"
#include "resource_manager2.h"

typedef struct _Resource_Name_Validator Resource_Name_Validator;

void
resource_name_validator_list_set(Resource_Name_Validator *validator, Eina_List **list, Eina_Bool sorted);
void
resource_name_validator_resource_set(Resource_Name_Validator *validator, Resource2 *resource);
Resource_Name_Validator *
resource_name_validator_new(const char *pattern, const char *sig);
void
resource_name_validator_free(Resource_Name_Validator *validator);
Elm_Regexp_Status
resource_name_validator_status_get(Resource_Name_Validator *validator);
void
resource_name_validator_helper(void *data, const Efl_Event *event);

#endif /* VALIDATOR_H */
