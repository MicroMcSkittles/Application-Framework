#pragma once

namespace Engine {
	class System {
	public:
		static float GetTime() { return s_Instance->GetTimeImpl(); }
	private:
		virtual float GetTimeImpl() = 0;

	private:
		static System* s_Instance;
	};
};