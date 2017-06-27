#ifndef XDBG_UTIL_H
#define XDBG_UTIL_H

#include <vector>
#include <sstream>

std::vector<std::string>
split(const std::string &s, char delimiter);

bool
is_prefix(const std::string &s, const std:: string &of);

#endif
