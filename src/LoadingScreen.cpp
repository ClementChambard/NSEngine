#include "LoadingScreen.h"

#include <SDL2/SDL_image.h>

#include "./NSEngine.hpp"

namespace ns {

std::future<void> LoadingScreen::drawing_task;
std::atomic_bool LoadingScreen::loading;
std::atomic_bool LoadingScreen::done;

void LoadingScreen::Load(std::function<void ()> load, std::function<void (std::atomic_bool&, std::atomic_bool&)> draw)
{
    loading = true;
    done = false;
    drawing_task = std::async(std::launch::async, draw, std::ref(loading), std::ref(done));
    load();
    loading = false;
    while (!done) {}
}

void blackLoadScreen(std::atomic_bool& run, std::atomic_bool& done)
{
    auto w = getInstance()->window().getSdlWindow();
    SDL_Renderer* sdlrenderer = SDL_CreateRenderer(w, -1, 0);
    SDL_SetRenderDrawColor(sdlrenderer, 0, 0, 0, 255);
    SDL_RenderClear(sdlrenderer);
    SDL_RenderPresent(sdlrenderer);
    while (run);
    SDL_RenderClear(sdlrenderer);
    SDL_RenderPresent(sdlrenderer);
    SDL_Delay(100);
    SDL_DestroyRenderer(sdlrenderer);
    done = true;
}
void nsLoadScreen(std::atomic_bool& run, std::atomic_bool& done)
{
    IMG_Init(IMG_INIT_PNG);
    auto w = getInstance()->window().getSdlWindow();
    SDL_Renderer* sdlrenderer = SDL_CreateRenderer(w, -1, 0);
    SDL_Surface* surf = IMG_Load("assets/engine/textures/TeamNano.png");
    SDL_Texture* tex = SDL_CreateTextureFromSurface(sdlrenderer, surf);
    SDL_Rect dest;
    dest.w = surf->w;
    dest.h = surf->h;
    SDL_FreeSurface(surf);
    dest.x = 1920/2-dest.w/2;
    dest.y = 1080/2-dest.h/2;
    i32 alpha = 0;
    SDL_SetRenderDrawColor(sdlrenderer, 0, 0, 0, 255);
    SDL_RenderClear(sdlrenderer);
    SDL_RenderPresent(sdlrenderer);
    SDL_Delay(400);
    while (alpha < 254)
    {
        SDL_RenderClear(sdlrenderer);
        SDL_SetTextureAlphaMod(tex, alpha);
        SDL_RenderCopy(sdlrenderer, tex, NULL, &dest);
        SDL_RenderPresent(sdlrenderer);
        SDL_Delay(16);
        alpha+=8; if (alpha > 255) alpha = 255;
    }
    SDL_Delay(1000);
    while (run);
    while (alpha > 0)
    {
        SDL_RenderClear(sdlrenderer);
        SDL_SetTextureAlphaMod(tex, alpha);
        SDL_RenderCopy(sdlrenderer, tex, NULL, &dest);
        SDL_RenderPresent(sdlrenderer);
        SDL_Delay(16);
        alpha-=8; if (alpha < 0) alpha = 0;
    }
    SDL_RenderClear(sdlrenderer);
    SDL_RenderPresent(sdlrenderer);
    SDL_Delay(400);
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(sdlrenderer);
    IMG_Quit();
    done = true;
}

}
