#include "RenderEngine.h"

void RenderingEngine::init_ValidationLayers()
{
#if defined(_DEBUG)
	layers.push_back("VK_LAYER_LUNARG_standard_validation");
#endif
}

void RenderingEngine::init_Instance()
{
	// vk::ApplicationInfo allows the programmer to specifiy some basic information about the
	// program, which can be useful for layers and tools to provide more debug information.
	vk::ApplicationInfo appInfo = vk::ApplicationInfo()
		.setPApplicationName("Vulkan C++ Windowed Program Template")
		.setApplicationVersion(1)
		.setPEngineName("LunarG SDK")
		.setEngineVersion(1)
		.setApiVersion(VK_API_VERSION_1_0);

	// vk::InstanceCreateInfo is where the programmer specifies the layers and/or extensions that
	// are needed.
	vk::InstanceCreateInfo instInfo = vk::InstanceCreateInfo()
		.setFlags(vk::InstanceCreateFlags())
		.setPApplicationInfo(&appInfo)
		.setEnabledExtensionCount(static_cast<uint32_t>(extensions.size()))
		.setPpEnabledExtensionNames(extensions.data())
		.setEnabledLayerCount(static_cast<uint32_t>(layers.size()))
		.setPpEnabledLayerNames(layers.data());

	// Create the Vulkan instance.
	try {
		instance = vk::createInstance(instInfo);
	}
	catch (const std::exception& e) {
		std::cout << "Could not create a Vulkan instance: " << e.what() << std::endl;
	}
}

void RenderingEngine::init_Window()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cout << "Could not initialize SDL." << std::endl;
	}

	window = SDL_CreateWindow("Vulkan Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);

	if (window == NULL) {
		std::cout << "Could not create SDL window." << std::endl;
	}
}

void RenderingEngine::init_Surface()
{
	try {
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
			break;
		}
	}
	catch (const std::exception& e) {
		std::cout << "Failed to create Vulkan surface: " << e.what() << std::endl;
		instance.destroy();
	}
}

void RenderingEngine::init_Device()
{
	//get the family queues
	uint32_t queueBlockCount = 2;
	std::vector<vk::DeviceQueueCreateInfo> deviceQueueCreateInfoList(queueBlockCount);
	deviceQueueCreateInfoList[0] = get_FamilyQueues(physicalDevice, 1, vk::QueueFlagBits::eGraphics);
	deviceQueueCreateInfoList[1] = get_FamilyQueues(physicalDevice, 1, vk::QueueFlagBits::eCompute);

	//set the desired features;
	vk::PhysicalDeviceFeatures deviceFeatures = vk::PhysicalDeviceFeatures();

	//create the device info object
	vk::DeviceCreateInfo deviceInfo = vk::DeviceCreateInfo()
		.setQueueCreateInfoCount(queueBlockCount)
		.setPQueueCreateInfos(deviceQueueCreateInfoList.data())
		.setPEnabledFeatures(&deviceFeatures);
//		.setEnabledExtensionCount(0)
//		.setPpEnabledExtensionNames(ppEnabledExtensionNames_);

	//initialize the device
	device = physicalDevice.createDevice(deviceInfo);

	queueGraphic1 = device.getQueue(deviceQueueCreateInfoList[0].queueFamilyIndex, 0);
	queueCompute1 = device.getQueue(deviceQueueCreateInfoList[1].queueFamilyIndex, 0);

	commandPoolGraphic = device.createCommandPool(vk::CommandPoolCreateInfo().setQueueFamilyIndex(deviceQueueCreateInfoList[0].queueFamilyIndex));
	commandPoolCompute = device.createCommandPool(vk::CommandPoolCreateInfo().setQueueFamilyIndex(deviceQueueCreateInfoList[1].queueFamilyIndex));

	//this object allocates stuff into the pool
	std::vector<vk::CommandBuffer> allocCBufferGraphic = device.allocateCommandBuffers(vk::CommandBufferAllocateInfo()
		.setCommandBufferCount(1)
		.setCommandPool(commandPoolGraphic)
		.setLevel(vk::CommandBufferLevel::ePrimary));

	//this object allocates stuff into the pool
	std::vector<vk::CommandBuffer> allocCBufferCompute = device.allocateCommandBuffers(vk::CommandBufferAllocateInfo()
		.setCommandBufferCount(1)
		.setCommandPool(commandPoolCompute)
		.setLevel(vk::CommandBufferLevel::ePrimary));


}

void RenderingEngine::init_SwapChain()
{
//	physicalDevice.getSurfaceSupportKHR()
}

// Test the devices to get the best one
bool RenderingEngine::isDeviceSuitable(vk::PhysicalDevice* device)
{
	return true;
}

// Pick a device to be used
void RenderingEngine::init_PhysicalDevice()
{
	vk::PhysicalDevice* result = nullptr;

	std::vector<vk::PhysicalDevice> devicesAvaliable = instance.enumeratePhysicalDevices();

	for each (vk::PhysicalDevice device in devicesAvaliable)
		if (isDeviceSuitable(&device)) 
		{
			std::string deviceName = device.getProperties().deviceName;
			std::cout << deviceName << " found.";

			result = &device;
		}

	if (!result)
		std::cout << "No gpu with vulkan support was found.";

	physicalDevice = *result;
}

vk::DeviceQueueCreateInfo RenderingEngine::get_FamilyQueues(vk::PhysicalDevice& pDevice, const uint32_t queuesCount, vk::QueueFlagBits family)
{
	std::vector<vk::QueueFamilyProperties> queueFamilyPropList = pDevice.getQueueFamilyProperties();

	vk::DeviceQueueCreateInfo graphicsFamilyPropInfo = vk::DeviceQueueCreateInfo();

	//seek the given family.
	bool found = false;
	for (size_t i = 0; i < queueFamilyPropList.size() && !found; i++) {
		if (queueFamilyPropList[i].queueFlags & family) {						 //<-- when find it, save it
			graphicsFamilyPropInfo.setQueueFamilyIndex(i);			 //<-- save in the info block the index of this family
			found = true;											 //<-- flag to stop the search
		}
	}

	if (!found)
		throw std::runtime_error("failed to load a family queue!");

	//set the priorities of all queues requested from this family
	std::vector<float> queuePriorities(queuesCount);
	for (size_t i = 0; i < queuesCount; i++)
		queuePriorities[i] = 0.0;

	//fill the info with remaining info
	graphicsFamilyPropInfo
		.setPQueuePriorities(queuePriorities.data())
		.setQueueCount(queuesCount);

	//return the queue info
	return graphicsFamilyPropInfo;
}

void RenderingEngine::get_AvailableWSIExtensions()
{
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

}



void RenderingEngine::initialize()
{
	init_ValidationLayers();

	get_AvailableWSIExtensions();

	init_Instance();

	init_Window();

	init_Surface();

	init_PhysicalDevice();

	init_Device();
}

void RenderingEngine::finalize()
{
	// Clean up.
	instance.destroySurfaceKHR(surface);

	SDL_DestroyWindow(window);

	SDL_Quit();

	instance.destroy();
}

void RenderingEngine::update()
{

	bool stillRunning = true;
	do 
	{
			//stillRunning = renderingEngine.update();


		SDL_Event event;
		while (SDL_PollEvent(&event)) {

			switch (event.type) {

			case SDL_QUIT:
				stillRunning = false;
				break;

			default:
				// Do nothing.
				stillRunning = true;
				break;
			}
		}

		SDL_Delay(10);
	} while (stillRunning);
}



RenderingEngine::RenderingEngine()
{
}

RenderingEngine::~RenderingEngine()
{
}
