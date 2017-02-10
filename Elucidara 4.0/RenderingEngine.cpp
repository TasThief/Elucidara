#include "stdafx.h"

void RenderingEngine::initialize()
{
	RenderingEngineBuilder builder(this);
	builder.build();
}

void RenderingEngine::debug(string message)
{
	coutMutex.lock();
	cout << message;
	coutMutex.unlock();
}

RenderingEngine::RenderingEngine()
{
	
}

void RenderingEngine::destruction_routine(string name, vector<flag*> waitFor, flag* owner, function<void()> routine)
{
	ThreadPool::add_command([this, name, waitFor, owner, routine]() {
		for each (flag* image in waitFor)
			image->sleep();
		debug("[STARTED]" + name + "\n");
		routine();
		owner->go();
		debug("[FINSHED]" + name + "\n");
	});
}

void RenderingEngine::destroy()
{
	debug("\n[DESTRUCTION STARTED]\n");
	map<string,flag> waitList;

	destruction_routine("Image views", 
	  { }, 
		&waitList["IMAGE_VIEW"], [this]() {
		for each (ImageView image in *imageView)
			device->destroyImageView(image);
		delete imageView;
	});

	destruction_routine("Swapchain", 
	  {/* &waitList["IMAGE"], &waitList["IMAGE_VIEW"] */},
		&waitList["SWAPCHAIN"], [this]() {
		device->destroySwapchainKHR(*swapchain);
		delete swapchain;
	});

	destruction_routine("Logical device", 
	  { &waitList["SWAPCHAIN"], &waitList["IMAGE_VIEW"] },
		&waitList["DEVICE"], [this]() {
		device->destroy();
		delete device;
	});

	destruction_routine("Surface", 
	  { &waitList["SWAPCHAIN"] },
		&waitList["SURFACE"], [this]() {
		instance->destroySurfaceKHR(*surface);
		delete surface;
	});


	for (map<string, flag>::iterator i = waitList.begin(); i != waitList.end(); i++)
	{
		debug("[WAITING]" + i->first + "\n");
		i->second.sleep();
	}

	
	delete validationLayers;
	delete queueMap;
	//delete imageView;
	
	delete images;
	debug("Extra memory cleaned\n");

	SDL_DestroyWindow(window);
	SDL_Quit();

	debug("Window closed\n");

	instance->destroy();
	debug("Instance destroyed\n");

	delete instance;
	debug("[DESTRUCTION CONPLETE]\n");
}
