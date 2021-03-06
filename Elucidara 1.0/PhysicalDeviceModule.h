#pragma once
#include "RenderEngine.h"
class Hub;
class SwapChainModule;
class SurfaceModule;
class PhysicalDeviceModule
{
public:
	const vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	int graphicsFamilyIndex = -1;
	int  presentFamilyIndex = -1;
	int  computeFamilyIndex = -1;

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

	VkPhysicalDeviceProperties properties;

	VkPhysicalDeviceFeatures deviceFeatures;

	PhysicalDeviceModule(SwapChainModule* swapChain, SurfaceModule* surface);

	~PhysicalDeviceModule();

	bool IsDeviceSuitable(VkPhysicalDevice device, SwapChainModule* swapChain, SurfaceModule* surface);

	void ResetFamilyIndex();

	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

	vector<VkPhysicalDevice> GetDeviceList();

	void FindQueueFamilies(VkPhysicalDevice device);

	
};

