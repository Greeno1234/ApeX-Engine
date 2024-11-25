#ifndef SYS_FILE_H
#define SYS_FILE_H

#include "memory.h"
#include "string.h"

#include <stdio.h>

namespace sys
{

template <typename T> struct vector;

struct file
{
  file();
  file(const string& _path);
  ~file();

  void close();
  void open(const string& _path);
  size_t read(vector<unsigned char>& _output);
  void write(vector<unsigned char>& _data, size_t _size);
  void write_line(const string& _line = "");
  bool read_line(string& _line);

private:
  string m_path;
  zero<int> m_mode;
  zero<FILE *> m_raw;

  file(const file& _copy);
  file& operator=(const file& _other);

  void open_read();
  void open_write();

};

}

/* Implementation */

#include "exception.h"
#include "panic.h"
#include "vector.h"
#include "config.h"

namespace sys
{

inline file::file()
{ }

inline file::file(const string& _path)
  : m_path(_path)
{ }

inline file::~file()
{
  if(m_raw)
  {
    fclose(m_raw);
  }
}

inline void file::close()
{
  if(m_raw)
  {
    fclose(m_raw);
    m_raw = NULL;
  }

  m_path = "";
  m_mode = 0;
}

inline void file::open(const string& _path)
{
  close();
  m_path = _path;
}

inline size_t file::read(vector<unsigned char>& _output)
{
  if(m_mode != 1)
  {
    open_read();
  }

  return fread(_output.unsafe_raw(), sizeof(_output[0]), _output.size(), m_raw);
}

inline void file::write(vector<unsigned char>& _data, size_t _size)
{
  if(_size > _data.size()) sys::panic("Invalid size to write");

  if(m_mode != 2)
  {
    open_write();
  }

  if(fwrite(_data.unsafe_raw(),
    sizeof(_data.unsafe_raw()[0]), _size, m_raw) != _size)
  {
    throw exception("Failed to write");
  }
}

inline void file::write_line(const string& _line)
{
  if(m_mode != 2)
  {
    open_write();
  }

  if(fputs(_line.unsafe_raw(), m_raw) < 0)
  {
    throw exception("Failed to write");
  }

  if(fputs("\n", m_raw) < 0)
  {
    throw exception("Failed to write EOL");
  }
}

inline bool file::read_line(string& _line)
{
  char buffer[SYS_BUFFER] = { 0 };

  if(m_mode != 1)
  {
    open_read();
  }

  _line.clear();

  while(true)
  {
    char *res = fgets(buffer, sizeof(buffer), m_raw);

    // If error reading; check if caused by end of file, if data has been
    // read up to this point return true or fall through to return false.
    // If a read error is flagged, throw.
    if(!res)
    {
      if(feof(m_raw))
      {
        if(_line.length() > 0)
        {
          return true;
        }

        return false;
      }

      throw exception("Error reading file");
    }

    for(size_t ci = 0; ci < sizeof(buffer); ++ci)
    {
      char c = buffer[ci];

      if(c == '\0')
      {
        break; // End of buffer reached, no new line in this read
      }
      else if(c == '\r')
      {
        continue; // Ignore Win32 char
      }
      else if(c == '\n')
      {
        return true; // End of line reached, return success
      }
      else
      {
        _line += c;
      }
    }
  }

  sys::panic("Outcomes feof, ferror or '\n' should have been encountered");
}

inline void file::open_read()
{
  if(m_raw)
  {
    fclose(m_raw);
  }

  m_mode = 0;
#ifdef _MSC_VER
  if(fopen_s(&m_raw, m_path.unsafe_raw(), "rb"))
  {
    throw exception(string() + "Failed to open file '" + m_path + "' for reading");
  }
#else
  m_raw = fopen(m_path.unsafe_raw(), "rb");
  if(!m_raw) throw exception(string() + "Failed to open file '" + m_path + "' for reading");
#endif

  m_mode = 1;
}

inline void file::open_write()
{
  if(m_raw)
  {
    fclose(m_raw);
  }

  m_mode = 0;
#ifdef _MSC_VER
  if(fopen_s(&m_raw, m_path.unsafe_raw(), "wb"))
  {
    throw exception(string() + "Failed to open file '" + m_path + "' for writing");
  }
#else
  m_raw = fopen(m_path.unsafe_raw(), "wb");
  if(!m_raw) throw exception(string() + "Failed to open file '" + m_path + "' for writing");
#endif
  m_mode = 2;
}

}

#endif

