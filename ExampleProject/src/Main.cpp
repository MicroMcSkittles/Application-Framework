#include <Engine.h>
#include <core/EntryPoint.h>

#include <iostream>

class MainLayer : public Engine::Layer {
public:
	MainLayer() : Layer("Main Layer") { }
	~MainLayer() { }

	virtual void onEvent(Engine::Event& e) override { 
		std::cout << e.ToString() << std::endl;
	}
};

class App : public Engine::Application {
public:
	App(int argc, char** argv) : Application(argc, argv) {
		pushLayer(new MainLayer());
	}
	~App() { }
};

Engine::Application* Engine::CreateApplication(int argc, char** argv) {
	return new App(argc, argv);
}
