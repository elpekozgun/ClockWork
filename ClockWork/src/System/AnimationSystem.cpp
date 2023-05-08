#include "AnimationSystem.h"


namespace CW
{
	AnimationSystem::AnimationSystem()
	{
		//Model CharacterModel(R"(C:/Users/user/Desktop/Idle/Idle.dae)");
	}
	
	void AnimationSystem::Update(float dt)
	{
		SwitchState();
		_animator->SetAnimation(activeAnimName);

		if (Play)
		{
			_animator->UpdateAnimation(dt);
		}
		
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

					//if(mesh.Vertices.size() < 1000)
					//	return;

					mesh.Shader.Use();
					auto& transforms = _animator->GetFinalBoneMatrices();
					mesh.Shader.setMat4Array("BoneMats", transforms.data(), 100);

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

	void AnimationSystem::SwitchState()
	{
		auto& input = Input::Instance();

		//activeAnimName = "Idle";
		if (input.GetKeyPressed(CW::KEY_0))
		{
			Idle = !Idle;
		}
		else if (input.GetKeyDown(CW::KEY_W))
		{
			activeAnimName = "run";
		}
		else if (input.GetKeyDown(CW::KEY_S))
		{
			activeAnimName = "run_back";
		}
		else if (input.GetKeyDown(CW::KEY_Q))
		{
			activeAnimName = "strafe_left";
		}
		else if (input.GetKeyDown(CW::KEY_E))
		{
			activeAnimName = "strafe_right";
		}
		else if (input.GetKeyDown(CW::KEY_X))
		{
			activeAnimName = "forward_flip";
		}
		else
			activeAnimName = "idle";
	}


}
