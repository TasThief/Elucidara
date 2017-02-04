#pragma once
#include "stdafx.h"
#include "DeviceSelector.h"

class Elucidara
{
public:
	vector<const char*> validationLayers;

	vk::Instance instance;

	SDL_Window* window;

	vk::SurfaceKHR surface;

	vk::Device device;

	vk::Queue gQueue;

	vk::CommandPool commandPool;

	void init_validationLayers();
	
	void init_instance();

	void init_window();

	void init_surface();

	void init_device();


	Elucidara();

	virtual ~Elucidara();
};
