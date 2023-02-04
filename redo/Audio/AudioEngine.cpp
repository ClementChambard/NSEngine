#include "AudioEngine.h"
#include "Error.h"
namespace NSEngine 
{

    std::vector<SoundSource*> AudioEngine::sources;
    std::vector<SoundSource*> AudioEngine::tempSources;
    std::vector<MusicBuffer*> AudioEngine::musics;
    bool AudioEngine::initialized = false;

    void AudioEngine::Init()
    {
        if (initialized) return;
        SoundDevice::Init();
        SoundBuffer::Init();
        initialized = true;
    }

    void AudioEngine::Clean()
    {
        if (!initialized) return;
        initialized = false;
        for (auto s : sources) delete s;
        for (auto m : musics) delete m;
        SoundBuffer::Delete();
        SoundDevice::Delete();
    }

    void AudioEngine::Update()
    {
        if (!initialized) return;
        for (auto m : musics) { if (m->isPlaying()) m->UpdateBufferStream(); }
        for (int i = 0; i < tempSources.size(); i++)
        {
            if (tempSources[i]!=nullptr && !tempSources[i]->isPlaying())
            {
                delete tempSources[i]; tempSources[i] = nullptr;
                if (i == tempSources.size()-1)
                {
                    int j = i;
                    while(tempSources[j]==nullptr && j-->=0) tempSources.pop_back();
                }
            }
        }
    }

    void AudioEngine::PlaySound(int soundID)
    {
        int i = tempSources.size();
        tempSources.push_back(new SoundSource());
        tempSources[i]->Play(soundID);
    }

    int AudioEngine::CreateSource()
    {
        if (!initialized) return -1;
        sources.push_back(new SoundSource());
        return sources.size()-1;
    }

    void AudioEngine::PlaySoundAtSource(int source, int soundID)
    {
        if (!initialized) return;
        sources[source]->Play(soundID);
    }

    int AudioEngine::AddSFX(const char* filename)
    {
        if (!initialized) return -1; 
        return SoundBuffer::addSoundEffect(filename); 
    }

    int AudioEngine::AddMusic(const char* filename)
    {
        if (!initialized) return -1;
        musics.push_back(new MusicBuffer(filename));
        return musics.size() -1;
    }

    void AudioEngine::PlayMusic(int musicID)
    {
        if (!initialized) return;
        musics[musicID]->Play();
    }

    void AudioEngine::PauseMusic(int musicID)
    {
        if (!initialized) return;
        musics[musicID]->Pause();
    }

    void AudioEngine::StopMusic(int musicID)
    {
        if (!initialized) return;
        musics[musicID]->Stop();
    }

    void AudioEngine::ResumeMusic(int musicID)
    {
        if (!initialized) return;
        musics[musicID]->Resume();
    }

}
