#pragma once

#include <vector>
#include <string>
#include <sstream>

// Code from https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/
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