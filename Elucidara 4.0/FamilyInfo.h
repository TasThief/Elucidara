#pragma once
#include "stdafx.h"
/*
struct FamilyRequest
{
	int count = 0;
	float priority = 0.0f;
};*/

struct FamilyIndex
{
	int index = -1;
	vk::QueueFamilyProperties info;

	bool validate(const vector<float>& request){
		return (info.queueCount > request.size() && index != -1);
	}
};