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
