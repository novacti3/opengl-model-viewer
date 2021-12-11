#pragma once

#include <vector>
#include <string>
#include <sstream>

// Code from https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/
// Splits the string into parts according to the delim character
inline std::vector<std::string> SplitString(const std::string &string, const char &delim)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(string);
   
   while (std::getline(tokenStream, token, delim))
   {
      tokens.push_back(token);
   }

   return tokens;
}

// Code from https://www.geeksforgeeks.org/how-to-find-index-of-a-given-element-in-a-vector-in-cpp/
/* 
Returns the index of the desired element if its present in the given collection.
Returns -1 if the element is NOT present
 */
template<typename T>
inline int FindIndexOfElement(const std::vector<T> &vector, const T &value)
{
   int index = 0;

   auto it = std::find(vector.begin(), vector.end(), value);
   if(it != vector.end())
      index = it - vector.begin();
   else
      index = -1;
   
   return index;   
}
// NOTE: Having the same func for maps and arrays would be handy 