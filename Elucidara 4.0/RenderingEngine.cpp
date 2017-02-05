#include "stdafx.h"

void RenderingEngine::initialize()
{
	RenderingEngineBuilder builder(this);
	builder.build();
}

RenderingEngine::RenderingEngine()
{
}


RenderingEngine::~RenderingEngine()
{
	cout << "destroying surface" << endl;
	instance->destroySurfaceKHR(*surface);

	cout << "destroying window" << endl;
	SDL_DestroyWindow(window);

	SDL_Quit();

	cout << "destroying surface" << endl;
	instance->destroy();


	cout << endl;

	cout << "deleting surface" << endl;
	delete surface;

	cout << "deleting instance" << endl;
	delete instance;

	cout << "deleting validation layers" << endl;
	delete validationLayers;
}
