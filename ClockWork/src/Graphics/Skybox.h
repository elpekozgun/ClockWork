#pragma once

#include "Core/Core.h"
#include "Shader.h"
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
        std::unique_ptr<SkyboxComponent> Load(std::vector<std::string>& faces, Shader& shader);
    };
}