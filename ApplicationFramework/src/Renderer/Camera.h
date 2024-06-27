#pragma once
#include <glm/glm.hpp>
#include "core/Core.h"

namespace Engine::Renderer {
	class Camera {
	public:
		glm::mat4& getViewProjection() { return m_ViewProjection; }
		glm::mat4& getProjection() { return m_Projection; }
		glm::mat4& getView() { return m_View; }
		glm::mat4& getInvProjection() { return m_InvProjection; }
		glm::mat4& getInvView() { return m_InvView; }

		glm::vec3& getPosition() { return m_Position; }
		glm::vec3& getDirection() { return m_Direction; }

		void setPosition(const glm::vec3& pos) { m_Position = pos; recalculateView(); }
		void setDirection(const glm::vec3& dir) { m_Direction = dir; recalculateView(); }

		virtual void onResize(int width, int height) = 0;
		virtual void recalculateProjection() = 0;
		virtual void recalculateView() = 0;
		virtual void displayImGui() = 0;

	protected:
		glm::vec3 m_Position{ 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Direction{ 0.0f, 0.0f, 0.0f };

		glm::mat4 m_Projection{ 1.0f };
		glm::mat4 m_InvProjection{ 1.0f };

		glm::mat4 m_View{ 1.0f };
		glm::mat4 m_InvView{ 1.0f };

		glm::mat4 m_ViewProjection{ 1.0f };
	};

	class OrthographicCamera : public Camera {
	public:
		static std::shared_ptr<OrthographicCamera> Create(float width, float height, float near, float far) {
			return std::shared_ptr<OrthographicCamera>(new OrthographicCamera(width, height, near, far));
		}

		virtual void onResize(int width, int height) override;
		virtual void recalculateProjection() override;
		virtual void recalculateView() override;
		virtual void displayImGui() override;
	private:
		OrthographicCamera(float width, float height, float near, float far);

	private:
		float m_Width;
		float m_Height;
		float m_AspectRatio;
		float m_ZoomLevel;
		float m_NearPlane;
		float m_FarPlane;
	};

	class PerspectiveCamera : public Camera {
	public:
		static std::shared_ptr<PerspectiveCamera> Create(float width, float height, float fov, float near_plane, float far_plane) {
			return std::shared_ptr<PerspectiveCamera>(new PerspectiveCamera(width, height, fov, near_plane, far_plane));
		}

		virtual void onResize(int width, int height) override;
		virtual void recalculateProjection() override;
		virtual void recalculateView() override;
		virtual void displayImGui() override;
	private:
		PerspectiveCamera(float width, float height, float fov, float near_plane, float far_plane);
	
	private:
		float m_Width;
		float m_Height;
		float m_Fov;
		float m_NearPlane;
		float m_FarPlane;
	};
}