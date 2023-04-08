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
#include "Game/Systems/PlayerController.h"
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
    VAO vao;

    MeshComponent groundMesh;

    groundMesh.Vertices = vertices;
    groundMesh.Indices = indices;
    groundMesh.Textures = textures;
    groundMesh.Setup();

    auto vertexShader = Shader::CreateShaderSource(R"(C:\_Dev\ClockWork\ClockWork\res\Shader\Default.vert)", ShaderType::Vertex);
    auto fragmentShader = Shader::CreateShaderSource(R"(C:\_Dev\ClockWork\ClockWork\res\Shader\Default.frag)", ShaderType::Fragment);
    groundMesh.Shader = Shader::CreateShader("shader", { vertexShader, fragmentShader });
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

    auto lightVertex = Shader::CreateShaderSource(R"(C:\_Dev\ClockWork\ClockWork\res\Shader\LightVertex.glsl)", ShaderType::Vertex);
    auto lightFragment = Shader::CreateShaderSource(R"(C:\_Dev\ClockWork\ClockWork\res\Shader\LightFragment.glsl)", ShaderType::Fragment);
    lightMesh.Shader = Shader::CreateShader("shader", { lightVertex, lightFragment });

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
        AddSystem<RenderSystem, RenderableComponent, TransformComponent>();

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
    camera.speed = 5.0f;
    camera.sensitivity = 0.1f;
    camera.Near = 0.5f;
    camera.Far = 500.0f;

    ecs.RegisterSingletonComponent<CameraComponent>(camera);


    Scene scene(ecs);
    //Model mariaModel(R"(C:/Users/user/Desktop/Vanguard/Vanguard By T. Choonyung.dae)");
    //Model mariaModel(R"(C:/Users/user/Desktop/damaged_helmet/scene.gltf)");
    //Model mariaModel(R"(C:/Users/user/Desktop/pbrsphere/scene.gltf)");
    //Model mariaModel(R"(C:/Users/user/Desktop/sphere/source/sphere_sampleScene_1cm.fbx)");
    //Model mariaModel(R"(C:/Users/user/Desktop/pbrspherefbx/source/sphere_sampleScene_1cm.fbx)");
    //Model mariaModel(R"(C:/Users/user/Desktop/robot/scene.gltf)");
    //Model mariaModel(R"(C:/Users/user/Desktop/doom_eternal_slayer/scene.gltf)");
    //Model mariaModel(R"(C:/Users/user/Desktop/medieval_vagrant_knights/scene.gltf)");
    //Model mariaModel(R"(C:/Users/user/Desktop/skull_downloadable/scene.gltf)");
    //Model mariaModel(R"(C:/Users/user/Desktop/golden_knight/scene.gltf)");
    //Model mariaModel(R"(C:/Users/user/Desktop/maria/scene.gltf)");
    //Model mariaModel(R"(C:/_Dev/ClockWork/ClockWork/res/3DModel/gladiator.glb)");
    Model mariaModel(R"(C:/_Dev/ClockWork/ClockWork/res/3DModel/maria/Maria WProp J J Ong.dae)");
    //Model mariaModel(R"(C:/_Dev/ClockWork/ClockWork/res/3DModel/cat/cat.obj)");

    auto vertexShader = Shader::CreateShaderSource(R"(C:\_Dev\ClockWork\ClockWork\res\Shader\Default.vert)", ShaderType::Vertex);
    //auto fragmentShader = Shader::CreateShaderSource(R"(C:\_Dev\ClockWork\ClockWork\res\Shader\Default.frag)", ShaderType::Fragment);
    auto fragmentShader = Shader::CreateShaderSource(R"(C:\_Dev\ClockWork\ClockWork\res\Shader\DefaultPBR.frag)", ShaderType::Fragment);
    auto defaultShader = Shader::CreateShader("shader", { vertexShader, fragmentShader });
    
    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0, 1.9f, 0);

    defaultShader.Use();
    defaultShader.setVec3("spotLight.Color", 150, 150, 150);
    defaultShader.SetFloat("spotLight.CutOff", cos(glm::radians(20.5f)));
    defaultShader.SetFloat("spotLight.OuterCutoff", cos(glm::radians(26.0f)));
    defaultShader.setVec3("pointLight.Position", lightPos);
    defaultShader.setVec3("pointLight.Color", vec3(150, 150, 150));
    defaultShader.setVec3("directLight.Direction", -0.2f, -1.0f, -0.3f);
    defaultShader.setVec3("directLight.Color", vec3(0.8f));
    defaultShader.SetFloat("directLight.Intensity", 1.0f);
    defaultShader.SetTexture("IrradianceMap", 4);

    /*std::vector<std::string> faces
    {
        R"(C:\_Dev\ClockWork\ClockWork\res\skybox\right.jpg)",
        R"(C:\_Dev\ClockWork\ClockWork\res\skybox\left.jpg)",
        R"(C:\_Dev\ClockWork\ClockWork\res\skybox\top.jpg)",
        R"(C:\_Dev\ClockWork\ClockWork\res\skybox\bottom.jpg)",
        R"(C:\_Dev\ClockWork\ClockWork\res\skybox\front.jpg)",
        R"(C:\_Dev\ClockWork\ClockWork\res\skybox\back.jpg)"
    };*/

    Skybox skybox;
    //auto skyboxComponent = skybox.Load(faces, skyboxShader);
    //auto skyboxComponent = skybox.LoadHdr(R"(C:/Users/user/Desktop/HDR_029_Sky_Cloudy_Free/HDR_029_Sky_Cloudy_Free/HDR_029_Sky_Cloudy_Ref.hdr)", skyboxShader);
    auto skyboxComponent= skybox.LoadHdr(R"(C:/Users/user/Desktop/daytime.hdr)");
    ecs.RegisterSingletonComponent<SkyboxComponent>(*skyboxComponent);



    //Model sponzaModel(R"(C:/Users/user/Desktop/sponza/scene.gltf)");


    std::vector<MeshComponent>& meshComponents = mariaModel.MeshComponents;
    std::vector<unsigned int> mariaAssets;
    for (auto& mesh : meshComponents)
    {
        mesh.Shader = defaultShader;
        auto asset = ecs.AddAsset(mesh);
        mariaAssets.push_back(asset);
    }

   /* std::vector<MeshComponent>& sponzaMeshComponents = sponzaModel.MeshComponents;
    std::vector<unsigned int> sponzaAssets;
    for (auto& mesh : sponzaMeshComponents)
    {
        mesh.Shader = defaultShader;
        auto asset = ecs.AddAsset(mesh);
        sponzaAssets.push_back(asset);
    }*/


    auto lights = std::vector<MeshComponent>{ makeLight() };
    auto lightId = ecs.AddAsset(lights[0]);


    RenderableComponent renderableMaria{ mariaAssets , false};
    RenderableComponent renderableLight{ std::vector<unsigned int>{lightId}, false };
    //RenderableComponent renderableSponza { sponzaAssets, false };

    auto light1 = scene.CreateEntity("light");
    auto sponza = scene.CreateEntity("sponza");

    std::default_random_engine generator;
    std::uniform_real_distribution<float> randPosition(-40.0f, 40.0f);
    std::uniform_real_distribution<float> randRotation(0.0f, 359.0f);
    std::uniform_real_distribution<float> randAcceleration(-0.1f, 0.1);
    std::uniform_real_distribution<float> randVelocity(-0.1f, 0.1f);

    float scale = 0.01f;

    //std::vector<glm::mat4> transforms;
    //for (unsigned int i = 0; i < 1000 ; i++)
    //{
    //    auto transform = TransformComponent
    //    { 
    //        glm::vec3(randPosition(generator) / scale, 0, randPosition(generator) / scale),
    //        glm::vec3(0, randRotation(generator), 0), 
    //        glm::vec3(scale) 
    //    };

    //    auto maria = scene.CreateEntity("maria1");
    //    scene.AddComponents 
    //    (
    //        maria, 
    //        transform, 
    //        Player{5.0f, 5.0f},
    //        renderableMaria
    //        //PhysicsComponent{glm::vec3(0), glm::vec3(randAcceleration(generator),0,randAcceleration(generator))}
    //    );
    //}


    auto model = scene.CreateEntity("maria1");
    scene.AddComponents
    (
        model,
        TransformComponent{ glm::vec3(0), glm::vec3(0)/*glm::vec3(3.141592f / 2.0f,0,0)*/, glm::vec3(0.01f)},
        Player{ 5.0f, 5.0f },
        renderableMaria
        //PhysicsComponent{glm::vec3(0), glm::vec3(randAcceleration(generator),0,randAcceleration(generator))}
    );

    scene.AddComponents(light1, TransformComponent{ glm::vec3(0, 1.9f, 0), glm::vec3(0), glm::vec3(1) }, renderableLight/*, PhysicsComponent{ glm::vec3(0, 0, 0.1f), glm::vec3(0, 0, 1)}*/);
    //scene.AddComponents(sponza, TransformComponent(glm::vec3(0), glm::vec3(0), glm::vec3(0.01f)), renderableSponza);
    

    //auto renderSystem = std::dynamic_pointer_cast<RenderSystem>(ecs.GetSystem<RenderSystem>());
    //auto ents = renderSystem->_entities;
    //for (auto& ent : ents)
    //{
    //    auto a = ecs.GetComponent<TransformComponent>(ent);
    //    auto b = ecs.GetComponent<RenderableComponent>(ent);
    //    renderSystem->RenderTuples.push_back({ a,b });
    //}


    //instanced rendering performance still slow..
    //for (auto& meshId : renderableMaria.MeshIds)
    //{
    //    ecs.MakeMeshInstanced(meshId, transforms);
    //}


    app->Run(1920, 1080);

    delete app;
}



int main()
{
#if CW_DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    {
        Game();
    }
#if CW_DEBUG
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtDumpMemoryLeaks();
#endif
    return 0;
}