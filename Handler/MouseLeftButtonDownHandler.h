#include "Handler.h"

using namespace std;

#ifndef CONSOLE_MOUSELEFTBUTTONDOWNHANDLER_H
#define CONSOLE_MOUSELEFTBUTTONDOWNHANDLER_H
template<typename T>
class MouseLeftButtonDownHandler : public Handler<T> {
    typedef void HandlerFunc(T* const);
public:
    explicit MouseLeftButtonDownHandler(HandlerFunc* handlerFunc) : Handler<T>(handlerFunc) {

    }
};
#endif //CONSOLE_MOUSELEFTBUTTONDOWNHANDLER_H