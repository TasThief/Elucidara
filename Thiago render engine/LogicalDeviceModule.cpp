#include "LogicalDeviceModule.h"



LogicalDeviceModule::LogicalDeviceModule()
{
}


LogicalDeviceModule::~LogicalDeviceModule()
{
}

void LogicalDeviceModule::InitializeLogicalDevice(PhysicalDeviceModule* physicalDevice)
{
	//save physical device Pointer
	physicalDeviceModuleRef = physicalDevice;

	//Initialize the device obj
	device.New( vkDestroyDevice );

	//initialize queues
	graphicsQueue.InitializeHandler(this, 1, 1.0f, physicalDeviceModuleRef->graphicsFamilyIndex);
	presentQueue.InitializeHandler(this, 1, 1.0f, physicalDeviceModuleRef->presentFamilyIndex);
	computeQueue.InitializeHandler(this, 1, 1.0f, physicalDeviceModuleRef->computeFamilyIndex);

	//create a list with all comandqueues info
	vector<VkDeviceQueueCreateInfo> queueInfo{};
	queueInfo.push_back(graphicsQueue.creationInfo);
	queueInfo.push_back(presentQueue.creationInfo);
	queueInfo.push_back(computeQueue.creationInfo);

	//initialize the virual device info
	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = queueInfo.data();
	createInfo.queueCreateInfoCount = 1;
	createInfo.pEnabledFeatures = &physicalDeviceModuleRef->deviceFeatures;
	createInfo.enabledExtensionCount = physicalDeviceModuleRef->deviceExtensions.size();
	createInfo.ppEnabledExtensionNames = physicalDeviceModuleRef->deviceExtensions.data();
	//create the virtual device info
	if (vkCreateDevice(physicalDeviceModuleRef->physicalDevice, &createInfo, nullptr, device.Reset()) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device!");
	}
	cout << "Logical device created " << endl;

	presentQueue.SetHandler();
	cout << "Graphics queue handler created" << endl;

	computeQueue.SetHandler();
	cout << "Presentation queue handler created" << endl;

	graphicsQueue.SetHandler();
	cout << "Compute queue handler created" << endl;

}

void ComandQueue::SetCreationInfo()
{
	creationInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	creationInfo.queueFamilyIndex = index;
	creationInfo.queueCount = count;
	creationInfo.pQueuePriorities = &priority;
	//creationInfo.pNext;
	//creationInfo.flags;
}

void ComandQueue::SetHandler()
{
	vkGetDeviceQueue(logicalDeviceModuleRef->device, index, 0, &handler);
}

void ComandQueue::InitializeHandler(LogicalDeviceModule* moduleRef, uint32_t queueCount, float queuePriority, uint32_t queueIndex)
{
	logicalDeviceModuleRef = moduleRef;
	count = queueCount;
	priority = queuePriority;
	index = queueIndex;
	SetCreationInfo();
}
