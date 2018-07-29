#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <utility>
#include <sstream>
#include <iostream>
#include <mutex>
#include "../Handler/Handler.h"
#include "../Handler/MouseLeftButtonDownHandler.h"
#include "../Handler/MouseLeftButtonUpHandler.h"
#include "../Handler/MouseOverHandler.h"
#include "../Handler/MouseOutHandler.h"
#include "Control.h"
#include "../Utilities/Utilities.h"

using namespace std;

#ifndef CONSOLE_BUTTON_H
#define CONSOLE_BUTTON_H

class Button : public Control {
private:

    typedef struct {
        SDL_Rect rectangle;

        struct {
            Uint8 r = 255, g = 255, b = 255, a = 255;
        } Color;

    } ButtonAttributes;

    typedef struct {
        SDL_Rect rectangle;
        string text;
        int fontSize = 12;
        string fontPath = "/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf";
        struct {
            Uint8 r = 0, g = 0, b = 0, a = 0;
        } Color;

    } TextAttributes;

    ButtonAttributes button_attr;
    TextAttributes text_attr;

    mutex buttonDown_mutex;
    mutex buttonUp_mutex;
    mutex mouseOver_mutex;
    mutex mouseOut_mutex;

    mutex attr_mutex;

    SDL_Texture* textTexture = nullptr;
    SDL_Surface* surface = nullptr;
    TTF_Font* font = nullptr;

    vector<MouseLeftButtonDownHandler<Button>*> buttonDown_Handlers;
    vector<MouseLeftButtonUpHandler<Button>*> buttonUp_Handlers;
    vector<MouseOverHandler<Button>*> mouseOver_Handlers;
    vector<MouseOutHandler<Button>*> mouseOut_Handlers;

    inline void initializeFont() {
        if (font == nullptr) {
            font = TTF_OpenFont(text_attr.fontPath.c_str(), text_attr.fontSize);
            TTF_SetFontStyle(font, TTF_STYLE_BOLD);
        }
    }

    inline void initializeTTF() const {
        if (TTF_Init() != 0) {
            stringstream ss;
            ss << "Error: Initializing TTF failed\n";
            handleError(ss.str().c_str());
        }
    }

    inline void freeTextResources() {
        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(surface);
    }

    void setText(SDL_Renderer* const renderer) {

        freeTextResources();
        initializeTTF();
        initializeFont();

        {   // lock scope
            lock_guard<mutex> lockGuard(attr_mutex);
            text_attr.rectangle.x = static_cast<int>(button_attr.rectangle.x + (button_attr.rectangle.w / 2) -
                                                     ((text_attr.text.length() / 2) *
                                                      (text_attr.fontSize - (0.35 * text_attr.fontSize))));
            text_attr.rectangle.y = static_cast<int>(button_attr.rectangle.y + (button_attr.rectangle.h / 2) -
                                                     ((text_attr.fontSize - (0.1 * text_attr.fontSize)) / 2));
        }

        surface = TTF_RenderText_Solid(font, text_attr.text.c_str(),
                                       {text_attr.Color.r, text_attr.Color.g, text_attr.Color.b, text_attr.Color.a});

        text_attr.rectangle.w = surface->w;
        text_attr.rectangle.h = surface->h;

        textTexture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_RenderCopy(renderer, textTexture, NULL, &text_attr.rectangle);
    }

    inline void handleError(const char* const c_str) const {
        cerr << c_str;
        exit(EXIT_FAILURE);
    }

    void resetButtonDownHandlers() {
        lock_guard<mutex> lockGuard(buttonDown_mutex);

        auto i = buttonDown_Handlers.begin();
        auto end = buttonDown_Handlers.end();

        for (; i != end; i++) {
            (*i)->SetEnabled();
        }
    }

    void resetButtonUpHandlers() {
        lock_guard<mutex> lockGuard(buttonUp_mutex);

        auto i = buttonUp_Handlers.begin();
        auto end = buttonUp_Handlers.end();

        for (; i != end; i++) {
            (*i)->SetEnabled();
        }
    }

    void resetMouseOverHandlers() {
        lock_guard<mutex> lockGuard(mouseOver_mutex);

        auto i = mouseOver_Handlers.begin();
        auto end = mouseOver_Handlers.end();

        for (; i != end; i++) {
            (*i)->SetEnabled();
        }
    }

    void resetMouseOutHandlers() {
        lock_guard<mutex> lockGuard(mouseOut_mutex);

        auto i = mouseOut_Handlers.begin();
        auto end = mouseOut_Handlers.end();

        for (; i != end; i++) {
            (*i)->SetEnabled();
        }
    }

    void invokeHandler(Handler<Button>* handler) {
        if (handler->IsEnabled()) {
            handler->Invoke(this);
            handler->SetEnabled(false);
        }
    }

    void onButtonDown() {
        resetButtonUpHandlers();
        lock_guard<mutex> lockGuard(this->buttonDown_mutex);

        auto currHandler = this->buttonDown_Handlers.begin();
        auto end = this->buttonDown_Handlers.end();

        for (; currHandler != end; currHandler++) {
            invokeHandler(*currHandler);
        }
    }

    void onButtonUp() {
        resetButtonDownHandlers();
        lock_guard<mutex> lockGuard(this->buttonUp_mutex);

        auto currHandler = this->buttonUp_Handlers.begin();
        auto end = this->buttonUp_Handlers.end();

        for (; currHandler != end; currHandler++) {
            invokeHandler(*currHandler);
        }
    }

    void onMouseOver() {
        resetMouseOutHandlers();
        lock_guard<mutex> lockGuard(this->mouseOver_mutex);

        auto currHandler = this->mouseOver_Handlers.begin();
        auto end = this->mouseOver_Handlers.end();

        for (; currHandler != end; currHandler++) {
            invokeHandler(*currHandler);
        }
    }

    void onMouseOut() {
        resetMouseOverHandlers();
        lock_guard<mutex> lockGuard(this->mouseOut_mutex);

        auto currHandler = this->mouseOut_Handlers.begin();
        auto end = this->mouseOut_Handlers.end();

        for (; currHandler != end; currHandler++) {
            invokeHandler(*currHandler);
        }
    }

public:
    Button() {
        text_attr.text = "Hell\0";
    }

    ~Button() {
        freeTextResources();
        TTF_Quit();
        Utilities::DeleteVector<MouseLeftButtonDownHandler<Button>*>(buttonDown_Handlers);
        Utilities::DeleteVector<MouseLeftButtonUpHandler<Button>*>(buttonUp_Handlers);
        Utilities::DeleteVector<MouseOverHandler<Button>*>(mouseOver_Handlers);
        Utilities::DeleteVector<MouseOutHandler<Button>*>(mouseOut_Handlers);
    }


    inline void
    SetRectangle(const unsigned int x, const unsigned int y, const unsigned int height, const unsigned int width) {
        lock_guard<mutex> lockGuard(attr_mutex);
        button_attr.rectangle.x = x;
        button_attr.rectangle.y = y;
        button_attr.rectangle.w = width;
        button_attr.rectangle.h = height;
    }

    inline void SetBackgroundColor(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a) {
        lock_guard<mutex> lockGuard(attr_mutex);
        button_attr.Color.r = r;
        button_attr.Color.g = g;
        button_attr.Color.b = b;
        button_attr.Color.a = a;
    }

    inline void SetFontSize(const unsigned int fontSize) {
        this->text_attr.fontSize = fontSize;
    }

    inline void SetFontPath(const string &fontPath) {
        this->text_attr.fontPath = fontPath;
    }

    inline void SetFontRGBAColor(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a) {
        this->text_attr.Color.r = r;
        this->text_attr.Color.g = g;
        this->text_attr.Color.b = b;
        this->text_attr.Color.a = a;
    }

    void Render(SDL_Renderer* const renderer) {
        {
            // lock scope
            lock_guard<mutex> lockGuard(attr_mutex);
            SDL_SetRenderDrawColor(renderer, button_attr.Color.r, button_attr.Color.g, button_attr.Color.b,
                                   button_attr.Color.a);
            SDL_RenderFillRect(renderer, &button_attr.rectangle);
        }
        setText(renderer);
    }

    void Update(const SDL_Event* const evt) {

        bool IsMouseOver;
        {   // lock scope
            lock_guard<mutex> lockGuard(attr_mutex);
            IsMouseOver = evt->button.x >= button_attr.rectangle.x &&
                          evt->button.x <= (button_attr.rectangle.x + button_attr.rectangle.w) &&
                          evt->button.y >= button_attr.rectangle.y &&
                          evt->button.y <= (button_attr.rectangle.y + button_attr.rectangle.h);
        }

        if (evt->type == SDL_MOUSEBUTTONDOWN) {
            if (evt->button.button == SDL_BUTTON_LEFT && IsMouseOver) {
                onButtonDown();
            }
        } else {
            if (evt->type == SDL_MOUSEBUTTONUP) {
                onButtonUp();
                if (IsMouseOver) {
                    resetMouseOverHandlers();
                    onMouseOver();
                }
            }
            if (evt->type == SDL_MOUSEMOTION) {
                if (IsMouseOver) {
                    onMouseOver();
                } else {
                    onMouseOut();
                }
            }
        }
    }

    inline void AddHandler(MouseLeftButtonDownHandler<Button>* handler) {
        lock_guard<mutex> lockGuard(this->buttonDown_mutex);
        this->buttonDown_Handlers.push_back(handler);
    }

    inline void AddHandler(MouseLeftButtonUpHandler<Button>* handler) {
        lock_guard<mutex> lockGuard(this->buttonUp_mutex);
        this->buttonUp_Handlers.push_back(handler);
    }

    inline void AddHandler(MouseOverHandler<Button>* handler) {
        lock_guard<mutex> lockGuard(this->mouseOver_mutex);
        this->mouseOver_Handlers.push_back(handler);
    }

    inline void AddHandler(MouseOutHandler<Button>* handler) {
        lock_guard<mutex> lockGuard(this->mouseOut_mutex);
        this->mouseOut_Handlers.push_back(handler);
    }
};

#endif //CONSOLE_BUTTON_H
