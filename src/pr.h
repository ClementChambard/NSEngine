#ifndef PR_H_
#define PR_H_

#include <iostream>

namespace NSEngine {

    extern std::string prBeg;
    extern std::string prEnd;
    extern std::string prSep;

    template<typename T>
    inline void setPrBeg(T const& beg) { prBeg = std::to_string(beg); }
    inline void setPrBeg(const char* beg) { prBeg = beg; }
    template<typename T>
    inline void setPrEnd(T const& end) { prEnd = std::to_string(end); }
    inline void setPrEnd(const char* end) { prEnd = end; }
    template<typename T>
    inline void setPrSep(T const& sep) { prSep = std::to_string(sep); }
    inline void setPrSep(const char* sep) { prSep = sep; }

    inline void _prStep() { std::cout << prEnd; }

    template<typename T, typename... Args>
    inline void _prStep(T t, Args... args)
    {
        std::cout << t;
        if (sizeof...(args) > 0) std::cout << prSep;
        _prStep(args...);
    }

    template<typename T, typename... Args>
    inline void pr(T t, Args... args)
    {
        std::cout << prBeg;
        _prStep(t, args...);
    }

}

#endif // PR_H_
