#include "ClockWork.h"

#include <memory>

int main()
{
    auto app = std::make_unique<CW::Core::Application>("game");
    app->Run(1920, 1080);
    
    return 0;
}