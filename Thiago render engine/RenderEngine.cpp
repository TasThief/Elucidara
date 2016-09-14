#include "RenderEngine.h"

Hub* Hub::s = nullptr;

VkInstance* Hub::GetInstance() {
	return 	&instance->instance;
}

GLFWwindow * Hub::GetWindow() {
	return surface->window;
}

VkSurfaceKHR * Hub::GetSurface() {
	return nullptr;
}

vector<const char*> Hub::GetValidationLayers(){
	return validationLayers->validationLayers;
}

bool Hub::AreValidationLayersEnabled(){
	return validationLayers->enableValidationLayers;
}

bool Hub::IsWindowOpen() {
	return surface->IsOpen();	
}

void Hub::ExecuteWindowEvents() {
	surface->ExecuteEvents();
}

Hub::Hub() {
	Hub::s = this;

	surface->InitializeWindow();
	instance->InitializeInstance();
	surface->InitializeSurface(&instance->instance);
	swapChain->InitializeSwapChain(&surface);
	physicalDevice->InitializePhysicalDevice(&swapChain,&surface);
	logicalDevice->InitializeLogicalDevice(&physicalDevice);
	swapChain->CreateSwapChain(&physicalDevice, &logicalDevice->device);
}

Hub::~Hub() {
	Hub::s = nullptr;
}