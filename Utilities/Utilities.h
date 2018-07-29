#include <vector>

using namespace std;
#ifndef CONSOLE_UTILITIES_H
#define CONSOLE_UTILITIES_H
class Utilities
{
public:
    template<typename H>
    static void DeleteVector(vector<H> v) {
        for (auto it = v.begin(); it != v.end(); ++it) {
            delete (*it);
        }
        v.clear();
    }
};
#endif //CONSOLE_UTILITIES_H
