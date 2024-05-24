#include "WindowsSystem.h"
#include <GLFW/glfw3.h>

namespace Engine {
	System* System::s_Instance = new WindowsSystem();

	float WindowsSystem::GetTimeImpl()
	{
		return glfwGetTime();
	}
}