#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Engine::Renderer {
	

	OrthographicCamera::OrthographicCamera(float width, float height, float near, float far)
		:m_Width(width), m_Height(height), m_AspectRatio(width / height), m_NearPlane(near), m_FarPlane(far), m_ZoomLevel(1)
	{
		m_Projection = glm::ortho(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, m_NearPlane, m_FarPlane);
		recalculateView();
	}
	void OrthographicCamera::onResize(int width, int height)
	{
		m_Width = (float)width;
		m_Height = (float)height;
		m_AspectRatio = m_Width / m_Height;
		recalculateProjection();
	}
	void OrthographicCamera::recalculateProjection()
	{
		m_Projection = glm::ortho(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel, -1000.0f, 1000.0f);
		m_InvProjection = glm::inverse(m_Projection);
		m_ViewProjection = m_Projection * m_View;
	}
	void OrthographicCamera::recalculateView()
	{
		m_View = glm::lookAt(m_Position, m_Position + m_Direction, glm::vec3(0.0f, 1.0f, 0.0f));
		m_InvView = glm::inverse(m_View);
		m_ViewProjection = m_Projection * m_View;
	}
	void OrthographicCamera::displayImGui()
	{
		/*ImGui::Begin("Orthographic Camera");
		glm::vec3 position = m_Position, direction = m_Direction;
		float zoom = m_ZoomLevel;
		ImGui::InputFloat3("Position", &position.x);
		ImGui::InputFloat3("Direction", &direction.x);
		ImGui::InputFloat("Zoom Level", &zoom);

		if (zoom != m_ZoomLevel) {
			m_ZoomLevel = zoom;
			recalculateProjection();
		}
		if (position != m_Position || direction != m_Direction) {
			m_Position = position;
			m_Direction = direction;
			recalculateView();
		}

		ImGui::End();*/
	}

	PerspectiveCamera::PerspectiveCamera(float width, float height, float fov, float near_plane, float far_plane)
		:m_Width(width), m_Height(height), m_Fov(fov), m_NearPlane(near_plane), m_FarPlane(far_plane)
	{
		m_Position = glm::vec3(0, 0, 3);
		m_Direction = glm::vec3(0, 0, -1);

		m_Projection = glm::perspectiveFov(m_Fov, m_Width, m_Height, m_NearPlane, m_FarPlane);
		recalculateView();
	}
	void PerspectiveCamera::onResize(int width, int height)
	{
		m_Width = width;
		m_Height = height;
		recalculateProjection();
	}
	void PerspectiveCamera::recalculateProjection()
	{
		m_Projection = glm::perspectiveFov(glm::radians(m_Fov), m_Width, m_Height, m_NearPlane, m_FarPlane);
		m_InvProjection = glm::inverse(m_Projection);
		m_ViewProjection = m_Projection * m_View;
	}
	void PerspectiveCamera::recalculateView()
	{
		m_View = glm::lookAt(m_Position, m_Position + m_Direction, glm::vec3(0, 1, 0));
		m_InvView = glm::inverse(m_View);
		m_ViewProjection = m_Projection * m_View;
	}
	void PerspectiveCamera::displayImGui()
	{
		/*ImGui::Begin("Perspective Camera");

		glm::vec3 position = m_Position, direction = m_Direction;
		float fov = m_Fov, near = m_NearPlane, far = m_FarPlane;

		ImGui::InputFloat3("Position", &position.x);
		ImGui::InputFloat3("Direction", &direction.x);
		ImGui::InputFloat("Fov", &fov);
		ImGui::InputFloat("Near Plane", &near);
		ImGui::InputFloat("Far Plane", &far);

		if (fov != m_Fov || near != m_NearPlane || far != m_FarPlane) {
			m_Fov = fov;
			m_NearPlane = near;
			m_FarPlane = far;
			recalculateProjection();
		}
		if (position != m_Position || direction != m_Direction) {
			m_Position = position;
			m_Direction = direction;
			recalculateView();
		}

		ImGui::End();*/
	}
}