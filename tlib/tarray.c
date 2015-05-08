#include "tarray.h"

#define T_ARRAY(t)           (TArray *) (t)

#define _t_alloc_array()     malloc(sizeof (TArray))
#define _t_realloc_vector(a, sz)   t_alloc(a,tpointer,sz)

TArray *
t_array_new ()
{
  TArray *array;

  array = _t_alloc_array ();
  array->len = 0;
  array->vector = NULL;

  return array;
}

void
t_array_set_free_func (TArray * array, TFunc free_func)
{
  array->free_func = free_func;
}

void
t_array_foreach (TArray * array, TFunc func, tpointer user_data)
{
  int i;
  for (i = 0; i < array->len; i++)
    func (array->vector[i], user_data);
}

void
t_array_append (TArray * array, tpointer data)
{
  array->len++;
  array->vector = realloc (array->vector, sizeof (tpointer) * array->len);
  array->vector[array->len - 1] = data;  
}

void
t_array_free (TArray * array, tpointer user_data)
{
  t_array_foreach (array, array->free_func, user_data);
}

tpointer
t_array_index (TArray * array, int index)
{
  return array->vector[index];
}

int
t_array_length (TArray * array)
{
  return array->len;
}

void
t_array_remove_last (TArray * array)
{
  array->len--;
  array->vector = realloc (array, sizeof (tpointer) * array->len);
}

TArray *
t_array_filter2_with_data (TArray * array, tpointer target, TEqDataFunc eq_func,
    tpointer user_data)
{
  int i;
  TArray *ret;
  ret = t_array_new ();

  for (i = 0; i < t_array_length (array); i++) {
    tpointer elem;
    elem = t_array_index (array, i);
    if (eq_func (elem, target, user_data))
      t_array_append (ret, elem);
  }
  return ret;
}

TArray *
t_array_filter_with_data (TArray * array, tpointer target,
    TCompDataFunc cmp_func, tpointer user_data)
{
	TArray *ret;
	int start_index = 0, end_index = t_array_length (array) - 1, mid;
  tpointer elem;

	ret = t_array_new ();
	TBoolean found = FALSE;

	while ((start_index <= end_index) && (!found)) {
		mid = (start_index + end_index) / 2;
    elem = t_array_index (array, mid);

		if (cmp_func (elem, target, user_data) == 0)
			found = TRUE;
		else if (cmp_func (elem, target, user_data) > 0)
			start_index = mid + 1;
		else
			end_index = mid - 1;
	}

	if (found) {
		int i = mid - 1, j = mid + 1;

		t_array_append (ret, elem);
		elem = t_array_index (array, i);
		while ((i >= 0) && (cmp_func (elem, target, user_data) == 0)){
			t_array_append (ret, elem);
      i--;
			elem = t_array_index (array, i);
		}

		elem = t_array_index (array, j);
		while ((j <= t_array_length (array) - 1) &&
        (cmp_func (elem, target, user_data) == 0)){
			t_array_append (ret, elem);
      j++;
			elem = t_array_index (array, j);
		}
	}
	return ret;
}


static void
_t_array_merge (TArray * array, int start_index, int middle_index,
    int end_index, TCompFunc cmp_func)
{
  int i, j, k, len_dummy;
  tpointer * dummy;

  dummy = malloc (sizeof (tpointer) * (end_index - start_index + 1));

  i = start_index;
  j = middle_index + 1;
  len_dummy = 0;
  while ((i <= middle_index) && (j <= end_index))
    if (cmp_func (array->vector[i], array->vector[j]) > 0)
      dummy[len_dummy++] = array->vector[j++];
    else
      dummy[len_dummy++] = array->vector[i++];

  if (i > middle_index)
    for (k = j; k <= end_index; k++)
      dummy[len_dummy++] = array->vector[k];

  if (j > end_index)
    for (k = i; k <= middle_index; k++)
      dummy[len_dummy++] = array->vector[k];

  for (i = 0; i < len_dummy; i++)
    array->vector[i + start_index] = dummy[i];
}

static void
_t_array_merge_sort (TArray * array, int start_index, int end_index,
    TCompFunc cmp_func)
{
  int middle_index;
  if (start_index == end_index)
    return;
  middle_index = (start_index + end_index) / 2;
  _t_array_merge_sort (array, start_index, middle_index, cmp_func);
  _t_array_merge_sort (array, middle_index + 1, end_index, cmp_func);
  _t_array_merge (array, start_index, middle_index, end_index, cmp_func);
}

void
t_array_merge_sort (TArray * array, TCompFunc cmp_func)
{
  _t_array_merge_sort (array, 0, array->len - 1, cmp_func);
}

void
t_array_insertion_sort (TArray * array, TCompFunc cmp_func)
{
  int i, j;
  for (i = 1; i < array->len; i++) {
    j = i;
    while ((j > 0) && (cmp_func (array->vector[j], array->vector[j - 1]) < 0)) {
      t_swap (array->vector[j - 1], array->vector[j]);
      j--;
    }
  }
}

/* TODO: this is *really* ugly! Make me more beatiful */

static
_t_array_merge_with_data (TArray * array, int start_index, int middle_index,
    int end_index, TCompDataFunc cmp_func, tpointer cmp_data)
{
  int i, j, k, len_dummy;
  tpointer * dummy;

  dummy = malloc (sizeof (tpointer) * (end_index - start_index + 1));

  i = start_index;
  j = middle_index + 1;
  len_dummy = 0;
  while ((i <= middle_index) && (j <= end_index))
    if (cmp_func (array->vector[i], array->vector[j], cmp_data) > 0)
      dummy[len_dummy++] = array->vector[j++];
    else
      dummy[len_dummy++] = array->vector[i++];

  if (i > middle_index)
    for (k = j; k <= end_index; k++)
      dummy[len_dummy++] = array->vector[k];

  if (j > end_index)
    for (k = i; k <= middle_index; k++)
      dummy[len_dummy++] = array->vector[k];

  for (i = 0; i < len_dummy; i++)
    array->vector[i + start_index] = dummy[i];
}

static void
_t_array_merge_sort_with_data (TArray * array, int start_index, int end_index,
    TCompDataFunc cmp_func, tpointer cmp_data)
{
  int middle_index;
  if (start_index == end_index)
    return;
  middle_index = (start_index + end_index) / 2;
  _t_array_merge_sort_with_data (array, start_index, middle_index, cmp_func,
      cmp_data);
  _t_array_merge_sort_with_data (array, middle_index + 1, end_index, cmp_func,
      cmp_data);
  _t_array_merge_with_data (array, start_index, middle_index, end_index,
      cmp_func, cmp_data);
}

void
t_array_merge_sort_with_data (TArray * array, TCompDataFunc cmp_func,
    tpointer cmp_data)
{
  _t_array_merge_sort_with_data (array, 0, array->len - 1, cmp_func, cmp_data);
}

void
t_array_insertion_sort_with_data (TArray * array, TCompDataFunc cmp_func,
    tpointer cmp_data)
{
  int i, j;
  for (i = 1; i < array->len; i++) {
    j = i;
    while ((j > 0) &&
      (cmp_func (array->vector[j], array->vector[j - 1], cmp_data) < 0)) {
      t_swap (array->vector[j - 1], array->vector[j]);
      j--;
    }
  }
}

/* TIM SORT */

static int
_calc_minrun(int n){
    int r = 0;
    while (n >= 64){
      r |= n & 1;
    n >>= 1;
  }
  return n + r;
}


static void
print (tpointer data, tpointer user_data)
{
  printf ("%d ", TPOINTER_TO_INT (data));
}


void
t_array_insert_sorted (TArray * array, tpointer element, TCompDataFunc cmp_func,
    tpointer user_data)
{
  /* FIXME
   * Use the right algorithm. This code sucks, but it is useful to
   * test the timsort quickly.
   */
   t_array_append (array, element);
   t_array_insertion_sort_with_data (array, cmp_func, user_data);
}

static TArray *
_buscar_run_with_data (TArray *array, int * i, int minrun,
    TCompDataFunc cmp_func, tpointer cmp_data)
{
  TArray *run;
  run = t_array_new ();
  printf ("Generando run\n");
  /*el ultimo elemento*/
  if (*i >= array->len - 1){
    t_array_append (run,array->vector[*i]);
    return run;
  }
    
  
  /*swap first and second*/
  if (cmp_func (array->vector[*i], array->vector[*i + 1], cmp_data) < 0)
    t_swap (array->vector[*i], array->vector[*i + 1]);
  
  /*add first & second*/
  t_array_append (run, array->vector[*i]);
  t_array_append (run, array->vector[*i + 1]);
  run->len = 2;
  (*i)++;        
  
  /*add*/
  while ((*i < array->len - 1) && (cmp_func(array->vector[*i],
      array->vector[*i + 1], cmp_data)>0)) {
    run->len++;
    t_array_append (run, array->vector[*i + 1]);
    (*i)++;
  }

  while ((*i < array->len - 1) && (run->len < minrun)) {
    t_array_insert_sorted(run, array->vector[*i + 1], cmp_func,
        cmp_data);
    (*i)++;
  }


  
  /*add*/
  while ((*i < array->len - 1) && (run->len >= minrun) &&
      (array->vector[*i+1] >= array->vector[*i])) {
    run->len++;
    t_array_append (run, array->vector[*i + 1]);
    (*i)++;
  }
  t_array_foreach (run, print, NULL);
  printf ("\n");
  printf ("i: %d\n", *i);
  printf (":p\n");
  return run;
}


static TArray *
_timsort_merge (TArray * a, TArray * b,TCompDataFunc cmp_func, tpointer cmp_data)
{
  int i, j, k, first_end, second_end;
  TArray *result;
  result = t_array_new();

  i = 0;
  j = 0;
  first_end = a->len;
  second_end = b->len;
  while ((i < first_end) && (j < second_end))
    if (cmp_func (a->vector[i], b->vector[j], cmp_data) > 0)
      t_array_append(result ,b->vector[j++]);
    else
      t_array_append(result ,a->vector[i++]);
    result->len++;

  if (i >= first_end)
    for (k = j; k < second_end; k++){
      t_array_append(result ,b->vector[k]);
      result->len++;
    }

  if (j >= second_end)
    for (k = i; k < first_end ; k++){
      t_array_append(result ,a->vector[k]);
      result->len++;
    }
  return result;
}

void
t_array_tim_sort_with_data (TArray * array, TCompDataFunc cmp_func,
    tpointer cmp_data)
{
  TArray *run_x, *run_y, *run_z;
  TArray *runs;
  int i = 0; /* indice para el array*/
  int minrun;
  
  if (array->len < 64) {
    t_array_insertion_sort_with_data (array,cmp_func, cmp_data);
    return;
  }

  minrun = _calc_minrun(array->len); /*Definido abajo*/

  runs = t_array_new ();
  
  while (i < array->len) {
    TArray *run;
    printf ("HOLA\n");
    run = _buscar_run_with_data (array, &i, minrun, cmp_func, cmp_data);
    t_array_append (runs, run);
    if (runs->len >= 3) {
      run_x = t_array_index (runs, 0); /*primero*/
      run_y = t_array_index (runs, 1); /*segundo*/
      run_z = t_array_index (runs, 2); /*tercero*/
      if (!(run_x->len > run_y->len + run_z->len && run_y->len > run_z->len)) {
        TArray *smaller, *larger, *merged;
        
        if (run_x->len < run_z->len) {
            smaller = run_x;
            larger = run_z;
        } else { 
            smaller = run_z;
            larger = run_x;
        }
        t_array_remove_last (runs);
        t_array_remove_last (runs);
        t_array_remove_last (runs);

        merged = _timsort_merge (run_y, smaller, cmp_func, cmp_data);
        t_array_append(runs, merged);
        t_array_append(runs, larger);
      }  else {
        TArray *merged;
        merged = _timsort_merge (run_x, run_y, cmp_func, cmp_data);
        t_array_remove_last (runs);
        t_array_remove_last (runs);
        t_array_remove_last (runs);
        t_array_append(runs, merged);
        t_array_append(runs, run_z);
      }
    }
  }

  printf (":)\n");

  t_array_remove_last (runs);
  t_array_remove_last (runs);
  run_x = t_array_index (runs, 0); /*primero*/
  run_y = t_array_index (runs, 1); /*segundo*/
  array->vector = _timsort_merge(run_x, run_y, cmp_func, cmp_data)->vector; /*mezcla final*/
}
