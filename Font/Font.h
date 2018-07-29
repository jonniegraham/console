#include <string>
#include <SDL2/SDL_ttf.h>

using namespace std;

#ifndef CONSOLE_FONT_H
#define CONSOLE_FONT_H

class Font {

private:
    SDL_Rect rectangle;
    TTF_Font* font = nullptr;
    SDL_Color color;
    int size = 12;

public:

    class Type {
    public:
        static const Type Lohit;
        static const Type FreeSerifItalic;
        static const Type Gargi;
        static const Type AbyssinicaSIL;
    private:
        string path;


    private:
        explicit Type(const string &path) {
            this->path = path;
        }

    public:
        string GetPath() const {
            return path;
        }
    };

    Font(const Type &type) {
        SetFont(type);
    }

    ~Font() {

    }

    inline void SetRectangle(const int x, const int y, const int width, const int height) {
        rectangle.x = x;
        rectangle.y = y;
        rectangle.w = width;
        rectangle.h = height;
    }

    inline void SetSize(const unsigned int size) {
        this->size = size;
    }

    inline void SetColor(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) {
        color.r = r;
        color.g = g;
        color.b = b;
        color.a = a;
    }

    inline void SetFont(const Type &type) {

        TTF_CloseFont(font);
        if (TTF_Init() != 0) {
            stringstream ss;
            ss << "Error: Initializing TTF failed\n";
            Utilities::HandleError(ss.str().c_str());
        }
        font = TTF_OpenFont(type.GetPath().c_str(), size);
    }

    const SDL_Rect &GetRectangle() const {
        return rectangle;
    }

    int getSize() const {
        return size;
    }

    const SDL_Color &GetColor() const {
        return color;
    }

    TTF_Font *GetFont() const {
        return font;
    }
};
const Font::Type Font::Type::Lohit = Type("/usr/share/fonts/truetype/lohit-telugu/Lohit-Telugu.ttf");
const Font::Type Font::Type::FreeSerifItalic = Type("/usr/share/fonts/truetype/freefont/FreeSerifItalic.ttf");
const Font::Type Font::Type::Gargi = Type("/usr/share/fonts/truetype/Gargi/Gargi.ttf");
const Font::Type Font::Type::AbyssinicaSIL = Type("/usr/share/fonts/truetype/abyssinica/AbyssinicaSIL-R.ttf");
#endif //CONSOLE_FONT_H
