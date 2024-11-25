#include "utils.h"

namespace rend
{

void split_string_whitespace(const sys::string& _input,
  sys::vector<sys::string>& _output)
{
  _output.clear();
  sys::string curr;

  for(size_t ci = 0; ci < _input.length(); ++ci)
  {
    char c = _input[ci];

    if(c == '\r' || c == '\n' || c == ' ' || c == '\t')
    {
      if(curr.length() > 0)   
      {
        _output.push_back(curr);
        curr = "";
      }
    }
    else
    {
      curr += c;
    }
  }

  if(curr.length() > 0)
  {
    _output.push_back(curr);
  }
}

void split_string_eol(const sys::vector<unsigned char>& _input,
  sys::vector<sys::string>& _output)
{
  _output.clear();
  sys::string curr;

  for(size_t ci = 0; ci < _input.size(); ++ci)
  {
    char c = _input[ci];

    if(c == '\r' || c == '\n')
    {
      _output.push_back(curr);
      curr = "";
    }
    else
    {
      curr += c;
    }
  }

  if(curr.length() > 0)
  {
    _output.push_back(curr);
  }
}

void split_string_eol(const sys::string& _input,
  sys::vector<sys::string>& _output)
{
  _output.clear();
  sys::string curr;

  for(size_t ci = 0; ci < _input.length(); ++ci)
  {
    char c = _input[ci];

    if(c == '\r' || c == '\n')
    {
      _output.push_back(curr);
      curr = "";
    }
    else
    {
      curr += c;
    }
  }

  if(curr.length() > 0)
  {
    _output.push_back(curr);
  }
}

void split_string(const sys::string& _input, char splitter,
  sys::vector<sys::string>& _output)
{
  _output.clear();
  sys::string curr;

  for(size_t ci = 0; ci < _input.length(); ++ci)
  {
    char c = _input[ci];

    if(c == splitter)
    {
      _output.push_back(curr);
      curr = "";
    }
    else
    {
      curr += c;
    }
  }

  if(curr.length() > 0)
  {
    _output.push_back(curr);
  }
}

}

