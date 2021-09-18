#pragma once

#include <vector>
#include <string>
#include <sstream>

// Code from https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/
std::vector<std::string> SplitString(const std::string &string, const char &delim)
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