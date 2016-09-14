#pragma once
#include "RenderEngine.h"

class LogicalDeviceModule;
class PhysicalDeviceModule;

class ComandQueue
{
public:
	VkQueue handler;
	float priority = 0;
	uint32_t count = 0;
	uint32_t index = -1;

	VkDeviceQueueCreateInfo creationInfo;

	LogicalDeviceModule* logicalDeviceModuleRef = nullptr;

	void SetCreationInfo();
	void SetHandler();

	void InitializeHandler(LogicalDeviceModule * moduleRef, uint32_t queueCount, float priority, uint32_t index);

};

class LogicalDeviceModule
{
public:
	ComandQueue graphicsQueue;
	ComandQueue	 presentQueue;
	ComandQueue	 computeQueue;
	ComandQueue  memoryqueue;

	PhysicalDeviceModule* physicalDeviceModuleRef = nullptr;

	VkDeleter<VkDevice> device;

	LogicalDeviceModule(PhysicalDeviceModule* physicalDevice);
	~LogicalDeviceModule();
	void InitializeLogicalDevice(PhysicalDeviceModule* physicalDevice);

};


