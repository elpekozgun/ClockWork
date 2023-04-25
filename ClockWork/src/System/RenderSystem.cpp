#include "RenderSystem.h"
#include <Core/Input.h>


namespace CW
{

	

	void CW::RenderSystem::Update(float dt)
	{
		//drawCall = 0;
		SwitchState();

		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		//glEnable(GL_MULTISAMPLE);

		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT /*| GL_STENCIL_BUFFER_BIT*/);

		std::map<unsigned int, std::vector<glm::mat4>> instanceTranslations;
		std::vector< std::map<unsigned int, std::vector<glm::mat4>>> totalTranslations;
		int tricount = 0;

		auto camera = _ecs->GetSingletonComponent<CameraComponent>();
		auto skybox = _ecs->GetSingletonComponent<SkyboxComponent>();
		auto cameraMatrix = camera->CameraMatrix();

		auto renderables = _ecs->GetComponentArray<RenderableComponent>();
		auto transforms = _ecs->GetComponentArray<TransformComponent>();


		for (auto& entity : _entities)
		{
			auto& renderable = renderables->GetData(entity);
			auto& transform = transforms->GetData(entity);

			//auto& renderable = _ecs->GetComponent<RenderableComponent>(entity);
			//auto& transform = _ecs->GetComponent<TransformComponent>(entity);

			auto transformMatrix = MatrixFromTransform(transform);

			auto clipMatrix = cameraMatrix * transformMatrix;

			for (auto& meshId : renderable.MeshIds)
			{
				if (instanced && renderable.Instanced)
				{
					auto& mesh = _ecs->GetAsset<MeshComponent>(meshId);

					if (frustum)
					{
						glm::vec4 clipPos = camera->CameraMatrix() * transformMatrix * glm::vec4(mesh.Vertices[0].Position, 1);

						if (std::abs(clipPos.x) <= clipPos.w * 0.9f &&
							std::abs(clipPos.y) <= clipPos.w * 0.9f &&
							std::abs(clipPos.z) <= clipPos.w)
						{
							instanceTranslations[meshId].push_back(transformMatrix);
						}
					}
					else
					{
						instanceTranslations[meshId].push_back(transformMatrix);
					}

					if (pagedInstanced)
					{
						tricount += mesh.Indices.size() / 3;
						if (tricount >= MaxTri)
						{
							totalTranslations.push_back(instanceTranslations);
							instanceTranslations.clear();
							tricount = 0;
						}
					}
				}
				else
				{
					if (CachedMeshes.find(meshId) == CachedMeshes.end())
					{
						auto& mesh = _ecs->GetAsset<MeshComponent>(meshId);
						CachedMeshes[meshId] = mesh;

						if (frustum)
						{
							unsigned int depth = 0;
							if (IsInFrustum(camera->Position, clipMatrix, mesh.AABB, depth))
							{
								Render(mesh, transform, *camera);
							}
						}
						else
						{
							Render(mesh, transform, *camera);
						}
					}
					else
					{
						auto& mesh = CachedMeshes[meshId];
						if (frustum)
						{
							unsigned int depth = 0;
							if (IsInFrustum(camera->Position, clipMatrix, mesh.AABB, depth))
							{
								Render(mesh, transform, *camera);
							}
						}
						else
						{
							Render(mesh, transform, *camera);
						}
					}
				}
			}
		}

		if (instanced)
		{
			if (pagedInstanced)
			{
				for (auto& translations : totalTranslations)
				{
					RenderInstanced(translations, *camera);
				}
			}
			else
			{
				RenderInstanced(instanceTranslations, *camera);
			}
		}

		if (drawSkybox)
		{
			glDepthFunc(GL_LEQUAL);
			skybox->Shader.Use();

			// both matrices same but wtf???
			//auto what = glm::mat4(glm::mat3(camera->CameraMatrix()));
			glm::mat4 modifiedCamMat = glm::mat4(glm::mat3(camera->View()));
			modifiedCamMat = camera->Projection() * modifiedCamMat;
			skybox->Shader.setMat4("CamMat", modifiedCamMat);
			skybox->Shader.SetTexture("SkyBox", 0);

			// skybox cube
			//skybox->Vao.Bind();
			glBindVertexArray(skybox->Vao);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->TextureId);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindVertexArray(0);
			glDepthFunc(GL_LESS);
		}

		cap += dt;
		//if (cap >= 1.0f)
		//{
		//	std::cout << drawCall << "\n";
		//	cap = 0;
		//}
	}

	void CW::RenderSystem::UpdateGetComponent(float dt)
	{
		drawCall = 0;
		SwitchState();

		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		//glEnable(GL_MULTISAMPLE);

		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT /*| GL_STENCIL_BUFFER_BIT*/);

		std::map<unsigned int, std::vector<glm::mat4>> instanceTranslations;
		std::vector< std::map<unsigned int, std::vector<glm::mat4>>> totalTranslations;
		int tricount = 0;

		auto camera = _ecs->GetSingletonComponent<CameraComponent>();
		auto skybox = _ecs->GetSingletonComponent<SkyboxComponent>();

		for (auto& entity : _entities)
		{
			auto& renderable = _ecs->GetComponent<RenderableComponent>(entity);
			auto& transform = _ecs->GetComponent<TransformComponent>(entity);

			auto transformMatrix = MatrixFromTransform(transform);

			for (auto& meshId : renderable.MeshIds)
			{
				if (instanced && renderable.Instanced)
				{
					auto& mesh = _ecs->GetAsset<MeshComponent>(meshId);

					if (frustum)
					{
						glm::vec4 clipPos = camera->CameraMatrix() * transformMatrix * glm::vec4(mesh.Vertices[0].Position, 1);

						if (std::abs(clipPos.x) <= clipPos.w * 0.9f &&
							std::abs(clipPos.y) <= clipPos.w * 0.9f &&
							std::abs(clipPos.z) <= clipPos.w)
						{
							instanceTranslations[meshId].push_back(transformMatrix);
						}
					}
					else
					{
						instanceTranslations[meshId].push_back(transformMatrix);
					}

					if (pagedInstanced)
					{
						tricount += mesh.Indices.size() / 3;
						if (tricount >= MaxTri)
						{
							totalTranslations.push_back(instanceTranslations);
							instanceTranslations.clear();
							tricount = 0;
						}
					}
				}
				else
				{
					auto& mesh = _ecs->GetAsset<MeshComponent>(meshId);

					if (frustum)
					{
						unsigned int depth = 0;
						glm::mat4 mat = camera->CameraMatrix() * transformMatrix;
						if (IsInFrustum(camera->Position, mat, mesh.AABB, depth))
						{
							Render(mesh, transform, *camera);
						}
					}
					else
					{
						Render(mesh, transform, *camera);
					}
				}
			}
		}

		if (instanced)
		{
			if (pagedInstanced)
			{
				for (auto& translations : totalTranslations) 
				{
					RenderInstanced(translations, *camera);
				}
			}
			else
			{
				RenderInstanced(instanceTranslations, *camera);
			}
		}

		if (drawSkybox)
		{
			glDepthFunc(GL_LEQUAL);
			skybox->Shader.Use();

			// both matrices same but wtf???
			//auto what = glm::mat4(glm::mat3(camera->CameraMatrix()));
			glm::mat4 modifiedCamMat = glm::mat4(glm::mat3(camera->View()));
			modifiedCamMat = camera->Projection() * modifiedCamMat;
			skybox->Shader.setMat4("CamMat", modifiedCamMat);
			skybox->Shader.SetTexture("SkyBox", 0);

			// skybox cube
			//skybox->Vao.Bind();
			glBindVertexArray(skybox->Vao);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->TextureId);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glDepthFunc(GL_LESS);
		}

		cap += dt;
		if (cap >= 1.0f)
		{
			std::cout << drawCall << "\n";
			cap = 0;
		}
	}


	void RenderSystem::Render(MeshComponent& mesh, TransformComponent& transform, CameraComponent& camera)
	{
		auto skybox = _ecs->GetSingletonComponent<SkyboxComponent>();
		mesh.Shader.Use();

		glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		glm::mat4 model = MatrixFromTransform(transform);  //transform.GetMatrix();
		model = transform.GetMatrix();  //transform.GetMatrix();
		auto camMat = CameraMat(camera);
		camMat = camera.CameraMatrix();

		mesh.Shader.SetBool("instanced", false);


		// TODO: UPDATING THESE PROPERTIES EVERY FRAME IS SUPER COSTLY, NO NEED TO CHANGE UNLESS THERE IS A CHANGE IN STATE...
		mesh.Shader.setMat4("Model", model);
		mesh.Shader.setMat4("CamMat", camMat);
		mesh.Shader.setVec3("CamPosition", camera.Position);

		mesh.Shader.setVec3("spotLight.Position", camera.Position);
		mesh.Shader.setVec3("spotLight.Direction", camera.Forward);

		if (!directionalLight)
		{
			mesh.Shader.setVec3("directLight.Color", 0, 0, 0);
		}

		unsigned int diffuseNo = 0;
		unsigned int specularNo = 0;
		unsigned int normalNo = 0;
		unsigned int metallicNo = 0;
		unsigned int roughnessNo = 0;
		for (unsigned int i = 0; i < mesh.Textures.size(); i++)
		{
			int no = 0;
			std::string name = mesh.Textures[i].TextureType;
			if (name == "Diffuse")
				no = diffuseNo++;
			else if (name == "Specular")
				no = specularNo++;
			else if (name == "Normal")
				no = normalNo++;
			else if (name == "Metallic")
				no = metallicNo++;
			else if (name == "Roughness")
				no = roughnessNo++;

			std::string fullName = name + std::to_string(no);

			mesh.Shader.SetBool("HasNormalMap", normalNo != 0);
			//mesh.Shader.SetBool("hasNormalMap",false);

			//mesh.Shader.SetTexture(fullName, i);
			mesh.Textures[i].Bind();

		}
		//mesh.Shader.SetTexture("IrradianceMap", 4);
		//mesh.Shader.SetTexture("PrefilterMap", 5);
		//mesh.Shader.SetTexture("BrdfLut", 6);

		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->IrradianceMap);

		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->PrefilterMap);

		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, skybox->BrdfLutMap);

		glBindVertexArray(mesh.VaoId);
		glDrawElements(GL_TRIANGLES, mesh.Indices.size(), GL_UNSIGNED_INT, 0);


		//drawCall++;
	}

	void RenderSystem::RenderInstanced(std::map<unsigned int, std::vector<glm::mat4>>& transformMap, CameraComponent& camera)
	{
		for (auto& pair : transformMap)
		{
			auto meshId = pair.first;
			auto transforms = pair.second;

			auto& mesh = _ecs->GetAsset<MeshComponent>(meshId);

			mesh.Shader.Use();
			mesh.Shader.SetBool("instanced", true);

			glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			auto camMat = camera.CameraMatrix();

			mesh.Shader.setMat4("CamMat", camMat);
			mesh.Shader.setVec3("eyePosition", camera.Position);
			mesh.Shader.setVec3("spotlight.position", camera.Position);
			mesh.Shader.setVec3("spotlight.direction", camera.Forward);

			unsigned int diffuseNo = 0;
			unsigned int specularNo = 0;
			for (unsigned int i = 0; i < mesh.Textures.size(); i++)
			{
				int no = 0;
				std::string name = mesh.Textures[i].TextureType;
				if (name == "Diffuse")
					no = diffuseNo++;
				else if (name == "Specular")
					no = specularNo++;

				std::string fullName = name + std::to_string(no);

				mesh.Shader.SetTexture(fullName, i);

				mesh.Textures[i].Bind();
			}

			glBindVertexArray(mesh.VaoId);
			glBindBuffer(GL_ARRAY_BUFFER, mesh.instanceVbo);
			glBufferData(GL_ARRAY_BUFFER, transforms.size() * sizeof(glm::mat4), transforms.data(), GL_STATIC_DRAW);

			glDrawElementsInstanced(GL_TRIANGLES, mesh.Indices.size(), GL_UNSIGNED_INT, 0, transforms.size());
			drawCall++;
		}
	}

	glm::mat4 RenderSystem::MatrixFromTransform(TransformComponent& transform)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, transform.Position);
		glm::quat rotation = glm::quat(glm::vec3(transform.Rotation.x, transform.Rotation.y, transform.Rotation.z));
		glm::mat quatMatrix = glm::toMat4(rotation);
		model *= quatMatrix;
		model = glm::scale(model, transform.Scale);
		return model;
	}

	glm::mat4 RenderSystem::CameraMat(CameraComponent& camera)
	{
		glm::mat4 view = glm::mat4(1);
		glm::mat4 projection = glm::mat4(1);

		view = glm::lookAt(camera.Position, camera.Position + camera.Forward, camera.Up);
		projection = glm::perspective(glm::radians(camera.FoV), (float)camera.Width / camera.height, camera.Near, camera.Far);

		auto right = glm::normalize(glm::cross(camera.Forward, camera.Up));

		return projection * view;
	}

	void RenderSystem::SwitchState()
	{
		auto& input = Input::Instance();
		if (input.GetKeyPressed(CW::KEY_1))
		{
			frustum = !frustum;
			std::cout << "frustum ";
			if (frustum)
				std::cout << " on\n";
			else
				std::cout << " off\n";
		}
		if (input.GetKeyPressed(CW::KEY_2))
		{
			instanced = !instanced;
			std::cout << "instanced ";
			if (instanced)
				std::cout << " on\n";
			else
				std::cout << " off\n";
		}
		if (input.GetKeyPressed(CW::KEY_3))
		{
			pagedInstanced = !pagedInstanced;
			std::cout << "pagedInstanced ";
			if (pagedInstanced)
				std::cout << " on\n";
			else
				std::cout << " off\n";
		}
		if (input.GetKeyPressed(CW::KEY_4))
		{
			drawSkybox = !drawSkybox;
			std::cout << "drawSkybox ";
			if (drawSkybox)
				std::cout << " on\n";
			else
				std::cout << " off\n";
		}
		if (input.GetKeyPressed(CW::KEY_5))
		{
			directionalLight = !directionalLight;
			std::cout << "directionalLight ";
			if (directionalLight)
				std::cout << " on\n";
			else
				std::cout << " off\n";
		}
		if (input.GetKeyPressed(CW::KEY_6))
		{
			normalScale += 0.05f;
			std::cout << "normal scale " << normalScale << "\n";
			for (auto& mesh : CachedMeshes)
			{
				mesh.second.Shader.Use();
				mesh.second.Shader.SetFloat("NormalStrength", std::clamp(normalScale, -2.0f, 2.0f));
			}
		}
		if (input.GetKeyPressed(CW::KEY_7))
		{
			normalScale -= 0.05f;
			std::cout << "normal scale " << normalScale << "\n";
			for (auto& mesh : CachedMeshes)
			{
				mesh.second.Shader.Use();
				mesh.second.Shader.SetFloat("NormalStrength", std::clamp(normalScale, -2.0f, 2.0f));
			}
		}
		if (input.GetKeyPressed(CW::KEY_8))
		{
			metalScale += 0.05f;
			std::cout << "metalness " << metalScale << "\n";
			for (auto& mesh : CachedMeshes)
			{
				mesh.second.Shader.Use();
				mesh.second.Shader.SetFloat("metallnessModifier", std::clamp(metalScale, -2.0f, 2.0f));
			}
		}
		if (input.GetKeyPressed(CW::KEY_9))
		{
			metalScale -= 0.05f;
			std::cout << "metalness " << metalScale << "\n";
			for (auto& mesh : CachedMeshes)
			{
				mesh.second.Shader.Use();
				mesh.second.Shader.SetFloat("metallnessModifier", std::clamp(metalScale, -2.0f, 2.0f));
			}
		}

		if (input.GetKeyPressed(CW::KEY_I))
		{
			smoothScale += 0.05f;
			std::cout << "smoothness " << smoothScale << "\n";
			for (auto& mesh : CachedMeshes)
			{
				mesh.second.Shader.Use();
				mesh.second.Shader.SetFloat("smoothnessModifier", std::clamp(smoothScale, -2.0f, 2.0f));
			}
		}
		if (input.GetKeyPressed(CW::KEY_O))
		{
			smoothScale -= 0.05f;
			std::cout << "smoothness " << smoothScale << "\n";
			for (auto& mesh : CachedMeshes)
			{
				mesh.second.Shader.Use();
				mesh.second.Shader.SetFloat("smoothnessModifier", std::clamp(smoothScale, -2.0f, 2.0f));
			}
		}

	}

	// lets make this recursively traverse as an octree.
	bool RenderSystem::IsInFrustum(glm::vec3& camPosition, glm::mat4& mvp, AABB& aabb, unsigned int& depth) 
	{
		//glm::vec3 min = glm::vec3(aabb.minX, aabb.minY, aabb.minZ);
		//glm::vec3 max = glm::vec3(aabb.maxX, aabb.maxY, aabb.maxZ);

		//min = glm::vec3(mvp * glm::vec4(min, 1));
		//max = glm::vec3(mvp * glm::vec4(max,1));

		//if (camPosition.x < max.x && camPosition.x > min.x && camPosition.y < max.y && camPosition.y > min.y && camPosition.z < max.z && camPosition.z > min.z)
		//{
		//	return true;
		//}

		//if (depth == maxOctreeDepth)
		//{
		//	return false;
		//}
		//depth++;

		std::array<glm::vec3, 8> aabbVertices = 
		{
			glm::vec3(aabb.minX, aabb.minY, aabb.minZ),
			glm::vec3(aabb.maxX, aabb.minY, aabb.minZ),
			glm::vec3(aabb.minX, aabb.maxY, aabb.minZ),
			glm::vec3(aabb.maxX, aabb.maxY, aabb.minZ),
			glm::vec3(aabb.minX, aabb.minY, aabb.maxZ),
			glm::vec3(aabb.maxX, aabb.minY, aabb.maxZ),
			glm::vec3(aabb.minX, aabb.maxY, aabb.maxZ),
			glm::vec3(aabb.maxX, aabb.maxY, aabb.maxZ)
		};

		for (unsigned int i = 0; i < 8; i++)
		{
			glm::vec4 clipPos = mvp * glm::vec4(aabbVertices[i], 1);
			if (std::abs(clipPos.x) > clipPos.w * 0.75f ||
				std::abs(clipPos.y) > clipPos.w * 0.75f ||
				std::abs(clipPos.z) > clipPos.w)
			{
				return false;
			}
		}

		/*float midx = (aabb.maxX + aabb.minX) * 0.5f;
		float midy = (aabb.maxY + aabb.minY) * 0.5f;
		float midz = (aabb.maxZ + aabb.minZ) * 0.5f;

		AABB leftTopFront{ midx, aabb.minX, aabb.maxY, midy, aabb.maxZ, midz};
		if (IsInFrustum(camPosition, mvp, leftTopFront, depth))
			return true;

		AABB leftBottomFront{ midx, aabb.minX, midy, aabb.minY, aabb.maxZ, midz };
		if (IsInFrustum(camPosition, mvp, leftBottomFront, depth))
			return true;

		AABB rightTopFront{ aabb.maxX, midx, aabb.maxY, midy , aabb.maxZ, midz };
		if (IsInFrustum(camPosition, mvp, rightTopFront, depth))
			return true;

		AABB rightBottomFront { aabb.maxX, midx, midy, aabb.minY, aabb.maxZ, midz};
		if (IsInFrustum(camPosition, mvp, rightBottomFront, depth))
			return true;

		AABB leftTopBack{ midx, aabb.minX, aabb.maxY, midy, midz, aabb.minZ };
		if (IsInFrustum(camPosition, mvp, leftTopBack, depth))
			return true;

		AABB leftBottomBack{ midx, aabb.minX, midy, aabb.minY, midz, aabb.minZ };
		if (IsInFrustum(camPosition, mvp, leftBottomBack, depth))
			return true;

		AABB rightTopBack{ aabb.maxX, midx, aabb.maxY, midy , midz, aabb.minZ };
		if(IsInFrustum(camPosition, mvp, rightTopBack, depth))
			return true;

		AABB rightBottomBack{ aabb.maxX, midx, midy, aabb.minY, midz, aabb.minZ };
		if (IsInFrustum(camPosition, mvp, rightBottomBack, depth))
			return true; */

		return true;
	}
}