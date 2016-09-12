#include "LogicalDeviceModule.h"



void LogicalDeviceModule::SetComandQueues()
{
	graphicsQueue.count = 1;
	graphicsQueue.priority = 1.0f;
	graphicsQueue.index = Hub::s->physicalDevice->graphicsFamilyIndex;
	graphicsQueue.SetCreationInfo();


	presentQueue.count = 1;
	presentQueue.priority = 1.0f;
	presentQueue.index = Hub::s->physicalDevice->presentFamilyIndex;
	presentQueue.SetCreationInfo();

	computeQueue.count = 1;
	computeQueue.priority = 1.0f;
	computeQueue.index = Hub::s->physicalDevice->computeFamilyIndex;
	computeQueue.SetCreationInfo();
}

LogicalDeviceModule::LogicalDeviceModule()
{
}


LogicalDeviceModule::~LogicalDeviceModule()
{
}

void LogicalDeviceModule::InitializeLogicalDevice()
{
	SetComandQueues();

	vector<VkDeviceQueueCreateInfo> queueInfo{};
	queueInfo.push_back(graphicsQueue.creationInfo);
	queueInfo.push_back(presentQueue.creationInfo);
	queueInfo.push_back(computeQueue.creationInfo);

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = queueInfo.data();
	createInfo.queueCreateInfoCount = 1;
	createInfo.pEnabledFeatures = &Hub::s->physicalDevice->deviceFeatures;

	if (vkCreateDevice(Hub::s->physicalDevice->physicalDevice, &createInfo, nullptr, device.Reset()) != VK_SUCCESS) {
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
	vkGetDeviceQueue(Hub::s->logicalDevice->device, index, 0, &handler);
}
