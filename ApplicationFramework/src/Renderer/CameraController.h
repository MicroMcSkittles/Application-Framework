#pragma once
#include "Camera.h"
#include "core/Event/Event.h"
#include "core/Event/MouseEvent.h"

namespace Engine::Renderer {
	class CameraController
	{
	public:
		static std::shared_ptr<CameraController> Create(std::shared_ptr<Camera> camera, 
			float speed = 5.0f, float sensitivity = 2.0f)
		{
			return std::shared_ptr<CameraController>(new CameraController(camera, speed, sensitivity));
		}

		void OnUpdate(float delta_time);
		void OnImGuiRender();

	private:
		CameraController(std::shared_ptr<Camera> camera, float speed, float sensitivity);

	private:
		std::shared_ptr<Camera> m_Camera;

		glm::vec2 m_LastMousePos = { 0.0f, 0.0f };
		float m_Speed = 5.0f;
		float m_Sensitivity = 2.f;
	};

	class CameraController2D {
	public:
		static std::shared_ptr<CameraController2D> Create(std::shared_ptr<Camera> camera, float speed)
		{
			return std::shared_ptr<CameraController2D>(new CameraController2D(camera, speed));
		}

		void OnUpdate(float delta_time);

	private:
		CameraController2D(std::shared_ptr<Camera> camera, float speed);

	private:
		std::shared_ptr<Camera> m_Camera;

		float m_Speed = 5.0f;
	};
};