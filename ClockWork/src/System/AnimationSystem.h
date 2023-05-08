#pragma once

#include "ECS/ISystem.h"
#include "ECS/ECS.h"
#include "Graphics/Model.h"
#include "core/Input.h"

namespace CW
{
	class CW_API AnimationSystem : public ISystem
	{
	public:
		AnimationSystem();
		void Update(float deltatime) override;

		Animator* _animator;

		std::unordered_map<unsigned int, MeshComponent> CachedMeshes;

		~AnimationSystem();

	private:
		bool Idle = true;
		bool Play = true;

		void SwitchState();
	};

}