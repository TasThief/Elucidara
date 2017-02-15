#pragma once
#include "RenderingEngineBuilder.h"

class RenderingEngine
{
public:
	mutex coutMutex;
	vector<const char*>*	 validationLayers = nullptr;
	Instance*				 instance = nullptr;
	SDL_Window*				 window = nullptr;
	SurfaceKHR*				 surface = nullptr;
	Device*					 device = nullptr;
	Queue*					 gQueue = nullptr;
	CommandPool*			 commandPool = nullptr;
	map<QFB, vector<Queue>>* queueMap = nullptr;
	SwapchainKHR*			 swapchain = nullptr;
	vector<ImageView>*		 imageView = nullptr;
	vector<Image>*			 images = nullptr;
	Image*					 depthBuffer = nullptr;

	void initialize();
	void debug(string message);
	RenderingEngine();
	void destruction_routine(string name, vector<flag*> waitFor, flag * owner, function<void()> routine);
	void destroy();
};

