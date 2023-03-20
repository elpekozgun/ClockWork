#include "ClockWork.h"
#include <Core/Core.h>

#include <memory>
#include <thread>
#include <chrono>

class EventConsumer
{
public:
    CW::Core::Application* _app;

    Delegate<EventConsumer, std::string> onPhysics;
    Delegate<EventConsumer, int> onRender;

    EventConsumer(CW::Core::Application* const app) : _app(app)
    {
        onPhysics.Set(this, &EventConsumer::OnPhysics);
        onRender.Set(this, &EventConsumer::OnRender);

        app->OnPhysics += &onPhysics;
        app->OnRender += &onRender;
    }

private:
    void OnPhysics(std::string param)
    {
        cout << param << "\n";
    }

    void OnRender(int renderTime)
    {
        cout << "Frame Time: " << renderTime << " hz\n";
    }
};

int main()
{
    auto app = std::make_unique<CW::Core::Application>("game");

    EventConsumer e(app.get());

    app->Run(1920, 1080);


    return 0;
}