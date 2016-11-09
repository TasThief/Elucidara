#pragma once

// Enable the WSI extensions
#if defined(__ANDROID__)
#define VK_USE_PLATFORM_ANDROID_KHR
#elif defined(__linux__)
#define VK_USE_PLATFORM_XLIB_KHR
#elif defined(_WIN32)
#define VK_USE_PLATFORM_WIN32_KHR
#endif

// Tell SDL not to mess with main()
#define SDL_MAIN_HANDLED

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <vulkan/vulkan.hpp>

#include <iostream>
#include <vector>

class FamilyQueue
{
	/*
	uint32_t index;
	vk::CommandPool commandPool;
	std::vector<vk::CommandBuffer> allocCBuffer;
	//vk::DeviceQueueCreateInfo creationInfo;

	vk::DeviceQueueCreateInfo get_DeviceQueueCreateInfo()
	{
		vk::DeviceQueueCreateInfo dicq = vk::DeviceQueueCreateInfo()
	}

	void AllocateCommandBuffer(uint32_t count,vk::CommandBufferLevel level)
	{
		queueGraphic1 = device.getQueue(deviceQueueCreateInfoList[0].queueFamilyIndex, 0);

		commandPoolGraphic = device.createCommandPool(vk::CommandPoolCreateInfo().setQueueFamilyIndex(deviceQueueCreateInfoList[0].queueFamilyIndex));
		commandPoolCompute = device.createCommandPool(vk::CommandPoolCreateInfo().setQueueFamilyIndex(deviceQueueCreateInfoList[1].queueFamilyIndex));

		//this object allocates stuff into the pool
		std::vector<vk::CommandBuffer> allocCBufferGraphic = device.allocateCommandBuffers(vk::CommandBufferAllocateInfo()
			.setCommandBufferCount(count)
			.setCommandPool(commandPool)
			.setLevel(level);
	}
	*/
};

class RenderingEngine
{
private:

	SDL_Window* window;

	std::vector<const char*> layers;

	std::vector<const char*> extensions;

	vk::Instance instance;

	vk::SurfaceKHR surface;

	vk::Device device;

	vk::PhysicalDevice physicalDevice;

	vk::CommandPool
		commandPoolGraphic,
		commandPoolCompute;

	vk::Queue 
		queueGraphic1,
		queueCompute1;



	void init_ValidationLayers();

	void init_Instance();

	void init_Window();

	void init_PhysicalDevice();

	void init_Device();

	void init_SwapChain();

	void init_Surface();



	bool isDeviceSuitable(vk::PhysicalDevice * device);


	vk::DeviceQueueCreateInfo get_FamilyQueues(vk::PhysicalDevice& pDevice, const uint32_t queuesCount, vk::QueueFlagBits family);



	void get_AvailableWSIExtensions();

public:

	void initialize();

	void finalize();

	void update();



	RenderingEngine();

	virtual ~RenderingEngine();
};
