#pragma once
#include "FamilyInfo.h"

class PhysicalDeviceMap
{
private:
	map <QFB, FamilyIndex*>* indexMap;

public:
	PhysicalDevice* device;
	
	void set_device(const PhysicalDevice device);

	FamilyIndex * get_familyIndex(const QFB type);

	bool check_fitness(map<QFB, vector<float>>& requestMap, function<bool(PhysicalDevice*)> predicate);

	PhysicalDeviceMap();

	void destroy();
};

