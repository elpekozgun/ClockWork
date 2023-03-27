#include <Core/Core.h>
#include <memory>
#include <thread>
#include <chrono>
#include <random>
#include <crtdbg.h>

#include "ClockWork.h"
#include "../Example.h"
#include "Systems/PlayerMovement.h"

#include <typeinfo>
#include <iostream>

using namespace CW;

void SampleApp()
{
    auto app = App::Create("game")->
        AddSystem<ExampleSizeSystem, ExampleSizeComponent>()->
        AddSystem<ExamplePositionSystem, ExamplePositionComponent>();

    auto& a = ECS::Instance();

    Scene scene;

    auto parent1 = scene.CreateEntity("parent1");
    auto parent2 = scene.CreateEntity("parent2");

    auto child11 = scene.CreateEntity("child11", parent1);
    auto child12 = scene.CreateEntity("child12", parent1);

    auto child13 = scene.CopyEntity(child11);


    scene.AddComponents(parent1, ExampleSizeComponent{ 5, 5 }, ExamplePositionComponent{ glm::vec3(1,1,1) });
    scene.AddComponents(child11, ExampleSizeComponent{ 10, 5 }, ExamplePositionComponent{ glm::vec3(1,1,1) });
    scene.AddComponents(child12, ExampleSizeComponent{ 15, 5 });
    scene.AddComponents(parent2, ExampleSizeComponent{ 5, 10 });

    app->Run(640,480);

    delete app;
}

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

    ecs.RegisterComponent<ExampleSizeComponent>();

    auto ts = ecs.RegisterSystem<ExampleSizeSystem/*, ExampleComponent*/>();

    Scene scene;

    auto parent1 = scene.CreateEntity("parent1");
    auto parent2 = scene.CreateEntity("parent2");

    auto child11 = scene.CreateEntity("child11", parent1);
    auto child12 = scene.CreateEntity("child12", parent1);
        
    //scene.AddComponent(parent1, ExampleComponent{ 5, 5});
    //scene.AddComponent(child11, ExampleComponent{ 10, 5 });
    //scene.AddComponent(child12, ExampleComponent{ 15, 5 });
    //scene.AddComponent(parent2, ExampleComponent{ 5, 10 });
    
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


void Game()
{
    auto app = App::Create("game")->
        AddSystem<PlayerController, InputComponent, TransformComponent>()->
        AddSystem<

    //auto& a = ECS::Instance();

    Scene scene;

    auto player = scene.CreateEntity("player");

    scene.AddComponents(player, TransformComponent(), InputComponent());

    app->Run(640, 480);

    delete app;
}





int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    
    {
        /*GravityComponent c;
        auto& a = c.GetTypeInfo();

        std::cout << a.name() << std::endl;*/


        SampleApp();
        //SceneTest();
    }

    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtDumpMemoryLeaks();
    return 0;
}