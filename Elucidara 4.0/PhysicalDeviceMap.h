#pragma once
#include "FamilyInfo.h"

class PhysicalDeviceMap
{
private:
	map <QFB, FamilyIndex*>* indexMap;

	int get_availableSurfaceFormat(SurfaceKHR & surface);

	int get_availablePresentationMode(SurfaceKHR & surface);

	int get_availableExtent(SurfaceKHR & surface);

public:
	FamilyIndex*			 presentQueueIndex;

	SurfaceCapabilitiesKHR   capabilities;

	SurfaceFormatKHR		 availableFormat;

	PresentModeKHR			 availablePresentMode;

	Extent2D				 availableExtent;

	void get_swapchainConstraints(SurfaceKHR & surface);

	PhysicalDevice* device;

	int grade = 0;

	void set_device(const PhysicalDevice device);

	FamilyIndex * get_familyIndex(const QFB type);

	bool check_fitness(map<QFB, vector<float>>& requestMap, SurfaceKHR& surface);

	void find_presentQueue(SurfaceKHR& surface);

	PhysicalDeviceMap();

	void destroy();
};

