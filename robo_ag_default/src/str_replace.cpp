#include "str_replace.h"

#include <string>
#include <iostream>

std::string str_replace(const std::string& search, const std::string& replace, const std::string& subject)
{
    std::string str = subject;
    size_t pos = 0;
    while((pos = str.find(search, pos)) != std::string::npos)
    {
        str.replace(pos, search.length(), replace);
        pos += replace.length();
    }
    return str;
} 
