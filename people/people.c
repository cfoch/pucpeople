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

TBoolean
persona_eq (tpointer pers1, tpointer target, tpointer crits)
{
  Persona *p1, *p2;
  TArray * criterias;
  int i;
  TBoolean eq = TRUE;

  p1 = PERSONA (pers1);
  p2 = PERSONA (target);
  criterias = (TArray *) crits;

  for (i = 0; i < t_array_length (criterias) && eq; i++) {
    switch (TPOINTER_TO_INT (t_array_index (criterias, i))) {
      case PERSONA_FIRST_NAME:
        eq = eq && (strcmp (p1->first_name, p2->first_name) == 0);
        break;
      case PERSONA_FATHER_LAST_NAME:
        eq = eq && (strcmp (p1->father_last_name, p2->father_last_name) == 0);
        break;
      case PERSONA_MOTHER_LAST_NAME:
        eq = eq && (strcmp (p1->mother_last_name, p2->mother_last_name) == 0);
        break;
      case PERSONA_YEAR:
        eq = eq && (p1->year == p2->year); /* eq = eq && ... */
        break;
      case PERSONA_CITY:
        eq = eq && (strcmp (p1->city, p2->city) == 0);
        break;
    }
  }
  return eq;
}

int
persona_cmp_binary (tpointer pers1, tpointer target, tpointer crits)
{
  Persona *p1, *p2;
  TArray * criterias;
  int i;
  int ret = 0;

  p1 = PERSONA (pers1);
  p2 = PERSONA (target);
  criterias = (TArray *) crits;

  for (i = 0; i < t_array_length (criterias) && (ret == 0); i++) {
    switch (TPOINTER_TO_INT (t_array_index (criterias, i))) {
      case PERSONA_FIRST_NAME:
        ret = strcmp (p2->first_name, p1->first_name);
        break;
      case PERSONA_FATHER_LAST_NAME:
        ret = strcmp (p2->father_last_name, p1->father_last_name);
        break;
      case PERSONA_MOTHER_LAST_NAME:
        ret = strcmp (p2->mother_last_name, p1->mother_last_name);
        break;
      case PERSONA_YEAR: {
        //printf ("comparando year: %d vs %d\n", p2->year, p1->year);
        ret =  p2->year - p1->year;
        break;
      }
      case PERSONA_CITY:
        ret = strcmp (p2->city, p1->city);
        break;
    }
  }

  return ret;
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
  int i;
  char *field[5];

  for (i = 0; i < 5; i++) {
    if (i == 0)
      field[i] = strdup (strtok (str, delimiter));
    else
      field[i] = strtok (NULL, delimiter);
    if (!field[i])
      return NULL;
    if (i == 4)
      field[i][strlen (field[i]) - 2] = '\0';
    field[i] = strdup (field[i]);
  }

  persona = malloc (sizeof (Persona));
  persona->first_name = strdup (field[0]);
  persona->father_last_name = strdup (field[1]);
  persona->mother_last_name = strdup (field[2]);
  persona->year = atoi (field[3]);
  persona->city = strdup (field[4]);

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

void
persona_ugly_print (tpointer persona, tpointer user_data)
{
  printf ("%s,%s,%s,%d,%s",
      PERSONA (persona)->first_name,
      PERSONA (persona)->father_last_name,
      PERSONA (persona)->mother_last_name,
      PERSONA (persona)->year,
      PERSONA (persona)->city);
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
    goto handle_error;

  while (fgets (line, 1000, f)) {
    Persona *persona;
    persona = persona_from_string (line, delimiter);
    if (!persona)
      goto handle_error;
    t_array_append (people, persona);
  }
  fclose (f);

  if (people->len == 0)
    goto handle_error;

  return people;

handle_error:
  free (people);
  return NULL;
}
