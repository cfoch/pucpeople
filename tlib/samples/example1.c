#include <stdio.h>
#include <assert.h>

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

int
sort_asc_with_data (tpointer a, tpointer b, tpointer user_data)
{
  return TPOINTER_TO_INT (a) - TPOINTER_TO_INT (b);
}

TBoolean
is_asc (TArray * array)
{
  int i;
  for (i = 0; i < t_array_length (array) - 1; i++) {
    if (TPOINTER_TO_INT (t_array_index (array, i)) >
        TPOINTER_TO_INT (t_array_index (array, i + 1))) {
      return FALSE;
    }
  }
  return TRUE;
}

TBoolean
main (int ** arg, char ** argv)
{
  TArray *array1, *array2, *array3;
  int i;

  srand (time (NULL));

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
  assert (is_asc (array1));

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
  assert (is_asc (array2));

  printf ("EXAMPLE 2: tim_sort \n");
  array3 = t_array_new ();

  for (i = 0; i < rand () % 10000; i++)
    t_array_append (array3, INT_TO_TPOINTER (rand () % 100));

  t_array_foreach (array3, print, NULL);
  printf ("\n");

  printf ("result: \n");
  t_array_tim_sort_with_data (array3, sort_asc_with_data, NULL);
  t_array_foreach (array3, print, NULL);
  printf ("\n");
  assert (is_asc (array3));

  return TRUE;
}
