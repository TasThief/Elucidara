#pragma once
#include "RenderEngine.h"
class Hub;

class InstanceModule
{
public:
	VkDeleter<VkInstance> instance;//{ vkDestroyInstance };
	InstanceModule();
	void InitializeInstance();
	~InstanceModule();

private:
	vector<const char*> GetRequiredExtensions();
	VkApplicationInfo GenerateApplicationInfoBlock();
	VkInstanceCreateInfo GenerateVkInstanceInfoBlock();
};


