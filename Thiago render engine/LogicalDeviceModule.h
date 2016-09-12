#pragma once
#include "RenderEngine.h"
class Hub;

class ComandQueue
{
public:
	VkQueue handler;
	float priority = 0;
	uint32_t count = 0;
	uint32_t index = -1;

	VkDeviceQueueCreateInfo creationInfo;

	void SetCreationInfo();
	void SetHandler();
};

class LogicalDeviceModule
{
public:
	ComandQueue graphicsQueue;
	ComandQueue	 presentQueue;
	ComandQueue	 computeQueue;
	ComandQueue  memoryqueue;

	void SetComandQueues();
	VkDeleter<VkDevice> device{ vkDestroyDevice };
	LogicalDeviceModule();
	~LogicalDeviceModule();
	void InitializeLogicalDevice();

};


