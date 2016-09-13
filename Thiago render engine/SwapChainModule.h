#pragma once
#include "RenderEngine.h"

class Hub;
class PhysicalDeviceModule;
class SurfaceModule;

class SwapChainModule
{
public:
	VkSurfaceCapabilitiesKHR capabilities;
	vector<VkSurfaceFormatKHR> formats;
	vector<VkPresentModeKHR> presentModes;

	PhysicalDeviceModule* physicalDeviceModuleRef = nullptr;
	SurfaceModule* surfaceModuleRef = nullptr;

	void InitializeSwapChain(VkPhysicalDevice* device, SurfaceModule * surface);
	bool IsAdequate();
	void QueryForSwapChainSupport(VkPhysicalDevice device);
	SwapChainModule();
	~SwapChainModule();
};

