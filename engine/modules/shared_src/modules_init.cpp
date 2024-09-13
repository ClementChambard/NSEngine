#ifdef NSMOD_USE_IMGUI
#include <imgui_ns.hpp>
#endif
#ifdef NSMOD_USE_AUDIO_PROTO
namespace ns::audio {
    void init();
    void clean();
}
#endif

#include <logger.h>
#include <asserts.h>

namespace ns {

static u32 g_nbModules = 0;

void NSMOD_initModules() {
    NS_INFO("Initializing optionnal submodules:");
#ifdef NSMOD_USE_IMGUI
    NS_INFO("- imgui_ns");
    NSMOD_INIT_imgui();
    g_nbModules++;
#endif
#ifdef NSMOD_USE_AUDIO_PROTO
    NS_INFO("- audio engine prototype");
    ns::audio::init();
    g_nbModules++;
#endif
    NS_INFO("Initialized %u submodules.", g_nbModules);
}

void NSMOD_shutdownModules() {
    NS_INFO("Shutting down optionnal submodules:");
#ifdef NSMOD_USE_AUDIO_PROTO
    NS_INFO("- audio engine prototype");
    ns::audio::clean();
    g_nbModules--;
#endif
#ifdef NSMOD_USE_IMGUI
    NS_INFO("- imgui_ns");
    NSMOD_SHUTDOWN_imgui();
    g_nbModules--;
#endif

    NS_ASSERT(g_nbModules == 0);
}

} // namespace ns
