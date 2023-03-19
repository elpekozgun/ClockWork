#include "ClockWork.h"

#include <memory>
#include <thread>
#include <chrono>

class Test
{
public:
    Event<int> TestEvent;

    void DoStuff(int val)
    {
        auto x = std::chrono::steady_clock::now() + std::chrono::milliseconds(val);
        std::this_thread::sleep_until(x);
        TestEvent.Invoke(val);
    }

};

//void Action(int t)
//{
//    cout << "hello" << endl;
//}

class AnotherClass
{
public:
    Delegate<int> Act;
    std::function<void(int)> act = std::bind(&AnotherClass::Action, this, placeholders::_1);

    AnotherClass()
    {
        Act.Set(act);
        //Act = (Action);
    }

    void Action(int t)
    {

    }
};




int main()
{
    //auto app = std::make_unique<CW::Core::Application>("game");
    //app->Run(1920, 1080);
    
    AnotherClass class1;
    AnotherClass class2;
    AnotherClass class3;
    AnotherClass class4;

    Test test;
    test.TestEvent += class1.Act;
    test.TestEvent += class2.Act;
    test.TestEvent += class3.Act;
    test.TestEvent += class4.Act;

    test.DoStuff(1000);

    return 0;
}