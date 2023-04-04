#include "RenderSystem.h"
#include <Core/InputState.h>


namespace CW
{
	// this runs super slow???
	void CW::RenderSystem::Update(float dt)
	{
		drawCall = 0;
		SwitchState();

		glEnable(GL_DEPTH_TEST);

		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT /*| GL_STENCIL_BUFFER_BIT*/);

		std::map<unsigned int, std::vector<glm::mat4>> instanceTranslations;

		std::vector< std::map<unsigned int, std::vector<glm::mat4>>> totalTranslations;
		int tricount = 0;

		auto camera = _ecs->GetSingletonComponent<CameraComponent>();
		for (auto& entity : _entities)
		{
			
			auto& renderable = _ecs->GetComponent<RenderableComponent>(entity);
			auto& transform = _ecs->GetComponent<TransformComponent>(entity);

			for (auto& meshId : renderable.MeshIds)
			{
				//if (renderable.Instanced)
				if (instanced)
				{
					auto& mesh = _ecs->GetAsset<MeshComponent>(meshId);

					if (frustum)
					{
						glm::vec4 clipPos = camera->CameraMatrix() * transform.GetMatrix() * glm::vec4(mesh.Vertices[0].Position, 1);

						if (std::abs(clipPos.x) <= clipPos.w * 0.9f &&
							std::abs(clipPos.y) <= clipPos.w * 0.9f &&
							std::abs(clipPos.z) <= clipPos.w)
						{
							instanceTranslations[meshId].push_back(MatrixFromTransform(transform));
						}
					}
					else
					{
						instanceTranslations[meshId].push_back(MatrixFromTransform(transform));
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
						glm::vec4 clipPos = camera->CameraMatrix() * transform.GetMatrix() * glm::vec4(mesh.Vertices[0].Position, 1);

						if (std::abs(clipPos.x) <= clipPos.w * 0.9f &&
							std::abs(clipPos.y) <= clipPos.w * 0.9f &&
							std::abs(clipPos.z) <= clipPos.w)
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

		cap += dt;
		if (cap >= 1.0f)
		{
			std::cout << drawCall << "\n";
			cap = 0;
		}
	}

	void CW::RenderSystem::UpdateSlow(float dt)
	{
		drawCall = 0;
		SwitchState();

		glEnable(GL_DEPTH_TEST);
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		std::map<unsigned int, std::vector<glm::mat4>> instanceTranslations;
		
		std::vector< std::map<unsigned int, std::vector<glm::mat4>>> totalTranslations;
		int tricount = 0;

		auto camera = _ecs->GetSingletonComponent<CameraComponent>();
		auto cameraMat = camera->CameraMatrix();
		auto skybox = _ecs->GetSingletonComponent<SkyboxComponent>();

		std::unordered_map<int, MeshComponent> meshComponents;

		std::map<unsigned int, std::vector<glm::mat4>> instanceRenderables;
		for (auto& entity : _entities)
		{
			auto& transform = _ecs->GetComponent<TransformComponent>(entity);
			auto& renderable = _ecs->GetComponent<RenderableComponent>(entity);

			auto transformMat = MatrixFromTransform(transform);

			for (auto& meshId : renderable.MeshIds)
			{
				if (meshComponents.find(meshId) == meshComponents.end())
				{
					auto& mesh = _ecs->GetAsset<MeshComponent>(meshId);

					mesh.Shader.Use();
					mesh.Shader.SetBool("instanced", false);
					mesh.Shader.setVec3("spotlight.position", camera->Position);
					mesh.Shader.setVec3("spotlight.direction", camera->Forward);
					mesh.Shader.setMat4("CamMat", cameraMat);
					mesh.Shader.setVec3("eyePosition", camera->Position);

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

					meshComponents[meshId] = mesh;
				}
			}


			for (auto& meshId : renderable.MeshIds)
			{
				auto& mesh = meshComponents[meshId];
				if (frustum)
				{
					glm::vec4 clipPos = cameraMat * transformMat * glm::vec4(mesh.Vertices[0].Position, 1);

					if (std::abs(clipPos.x) <= clipPos.w * 0.9f &&
						std::abs(clipPos.y) <= clipPos.w * 0.9f &&
						std::abs(clipPos.z) <= clipPos.w)
					{
						if (instanced)
						{
							instanceTranslations[meshId].push_back(transformMat);
						}
						else
						{
							Render(mesh, transformMat);
						}
					}
				}
				else
				{
					if (instanced)
					{
						instanceTranslations[meshId].push_back(transformMat);
					}
					else
					{
						Render(mesh, transformMat);
					}
				}
			}
		}

		if (instanced)
		{
			for (auto& pair : meshComponents)
			{
				RenderInstanced(pair.second, instanceTranslations);
			}
		}
		
		//glDepthFunc(GL_LEQUAL);  
		//skybox->Shader.Use();

		//// both matrices same but wtf???
		////auto what = glm::mat4(glm::mat3(cameraMat));
		//glm::mat4 modifiedCamMat = glm::mat4(glm::mat3(camera->View()));
		//modifiedCamMat = camera->Projection() * modifiedCamMat;
		//skybox->Shader.setMat4("CamMat", modifiedCamMat);
		//
		//// skybox cube
		//skybox->Vao.Bind();
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->TextureId);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glDepthFunc(GL_LESS);


	
		cap += dt;
		if (cap >= 1.0f)
		{
			std::cout << drawCall << "\n";
			cap = 0;
		}
	}

	void CW::RenderSystem::Update2(float dt)
	{
		drawCall = 0;
		SwitchState();

		glEnable(GL_DEPTH_TEST /*| GL_STENCIL_TEST*/);
		//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);


		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT /*| GL_STENCIL_BUFFER_BIT*/);

		std::map<unsigned int, std::vector<glm::mat4>> instanceTranslations;
		//auto& ecs = ECS::Instance();

		std::vector< std::map<unsigned int, std::vector<glm::mat4>>> totalTranslations;
		int tricount = 0;

		auto camera = _ecs->GetSingletonComponent<CameraComponent>();
		for (auto& entity : _entities)
			//for (auto& tuple : RenderTuples)
		{
			//auto& renderable = tuple.second;
			//auto& transform = tuple.first;

			auto& renderable = _ecs->GetComponent<RenderableComponent>(entity);
			auto& transform = _ecs->GetComponent<TransformComponent>(entity);

			for (auto& meshId : renderable.MeshIds)
			{
				//if (renderable.Instanced)
				if (instanced)
				{
					auto& mesh = _ecs->GetAsset<MeshComponent>(meshId);

					if (frustum)
					{
						glm::vec4 clipPos = camera->CameraMatrix() * transform.GetMatrix() * glm::vec4(mesh.Vertices[0].Position, 1);

						if (std::abs(clipPos.x) <= clipPos.w * 0.9f &&
							std::abs(clipPos.y) <= clipPos.w * 0.9f &&
							std::abs(clipPos.z) <= clipPos.w)
						{
							instanceTranslations[meshId].push_back(MatrixFromTransform(transform));
						}
					}
					else
					{
						instanceTranslations[meshId].push_back(MatrixFromTransform(transform));
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
						glm::vec4 clipPos = camera->CameraMatrix() * transform.GetMatrix() * glm::vec4(mesh.Vertices[0].Position, 1);

						if (std::abs(clipPos.x) <= clipPos.w * 0.9f &&
							std::abs(clipPos.y) <= clipPos.w * 0.9f &&
							std::abs(clipPos.z) <= clipPos.w)
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

		cap += dt;
		if (cap >= 1.0f)
		{
			std::cout << drawCall << "\n";
			cap = 0;
		}
	}

	void RenderSystem::Render(MeshComponent& mesh,glm::mat4& modelMat)
	{
		mesh.Shader.Use();
		mesh.Shader.setMat4("Model", modelMat);


		mesh.Vao.Bind();
		glDrawElements(GL_TRIANGLES, mesh.Indices.size(), GL_UNSIGNED_INT, 0);

		drawCall++;
	}

	void RenderSystem::Render(MeshComponent& mesh, TransformComponent& transform, CameraComponent& camera)
	{
		//glStencilFunc(GL_ALWAYS, 1, 0xFF);
		//glStencilMask(0xFF);

		mesh.Shader.Use();

		glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		glm::mat4 model = MatrixFromTransform(transform);  //transform.GetMatrix();
		auto camMat = CameraMat(camera);

		mesh.Shader.SetBool("instanced", false);

		mesh.Shader.setVec3("spotlight.position", camera.Position);
		mesh.Shader.setVec3("spotlight.direction", camera.Forward);
		mesh.Shader.setMat4("Model", model);
		mesh.Shader.setMat4("CamMat", camMat);
		mesh.Shader.setVec3("eyePosition", camera.Position);

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

		mesh.Vao.Bind();
		glDrawElements(GL_TRIANGLES, mesh.Indices.size(), GL_UNSIGNED_INT, 0);

		//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		//glStencilMask(0x00);
		//glDisable(GL_DEPTH_TEST);

		//// use outline shader here.

		//glStencilMask(0xFF);
		//glStencilFunc(GL_ALWAYS, 0, 0xFF);
		//glEnable(GL_DEPTH_TEST);

		drawCall++;
	}


	// on mesh with 46k tris it doesnt increase the performance in release mode, 
	// however on 30k elements of quads it boosts 3x speed. (10fps to 30fps)
	// maybe we can do a paged instancing instead of pushing all at once for complex geometry??
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

			mesh.Vao.Bind();
			glBindBuffer(GL_ARRAY_BUFFER, mesh.instanceVbo);
			glBufferData(GL_ARRAY_BUFFER, transforms.size() * sizeof(glm::mat4), transforms.data(), GL_STATIC_DRAW);

			glDrawElementsInstanced(GL_TRIANGLES, mesh.Indices.size(), GL_UNSIGNED_INT, 0, transforms.size());
			drawCall++;
		}
	}

	void RenderSystem::RenderInstanced(MeshComponent& mesh, std::map<unsigned int, std::vector<glm::mat4>>& transformMap)
	{
		for (auto& pair : transformMap)
		{
			auto& transforms = pair.second;

			mesh.Shader.Use();
			mesh.Shader.SetBool("instanced", true);

			mesh.Vao.Bind();
			glBindBuffer(GL_ARRAY_BUFFER, mesh.instanceVbo);
			glBufferData(GL_ARRAY_BUFFER, transforms.size() * sizeof(glm::mat4), transforms.data(), GL_STATIC_DRAW);

			glDrawElementsInstanced(GL_TRIANGLES, mesh.Indices.size(), GL_UNSIGNED_INT, 0, transforms.size());
			drawCall++;
		}
	}


	glm::mat4 RenderSystem::MatrixFromTransform(TransformComponent& transform	)
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
		auto& input = InputState::Instance();
		if (input.IsKeyDown(CW::KEY_1))
		{
			std::cout << "instanced on \n";
			instanced = true;
		}
		if (input.IsKeyDown(CW::KEY_2))
		{
			std::cout << "instanced off \n";
			instanced = false;
		}
		if (input.IsKeyDown(CW::KEY_3))
		{
			std::cout << "paged on \n";
			pagedInstanced = false;
		}
		if (input.IsKeyDown(CW::KEY_4))
		{
			std::cout << "paged off \n";
			pagedInstanced = false;
		}
		if (input.IsKeyDown(CW::KEY_5))
		{
			std::cout << "frustum on \n";
			frustum = true;
		}
		if (input.IsKeyDown(CW::KEY_6))
		{
			std::cout << "frustum off \n";
			frustum = false;
		}
	}
}