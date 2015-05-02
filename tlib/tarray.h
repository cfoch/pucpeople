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

#endif /* __T_ARRAY_H__ */
