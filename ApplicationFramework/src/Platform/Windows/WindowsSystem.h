#pragma once
#include "core/System.h"

namespace Engine {
	class WindowsSystem : public System
	{
	private:
		virtual float GetTimeImpl() override;
	};
}