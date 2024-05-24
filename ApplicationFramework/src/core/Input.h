#pragma once
#include "Core.h"
#include "KeyCodes.h"
#include "MouseButtonCodes.h"

namespace Engine {

	class Input {
	public:
		// Returns bool based on if key (represented by keycode) is pressed.
		inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }

		// Returns bool based on if mouse button (represented by mouseButtonCode) is pressed.
		inline static bool IsMouseButtonPressed(int mouseButtonCode) { return s_Instance->IsMouseButtonPressedImpl(mouseButtonCode); }
		// Returns the mouse position as std pair.
		inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		// Returns mouse X position.
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		// Returns mouse Y position.
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }

		// Shows curser if variable hide is false and hides cursor if hide is true.
		inline static void HideCursor(bool hide) { s_Instance->HideCursorImpl(hide); }

	protected:
		// implementations

		virtual bool IsKeyPressedImpl(int keycode) = 0;

		virtual bool IsMouseButtonPressedImpl(int keycode) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

		virtual void HideCursorImpl(bool hide) = 0;

	private:
		// Input instance.
		static Input* s_Instance;
	};

}