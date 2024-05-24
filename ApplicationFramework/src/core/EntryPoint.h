#pragma once

extern Engine::Application* Engine::CreateApplication(int argc, char** argv);

int main(int argc, char** argv) {
	Engine::Application* app = Engine::CreateApplication(argc, argv);
	app->run();
	delete(app);
}
