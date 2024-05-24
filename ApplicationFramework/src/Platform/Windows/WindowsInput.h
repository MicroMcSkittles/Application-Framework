#pragma once
#include "core/Input.h"

namespace Engine {
	class WindowsInput : public Input
	{
	protected:
		// Keys
		virtual bool IsKeyPressedImpl(int keycode) override;

		// Mouse
		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;

		virtual void HideCursorImpl(bool hide) override;
	};
}