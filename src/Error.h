#ifndef ERRORSDOTH
#define ERRORSDOTH

#include "pr.h"
#include "./defines.h"

namespace ns {

enum class MessagePrintLevel {
    LV_FATAL,
    LV_ERROR,
    LV_WARN,
    LV_INFO
};
inline bool operator<(MessagePrintLevel l, MessagePrintLevel r) { return (i32) l < (i32) r; }
inline bool operator>(MessagePrintLevel l, MessagePrintLevel r) { return (i32) l > (i32) r; }
inline bool operator<=(MessagePrintLevel l, MessagePrintLevel r) { return (i32) l <= (i32) r; }
inline bool operator>=(MessagePrintLevel l, MessagePrintLevel r) { return (i32) l >= (i32) r; }

extern void setPrintLevel(MessagePrintLevel lvl);
extern MessagePrintLevel getPrintLevel();

extern void FatalErrorQuit();

template<typename T, typename... Args> inline void fatalError(T t, Args... args)
{
    if (getPrintLevel() < MessagePrintLevel::LV_FATAL) return;
    setPrBeg("[FATAL ERROR] : ");
    setPrEnd("\n");
    setPrSep(" ");
    pr(t, args...);
    FatalErrorQuit();
}

template<typename T, typename... Args> inline void error(T t, Args... args)
{
    if (getPrintLevel() < MessagePrintLevel::LV_ERROR) return;
    setPrBeg("[ERROR] : ");
    setPrEnd("\n");
    setPrSep(" ");
    pr(t, args...);
}

template<typename T, typename... Args> inline void warning(T t, Args... args)
{
    if (getPrintLevel() < MessagePrintLevel::LV_WARN) return;
    setPrBeg("[WARNING] : ");
    setPrEnd("\n");
    setPrSep(" ");
    pr(t, args...);
}

template<typename T, typename... Args> inline void info(T t, Args... args)
{
    if (getPrintLevel() < MessagePrintLevel::LV_INFO) return;
    setPrBeg("[INFO] : ");
    setPrEnd("\n");
    setPrSep(" ");
    pr(t, args...);
}

}

#endif
