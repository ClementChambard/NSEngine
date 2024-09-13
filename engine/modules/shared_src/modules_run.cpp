#ifdef NSMOD_USE_AUDIO_PROTO
namespace ns::audio {
    void update();
}
#endif

namespace ns {

void NSMOD_updateModules() {
#ifdef NSMOD_USE_AUDIO_PROTO
    ns::audio::update();
#endif
}

} // namespace ns
