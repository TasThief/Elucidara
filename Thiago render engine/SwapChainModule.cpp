#include "SwapChainModule.h"


void SwapChainModule::InitializeSwapChain(VkPhysicalDevice* device, SurfaceModule * surface)
{
	//physicalDeviceModuleRef = physicalDeviceModule;
	surfaceModuleRef = surface;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(*device, surfaceModuleRef->surface, &capabilities);


	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(*device, surfaceModuleRef->surface, &formatCount, nullptr);

	if (formatCount != 0)
	{
		formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(*device, surfaceModuleRef->surface, &formatCount, formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(*device, surfaceModuleRef->surface, &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(*device, surfaceModuleRef->surface, &presentModeCount, presentModes.data());
	}
}

bool SwapChainModule::IsAdequate()
{
	return !formats.empty() && !presentModes.empty();
}

void SwapChainModule::QueryForSwapChainSupport(VkPhysicalDevice device)
{
}

SwapChainModule::SwapChainModule()
{
}


SwapChainModule::~SwapChainModule()
{
}
