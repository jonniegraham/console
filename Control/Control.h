#include <SDL2/SDL.h>

#ifndef CONSOLE_CONTROL_H
#define CONSOLE_CONTROL_H

class Control {
private:
protected:
public:
    virtual void Render(SDL_Renderer* const renderer) = 0;
    virtual void Update(const SDL_Event* const evt) = 0;
};

#endif //CONSOLE_CONTROL_H
