#pragma once
#include "RenderEngine.h"
class Hub;

class ValidationLayerModule
{
private:
	bool CheckValidationLayerSupport();
	bool CheckLayer(const  vector<VkLayerProperties> availableLayers, const char * name);
public:
	const  vector<const char*> validationLayers = {
		"VK_LAYER_LUNARG_standard_validation"
	};

	bool enableValidationLayers = false;
	ValidationLayerModule();
	~ValidationLayerModule();
};

