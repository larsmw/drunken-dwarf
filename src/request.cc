#include "request.h"

using namespace std;

list<string> tokenize(  string const& str,  char const token[])
{
  list<string> results;
  string::size_type j = 0;
  while (j < str.length())
  {
    string::size_type k = str.find(token, j);
    if (k == string::npos)
      k = str.length();

    results.push_back(str.substr(j, k-j));
    j = k + 1;
  }
  return results;
}

string processGetRequest(string strGet)
{
  cout << strGet << endl;
  return "<html><title>Test</title><body><h2>jadaaa...</h2></html>";
}
