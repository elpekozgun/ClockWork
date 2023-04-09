#pragma once 
#include <iostream>
#include "glm.hpp"
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <chrono>

using namespace std;
using namespace glm;


// 36 byte
struct Transform
{
    vec3 position;
    vec3 rotation;
    vec3 scale;
};

// 36 byte
struct Physics
{
    vec3 acceleration;
    vec3 velocity;
    vec3 torque;
};

struct Render
{
    float mesh;
    float material;

    void Draw() { };
};

// 72 byte
struct TransformPhysics
{
    vec3 position;
    vec3 rotation;
    vec3 scale;
    vec3 acceleration;
    vec3 velocity;
    vec3 torque;
};

// 72 byte
struct TransformPhysicsRender
{
    vec3 position;
    vec3 rotation;
    vec3 scale;
    vec3 acceleration;
    vec3 velocity;
    vec3 torque;
    float mesh;
    float material;

    void Draw() { };
};

class IComponentContainer
{
public:
    virtual ~IComponentContainer() = default;
};

template<typename T>
class ComponentContainer : public IComponentContainer
{
public:
    void AddData(int id, T component)
    {
        Components.push_back(component);
    }

    T& GetData(unsigned int id)
    {
        return Components[id];
    }

    std::vector<T> Components{};
};

class TupleSOA
{
public:
    std::vector<tuple<Transform, Render, Physics>> GetTuples()
    {
        std::vector<tuple<Transform, Render, Physics>> tuples;

        for (size_t i = 0; i < Transforms.size(); i++)
        {
            tuples.emplace_back(Transforms[i], Renders[i], physics[i]);
        }
        return tuples;
    }

    void AddData(Transform t, Render r, Physics p)
    {
        Transforms.emplace_back(t);
        Renders.emplace_back(r);
        physics.emplace_back(p);
    }

    void AddDataToArcheTypes(Transform t, Render r, Physics p)
    {
        Archetypes.emplace_back(t, r, p);
    }

    std::vector<tuple<Transform, Render, Physics>> GetArchetypeTuple()
    {
        return Archetypes;
    }

    std::vector<tuple<Transform, Render, Physics>> Archetypes;

    std::vector<Transform> Transforms;
    std::vector<Physics> physics;
    std::vector<Render> Renders;
};

template<typename T>
class ArrayOfStruct
{
public:

    template<typename T>
    T& GetComponent(int entity)
    {
        return TVector.GetData(entity);
    }

    template<typename T>
    void AddData(T data, int id)
    {
        TVector.AddData(id, data);
    }


    ComponentContainer<T> TVector;
};

class StructOfArrays
{
public:
    template<typename T>
    void AddType()
    {
        string type = typeid(T).name();
        if (!_componentTypes.contains(type))
        {
            _componentTypes.insert(type);
            _componentArrays.insert({ type, std::make_shared<ComponentContainer<T>>() });
        }
    }



    template<typename T>
    void AddData(unsigned int id, T data)
    {
        GetComponentContainer<T>()->AddData(id, data);
    }

    //template<typename... T>
    //std::shared_ptr<ComponentContainer<T...>> GetComponentContainer()
    //{
    //    string type = typeid(typename std::common_type<T...>::type).name();
    //    //std::string typeName = typeid(T).name();
    //    return std::static_pointer_cast<ComponentContainer<T...>>(_componentArrays[type]);
    //}

    template<typename T>
    std::shared_ptr<ComponentContainer<T>> GetComponentContainer()
    {
        std::string type = typeid(T).name();
        return std::static_pointer_cast<ComponentContainer<T>>(_componentArrays[type]);
    }

    template<typename T>
    T& GetComponent(int entity)
    {
        return GetComponentContainer<T>()->GetData(entity);
    }

    std::unordered_map<std::string, std::shared_ptr<IComponentContainer>> _componentArrays;
    std::unordered_set<std::string> _componentTypes{};

};


void BenchmarkSOA(unsigned int count, unsigned int iteration)
{
    std::vector<std::string> timesSoA;

    double totalTime = 0.0;

    StructOfArrays soa;

    soa.AddType<Transform>();
    soa.AddType<Physics>();
    soa.AddType<Render>();

    for (size_t i = 0; i < count; i++)
    {
        soa.AddData<Transform>(i, Transform{ vec3(0), vec3(0), vec3(1) });
        soa.AddData<Physics>(i, Physics{ vec3(0,0.001f, 0), vec3(0, 0.1f, 0),  vec3(0.001f,0 , 0) });
        soa.AddData<Render>(i, Render{});
    }

    for (unsigned int j = 0; j < iteration; j++)
    {
        auto start = chrono::high_resolution_clock::now();
        float dt = 0.02f;
        for (unsigned int i = 0; i < count; i++)
        {
            auto& t = soa.GetComponent<Transform>(i);
            auto& p = soa.GetComponent<Physics>(i);
            auto& r = soa.GetComponent<Render>(i);

            p.velocity += dt * p.acceleration;
            p.torque += vec3(0);
            t.position += p.velocity;
            t.rotation += p.torque;
            t.scale = vec3(1);
            r.material = 6;
            r.mesh = 32;
            r.Draw();
        }
        auto end = chrono::high_resolution_clock::now();

        std::chrono::duration<double> elapsed = end - start;
        auto duration = elapsed.count();

        timesSoA.push_back(std::to_string(duration));
        totalTime += duration;
    }


    std::cout << "SOA: \t\t" << totalTime / iteration << "\n";

}

void BenchmarkSOAWithArray(unsigned int count, unsigned int iteration)
{
    std::vector<std::string> timesSoA;

    double totalTime = 0.0;

    StructOfArrays soa;

    soa.AddType<Transform>();
    soa.AddType<Physics>();
    soa.AddType<Render>();

    for (size_t i = 0; i < count; i++)
    {
        soa.AddData<Transform>(i, Transform{ vec3(0), vec3(0), vec3(1) });
        soa.AddData<Physics>(i, Physics{ vec3(0,0.001f, 0), vec3(0, 0.1f, 0),  vec3(0.001f,0 , 0) });
        soa.AddData<Render>(i, Render{});
    }


    for (unsigned int j = 0; j < iteration; j++)
    {
        auto renders = *soa.GetComponentContainer<Render>();
        auto transforms = *soa.GetComponentContainer<Transform>();
        auto physics = *soa.GetComponentContainer<Physics>();

        auto start = chrono::high_resolution_clock::now();
        float dt = 0.02f;
        for (unsigned int i = 0; i < renders.Components.size(); i++)
        {
            auto& t = transforms.Components[i];
            auto& p = physics.Components[i];
            auto& r = renders.Components[i];

            p.velocity += dt * p.acceleration;
            p.torque += vec3(0);
            t.position += p.velocity;
            t.rotation += p.torque;
            t.scale = vec3(1);
            r.material = 6;
            r.mesh = 32;
            r.Draw();
        }
        auto end = chrono::high_resolution_clock::now();

        std::chrono::duration<double> elapsed = end - start;
        auto duration = elapsed.count();

        timesSoA.push_back(std::to_string(duration));
        totalTime += duration;
    }


    std::cout << "SOA Direct: \t" << totalTime / iteration << "\n";

}

void BenchmarkSOAWithTuple(unsigned int count, unsigned int iteration)
{
    std::vector<std::string> timesSoA;

    double totalTime = 0.0;

    TupleSOA soa;

    for (size_t i = 0; i < count; i++)
    {
        soa.AddData(Transform{ vec3(0), vec3(0), vec3(1) }, Render{}, Physics{ vec3(0,0.001f, 0), vec3(0, 0.1f, 0),  vec3(0.001f,0 , 0) });
    }


    for (unsigned int j = 0; j < iteration; j++)
    {
        auto start = chrono::high_resolution_clock::now();
        float dt = 0.02f;

        auto tuples = soa.GetTuples();

        for (unsigned int i = 0; i < tuples.size(); i++)
        {
            auto& t = std::get<Transform>(tuples[i]);
            auto& r = std::get<Render>(tuples[i]);
            auto& p = std::get<Physics>(tuples[i]);

            p.velocity += dt * p.acceleration;
            p.torque += vec3(0);
            t.position += p.velocity;
            t.rotation += p.torque;
            t.scale = vec3(1);
            r.material = 6;
            r.mesh = 32;
            r.Draw();
        }
        auto end = chrono::high_resolution_clock::now();

        std::chrono::duration<double> elapsed = end - start;
        auto duration = elapsed.count();

        timesSoA.push_back(std::to_string(duration));
        totalTime += duration;
    }


    std::cout << "SOA Query: \t" << totalTime / iteration << "\n";

}

void BenchmarkSOAWithArchetype(unsigned int count, unsigned int iteration)
{
    std::vector<std::string> timesSoA;

    double totalTime = 0.0;

    TupleSOA soa;

    for (size_t i = 0; i < count; i++)
    {
        soa.AddDataToArcheTypes(Transform{ vec3(0), vec3(0), vec3(1) }, Render{}, Physics{ vec3(0,0.001f, 0), vec3(0, 0.1f, 0),  vec3(0.001f,0 , 0) });
    }


    for (unsigned int j = 0; j < iteration; j++)
    {
        auto start = chrono::high_resolution_clock::now();
        float dt = 0.02f;

        auto tuples = soa.Archetypes;

        for (unsigned int i = 0; i < tuples.size(); i++)
        {
            auto& t = std::get<Transform>(tuples[i]);
            auto& r = std::get<Render>(tuples[i]);
            auto& p = std::get<Physics>(tuples[i]);

            p.velocity += dt * p.acceleration;
            p.torque += vec3(0);
            t.position += p.velocity;
            t.rotation += p.torque;
            t.scale = vec3(1);
            r.material = 6;
            r.mesh = 32;
            r.Draw();
        }
        auto end = chrono::high_resolution_clock::now();

        std::chrono::duration<double> elapsed = end - start;
        auto duration = elapsed.count();

        timesSoA.push_back(std::to_string(duration));
        totalTime += duration;
    }


    std::cout << "SOA Archetype: \t" << totalTime / iteration << "\n";

}

void BenchmarkAOS(unsigned int count, unsigned int iteration)
{
    std::vector<std::string> timesAoS;

    ArrayOfStruct<TransformPhysicsRender> aos;

    double totalTime = 0.0;

    for (size_t i = 0; i < count; i++)
    {
        aos.AddData(TransformPhysicsRender{ vec3(0), vec3(0), vec3(1), vec3(0,0.001f, 0), vec3(0, 0.1f, 0), vec3(0.001f,0 , 0) }, i);
    }

    for (unsigned int j = 0; j < iteration; j++)
    {
        auto start = chrono::high_resolution_clock::now();
        float dt = 0.02f;
        for (unsigned int i = 0; i < aos.TVector.Components.size(); i++)
        {
            auto& tp = aos.GetComponent<TransformPhysicsRender>(i);

            tp.velocity += dt * tp.acceleration;
            tp.torque += vec3(0);
            tp.position += tp.velocity;
            tp.rotation += tp.torque;
            tp.scale = vec3(1);
            tp.material = 6;
            tp.mesh = 32;
            tp.Draw();

        }
        auto end = chrono::high_resolution_clock::now();

        std::chrono::duration<double> elapsed = end - start;
        auto duration = elapsed.count();
        timesAoS.push_back(std::to_string(duration));

        totalTime += duration;
    }


    std::cout << "AOS: \t\t" << totalTime / iteration << "\n";
}
