#pragma once

#include "Core/Core.h"
#include "ShaderFactory.h"
#include "VAO.h"
#include "EBO.h"
#include "ECS/Component.h"

#include <vector>
#include <stb_image.h>

namespace CW
{
    class CW_API Skybox
    {
    public:
        Skybox();
        std::unique_ptr<SkyboxComponent> Load(std::vector<std::string>& faces);
        std::unique_ptr<SkyboxComponent> LoadHdr(const char* path);
        void RenderCube();
        void SetupCube(std::vector<float>& vertices);

        void RenderQuad();
        void SetupQuad();

        unsigned int cubeVAO = 0;
        unsigned int cubeVBO = 0;

        unsigned int quadVBO = 0;
        unsigned int quadVAO = 0;
    };
}