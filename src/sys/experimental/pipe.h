#ifndef SYS_PIPE_H
#define SYS_PIPE_H

#include "Zero.h"
#include "String.h"
#include "Exception.h"
#include "panic.h"
#include "Vector.h"
#include "config.h"

#include <stdio.h>

namespace sys
{

struct Pipe
{
  Pipe() { }

  Pipe(const String& _command)
  {
    open(_command);
  }

  ~Pipe()
  {
    if(m_raw)
    {
      pclose(m_raw);
    }
  }

  void open(const String& _command)
  {
    if(m_raw)
    {
      pclose(m_raw);
    }

    m_raw = popen(_command.unsafe_raw(), "r");

    if(!m_raw)
    {
      throw sys::Exception(sys::String() + "Failed to open pipe '" + _command + "'");
    }
  }

  void close()
  {
    if(!m_raw)
    {
      throw sys::Exception("Pipe is not open");
    }

    int res = pclose(m_raw);
    m_raw = NULL;

    if(res != 0)
    {
      throw sys::Exception("Error on closing pipe");
    }
  }

  size_t read(Vector<unsigned char>& _output)
  {
    if(!m_raw)
    {
      throw sys::Exception("Pipe is not open");
    }

    return fread(_output.unsafe_raw(), sizeof(_output[0]), _output.size(), m_raw);
  }

  bool read_line(String& _line)
  {
    if(!m_raw)
    {
      throw sys::Exception("Pipe is not open");
    }

    char buffer[SYS_BUFFER] = { 0 };

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

        throw Exception("Error reading file");
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

private:
  Zero<FILE *> m_raw;

  Pipe(const Pipe& _copy);
  Pipe& operator=(const Pipe& _other);

};

}

#endif

