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
#include "../Font/Font.h"

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
        Font* font;
        string text = "";

    } TextAttributes;

    ButtonAttributes buttonAttributes;
    TextAttributes textAttributes;

    mutex buttonDown_mutex;
    mutex buttonUp_mutex;
    mutex mouseOver_mutex;
    mutex mouseOut_mutex;
    mutex buttonAttributes_mutex;

    SDL_Texture* textTexture = nullptr;
    SDL_Surface* surface = nullptr;


    vector<MouseLeftButtonDownHandler<Button>*> buttonDown_handlers;
    vector<MouseLeftButtonUpHandler<Button>*> buttonUp_handlers;
    vector<MouseOverHandler<Button>*> mouseOver_handlers;
    vector<MouseOutHandler<Button>*> mouseOut_handlers;

    inline void initializeFont() {
        if (textAttributes.font == nullptr) {
            Utilities::HandleError("Foreground Font not set.");
        }
        TTF_SetFontStyle(textAttributes.font->GetFont(), TTF_STYLE_BOLD);
    }

    inline void initializeTTF() const {
        if (TTF_Init() != 0) {
            stringstream ss;
            ss << "Error: Initializing TTF failed\n";
            Utilities::HandleError(ss.str().c_str());
        }
    }

    inline void freeTextResources() {
        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(surface);
    }

    void RenderText(SDL_Renderer* const renderer) {

        freeTextResources();
        initializeTTF();
        initializeFont();
        int x, y;
        {
            lock_guard<mutex> lockGuard(buttonAttributes_mutex);
            x = static_cast<int>(buttonAttributes.rectangle.x +
                                     (buttonAttributes.rectangle.w / 2) -
                                     ((textAttributes.text.length() / 2) *
                                      (textAttributes.font->getSize() -
                                       (0.35 * textAttributes.font->getSize()))));
            y = static_cast<int>(buttonAttributes.rectangle.y +
                                     (buttonAttributes.rectangle.h / 2) -
                                     ((textAttributes.font->getSize() - (0.1 * textAttributes.font->getSize())) /
                                      2));
        }

        surface = TTF_RenderText_Solid(textAttributes.font->GetFont(), textAttributes.text.c_str(),
                                       textAttributes.font->GetColor());

        textAttributes.font->SetRectangle(x, y, surface->w, surface->h);

        textTexture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_RenderCopy(renderer, textTexture, NULL, &textAttributes.font->GetRectangle());
    }

    void resetButtonDownHandlers() {
        lock_guard<mutex> lockGuard(buttonDown_mutex);

        auto i = buttonDown_handlers.begin();
        auto end = buttonDown_handlers.end();

        for (; i != end; i++) {
            (*i)->SetEnabled();
        }
    }

    void resetButtonUpHandlers() {
        lock_guard<mutex> lockGuard(buttonUp_mutex);

        auto i = buttonUp_handlers.begin();
        auto end = buttonUp_handlers.end();

        for (; i != end; i++) {
            (*i)->SetEnabled();
        }
    }

    void resetMouseOverHandlers() {
        lock_guard<mutex> lockGuard(mouseOver_mutex);

        auto i = mouseOver_handlers.begin();
        auto end = mouseOver_handlers.end();

        for (; i != end; i++) {
            (*i)->SetEnabled();
        }
    }

    void resetMouseOutHandlers() {
        lock_guard<mutex> lockGuard(mouseOut_mutex);

        auto i = mouseOut_handlers.begin();
        auto end = mouseOut_handlers.end();

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
        lock_guard<mutex> lockGuard(buttonDown_mutex);

        auto currHandler = buttonDown_handlers.begin();
        auto end = buttonDown_handlers.end();

        for (; currHandler != end; currHandler++) {
            invokeHandler(*currHandler);
        }
    }

    void onButtonUp() {
        resetButtonDownHandlers();
        lock_guard<mutex> lockGuard(buttonUp_mutex);

        auto currHandler = this->buttonUp_handlers.begin();
        auto end = this->buttonUp_handlers.end();

        for (; currHandler != end; currHandler++) {
            invokeHandler(*currHandler);
        }
    }

    void onMouseOver() {
        resetMouseOutHandlers();
        lock_guard<mutex> lockGuard(mouseOver_mutex);

        auto currHandler = mouseOver_handlers.begin();
        auto end = mouseOver_handlers.end();

        for (; currHandler != end; currHandler++) {
            invokeHandler(*currHandler);
        }
    }

    void onMouseOut() {
        resetMouseOverHandlers();
        lock_guard<mutex> lockGuard(mouseOut_mutex);

        auto currHandler = mouseOut_handlers.begin();
        auto end = mouseOut_handlers.end();

        for (; currHandler != end; currHandler++) {
            invokeHandler(*currHandler);
        }
    }

public:
    Button() {
        textAttributes.text = "Hell\0";
    }

    ~Button() {
        freeTextResources();
        TTF_Quit();
        Utilities::DeleteVector<MouseLeftButtonDownHandler<Button>*>(buttonDown_handlers);
        Utilities::DeleteVector<MouseLeftButtonUpHandler<Button>*>(buttonUp_handlers);
        Utilities::DeleteVector<MouseOverHandler<Button>*>(mouseOver_handlers);
        Utilities::DeleteVector<MouseOutHandler<Button>*>(mouseOut_handlers);
    }


    inline void
    SetRectangle(const unsigned int x, const unsigned int y, const unsigned int height, const unsigned int width) {
        lock_guard<mutex> lockGuard(buttonAttributes_mutex);
        buttonAttributes.rectangle.x = x;
        buttonAttributes.rectangle.y = y;
        buttonAttributes.rectangle.w = width;
        buttonAttributes.rectangle.h = height;
    }

    inline void SetBackgroundColor(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a) {
        lock_guard<mutex> lockGuard(buttonAttributes_mutex);
        buttonAttributes.Color.r = r;
        buttonAttributes.Color.g = g;
        buttonAttributes.Color.b = b;
        buttonAttributes.Color.a = a;
    }

    inline void SetText(const string &text) {
        textAttributes.text = text;
    }

    inline void SetFont(Font *font) {
        textAttributes.font = font;
    }

    void Render(SDL_Renderer* const renderer) {
        {
            // lock scope
            lock_guard<mutex> lockGuard(buttonAttributes_mutex);
            SDL_SetRenderDrawColor(renderer, buttonAttributes.Color.r, buttonAttributes.Color.g,
                                   buttonAttributes.Color.b,
                                   buttonAttributes.Color.a);
            SDL_RenderFillRect(renderer, &buttonAttributes.rectangle);
        }
        RenderText(renderer);
    }

    void Update(const SDL_Event* const evt) {

        bool IsMouseOver;
        {   // lock scope
            lock_guard<mutex> lockGuard(buttonAttributes_mutex);
            IsMouseOver = evt->button.x >= buttonAttributes.rectangle.x &&
                          evt->button.x <= (buttonAttributes.rectangle.x + buttonAttributes.rectangle.w) &&
                          evt->button.y >= buttonAttributes.rectangle.y &&
                          evt->button.y <= (buttonAttributes.rectangle.y + buttonAttributes.rectangle.h);
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
        lock_guard<mutex> lockGuard(buttonDown_mutex);
        buttonDown_handlers.push_back(handler);
    }

    inline void AddHandler(MouseLeftButtonUpHandler<Button>* handler) {
        lock_guard<mutex> lockGuard(buttonUp_mutex);
        buttonUp_handlers.push_back(handler);
    }

    inline void AddHandler(MouseOverHandler<Button>* handler) {
        lock_guard<mutex> lockGuard(mouseOver_mutex);
        mouseOver_handlers.push_back(handler);
    }

    inline void AddHandler(MouseOutHandler<Button>* handler) {
        lock_guard<mutex> lockGuard(mouseOut_mutex);
        mouseOut_handlers.push_back(handler);
    }
};

#endif //CONSOLE_BUTTON_H
