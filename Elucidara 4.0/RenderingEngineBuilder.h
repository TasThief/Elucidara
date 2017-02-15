#pragma once
#include "timewrap.h"
#include "FamilyInfo.h"
#include "PhysicalDeviceMap.h"

class RenderingEngine;


class RenderingEngineBuilder
{
private:
public:

	timewrap<RenderingEngine> engine;

	map<QFB, vector<float>> requestMap;

	timewrap<map<QFB, vector<Queue>>> queueMap		  ;
	timewrap<Queue*>				  presentQueue	  ;
	timewrap<vector<const char*>>	  layers		  ;
	timewrap<Instance>				  instance	      ;
	timewrap<SDL_Window>			  window		  ;
	timewrap<SurfaceKHR>			  surface		  ;
	timewrap<Device>				  device		  ;
	timewrap<PhysicalDeviceMap>		  physicalDevice  ;
	timewrap<SwapchainKHR>			  swapchain		  ;
	timewrap<vector<ImageView>>		  imageView		  ;
	timewrap<vector<Image>>			  images		  ;
	timewrap<Image>					  depthBufferImage;
	timewrap<ImageView>				  depthBufferView ;

	void build_window		  ();
	void build_layers		  ();
	void build_physicalDevice ();
	void build_device		  ();
	void build_queueHandlers  ();
	void build_swapchain	  ();
	void build_imageViews	  ();
	void build_instance	      ();
	void build_surface		  ();
	void build_depthBuffer	  ();

	PhysicalDeviceMap * compare_devices(PhysicalDeviceMap * map1, PhysicalDeviceMap * map2);

	void build				  ();
// 	void build_gQueue		  ();
// 	void build_commandPool	  ();

	RenderingEngineBuilder(RenderingEngine* engine);
	~RenderingEngineBuilder();
};

