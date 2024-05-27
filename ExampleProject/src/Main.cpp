#include <Engine.h>
#include <core/EntryPoint.h>

#include <Renderer/CameraController.h>

#include <iostream>

using namespace Engine;

class MainLayer : public Layer {
public:
	MainLayer() : Layer("Main Layer") { }
	~MainLayer() { }

	virtual void onAttach() override { 

	}

	virtual void onUpdate(float delta_time) override { 
	
	}

	virtual void onRender() override { 
		
	}

	virtual void onImGuiRender() override { 
	
	}

private:

};

class App : public Application {
public:
	App(int argc, char** argv) : Application(argc, argv) {
		pushLayer(new MainLayer());
	}
	~App() { }
};

Engine::Application* Engine::CreateApplication(int argc, char** argv) {
	return new App(argc, argv);
}
