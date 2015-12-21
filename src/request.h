#include <string>
#include <list>
#include <iostream>

using namespace std;

list<string> tokenize(  string const& str,  char const token[]);

/**
   @return : The finished html
   @param : GET request
*/
string processGetRequest(string strGet);
