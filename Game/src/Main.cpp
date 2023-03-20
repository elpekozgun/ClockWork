#include "ClockWork.h"
#include <Core/Core.h>

#include <memory>
#include <thread>
#include <chrono>

class Test
{
public:
    Event TestEvent;

    inline void DoStuff(int val)
    {
        auto x = std::chrono::steady_clock::now() + std::chrono::milliseconds(val);
        std::this_thread::sleep_until(x);
        TestEvent.Invoke();
    }

};

class AnotherClass
{
public:

    Delegate<AnotherClass> Action;

    inline AnotherClass()
    {
        Action.Set(this, &AnotherClass::Trigger);
    }

    void Trigger()
    {
        cout << "hello\n";
    }
};

void OnPhysics()
{
    cout << "physics updated\n";
}

void OnRender()
{
    cout << "render completed\n";
}

class EventConsumer
{
public:
    Delegate<EventConsumer> onPhysics;
    Delegate<EventConsumer> onRender;

    inline EventConsumer()
    {
        onPhysics.Set(this, &EventConsumer::OnPhysics);
        onRender.Set(this, &EventConsumer::OnRender);
    }

private:
    void OnPhysics()
    {
        cout << "physics updated\n";
    }

    void OnRender()
    {
        cout << "render completed\n";
    }
};

int main()
{
    auto app = std::make_unique<CW::Core::Application>("game");

    EventConsumer e;
    app->OnRender += &e.onRender;
    app->OnPhysics += &e.onPhysics;

    app->Run(1920, 1080);


    return 0;
}