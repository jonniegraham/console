#include "Handler.h"

using namespace std;

#ifndef CONSOLE_MOUSEOVERHANDLER_H
#define CONSOLE_MOUSEOVERHANDLER_H
template<typename T>
class MouseOverHandler : public Handler<T> {
    typedef void HandlerFunc(T* const);
public:
    explicit MouseOverHandler(HandlerFunc* handlerFunc) : Handler<T>(handlerFunc) {

    }
};
#endif //CONSOLE_MOUSEOVERHANDLER_H