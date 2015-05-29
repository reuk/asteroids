#include <iostream>
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

using namespace std;

const auto SCREEN_X = 640;
const auto SCREEN_Y = 480;

auto surface_destroyer  = [](auto&& surface)    {SDL_FreeSurface(surface);};
auto texture_destroyer  = [](auto&& texture)    {SDL_DestroyTexture(texture);};
auto renderer_destroyer = [](auto&& renderer)   {SDL_DestroyRenderer(renderer);};
auto window_destroyer   = [](auto&& window)     {SDL_DestroyWindow(window);};

using Surface   = unique_ptr<SDL_Surface,   decltype(surface_destroyer)>;
using Texture   = unique_ptr<SDL_Texture,   decltype(texture_destroyer)>;
using Renderer  = unique_ptr<SDL_Renderer,  decltype(renderer_destroyer)>;
using Window    = unique_ptr<SDL_Window,    decltype(window_destroyer)>;

auto load_surface(const std::string & path)
{
    Surface surface(
            IMG_Load(path.c_str()),
            surface_destroyer);

    if (surface == nullptr)
        cout << "failed to load surface: " << IMG_GetError() << endl;

    return surface;
}

auto load_surface_optimized
(   const std::string & path
,   const SDL_PixelFormat & format
)
{
    auto surface = load_surface(path);

    if (surface == nullptr)
        return surface;

    Surface optimized(
            SDL_ConvertSurface(surface.get(), &format, 0),
            surface_destroyer);

    if (optimized == nullptr)
        cout << "failed to optimize surface: " << SDL_GetError() << endl;

    return optimized;
}

auto load_texture(const std::string & path, SDL_Renderer & renderer)
{
    auto surface = load_surface(path);

    if (surface == nullptr)
        return Texture(nullptr, texture_destroyer);

    Texture texture(
            SDL_CreateTextureFromSurface(&renderer, surface.get()),
            texture_destroyer);

    if (texture == nullptr)
        cout << "failed to create texture: " << SDL_GetError() << endl;

    return texture;
}

int main(int argc, char * args[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "could not init sdl: " << SDL_GetError() << endl;
        return 1;
    }

    //  This scope makes sure that the smart pointers inside are destructed
    //  BEFORE we call the _Quit() functions.
    {
        Window window(
                SDL_CreateWindow(
                    "SDL Tutorial",
                    SDL_WINDOWPOS_UNDEFINED,
                    SDL_WINDOWPOS_UNDEFINED,
                    SCREEN_X,
                    SCREEN_Y,
                    SDL_WINDOW_SHOWN),
                window_destroyer);

        if (window == nullptr)
        {
            cout << "could not create window" << endl;
            return 1;
        }

        Renderer renderer(
                SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED),
                renderer_destroyer);

        if (renderer == nullptr)
        {
            cout << "could not create renderer: " << SDL_GetError() << endl;
            return 1;
        }

        auto img_flags = IMG_INIT_PNG;
        if (! (IMG_Init(img_flags)) & img_flags)
        {
            cout << "could not init image handling: " << IMG_GetError() << endl;
            return 1;
        }

        auto & surface = *SDL_GetWindowSurface(window.get());
        auto image = load_texture("loaded.png", *renderer);

        if (image == nullptr)
            return 1;

        auto quit = false;
        while (! quit)
        {
            SDL_Event e;
            while (SDL_PollEvent(&e) != 0)
                switch (e.type)
                {
                    case SDL_QUIT:
                        quit = true;
                        break;

                    default:
                        break;
                }


            SDL_SetRenderDrawColor(renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(renderer.get());

            SDL_Rect fill_rect = {SCREEN_X / 4, SCREEN_Y / 4, SCREEN_X / 2, SCREEN_Y / 2};
            SDL_SetRenderDrawColor(renderer.get(), 0xFF, 0x00, 0x00, 0xFF);
            SDL_RenderFillRect(renderer.get(), &fill_rect);

            SDL_Rect outline_rect = {SCREEN_X / 6, SCREEN_Y / 6, SCREEN_X * 2 / 3, SCREEN_Y * 2 / 3};
            SDL_SetRenderDrawColor(renderer.get(), 0x00, 0xFF, 0x00, 0xFF);
            SDL_RenderDrawRect(renderer.get(), &outline_rect);

            SDL_SetRenderDrawColor(renderer.get(), 0x00, 0x00, 0xFF, 0xFF);
            SDL_RenderDrawLine(renderer.get(), 0, SCREEN_Y / 2, SCREEN_X, SCREEN_Y / 2);

            SDL_SetRenderDrawColor(renderer.get(), 0xFF, 0xFF, 0x00, 0xFF);
            for (auto i = 0; i < SCREEN_Y; i += 4)
                SDL_RenderDrawPoint(renderer.get(), SCREEN_X / 2, i);

            /*
            SDL_RenderCopy(renderer.get(), image.get(), nullptr, nullptr);
            */

            SDL_RenderPresent(renderer.get());
        }
    }

    IMG_Quit();
    SDL_Quit();

    return 0;
}
