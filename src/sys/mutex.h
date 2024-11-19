#ifndef SYS_MUTEX_H
#define SYS_MUTEX_H

#include "panic.h"
#include "zero.h"

#include <pthread.h>

namespace sys
{

struct mutex;

struct mutex_lock
{
  mutex_lock();
  ~mutex_lock();

  mutex_lock(const mutex_lock& _copy);
  mutex_lock& operator=(const mutex_lock& _other);

private:
  friend struct mutex;

  mutable ptr<mutex> m_mutex;

};

struct mutex : sys::base
{
  mutex();
  ~mutex();

  mutex_lock lock();

private:
  friend struct mutex_lock;

  mutable pthread_mutex_t m_lock;

  mutex(const mutex& _copy);
  mutex& operator=(const mutex& _other);
};

}

// Implementation

namespace sys
{

mutex::mutex()
{
  if(pthread_mutex_init(&m_lock, NULL))
  {
    panic("Failed to initialize mutex");
  }
}

mutex::~mutex()
{
  // TODO: sys::base destructor checks lifetime (mutex_lock). This happens before that.
  //       Conveniently, this will also fail if observers present.

  if(pthread_mutex_destroy(&m_lock))
  {
    panic("Failed to destroy mutex");
  }
}

mutex_lock mutex::lock()
{
  pthread_mutex_lock(&m_lock);

  mutex_lock rtn;
  rtn.m_mutex = this;

  return rtn;
}


mutex_lock::mutex_lock() { }

mutex_lock::~mutex_lock()
{
  if(m_mutex)
  {
    pthread_mutex_unlock(&m_mutex->m_lock);
  }
}

/********************************************************************
 * Copy constructor
 *
 * Copy across the lock status and mutex pointer whilst also
 * stripping them away from the original instance.
 ********************************************************************/
mutex_lock::mutex_lock(const mutex_lock& _copy)
  : m_mutex(_copy.m_mutex)
{
  _copy.m_mutex = NULL;
}

/********************************************************************
 * Assignment operator
 *
 * Copy across the lock status and mutex pointer whilst also
 * stripping them away from the original instance.
 ********************************************************************/
mutex_lock& mutex_lock::operator=(const mutex_lock& _other)
{
  mutex_lock tmp(*this);

  m_mutex = _other.m_mutex;
  _other.m_mutex = NULL;

  return *this;
}

}

#endif

