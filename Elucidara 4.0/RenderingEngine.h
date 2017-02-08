#pragma once
#include "RenderingEngineBuilder.h"

class RenderingEngine
{
public:

	vector<const char*>*	 validationLayers = nullptr;
	Instance*				 instance = nullptr;
	SDL_Window*				 window = nullptr;
	SurfaceKHR*				 surface = nullptr;
	Device*					 device = nullptr;
	Queue*					 gQueue = nullptr;
	CommandPool*			 commandPool = nullptr;
	map<QFB, vector<Queue>>* queueMap = nullptr;
	SwapchainKHR*			 swapchain = nullptr;

	void initialize();

	RenderingEngine();
	~RenderingEngine();
};

