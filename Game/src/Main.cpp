#include <Core/Core.h>
#include <memory>
#include <thread>
#include <chrono>
#include <random>
#include <crtdbg.h>

#include "ClockWork.h"
#include "../Example.h"

using namespace CW;

class EventTracker
{
public:
    Delegate<EventTracker, float> OnEvent;

    EventTracker()
    {
        OnEvent.Set(this, &EventTracker::OnPhysics);
    }
    

private:
    void OnPhysics(float time)
    {
        cout << "physics updated in: "<< time << " ms.\n";
    }
};

void SampleApp()
{
    auto app = App::Create("game")->
        AddSystem<ExampleSystem>()->
        RegisterComponent<ExampleComponent>();

    Scene scene;

    auto parent1 = scene.CreateEntity("parent1");
    auto parent2 = scene.CreateEntity("parent2");

    auto child11 = scene.CreateEntity("child11", parent1);
    auto child12 = scene.CreateEntity("child12", parent1);

    scene.AddComponent(parent1, ExampleComponent{ 5, 5 });
    scene.AddComponent(child11, ExampleComponent{ 10, 5 });
    scene.AddComponent(child12, ExampleComponent{ 15, 5 });
    scene.AddComponent(parent2, ExampleComponent{ 5, 10 });

    app->Run(800, 600);

    delete app;
}

using namespace CW;

void RandomDistributionExample() 
{
    std::default_random_engine generator;
    std::uniform_real_distribution<float> randPosition(-100.0f, 100.0f);
    std::uniform_real_distribution<float> randRotation(0.0f, 3.0f);
    std::uniform_real_distribution<float> randScale(3.0f, 5.0f);
    std::uniform_real_distribution<float> randGravity(-10.0f, -1.0f);

    float scale = randScale(generator);
    randGravity(generator);
    randPosition(generator);
    randRotation(generator);
}

void SceneTest()
{
    ECS& ecs = ECS::Instance();
    ecs.Init();

    ecs.RegisterComponent<ExampleComponent>();

    auto ts = ecs.RegisterSystem<ExampleSystem>();

    Scene scene;

    auto parent1 = scene.CreateEntity("parent1");
    auto parent2 = scene.CreateEntity("parent2");

    auto child11 = scene.CreateEntity("child11", parent1);
    auto child12 = scene.CreateEntity("child12", parent1);
        
    scene.AddComponent(parent1, ExampleComponent{ 5, 5});
    scene.AddComponent(child11, ExampleComponent{ 10, 5 });
    scene.AddComponent(child12, ExampleComponent{ 15, 5 });
    scene.AddComponent(parent2, ExampleComponent{ 5, 10 });
    
    //EventTracker consumer;

    float dt = 0;
    while (true)
    {
        auto tStart = std::chrono::high_resolution_clock::now();

        //std::cout << dt << "\n";

        ts->Update(dt);

        auto tEnd = std::chrono::high_resolution_clock::now();
        dt = std::chrono::duration<float, std::milli>(tEnd - tStart).count();
    }

    scene.DestroyEntity(parent1);
    scene.DestroyEntity(parent2);
}







int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    
    {
        SampleApp();
        //SceneTest();
    }

    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtDumpMemoryLeaks();
    return 0;
}