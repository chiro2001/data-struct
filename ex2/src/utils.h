//
// Created by Chiro on 2021/4/3.
//

#ifndef STUDENT_LINKED_LIST_UTILS_H
#define STUDENT_LINKED_LIST_UTILS_H

#include <stdlib.h>
#include <stdio.h>

//#define DEBUG

#define LOGFMT "[%-15s][Line:%-4d] "

#ifdef DEBUG
#define PINT(x) do {                                                 \
  printf(LOGFMT #x " = %d\n", __func__, __LINE__, x); \
  fflush(stdout);                                                    \
} while (0)
#else
#define PINT(x)
#endif

#ifdef DEBUG
#define LOG(x) do {                                                  \
  printf(LOGFMT #x "\n", __func__, __LINE__);         \
  fflush(stdout);                                                    \
} while (0)
#else
#define LOG(x)
#endif

#ifdef DEBUG
#define LOGS(x) do {                                                 \
  printf(LOGFMT "%s\n", __func__, __LINE__, x);          \
  fflush(stdout);                                                    \
} while (0)
#else
#define LOGS(x)
#endif

#define myassert(x) do { \
  if (!x) {             \
    fprintf(stderr, "MAssert failed with " #x " = 0x%X!\nExiting with code(1)...\n", (int)((size_t)(x))); \
    exit(1);            \
  }                     \
} while (0)

#define myassertM(x, message) do { \
  if (!x) {             \
    fprintf(stderr, "MAssert failed with " #x " = 0x%X!\nError message: %s\nExiting with code(1)...\n", (int)((size_t)(x)), message); \
    exit(1);            \
  }                     \
} while (0)

#endif //STUDENT_LINKED_LIST_UTILS_H
