#include "LayerStack.h"

namespace Engine {
	LayerStack::LayerStack()
	{
		m_LayerInstertIndex = 0;
	}
	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers) {
			delete layer;
		}
	}

	void LayerStack::pushLayer(Layer * layer)
	{
		m_Layers.emplace(begin() + m_LayerInstertIndex, layer);
		m_LayerInstertIndex++;
	}
	void LayerStack::pushOverlay(Layer * layer)
	{
		m_Layers.emplace_back(layer);
	}
	void LayerStack::popLayer(Layer * layer)
	{
		auto it = std::find(begin(), end(), layer);
		if (it != m_Layers.end()) {
			m_Layers.erase(it);
			m_LayerInstertIndex--;
		}
	}
	void LayerStack::popOverlay(Layer * layer)
	{
		auto it = std::find(begin(), end(), layer);
		if (it != m_Layers.end()) {
			m_Layers.erase(it);
		}
	}
}