#pragma once
#include "stdafx.h"
#include "FamilyQueueInfo.h"

class DeviceSelector
{
public:
	//Get the device
	vk::PhysicalDevice * get_device();

	//Get a family info block locked inside this the family map
	// @param family type
	FamilyQueueInfo * get_familyInfo(const vk::QueueFlagBits type);

	// Initialize the device selector and find the best device
	// @param Vulkan instance
	void initialize(const vk::Instance &instance);

	// Allocate memory
	DeviceSelector();

	// Deallocate memory
	~DeviceSelector();

private:
	//The best device selected
	vk::PhysicalDevice* device;

	//Map containing all the family indexes and other sorts of info
	map <vk::QueueFlagBits, FamilyQueueInfo*> familyMap;
	
	// Collect physical devices recognized by the given instance then exclude the ones unable to be used
	// by the application
	// @param The Vulkan instance. 
	// @return A list of available physical devices fit for the application.                             
	vector<vk::PhysicalDevice> collect_devices(const vk::Instance &instance);

	// Check if the device cant be used by the application.
	// @param The device tested 
	// @return if the device is fit or not
	bool check_fitness(const vk::PhysicalDevice & device);
	
	// Compare two devices and return the best one (this method is currently un-implemented)
	// @param two devices for comparison
	// @return the best device
	vk::PhysicalDevice compare_devices(const vk::PhysicalDevice & device1, const vk::PhysicalDevice & device2);

	// Compare from all devices available and fit what is the best
	// @param The device tested 
	// @return if the device is fit or not
	vk::PhysicalDevice select_bestDevice(const vector<vk::PhysicalDevice>& devices);
	
	// Fill the family map with the device's information
	// @param the device mapped
	void collect_indexsAndInfos(const vk::PhysicalDevice* device);

};

