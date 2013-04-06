#include "request.h"


std::list<std::string> tokenize(  std::string const& str,  char const token[])
{
  std::list<std::string> results;
  std::string::size_type j = 0;
  while (j < str.length())
  {
    std::string::size_type k = str.find(token, j);
    if (k == std::string::npos)
      k = str.length();

    results.push_back(str.substr(j, k-j));
    j = k + 1;
  }
  return results;
}
