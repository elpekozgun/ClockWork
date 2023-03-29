#include <Core/Core.h>
#include <memory>
#include <thread>
#include <chrono>
#include <random>
#include <crtdbg.h>

#include "ClockWork.h"
#include "../Example.h"
#include "Game/Systems/PlayerController.h"

#include <typeinfo>
#include <iostream>

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

void Game()
{
    auto app = App::Create("game")->
        AddSystem<PlayerController, TransformComponent, Player>()->
        AddSystem<PhysicsSystem, TransformComponent, PhysicsComponent>();

    Scene scene;

    auto player = scene.CreateEntity("player");
    auto item1 = scene.CreateEntity("item1");
    auto item2 = scene.CreateEntity("item2");
    auto item3 = scene.CreateEntity("item3");
    auto item4 = scene.CreateEntity("item4");
    auto item5 = scene.CreateEntity("item5");
    auto player2 = scene.CreateEntity("player2");


    scene.AddComponents(player, TransformComponent(), Player());
    scene.AddComponents(item1, TransformComponent{glm::vec3(1, 1, 1), glm::vec3(1, 2, 3), glm::vec3(1, 2, 3)}, PhysicsComponent{ glm::vec3(0, 0, 0.1f), glm::vec3(0, 0, 1)});
    scene.AddComponents(player2, TransformComponent(), Player());
    scene.AddComponents(item2, TransformComponent{ glm::vec3(2, 2, 2), glm::vec3(1, 2, 3), glm::vec3(1, 2, 3) }, PhysicsComponent{ glm::vec3(0.1f, 0, 0), glm::vec3(1, 0, 0)});
    scene.AddComponents(item3, TransformComponent{ glm::vec3(3, 3, 3), glm::vec3(1, 2, 3), glm::vec3(1, 2, 3) }, PhysicsComponent{glm::vec3(0.1f, 0, 0.1f), glm::vec3(1, 0, 1)});
    scene.AddComponents(item4, TransformComponent{ glm::vec3(4, 4, 4), glm::vec3(1, 2, 3), glm::vec3(1, 2, 3) }, PhysicsComponent{glm::vec3(0.1f, 0.2f, 0.3f), glm::vec3(-1, -1, -1)});
    scene.AddComponents(item5, TransformComponent{ glm::vec3(5, 5, 5), glm::vec3(1, 2, 3), glm::vec3(1, 2, 3) }, PhysicsComponent{ glm::vec3(0.3f, -0.1f, 0.2f), glm::vec3(-1, 2, -1) });

    scene.DestroyEntity(item2);


    app->Run(640, 480);

    delete app;
}



int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    
    {
        Game();
    }

    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtDumpMemoryLeaks();
    return 0;
}