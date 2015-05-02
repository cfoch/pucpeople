#ifndef __T_ARRAY_H__
#define __T_ARRAY_H__
#include "tlib.h"

struct _TArray
{
  tpointer *vector;
  int len;
  TFunc *free_func;
};

typedef struct _TArray TArray;

TArray * t_array_new ();
void t_array_foreach (TArray * array, TFunc func, tpointer user_data);
void t_array_append (TArray * array, tpointer data);
tpointer t_array_index (TArray * array, int index);
int t_array_length (TArray * array);
void t_array_merge_sort (TArray * array, TCompFunc cmp_func);
void t_array_insertion_sort (TArray * array, TCompFunc cmp_func);
void t_array_merge_sort_with_data (TArray * array, TCompDataFunc cmp_func,
    tpointer cmp_data);
void t_array_insertion_sort_with_data (TArray * array, TCompDataFunc cmp_func,
    tpointer cmp_data);

#endif /* __T_ARRAY_H__ */
