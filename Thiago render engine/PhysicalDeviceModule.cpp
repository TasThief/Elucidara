#include "PhysicalDeviceModule.h"

bool PhysicalDeviceModule::IsDeviceSuitable(VkPhysicalDevice device) {

	bool result = true;
	FindQueueFamilies(device);


	if (graphicsFamilyIndex < 0 || presentFamilyIndex < 0 || computeFamilyIndex < 0)
	{
		graphicsFamilyIndex = -1;
		presentFamilyIndex = -1;
		computeFamilyIndex = -1;
		result = false;
	}

	if(result)
		cout << "Graphics, presentation and compute queue families found" << endl;

	return result;
}

vector<VkPhysicalDevice> PhysicalDeviceModule::GetDeviceList()
{
	//ammount of physical devices
	uint32_t deviceCount = 0;

	//get howmuch i have at my disposal
	vkEnumeratePhysicalDevices(*Hub::s->GetInstance(), &deviceCount, nullptr);
	
	//if none was found throw an error
	if (deviceCount == 0)
		throw runtime_error("failed to find GPUs with Vulkan support!");

	//the vector filled with physical devices objects
	vector<VkPhysicalDevice> devices(deviceCount);

	//fill the physical devices list
	vkEnumeratePhysicalDevices(*Hub::s->GetInstance(), &deviceCount, devices.data());

	//return the vector list;
	return devices;
}

void PhysicalDeviceModule::InitializePhysicalDevice()
{
	for (const auto& device : GetDeviceList()) {
		if (IsDeviceSuitable(device)) {
			physicalDevice = device;
			break;
		}
	}

	if (physicalDevice == VK_NULL_HANDLE) {
		throw runtime_error("failed to find a suitable GPU!");
	}

	vkGetPhysicalDeviceProperties(physicalDevice, &properties);
	cout << "Physical device fully initialized using " <<properties.deviceName<< " as main device"<< endl;

	VkPhysicalDeviceFeatures temp{};
	deviceFeatures = temp;
}

void PhysicalDeviceModule::FindQueueFamilies(VkPhysicalDevice device)
{
	//family count
	uint32_t queueFamilyCount = 0;

	//fill the count value
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	//initialize vector with family properties
	vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);

	//fill the vector
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	for (int i = 0; i < queueFamilyCount &&
		(graphicsFamilyIndex < 0 || presentFamilyIndex < 0 || computeFamilyIndex < 0); i++)
	{
		if (queueFamilies[i].queueCount > 0 && queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			graphicsFamilyIndex = i;

		if (queueFamilies[i].queueCount > 0 && queueFamilies[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
			presentFamilyIndex = i;

		if (queueFamilies[i].queueCount > 0 && queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
			computeFamilyIndex = i;
	}
}


PhysicalDeviceModule::PhysicalDeviceModule(){}

PhysicalDeviceModule::~PhysicalDeviceModule(){}

