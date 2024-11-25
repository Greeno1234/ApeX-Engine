#ifndef SYS_EXCEPTION_H
#define SYS_EXCEPTION_H

#include "string.h"

namespace sys
{

struct exception
{
  exception();
  exception(const string& _message);
  virtual ~exception();

#ifdef SYS_DEBUG
  ref_lock<const string> what() const;
#else
  const string& what() const;
#endif
  void panic() const;

private:
  string m_message;

};

}

/* Implementation */

#include "io.h"
#include "panic.h"
#include "config.h"
#include "lock.h"

namespace sys
{

/***************************************************************************
 t
 t try
 t {
 t   throw sys::exception();
 t }
 t catch(sys::exception& e)
 t { }
 t
 **************************************************************************/
inline exception::exception()
{ }

/***************************************************************************
 t
 t try
 t {
 t   throw sys::exception("Sample");
 t }
 t catch(sys::exception& e)
 t { }
 t
 **************************************************************************/
inline exception::exception(const string& _message)
  : m_message(_message)
{
#ifdef SYS_DEBUG
  out(string("[sys] throw Exception: ") + _message + '\n');
#endif
}

inline exception::~exception()
{ }

/***************************************************************************
 t
 t try
 t {
 t   throw sys::exception("Sample");
 t }
 t catch(sys::exception& e)
 t {
 t   //sys::out(e.what() + "\n");
 t   e.what() + "\n";
 t }
 t
 **************************************************************************/
#ifdef SYS_DEBUG
inline ref_lock<const string> exception::what() const
{
  return ref_lock<const string>(&m_message, m_message.critical_pin_lock());
}
#else
inline const string& exception::what() const
{
  return m_message;
}
#endif

inline void exception::panic() const
{
  sys::panic(m_message);
}

}

#endif

