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
		void pushLayer(Layer* layer);
		// Adds layer on top of vector.
		void pushOverlay(Layer* layer);
		// Removes layer from vector.
		void popLayer(Layer* layer);
		// Removes layer from vector.
		void popOverlay(Layer* layer);

		// Returns the begining of vector.
		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		// Returns the end of vector.
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	private:
		// The Layer vector.
		std::vector<Layer*> m_Layers;
		// Used to insert overlays.
		int m_LayerInstertIndex;
	};
}