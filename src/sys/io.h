#ifndef SYS_IO_H
#define SYS_IO_H

//#include "String.h"

#include <stdio.h>

namespace sys
{

/* out */

inline void out(const char* _str)
{
  fprintf(stdout, "%s", _str);
}

/*
inline void out(const String& _str)
{
  fprintf(stdout, "%s", _str.unsafe_raw());
}
*/

/* err */

inline void err(const char* _str)
{
  fprintf(stderr, "%s", _str);
}

/*
inline void err(const String& _str)
{
  fprintf(stderr, "%s", _str.unsafe_raw());
}
*/

}

#endif

