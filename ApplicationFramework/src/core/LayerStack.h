#pragma once
#include <vector>
#include "Layer.h"

namespace Engine {
	// A vector wraper for holding layers.
	class LayerStack
	{
	public:
		LayerStack();
		virtual ~LayerStack();

		// Adds layer to vector.
		void PushLayer(std::shared_ptr<Layer> layer);
		// Adds layer on top of vector.
		void PushOverlay(std::shared_ptr<Layer> layer);
		// Removes layer from vector.
		void PopLayer(std::shared_ptr<Layer> layer);
		// Removes layer from vector.
		void PopOverlay(std::shared_ptr<Layer> layer);

		// Returns the begining of vector.
		std::vector<std::shared_ptr<Layer>>::iterator begin() { return m_Layers.begin(); }
		// Returns the end of vector.
		std::vector<std::shared_ptr<Layer>>::iterator end() { return m_Layers.end(); }
	private:
		// The Layer vector.
		std::vector<std::shared_ptr<Layer>> m_Layers;
		// Used to insert overlays.
		int m_LayerInstertIndex;
	};
}