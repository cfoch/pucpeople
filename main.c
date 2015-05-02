#include <stdio.h>
#include <getopt.h>

#include "tlib.h"
#include "people.h"

void
display_help ()
{
  printf (
    "Usage: pucpeople [options]\n"
    "Options: \n"

    "\t--file (-f)"
    "\t\t\t\t"
    "A file path with the data separated by a delimiter"
    "\n"

    "\t--merge-sort (-m)"
    "\t\t\t\t"
    "Sort data using merge sort"
    "\n"

    "\t--insertion-sort (-i)"
    "\t\t\t\t"
    "Sort data using insertion sort"
    "\n"

    "\t--desc (-d)"
    "\n"

    "\t--asc (-a)"
    "\n"

    "\t--priority (-p) [father-last-name | mother-last-name | city | year | first-name]"
    "\n"
  );
}

int
main (int argc, char ** argv)
{
  TArray *people;
  TArray *priority_cmp_funcs;
  TBoolean asc;

  priority_cmp_funcs = t_array_new ();
  asc = TRUE;

  if (!argv[1])
    goto error_no_args;

  while (1) {
    static struct option long_options[] = {
      {"help", no_argument, NULL, 'h'},
      {"file", required_argument, NULL, 'f'},
      {"desc", no_argument, NULL, 'd'},
      {"asc", no_argument, NULL, 'a'},
      {"merge-sort", no_argument, NULL, 'm'},
      {"insertion-sort", no_argument, NULL, 'i'},
      {"priority", required_argument, NULL, 'p'},
      {NULL, 0, NULL, 0}
    };
    int option_index = 0, c;
    c = getopt_long (argc, argv, "f:hadmip:", long_options, &option_index);
    if (c == -1)
      break;

    switch (c) {
      case 'f':
        people = people_from_file (optarg, ",");
        if (!people)
          goto file_not_found;
        break;
      case 'p':
      {
        int index = optind - 1;
        while (index < argc) {
          tchar *sort_by;

          sort_by = argv[index];
          if (strcmp (sort_by, "year") == 0)
            t_array_append (priority_cmp_funcs, people_cmp_by_year);
          else if (strcmp (sort_by, "father-last-name") == 0)
            t_array_append (priority_cmp_funcs, people_cmp_by_father_last_name);
          else if (strcmp (sort_by, "mother-last-name") == 0)
            t_array_append (priority_cmp_funcs, people_cmp_by_mother_last_name);
          else if (strcmp (sort_by, "city") == 0)
            t_array_append (priority_cmp_funcs, people_cmp_by_city);
          else if (strcmp (sort_by, "first-name") == 0)
            t_array_append (priority_cmp_funcs, people_cmp_by_first_name);
          else
            break;

          index++;
        }
        optind = index - 1;
        break;
      }
      case 'd':
        asc = FALSE;
        break;
      case 'a':
        break;
      case 'm':
        if (asc)
          t_array_merge_sort_with_data (people, people_cmp_with_priority,
              priority_cmp_funcs);
        else
          t_array_merge_sort_with_data (people, people_cmp_with_priority_rev,
              priority_cmp_funcs);
        break;
      case 'i':
        if (asc)
          t_array_insertion_sort_with_data (people, people_cmp_with_priority,
              priority_cmp_funcs);
        else
          t_array_insertion_sort_with_data (people,
              people_cmp_with_priority_rev, priority_cmp_funcs);
        break;
      case 'h':
        display_help ();
        return TRUE;
        break;
      default:
        abort ();
    }
  }

  t_array_foreach (people, persona_beautiful_print, NULL);


  return TRUE;

error_no_args:
  free (priority_cmp_funcs);
  display_help ();
  return FALSE;

file_not_found:
  free (priority_cmp_funcs);
  printf ("File was not found\n");
  return FALSE;
}
