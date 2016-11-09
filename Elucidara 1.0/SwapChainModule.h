#pragma once
#include "RenderEngine.h"

class Hub;
class PhysicalDeviceModule;
class LogicalDeviceModule;
class SurfaceModule;

class SwapChainModule
{
public:

	VkDeleter<VkSwapchainKHR> swapChain;
	vector<VkImage> swapChainImages;
	vector<VkDeleter<VkImageView>> swapChainImageViews;

	VkSurfaceCapabilitiesKHR capabilities;
	vector<VkSurfaceFormatKHR> formats;
	vector<VkPresentModeKHR> presentModes;

	VkSurfaceFormatKHR format;
	VkPresentModeKHR presentMode;
	VkExtent2D extent;

	PhysicalDeviceModule* physicalDeviceModuleRef = nullptr;
	SurfaceModule* surfaceModuleRef = nullptr;

	void CreateImageViews(VkDevice* device);
	void CreateSwapChain(PhysicalDeviceModule* physicalDeviceModuleRef, VkDevice* logicalDeviceModule);
	bool IsAdequate();
	void QueryForSwapChainSupport(VkPhysicalDevice* device);
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat();
	VkPresentModeKHR ChooseSwapPresentMode();
	VkExtent2D ChooseSwapExtent();
	SwapChainModule(SurfaceModule * surface);
	~SwapChainModule();
};

