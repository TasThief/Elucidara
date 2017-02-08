#pragma once
#include "stdafx.h"

struct FamilyIndex
{
	int index = -1;
	QueueFamilyProperties info;

	bool validate(const vector<float>& request){
		return (info.queueCount > request.size() && index != -1);
	}
};