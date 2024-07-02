#include "LayerStack.h"

namespace Engine {
	LayerStack::LayerStack()
	{
		m_LayerInstertIndex = 0;
	}
	LayerStack::~LayerStack()
	{
		for (std::shared_ptr<Layer> layer : m_Layers) {
			layer.reset();
		}
	}

	void LayerStack::PushLayer(std::shared_ptr<Layer> layer)
	{
		m_Layers.emplace(begin() + m_LayerInstertIndex, layer);
		m_LayerInstertIndex++;
	}
	void LayerStack::PushOverlay(std::shared_ptr<Layer> layer)
	{
		m_Layers.emplace_back(layer);
	}
	void LayerStack::PopLayer(std::shared_ptr<Layer> layer)
	{
		auto it = std::find(begin(), end(), layer);
		if (it != m_Layers.end()) {
			m_Layers.erase(it);
			m_LayerInstertIndex--;
		}
	}
	void LayerStack::PopOverlay(std::shared_ptr<Layer> layer)
	{
		auto it = std::find(begin(), end(), layer);
		if (it != m_Layers.end()) {
			m_Layers.erase(it);
		}
	}
}