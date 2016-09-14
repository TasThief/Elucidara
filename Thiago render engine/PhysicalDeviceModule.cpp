#include "PhysicalDeviceModule.h"

bool PhysicalDeviceModule::IsDeviceSuitable(VkPhysicalDevice device,SwapChainModule* swapChain,SurfaceModule* surface) {

	FindQueueFamilies(device);

	swapChain->QueryForSwapChainSupport(&device);

	//check if have the families
	if ((graphicsFamilyIndex > -1 && presentFamilyIndex > -1 && computeFamilyIndex > -1)&&
		(CheckDeviceExtensionSupport(device)) &&
		(swapChain->IsAdequate()))
		return true;
	else
		ResetFamilyIndex();

	return false;
} 
void PhysicalDeviceModule::ResetFamilyIndex()
{
	graphicsFamilyIndex = -1;
	presentFamilyIndex = -1;
	computeFamilyIndex = -1;
}
bool PhysicalDeviceModule::CheckDeviceExtensionSupport(const VkPhysicalDevice device)
{
	//how much extensions i'll have
	uint32_t extensionCount;

	//get that number
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	//initialize a list with extension properties
	vector<VkExtensionProperties> availableExtensions(extensionCount);

	//save its data
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	//clone a list of required extensions
	set<string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	cout << endl << "Available extensions" << endl;
	for (const auto& extension : availableExtensions)
		cout << extension.extensionName << endl;
	cout << endl;

	//checklist if this device has those extensions
	for (const auto& extension : availableExtensions)
		requiredExtensions.erase(extension.extensionName);
	bool result = requiredExtensions.empty();
	//return if there are no item left
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

void PhysicalDeviceModule::InitializePhysicalDevice(SwapChainModule* swapChain, SurfaceModule* surface)
{
	for (const auto& device : GetDeviceList()) {
		if (IsDeviceSuitable(device, swapChain, surface)) {
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

