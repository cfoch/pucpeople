#include <stdio.h>

#include "tlib.h"

void
print (tpointer data, tpointer user_data)
{
  printf ("%d ", TPOINTER_TO_INT (data));
}

int
sort_asc (tpointer a, tpointer b)
{
  return TPOINTER_TO_INT (a) - TPOINTER_TO_INT (b);
}

TBoolean
main (int ** arg, char ** argv)
{
  TArray *array1, *array2;

  printf ("EXAMPLE 1: merge_sort\n");
  array1 = t_array_new ();

  t_array_append (array1, INT_TO_TPOINTER (9));
  t_array_append (array1, INT_TO_TPOINTER (4));
  t_array_append (array1, INT_TO_TPOINTER (5));
  t_array_append (array1, INT_TO_TPOINTER (8));
  t_array_append (array1, INT_TO_TPOINTER (1));
  t_array_append (array1, INT_TO_TPOINTER (42));

  t_array_foreach (array1, print, NULL);
  printf ("\n");

  t_array_merge_sort (array1, sort_asc);
  t_array_foreach (array1, print, NULL);
  printf ("\n");

  printf ("EXAMPLE 2: insertion_sort \n");
  array2 = t_array_new ();

  t_array_append (array2, INT_TO_TPOINTER (3));
  t_array_append (array2, INT_TO_TPOINTER (4));
  t_array_append (array2, INT_TO_TPOINTER (0));
  t_array_append (array2, INT_TO_TPOINTER (1));
  t_array_append (array2, INT_TO_TPOINTER (9));
  t_array_append (array2, INT_TO_TPOINTER (2));

  t_array_foreach (array2, print, NULL);
  printf ("\n");

  t_array_insertion_sort (array2, sort_asc);
  t_array_foreach (array2, print, NULL);
  printf ("\n");


  return TRUE;
}
