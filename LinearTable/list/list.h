#include <stdint.h>
#define LIST_DATA_SIZE 1024
typedef int ElemType;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;

typedef struct {
  ElemType *data;
  u32 size;
} List;

List *InitList(List *li);
void DestroyList(List *li);
void ClearList(List *li);
u32 ListLength(List *li);
ElemType GetElem(List *li, u32 i, ElemType *e);
u32 LocateElem(List *li, ElemType e, int (*compare)(const ElemType *, const ElemType *));
int ElemCompare(const ElemType *a, const ElemType *b);
u8 PriorElem(List *li, ElemType cur_e, ElemType *pre_e);