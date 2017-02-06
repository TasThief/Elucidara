#include "stdafx.h"
#include "ThreadPool.h"



int main()
{
	ThreadPool tp(3);

	RenderingEngine engine;
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
	return 0;
}