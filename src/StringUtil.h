#ifndef STRINGUTIL_INCLUDED_H
#define STRINGUTIL_INCLUDED_H

#include <string>
#include <vector>

extern bool stringBeginWith(std::string s, std::string pattern);
extern std::vector<std::string> stringSplit(std::string s, const char sep = ' ');

extern void STRINGUTILTESTS();
#endif
