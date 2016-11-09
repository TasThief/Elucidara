#pragma once
#define NDEBUG
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <cstring>
#include <set>
#include <algorithm>
#include <fstream>

#include "Deleter.h"
#include "VkDeleter.h"

#include "SurfaceModule.h"
#include "InstanceModule.h"
#include "ValidationLayerModule.h"
#include "PhysicalDeviceModule.h"
#include "LogicalDeviceModule.h"
#include "SwapChainModule.h"
#include "PipelineModule.h"

using namespace std;

class ValidationLayerModule;
class SurfaceModule;
class InstanceModule;
class PhysicalDeviceModule;
class LogicalDeviceModule;
class SwapChainModule;
class PipelineModule;

class Hub
{

public:
	ValidationLayerModule* validationLayers;
	SwapChainModule* swapChain;
	LogicalDeviceModule* logicalDevice;
	SurfaceModule* surface;
	PhysicalDeviceModule* physicalDevice;
	InstanceModule* instance;
	PipelineModule* pipeline;

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

