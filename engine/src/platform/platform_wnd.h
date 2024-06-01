#ifndef PLATFORMWND_INCLUDED_H
#define PLATFORMWND_INCLUDED_H

#define NS_WND_FULLSCREEN (1 << 0)

#include "../defines.h"

namespace ns::platform {

struct Wnd;

void wnd_init(Wnd** ppWnd, cstr name, u32 width, u32 height, u32 flags);
void wnd_destroy(Wnd *pWnd);
void wnd_set_size(Wnd *pWnd, u32 width, u32 height, bool fullscreen);
void wnd_swap_buffers(Wnd *pWnd);
void wnd_set_mouse_pos(Wnd *pWnd, u32 x, u32 y);

};

#endif // PLATFORMWND_INCLUDED_H
