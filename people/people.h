#ifndef __PEOPLE_H__
#define __PEOPLE_H__

#include <string.h>
#include <stdarg.h>

#include "tlib.h"

struct _Persona {
  const tchar *first_name;
  const tchar *father_last_name;
  const tchar *mother_last_name;
  int year;
  const char * city;
};

typedef struct _Persona Persona;

int people_cmp_by_first_name (tpointer a, tpointer b);
int people_cmp_by_father_last_name (tpointer a, tpointer b);
int people_cmp_by_mother_last_name (tpointer a, tpointer b);
int people_cmp_by_year (tpointer a, tpointer b);
int people_cmp_by_city (tpointer a, tpointer b);
int people_cmp_with_priority (tpointer a, tpointer b, tpointer ptr_cmp_funcs);
Persona * persona_from_string (tchar * str, const char * delimiter);
void persona_beautiful_print (tpointer persona, tpointer user_data);
TArray * people_from_file (const char * filepath, const char * delimiter);
int people_cmp_with_priority_rev (tpointer a, tpointer b,
    tpointer ptr_cmp_funcs);

#endif
