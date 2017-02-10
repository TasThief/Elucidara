#include "stdafx.h"
#include "ThreadPool.h"



int main()
{

	RenderingEngine engine;
	ThreadPool tp(4);
	engine.initialize();


	// Poll for user input.
	bool stillRunning = true;
	while (stillRunning) {

		SDL_Event event;
		while (SDL_PollEvent(&event)) {

			switch (event.type) {

			case SDL_QUIT:
				stillRunning = false;
				break;

			default:
				// Do nothing.
				break;
			}
		}

		SDL_Delay(10);
	}

	engine.destroy();
	return 0;
}