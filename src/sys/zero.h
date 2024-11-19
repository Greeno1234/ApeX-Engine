#ifndef SYS_ZERO_H
#define SYS_ZERO_H

namespace sys
{

/***************************************************************************
 * zero
 *
 * Provides a way for member variables to be initialized to zero implicitly
 * rather than via initializer lists. Acts as a template wrapper class which
 * forwards through the contained internal variable but also initializing to
 * zero in its constructor.
 ***************************************************************************/
template <typename T>
struct zero
{
  zero();

  zero(const T& _value);
  zero& operator=(const T& _value);

  operator T&();
  operator const T&() const;

  T* operator&();
  const T* operator&() const;

  T operator->();
  const T operator->() const;

  T& get();
  const T& get() const;

private:
  T m_value;
};

}

/* Implementation */

namespace sys
{

/***************************************************************************
 t
 t  sys::zero<int> i;
 t  sys::zero<const int> ci;
 t
 **************************************************************************/
template <typename T>
zero<T>::zero() :
  m_value(0)
{ }

/***************************************************************************
 t
 t  sys::zero<int> i(1);
 t  sys::zero<const int> ci(1);
 t
 **************************************************************************/
template <typename T>
zero<T>::zero(const T& _value) :
  m_value(_value)
{ }

/***************************************************************************
 t
 t  sys::zero<int> i;
 t  i = 1;
 t
 **************************************************************************/
template <typename T>
zero<T>& zero<T>::operator=(const T& _value)
{
  m_value = _value;

  return *this;
}

/***************************************************************************
 t
 t  sys::zero<int> i;
 t  if(i) { }
 t  i = 9;
 t  int ri(i);
 t  ++i;
 t  i++;
 t  i += 2;
 t  i = i + 1;
 t  const sys::zero<int> ci(i);
 t
 **************************************************************************/
template <typename T>
zero<T>::operator T&()
{
  return m_value;
}

/***************************************************************************
 t
 t  sys::zero<const int> ci(1);
 t  const sys::zero<int> ci2(ci);
 t  const int rci(ci);
 t  int ri(ci);
 t
 **************************************************************************/
template <typename T>
zero<T>::operator const T&() const
{
  return m_value;
}

/***************************************************************************
 t
 t  sys::zero<int> i(1);
 t  int* pi(&i);
 t  pi = &i;
 t
 **************************************************************************/
template <typename T>
T* zero<T>::operator&()
{
  return &m_value;
}

/***************************************************************************
 t
 t  sys::zero<const int> ci(1);
 t  const int* pi(&ci);
 t  pi = &ci;
 t
 **************************************************************************/
template <typename T>
const T* zero<T>::operator&() const
{
  return &m_value;
}

template <typename T>
T zero<T>::operator->()
{
  return m_value;
}

template <typename T>
const T zero<T>::operator->() const
{
  return m_value;
}

template <typename T>
T& zero<T>::get()
{
  return m_value;
}

template <typename T>
const T& zero<T>::get() const
{
  return m_value;
}

}

#endif

