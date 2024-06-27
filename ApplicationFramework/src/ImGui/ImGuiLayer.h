#pragma once
#include "core/Layer.h"

namespace Engine {
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();

		virtual void onAttach() override;
		virtual void onDetach() override;

		void Begin();
		void End();

		void AddWindowFlag(int flag) { m_WindowFlags |= flag; }
		void RemoveWindowFlag(int flag) { m_WindowFlags &= ~flag; }
		void EnableDocking(bool enableDocking) { m_EnableDocking = enableDocking; }

	private:
		int m_WindowFlags;
		bool m_EnableDocking;
	};
}