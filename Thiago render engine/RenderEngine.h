#pragma once
#define NDEBUG
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <cstring>
#include <set>
#include <algorithm>

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
	ValidationLayerModule* validationLayers;
	SwapChainModule* swapChain;
	LogicalDeviceModule* logicalDevice;
	SurfaceModule* surface;
	PhysicalDeviceModule* physicalDevice;
	InstanceModule* instance;

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

