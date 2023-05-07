#include "AnimationSystem.h"


namespace CW
{
	AnimationSystem::AnimationSystem()
	{
		//Model CharacterModel(R"(C:/Users/user/Desktop/Idle/Idle.dae)");
	}
	
	void AnimationSystem::Update(float dt)
	{
		//_animator->UpdateAnimation(dt);
		
		auto renderables = _ecs->GetComponentArray<RenderableComponent>();
		
		for (auto& entity : _entities)
		{
			auto& renderable = renderables->GetData(entity);

			for (auto& meshId : renderable.MeshIds )
			{
				//if (CachedMeshes.find(meshId) == CachedMeshes.end())
				{
					auto& mesh = _ecs->GetAsset<MeshComponent>(meshId);
					//CachedMeshes[meshId] = mesh;

					//mesh.Shader.Use();
					//auto transforms = _animator->GetFinalBoneMatrices();
					//for (unsigned int i = 0; i < transforms.size(); i++)
					//{
					//	mesh.Shader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
					//	//mesh.Shader.setMat4("finalBonesMatrices[32]", transforms[i]);
					//}

					//glBindVertexArray(mesh.VaoId);
					//glDrawElements(GL_TRIANGLES, mesh.Indices.size(), GL_UNSIGNED_INT, 0);
				}
				//else
				//{
				//	auto& mesh = CachedMeshes[meshId];

				//	mesh.Shader.Use();
				//	auto transforms = _animator->GetFinalBoneVertices();
				//	for (unsigned int i = 0; i < transforms.size(); i++)
				//	{
				//		std::string a = "finalBonesMatrices[" + std::to_string(i) + "]";
				//		mesh.Shader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
				//	}
				//}
			}
		}

	}

	AnimationSystem::~AnimationSystem()
	{
	}

}
