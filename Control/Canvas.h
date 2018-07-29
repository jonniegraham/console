#include <iostream>
#include <vector>
#include <thread>
#include "Control.h"

using namespace std;

#ifndef CONSOLE_CANVAS_H
#define CONSOLE_CANVAS_H

class Canvas {
private:
    vector<Control*> controls;
    SDL_Renderer* renderer = nullptr;
    SDL_Window* window = nullptr;

    void InitializeWindow() {
        window = SDL_CreateWindow("", 350, 150, 800, 500, SDL_WINDOW_SHOWN);
        if (window == nullptr) {
            fprintf(stderr, "Creating window failed: %s\n", SDL_GetError());
        }
    }

    void InitializeRenderer() {
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            fprintf(stderr, "Creating renderer failed: %s\n", SDL_GetError());
        }
    }

    bool IsQuitEvent(SDL_Event* evt) {
        return evt->type == SDL_QUIT ||
               (evt->type == SDL_WINDOWEVENT && evt->window.event == SDL_WINDOWEVENT_CLOSE) ||
               (evt->type == SDL_KEYDOWN && evt->key.keysym.sym == SDLK_ESCAPE);
    }

    void Render(const SDL_Event* evt) const {

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        auto i = controls.begin();
        auto end = controls.end();

        for (; i != end; i++) {
            (*i)->Update(evt);
            (*i)->Render(renderer);
        }

        SDL_RenderPresent(renderer);
    }

public:
    Canvas() {
        SDL_Init(SDL_INIT_VIDEO);
        InitializeWindow();
        InitializeRenderer();
    }

    ~Canvas() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        Utilities::DeleteVector<Control*>(controls);
    }

    void Add(Control* control) {
        controls.emplace_back(control);
    }

    void Show() {

        thread t_outer([&]() {

            while (true) {
                SDL_Event evt;
                SDL_PollEvent(&evt);

                if (IsQuitEvent(&evt))
                    break;

                Render(&evt);
            }
        });
        t_outer.join();

        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
    }

};


#endif //CONSOLE_CANVAS_H
