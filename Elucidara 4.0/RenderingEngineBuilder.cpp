#include "stdafx.h"
#include "RenderingEngineBuilder.h"


RenderingEngineBuilder::RenderingEngineBuilder(RenderingEngine* engine)
{
	this->engine = engine;
}

void RenderingEngineBuilder::build_layers()
{
	//initialize the layer's array
	engine->validationLayers = new vector<const char*>();

	//if in debugmode
#if defined(_DEBUG)

	//push back the lunarG standard validation layer
	engine->validationLayers->push_back("VK_LAYER_LUNARG_standard_validation");

	//verify if the layers are suported
	uint32_t layerCount;
	enumerateInstanceLayerProperties(&layerCount, nullptr);

	vector<LayerProperties> availableLayers(layerCount);
	enumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : *engine->validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers)
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}

		//if they are not throw an error
		if (!layerFound)
			throw std::system_error(std::error_code(), "Layer not setup properly!");
	}
#endif

	cout << "validation layers completed" << endl;
	//save it in the timewrap
	layers.set(engine->validationLayers);

}

void RenderingEngineBuilder::build_window()
{
	// Create an SDL window that supports Vulkan and OpenGL rendering.
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cout << "Could not initialize SDL." << std::endl;
	}

	engine->window = SDL_CreateWindow(
		"Vulkan Window",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		1280, 720,
		SDL_WINDOW_OPENGL);

	if (engine->window == NULL) {
		std::cout << "Could not create SDL window." << std::endl;
	}

	cout << "window completed" << endl;
	window.set(engine->window);

}

void RenderingEngineBuilder::build_instance()
{
	layers.wait();

	// ApplicationInfo allows the programmer to specifiy some basic information about the
	// program, which can be useful for layers and tools to provide more debug information.
	ApplicationInfo appInfo = ApplicationInfo()
		.setPApplicationName("Fps")
		.setApplicationVersion(1)
		.setPEngineName("Elucidara")
		.setEngineVersion(3.2)
		.setApiVersion(VK_API_VERSION_1_0);

	std::vector<const char*> extensions;
	extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);

#if defined(VK_USE_PLATFORM_ANDROID_KHR)
	extensions.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_MIR_KHR)
	extensions.push_back(VK_KHR_MIR_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
	extensions.push_back(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_WIN32_KHR)
	extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
	extensions.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
#endif


	// InstanceCreateInfo is where the programmer specifies the layers and/or extensions that are needed.
	InstanceCreateInfo instInfo = InstanceCreateInfo()
		.setFlags(InstanceCreateFlags())
		.setPApplicationInfo(&appInfo)
		.setEnabledExtensionCount(static_cast<uint32_t>(extensions.size()))
		.setPpEnabledExtensionNames(extensions.data())
		.setEnabledLayerCount(static_cast<uint32_t>(layers.get()->size()))
		.setPpEnabledLayerNames(layers.get()->data());

	// Create the Vulkan instance.
	engine->instance = new Instance(createInstance(instInfo));
	
	cout << "instance completed" << endl;
	instance.set(engine->instance);

}

void RenderingEngineBuilder::build_surface()
{
	//wait for the window to be acquired
	window.wait();
	SDL_SysWMinfo windowInfo;

	SDL_VERSION(&windowInfo.version);

	if (!SDL_GetWindowWMInfo(window, &windowInfo)) {
		throw std::system_error(std::error_code(), "SDK window manager info is not available.");
	}

	//wait for the instance to be acquired
	instance.wait();

#if defined(SDL_VIDEO_DRIVER_ANDROID) && defined(VK_USE_PLATFORM_ANDROID_KHR)
	if (windowInfo.subsystem == SDL_SYSWM_ANDROID)
	{
		vk::AndroidSurfaceCreateInfoKHR surfaceInfo = vk::AndroidSurfaceCreateInfoKHR()
			.setWindow(windowInfo.info.android.window);

		engine->surface = new SurfaceKHR(instance.get()->createAndroidSurfaceKHR(surfaceInfo));
	}
#endif

#if defined(SDL_VIDEO_DRIVER_MIR) && defined(VK_USE_PLATFORM_MIR_KHR)
	if (windowInfo.subsystem == SDL_SYSWM_MIR)
	{
		vk::MirSurfaceCreateInfoKHR surfaceInfo = vk::MirSurfaceCreateInfoKHR()
			.setConnection(windowInfo.info.mir.connection)
			.setMirSurface(windowInfo.info.mir.surface);

		engine->surface = new SurfaceKHR(instance.get()->createMirSurfaceKHR(surfaceInfo));
	}
#endif

#if defined(SDL_VIDEO_DRIVER_WAYLAND) && defined(VK_USE_PLATFORM_WAYLAND_KHR)
	if (windowInfo.subsystem == SDL_SYSWM_WAYLAND)
	{
		vk::WaylandSurfaceCreateInfoKHR surfaceInfo = vk::WaylandSurfaceCreateInfoKHR()
			.setDisplay(windowInfo.info.wl.display)
			.setSurface(windowInfo.info.wl.surface);

		engine->surface = new SurfaceKHR(instance.get()->createWaylandSurfaceKHR(surfaceInfo));
	}
#endif

#if defined(SDL_VIDEO_DRIVER_WINDOWS) && defined(VK_USE_PLATFORM_WIN32_KHR)
	if (windowInfo.subsystem == SDL_SYSWM_WINDOWS)
	{
		vk::Win32SurfaceCreateInfoKHR surfaceInfo = vk::Win32SurfaceCreateInfoKHR()
			.setHinstance(GetModuleHandle(NULL))
			.setHwnd(windowInfo.info.win.window);

		engine->surface = new SurfaceKHR(instance.get()->createWin32SurfaceKHR(surfaceInfo));
	}
#endif

#if defined(SDL_VIDEO_DRIVER_X11) && defined(VK_USE_PLATFORM_XLIB_KHR)
	if (windowInfo.subsystem == SDL_SYSWM_X11)
	{
		vk::XlibSurfaceCreateInfoKHR surfaceInfo = vk::XlibSurfaceCreateInfoKHR()
			.setDpy(windowInfo.info.x11.display)
			.setWindow(windowInfo.info.x11.window);

		engine->surface = new SurfaceKHR(instance.get()->createXlibSurfaceKHR(surfaceInfo));
	}
#endif

	else
		throw std::system_error(std::error_code(), "Unsupported window manager is in use.");

	cout << "surface completed" << endl;
	surface.set(engine->surface);
}

void RenderingEngineBuilder::build_physicalDevice()
{

}

void RenderingEngineBuilder::build()
{
	cout << "builder initialized" << endl << endl;

	cout << "starting to build: surface" << endl;
	thread t_surface(&RenderingEngineBuilder::build_surface, this);
	t_surface.detach();

	cout << "starting to build: instance" << endl;
	thread t_instance(&RenderingEngineBuilder::build_instance, this);
	t_instance.detach();
	
	cout << "starting to build: validation layers" << endl;
	thread t_layers(&RenderingEngineBuilder::build_layers, this);
	t_layers.detach();

	//thread t_window(&RenderingEngineBuilder::build_window, this);
	//t_window.detach();
	cout << "starting to build: window" << endl;
	build_window();


	surface.wait();
	instance.wait();
	window.wait();
	layers.wait();

	cout << endl << "building finished" << endl;
}


RenderingEngineBuilder::~RenderingEngineBuilder()
{
}

//void RenderingEngineBuilder::build_gQueue(Queue & value)
//{
//	instance = new Instance(value);
//	engine->instance = instance;
//	flag_instance.notify_all();
//}
//
//void RenderingEngineBuilder::build_commandPool(CommandPool & value)
//{
//	instance = new Instance(value);
//	engine->instance = instance;
//	flag_instance.notify_all();
//}
