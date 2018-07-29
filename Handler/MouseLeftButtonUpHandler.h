#include "Handler.h"

using namespace std;

#ifndef CONSOLE_MOUSELEFTBUTTONUPHANDLER_H
#define CONSOLE_MOUSELEFTBUTTONUPHANDLER_H
template<typename T>
class MouseLeftButtonUpHandler : public Handler<T> {
    typedef void HandlerFunc(T* const);
public:
    explicit MouseLeftButtonUpHandler(HandlerFunc* handlerFunc) : Handler<T>(handlerFunc) { }
};
#endif //CONSOLE_MOUSELEFTBUTTONUPHANDLER_H
