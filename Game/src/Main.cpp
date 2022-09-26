#include "ClockWork.h"

int main()
{
    CW::Core::Window* window = new CW::Core::Window();

    window->InitWindow("Game", 1440, 960);
    window->Update();
    

    delete window;
    return 0;
}