#ifndef SYS_LINKEDLIST_H
#define SYS_LINKEDLIST_H

#include "autoptr.h"
#include "ptr.h"
#include "config.h"
#include "base.h"

namespace sys
{

#ifdef SYS_DEBUG
template <typename T> struct ref_lock;
template <typename T> struct ptr_lock;
#endif

template <typename T>
struct linked_list : base
{
private:
  struct node : base
  {
    node();
    node(const T& _value);

    template <typename A> node(A _a);
    template <typename A, typename B> node(A _a, B _b);

    T m_value;
    auto_ptr<node> m_next;
    ptr<node> m_prev;
  };

public:
  struct iterator
  {
    iterator& operator++();
    bool operator!=(const iterator& _other) const;
    bool operator==(const iterator& _other) const;

#ifdef SYS_DEBUG
    ptr_lock<T> operator->() const;
    ref_lock<T> operator*() const;
#else
    T* operator->() const;
    T& operator*() const;
#endif

  private:
    template <typename U>
    friend struct linked_list;

    mutable ptr<node> m_node;
  };

  linked_list();
  linked_list(const linked_list& _copy);
  linked_list& operator=(const linked_list& _assign);
  ~linked_list();

  ptr<T> push_back(const T& _value);

  iterator emplace_back();
  template <typename A> iterator emplace_back(A _a);
  template <typename A, typename B> iterator emplace_back(A _a, B _b);

  void clear();
  iterator begin() const;
  iterator end() const;
  iterator erase(const iterator& _it);

private:
  auto_ptr<node> m_first;
  ptr<node> m_last;

};

/* Implementation */

#include "panic.h"

template <typename T>
linked_list<T>::node::node()
{ }

template <typename T>
linked_list<T>::node::node(const T& _value)
  : m_value(_value)
{ }

template <typename T>
template <typename A> linked_list<T>::node::node(A _a)
  : m_value(_a)
{ }

template <typename T>
template <typename A, typename B> linked_list<T>::node::node(A _a, B _b)
  : m_value(_a, _b)
{ }

#define REF_LIST_ATTACH           \
  if(m_last)                      \
  {                               \
    strong->m_prev = m_last;      \
    m_last->m_next = strong;      \
    m_last = weak;                \
  }                               \
  else                            \
  {                               \
    m_first = strong;             \
    m_last = weak;                \
  }

/***************************************************************************
 t
 t  sys::linked_list<Employee> emps;
 t  emps.push_back(Employee());
 t
 t  sys::linked_list<const Employee> cemps;
 t  cemps.push_back(Employee());
 t
 **************************************************************************/
template <typename T>
ptr<T> linked_list<T>::push_back(const T& _value)
{
  //auto_ptr<node> strong = make_auto<typename linked_list<T>::node>(_value);
  auto_ptr<node> strong = make_auto<node>(_value);
  ptr<node> weak(strong);

  REF_LIST_ATTACH

  ptr<T> rtn(&weak->m_value
#ifdef SYS_DEBUG
  , weak->critical_pin_lock()
#endif
  );

  return rtn;
}

/***************************************************************************
 t
 t sys::linked_list<Employee> emps;
 t sys::linked_list<Employee>::iterator it = emps.emplace_back();
 t it->id(9);
 t it->id();
 t
 t sys::linked_list<const Employee> cemps;
 t sys::linked_list<const Employee>::iterator cit = cemps.emplace_back();
 t cit->id();
 t
 **************************************************************************/
template <typename T>
typename linked_list<T>::iterator linked_list<T>::emplace_back()
{
  auto_ptr<node> strong = make_auto<typename linked_list<T>::node>();
  ptr<node> weak(strong);

  REF_LIST_ATTACH

  iterator rtn;
  rtn.m_node = weak;

  return rtn;
}

template <typename T>
template <typename A>
typename linked_list<T>::iterator linked_list<T>::emplace_back(A _a)
{
  auto_ptr<node> strong = make_auto<typename linked_list<T>::node>(_a);
  ptr<node> weak(strong);

  REF_LIST_ATTACH

  iterator rtn;
  rtn.m_node = weak;

  return rtn;
}

template <typename T>
template <typename A, typename B>
typename linked_list<T>::iterator linked_list<T>::emplace_back(A _a, B _b)
{
  auto_ptr<node> strong = make_auto<typename linked_list<T>::node>(_a, _b);
  ptr<node> weak(strong);

  REF_LIST_ATTACH

  iterator rtn;
  rtn.m_node = weak;

  return rtn;
}

/***************************************************************************
 t
 t sys::linked_list<Employee> emps;
 t
 **************************************************************************/
template <typename T>
linked_list<T>::linked_list()
{ }

/***************************************************************************
 t
 t sys::linked_list<Employee> emps;
 t sys::linked_list<Employee>::iterator it = emps.emplace_back();
 t emps.emplace_back();
 t sys::linked_list<Employee> emps2(emps);
 t sys::linked_list<Employee>::iterator it2 = emps2.emplace_back();
 t
 **************************************************************************/
template <typename T>
linked_list<T>::linked_list(const linked_list& _copy)
{
  for(typename linked_list::iterator it = _copy.begin();
    it != _copy.end(); ++it)
  {
    push_back(*it);
  }
}

/***************************************************************************
 t
 t sys::linked_list<Employee> emps;
 t emps.emplace_back();
 t emps.emplace_back();
 t sys::linked_list<Employee> emps2;
 t emps2.emplace_back();
 t emps2 = emps;
 t
 **************************************************************************/
template <typename T>
linked_list<T>& linked_list<T>::operator=(const linked_list& _assign)
{
  auto_ptr<node> firstsink(m_first);
  m_last.reset();

  for(typename linked_list::iterator it = _assign.begin();
    it != _assign.end(); ++it)
  {
    push_back(*it);
  }

  return *this;
}

/***************************************************************************
 t
 t sys::linked_list<Employee> emps;
 t
 **************************************************************************/
template <typename T>
linked_list<T>::~linked_list()
{
  clear();
}

/***************************************************************************
 t
 t sys::linked_list<Employee> emps;
 t emps.clear();
 t emps.emplace_back();
 t emps.emplace_back();
 t emps.clear();
 t emps.emplace_back();
 t emps.clear();
 t emps.clear();
 **************************************************************************/
template <typename T>
void linked_list<T>::clear()
{
  m_last.reset();
  m_first.reset();
}

/***************************************************************************
 t
 t sys::linked_list<Employee> emps;
 t emps.emplace_back();
 t emps.emplace_back();
 t sys::linked_list<Employee>::iterator it = emps.begin();
 t ++it;
 t ++it;
 t
 t if(it == emps.end()) { }
 t else abort();
 t
 **************************************************************************/
template <typename T>
typename linked_list<T>::iterator& linked_list<T>::iterator::operator++()
{
  if(!m_node)
  {
    sys::panic("Invalid iterator or past end");
  }

  ptr<node> next = m_node->m_next;

  m_node = next;

  return *this;
}

/***************************************************************************
 t
 t sys::linked_list<Employee> emps;
 t emps.emplace_back();
 t sys::linked_list<Employee>::iterator ita = emps.emplace_back();
 t sys::linked_list<Employee>::iterator itb = emps.begin();
 t
 t if(ita != itb) { }
 t else abort();
 t
 **************************************************************************/
template <typename T>
bool linked_list<T>::iterator::operator!=(const iterator& _other) const
{
  bool rtn = m_node == _other.m_node;

  return !rtn;
}

/***************************************************************************
 t
 t sys::linked_list<Employee> emps;
 t sys::linked_list<Employee>::iterator ita = emps.emplace_back();
 t sys::linked_list<Employee>::iterator itb = emps.begin();
 t
 t if(ita == itb) { }
 t else abort();
 t
 **************************************************************************/
template <typename T>
bool linked_list<T>::iterator::operator==(const iterator& _other) const
{
  bool rtn = m_node == _other.m_node;

  return rtn;
}


/***************************************************************************
 t
 t sys::linked_list<Employee> emps;
 t sys::linked_list<Employee>::iterator it = emps.emplace_back();
 t it->id(9);
 t it->id();
 t
 t sys::linked_list<const Employee> cemps;
 t sys::linked_list<const Employee>::iterator cit = cemps.emplace_back();
 t cit->id();
 t
 **************************************************************************/
#ifdef SYS_DEBUG
template <typename T>
ptr_lock<T> linked_list<T>::iterator::operator->() const
{
  return ptr_lock<T>(&m_node->m_value, m_node.critical_pin_lock());
}
#else
template <typename T>
T* linked_list<T>::iterator::operator->() const
{
  return &m_node->m_value;
}
#endif

/***************************************************************************
 t
 t sys::linked_list<Employee> emps;
 t sys::linked_list<Employee>::iterator it = emps.emplace_back();
 t EmployeeId(*it, 9);
 t EmployeeId(*it);
 t
 t sys::linked_list<const Employee> cemps;
 t sys::linked_list<const Employee>::iterator cit = cemps.emplace_back();
 t EmployeeId(*it);
 t
 **************************************************************************/
#ifdef SYS_DEBUG
template <typename T>
ref_lock<T> linked_list<T>::iterator::operator*() const
{
  return ref_lock<T>(&m_node->m_value, m_node.critical_pin_lock());
}
#else
template <typename T>
T& linked_list<T>::iterator::operator*() const
{
  return m_node->m_value;
}
#endif

/***************************************************************************
 t
 t sys::linked_list<Employee> emps;
 t if(emps.begin() != emps.end()) abort();
 t sys::linked_list<Employee>::iterator it = emps.emplace_back();
 t if(emps.begin() == emps.end()) abort();
 t if(emps.begin() != it) abort();
 t
 **************************************************************************/
template <typename T>
typename linked_list<T>::iterator linked_list<T>::begin() const
{
  if(!m_first)
  {
    return end();
  }

  iterator rtn;
  rtn.m_node = m_first;

  return rtn;
}

/***************************************************************************
 t
 t sys::linked_list<Employee> emps;
 t if(emps.end() != emps.begin()) abort();
 t sys::linked_list<Employee>::iterator it = emps.emplace_back();
 t if(emps.end() == emps.begin()) abort();
 t if(emps.end() == it) abort();
 t
 **************************************************************************/
template <typename T>
typename linked_list<T>::iterator linked_list<T>::end() const
{
  iterator rtn;

  return rtn;
}

/***************************************************************************
 t
 t sys::linked_list<Employee> emps;
 t sys::linked_list<Employee>::iterator it = emps.emplace_back();
 t it = emps.erase(it);
 t if(emps.begin() != emps.end()) abort();
 t if(it != emps.end()) abort();
 t it = emps.emplace_back();
 t sys::linked_list<Employee>::iterator it2 = emps.emplace_back();
 t it = emps.erase(it);
 t if(emps.begin() != it2) abort();
 t if(it != emps.begin()) abort();
 t if(it != it2) abort();
 t
 **************************************************************************/
template <typename T>
typename linked_list<T>::iterator linked_list<T>::erase(const iterator& _it)
{
  sys::auto_ptr<node> life;
  ptr<node> curr = _it.m_node;

  iterator rtn(_it);
  ++rtn;

  if(curr->m_prev && curr->m_next)
  {
    life = curr->m_prev->m_next;
    // Prev is safe due to ptr<T>. Do it first
    curr->m_next->m_prev = curr->m_prev;
    curr->m_prev->m_next = curr->m_next;
  }
  else if(curr->m_prev)
  {
    life = curr->m_prev->m_next;
    curr->m_prev->m_next.reset();
  }
  else if(curr->m_next)
  {
    curr->m_next->m_prev.reset();
  }

  if(!curr->m_prev)
  {
    life = m_first;
    m_first = curr->m_next;
  }

  if(!curr->m_next)
  {
    m_last = curr->m_prev;
  }

  _it.m_node.reset();

  return rtn;
}

}

#endif

