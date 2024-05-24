#pragma once
#include "Event.h"
#include <sstream>

namespace Engine {
	class WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(WindowClose)
	};
	class WindowFocusEvent : public Event {
	public:
		WindowFocusEvent() {}

		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(WindowFocus)
	};
	class WindowLostFocusEvent : public Event {
	public:
		WindowLostFocusEvent() {}

		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(WindowLostFocus)
	};
	class WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) { }

		virtual std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowResize: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		unsigned int getWidth() { return m_Width; }
		unsigned int getHeight() { return m_Height; }

		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(WindowResize)
	private:
		unsigned int m_Width;
		unsigned int m_Height;
	};
}