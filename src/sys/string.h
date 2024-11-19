#ifndef SYS_STRING_H
#define SYS_STRING_H

#include "base.h"
#include "vector.h"
#include "config.h"

#include <string.h>

#define SYS_STRING_T(T)             \
  string(T _value);                 \
  string& operator=(T _value);      \
  string& operator+=(T _value);     \
  string operator+(T _value) const;

namespace sys
{

struct string : base
{
  string();
  //string(const string& _copy);
  //string& operator=(const string& _assign);

  string& operator+=(const string& _value);
  string operator+(const string& _value) const;

  string(const char* _value);
  string& operator=(const char* _value);
  string& operator+=(const char* _value);
  string operator+(const char* _value) const;

  string(char _value);
  string& operator+=(char _value);
  string operator+(char _value) const;

  SYS_STRING_T(int)
  SYS_STRING_T(long)
  //SYS_STRING_T(long long)
  SYS_STRING_T(unsigned)
  SYS_STRING_T(unsigned long)
  //SYS_STRING_T(unsigned long long)
  SYS_STRING_T(float)
  SYS_STRING_T(double)
  SYS_STRING_T(long double)

  bool operator==(const string& _other) const;
  bool operator!=(const string& _other) const;
  bool operator<(const string& _other) const;
  bool operator>(const string& _other) const;
  bool operator<=(const string& _other) const;
  bool operator>=(const string& _other) const;

#ifdef SYS_DEBUG
  char& operator[](const index_lock& _index);
  const char& operator[](const index_lock& _index) const;
#else
  char& operator[](size_t _index);
  const char& operator[](size_t _index) const;
#endif

  size_t length() const;
  void resize(size_t _newsize);
  void clear();
  const char* unsafe_raw() const;
  string substring(size_t _begin, size_t _count) const;
  string substring(size_t _begin) const;

private:
  vector<char> m_data;

};

//inline string operator+(const string& _a, const string& _b);
inline void panic(const string& _message);
inline void out(const string& _message);
inline void err(const string& _message);

}

/* Implementation */

#include "panic.h"

#define SYS_STRING_T_IMPL(T, S)                   \
  inline string::string(T _value)                 \
  {                                               \
    m_data.push_back(0);                          \
    *this += _value;                              \
  }                                               \
                                                  \
  inline string& string::operator=(T _value)      \
  {                                               \
    char buffer[SYS_STRING_BUFFER];               \
    snprintf(buffer, sizeof(buffer), S, _value);  \
    *this = buffer;                               \
                                                  \
    return *this;                                 \
  }                                               \
                                                  \
  inline string& string::operator+=(T _value)     \
  {                                               \
    char buffer[SYS_STRING_BUFFER];               \
    snprintf(buffer, sizeof(buffer), S, _value);  \
    *this += buffer;                              \
                                                  \
    return *this;                                 \
  }                                               \
                                                  \
  inline string string::operator+(T _value) const \
  {                                               \
    string rtn(*this);                            \
    rtn += _value;                                \
                                                  \
    return rtn;                                   \
  }

#define SYS_TAKE_RAW                 \
  size_t len = strlen(_value);       \
  m_data.resize(len + 1);            \
                                     \
  for(size_t ci = 0; ci < len; ++ci) \
  {                                  \
    m_data[ci] = _value[ci];         \
  }                                  \
                                     \
  m_data[m_data.size() - 1] = 0;     \

namespace sys
{

/***************************************************************************
 t
 t sys::string a;
 t sys::string b = sys::string();
 t
 t if(a != "") abort();
 t
 ***************************************************************************/
inline string::string()
{
  m_data.push_back(0);
}

/***************************************************************************
 t
 t sys::string a('c');
 t if(a != "c") abort();
 t sys::string b = sys::string('c');
 t if(b != "c") abort();
 t sys::string c = 'c';
 t if(c != "c") abort();
 t
 t
 ***************************************************************************/
inline string::string(char _value)
{
  m_data.push_back(_value);
  m_data.push_back(0);
}

/***************************************************************************
 t
 t sys::string a;
 t a += 'f';
 t a += 'o';
 t a += 'o';
 t
 t if(a != "foo") abort();
 t
 ***************************************************************************/
inline string& string::operator+=(char _value)
{
  m_data[m_data.size() - 1] = _value;
  m_data.push_back(0);

  return *this;
}

/***************************************************************************
 t
 t sys::string a;
 t sys::string b;
 t b = a + 'f';
 t b = a + 'f';
 t
 t if(a != "") abort();
 t if(b != "f") abort();
 t
 ***************************************************************************/
inline string string::operator+(char _value) const
{
  string rtn(*this);
  rtn += _value;

  return rtn;
}

/***************************************************************************
 t
 t sys::string a("foo");
 t sys::string b("bar");
 t a += b;
 t
 t if(a != "foobar") abort();
 t
 ***************************************************************************/
inline string& string::operator+=(const string& _value)
{
  size_t len = _value.length();
  size_t cs = length();

  m_data.resize(cs + len + 1);

  for(size_t ci = 0; ci < len; ++ci)
  {
    m_data[cs + ci] = _value[ci];
  }

  m_data[m_data.size() - 1] = 0;

  return *this;
}

/***************************************************************************
 t
 t sys::string a("foo");
 t sys::string b("bar");
 t a = a + b;
 t
 t if(a != "foobar") abort();
 t
 ***************************************************************************/
inline string string::operator+(const string& _value) const
{
  string rtn(*this);
  rtn += _value;

  return rtn;
}

/***************************************************************************
 t
 t sys::string a("foobar");
 t if(a != "foobar") abort();
 t sys::string b = sys::string("foobar");
 t if(b != "foobar") abort();
 t sys::string c = "foobar";
 t if(c != "foobar") abort();
 t
 ***************************************************************************/
inline string::string(const char* _value)
{
  SYS_TAKE_RAW
}

/***************************************************************************
 t
 t sys::string a;
 t a = "foo";
 t a = "foobar";
 t
 t if(a != "foobar") abort();
 t
 ***************************************************************************/
inline string& string::operator=(const char* _value)
{
  SYS_TAKE_RAW

  return *this;
}

/***************************************************************************
 t
 t sys::string a("foo");
 t a += "bar";
 t
 t if(a != "foobar") abort();
 t
 ***************************************************************************/
inline string& string::operator+=(const char* _value)
{
  size_t len = strlen(_value);
  size_t cs = length();

  m_data.resize(cs + len + 1);

  for(size_t ci = 0; ci < len; ++ci)
  {
    m_data[cs + ci] = _value[ci];
  }

  m_data[m_data.size() - 1] = 0;

  return *this;
}

/***************************************************************************
 t
 t sys::string a("foo");
 t a = a + "bar";
 t
 t if(a != "foobar") abort();
 t
 ***************************************************************************/
inline string string::operator+(const char* _value) const
{
  string rtn(*this);
  rtn += _value;

  return rtn;
}

/***************************************************************************
 t
 t int i = 1;
 t long ld = 2;
 t unsigned u = 3;
 t unsigned long lu = 4;
 t float f = 5;
 t double lf = 6;
 t long double Lf = 7;
 t
 t sys::string a;
 t a += i;
 t a += ld;
 t a += u;
 t a += lu;
 t a += f;
 t a += lf;
 t a += Lf;
 t
 ***************************************************************************/
SYS_STRING_T_IMPL(int, "%i")
SYS_STRING_T_IMPL(long, "%ld")
//SYS_STRING_T_IMPL(long long, "%lld")
SYS_STRING_T_IMPL(unsigned, "%u")
SYS_STRING_T_IMPL(unsigned long, "%lu")
//SYS_STRING_T_IMPL(unsigned long long, "%llu")
SYS_STRING_T_IMPL(float, "%f")
SYS_STRING_T_IMPL(double, "%lf")
SYS_STRING_T_IMPL(long double, "%Lf")

/***************************************************************************
 t
 t sys::string a("foo");
 t sys::string b("foo");
 t
 t if(a == b) { }
 t else abort();
 t
 ***************************************************************************/
inline bool string::operator==(const string& _other) const
{
  if(strcmp(m_data.unsafe_raw(), _other.m_data.unsafe_raw()) == 0)
  {
    return true;
  }

  return false;
}

/***************************************************************************
 t
 t sys::string a("foo");
 t sys::string b("bar");
 t
 t if(a != b) { }
 t else abort();
 t
 ***************************************************************************/
inline bool string::operator!=(const string& _other) const
{
  return !(*this == _other);
}

/***************************************************************************
 t
 t sys::string a("aaa");
 t sys::string b("bbb");
 t
 t if(a < b) { }
 t else abort();
 t
 ***************************************************************************/
inline bool string::operator<(const string& _other) const
{
  if(strcmp(m_data.unsafe_raw(), _other.m_data.unsafe_raw()) < 0)
  {
    return true;
  }

  return false;
}

/***************************************************************************
 t
 t sys::string a("bbb");
 t sys::string b("aaa");
 t
 t if(a > b) { }
 t else abort();
 t
 ***************************************************************************/
inline bool string::operator>(const string& _other) const
{
  if(strcmp(m_data.unsafe_raw(), _other.m_data.unsafe_raw()) > 0)
  {
    return true;
  }

  return false;
}

/***************************************************************************
 t
 t sys::string a("aaa");
 t sys::string b("bbb");
 t sys::string c("aaa");
 t
 t if(a <= b) { }
 t else abort();
 t if(a <= c) { }
 t else abort();
 t
 ***************************************************************************/
inline bool string::operator<=(const string& _other) const
{
  if(strcmp(m_data.unsafe_raw(), _other.m_data.unsafe_raw()) <= 0)
  {
    return true;
  }

  return false;
}

/***************************************************************************
 t
 t sys::string a("bbb");
 t sys::string b("aaa");
 t sys::string c("bbb");
 t
 t if(a >= b) { }
 t else abort();
 t if(a >= c) { }
 t else abort();
 t
 ***************************************************************************/
inline bool string::operator>=(const string& _other) const
{
  if(strcmp(m_data.unsafe_raw(), _other.m_data.unsafe_raw()) >= 0)
  {
    return true;
  }

  return false;
}

/***************************************************************************
 t
 t sys::string a("foo");
 t
 t if(a[0] != 'f') abort();
 t if(a[2] != 'o') abort();
 t a[0] = 'o';
 t a[2] = 'f';
 t if(a[2] != 'f') abort();
 t if(a[0] != 'o') abort();

 t const sys::string c("foo");
 t if(c[0] != 'f') abort();
 t if(c[2] != 'o') abort();
 t
 ***************************************************************************/
#ifdef SYS_DEBUG
inline char& string::operator[](const index_lock& _index)
{
  if(_index >= length())
  {
    string msg("Character index out of bounds");
    // TODO: Ambiguous + / += with int/size_t/etc
    msg += " [index=";
    msg += (int)_index;
    msg += ", length=";
    msg += (int)length();
    msg += "]";
    //msg = msg + " [index=" + _index + ", length=" + length() + "]";
    panic(msg);
  }

  return m_data[_index];
}

inline const char& string::operator[](const index_lock& _index) const
{
  if(_index >= length())
  {
    panic("Character (const) index out of bounds");
  }

  return m_data[_index];
}
#else
inline char& string::operator[](size_t _index)
{
  return m_data[_index];
}

inline const char& string::operator[](size_t _index) const
{
  return m_data[_index];
}
#endif

/***************************************************************************
 t
 t sys::string a("foo");
 t if(a.length() != 3) abort();
 t a += "bar";
 t if(a.length() != 6) abort();
 t a += "baz";
 t if(a.length() != 9) abort();
 t a = "";
 t if(a.length() != 0) abort();
 t
 ***************************************************************************/
inline size_t string::length() const
{
  return m_data.size() - 1;
}

/***************************************************************************
 t
 t sys::string a("foo");
 t a.resize(6);
 t if(a.length() != 6) abort();
 t
 ***************************************************************************/
inline void string::resize(size_t _newsize)
{
  m_data.resize(_newsize + 1);
  m_data[_newsize] = 0;
}

/***************************************************************************
 t
 t sys::string a("foo");
 t a.clear();
 t if(a.length() != 0) abort();
 t
 ***************************************************************************/
inline void string::clear()
{
  m_data.resize(1);
  m_data[0] = 0;
}

inline const char* string::unsafe_raw() const
{
  return m_data.unsafe_raw();
}

/***************************************************************************
 t
 t sys::string a("foobarbaz");
 t
 t sys::string foo = a.substring(0, 3);
 t sys::string bar = a.substring(3, 3);
 t sys::string baz = a.substring(6, 3);
 t
 t if(foo != "foo") abort();
 t if(bar != "bar") abort();
 t if(baz != "baz") abort();
 t
 ***************************************************************************/
inline string string::substring(size_t _begin, size_t _count) const
{
  if(_begin + _count > length())
  {
    sys::panic("Substring out of bounds");
  }

  string rtn;

  for(size_t i = _begin; i < _begin + _count; ++i)
  {
    rtn += m_data[i];
  }

  return rtn;
}

/***************************************************************************
 t
 t sys::string a("foobarbaz");
 t
 t sys::string foo = a.substring(0);
 t sys::string bar = a.substring(3);
 t sys::string baz = a.substring(6);
 t
 t if(foo != "foobarbaz") abort();
 t if(bar != "barbaz") abort();
 t if(baz != "baz") abort();
 t
 ***************************************************************************/
inline string string::substring(size_t _begin) const
{
  if(_begin > length())
  {
    sys::panic("Substring begins out of bounds");
  }

  return substring(_begin, length() - _begin);
}

inline void panic(const string& _message)
{
  printf("Panic: %s\n", _message.unsafe_raw());
  abort();
}

inline void out(const string& _message)
{
  fprintf(stdout, "%s", _message.unsafe_raw());
}

inline void err(const string& _message)
{
  fprintf(stderr, "%s", _message.unsafe_raw());
}

/*
inline string operator+(const string& _a, const string& _b)
{
  string rtn(_a);
  rtn += _b;

  return rtn;
}

inline string to_string(int _value)
{
  char buffer[SYS_STRING_BUFFER];

  snprintf(buffer, sizeof(buffer), "%i", _value);

  return buffer;
}

inline string to_string(double _value)
{
  char buffer[SYS_STRING_BUFFER];

  snprintf(buffer, sizeof(buffer), "%f", _value);

  return buffer;
}
*/

inline string operator+(const char* _a, const string& _b)
{
  string rtn(_a);
  rtn += _b;

  return rtn;
}

inline string operator+(char _a, const string& _b)
{
  string rtn(_a);
  rtn += _b;

  return rtn;
}

inline string operator+(int _a, const string& _b)
{
  string rtn(_a);
  rtn += _b;

  return rtn;
}


}

#endif

