#pragma once
#define NDEBUG
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <cstring>

#include "Deleter.h"
#include "VkDeleter.h"

#include "SurfaceModule.h"
#include "InstanceModule.h"
#include "ValidationLayerModule.h"
#include "PhysicalDeviceModule.h"
#include "LogicalDeviceModule.h"

using namespace std;

class ValidationLayerModule;
class SurfaceModule;
class InstanceModule;
class PhysicalDeviceModule;
class LogicalDeviceModule;

class Hub
{
public:
	Deleter<ValidationLayerModule> validationLayers;
	Deleter<SurfaceModule> surface;
	Deleter<InstanceModule> instance;
	Deleter<PhysicalDeviceModule> physicalDevice;
	Deleter<LogicalDeviceModule> logicalDevice;

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

