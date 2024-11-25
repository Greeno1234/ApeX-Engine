#include "Debug.h"

namespace rend
{

sys::zero<bool> Debug::m_enable;

void Debug::enable(bool _enable)
{
  m_enable = _enable;
}

void Debug::out(const sys::string& _message)
{
  if(m_enable)
  {
    sys::out("Debug: " + _message + "\n");
  }
}

}

