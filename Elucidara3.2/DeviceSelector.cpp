#include "DeviceSelector.h"

//Get the device
vk::PhysicalDevice* DeviceSelector::get_device()
{
	return device;
}

//Get a family info block locked inside this the family map
// @param family type
FamilyQueueInfo* DeviceSelector::get_familyInfo(const vk::QueueFlagBits type)
{
	if (type == vk::QueueFlagBits::eCompute  ||
		type == vk::QueueFlagBits::eGraphics ||
		type == vk::QueueFlagBits::eTransfer ||
		type == vk::QueueFlagBits::eSparseBinding)
		return familyMap[type];
	else
		throw invalid_argument("indexes can only be one of the 4 available");
}

// Collect physical devices recognized by the given instance then exclude the ones unable to be used
// by the application
// @param The Vulkan instance.
// @return A list of available physical devices fit for the application.
vector<vk::PhysicalDevice> DeviceSelector::collect_devices(const vk::Instance &instance)
{
	//List from vulkan containing all available devices
	vector<vk::PhysicalDevice> physicalDeviceList = instance.enumeratePhysicalDevices();

	//New list containing the devices considered 'fit' for this application
	vector<vk::PhysicalDevice> result = vector<vk::PhysicalDevice>();

	//sort the devices into the new list
	for (vector<vk::PhysicalDevice>::iterator i = physicalDeviceList.begin(); i != physicalDeviceList.end(); i++)
		if (check_fitness(*i))
			result.push_back(*i);

	//return the new list
	return result;
}

// Check if the device cant be used by the application.
// @param The device tested
// @return if the device is fit or not
bool DeviceSelector::check_fitness(const vk::PhysicalDevice &device)
{
	//map device's indexes and information
	collect_indexsAndInfos(&device);

	//check if each family queue in this device attends to the applcation
	for (map<vk::QueueFlagBits, FamilyQueueInfo*>::iterator itr = familyMap.begin(); itr != familyMap.end(); itr++)
		if (!itr->second->validate())
			return false;
	return true;
}

// Compare two devices and return the best one (this method is currently un-implemented)
// @param two devices for comparison
// @return the best device
vk::PhysicalDevice DeviceSelector::compare_devices(const vk::PhysicalDevice& device1, const vk::PhysicalDevice& device2)
{
	return device1;
}

// Compare from all devices available and fit what is the best
// @param The device tested
// @return if the device is fit or not
vk::PhysicalDevice DeviceSelector::select_bestDevice(const vector<vk::PhysicalDevice>& devices)
{
	//the chosen device
	vk::PhysicalDevice* result = nullptr;

	//for each device considered fit for this aplication
	for each (vk::PhysicalDevice device in devices)
		//if noone was selected save the first one, if not compare both and pick the best
		result = (result == nullptr) ? &device : &compare_devices(device, *result);

	//return the best device
	return *result;
}

// Initialize the device selector and find the best device
// @param Vulkan instance
void DeviceSelector::initialize(const vk::Instance &instance)
{
	//set some specifications for this aplication
	familyMap[vk::QueueFlagBits::eGraphics]
		->set_count(1)
		->set_priority(1.0f)
		->set_required(true);

	//collect all devices
	vector<vk::PhysicalDevice> devices = collect_devices(instance);

	//save the best device found
	device = new vk::PhysicalDevice(select_bestDevice(devices));

	//remap the information from the best device found
	collect_indexsAndInfos(device);
}

// Allocate memory
DeviceSelector::DeviceSelector()
{
	//set the device to have a null pointer
	device = nullptr;

	//initialize the family map to have 4 keys, one for each family type
	familyMap[vk::QueueFlagBits::eCompute] = new FamilyQueueInfo();
	familyMap[vk::QueueFlagBits::eGraphics] = new FamilyQueueInfo();
	familyMap[vk::QueueFlagBits::eSparseBinding] = new FamilyQueueInfo();
	familyMap[vk::QueueFlagBits::eTransfer] = new FamilyQueueInfo();
}

// Deallocate memory
DeviceSelector::~DeviceSelector()
{
	//if there's a device saved
	if (device != nullptr) {
		//destroy it
		delete device;

		//reset pointer value;
		device = nullptr;
	}

	//for each key in the family map, destroy each one of them;
	for (map<vk::QueueFlagBits, FamilyQueueInfo*>::iterator itr = familyMap.begin(); itr != familyMap.end(); itr++)
		delete itr->second;
}

// Fill the family map with the device's information
// @param the device mapped
void DeviceSelector::collect_indexsAndInfos(const vk::PhysicalDevice* device)
{
	//the family prorperties collected from the device
	vector<vk::QueueFamilyProperties> properties = device->getQueueFamilyProperties();

	//for each key in the family map
	for (map<vk::QueueFlagBits, FamilyQueueInfo*>::iterator itr = familyMap.begin(); itr != familyMap.end(); itr++)

		//seek for a prorperty block related to it's type
		for (size_t j = 0; j < properties.size(); j++)

			//when find it, if that family has queues in it
			if ((properties[j].queueCount > 0 && properties[j].queueFlags & itr->first))

				//set the information
				itr->second
				   ->set_info(properties[j])
				   ->set_index(j);
}