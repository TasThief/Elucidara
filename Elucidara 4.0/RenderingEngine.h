#pragma once
#include "RenderingEngineBuilder.h"

class RenderingEngine
{
public:

	vector<const char*>* validationLayers = nullptr;
	Instance*			 instance = nullptr;
	SDL_Window*			 window = nullptr;
	SurfaceKHR*			 surface = nullptr;
	Device*				 device = nullptr;
	Queue*				 gQueue = nullptr;
	CommandPool*		 commandPool = nullptr;
//	PhysicalDeviceMap*   physicalDevice = nullptr;

	void initialize();

	RenderingEngine();
	~RenderingEngine();
};

