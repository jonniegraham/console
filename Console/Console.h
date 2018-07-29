#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <sstream>
#include <string>
#include <iostream>
#include <err.h>

#ifndef CONSOLE_CONSOLE_H
#define CONSOLE_CONSOLE_H

using namespace std;

class Console {
private:
    unsigned int width = 800;
    unsigned int height = 300;
    unsigned int lineNum = 0;
    unsigned int lineHeight = 24;
    unsigned int fontSize = 12;

    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    TTF_Font *font = nullptr;

    SDL_Color textColor = {255, 255, 255, 0};
    SDL_Color backgroundColor = {0, 0, 0, 0};

    std::vector<SDL_Texture *> textures;
    std::vector<SDL_Rect> rectangles;

    Console() {}

    const unsigned int lineY() const {
        return this->lineNum * this->lineHeight;
    }

public:

    enum FONT_STYLE {
        NORMAL = 0, BOLD, ITALIC, UNDERLINE, STRIKETHROUGH
    };

    Console(const unsigned int width, const unsigned int height) {
        this->width = width;
        this->height = height;

        this->lineNum = 0;

        SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
        SDL_CreateWindowAndRenderer(this->width, this->height, 0, &window, &renderer);

        if (TTF_Init() != 0) {
            stringstream ss;
            ss << "Error: Initializing TTF failed\n";
            HandleError(ss.str().c_str());
        }
    }

    ~Console() {
        TTF_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    inline void SetLineHeight(const unsigned int lineHeight) {
        this->lineHeight = lineHeight;
    }

    inline void SetFontStyle(const int weight) const {
        TTF_SetFontStyle(font, weight);
    }

    void SetFont(const char *const fontPath, const unsigned int fontSize) {

        this->fontSize = fontSize;
        this->font = TTF_OpenFont(fontPath, this->fontSize);

        if (font == nullptr) {
            stringstream ss;
            ss << "Error: Font not found at " << fontPath << "\n";
            HandleError(ss.str().c_str());
        }
    }

    inline void SetColor(const SDL_Color textColor) {
        this->textColor = textColor;
    }

    inline void HandleError(const char* const c_str) const {
        cerr << c_str;
        this->~Console();
        exit(EXIT_FAILURE);
    }

    inline void SetBackground(const SDL_Color backgroundColor) {
        this->backgroundColor = backgroundColor;
    }

    void WriteLine(const char* const text = "", const bool renderNow = true) {
        // start coordinates of current line.

        // new line ?
        if (strcmp(text, "\n") == 0 || strlen(text) == 0) {
            this->lineNum++;
            return;
        }

        // start coordinates of current line.
        unsigned int y = lineY(), x = 0;

        lineNum++;

        int text_width, text_height;
        SDL_Surface *surface;

        surface = TTF_RenderText_Solid(font, text, textColor);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

        textures.emplace_back(texture);

        text_width = surface->w;
        text_height = surface->h;

        SDL_FreeSurface(surface);

        SDL_Rect rect;
        rect.x = x;
        rect.y = y;
        rect.w = text_width;
        rect.h = text_height;

        rectangles.emplace_back(rect);

        if (renderNow) {
            SDL_SetRenderDrawColor(renderer, this->backgroundColor.r, this->backgroundColor.g, this->backgroundColor.b,
                                   this->backgroundColor.a);
            SDL_RenderClear(renderer);

            for (unsigned int i = 0; i < textures.size(); i++) {
                SDL_RenderCopy(renderer, textures[i], NULL, &rectangles[i]);
            }

            if (renderNow)
                SDL_RenderPresent(renderer);
        }
    }
};

#endif //CONSOLE_CONSOLE_H
