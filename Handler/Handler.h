using namespace std;

#ifndef CONSOLE_EVENT_H
#define CONSOLE_EVENT_H

template<typename T>
class Handler {


public:
    typedef void HandlerFunc(T* const);
    Handler(){}
    Handler(HandlerFunc* handlerFunc) {
        this->handlerFunc = handlerFunc;
    }

    inline void Invoke(T* const t) {
        handlerFunc(t);
    }

    inline void SetEnabled(const bool isEnabled = true) {
        this->isEnabled = isEnabled;
    }

    inline const bool IsEnabled() const {
        return isEnabled;
    }
private:
    HandlerFunc* handlerFunc;
    bool isEnabled = true;
};

#endif //CONSOLE_EVENT_H
