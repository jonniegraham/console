#include "Handler.h"
#include "../Control/Control.h"

using namespace std;

#ifndef CONSOLE_MOUSEOUTHANDLER_H
#define CONSOLE_MOUSEOUTHANDLER_H
template<typename T>
class MouseOutHandler : public Handler<T> {
    static_assert(is_base_of<Control, T>::value, "T must inherit from Control");
    typedef void HandlerFunc(T* const);
public:
    explicit MouseOutHandler(HandlerFunc* handlerFunc) : Handler<T>(handlerFunc) {

    }
};
#endif //CONSOLE_MOUSEOUTHANDLER_H