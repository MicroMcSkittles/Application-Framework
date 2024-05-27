#include "CameraController.h"

#include "Camera.h"
//#include <imGUI/imgui.h>

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "core/Input.h"


namespace Engine::Renderer {
	Engine::Renderer::CameraController::CameraController(std::shared_ptr<Camera> camera, float speed, float sensitivity)
		:m_Camera(camera), m_Speed(speed), m_Sensitivity(sensitivity) { }

	void Engine::Renderer::CameraController::OnUpdate(float delta_time)
	{
		double mouseX = Input::GetMouseX(), mouseY = Input::GetMouseY();
		glm::vec2 delta = (glm::vec2(mouseX, mouseY) - m_LastMousePos) * 0.002f;
		m_LastMousePos = glm::vec2(mouseX, mouseY);
		
		if (!Input::IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
			Input::HideCursor(false);
			return;
		}

		Input::HideCursor(true);

		bool moved = false;
		constexpr glm::vec3 upDirection(0.0f, 1.0f, 0.0f);
		glm::vec3 direction = m_Camera->getDirection();
		glm::vec3 position = m_Camera->getPosition();

		glm::vec3 rightDirection = glm::cross(glm::vec3(direction.x, 0, direction.z), upDirection);
	
		if (Input::IsKeyPressed(KEY_W)) {
			position += delta_time * m_Speed * direction;
			moved = true;
		}
		if (Input::IsKeyPressed(KEY_S)) {
			position -= delta_time * m_Speed * direction;
			moved = true;
		}
		if (Input::IsKeyPressed(KEY_D)) {
			position += delta_time * m_Speed * rightDirection;
			moved = true;
		}
		if (Input::IsKeyPressed(KEY_A)) {
			position -= delta_time * m_Speed * rightDirection;
			moved = true;
		}
		if (Input::IsKeyPressed(KEY_E)) {
			position += delta_time * m_Speed * upDirection;
			moved = true;
		}
		if (Input::IsKeyPressed(KEY_Q)) {
			position -= delta_time * m_Speed * upDirection;
			moved = true;
		}

		if (delta.x != 0 || delta.y != 0) {
			float pitchDelta = delta.y * m_Sensitivity;
			float yawDelta = delta.x * m_Sensitivity;

			glm::quat q = glm::normalize(glm::cross(glm::angleAxis(-pitchDelta, rightDirection), glm::angleAxis(-yawDelta, glm::vec3(0.0f, 1.0f, 0.0f))));
			direction = glm::rotate(q, direction);

			moved = true;
		}

		if (moved) {
			m_Camera->setDirection(direction);
			m_Camera->setPosition(position);

			m_Camera->recalculateProjection();
			m_Camera->recalculateView();
		}
	}

	void CameraController::OnImGuiRender()
	{
		/*ImGui::Begin("Camera Controller");
	
		ImGui::InputFloat("Speed", &m_Speed);
		ImGui::InputFloat("Sensitivity", &m_Sensitivity);

		ImGui::End();*/
	}
};