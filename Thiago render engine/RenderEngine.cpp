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

	validationLayers = new ValidationLayerModule();
	surface = new SurfaceModule();

	surface->InitializeWindow();
	instance = new InstanceModule();
	surface->InitializeSurface(&instance->instance);
	swapChain = new SwapChainModule(surface);
	physicalDevice = new PhysicalDeviceModule(swapChain, surface);
	logicalDevice = new LogicalDeviceModule(physicalDevice);
	swapChain->CreateSwapChain(physicalDevice, &logicalDevice->device);
	swapChain->CreateImageViews(&logicalDevice->device);
	pipeline = new PipelineModule(&logicalDevice->device);
}

Hub::~Hub() {
	delete pipeline;
	delete validationLayers;
	delete swapChain;
	delete logicalDevice;
	delete surface;
	delete physicalDevice;
	delete instance;
	Hub::s = nullptr;
}
