#ifndef SYS_WEAKPTR_H
#define SYS_WEAKPTR_H

#include "zero.h"
#include "config.h"
#include "weakpin.h"

namespace sys
{

#ifdef SYS_DEBUG
template <typename T> struct ref_lock;
template <typename T> struct ptr_lock;
#endif

template <typename T> struct auto_ptr;
template <typename T> struct ptr;

template <typename U, typename V>
weak_ptr<U> dynamic_ptr_cast(const weak_ptr<V>& _other);

template <typename T>
struct weak_ptr
{
  weak_ptr();
  weak_ptr(const weak_ptr& _copy);
  weak_ptr& operator=(const weak_ptr& _other);
  ~weak_ptr();

  void reset();
  weak_pin::lock weak_pin_lock() const;

#ifdef SYS_DEBUG
  critical_pin::lock critical_pin_lock() const;

  ref_lock<T> operator*() const;
  ptr_lock<T> operator->() const;
  ptr_lock<T> get() const;
#else
  T& operator*() const;
  T* operator->() const;
  T* get() const;
#endif

  weak_ptr(T* _other);
  weak_ptr& operator=(T* _other);
  // Comparison is out-of-class definition

  template <typename U> weak_ptr(const weak_ptr<U>& _other);
  template <typename U> weak_ptr& operator=(const weak_ptr<U>& _other);
  template <typename U> bool operator==(const weak_ptr<U>& _other) const;
  template <typename U> bool operator!=(const weak_ptr<U>& _other) const;

  // TODO: Not possible to convert from ptr to weak_ptr?
  template <typename U> bool operator==(const ptr<U>& _other) const;
  template <typename U> bool operator!=(const ptr<U>& _other) const;

  template <typename U> weak_ptr(const auto_ptr<U>& _other);
  template <typename U> weak_ptr& operator=(const auto_ptr<U>& _other);
  template <typename U> bool operator==(const auto_ptr<U>& _other) const;
  template <typename U> bool operator!=(const auto_ptr<U>& _other) const;

private:
  zero<T*> m_ptr;
  weak_pin::lock m_lock;

  void bool_func() const { }
  typedef void (weak_ptr::*bool_type)() const;

  template <typename U, typename V>
  friend weak_ptr<U> dynamic_ptr_cast(const weak_ptr<V>& _other);

public:
  operator bool_type() const;
};

}

/* Implementation */

#include "panic.h"
#include "lock.h"
#include "autoptr.h"
#include "ptr.h"

namespace sys
{

template <typename U, typename V>
weak_ptr<U> dynamic_ptr_cast(const weak_ptr<V>& _other)
{
  weak_ptr<U> rtn;

  rtn.m_ptr = dynamic_cast<U*>(static_cast<V*>(_other.m_ptr));
  if(!rtn.m_ptr) return rtn;

  rtn.m_lock = _other.m_lock;

  return rtn;
}

template <typename T>
weak_ptr<T>::weak_ptr()
{ }

template <typename T>
weak_ptr<T>::weak_ptr(const weak_ptr& _copy)
  : m_ptr(_copy.m_ptr)
  , m_lock(_copy.m_lock)
{ }

template <typename T>
weak_ptr<T>& weak_ptr<T>::operator=(const weak_ptr& _other)
{
  if(get() == _other.get()) return *this;

  weak_ptr tmp(*this);

  m_ptr = _other.m_ptr;
  m_lock = _other.m_lock;

  return *this;
}

template <typename T>
weak_ptr<T>::weak_ptr(T* _other)
  : m_ptr(_other)
  , m_lock(_other ? _other->weak_pin_lock() : weak_pin::lock())
{ }

template <typename T>
weak_ptr<T>& weak_ptr<T>::operator=(T* _other)
{
  if(!_other)
  {
    m_ptr = NULL;
    m_lock = weak_pin::lock();

    return *this;
  }

  if(get() == _other) return *this;

  weak_ptr tmp(*this);

  m_ptr = _other;
  m_lock = _other->weak_pin_lock();

  return *this;
}

template <typename T>
void weak_ptr<T>::reset()
{
  m_ptr = NULL;
  m_lock = weak_pin::lock();
}

template <typename T>
weak_ptr<T>::~weak_ptr()
{
  reset();
}

template <typename T>
template <typename U>
weak_ptr<T>::weak_ptr(const weak_ptr<U>& _other)
  : m_ptr(_other.get())
  , m_lock(_other.weak_pin_lock())
{ }

template <typename T>
template <typename U>
weak_ptr<T>& weak_ptr<T>::operator=(const weak_ptr<U>& _other)
{
  if(get() == _other.get()) return *this;

  weak_ptr tmp(*this);

  m_ptr = _other.get();
  m_lock = _other.weak_pin_lock();

  return *this;
}

template <typename T>
template <typename U>
weak_ptr<T>::weak_ptr(const auto_ptr<U>& _other)
  : m_ptr(_other.get())
  , m_lock(_other.weak_pin_lock())
{ }

template <typename T>
template <typename U>
weak_ptr<T>& weak_ptr<T>::operator=(const auto_ptr<U>& _other)
{
  if(get() == _other.get()) return *this;

  weak_ptr tmp(*this);

  m_ptr = _other.get();
  m_lock = _other.weak_pin_lock();

  return *this;
}

template <typename T>
weak_pin::lock weak_ptr<T>::weak_pin_lock() const
{
  return m_lock;
}

#ifdef SYS_DEBUG

template <typename T>
critical_pin::lock weak_ptr<T>::critical_pin_lock() const
{
  return m_lock.critical_pin_lock();
}

template <typename T>
ref_lock<T> weak_ptr<T>::operator*() const
{
  if(!*this)
  {
    panic("Attempt to access NULL reference");
  }

  return ref_lock<T>(m_ptr, m_lock.critical_pin_lock());
}

template <typename T>
ptr_lock<T> weak_ptr<T>::operator->() const
{
  if(!*this)
  {
    panic("Attempt to access NULL pointer");
  }

  return ptr_lock<T>(m_ptr, m_lock.critical_pin_lock());
}

template <typename T>
ptr_lock<T> weak_ptr<T>::get() const
{
  if(!*this)
  {
    return ptr_lock<T>(NULL, critical_pin::lock());
  }

  return ptr_lock<T>(m_ptr, m_lock.critical_pin_lock());
}

#else

template <typename T>
T& weak_ptr<T>::operator*() const
{
  return *get();
}

template <typename T>
T* weak_ptr<T>::operator->() const
{
  return get();
}

template <typename T>
T* weak_ptr<T>::get() const
{
  if(!*this) return NULL;

  return m_ptr;
}

#endif

template <typename T>
template <typename U>
bool weak_ptr<T>::operator==(const weak_ptr<U>& _other) const
{
  return get() == _other.get();
}

template <typename T>
template <typename U>
bool weak_ptr<T>::operator!=(const weak_ptr<U>& _other) const
{
  return get() != _other.get();
}

template <typename T>
template <typename U>
bool weak_ptr<T>::operator==(const ptr<U>& _other) const
{
  return get() == _other.get();
}

template <typename T>
template <typename U>
bool weak_ptr<T>::operator!=(const ptr<U>& _other) const
{
  return get() != _other.get();
}

template <typename T>
template <typename U>
bool weak_ptr<T>::operator==(const auto_ptr<U>& _other) const
{
  return get() == _other.get();
}

template <typename T>
template <typename U>
bool weak_ptr<T>::operator!=(const auto_ptr<U>& _other) const
{
  return get() != _other.get();
}

template <typename T>
weak_ptr<T>::operator bool_type() const
{
  if(m_ptr && m_lock.valid())
  {
    return &weak_ptr::bool_func;
  }

  return NULL;
}

template <typename T, typename U>
bool operator==(const weak_ptr<T>& _a, const U* _b)
{
  return _a.get() == _b;
}

template <typename T, typename U>
bool operator!=(const weak_ptr<T>& _a, const U* _b)
{
  return _a.get() != _b;
}

template <typename T, typename U>
bool operator==(const U* _a, const weak_ptr<T>& _b)
{
  return _a == _b.get();
}

template <typename T, typename U>
bool operator!=(const U* _a, const weak_ptr<T>& _b)
{
  return _a != _b.get();
}

}

#endif

