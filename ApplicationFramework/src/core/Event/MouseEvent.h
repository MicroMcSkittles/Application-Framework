#pragma once
#include "Event.h"
#include <sstream>

namespace Engine {
	class MouseButtonEvent : public Event {
	public:
		inline int getKeyCode() { return m_ButtonCode; }

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse | EventCategoryMouseButton)
	protected:
		MouseButtonEvent(int buttoncode)
			: m_ButtonCode(buttoncode) { }

		int m_ButtonCode;
	};
	class MouseButtonPressedEvent : public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(int buttoncode)
			: MouseButtonEvent(buttoncode) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonPressed: " << m_ButtonCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};
	class MouseButtonReleasedEvent : public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(int buttoncode)
			: MouseButtonEvent(buttoncode) {}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonReleased: " << m_ButtonCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

	class MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(unsigned int x, unsigned int y)
			: m_X(x), m_Y(y) {}

		unsigned int getX() { return m_X; }
		unsigned int getY() { return m_Y; }

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseMoved: " << m_X << ", " << m_Y;
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
		EVENT_CLASS_TYPE(MouseMoved)
	private:
		unsigned int m_X;
		unsigned int m_Y;
	};

	class MouseScolledEvent : public Event {
	public:
		MouseScolledEvent(float x, float y)
			: m_X(x), m_Y(y) {}

		unsigned int getX() { return m_X; }
		unsigned int getY() { return m_Y; }

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseScolled: " << m_X << ", " << m_Y;
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
		EVENT_CLASS_TYPE(MouseScolled)
	private:
		float m_X;
		float m_Y;
	};
}