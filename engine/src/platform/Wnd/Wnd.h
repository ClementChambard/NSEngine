#ifndef WND_H
#define WND_H

#include "../../defines.h"

#define NS_WND_FULLSCREEN (1 << 0)

#define WND_BACKEND_X11 0
#define WND_BACKEND_SDL 1

#define WND_BACKEND WND_BACKEND_SDL


namespace ns {

struct IEventHandler;

namespace platform {

struct Wnd;
struct Event;

bool wnd_init(Wnd **wnd, cstr name, u32 width, u32 height, u32 flags);
void wnd_destroy(Wnd *wnd);
void wnd_swap(Wnd *wnd);
void wnd_set_size(Wnd *pWnd, u32 width, u32 height, bool fullscreen);
void wnd_set_mouse_pos(Wnd *pWnd, u32 x, u32 y);
void wnd_add_event_handler(Wnd *pWnd, IEventHandler* eventHandler, u32 priority);
void wnd_handle_events(Wnd *pWnd);

} // namespace platform

} // namespace ns

#endif // WND_H
