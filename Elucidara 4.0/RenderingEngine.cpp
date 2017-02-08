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
	cout << "destroying device" << endl;
	device->destroy();
	delete device;

	cout << "destroying surface" << endl;
	instance->destroySurfaceKHR(*surface);
	delete surface;

	cout << "destroying window" << endl;
	SDL_DestroyWindow(window);
	SDL_Quit();

	cout << "destroying surface" << endl;
	instance->destroy();
	delete instance;

	cout << "deleting validation layers" << endl;
	delete validationLayers;

	cout << "deleting queue handler map" << endl;
	delete queueMap;
}
