#include "Control/Button.h"
#include "Control/Canvas.h"

int main(int argc, char* argv[]) {

    auto* b1 = new Button();
    b1->SetRectangle(5, 5, 30, 100);
    b1->AddHandler(new MouseLeftButtonDownHandler<Button>([](Button* thisButton) {
        thisButton->SetBackgroundColor(180, 180, 180, 255);
    }));
    b1->AddHandler(new MouseLeftButtonUpHandler<Button>([](Button* thisButton) {
        thisButton->SetBackgroundColor(249, 249, 249, 255);
    }));
    b1->AddHandler(new MouseOverHandler<Button>([](Button* thisButton) {
        thisButton->SetBackgroundColor(210, 210, 210, 255);
    }));
    b1->AddHandler(new MouseOutHandler<Button>([](Button* thisButton) {
        thisButton->SetBackgroundColor(255, 255, 255, 255);
    }));

    auto* b2 = new Button();
    b2->SetRectangle(110, 5, 30, 100);
    b2->AddHandler(new MouseLeftButtonDownHandler<Button>([](Button* thisButton) {
        thisButton->SetBackgroundColor(180, 180, 180, 255);
    }));
    b2->AddHandler(new MouseLeftButtonUpHandler<Button>([](Button* thisButton) {
        thisButton->SetBackgroundColor(249, 249, 249, 255);
    }));
    b2->AddHandler(new MouseOverHandler<Button>([](Button* thisButton) {
        thisButton->SetBackgroundColor(210, 210, 210, 255);
    }));
    b2->AddHandler(new MouseOutHandler<Button>([](Button* thisButton) {
        thisButton->SetBackgroundColor(255, 255, 255, 255);
    }));

    Canvas canvas;
    canvas.Add(b1);
    canvas.Add(b2);

    canvas.Show();

    return 0;
}

/*
#include <iostream>


int main() {

    Console Console(800, 300);
    Console.SetColor({70, 130, 180, 0});
    Console.SetBackground({255, 255, 255, 1});
    Console.SetFont("/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf", 24);
    Console.SetFontStyle(Console.ITALIC);

    Console.WriteLine("Curriculum Vitae", false);

    Console.SetLineHeight(40);
    Console.SetFont("/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf", 18);
    Console.SetFontStyle(Console.NORMAL);

    Console.WriteLine("Name   : John Graham", false);

    Console.SetLineHeight(30);

    Console.WriteLine("", false);
    Console.WriteLine("Address: 39 Inglis Street", false);
    Console.WriteLine("         Seatoun", true);



    getchar();

    return EXIT_SUCCESS;
}
 */