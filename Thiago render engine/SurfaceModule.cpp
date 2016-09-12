#include "SurfaceModule.h"

SurfaceModule::SurfaceModule(){
}
	
void SurfaceModule::InitializeWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	 cout << "Window created" <<  endl;
}

SurfaceModule::~SurfaceModule(){
	glfwDestroyWindow(window);
}

bool SurfaceModule::IsOpen(){
	return !glfwWindowShouldClose(window);
}

void SurfaceModule::ExecuteEvents(){
	glfwPollEvents();
}

void SurfaceModule::InitializeSurface()
{
/*	if (glfwCreateWindowSurface(Hub::s->instance->instance, window, nullptr, &surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}*/

//	surface
	cout << "Surface created" << endl;
}
