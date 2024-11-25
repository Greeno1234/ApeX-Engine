/*
 * TODO: (*iter)[9] ref_lock
 */

#ifndef SYS_LOCK_H
#define SYS_LOCK_H

#include "zero.h"
#include "config.h"

#ifdef SYS_DEBUG

namespace sys
{

/***************************************************************************
 * ptr_lock
 *
 * Hooks into the operator-> dereference and creates a wrapper object that
 * forwards through the T* but also increments a lock throughout the
 * lifespan of the object.
 ***************************************************************************/
template <typename T>
struct ptr_lock
{
  ptr_lock(T* _ptr, const critical_pin::lock& _lock);

  operator T*() const;

  T* operator->() const;

private:
  zero<T*> m_ptr;
  critical_pin::lock m_lock;
};

/***************************************************************************
 * ref_lock
 ***************************************************************************/
template <typename T>
struct ref_lock
{
  ref_lock(T* _ptr, const critical_pin::lock& _lock);

  operator T&();
  operator const T&() const;
  ref_lock& operator=(const T& _value);

  T operator+(const T& _value);

  T* operator&() const;
  T& operator->() const;

private:
  zero<T*> m_ptr;
  critical_pin::lock m_lock;
};

/***************************************************************************
 * IndexLock
 *
 * Hooks into the array operator[] in that it creates a temporary object
 * which passes through an array index variable whilst at the same time
 * incrementing the lock during the lifespan of the wrapper object.
 ***************************************************************************/
struct index_lock
{
  index_lock(size_t _index) :
    m_index(_index)
  { }

  operator size_t() const
  {
    return m_index;
  }

  void lock(const critical_pin::lock& _lock) const
  {
    m_lock = _lock;
  }

private:
  zero<size_t> m_index;
  mutable critical_pin::lock m_lock;

  index_lock(const index_lock& _copy);
  index_lock& operator=(const index_lock& _other);
};

}

/* Implementation */

#include "panic.h"

namespace sys
{

/***************************************************************************
 * ptr_lock
 *
 * Construct ptr_lock by assigning all variables. The internal lock also
 * increments the LockBlock reference count.
 ***************************************************************************/
template <typename T>
ptr_lock<T>::ptr_lock(T* _ptr, const critical_pin::lock& _lock) :
  m_ptr(_ptr),
  m_lock(_lock)
{ }

template <typename T>
T* ptr_lock<T>::operator->() const
{
  if(!m_ptr) panic("Attempting to dereference NULL pointer");
  return m_ptr;
}

/*
 * Allows for implicit cast to T*. I.e when passed as parameter
 */
template <typename T>
ptr_lock<T>::operator T*() const
{
  return m_ptr;
}

template <typename T>
ref_lock<T>::ref_lock(T* _ptr, const critical_pin::lock& _lock) :
  m_ptr(_ptr),
  m_lock(_lock)
{
  if(!m_ptr) panic("Attempting to dereference NULL pointer");
}

/*
 * Allows for implicit cast to T&. I.e when passed as parameter
 */
template <typename T>
ref_lock<T>::operator T&()
{
  if(!m_ptr) panic("Attempting to dereference NULL pointer");
  return *m_ptr;
}

template <typename T>
ref_lock<T>::operator const T&() const
{
  if(!m_ptr) panic("Attempting to dereference NULL pointer");
  return *m_ptr;
}

template <typename T>
ref_lock<T>& ref_lock<T>::operator=(const T& _value)
{
  if(!m_ptr) panic("Attempting to assign NULL pointer");
  *m_ptr = _value;

  return *this;
}

template <typename T>
T ref_lock<T>::operator+(const T& _value)
{
  if(!m_ptr) panic("Attempting to assign NULL pointer");
  return *m_ptr + _value;
}

template <typename T>
T* ref_lock<T>::operator&() const
{
  if(!m_ptr) panic("Attempting to dereference NULL pointer");
  return m_ptr;
}

template <typename T>
T& ref_lock<T>::operator->() const
{
  if(!m_ptr) panic("Attempting to dereference NULL pointer");
  return *m_ptr;
}

}

#endif

#endif

