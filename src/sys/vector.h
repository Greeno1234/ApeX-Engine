#ifndef SYS_VECTOR_H
#define SYS_VECTOR_H

#include "ptr.h"
#include "zero.h"
#include "criticalpin.h"
#include "base.h"

#include <new>

//void* operator new(size_t, void*) noexcept;

//inline void* operator new(size_t _count, void* _ptr) noexcept
//{
//  return _ptr;
//}

namespace sys
{

#ifdef SYS_DEBUG
template <typename T> struct ref_lock;
template <typename T> struct ptr_lock;
#endif

#ifdef _MSC_VER
  #include <BaseTsd.h>
  typedef SSIZE_T ssize_t;
#endif

template <typename T> struct vector;

template <typename T>
struct vector_iterator
{
  vector_iterator& operator++();
  vector_iterator operator+(size_t _value) const;
  bool operator!=(const vector_iterator& _other) const;
  bool operator==(const vector_iterator& _other) const;

#ifdef SYS_DEBUG
  ptr_lock<T> operator->() const;
  ref_lock<T> operator*() const;
#else
  T* operator->() const;
  T& operator*() const;
#endif

private:
  template <typename U> friend struct vector;

  mutable zero<size_t> m_index;
  mutable ptr<const vector<T> > m_owner;
#ifdef SYS_DEBUG
  mutable critical_pin::lock m_lock;
#endif
};

template <typename T>
struct vector : base
{
  typedef vector_iterator<T> iterator;

  vector(size_t _size);

  vector();
  vector(const vector& _copy);
  vector& operator=(const vector& _other);
  ~vector();

#ifdef SYS_DEBUG
  T& operator[](const index_lock& _index);
  const T& operator[](const index_lock& _index) const;
#else
  T& operator[](size_t _index);
  const T& operator[](size_t _index) const;
#endif

  ptr<T> get(size_t _index);
  ptr<const T> get(size_t _index) const;

  ptr<T> push_back(const T& _value);
  ptr<T> insert(const T& _value, size_t _index);
  void insert(vector_iterator<T> _position, vector_iterator<T> _first, vector_iterator<T> _last);
  void clear();
  void resize(size_t _size);
  bool contains(const T& _value) const;

  size_t size() const;

  T* unsafe_raw() const;

  iterator begin() const;
  iterator end() const;
  iterator erase(const iterator& _it);
  iterator erase(const iterator& _first, const iterator& _last);
  void erase(size_t _index);

  void sort(bool (*_func)(T&, T&));

private:
  template <typename U> friend struct vector_iterator;
  zero<size_t> m_total;
  zero<T*> m_raw;
  zero<size_t> m_size;

#ifdef SYS_DEBUG
  critical_pin m_invalidationpin;
  void check_lock() const;
#endif

  void allocate(size_t _size);
  void quicksort(int _low, int _high, bool (*_func)(T&, T&));
  int partition(int _low, int _high, bool (*_func)(T&, T&));
  bool compareFunc(T& _a, T& _b);
  void swap(T& _a, T& _b);
};

}

/* Implementation */

#include "panic.h"
#include "lock.h"

namespace sys
{

template <typename T>
vector<T>::vector() :
  m_total(8),
  m_raw(static_cast<T*>(calloc(m_total, sizeof(T))))
{
  if(!m_raw)
  {
    panic("Failed to allocate initial space");
  }
}

template <typename T>
vector<T>::vector(size_t _size)
{
  resize(_size);
}

template <typename T>
vector<T>::vector(const vector& _copy) :
  m_total(_copy.m_size),
  m_raw(static_cast<T*>(calloc(m_total, sizeof(T))))
{
  if(!m_raw)
  {
    panic("Failed to allocate initial space");
  }

  try
  {
    for(size_t ei = 0; ei < _copy.m_size; ++ei)
    {
      new(&m_raw[ei]) T(_copy.m_raw[ei]);
      ++m_size;
    }
  }
  catch(...)
  {
    /* TODO: Can use clear() */
    for(ssize_t ei = m_size - 1; ei >= 0; --ei)
    {
      m_raw[ei].~T();
    }

    free(m_raw);

    throw;
  }
}

template <typename T>
vector<T>& vector<T>::operator=(const vector& _other)
{
#ifdef SYS_DEBUG
  check_lock();
#endif

  size_t total = _other.m_size;
  T* raw = static_cast<T*>(calloc(total, sizeof(T)));
  size_t size = 0;

  if(!raw)
  {
    panic("Failed to allocate initial space");
  }

  try
  {
    for(size_t ei = 0; ei < _other.m_size; ++ei)
    {
      new(&raw[ei]) T(_other.m_raw[ei]);
      ++size;
    }
  }
  catch(...)
  {
    for(ssize_t ei = size - 1; ei >= 0; --ei)
    {
      raw[ei].~T();
    }

    free(raw);

    throw;
  }

  clear();
  free(m_raw);
  m_size = size;
  m_raw = raw;
  m_total = total;

  return *this;
}

template <typename T>
vector<T>::~vector()
{
  clear();
  free(m_raw);
}

#ifdef SYS_DEBUG
template <typename T>
T& vector<T>::operator[](const index_lock& _index)
{
  _index.lock(critical_pin::lock(m_invalidationpin));

  if(_index >= m_size)
  {
    panic("Index out of bounds");
  }

  return m_raw[_index];
}

template <typename T>
const T& vector<T>::operator[](const index_lock& _index) const
{
  _index.lock(critical_pin::lock(m_invalidationpin));

  if(_index >= m_size)
  {
    panic("Index out of bounds");
  }

  return m_raw[_index];
}
#else
template <typename T>
T& vector<T>::operator[](size_t _index)
{
  return m_raw[_index];
}
     
template <typename T>
const T& vector<T>::operator[](size_t _index) const
{ 
  return m_raw[_index];
}
#endif

template <typename T>
ptr<T> vector<T>::get(size_t _index)
{
#ifdef SYS_DEBUG
  if(_index >= m_size)
  {
    panic("Index out of bounds");
  }

  return ptr<T>(&m_raw[_index], critical_pin::lock(m_invalidationpin));
#else
  return ptr<T>(&m_raw[_index]);
#endif
}

template <typename T>
ptr<const T> vector<T>::get(size_t _index) const
{
#ifdef SYS_DEBUG
  if(_index >= m_size)
  {
    panic("Index out of bounds");
  }

  return ptr<T>(&m_raw[_index], critical_pin::lock(m_invalidationpin));
#else
  return ptr<T>(&m_raw[_index]);
#endif
}

template <typename T>
bool vector<T>::contains(const T& _value) const
{
  for(size_t i = 0; i < size(); ++i)
  {
    if((*this)[i] == _value)
    {
      return true;
    }
  }

  return false;
}

template <typename T>
void vector<T>::insert(vector_iterator<T> _position, vector_iterator<T> _first, vector_iterator<T> _last)
{
  size_t position = _position.m_index;

  if(_position == end())
  {
    position = size();
  }

  if(position > size())
  {
    panic("Iterator out of range");
  }

  _position = _first;

  for(vector_iterator<T> it = _first; it != _last; ++it)
  {
    insert(*it, position);
    ++position;
  }
}

// TODO: Do we really want to return pointer to new element?
// TODO: Potentially an issue to use _value ref upon invalidation
// TODO: FIX: Return iterator to new element
template <typename T>
ptr<T> vector<T>::insert(const T& _value, size_t _index)
{
#ifdef SYS_DEBUG
  check_lock();
#endif

  push_back(_value);

  for(size_t i = size() - 1; i > _index; --i)
  {
    m_raw[i] = m_raw[i - 1];
  }

  m_raw[_index] = _value;

#ifdef SYS_DEBUG
  return ptr<T>(&m_raw[_index], critical_pin::lock(m_invalidationpin));
#else
  return ptr<T>(&m_raw[_index]);
#endif
}

// TODO: Do we really want to return pointer to new element?
template <typename T>
ptr<T> vector<T>::push_back(const T& _value)
{
#ifdef SYS_DEBUG
  check_lock();
#endif

  size_t size = m_size;
  allocate(size + 1);
  new(&m_raw[size]) T(_value);
  ++m_size;

#ifdef SYS_DEBUG
  return ptr<T>(&m_raw[size], critical_pin::lock(m_invalidationpin));
#else
  return ptr<T>(&m_raw[size]);
#endif
}

template <typename T>
size_t vector<T>::size() const
{
  return m_size;
}

template <typename T>
void vector<T>::clear()
{
#ifdef SYS_DEBUG
  check_lock();
#endif

  for(ssize_t ei = m_size - 1; ei >= 0; --ei)
  {
    m_raw[ei].~T();
  }

  m_size = 0;
}

template <typename T>
void vector<T>::resize(size_t _size)
{
#ifdef SYS_DEBUG
  check_lock();
#endif

  if(_size > m_size)
  {
    allocate(_size);

    while(m_size != _size)
    {
      new(&m_raw[m_size]) T();
      ++m_size;
    }
  }
  else if(_size < m_size)
  {
    while(m_size != _size)
    {
      m_raw[m_size - 1].~T();
      --m_size;
    }
  }
}

template <typename T>
void vector<T>::erase(size_t _index)
{
  // TODO: Optimize. Improve iterator
  iterator it(begin());

  for(size_t i = 0; i < _index; ++i)
  {
    ++it;
  }

  erase(it);
}

template <typename T>
vector_iterator<T> vector<T>::erase(const vector_iterator<T>& _first, const vector_iterator<T>& _last)
{
  size_t first = _first.m_index;
  size_t last = _last.m_index;

  //_first = vector<T>::iterator();
  // const, can't assign. Variables are mutable though.
  _first.m_index = 0;
  _last.m_index = 0;
  _first.m_owner.reset();
  _last.m_owner.reset();
#ifdef SYS_DEBUG
  _first.m_lock = critical_pin::lock();
  _last.m_lock = critical_pin::lock();
#endif

  for(size_t i = first; i < last; ++i)
  {
    erase(first);
  }

  return begin() + first;
}

// TODO: Be careful if new fails, it is left in destructed state
// TODO: Can this accept const?
template <typename T>
vector_iterator<T> vector<T>::erase(const vector_iterator<T>& _it)
{
  if(_it.m_index >= m_size)
  {
    sys::panic("Attempt to erase end iterator");
  }

  if(!m_size)
  {
    sys::panic("Attempt to erase from zero sized vector");
  }

  size_t index = _it.m_index;
  //_it = vector<T>::iterator();
  _it.m_index = 0;
  _it.m_owner.reset();
#ifdef SYS_DEBUG
  _it.m_lock = critical_pin::lock();
#endif

  for(size_t i = index; i < m_size - 1; ++i)
  {
    // TODO: Which is best?
    //m_raw[i] = m_raw[i + 1];

    m_raw[i].~T();
    new(&m_raw[i]) T(m_raw[i + 1]);
  }

  resize(m_size - 1);

  // TODO: return begin() + index
  vector_iterator<T> rtn(begin());

  for(size_t i = 0; i < index; ++i)
  {
    ++rtn;
  }

  //_it = end();

  return rtn;
}

template <typename T>
vector_iterator<T> vector<T>::begin() const
{
  if(!m_size)
  {
    return end();
  }

  vector_iterator<T> rtn;

  rtn.m_index = 0;
  rtn.m_owner = this;
#ifdef SYS_DEBUG
  rtn.m_lock = critical_pin::lock(m_invalidationpin);
#endif

  return rtn;
}

template <typename T>
vector_iterator<T> vector<T>::end() const
{
  vector_iterator<T> rtn;

  rtn.m_index = m_size;
  rtn.m_owner = this;
#ifdef SYS_DEBUG
  rtn.m_lock = critical_pin::lock(m_invalidationpin);
#endif

  return rtn;
}

template <typename T>
vector_iterator<T>& vector_iterator<T>::operator++()
{
  if(m_index >= m_owner->size())
  {
    sys::panic("Iteration would go past end");
  }

  ++m_index;

  return *this;
}

template <typename T>
vector_iterator<T> vector_iterator<T>::operator+(size_t _value) const
{
  vector_iterator rtn(*this);

  for(size_t i = 0; i < _value; ++i)
  {
    ++rtn;
  }

  return rtn;
}

// TODO: Make this opposite of ==
template <typename T>
bool vector_iterator<T>::operator!=(const vector_iterator& _other) const
{
  bool rtn = m_index == _other.m_index &&
    m_owner == _other.m_owner;

  return !rtn;
}

template <typename T>
bool vector_iterator<T>::operator==(const vector_iterator& _other) const
{
  bool rtn = m_index == _other.m_index &&
    m_owner == _other.m_owner;

  return rtn;
}

#ifdef SYS_DEBUG
template <typename T>
ptr_lock<T> vector_iterator<T>::operator->() const
{
  if(m_index >= m_owner->size())
  {
    sys::panic("Iteration is past end");
  }

  return ptr_lock<T>(&m_owner->m_raw[m_index],
    critical_pin::lock(m_lock));
}

template <typename T>
ref_lock<T> vector_iterator<T>::operator*() const
{
  if(!m_owner)
  {
    sys::panic("Iterator is invalid");
  }

  if(m_index >= m_owner->size())
  {
    sys::panic("Iteration is past end");
  }

  return ref_lock<T>(&m_owner->m_raw[m_index],
    critical_pin::lock(m_lock));
}
#else
template <typename T>
T* vector_iterator<T>::operator->() const
{
  return &m_owner->m_raw[m_index];
}

template <typename T>
T& vector_iterator<T>::operator*() const
{
  return m_owner->m_raw[m_index];
}
#endif

#ifdef SYS_DEBUG
template <typename T>
void vector<T>::check_lock() const
{
  m_invalidationpin.reset();
}
#endif

template <typename T>
void vector<T>::allocate(size_t _size)
{
  if(m_total >= _size)
  {
    return;
  }

  size_t ns = 8;

  while(ns < _size)
  {
    ns *= 2;
  }

  size_t total = ns;
  T* raw = static_cast<T*>(calloc(total, sizeof(T)));
  size_t size = 0;

  if(!raw)
  {
    panic("Failed to allocate initial space");
  }

  try
  {
    for(size_t ei = 0; ei < m_size; ++ei)
    {
      new(&raw[ei]) T(m_raw[ei]);
      ++size;
    }
  }
  catch(...)
  {
    for(ssize_t ei = size - 1; ei >= 0; --ei)
    {
      raw[ei].~T();
    }

    free(raw);

    throw;
  }

  clear();
  free(m_raw);
  m_size = size;
  m_raw = raw;
  m_total = total;
}

template <typename T>
void vector<T>::sort(bool (*_func)(T&, T&))
{
  quicksort(0, size() - 1, _func);
}

template <typename T>
void vector<T>::swap(T& _a, T& _b)
{
  T t = _a;
  _a = _b;
  _b = t;
}

template <typename T>
bool vector<T>::compareFunc(T& _a, T& _b)
{
  if(_a > _b)
  {
    return true;
  }

  return false;
}

template <typename T>
int vector<T>::partition(int _low, int _high, bool (*_func)(T&, T&))
{
  int i = _low - 1;
  vector<T>& self = *this;

  for(int j = _low; j <= _high - 1; ++j)
  {
    if(_func(self[j], self[_high]))
    {
      ++i;
      swap(self[i], self[j]);
    }
  }

  swap(self[i + 1], self[_high]);

  return i + 1;
}

template <typename T>
T* vector<T>::unsafe_raw() const
{
  return &m_raw[0];
}

template <typename T>
void vector<T>::quicksort(int _low, int _high, bool (*_func)(T&, T&))
{
  if(_low < _high)
  {
    int pivot = partition(_low, _high, _func);

    quicksort(_low, pivot - 1, _func);
    quicksort(pivot + 1, _high, _func);
  }
}

}

#endif

