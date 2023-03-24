#include "ClockWork.h"
#include <Core/Core.h>

#include <memory>
#include <thread>
#include <chrono>
#include <random>
#include <ECS/Entity/Entity.h>
#include <crtdbg.h>


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

        //app->OnPhysics += &onPhysics;
        //app->OnRender += &onRender;
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

void Basic()
{
    auto app = std::make_unique<CW::Core::Application>("game");

    //EventConsumer e(app.get());

    app->Run(640, 480);

    
}

using namespace CW;

void ECSTest() 
{
    ECS& ecs = ECS::Instance();
    ecs.Init();

    ecs.RegisterComponent<Gravity>();
    ecs.RegisterComponent<Transform>();
    ecs.RegisterComponent<RigidBody>();

    auto physicsSystem = ecs.RegisterSystem<PseudoPhysicsSystem>();
    {
        ComponentMask mask;
        mask.set(ecs.GetComponentType<Gravity>());
        mask.set(ecs.GetComponentType<Transform>());
        mask.set(ecs.GetComponentType<RigidBody>());
        ecs.SetSystemMask<PseudoPhysicsSystem>(mask);
    }

    std::vector<EntityId> entities(5);

    std::default_random_engine generator;
    std::uniform_real_distribution<float> randPosition(-100.0f, 100.0f);
    std::uniform_real_distribution<float> randRotation(0.0f, 3.0f);
    std::uniform_real_distribution<float> randScale(3.0f, 5.0f);
    std::uniform_real_distribution<float> randGravity(-10.0f, -1.0f);



    float scale = randScale(generator);

    for (auto& entity : entities)
    {
        entity = ecs.CreateEntity();

        ecs.AddComponent(entity, Gravity{ vec3(0.0f, randGravity(generator), 0.0f) });
        ecs.AddComponent(entity, RigidBody{ .velocity = vec3(0,0,0), .acceleration = vec3(0,0,0)});
        ecs.AddComponent(entity, Transform
        {  
            .position = vec3(randPosition(generator), randPosition(generator), randPosition(generator)),
            .rotation = vec3(randRotation(generator), randRotation(generator),  randRotation(generator)),
            .scale = vec3(1,1,1)
        });

    }
    
    ecs.DestroyEntity(entities[3]);


    float dt = 0;

    bool quit = 0;

    EventTracker consumer;

    physicsSystem->OnUpdated += &consumer.OnEvent;


    while (!quit)
    {
        auto tStart = std::chrono::high_resolution_clock::now();

        physicsSystem->Update(dt);

        auto tEnd = std::chrono::high_resolution_clock::now();
        dt = std::chrono::duration<float, std::milli>(tEnd - tStart).count();
    }

}

void EntityTest()
{
    /*ECS& ecs = ECS::Instance();
    ecs.Init();

    ecs.RegisterComponent<Gravity>();
    ecs.RegisterComponent<Transform>();
    ecs.RegisterComponent<RigidBody>();
    ecs.RegisterComponent<RigidBody>();

    auto physicsSystem = ecs.RegisterSystem<PseudoPhysicsSystem>();
    {
        ComponentMask mask;
        mask.set(ecs.GetComponentType<Gravity>());
        mask.set(ecs.GetComponentType<Transform>());
        mask.set(ecs.GetComponentType<RigidBody>());
        ecs.SetSystemMask<PseudoPhysicsSystem>(mask);
    }

    auto a = ecs.GetComponentType<RigidBody>();


    Entity& parent1 = Entity::New();
    
    std::default_random_engine generator;
    std::uniform_real_distribution<float> randPosition(-100.0f, 100.0f);
    std::uniform_real_distribution<float> randRotation(0.0f, 3.0f);
    std::uniform_real_distribution<float> randScale(3.0f, 5.0f);
    std::uniform_real_distribution<float> randGravity(-10.0f, -1.0f);

    float scale = randScale(generator);

    parent1.AddComponent(Gravity{ vec3(0.0f, randGravity(generator), 0.0f) });
    parent1.AddComponent(RigidBody{ .velocity = vec3(0,0,0), .acceleration = vec3(0,0,0) });
    parent1.AddComponent(Transform
        {
            .position = vec3(randPosition(generator), randPosition(generator), randPosition(generator)),
            .rotation = vec3(randRotation(generator), randRotation(generator),  randRotation(generator)),
            .scale = vec3(1,1,1)
        });

    Entity& child11 = Entity::New(parent1);
    Entity& child12 = Entity::New(parent1);

    Entity& parent2 = Entity::New();
    parent2.AddComponent(Gravity{ vec3(0.0f, randGravity(generator), 0.0f) });
    parent2.AddComponent(RigidBody{ .velocity = vec3(0,0,0), .acceleration = vec3(0,0,0) });
    parent2.AddComponent(Transform
        {
            .position = vec3(randPosition(generator), randPosition(generator), randPosition(generator)),
            .rotation = vec3(randRotation(generator), randRotation(generator),  randRotation(generator)),
            .scale = vec3(1,1,1)
        });

    Entity& child21 = Entity::New(parent2);
    Entity& child22 = Entity::New(parent2);
    Entity& child23 = Entity::New(parent2);

    child11.AddComponent(Gravity{ vec3(0.0f, randGravity(generator), 0.0f) });
    child11.AddComponent(RigidBody{ .velocity = vec3(0,0,0), .acceleration = vec3(0,0,0) });
    child11.AddComponent(Transform
        {
            .position = vec3(randPosition(generator), randPosition(generator), randPosition(generator)),
            .rotation = vec3(randRotation(generator), randRotation(generator),  randRotation(generator)),
            .scale = vec3(1,1,1)
        });

    child12.AddComponent(Gravity{ vec3(0.0f, randGravity(generator), 0.0f) });
    child12.AddComponent(RigidBody{ .velocity = vec3(0,0,0), .acceleration = vec3(0,0,0) });
    child12.AddComponent(Transform
        {
            .position = vec3(randPosition(generator), randPosition(generator), randPosition(generator)),
            .rotation = vec3(randRotation(generator), randRotation(generator),  randRotation(generator)),
            .scale = vec3(1,1,1)
        });

    child21.AddComponent(Gravity{ vec3(0.0f, randGravity(generator), 0.0f) });
    child21.AddComponent(RigidBody{ .velocity = vec3(0,0,0), .acceleration = vec3(0,0,0) });
    child21.AddComponent(Transform
        {
            .position = vec3(randPosition(generator), randPosition(generator), randPosition(generator)),
            .rotation = vec3(randRotation(generator), randRotation(generator),  randRotation(generator)),
            .scale = vec3(1,1,1)
        });

    child22.AddComponent(Gravity{ vec3(0.0f, randGravity(generator), 0.0f) });
    child22.AddComponent(RigidBody{ .velocity = vec3(0,0,0), .acceleration = vec3(0,0,0) });
    child22.AddComponent(Transform
        {
            .position = vec3(randPosition(generator), randPosition(generator), randPosition(generator)),
            .rotation = vec3(randRotation(generator), randRotation(generator),  randRotation(generator)),
            .scale = vec3(1,1,1)
        });

    child23.AddComponent(Gravity{ vec3(0.0f, randGravity(generator), 0.0f) });
    child23.AddComponent(RigidBody{ .velocity = vec3(0,0,0), .acceleration = vec3(0,0,0) });
    child23.AddComponent(Transform
        {
            .position = vec3(randPosition(generator), randPosition(generator), randPosition(generator)),
            .rotation = vec3(randRotation(generator), randRotation(generator),  randRotation(generator)),
            .scale = vec3(1,1,1)
        });

    float dt = 0;

    bool quit = 0;


    child12.Destroy();
    child22.Destroy();


    EventTracker consumer;

    physicsSystem->OnUpdated += &consumer.OnEvent;

    while (!quit)
    {
        auto tStart = std::chrono::high_resolution_clock::now();

        physicsSystem->Update(dt);

        auto tEnd = std::chrono::high_resolution_clock::now();
        dt = std::chrono::duration<float, std::milli>(tEnd - tStart).count();
    }*/

}

void EntityParentChildTest()
{
    ECS& ecs = ECS::Instance();
    ecs.Init();

    auto parent1 = Entity::Create("parent1");
    auto child11 = Entity::Create(parent1, "child11");
    auto child12 = Entity::Create(parent1, "child12");
    auto parent2 = Entity::Create("parent2");
    auto child21 = Entity::Create(parent2, "child21");
    auto child22 = Entity::Create(parent2, "child22");
    auto child23 = Entity::Create(parent2, "child23");

    /*int* asd = new int[3];

    asd[0] = 4;
    asd[1] = 5;
    asd[2] = 6;*/

    Entity::Destroy(child12);
    Entity::Destroy(child11);
    Entity::Destroy(parent1);
    
    

    // TODO: Entities and their parents hold a reference to each other, which results in cycle dependancy
    // either have one way relation or manually cleanup.
}

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    
    {
        EntityParentChildTest();
    }

    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtDumpMemoryLeaks();
    //Basic();
    return 0;
}