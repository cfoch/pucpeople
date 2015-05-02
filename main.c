#include "tlib.h"
#include "people.h"

int
main ()
{
  TArray *people;

  people = people_from_file ("input.txt", ",");
  t_array_foreach (people, persona_beautiful_print, NULL);

  return TRUE;
}
