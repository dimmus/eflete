/**
 *  Getting first object from project. Needed to access top-level blocks
 */
#define GET_OBJ(project, edje_object) \
   Eina_Inlist *__styles, *__groups, *__widgets = NULL; \
   Widget *__widget; \
   Style *__style; \
   Group *__group; \
   __widgets = project->widgets; \
   if (!__widgets) edje_object = NULL; \
   else\
     { \
         __widget = EINA_INLIST_CONTAINER_GET(__widgets, Widget); \
         __styles = __widget->styles; \
         if (!__styles) edje_object = NULL; \
         else \
           { \
               __style = EINA_INLIST_CONTAINER_GET(__styles, Style); \
               __groups = __style->groups; \
               if (!__groups) edje_object = NULL; \
               else\
                 { \
                     __group = EINA_INLIST_CONTAINER_GET(__groups, Group); \
                     edje_object = __group->obj; \
                 } \
           } \
     }
