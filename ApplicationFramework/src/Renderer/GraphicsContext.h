#pragma once

namespace Engine::Renderer {
	class GraphicsContext {
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};
}