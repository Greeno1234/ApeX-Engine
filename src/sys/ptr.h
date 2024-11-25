#ifndef SYS_PTR_H
#define SYS_PTR_H

#include "zero.h"
#include "criticalpin.h"
#include "config.h"

namespace sys
{

#ifdef SYS_DEBUG
template <typename T> struct ref_lock;
template <typename T> struct ptr_lock;
#endif

template <typename T> struct auto_ptr;
template <typename T> struct weak_ptr;

template <typename T>
struct ptr
{
  ptr();

  // Implicit
  // ptr(const ptr& _copy);
  // ptr& operator=(const ptr& _assign);

  template <typename U> ptr(const ptr<U>& _other);
  template <typename U> ptr& operator=(const ptr<U>& _other);
  template <typename U> bool operator==(const ptr<U>& _other) const;
  template <typename U> bool operator!=(const ptr<U>& _other) const;

  template <typename U> ptr(const auto_ptr<U>& _other);
  template <typename U> ptr& operator=(const auto_ptr<U>& _other);
  template <typename U> bool operator==(const auto_ptr<U>& _other) const;
  template <typename U> bool operator!=(const auto_ptr<U>& _other) const;

  template <typename U> ptr(const weak_ptr<U>& _other);
  template <typename U> ptr& operator=(const weak_ptr<U>& _other);
  template <typename U> bool operator==(const weak_ptr<U>& _other) const;
  template <typename U> bool operator!=(const weak_ptr<U>& _other) const;

  ptr(T* _t);
  ptr& operator=(T* _other);

#ifdef SYS_DEBUG
  ptr(T* _t, critical_pin::lock _lock);

  ref_lock<T> operator*() const;
  ptr_lock<T> operator->() const;
  ptr_lock<T> get() const;

  critical_pin::lock critical_pin_lock() const;
#else
  T& operator*() const;
  T* operator->() const;
  T* get() const;
#endif

  void reset();

private:
  zero<T*> m_ptr;
#ifdef SYS_DEBUG
  critical_pin::lock m_lock;
#endif

  void bool_func() const { }
  typedef void (ptr::*bool_type)() const;

public:
  operator bool_type() const;
};

}

/* Implementation */

#include "lock.h"
#include "autoptr.h"
#include "weakptr.h"
#include "base.h"

namespace sys
{

/********************************************************************
 * Constructor (Default)
 *
 * Take advantage of automatic zeroing of member variables.
 t
 t sys::ptr<Employee> emp1;
 t sys::ptr<Employee> emp2 = sys::ptr<Employee>();
 t
 ********************************************************************/
template <typename T>
ptr<T>::ptr()
{ }

/********************************************************************
 * Constructor (T*)
 *
 * Captures the specified raw pointer and obtains a lock from the
 * EnablePtr base struct.
 t
 t Employee emp;
 t sys::ptr<Employee> pemp1(&emp);
 t sys::ptr<Employee> pemp2 = sys::ptr<Employee>(&emp);
 t
 ********************************************************************/
template <typename T>
ptr<T>::ptr(T* _t)
  : m_ptr(_t)
#ifdef SYS_DEBUG
  , m_lock(_t ? _t->critical_pin_lock() : critical_pin::lock())
#endif
{ }

/********************************************************************
 * Constructor (T*)
 *
 * Captures the specified raw pointer and obtains a lock from the
 * passed in lock. Allowing for a custom owner of memory.
 t
 t Employee emp;
 t //sys::ptr<Employee> pemp1(&emp, emp.critical_pin_lock());
 t //sys::ptr<Employee> pemp2 = sys::ptr<Employee>(&emp, emp.critical_pin_lock());
 t
 ********************************************************************/
#ifdef SYS_DEBUG
template <typename T>
ptr<T>::ptr(T* _t, critical_pin::lock _lock)
  : m_ptr(_t)
  , m_lock(_lock)
{ }
#endif

/********************************************************************
 * Assignment operator (T*)
 *
 * Assignment operator to replace the existing raw and lock with the
 * specified raw pointer and obtains a new lock from the EnablePtr
 * base struct.
 t
 t Employee emp;
 t sys::ptr<Employee> pemp;
 t pemp = &emp;
 t
 ********************************************************************/
template <typename T>
ptr<T>& ptr<T>::operator=(T* _other)
{
  m_ptr = _other;
#ifdef SYS_DEBUG
  m_lock = m_ptr ? m_ptr->critical_pin_lock() : critical_pin::lock();
#endif

  return *this;
}

/********************************************************************
 * Constructor (ptr)
 *
 * Similar to the synthesised copy constructor, effectively capturing
 * a raw pointer and the corresponding lock from the specified ptr.
 t
 t Manager man;
 t sys::ptr<Manager> pman(&man);
 t sys::ptr<Employee> pemp1(pman);
 t sys::ptr<Employee> pemp2 = sys::ptr<Employee>(pman);
 t
 ********************************************************************/
template <typename T>
template <typename U>
ptr<T>::ptr(const ptr<U>& _other)
  : m_ptr(_other.get())
#ifdef SYS_DEBUG
  , m_lock(_other.critical_pin_lock())
#endif
{ }

/********************************************************************
 t
 t sys::auto_ptr<Manager> pman(sys::make_auto<Manager>());
 t sys::ptr<Employee> pemp1(pman);
 t sys::ptr<Employee> pemp2 = sys::ptr<Employee>(pman);
 t
 ********************************************************************/
template <typename T>
template <typename U>
ptr<T>::ptr(const auto_ptr<U>& _other)
  : m_ptr(_other.get())
#ifdef SYS_DEBUG
  , m_lock(_other.critical_pin_lock())
#endif
{ }

/********************************************************************
 t
 t Manager man;
 t sys::weak_ptr<Manager> pman(&man);
 t sys::ptr<Employee> pemp1(pman);
 t sys::ptr<Employee> pemp2 = sys::ptr<Employee>(pman);
 t
 ********************************************************************/
template <typename T>
template <typename U>
ptr<T>::ptr(const weak_ptr<U>& _other)
  : m_ptr(_other.get())
#ifdef SYS_DEBUG
  , m_lock(_other.critical_pin_lock())
#endif
{ }

/********************************************************************
 * Assignment operator (ptr)
 *
 * Similar to the synthesised assignment operator, replacing its own
 * raw pointer and lock with that of the specified ptr.
 t
 t Manager man;
 t sys::ptr<Manager> pman;
 t sys::ptr<Employee> pemp;
 t pemp = pman;
 t
 ********************************************************************/
template <typename T>
template <typename U>
ptr<T>& ptr<T>::operator=(const ptr<U>& _other)
{
  m_ptr = _other.get();
#ifdef SYS_DEBUG
  m_lock = _other.critical_pin_lock();
#endif

  return *this;
}

/********************************************************************
 t
 t sys::auto_ptr<Manager> pman(sys::make_auto<Manager>());
 t sys::ptr<Employee> pemp;
 t pemp = pman;
 t
 ********************************************************************/
template <typename T>
template <typename U>
ptr<T>& ptr<T>::operator=(const auto_ptr<U>& _other)
{
  m_ptr = _other.get();
#ifdef SYS_DEBUG
  m_lock = _other.critical_pin_lock();
#endif

  return *this;
}

/********************************************************************
 t
 t Manager man;
 t sys::weak_ptr<Manager> pman(&man);
 t sys::ptr<Employee> pemp;
 t pemp = pman;
 t
 ********************************************************************/
template <typename T>
template <typename U>
ptr<T>& ptr<T>::operator=(const weak_ptr<U>& _other)
{
  m_ptr = _other.get();
#ifdef SYS_DEBUG
  m_lock = _other.critical_pin_lock();
#endif

  return *this;
}

#ifdef SYS_DEBUG

template <typename T>
critical_pin::lock ptr<T>::critical_pin_lock() const
{
  return m_lock;
}

template <typename T>
ref_lock<T> ptr<T>::operator*() const
{
  if(!*this)
  {
    panic("Attempt to access NULL reference");
  }

  return ref_lock<T>(m_ptr, m_lock);
}

template <typename T>
ptr_lock<T> ptr<T>::operator->() const
{
  if(!*this)
  {
    panic("Attempt to access NULL pointer");
  }

  return ptr_lock<T>(m_ptr, m_lock);
}

template <typename T>
ptr_lock<T> ptr<T>::get() const
{
  if(!*this)
  {
    return ptr_lock<T>(NULL, critical_pin::lock());
  }

  return ptr_lock<T>(m_ptr, m_lock);
}

#else

template <typename T>
T& ptr<T>::operator*() const
{
  return *get();
}

template <typename T>
T* ptr<T>::operator->() const
{
  return get();
}

template <typename T>
T* ptr<T>::get() const
{
  if(!*this)
  {
    return NULL;
  }

  return m_ptr;
}

#endif

/********************************************************************
 * Comparison (ptr)
 *
 * Pointer comparison between contained raw pointer and the raw
 * pointer contained within the specified object.
 ********************************************************************/
template <typename T>
template <typename U>
bool ptr<T>::operator==(const weak_ptr<U>& _other) const
{
  return get() == _other.get();
}

template <typename T>
template <typename U>
bool ptr<T>::operator!=(const weak_ptr<U>& _other) const
{
  return get() != _other.get();
}

template <typename T>
template <typename U>
bool ptr<T>::operator==(const ptr<U>& _other) const
{
  return get() == _other.get();
}

template <typename T>
template <typename U>
bool ptr<T>::operator!=(const ptr<U>& _other) const
{
  return get() != _other.get();
}

template <typename T>
template <typename U>
bool ptr<T>::operator==(const auto_ptr<U>& _other) const
{
  return get() == _other.get();
}

template <typename T>
template <typename U>
bool ptr<T>::operator!=(const auto_ptr<U>& _other) const
{
  return get() != _other.get();
}

template <typename T>
ptr<T>::operator bool_type() const
{
  if(m_ptr)
  {
    return &ptr::bool_func;
  }

  return NULL;
}

template <typename T>
void ptr<T>::reset()
{
#ifdef SYS_DEBUG
  m_lock = critical_pin::lock();
#endif
  m_ptr = NULL;
}

/********************************************************************
 * Comparison (T*)
 *
 * Pointer comparison between contained raw pointer and the specified
 * raw pointer.
 ********************************************************************/
template <typename T, typename U>
bool operator==(const ptr<T>& _a, const U* _b)
{
  return _a.get() == _b;
}

template <typename T, typename U>
bool operator!=(const ptr<T>& _a, const U* _b)
{
  return _a.get() != _b;
}

template <typename T, typename U>
bool operator==(const U* _a, const ptr<T>& _b)
{
  return _a == _b.get();
}

template <typename T, typename U>
bool operator!=(const U* _a, const ptr<T>& _b)
{
  return _a != _b.get();
}

}

#endif

