#include "ClockWork.h"
#include <Core/Core.h>

#include <memory>
#include <thread>
#include <chrono>
#include <random>

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

    app->Run(1920, 1080);

    
}

void ECSTest() 
{
    ECS& ecs = ECS::Instance();
    ecs.Init();

    ecs.RegisterComponent<Gravity>();
    ecs.RegisterComponent<Transform>();
    ecs.RegisterComponent<RigidBody>();

    auto physicsSystem = ecs.RegisterSystem<PseudoPhysicsSystem>();

    Mask mask;
    mask.set(ecs.GetComponentType<Gravity>());
    mask.set(ecs.GetComponentType<Transform>());
    mask.set(ecs.GetComponentType<RigidBody>());
    ecs.SetSystemMask<PseudoPhysicsSystem>(mask);

    std::vector<Entity> entities(1000);

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

int main()
{
    ECSTest();
    return 0;
}