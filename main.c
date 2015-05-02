#include "tlib.h"
#include "people.h"

int
main ()
{
  TArray *people;
  TArray *priority_cmp_funcs;

  people = people_from_file ("input.txt", ",");

  priority_cmp_funcs = t_array_new ();
  t_array_append (priority_cmp_funcs, people_cmp_by_year);
  t_array_append (priority_cmp_funcs, people_cmp_by_father_last_name);
  t_array_append (priority_cmp_funcs, people_cmp_by_mother_last_name);
  t_array_append (priority_cmp_funcs, people_cmp_by_city);

  t_array_merge_sort_with_data (people, people_cmp_with_priority,
      priority_cmp_funcs);
  t_array_foreach (people, persona_beautiful_print, NULL);

  

  return TRUE;
}
