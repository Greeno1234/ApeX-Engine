#include <sys/string.h>
#include <sys/vector.h>

namespace rend
{

void split_string_whitespace(const sys::string& _input,
  sys::vector<sys::string>& _output);

void split_string_eol(const sys::string& _input,
  sys::vector<sys::string>& _output);

void split_string_eol(const sys::vector<unsigned char>& _input,
  sys::vector<sys::string>& _output);

void split_string(const sys::string& _input, char splitter,
  sys::vector<sys::string>& _output);

}
