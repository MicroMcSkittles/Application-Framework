#pragma once
#include <string>
#include "Event/Event.h"

namespace Engine {
	class Layer
	{
	public:
		// Layer constructor.
		Layer(std::string debug_name);
		// Layer deconstructor.
		virtual ~Layer();

		// Called when attached to application.
		virtual void onAttach() { };
		// Called when detached to application.
		virtual void onDetach() { };
		// Called once per frame.
		virtual void onUpdate(float delta_time) { };
		// Called when application renders.
		virtual void onRender() { };
		// Called when application processes a event.
		virtual void onEvent(Event& e) { };
		// Called once per frame on ImGui render if ImGui is enabled.
		virtual void onImGuiRender() { };

		// Returns the dedug name.
		std::string getDebugName() {
			return debug_name;
		}
	private:
		// A name for debuging.
		std::string debug_name;
	};
}