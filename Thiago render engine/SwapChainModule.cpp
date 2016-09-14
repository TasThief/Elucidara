#include "SwapChainModule.h"

void SwapChainModule::CreateImageViews(VkDevice* device)
{
	swapChainImageViews.resize(swapChainImages.size());
	for (uint32_t i = 0; i < swapChainImages.size(); i++) {
		swapChainImageViews[i].New(*device, vkDestroyImageView);
		
		VkImageViewCreateInfo createInfo = {};
		//type of struct
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	//	createInfo.pNext;
	//	createInfo.flags;
		//image i am creating
		createInfo.image = swapChainImages[i];
		//type of texture 1d 2d 3d or cubemaps
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;

		createInfo.format = format.format;

		//how the color channels are working
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		//image information, depth, mipmaps....
		//rgb color
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.baseMipLevel = 1;
		createInfo.subresourceRange.layerCount = 0;
		createInfo.subresourceRange.levelCount = 1;

		//create the image view
		if (vkCreateImageView(*device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS)
			throw runtime_error("failed to create image views");

		cout << i << " image view created" << endl;
	}
//	for (auto& imageView : swapChainImageViews)
//		imageView.New(*device, vkDestroyImageView);

}

void SwapChainModule::CreateSwapChain(PhysicalDeviceModule* physicalDeviceModuleRef, VkDevice* device)
{
	/*
	If i want to make elucidara's main purpose to happen here lies one of the major
	implementations.. i just discovered that i can allow alpha pixels to be blended
	by other surfaces (on composite alpha information) if i wanted to create a engine
	that mimics a canvas beeing painted i would need to create 2 swap chains, one for
	the canvas and other for the content, the canvas has alpha pixels and these pixels
	should be blended to the other swapchain, so probably i would need to have 2 
	swapchains that drop imagens on the same screen... it might be possible
	*/
	swapChain.New(*device, vkDestroySwapchainKHR);

	format = ChooseSwapSurfaceFormat();
	presentMode = ChooseSwapPresentMode();
	extent = ChooseSwapExtent();

	uint32_t imageCount = capabilities.minImageCount + 1;
	if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) {
		imageCount = capabilities.maxImageCount;
	}
	
	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
//	createInfo.pNext;
//	createInfo.flags;
	createInfo.surface = surfaceModuleRef->surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = format.format;
	createInfo.imageColorSpace = format.colorSpace;
	createInfo.imageExtent = extent;
	//2 for [VR] number of images 2 to create a stereo image set!!!
	createInfo.imageArrayLayers = 1;

	//if i want to use post processing stuff use this tag VK_IMAGE_USAGE_TRANSFER_DST_BIT
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	
	uint32_t queueFamilyIndices[] = { 
		(uint32_t)physicalDeviceModuleRef->graphicsFamilyIndex, 
		(uint32_t)physicalDeviceModuleRef->presentFamilyIndex };

	//An image is owned by one queue family at a time and ownership must be explicitly transfered before using it in another queue family. This option offers the best performance.
	createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	createInfo.queueFamilyIndexCount = 0;
	createInfo.pQueueFamilyIndices = nullptr;

	if (queueFamilyIndices[0] != queueFamilyIndices[1]) {
		//Images can be used across multiple queue families without explicit ownership transfers.
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	//if thre are some transformations in the images (like rotation), might be usefull to deal with screen rescale
	createInfo.preTransform = capabilities.currentTransform;

	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(*device, &createInfo, nullptr, swapChain.Reset()) != VK_SUCCESS)
		throw runtime_error("Failed to create swapchain");
	cout << "Swap Chain created" << endl;

	vkGetSwapchainImagesKHR(*device, swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(*device, swapChain, &imageCount, swapChainImages.data());
	cout << "Swap Chain Image handlers were created" << endl;
}

bool SwapChainModule::IsAdequate()
{
	return !formats.empty() && !presentModes.empty();
}

void SwapChainModule::QueryForSwapChainSupport(VkPhysicalDevice* device)
{
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

VkSurfaceFormatKHR SwapChainModule::ChooseSwapSurfaceFormat()
{
	//if its undefined, define it the way i want
	if (formats.size() == 1 && formats[0].format == VK_FORMAT_UNDEFINED)
		return{ VK_FORMAT_B10G11R11_UFLOAT_PACK32, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };

	//if its defined search from all of them the one i want
	for (const auto& format : formats)
		if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			return format;

	//if i could no find something i want go with anything available
	return formats[0];
}

VkPresentModeKHR SwapChainModule::ChooseSwapPresentMode()
{
	//if mailbox is available used
	for (const auto& presentMode : presentModes)
		if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			cout << "swap chain mode = Mailbox" << endl;
			return presentMode;
		}

	cout << "swap chain mode = Fifo" << endl;
	//if not use fifo
	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapChainModule::ChooseSwapExtent()
{
	//??
	if (capabilities.currentExtent.width != numeric_limits<uint32_t>::max())
		return capabilities.currentExtent;
	else {
		VkExtent2D actualExtent = { surfaceModuleRef->WIDTH, surfaceModuleRef->HEIGHT };

		//??
		actualExtent.width = max(capabilities.minImageExtent.width, min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = max(capabilities.minImageExtent.height, min(capabilities.maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}
}

SwapChainModule::SwapChainModule(SurfaceModule * surface) {
	surfaceModuleRef = surface;

}


SwapChainModule::~SwapChainModule()
{
}
