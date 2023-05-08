#include <Core/Core.h>
#include <memory>
#include <thread>
#include <chrono>
#include <random>
#include <crtdbg.h>
#include <typeinfo>
#include <iostream>

#include "ClockWork.h"
#include "../Example.h"
#include <Graphics/Skybox.h>



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

MeshComponent MakeGround()
{

    std::vector<Vertex> vertices =
    {
        Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(0.0f, 0.0f)},
        Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec2(0.0f, 1.0f)},
        Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
        Vertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)}
    };
    // Indices for vertices order
    std::vector<GLuint> indices =
    {
        0, 1, 2,
        0, 2, 3
    };

    std::vector<Texture> textures
    {
        Texture(R"(C:\_Dev\ClockWork\ClockWork\res\Texture\crate.png)", "Diffuse", 0),
        Texture(R"(C:\_Dev\ClockWork\ClockWork\res\Texture\cratespecular.png)", "Specular", 1)
    };
    MeshComponent groundMesh;

    groundMesh.Vertices = vertices;
    groundMesh.Indices = indices;
    groundMesh.Textures = textures;
    groundMesh.Setup();

    auto vertexShader = ShaderFactory::CreateShaderSource(R"(C:\_Dev\ClockWork\ClockWork\res\Shader\Default.vert)", ShaderType::Vertex);
    auto fragmentShader = ShaderFactory::CreateShaderSource(R"(C:\_Dev\ClockWork\ClockWork\res\Shader\Default.frag)", ShaderType::Fragment);
    groundMesh.Shader = ShaderFactory::CreateShader("shader", { vertexShader, fragmentShader });
    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0, 1.9f, 0);

    groundMesh.Shader.Use();

    groundMesh.Shader.setVec4("LightColor", lightColor);
    groundMesh.Shader.SetFloat("Shineness", 32.0f);
    groundMesh.Shader.setVec3("spotlight.ambient", 0.2f, 0.2f, 0.2f);
    groundMesh.Shader.setVec3("spotlight.diffuse", 0.8f, 0.8f, 0.8f);
    groundMesh.Shader.setVec3("spotlight.specular", 1.0f, 1.0f, 1.0f);
    groundMesh.Shader.SetFloat("spotlight.cutOff", cos(glm::radians(20.5f)));
    groundMesh.Shader.SetFloat("spotlight.outerCutOff", cos(glm::radians(26.0f)));
    groundMesh.Shader.SetFloat("spotlight.Kconstant", 1.0f);
    groundMesh.Shader.SetFloat("spotlight.Klinear", 0.027f);
    groundMesh.Shader.SetFloat("spotlight.Kquad", 0.0028f);
    groundMesh.Shader.setVec3("pointLights[0].position", lightPos);
    groundMesh.Shader.setVec3("pointLights[0].diffuse", 1.0f, 1.0f, 1.0f);
    groundMesh.Shader.setVec3("pointLights[0].ambient", 0.15f, 0.15f, 0.15f);
    groundMesh.Shader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    groundMesh.Shader.SetFloat("pointLights[0].Kconstant", 1.0f);
    groundMesh.Shader.SetFloat("pointLights[0].Klinear", 0.09f);
    groundMesh.Shader.SetFloat("pointLights[0].Kquad", 0.032f);
    groundMesh.Shader.SetBool("IsBlinnPhong", true);


    return groundMesh;
}

MeshComponent makeLight()
{
    MeshComponent lightMesh;

    std::vector<Vertex> vertices
    {
        Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
        Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
        Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
        Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
        Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
        Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
        Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
        Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
    };

    std::vector<unsigned int> indices
    {
        0, 1, 2,
        0, 2, 3,
        0, 4, 7,
        0, 7, 3,
        3, 7, 6,
        3, 6, 2,
        2, 6, 5,
        2, 5, 1,
        1, 5, 4,
        1, 4, 0,
        4, 5, 6,
        4, 6, 7
    };
    
    lightMesh.Vertices = vertices;
    lightMesh.Indices = indices;

    lightMesh.Setup();

    auto lightVertex = ShaderFactory::CreateShaderSource(R"(C:\_Dev\ClockWork\ClockWork\res\Shader\LightVertex.glsl)", ShaderType::Vertex);
    auto lightFragment = ShaderFactory::CreateShaderSource(R"(C:\_Dev\ClockWork\ClockWork\res\Shader\LightFragment.glsl)", ShaderType::Fragment);
    lightMesh.Shader = ShaderFactory::CreateShader("shader", { lightVertex, lightFragment });

    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0, 1.9f, 0);

    lightMesh.Shader.Use();
    lightMesh.Shader.setVec4("LightColor", lightColor);

    return lightMesh;
}

void Game()
{
    auto app = App::Create("game")->
        AddSystem<PlayerController, TransformComponent, Player>()->
        AddSystem<PhysicsSystem, TransformComponent, PhysicsComponent>()->
        AddSystem<CameraSystem, CameraComponent>()->
        AddSystem<CollisionSystem, AABBComponent, TransformComponent>()->
        AddSystem<RenderSystem, RenderableComponent, TransformComponent>()->
        AddSystem<AABBRenderer, AABBComponent, TransformComponent>()->
        AddSystem<AnimationSystem, RenderableComponent>();


    auto& ecs = app->GetECS();

    

    CameraComponent camera;

    camera.Position = glm::vec3(0, 0, 1);
    camera.Forward = glm::vec3(0, 0, -1);
    camera.Up = glm::vec3(0, 1, 0);
    //camera.Position = glm::vec3(-1.45f, 0.9f, -0.8f);
    //camera.Forward = glm::vec3(0.832167f, -0.377841f, 0.405875f);
    //camera.Up = glm::vec3(0.339601f, 0.925871f, 0.165634f);
    camera.Width = 1920;
    camera.height = 1080;
    camera.FoV = 60;
    camera.speed = 15.0f;
    camera.sensitivity = 0.1f;
    camera.Near = 0.5f;
    camera.Far = 500.0f;

    ecs.RegisterSingletonComponent<CameraComponent>(camera);


    Scene scene(ecs);
    //Model CharacterModel(R"(C:/Users/user/Desktop/kalestra_the_sorceress/scene.gltf)");
    //Model CharacterModel(R"(C:/Users/user/Desktop/spartan_armour_mkv_-_halo_reach/Scene.gltf)");
    Model CharacterModel(R"(C:/Users/user/Desktop/Idle/Idle.dae)");
    //Model CharacterModel(R"(C:/Users/user/Desktop/Great Sword Idle/Great Sword Idle.dae)");
    //Model CharacterModel(R"(C:/_Dev/ClockWork/ClockWork/res/3DModel/maria/maria WProp J J Ong.dae)");
    //Model CharacterModel(R"(C:/Users/user/Desktop/robot_steampunk_3d-coat_4.5_pbr/scene.gltf)");
    //Model CharacterModel(R"(C:/Users/user/Desktop/Vanguard/Vanguard By T. Choonyung.dae)");
    //Model CharacterModel(R"(C:/Users/user/Desktop/damaged_helmet/scene.gltf)");
    //Model CharacterModel(R"(C:/Users/user/Desktop/pbrsphere/scene.gltf)");
    //Model CharacterModel(R"(C:/Users/user/Desktop/sphere/source/sphere_sampleScene_1cm.fbx)");
    //Model CharacterModel(R"(C:/Users/user/Desktop/pbrspherefbx/source/sphere_sampleScene_1cm.fbx)");
    //Model CharacterModel(R"(C:/Users/user/Desktop/robot/scene.gltf)");
    //Model CharacterModel(R"(C:/Users/user/Desktop/doom_eternal_slayer/scene.gltf)");
    //Model CharacterModel(R"(C:/Users/user/Desktop/medieval_vagrant_knights/scene.gltf)");
    //Model CharacterModel(R"(C:/Users/user/Desktop/skull_downloadable/scene.gltf)");
    //Model CharacterModel(R"(C:/Users/user/Desktop/golden_knight/scene.gltf)");
    //Model CharacterModel(R"(C:/Users/user/Desktop/Character/scene.gltf)");
    //Model CharacterModel(R"(C:/_Dev/ClockWork/ClockWork/res/3DModel/gladiator.glb)");
    //Model CharacterModel(R"(C:/_Dev/ClockWork/ClockWork/res/3DModel/cat/cat.obj)");

    //Animation animation(R"(C:/_Dev/ClockWork/ClockWork/res/3DModel/maria/maria WProp J J Ong.dae)", &CharacterModel);
    //Animator animator(&animation);
    
    auto as = std::dynamic_pointer_cast<AnimationSystem>(ecs.GetSystem<AnimationSystem>());
    //Animation animation1(R"(C:/Users/user/Desktop/spartan_armour_mkv_-_halo_reach/Scene.gltf)", &CharacterModel);
    //Animation animation(R"(C:/Users/user/Desktop/Idle/Idle.dae)", &CharacterModel);

    Animation animation1(R"(C:/Users/user/Desktop/Idle/Idle.dae)", &CharacterModel);
    Animation animation2(R"(C:/Users/user/Desktop/Great Sword Idle/Great Sword Idle.dae)", &CharacterModel);
    //Animation animation3(R"(C:/Users/user/Desktop/GreatSwordAnimations/great sword walk.fbx)", &CharacterModel);
    //Animation animation4(R"(C:/Users/user/Desktop/GreatSwordAnimations/two handed sword death (2).fbx)", &CharacterModel);
    
    Animator animator(&animation2);

    animator.RegisterAnimation("idle", & animation1);
    animator.RegisterAnimation("greatSword", & animation2);

    //
    as->_animator = &animator;

    //auto vertexShader = ShaderFactory::CreateShaderSource(R"(C:\_Dev\ClockWork\ClockWork\res\Shader\Default.vert)", ShaderType::Vertex);
    auto vertexShader = ShaderFactory::CreateShaderSource(R"(C:\_Dev\ClockWork\ClockWork\res\Shader\animated.vert)", ShaderType::Vertex);
    //auto fragmentShader = ShaderFactory::CreateShaderSource(R"(C:\_Dev\ClockWork\ClockWork\res\Shader\Default.frag)", ShaderType::Fragment);
    auto fragmentShader = ShaderFactory::CreateShaderSource(R"(C:\_Dev\ClockWork\ClockWork\res\Shader\DefaultPBR.frag)", ShaderType::Fragment);
    auto defaultShader = ShaderFactory::CreateShader("shader", { vertexShader, fragmentShader });
    
    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0, 1.9f, 0);

    defaultShader.Use();
    defaultShader.setVec3("AlbedoColor", vec3(1)/*vec3(0.784)*/);
    defaultShader.setVec3("spotLight.Color", 150, 150, 150);
    defaultShader.SetFloat("spotLight.CutOff", cos(glm::radians(20.5f)));
    defaultShader.SetFloat("spotLight.OuterCutoff", cos(glm::radians(26.0f)));
    defaultShader.setVec3("pointLight.Position", lightPos);
    defaultShader.setVec3("pointLight.Color", vec3(150, 150, 150));
    defaultShader.setVec3("directLight.Direction", -0.2f, -1.0f, -0.3f);
    defaultShader.setVec3("directLight.Color", vec3(0.8f));
    defaultShader.SetFloat("directLight.Intensity", 1.0f);
    defaultShader.SetTexture("IrradianceMap", 4);

    defaultShader.setVec3("spotLight.Color", 150, 150, 150);
    defaultShader.SetFloat("spotLight.CutOff", cos(glm::radians(20.5f)));
    defaultShader.SetFloat("spotLight.OuterCutoff", cos(glm::radians(26.0f)));
    defaultShader.setVec3("spotLight.Position", camera.Position);
    defaultShader.setVec3("spotLight.Direction", camera.Forward);

    defaultShader.setVec3("pointLight.Position", vec3(0, 1.9f, 0));
    defaultShader.setVec3("pointLight.Color", vec3(150, 150, 150));
    defaultShader.setVec3("directLight.Direction", -0.2f, -1.0f, -0.3f);
    defaultShader.setVec3("directLight.Color", vec3(1.0f));
    defaultShader.SetFloat("directLight.Intensity", 1.0f);
    defaultShader.setVec3("directLight.direction", -0.2f, -1.0f, -0.3f);
    defaultShader.setVec3("directLight.ambient", vec3(0.1f));
    defaultShader.setVec3("directLight.diffuse", glm::vec3(0.3f));
    defaultShader.setVec3("directLight.specular", glm::vec3(0.2f));
    defaultShader.SetTexture("IrradianceMap", 4);
    defaultShader.SetTexture("PrefilterMap", 5);
    defaultShader.SetTexture("BrdfLut", 6);


    Skybox skybox;
    auto skyboxComponent= skybox.LoadHdr(R"(C:/Users/user/Desktop/daytime.hdr)");
    ecs.RegisterSingletonComponent<SkyboxComponent>(*skyboxComponent);

    std::vector<MeshComponent>& meshComponents = CharacterModel.MeshComponents;
    std::vector<unsigned int> CharacterAssets;

    for (auto& mesh : meshComponents)
    {
        mesh.Shader = defaultShader;
        auto asset = ecs.AddAsset(mesh);
        CharacterAssets.push_back(asset);
    }

    auto& aabb1 = meshComponents[0].AABB;
    auto& aabb2 = meshComponents[1].AABB;

    // make it square for testing collisions
    AABBComponent AABB1{ glm::vec3(aabb1.minX, aabb1.minY, aabb1.minX), glm::vec3(aabb1.maxX, aabb1.maxY, aabb1.maxX) };
    AABBComponent AABB2{ glm::vec3(aabb2.minX, aabb2.minY, aabb2.minX), glm::vec3(aabb2.maxX, aabb2.maxY, aabb2.maxX)};
  
    auto lights = std::vector<MeshComponent>{ makeLight() };
    auto lightId = ecs.AddAsset(lights[0]);

    RenderableComponent renderableCharacter{ CharacterAssets , false};
    RenderableComponent renderableLight{ std::vector<unsigned int>{lightId}, false };

    std::default_random_engine generator;
    std::uniform_real_distribution<float> randPosition(-25.5f, 25.5f);
    std::uniform_real_distribution<float> randRotation(0.0f, 359.0f);
    std::uniform_real_distribution<float> randAcceleration(-0.1f, 0.1);
    std::uniform_real_distribution<float> randVelocity(-0.1f, 0.1f);
     
    float scale = 1.00f;

    auto transform1 = TransformComponent
    {
        glm::vec3(0, 0, 0),
        glm::vec3(0, randRotation(generator), 0),
        glm::vec3(scale)
    };


    auto Character1 = scene.CreateEntity("Character1");
    //auto CharacterWeapon1 = scene.CreateEntity("CharacterWeapon");
    scene.AddComponents
    (
        Character1,
        transform1,
        Player{ 5.0f, 5.0f },
        RenderableComponent{ CharacterAssets /*std::vector<unsigned int>{CharacterAssets[0]}*/, false },
        AABB1,
        PhysicsComponent{ glm::vec3(0), glm::vec3(0) }
        //PhysicsComponent{glm::vec3(0), glm::vec3(randAcceleration(generator),0,randAcceleration(generator))}
    );

    //scene.AddComponents
    //(
    //    CharacterWeapon1,
    //    transform1,
    //    Player{ 5.0f, 5.0f },
    //    RenderableComponent{ std::vector<unsigned int>{CharacterAssets[1]},false },
    //    AABB2
    //    //PhysicsComponent{ glm::vec3(0), glm::vec3(randAcceleration(generator),0,randAcceleration(generator)) }
    //);

    auto light1 = scene.CreateEntity("light");


    std::vector<glm::mat4> transforms;
    for (unsigned int i = 0; i < 100; i++)
    {
        auto pos = glm::vec3(randPosition(generator) * 5, 3, randPosition(generator) * 5);
        auto transform = TransformComponent
        { 
            //pos,
            glm::vec3(randPosition(generator) , 3, randPosition(generator)),
            glm::vec3(0, randRotation(generator), 0),
            //glm::vec3(0, 0, 0),
            glm::vec3(scale) 
        };
        //auto transform = TransformComponent
        //{
        //    glm::vec3(0),
        //    glm::vec3(0, randRotation(generator), 0),
        //    glm::vec3(scale)
        //};

        transforms.emplace_back(transform.GetMatrix());

        auto Character = scene.CreateEntity("Character1");
        //auto CharacterWeapon = scene.CreateEntity("CharacterWeapon");
        scene.AddComponents 
        (
            Character, 
            transform, 
            Player{5.0f, 5.0f},
            RenderableComponent{ CharacterAssets, /*std::vector<unsigned int>{CharacterAssets[0]},*/ false },
            //PhysicsComponent{glm::vec3(0), glm::vec3(randAcceleration(generator),0,randAcceleration(generator))},
            PhysicsComponent{ glm::vec3(0), glm::vec3(0) },
            AABB1
        );
    }

    scene.AddComponents(light1, TransformComponent{ glm::vec3(0, 1.9f, 0), glm::vec3(0), glm::vec3(1) }, renderableLight/*, PhysicsComponent{ glm::vec3(0, 0, 0.1f), glm::vec3(0, 0, 1)}*/);

    app->Run(1920, 1080);

    delete app;
}

void ThreadPoolTest()
{
    CW::ThreadPool tp;

    int array[100];

    for (size_t i = 0; i < 10; i++)
    {
        array[i] = i;
    }

    for (unsigned int i = 0; i < 10; i++)
    {
        auto& val = array[i];
        tp.Push([&val, i](int) 
        {
            val+= 2;
        });

        tp.Push([&val, i](int)
        {
            val--;
        });

        tp.Push([&val, i](int)
        {
            std::cout << val << "\n";
        });
        tp.Wait();
    }

}

int main()
{
#if !CW_DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    {
        //ThreadSafeTest();
        Game();
    }
#if CW_DEBUG
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtDumpMemoryLeaks();
#endif
    return 0;
}