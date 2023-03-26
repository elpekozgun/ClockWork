#include "ECS/ECS.h"

struct ExampleComponent
{
    float width;
    float height;
};

class ExampleSystem : public CW::ISystem
{
public:
    void Update(float dt) override
    {
        _currentDelta += dt;
        if (_currentDelta >= _updateRate)
        {
            CW::ECS& instance = CW::ECS::Instance();
            for (auto& entity : _entities)
            {
                auto& example = instance.GetComponent<ExampleComponent>(entity);

                example.width += dt;
                example.height += dt;

                std::cout << "entityId: " << entity << "- width :" << example.width << " height: " << example.height << "\n";
            }
            _currentDelta = 0;
        }
    };
    void GetInput(int getinput) override {};
};
