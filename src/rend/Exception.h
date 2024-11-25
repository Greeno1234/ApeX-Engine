#include <sys/exception.h>
#include <sys/string.h>

namespace rend
{

struct Exception : sys::exception
{
  Exception(const sys::string& _message)
    : sys::exception(_message)
  { }
};

}

