#ifndef SYS_CRITICALPIN_H
#define SYS_CRITICALPIN_H

#include "zero.h"
#include "panic.h"
#include "config.h"

#ifdef SYS_DEBUG

namespace sys
{

struct critical_pin
{
  struct lock
  {
    lock();
    lock(const lock& _copy);
    lock& operator=(const lock& _assign);
    ~lock();

    lock(const critical_pin& _criticalpin);

  private:
    zero<const critical_pin*> m_criticalpin;

  };

  critical_pin();
  critical_pin(const critical_pin& _copy);
  critical_pin& operator=(const critical_pin& _assign);
  ~critical_pin();

  void reset() const;

private:
  mutable zero<int> m_lockcount;
};

}

/* Implementation */

namespace sys
{

inline critical_pin::critical_pin()
{ }

inline critical_pin::critical_pin(const critical_pin& _copy)
{ }

inline critical_pin& critical_pin::operator=(const critical_pin& _assign)
{
  return *this;
}

inline critical_pin::~critical_pin()
{
  reset();
}

inline void critical_pin::reset() const
{
  if(m_lockcount)
  {
    printf("Locks: %i\n", (int)m_lockcount);
    panic("Attempting to destruct locked memory");
  }
}

inline critical_pin::lock::lock() { }

inline critical_pin::lock::lock(const lock& _copy)
  : m_criticalpin(_copy.m_criticalpin)
{
  if(m_criticalpin) ++m_criticalpin->m_lockcount;
}

inline critical_pin::lock& critical_pin::lock::operator=(const lock& _assign)
{
  if(m_criticalpin) --m_criticalpin->m_lockcount;
  m_criticalpin = _assign.m_criticalpin;
  if(m_criticalpin) ++m_criticalpin->m_lockcount;

  return *this;
}

inline critical_pin::lock::~lock()
{
  if(m_criticalpin) --m_criticalpin->m_lockcount;
}

inline critical_pin::lock::lock(const critical_pin& _criticalpin)
  : m_criticalpin(&_criticalpin)
{
  if(m_criticalpin) ++m_criticalpin->m_lockcount;
}

}

#endif

#endif

