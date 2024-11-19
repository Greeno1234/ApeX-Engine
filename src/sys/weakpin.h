#ifndef SYS_REFDATA_H
#define SYS_REFDATA_H

#include "config.h"
#include "zero.h"
#include "criticalpin.h"

namespace sys
{

struct weak_pin
{
  struct data
  {
    zero<size_t> m_weakcount;
    zero<size_t> m_strongcount;
#ifdef SYS_DEBUG
    critical_pin m_criticalpin;
#endif
  };

  struct lock
  {
    lock();
    lock(const lock& _copy);
    lock& operator=(const lock& _assign);
    ~lock();

    lock(const weak_pin& _weakpin);

#ifdef SYS_DEBUG
    critical_pin::lock critical_pin_lock() const;
#endif

    void assert_valid() const;
    bool valid() const;

  private:
    zero<data*> m_data;
  };

  weak_pin();
  weak_pin(const weak_pin& _copy);
  weak_pin& operator=(const weak_pin& _other);
  ~weak_pin();

  void reset() const;

#ifdef SYS_DEBUG
  critical_pin::lock critical_pin_lock() const;
#endif

private:
  mutable zero<data*> m_data;

};

}

/* Implementation */

#include "panic.h"

namespace sys
{

inline weak_pin::weak_pin()
{ }

inline weak_pin::weak_pin(const weak_pin& _copy)
{ }

inline weak_pin& weak_pin::operator=(const weak_pin& _other)
{
  return *this;
}

inline weak_pin::~weak_pin()
{
  reset();
}

inline void weak_pin::reset() const
{
  if(!m_data) return;

  --m_data->m_strongcount;

  if(m_data->m_strongcount)
  {
    panic("Reference count not isolated");
  }

#ifdef SYS_DEBUG
  m_data->m_criticalpin.reset();
#endif

  // Delete if no observers
  if(!m_data->m_weakcount)
  {
    delete m_data.get();
  }

  m_data = NULL;
}

#ifdef SYS_DEBUG
inline critical_pin::lock weak_pin::critical_pin_lock() const
{
  return weak_pin::lock(*this).critical_pin_lock();
}
#endif

inline weak_pin::lock::lock() { }

inline weak_pin::lock::lock(const lock& _copy)
  : m_data(_copy.m_data)
{
  if(m_data) ++m_data->m_weakcount;
}

inline weak_pin::lock& weak_pin::lock::operator=(const lock& _assign)
{
  weak_pin::lock tmp(*this);

  if(m_data) --m_data->m_weakcount;
  m_data = _assign.m_data;
  if(m_data) ++m_data->m_weakcount;

  return *this;
}

inline weak_pin::lock::~lock()
{
  if(!m_data) return;

  --m_data->m_weakcount;

  if(m_data->m_weakcount || m_data->m_strongcount) return;
  delete m_data.get();
}

inline weak_pin::lock::lock(const weak_pin& _weakpin)
  : m_data(_weakpin.m_data)
{
  if(!m_data)
  {
    _weakpin.m_data = new data();
    m_data = _weakpin.m_data;
    ++m_data->m_strongcount;
  }

  ++m_data->m_weakcount;
}

inline bool weak_pin::lock::valid() const
{
  if(!m_data)
  {
    return false;
  }

  if(!m_data->m_strongcount)
  {
    return false;
  }

  return true;
}

inline void weak_pin::lock::assert_valid() const
{
  if(!valid())
  {
    panic("Attempt to access invalidated memory");
  }
}

#ifdef SYS_DEBUG
inline critical_pin::lock weak_pin::lock::critical_pin_lock() const
{
  if(!m_data) return critical_pin::lock();

  return critical_pin::lock(m_data->m_criticalpin);
}
#endif

}

#endif

