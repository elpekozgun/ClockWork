#include "ECS/ECS.h"
#include "ECS/component.h"

struct ExampleSizeComponent
{
    float width;
    float height;
};

struct ExamplePositionComponent
{
    glm::vec3 Position;
};

class ExampleSizeSystem : public CW::ISystem
{
public:
    ExampleSizeSystem()
    {
    }

    void Update(float dt) override
    {
        _currentDelta += dt;
        if (_currentDelta >= _updateRate)
        {
            CW::ECS& instance = CW::ECS::Instance();
            for (auto& entity : _entities)
            {
                auto& example = instance.GetComponent<ExampleSizeComponent>(entity);

                example.width += dt;
                example.height += dt;

                std::cout << "entityId: " << entity << "- width :" << example.width << " height: " << example.height << "\n";
            }
            _currentDelta = 0;
        }
    };
    void GetInput(int getinput) override {};
};

class ExamplePositionSystem : public CW::ISystem
{
public:

    ExamplePositionSystem()
    {
    }

    void Update(float dt) override
    {
        _currentDelta += dt;
        if (_currentDelta >= _updateRate)
        {
            CW::ECS& instance = CW::ECS::Instance();
            for (auto& entity : _entities)
            {
                auto& examplePos = instance.GetComponent<ExamplePositionComponent>(entity);

                examplePos.Position += glm::vec3(dt);

                std::cout << "entityId: " << entity 
                    << " position X : " << examplePos.Position.x
                    << " position Y : " << examplePos.Position.y
                    << " position Z : " << examplePos.Position.z << "\n";
            }
            _currentDelta = 0;
        }
    };
    void GetInput(int getinput) override {};
};


