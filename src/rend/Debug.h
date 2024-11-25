#include <sys/string.h>
#include <sys/memory.h>

namespace rend
{

struct Debug
{
  static void enable(bool _enable);
  static void out(const sys::string& _message);

private:
  static sys::zero<bool> m_enable;
};

}

