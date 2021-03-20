#include <stdint.h>
#include <stdio.h>

typedef void (*myown_call)(void);

extern myown_call _myown_start;
extern myown_call _myown_end;

#define _init __attribute__((unused, section(".myown")))
#define func_init(func) myown_call _fn_##func _init = func

static void mspec1(void) { write(1, "aha!\n", 5); }

static void mspec2(void) { write(1, "aloha!\n", 7); }

static void mspec3(void) { write(1, "hello!\n", 7); }

func_init(mspec1);
func_init(mspec2);
func_init(mspec3);

/* exactly like below:
static myown_call mc1  __attribute__((unused, section(".myown"))) = mspec1;
static myown_call mc2  __attribute__((unused, section(".myown"))) = mspec2;
static myown_call mc3  __attribute__((unused, section(".myown"))) = mspec3;
*/

void do_initcalls(void) {
  myown_call *call_ptr = &_myown_start;
  do {
    fprintf(stderr, "call_ptr: %p\n", call_ptr);
    (*call_ptr)();
    ++call_ptr;
  } while (call_ptr < &_myown_end);
}

int main(void) {
  do_initcalls();
  return 0;
}
