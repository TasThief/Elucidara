#pragma once
#include "timewrap.h"
#include "FamilyInfo.h"
#include "PhysicalDeviceMap.h"

class RenderingEngine;


class RenderingEngineBuilder
{
private:
public:
//	const int WIDTH = 1280;
//	const int HEIGHT = 720;

	RenderingEngine* engine = nullptr;

	map<QFB, vector<float>> requestMap;

	timewrap<map<QFB, vector<Queue>>> queueMap		  ;
	timewrap<Queue*>				  presentQueue	  ;
	timewrap<vector<const char*>>	  layers		  ;
	timewrap<Instance>				  instance	      ;
	timewrap<SDL_Window>			  window		  ;
	timewrap<SurfaceKHR>			  surface		  ;
	timewrap<Device>				  device		  ;
	timewrap<PhysicalDeviceMap>		  physicalDevice  ;
	timewrap<SwapchainKHR>			  swapchain;

	void build_window		  ();
	void build_layers		  ();
	void build_physicalDevice ();
	void build_device		  ();
	void build_queueHandlers  ();
	void build_swapchain	  ();
	void build_instance	      ();
	void build_surface		  ();


	PhysicalDeviceMap * compare_devices(PhysicalDeviceMap * map1, PhysicalDeviceMap * map2);

	void build				  ();
// 	void build_gQueue		  ();
// 	void build_commandPool	  ();

	RenderingEngineBuilder(RenderingEngine* engine);
	~RenderingEngineBuilder();
};

