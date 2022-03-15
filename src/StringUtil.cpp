#include "StringUtil.h"

bool stringBeginWith(std::string s, std::string pattern)
{
    if (pattern.size() > s.size()) return false;
    for (int i = 0; i < pattern.size(); i++)
        if (pattern[i] != s[i]) return false;
    return true;
}

std::vector<std::string> stringSplit(std::string s, const char sep)
{
    int pos1 = 0;
    int pos2 = 0;
    std::vector<std::string> strs;
    while (pos1 < s.size())
    {
        for (pos2 = pos1; s[pos2] != sep && pos2 < s.size(); pos2++);
        strs.push_back(s.substr(pos1, pos2-pos1));
        pos1 = pos2+1;
    }
    return strs;
}

#include "Error.h"

bool compareVector(std::vector<std::string> v1, std::vector<std::string> v2)
{
    if (v1.size() != v2.size()) return false;
    for (int i = 0; i < v1.size(); i++) if (v1[i] != v2[i]) return false;
    return true;
}

void STRINGUTILTESTS()
{
    std::string s1 = "v 12 223 2111";
    std::string s2 = "test string";
    std::string s3 = "str3";
    if (stringBeginWith(s1,"t")) NSEngine::fatalError("StringUtil test1 fail");
    if (!stringBeginWith(s1,"v ")) NSEngine::fatalError("StringUtil test2 fail");
    if (!stringBeginWith(s2,"t")) NSEngine::fatalError("StringUtil test3 fail");
    if (!stringBeginWith(s2,"test s")) NSEngine::fatalError("StringUtil test4 fail");
    if (stringBeginWith(s3,"tds")) NSEngine::fatalError("StringUtil test5 fail");
    if (stringBeginWith(s3,"tafasfa")) NSEngine::fatalError("StringUtil test6 fail");
    std::vector<std::string> ans1 = {"v","12","223","2111"};
    std::vector<std::string> ans2 = {"test","string"};
    std::vector<std::string> ans3 = {"str3"};
    if (!compareVector(ans1,stringSplit(s1))) NSEngine::fatalError("StringUtil test7 fail");
    if (!compareVector(ans2,stringSplit(s2))) NSEngine::fatalError("StringUtil test8 fail");
    if (!compareVector(ans3,stringSplit(s3))) NSEngine::fatalError("StringUtil test9 fail");
}
