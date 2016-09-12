#include "Engine.h"

Engine::Engine() {
	Hub::s = new Hub();
}

Engine::~Engine() {
	delete Hub::s;
}

void Engine::Run() {
	while (Hub::s->IsWindowOpen()) {
		Hub::s->ExecuteWindowEvents();
	}
}

