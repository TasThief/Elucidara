#pragma once
#include "RenderEngine.h"
class Hub;

class SurfaceModule
{
public:
	const int WIDTH = 800;
	const int HEIGHT = 600;

	GLFWwindow* window;

	SurfaceModule();
	~SurfaceModule();

	bool IsOpen();
	void ExecuteEvents();
	void InitializeWindow();
	void InitializeSurface();

	VkDeleter<VkSurfaceKHR> surface; // { Hub::s->instance->instance, vkDestroySurfaceKHR };

};

