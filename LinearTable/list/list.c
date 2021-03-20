#include <list.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define xassert(val)                                                      \
  do {                                                                    \
    if (val) {                                                            \
      fprintf(stderr,"XAssert failed with value %x(%d)!\nExiting with code -1\n", val, val); \
      exit(-1);                                                           \
    }                                                                     \
  } while(0)
#define throwif(x, text)                                                 \
  do {                                                                   \
    if (x) {                                                             \
      fprintf(stderr, "Meet exception at %s:%d:0:\t%s\n", __FILE__, __LINE__, text); \
      xassert(-1);                                                       \
    }                                                                    \
  } while (0)

List *InitList(List *li) {
  if (!li) {
    li = (List *) malloc(sizeof(List));
    throwif(!li, "Can not init List!");
    li->data = NULL;
    ClearList(li);
  }
  DestroyList(li);
  li->data = (ElemType *) malloc(sizeof(ElemType) * (LIST_DATA_SIZE + 1));
  return li;
}

void DestroyList(List *li) {
  if (!li) return;
  if (!li->data) {
    free(li->data);
  }
  ClearList(li);
}

void ClearList(List *li) { li->size = 0; }

u32 ListLength(List *li) { return li->size; }

ElemType GetElem(List *li, u32 i, ElemType *e) {
  throwif(i > li->size || i == 0, "Out of index!");
  *e = li->data[i + 1];
  return *e;
}

u32 LocateElem(List *li, ElemType e, int (*compare)(const ElemType *, const ElemType *)) {
  for (u32 i = 0; i < li->size; i++) {
    if (compare(li->data + i, &e)) return i;
  }
  return 0;
}

int ElemCompare(const ElemType *a, const ElemType *b) {
  if (!a || !b || (*a != *b)) return 0;
  return 1;
}

u8 PriorElem(List *li, ElemType cur_e, ElemType *pre_e) {
  u32 i = LocateElem(li, cur_e, ElemCompare);
  if (!i) return 1;
  *pre_e = 
  return 0;
}