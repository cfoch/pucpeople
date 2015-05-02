#include <stdio.h>
#include <stdlib.h>

#include "people.h"

#define PERSONA(p)                                ((Persona *) (p))

int
people_cmp_by_first_name (tpointer a, tpointer b)
{
  return strcmp (PERSONA (a)->first_name, PERSONA (b)->first_name);
}

int
people_cmp_by_father_last_name (tpointer a, tpointer b)
{
  return strcmp (PERSONA (a)->father_last_name, PERSONA (b)->father_last_name);
}

int
people_cmp_by_mother_last_name (tpointer a, tpointer b)
{
  return strcmp (PERSONA (a)->mother_last_name, PERSONA (b)->mother_last_name);
}

int
people_cmp_by_year (tpointer a, tpointer b)
{
  return PERSONA (a)->year - PERSONA (b)->year;
}

int
people_cmp_by_city (tpointer a, tpointer b)
{
  return strcmp (PERSONA (a)->city, PERSONA (b)->city);
}

int
people_cmp_with_priority (tpointer a, tpointer b, tpointer ptr_cmp_funcs)
{
  int i;
  int maybe_ret;
  TBoolean stop_me = FALSE;
  TArray *cmp_funcs;

  cmp_funcs = (TArray *) ptr_cmp_funcs;
  for (i = 0; i < t_array_length (cmp_funcs) && (!stop_me); i++) {
    TCompFunc cmp_func;

    cmp_func = (TCompFunc) t_array_index (cmp_funcs, i);
    maybe_ret = cmp_func (a, b);
    stop_me = maybe_ret != 0;
  }
  return maybe_ret;
}

int
people_cmp_with_priority_rev (tpointer a, tpointer b, tpointer ptr_cmp_funcs)
{
  return -people_cmp_with_priority (a, b, ptr_cmp_funcs);
}

Persona *
persona_from_string (tchar * str, const char * delimiter)
{
  Persona *persona;
  char * pch;

  persona = malloc (sizeof (Persona));
  persona->first_name = strdup (strtok (str, delimiter));
  persona->father_last_name = strdup (strtok (NULL, delimiter));
  persona->mother_last_name = strdup (strtok (NULL, delimiter));
  persona->year = atoi (strtok (NULL, delimiter));
  persona->city = strdup (strtok (NULL, delimiter));

  return persona;
}

void
persona_beautiful_print (tpointer persona, tpointer user_data)
{
  printf ("%s\n", PERSONA (persona)->first_name);
  printf ("%s\n", PERSONA (persona)->father_last_name);
  printf ("%s\n", PERSONA (persona)->mother_last_name);
  printf ("%d\n", PERSONA (persona)->year);
  printf ("%s\n", PERSONA (persona)->city);
  printf ("\n");
}


TArray *
people_from_file (const char * filepath, const char * delimiter)
{
  TArray *people;
  FILE *f;
  tchar line[1000];

  people = t_array_new ();

  f = fopen (filepath, "r");

  if (!f)
    goto file_not_found;

  while (fscanf (f, " %[^\n]s", line) == 1) {
    Persona *persona;
    persona = persona_from_string (line, delimiter);
    t_array_append (people, persona);
  }
  return people;

file_not_found:
  free (people);
  return NULL;
}
