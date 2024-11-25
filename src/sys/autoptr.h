#ifndef SYS_AUTOPTR_H
#define SYS_AUTOPTR_H

#include "config.h"
#include "criticalpin.h"
#include "weakpin.h"
#include "zero.h"

namespace sys
{

#ifdef SYS_DEBUG
template <typename T> struct ref_lock;
template <typename T> struct ptr_lock;
#endif

template <typename T> struct ptr;
template <typename T> struct weak_ptr;
template <typename T> struct auto_ptr;
template <typename T> auto_ptr<T> make_auto();
// TODO: Better forwarding
template <typename T, typename A> auto_ptr<T> make_auto(A& _a);
template <typename T, typename A> auto_ptr<T> make_auto(const A& _a);
//template <typename T, typename A> auto_ptr<T> make_auto(A _a);
template <typename T, typename A, typename B> auto_ptr<T> make_auto(A _a, B _b);
template <typename T, typename A, typename B, typename C> auto_ptr<T> make_auto(A _a, B _b, C _c);

struct auto_ptr_data
{
  zero<void (*)(const void*)> m_deleter;
  zero<const void*> m_ptr;
  weak_pin m_weakpin;
#ifdef SYS_DEBUG
  critical_pin m_criticalpin;
#endif
};

template <typename T>
struct auto_ptr
{
  auto_ptr();
  auto_ptr(const auto_ptr& _copy);
  auto_ptr& operator=(const auto_ptr& _other);
  ~auto_ptr();

  template <typename U> auto_ptr(const auto_ptr<U>& _other);
  template <typename U> auto_ptr& operator=(const auto_ptr<U>& _other);

  void reset();
  // TODO: swap

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

private:
  template <typename U> friend struct auto_ptr;
  template <typename U> friend auto_ptr<U> make_auto();
  template <typename U, typename A> friend auto_ptr<U> make_auto(A& _a);
  template <typename U, typename A> friend auto_ptr<U> make_auto(const A& _a);
  //template <typename U, typename A> friend auto_ptr<U> make_auto(A _a);
  template <typename U, typename A, typename B> friend auto_ptr<U> make_auto(A _a, B _b);
  template <typename U, typename A, typename B, typename C> friend auto_ptr<U> make_auto(A _a, B _b, C _c);

  mutable zero<T*> m_ptr;
  mutable zero<auto_ptr_data*> m_data;

  static void deleter(const void* _ptr);

  void bool_func() const { }
  typedef void (auto_ptr::*bool_type)() const;

public:
  operator bool_type() const;
};

}

/* Implementation */

#include "panic.h"
#include "lock.h"

namespace sys
{

/***************************************************************************
 t
 t  sys::auto_ptr<Employee> emp;
 t  sys::auto_ptr<const Employee> cemp;
 t  sys::auto_ptr<int> i;
 t  sys::auto_ptr<const int> ci;
 t
 **************************************************************************/
template <typename T>
auto_ptr<T>::auto_ptr()
{ }

/***************************************************************************
 t
 t  sys::auto_ptr<Employee> emp(sys::make_auto<Employee>());
 t  sys::auto_ptr<Employee> empcopy(emp);
 t  sys::auto_ptr<const Employee> cemp(sys::make_auto<const Employee>());
 t  sys::auto_ptr<const Employee> cempcopy(cemp);
 t  sys::auto_ptr<Employee> empty;
 t  sys::auto_ptr<Employee> emptycopy(empty);
 t
 **************************************************************************/
template <typename T>
auto_ptr<T>::auto_ptr(const auto_ptr& _copy)
  : m_ptr(_copy.m_ptr)
  , m_data(_copy.m_data)
{
  _copy.m_ptr = NULL;
  _copy.m_data = NULL;
}

/***************************************************************************
 t
 t  sys::auto_ptr<Employee> emp(sys::make_auto<Employee>());
 t  sys::auto_ptr<Employee> empcopy;
 t  empcopy = emp;
 t  sys::auto_ptr<const Employee> cemp(sys::make_auto<const Employee>());
 t  sys::auto_ptr<const Employee> cempcopy;
 t  cempcopy = cemp;
 t  sys::auto_ptr<Employee> empty;
 t  sys::auto_ptr<Employee> emptycopy;
 t  emptycopy = empty;
 t
 **************************************************************************/
template <typename T>
auto_ptr<T>& auto_ptr<T>::operator=(const auto_ptr& _other)
{
  if(get() == _other.get()) return *this;

  auto_ptr tmp(*this);

  m_ptr = _other.m_ptr;
  m_data = _other.m_data;

  _other.m_ptr = NULL;
  _other.m_data = NULL;

  return *this;
}

/***************************************************************************
 t
 t  sys::auto_ptr<Employee> emp(sys::make_auto<Employee>());
 t  emp.reset();
 t  sys::auto_ptr<const Employee> cemp(sys::make_auto<const Employee>());
 t  cemp.reset();
 t  sys::auto_ptr<Employee> empty;
 t  empty.reset();
 t
 **************************************************************************/
template <typename T>
void auto_ptr<T>::reset()
{
  if(*this)
  {
    m_data->m_deleter.get()(m_data->m_ptr);
    delete m_data.get();
  }

  m_ptr = NULL;
  m_data = NULL;
}

/***************************************************************************
 t
 t  sys::auto_ptr<Employee> emp(sys::make_auto<Employee>());
 t  sys::auto_ptr<Employee> empty;
 t
 **************************************************************************/
template <typename T>
auto_ptr<T>::~auto_ptr()
{
  reset();
}

/***************************************************************************
 t
 t  sys::auto_ptr<Employee> emp(sys::make_auto<Manager>());
 t  sys::auto_ptr<Manager> man(sys::make_auto<Manager>());
 t  sys::auto_ptr<Employee> mancopy(man);
 t  sys::auto_ptr<const Employee> cempman(sys::make_auto<Manager>());
 t  sys::auto_ptr<const Employee> cemp(sys::make_auto<const Manager>());
 t  sys::auto_ptr<const Manager> cman(sys::make_auto<const Manager>());
 t  sys::auto_ptr<const Employee> cmancopy(cman);
 t
 **************************************************************************/
template <typename T>
template <typename U>
auto_ptr<T>::auto_ptr(const auto_ptr<U>& _other)
  : m_ptr(_other.m_ptr)
  , m_data(_other.m_data)
{
  _other.m_ptr = NULL;
  _other.m_data = NULL;
}

/***************************************************************************
 t
 t  sys::auto_ptr<Manager> man(sys::make_auto<Manager>());
 t  sys::auto_ptr<Employee> empcopy;
 t  empcopy = man;
 t
 t  sys::auto_ptr<const Employee> cempman;
 t  cempman = sys::make_auto<Manager>();
 t
 t  sys::auto_ptr<const Manager> cman(sys::make_auto<const Manager>());
 t  sys::auto_ptr<const Employee> cempcopy;
 t  cempcopy = cman;
 t  sys::auto_ptr<Manager> empty;
 t  sys::auto_ptr<Employee> emptycopy;
 t  emptycopy = empty;
 t
 **************************************************************************/
template <typename T>
template <typename U>
auto_ptr<T>& auto_ptr<T>::operator=(const auto_ptr<U>& _other)
{
  if(get() == _other.get()) return *this;

  auto_ptr tmp(*this);

  m_ptr = _other.m_ptr;
  m_data = _other.m_data;

  _other.m_ptr = NULL;
  _other.m_data = NULL;

  return *this;
}

#ifdef SYS_DEBUG

/***************************************************************************
 t
 t  sys::auto_ptr<Manager> man(sys::make_auto<Manager>());
 t  //man.critical_pin_lock();
 t
 **************************************************************************/
template <typename T>
critical_pin::lock auto_ptr<T>::critical_pin_lock() const
{
  if(!*this)
  {
    return critical_pin::lock();
  }

  return critical_pin::lock(m_data->m_criticalpin);
}

/***************************************************************************
 t
 t  sys::auto_ptr<Manager> man(sys::make_auto<Manager>());
 t  Manager& man1(*man);
 t  const Manager& man2(*man);
 t  sys::auto_ptr<const Manager> cman(sys::make_auto<Manager>());
 t  const Manager& cman1(*cman);
 t
 **************************************************************************/
template <typename T>
ref_lock<T> auto_ptr<T>::operator*() const
{
  if(!*this)
  {
    panic("Attempt to access NULL reference");
  }

  return ref_lock<T>(m_ptr, critical_pin_lock());
}

/***************************************************************************
 t
 t  sys::auto_ptr<Manager> man(sys::make_auto<Manager>());
 t  man->id(9);
 t  sys::auto_ptr<const Manager> cman(sys::make_auto<Manager>());
 t  cman->id();
 t
 **************************************************************************/
template <typename T>
ptr_lock<T> auto_ptr<T>::operator->() const
{
  if(!*this)
  {
    panic("Attempt to access NULL pointer");
  }

  return ptr_lock<T>(m_ptr, critical_pin_lock());
}

/***************************************************************************
 t
 t  sys::auto_ptr<Manager> man(sys::make_auto<Manager>());
 t  Manager* man1(man.get());
 t  const Manager* man2(man.get());
 t  sys::auto_ptr<const Manager> cman(sys::make_auto<Manager>());
 t  const Manager* cman1(cman.get());
 t
 **************************************************************************/
template <typename T>
ptr_lock<T> auto_ptr<T>::get() const
{
  if(!*this)
  {
    return ptr_lock<T>(NULL, critical_pin::lock());
  }

  return ptr_lock<T>(m_ptr, critical_pin_lock());
}

#else

template <typename T>
T& auto_ptr<T>::operator*() const
{
  return *m_ptr;
}

template <typename T>
T* auto_ptr<T>::operator->() const
{
  return m_ptr;
}

template <typename T>
T* auto_ptr<T>::get() const
{
  return m_ptr;
}

#endif

template <typename T>
void auto_ptr<T>::deleter(const void* _ptr)
{
  const T* ptr = static_cast<const T*>(_ptr);
  delete ptr;
}

/***************************************************************************
 t
 t  sys::auto_ptr<Employee> emp;
 t  if(emp) {}
 t  if(!emp) {}
 t  emp = sys::make_auto<Employee>();
 t  if(emp) {}
 t  if(!emp) {}
 t  sys::auto_ptr<const Employee> cemp;
 t  if(cemp) {}
 t  if(!cemp) {}
 t  cemp = sys::make_auto<Employee>();
 t  if(cemp) {}
 t  if(!cemp) {}
 t
 **************************************************************************/
template <typename T>
auto_ptr<T>::operator bool_type() const
{
  // TODO: Issue on MSVC
/*
  if(m_ptr && m_data)
  {
    return &auto_ptr::bool_func;
  }

  return NULL;
*/

  if(m_ptr)
  {
    if(m_data)
    {
      return &auto_ptr::bool_func;
    }
  }

  return NULL;
}

template <typename T>
weak_pin::lock auto_ptr<T>::weak_pin_lock() const
{
  if(!*this)
  {
    return weak_pin::lock();
  }

  return weak_pin::lock(m_data->m_weakpin);
}

/***************************************************************************
 t
 t sys::auto_ptr<int> pint(sys::make_auto<int>());
 t sys::auto_ptr<Employee> emp(sys::make_auto<Employee>());
 t emp->id();
 t
 **************************************************************************/
template <typename T>
auto_ptr<T> make_auto()
{
  auto_ptr<T> rtn;

  rtn.m_ptr = new T();
  rtn.m_data = new auto_ptr_data();
  rtn.m_data->m_deleter = auto_ptr<T>::deleter;
  rtn.m_data->m_ptr = rtn.m_ptr;

  return rtn;
}

/***************************************************************************
 t
 t sys::auto_ptr<int> pint(sys::make_auto<int>(9));
 t sys::auto_ptr<Employee> emp(sys::make_auto<Employee>(9));
 t emp->id();
 t
 **************************************************************************/
template <typename T, typename A>
auto_ptr<T> make_auto(A& _a)
{
  auto_ptr<T> rtn;

  rtn.m_ptr = new T(_a);
  rtn.m_data = new auto_ptr_data();
  rtn.m_data->m_deleter = auto_ptr<T>::deleter;
  rtn.m_data->m_ptr = rtn.m_ptr;

  return rtn;
}

template <typename T, typename A>
auto_ptr<T> make_auto(const A& _a)
{
  auto_ptr<T> rtn;

  rtn.m_ptr = new T(_a);
  rtn.m_data = new auto_ptr_data();
  rtn.m_data->m_deleter = auto_ptr<T>::deleter;
  rtn.m_data->m_ptr = rtn.m_ptr;

  return rtn;
}

/*
template <typename T, typename A>
auto_ptr<T> make_auto(A _a)
{
  auto_ptr<T> rtn;

  rtn.m_ptr = new T(_a);
  rtn.m_data = new auto_ptr_data();
  rtn.m_data->m_deleter = auto_ptr<T>::deleter;
  rtn.m_data->m_ptr = rtn.m_ptr;

  return rtn;
}
*/

template <typename T, typename A, typename B>
auto_ptr<T> make_auto(A _a, B _b)
{
  auto_ptr<T> rtn;

  rtn.m_ptr = new T(_a, _b);
  rtn.m_data = new auto_ptr_data();
  rtn.m_data->m_deleter = auto_ptr<T>::deleter;
  rtn.m_data->m_ptr = rtn.m_ptr;

  return rtn;
}

template <typename T, typename A, typename B, typename C>
auto_ptr<T> make_auto(A _a, B _b, C _c)
{
  auto_ptr<T> rtn;

  rtn.m_ptr = new T(_a, _b, _c);
  rtn.m_data = new auto_ptr_data();
  rtn.m_data->m_deleter = auto_ptr<T>::deleter;
  rtn.m_data->m_ptr = rtn.m_ptr;

  return rtn;
}

/***************************************************************************
 t
 t sys::auto_ptr<Employee> emp1(sys::make_auto<Employee>());
 t sys::auto_ptr<Employee> emp2(sys::make_auto<Employee>());
 t if(emp1 == emp2) { }
 t if(emp2 == emp1) { }
 t if(emp1 != emp2) { }
 t if(emp2 != emp1) { }
 t sys::auto_ptr<Manager> man(sys::make_auto<Manager>());
 t if(emp1 == man) { }
 t if(man == emp1) { }
 t if(emp1 != man) { }
 t if(man != emp1) { }
 t
 **************************************************************************/
template <typename T, typename U>
bool operator==(const auto_ptr<T>& _a, const auto_ptr<U>& _b)
{
  return _a.get() == _b.get();
}

template <typename T, typename U>
bool operator!=(const auto_ptr<T>& _a, const auto_ptr<U>& _b)
{
  return _a.get() != _b.get();
}

/***************************************************************************
 t
 t sys::auto_ptr<Manager> man1(sys::make_auto<Manager>());
 t sys::ptr<Manager> man2(man1);
 t if(man2 == man1) { }
 t if(man1 == man2) { }
 t if(man2 != man1) { }
 t if(man1 != man2) { }
 t sys::ptr<Employee> emp(man1);
 t if(man1 == emp) { }
 t if(emp == man1) { }
 t if(man1 != emp) { }
 t if(emp != man1) { }
 t
 **************************************************************************/
template <typename T, typename U>
bool operator==(const auto_ptr<T>& _a, const ptr<U>& _b)
{
  return _a.get() == _b.get();
}

template <typename T, typename U>
bool operator!=(const auto_ptr<T>& _a, const ptr<U>& _b)
{
  return _a.get() != _b.get();
}

template <typename T, typename U>
bool operator==(const ptr<T>& _a, const auto_ptr<U>& _b)
{
  return _a.get() == _b.get();
}

template <typename T, typename U>
bool operator!=(const ptr<T>& _a, const auto_ptr<U>& _b)
{
  return _a.get() != _b.get();
}

/***************************************************************************
 t
 t sys::auto_ptr<Manager> man1(sys::make_auto<Manager>());
 t sys::weak_ptr<Manager> man2(man1);
 t if(man2 == man1) { }
 t if(man1 == man2) { }
 t if(man2 != man1) { }
 t if(man1 != man2) { }
 t sys::weak_ptr<Employee> emp(man1);
 t if(man1 == emp) { }
 t if(emp == man1) { }
 t if(man1 != emp) { }
 t if(emp != man1) { }
 t
 **************************************************************************/
template <typename T, typename U>
bool operator==(const auto_ptr<T>& _a, const weak_ptr<U>& _b)
{
  return _a.get() == _b.get();
}

template <typename T, typename U>
bool operator!=(const auto_ptr<T>& _a, const weak_ptr<U>& _b)
{
  return _a.get() != _b.get();
}

template <typename T, typename U>
bool operator==(const weak_ptr<T>& _a, const auto_ptr<U>& _b)
{
  return _a.get() == _b.get();
}

template <typename T, typename U>
bool operator!=(const weak_ptr<T>& _a, const auto_ptr<U>& _b)
{
  return _a.get() != _b.get();
}

/***************************************************************************
 t
 t sys::auto_ptr<Manager> man1(sys::make_auto<Manager>());
 t Manager* man2(man1.get());
 t if(man2 == man1) { }
 t if(man1 == man2) { }
 t if(man2 != man1) { }
 t if(man1 != man2) { }
 t Employee* emp(man1.get());
 t if(man1 == emp) { }
 t if(emp == man1) { }
 t if(man1 != emp) { }
 t if(emp != man1) { }
 t
 **************************************************************************/
template <typename T, typename U>
bool operator==(const auto_ptr<T>& _a, const U* _b)
{
  return _a.get() == _b;
}

template <typename T, typename U>
bool operator!=(const auto_ptr<T>& _a, const U* _b)
{
  return _a.get() != _b;
}

template <typename T, typename U>
bool operator==(const T* _a, const auto_ptr<U>& _b)
{
  return _a == _b.get();
}

template <typename T, typename U>
bool operator!=(const T* _a, const auto_ptr<U>& _b)
{
  return _a != _b.get();
}

}

#endif

