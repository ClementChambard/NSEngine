#ifdef NSMOD_USE_IMGUI
#include <imgui_ns.hpp>
#endif

#include <logger.h>
#include <NSEngine.hpp>

namespace ns {

static u32 g_nbModules = 0;

void NSMOD_initModules() {
    NS_INFO("Initializing optionnal submodules:");
#ifdef NSMOD_USE_IMGUI
    NS_INFO("- imgui_ns");
    new ImGuiContext(*getMainWindow());
    g_nbModules++;
#endif
    NS_INFO("Initialized %u submodules.", g_nbModules);
}

} // namespace ns
