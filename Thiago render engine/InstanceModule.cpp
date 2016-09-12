#include "InstanceModule.h"

 vector<const char*> InstanceModule::GetRequiredExtensions()
{
	//the extentions to be returned
	 vector<const char*> extensions;

	//the amount of extentions
	unsigned int glfwExtensionCount = 0;

	//the name of the extentions
	const char** glfwExtensions;

	//Get required extentions
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	//Build the extentions list
	for (unsigned int i = 0; i < glfwExtensionCount; i++)
		extensions.push_back(glfwExtensions[i]);

	if (Hub::s->AreValidationLayersEnabled())
		extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

	return extensions;
}


VkApplicationInfo InstanceModule::GenerateApplicationInfoBlock()
{
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Thiago Render Engine";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;
	//appInfo.pNext;

	return appInfo;
}

VkInstanceCreateInfo InstanceModule::GenerateVkInstanceInfoBlock()
{
	unsigned int glfwExtensionCount = 0;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	createInfo.enabledLayerCount = 0;

	if (Hub::s->AreValidationLayersEnabled()) {
		createInfo.enabledLayerCount = Hub::s->GetValidationLayers().size();
		createInfo.ppEnabledLayerNames = Hub::s->GetValidationLayers().data();
	}
	//createInfo.flags;
	//createInfo.pNext;

	return createInfo;
}

InstanceModule::InstanceModule(){
}

void InstanceModule::InitializeInstance()
{
	instance.New( vkDestroyInstance );
	VkInstanceCreateInfo info = GenerateVkInstanceInfoBlock();
	info.pApplicationInfo = &GenerateApplicationInfoBlock();

	auto extensions = GetRequiredExtensions();
	info.enabledExtensionCount = extensions.size();
	info.ppEnabledExtensionNames = extensions.data();

	if (vkCreateInstance(&info, nullptr, instance.Reset()) != VK_SUCCESS) {
		throw  runtime_error("failed to create instance!");
	}

	 cout << "Vulkan instance created" <<  endl;
}

InstanceModule::~InstanceModule()
{
}
