#pragma once
#include <sstream>
#include <stdlib.h>
#include <string>

class StringTools {
  public:
    static int toInt(const std::string &string) { return atoi(string.c_str()); }

    static std::string toString(int i) {
        std::ostringstream stream;
        stream << i;
        return stream.str();
    }
};
