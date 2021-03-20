#include <stdio.h>
#include <string.h>

#define LIST_ELEM_TYPE int
typedef LIST_ELEM_TYPE ElemType;
#define LIST_INIT_SIZE 1024

typedef struct List_t {
  ElemType *data;
  size_t size;
  // void bind()
} List;

List ListInit() {
  List li;
  li.size = 0;
  li.data = (List*)malloc(sizeof(ElemType) * LIST_INIT_SIZE);
  if (!li.data) return li;
  return li;
}

int main() {
  return 0;
}

