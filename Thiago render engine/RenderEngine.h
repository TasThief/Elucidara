#pragma once
#define NDEBUG
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <cstring>
#include <set>

#include "Deleter.h"
#include "VkDeleter.h"

#include "SurfaceModule.h"
#include "InstanceModule.h"
#include "ValidationLayerModule.h"
#include "PhysicalDeviceModule.h"
#include "LogicalDeviceModule.h"
#include "SwapChainModule.h"

using namespace std;

class ValidationLayerModule;
class SurfaceModule;
class InstanceModule;
class PhysicalDeviceModule;
class LogicalDeviceModule;
class SwapChainModule;

class Hub
{

public:
	Deleter<ValidationLayerModule> validationLayers;
	Deleter<InstanceModule> instance;
	Deleter<PhysicalDeviceModule> physicalDevice;
	Deleter<LogicalDeviceModule> logicalDevice;
	Deleter<SurfaceModule> surface;
	Deleter<SwapChainModule> swapChain;

	static Hub* Hub::s;
	VkInstance* GetInstance();
	GLFWwindow* GetWindow();
	VkSurfaceKHR* GetSurface();
    vector<const char*> GetValidationLayers();

	bool AreValidationLayersEnabled();
	bool IsWindowOpen();
	void ExecuteWindowEvents();

	Hub();
	~Hub();

};

