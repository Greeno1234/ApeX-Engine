#ifndef SYS_ATOMIC_H
#define SYS_ATOMIC_H

#include "Zero.h"
#include "panic.h"
#include "Mutex.h"

namespace sys
{

template <typename T>
struct Atomic
{
  Atomic();

  operator T() const;

  Atomic& operator++();
  Atomic& operator--();

private:
  Zero<T> m_value;
  mutable Mutex m_mutex;
  Zero<bool> m_active;

  Atomic(const Atomic& _copy);
  Atomic& operator=(const Atomic& _other);
};

template <typename T>
Atomic<T>::Atomic() { m_active = true; }

template <typename T>
Atomic<T>::operator T() const
{
  MutexLock ml;
  if(m_active) ml = m_mutex.lock();

  return m_value;
}

template <typename T>
Atomic<T>& Atomic<T>::operator++()
{
  MutexLock ml;
  if(m_active) ml = m_mutex.lock();
  ++m_value;

  return *this;
}

template <typename T>
Atomic<T>& Atomic<T>::operator--()
{
  MutexLock ml;
  if(m_active) ml = m_mutex.lock();
  --m_value;

  return *this;
}

}

#endif

