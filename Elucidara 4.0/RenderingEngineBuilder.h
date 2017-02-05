#pragma once
#include "timewrap.h"

class RenderingEngine;

class RenderingEngineBuilder
{
private:
	RenderingEngine* engine = nullptr;

	timewrap<vector<const char*>>	layers		    ;
	timewrap<Instance>				instance		;
	timewrap<SDL_Window>			window			;
	timewrap<SurfaceKHR>			surface			;
	timewrap<Device>				device			;
	timewrap<PhysicalDevice>		physicalDevice  ;
	
	void build_window		  ();
	void build_layers		  ();
	void build_physicalDevice ();
	void build_instance	      ();
	void build_surface		  ();

public:
	void build				  ();
// 	void build_gQueue		  ();
// 	void build_commandPool	  ();

	RenderingEngineBuilder(RenderingEngine* engine);
	~RenderingEngineBuilder();
};

