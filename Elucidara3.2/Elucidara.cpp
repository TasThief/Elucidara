#include "Elucidara.h"

Elucidara::Elucidara()
{
	init_validationLayers();
	init_instance();
	init_window();
	init_surface();
	init_device();
}

Elucidara::~Elucidara()
{
	//destroy surface
	instance.destroySurfaceKHR(surface);

	//destroy window
	SDL_DestroyWindow(window);
	SDL_Quit();

	//destroy device
	device.destroy();

	//destroy instance
	instance.destroy();
}

void Elucidara::init_validationLayers()
{
#if defined(_DEBUG)
	validationLayers.push_back("VK_LAYER_LUNARG_standard_validation");

	uint32_t layerCount;
	vk::enumerateInstanceLayerProperties(&layerCount, nullptr);

	vector<vk::LayerProperties> availableLayers(layerCount);
	vk::enumerateInstanceLayerProperties(&layerCount, availableLayers.data());
	
	for (const char* layerName : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers)
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}

		if (!layerFound)
			throw std::system_error(std::error_code(), "Layer not setup properly!");
	}
#endif
}

void Elucidara::init_instance()
{
	// vk::ApplicationInfo allows the programmer to specifiy some basic information about the
	// program, which can be useful for layers and tools to provide more debug information.
	vk::ApplicationInfo appInfo = vk::ApplicationInfo()
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

	// vk::InstanceCreateInfo is where the programmer specifies the layers and/or extensions that
	// are needed.
	vk::InstanceCreateInfo instInfo = vk::InstanceCreateInfo()
		.setFlags(vk::InstanceCreateFlags())
		.setPApplicationInfo(&appInfo)
		.setEnabledExtensionCount(static_cast<uint32_t>(extensions.size()))
		.setPpEnabledExtensionNames(extensions.data())
		.setEnabledLayerCount(static_cast<uint32_t>(validationLayers.size()))
		.setPpEnabledLayerNames(validationLayers.data());

	// Create the Vulkan instance.
	instance = vk::createInstance(instInfo);
}

void Elucidara::init_window()
{
	// Create an SDL window that supports Vulkan and OpenGL rendering.
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cout << "Could not initialize SDL." << std::endl;
	}

	window = SDL_CreateWindow(
		"Vulkan Window",
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 
		1280, 720, 
		SDL_WINDOW_OPENGL);

	if (window == NULL) {
		std::cout << "Could not create SDL window." << std::endl;
	}
}

void Elucidara::init_surface()
{
	SDL_SysWMinfo windowInfo;
	SDL_VERSION(&windowInfo.version);
	if (!SDL_GetWindowWMInfo(window, &windowInfo)) {
		throw std::system_error(std::error_code(), "SDK window manager info is not available.");
	}

	switch (windowInfo.subsystem) {

#if defined(SDL_VIDEO_DRIVER_ANDROID) && defined(VK_USE_PLATFORM_ANDROID_KHR)
	case SDL_SYSWM_ANDROID: {
		vk::AndroidSurfaceCreateInfoKHR surfaceInfo = vk::AndroidSurfaceCreateInfoKHR()
			.setWindow(windowInfo.info.android.window);
		surface = instance.createAndroidSurfaceKHR(surfaceInfo);
		break;
	}
#endif

#if defined(SDL_VIDEO_DRIVER_MIR) && defined(VK_USE_PLATFORM_MIR_KHR)
	case SDL_SYSWM_MIR: {
		vk::MirSurfaceCreateInfoKHR surfaceInfo = vk::MirSurfaceCreateInfoKHR()
			.setConnection(windowInfo.info.mir.connection)
			.setMirSurface(windowInfo.info.mir.surface);
		surface = instance.createMirSurfaceKHR(surfaceInfo);
		break;
	}
#endif

#if defined(SDL_VIDEO_DRIVER_WAYLAND) && defined(VK_USE_PLATFORM_WAYLAND_KHR)
	case SDL_SYSWM_WAYLAND: {
		vk::WaylandSurfaceCreateInfoKHR surfaceInfo = vk::WaylandSurfaceCreateInfoKHR()
			.setDisplay(windowInfo.info.wl.display)
			.setSurface(windowInfo.info.wl.surface);
		surface = instance.createWaylandSurfaceKHR(surfaceInfo);
		break;
	}
#endif

#if defined(SDL_VIDEO_DRIVER_WINDOWS) && defined(VK_USE_PLATFORM_WIN32_KHR)
	case SDL_SYSWM_WINDOWS: {
		vk::Win32SurfaceCreateInfoKHR surfaceInfo = vk::Win32SurfaceCreateInfoKHR()
			.setHinstance(GetModuleHandle(NULL))
			.setHwnd(windowInfo.info.win.window);
		surface = instance.createWin32SurfaceKHR(surfaceInfo);
		break;
	}
#endif

#if defined(SDL_VIDEO_DRIVER_X11) && defined(VK_USE_PLATFORM_XLIB_KHR)
	case SDL_SYSWM_X11: {
		vk::XlibSurfaceCreateInfoKHR surfaceInfo = vk::XlibSurfaceCreateInfoKHR()
			.setDpy(windowInfo.info.x11.display)
			.setWindow(windowInfo.info.x11.window);
		surface = instance.createXlibSurfaceKHR(surfaceInfo);
		break;
	}
#endif

	default:
		throw std::system_error(std::error_code(), "Unsupported window manager is in use.");
	}
}

void Elucidara::init_device()
{
	//save the device and map its information
	DeviceSelector pDevice;
	pDevice.initialize(instance);
	
	//save information of the queues 
	vector<vk::DeviceQueueCreateInfo> queueCreateInfo;
	queueCreateInfo.push_back(pDevice.get_familyInfo(vk::QueueFlagBits::eGraphics)->build_queueCreateInfo());

	//initialize the device using the colected information
	vk::PhysicalDeviceFeatures features;
	vk::DeviceCreateInfo createInfo;
	createInfo
		.setPQueueCreateInfos(queueCreateInfo.data())
		.setQueueCreateInfoCount(queueCreateInfo.size())
		.setPEnabledFeatures(&features)
		.setEnabledExtensionCount(0)
#if defined(_DEBUG)
		.setEnabledLayerCount(validationLayers.size())
		.setPpEnabledLayerNames(validationLayers.data());
#else
		.setEnabledLayerCount(0);
#endif
	device = pDevice.get_device()->createDevice(createInfo);

	//get a graphics queue handler
	gQueue = device.getQueue(pDevice.get_familyInfo(vk::QueueFlagBits::eGraphics)->get_index(), 0);

	//create a graphics commandpool to store command b
	vk::CommandPoolCreateInfo poolInfo;
	poolInfo.setQueueFamilyIndex(pDevice.get_familyInfo(vk::QueueFlagBits::eGraphics)->get_index());
	commandPool = device.createCommandPool(poolInfo);


	vk::CommandBufferAllocateInfo allocInfo;
	allocInfo
		.setLevel(vk::CommandBufferLevel::ePrimary)
		.setCommandPool(commandPool)
		.setCommandBufferCount(1);

	device.allocateCommandBuffers(allocInfo);
}
