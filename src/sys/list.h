#ifndef SYS_LIST_H
#define SYS_LIST_H

#include "vector.h"
#include "base.h"
#include "criticalpin.h"

namespace sys
{

#ifdef SYS_DEBUG
template <typename T> struct ref_lock;
template <typename T> struct ptr_lock;
#endif

template <typename T>
struct list : base
{
  struct entry
  {
    entry()
      : m_ptr()
      , m_deleter()
      , m_copier()
#ifdef SYS_DEBUG
      , m_pin()
#endif
    { }

    T* m_ptr;
    void (*m_deleter)(T*);
    T* (*m_copier)(T*);
#ifdef SYS_DEBUG
    critical_pin m_pin;
#endif
  };

  list();
  list(const list& _copy);
  ~list();

  size_t size() const;
  void clear();

  template <typename U> void push_back(const U& _value);

#ifdef SYS_DEBUG
  T& operator[](const index_lock& _index);
  const T& operator[](const index_lock& _index) const;
#else
  T& operator[](size_t _index);
  const T& operator[](size_t _index) const;
#endif

private:
  vector<entry*> m_entries;

  template <typename U> static void deleter(T* _ptr);
  template <typename U> static T* copier(T* _ptr);
};

}

/* Implementation */

#include "lock.h"

namespace sys
{

template <typename T>
template <typename U>
void list<T>::deleter(T* _ptr)
{
  const U* ptr = static_cast<const U*>(_ptr);
  delete ptr;
}

template <typename T>
template <typename U>
T* list<T>::copier(T* _ptr)
{
  const U* ptr = static_cast<const U*>(_ptr);
  U* rtn = new U(*ptr);

  return rtn;
}

template <typename T>
list<T>::list()
{ }

template <typename T>
list<T>::list(const list& _copy)
{
  for(typename vector<entry*>::iterator it = _copy.m_entries.begin();
    it != _copy.m_entries.end(); ++it)
  {
    entry* e = *it;

    entry* n = new entry();
    n->m_ptr = e->m_copier(e->m_ptr);
    n->m_deleter = e->m_deleter;
    n->m_copier = e->m_copier;
    m_entries.push_back(n);
  }
}

template <typename T>
list<T>::~list()
{
  clear();
}

template <typename T>
void list<T>::clear()
{
  for(typename vector<entry*>::iterator it = m_entries.begin(); it != m_entries.end(); ++it)
  {
    entry* e = *it;
#ifdef SYS_DEBUG
    e->m_pin.reset();
#endif
    e->m_deleter(e->m_ptr);
    delete e;
  }

  m_entries.clear();
}

template <typename T>
template <typename U>
void list<T>::push_back(const U& _value)
{
  entry* e = new entry();
  e->m_ptr = new U(_value);
  e->m_deleter = deleter<U>;
  e->m_copier = copier<U>;
  m_entries.push_back(e);
}

template <typename T>
size_t list<T>::size() const
{
  return m_entries.size();
}

#ifdef SYS_DEBUG
template <typename T>
T& list<T>::operator[](const index_lock& _index)
{
  if(_index >= m_entries.size())
  {
    panic("Index out of bounds");
  }

  entry* e = m_entries[_index];
  _index.lock(critical_pin::lock(e->m_pin));

  return *e->m_ptr;
}

template <typename T>
const T& list<T>::operator[](const index_lock& _index) const
{
  if(_index >= m_entries.size())
  {
    panic("Index out of bounds");
  }

  entry* e = m_entries[_index];
  _index.lock(critical_pin::lock(e->m_pin));

  return *e->m_ptr;
}
#else
template <typename T>
T& list<T>::operator[](size_t _index)
{
  return *m_entries[_index]->m_ptr;
}
     
template <typename T>
const T& list<T>::operator[](size_t _index) const
{ 
  return *m_entries[_index]->m_ptr;
}
#endif

}

#endif

