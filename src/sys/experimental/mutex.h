#ifndef SYS_MUTEX_H
#define SYS_MUTEX_H

#include "panic.h"
#include "Zero.h"

#include <pthread.h>

namespace sys
{

struct Mutex;

struct MutexLock
{
  MutexLock() { }

  ~MutexLock()
  {
    if(m_locked)
    {
      pthread_mutex_unlock(m_lock);
    }
  }

  /********************************************************************
   * Copy constructor
   *
   * Copy across the lock status and mutex pointer whilst also
   * stripping them away from the original instance.
   ********************************************************************/
  MutexLock(const MutexLock& _copy) :
    m_lock(_copy.m_lock),
    m_locked(_copy.m_locked)
  {
    _copy.m_locked = false;
    _copy.m_lock = NULL;
  }

  /********************************************************************
   * Assignment operator
   *
   * Copy across the lock status and mutex pointer whilst also
   * stripping them away from the original instance.
   ********************************************************************/
  MutexLock& operator=(const MutexLock& _other)
  {
    MutexLock tmp(*this);

    m_lock = _other.m_lock;
    m_locked = _other.m_locked;

    _other.m_locked = false;
    _other.m_lock = NULL;

    return *this;
  }

private:
  friend struct Mutex;

  mutable Zero<pthread_mutex_t*> m_lock;
  mutable Zero<bool> m_locked;

};

struct Mutex
{
  Mutex()
  {
    if(pthread_mutex_init(&m_lock, NULL))
    {
      panic("Failed to initialize mutex");
    }
  }

  ~Mutex()
  {
    if(pthread_mutex_destroy(&m_lock))
    {
      panic("Failed to destroy mutex");
    }
  }

  MutexLock lock() const
  {
    pthread_mutex_lock(&m_lock);

    MutexLock rtn;
    rtn.m_lock = &m_lock;
    rtn.m_locked = true;

    return rtn;
  }

private:
  mutable pthread_mutex_t m_lock;

  Mutex(const Mutex& _copy);
  Mutex& operator=(const Mutex& _other);
};

}

#endif

