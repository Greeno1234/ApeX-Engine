#ifndef SYS_PANIC_H
#define SYS_PANIC_H

#include <stdlib.h>
#include <stdio.h>

namespace sys
{

inline void panic(const char* _message)
{
  printf("Panic: %s\n", _message);
  abort();
}

}

#endif

