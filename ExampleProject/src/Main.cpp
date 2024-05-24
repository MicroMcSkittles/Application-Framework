#include <Engine.h>
#include <core/EntryPoint.h>

#include <iostream>

class MainLayer : public Engine::Layer {
public:
	MainLayer() : Layer("Main Layer") { }
	~MainLayer() { }

	virtual void onAttach() override {
		std::cout << "Attached" << std::endl;
	}

	virtual void onUpdate(float delta_time) override {
		std::cout << "Updated" << std::endl;
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
