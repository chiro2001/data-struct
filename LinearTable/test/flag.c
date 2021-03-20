#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef void (*callback)(void);

typedef struct {
  size_t magic1;
  size_t magic2;
  const char *name;
  callback fn;
} callback_t;

#define CALLBACK __attribute__((section(".callback"), used))

CALLBACK callback_t flag;
// __attribute__((section(".callback"), used)) callback_t flag;
#define MAGIC1 0xa0a0a0a0
#define MAGIC2 0x0a0a0a0a

// 以下两个变量用于记录段起始结束地址
callback_t *start = &flag;
callback_t *end = &flag;

__attribute__((constructor)) void callback_init() {
  puts("constructor");
  while (1) {
    callback_t *tmp = start - 1;
    if (tmp->magic1 == MAGIC1 && tmp->magic2 == MAGIC2) {
      start--;
    } else {
      break;
    }
  }
  while (1) {
    callback_t *tmp = end + 1;
    if (tmp->magic1 == MAGIC1 && tmp->magic2 == MAGIC2) {
      end++;
    } else {
      break;
    }
  }
}

void do_callback(const char *name) {
  size_t i;
  callback_t *tmp = start;

  while (tmp != end) {
    // 注意跳过flag哦
    if (tmp != &flag && !strcmp(tmp->name, name)) {
      return tmp->fn();
    }
    tmp++;
  }
}

void test(void) CALLBACK {
  puts("TEST called!");
}

int main() {
  do_callback("test");
}