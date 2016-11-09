#include "ValidationLayerModule.h"

ValidationLayerModule::ValidationLayerModule(){
#ifdef NDEBUG
	enableValidationLayers = false;
	 cout << "Debug mode Off: Validation layers disabled" <<  endl;
#else
	enableValidationLayers = true;
	 cout << "Debug mode On: Validation layers enabled" <<  endl;
	if(!CheckValidationLayerSupport())
		throw  runtime_error("validation layers requested, but not available!");
#endif
}

ValidationLayerModule::~ValidationLayerModule(){
}

bool ValidationLayerModule::CheckValidationLayerSupport(){
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	 vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
	
	for (const char* layerName : validationLayers)
		if (!CheckLayer(availableLayers, layerName))
			return false;

	return true;
}

bool ValidationLayerModule::CheckLayer(const  vector<VkLayerProperties> availableLayers,const char* name){
	bool layerFound = false;

	for (const auto& layerProperties : availableLayers)
		if (strcmp(name, layerProperties.layerName) == 0) {
			layerFound = true;
			break;
		}

	if (layerFound)
		 cout << " > " << name << " was found" <<  endl;
	else
		 cout << "x> " << name << " was not found!!!" <<  endl;

	return layerFound;
}