#include "stdafx.h"
#include "RenderingEngineBuilder.h"


RenderingEngineBuilder::RenderingEngineBuilder(RenderingEngine* engine)
{
	this->engine.set(engine);

	requestMap[QFB::eGraphics].push_back(1.0f);
	requestMap[QFB::eGraphics].push_back(1.0f);
}

void RenderingEngineBuilder::build_layers()
{
	//initialize the layer's array
	engine.get()->validationLayers = new vector<const char*>();

	//if in debugmode
#if defined(_DEBUG)

	//push back the lunarG standard validation layer
	engine.get()->validationLayers->push_back("VK_LAYER_LUNARG_standard_validation");

	//verify if the layers are suported
	uint32_t layerCount;
	enumerateInstanceLayerProperties(&layerCount, nullptr);

	vector<LayerProperties> availableLayers(layerCount);
	enumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : *engine.get()->validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers)
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}

		//if they are not throw an error
		if (!layerFound)
			throw system_error(std::error_code(), "Layer not setup properly!");
	}
#endif

	cout << "validation layers completed" << endl;
	//save it in the timewrap
	layers.set(engine.get()->validationLayers);

}

void RenderingEngineBuilder::build_window()
{
	// Create an SDL window that supports Vulkan and OpenGL rendering.
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cout << "Could not initialize SDL." << std::endl;
	}

	engine.get()->window = SDL_CreateWindow(
		"Vulkan Window",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WIDTH, HEIGHT,
		SDL_WINDOW_OPENGL);

	if (engine.get()->window == NULL) {
		std::cout << "Could not create SDL window." << std::endl;
	}

	cout << "window completed" << endl;
	window.set(engine.get()->window);

}

void RenderingEngineBuilder::build_instance()
{
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

	InstanceCreateInfo instInfo = InstanceCreateInfo()
		.setFlags(InstanceCreateFlags())
		.setPApplicationInfo(&appInfo)
		.setEnabledExtensionCount(static_cast<uint32_t>(extensions.size()))
		.setPpEnabledExtensionNames(extensions.data())
		.setEnabledLayerCount(static_cast<uint32_t>(layers.get()->size()))
		.setPpEnabledLayerNames(layers.get()->data());

	engine.get()->instance = new Instance(createInstance(instInfo));
	
	cout << "instance completed" << endl;
	instance.set(engine.get()->instance);
}

void RenderingEngineBuilder::build_surface()
{
	//wait for the window to be acquired
//	window.wait();
	SDL_SysWMinfo windowInfo;

	SDL_VERSION(&windowInfo.version);

	if (!SDL_GetWindowWMInfo(window.get(), &windowInfo)) {
		throw system_error(std::error_code(), "SDK window manager info is not available.");
	}

	//wait for the instance to be acquired
//	instance.wait();

#if defined(SDL_VIDEO_DRIVER_ANDROID) && defined(VK_USE_PLATFORM_ANDROID_KHR)
	if (windowInfo.subsystem == SDL_SYSWM_ANDROID)
	{
		AndroidSurfaceCreateInfoKHR surfaceInfo = AndroidSurfaceCreateInfoKHR()
			.setWindow(windowInfo.info.android.window);

		engine.get()->surface = new SurfaceKHR(instance.get()->createAndroidSurfaceKHR(surfaceInfo));
	}
#endif

#if defined(SDL_VIDEO_DRIVER_MIR) && defined(VK_USE_PLATFORM_MIR_KHR)
	if (windowInfo.subsystem == SDL_SYSWM_MIR)
	{
		MirSurfaceCreateInfoKHR surfaceInfo = MirSurfaceCreateInfoKHR()
			.setConnection(windowInfo.info.mir.connection)
			.setMirSurface(windowInfo.info.mir.surface);

		engine.get()->surface = new SurfaceKHR(instance.get()->createMirSurfaceKHR(surfaceInfo));
	}
#endif

#if defined(SDL_VIDEO_DRIVER_WAYLAND) && defined(VK_USE_PLATFORM_WAYLAND_KHR)
	if (windowInfo.subsystem == SDL_SYSWM_WAYLAND)
	{
		WaylandSurfaceCreateInfoKHR surfaceInfo = WaylandSurfaceCreateInfoKHR()
			.setDisplay(windowInfo.info.wl.display)
			.setSurface(windowInfo.info.wl.surface);

		engine.get()->surface = new SurfaceKHR(instance.get()->createWaylandSurfaceKHR(surfaceInfo));
	}
#endif

#if defined(SDL_VIDEO_DRIVER_WINDOWS) && defined(VK_USE_PLATFORM_WIN32_KHR)
	if (windowInfo.subsystem == SDL_SYSWM_WINDOWS)
	{
		Win32SurfaceCreateInfoKHR surfaceInfo = Win32SurfaceCreateInfoKHR()
			.setHinstance(GetModuleHandle(NULL))
			.setHwnd(windowInfo.info.win.window);

		engine.get()->surface = new SurfaceKHR(instance.get()->createWin32SurfaceKHR(surfaceInfo));
	}
#endif

#if defined(SDL_VIDEO_DRIVER_X11) && defined(VK_USE_PLATFORM_XLIB_KHR)
	if (windowInfo.subsystem == SDL_SYSWM_X11)
	{
		XlibSurfaceCreateInfoKHR surfaceInfo = XlibSurfaceCreateInfoKHR()
			.setDpy(windowInfo.info.x11.display)
			.setWindow(windowInfo.info.x11.window);

		engine.get()->surface = new SurfaceKHR(instance.get()->createXlibSurfaceKHR(surfaceInfo));
	}
#endif

	else
		throw system_error(std::error_code(), "Unsupported window manager is in use.");

	cout << "surface completed" << endl;
	surface.set(engine.get()->surface);
}

void RenderingEngineBuilder::build_depthBuffer()
{
	Extent3D extent(physicalDevice.get()->availableExtent.width, physicalDevice.get()->availableExtent.height, 1);
	
	ImageCreateInfo imageInfo;
	imageInfo
		.setImageType(ImageType::e2D)
		.setFormat(Format::eD16Unorm)
		.setExtent(extent)
		.setArrayLayers(1)
		.setMipLevels(1)
	//	.setInitialLayout(ImageLayout::eDepthStencilAttachmentOptimal)
		.setUsage(ImageUsageFlagBits::eDepthStencilAttachment);
	
	engine.get()->depthBuffer = new Image(device.get()->createImage(imageInfo));
	
	depthBufferImage.set(engine.get()->depthBuffer);
	
	MemoryRequirements imageMR = device.get()->getImageMemoryRequirements(*depthBufferImage.get());
	
	MemoryAllocateInfo memAllocInfo;
	memAllocInfo
		.setAllocationSize(imageMR.size);
		//.setMemoryTypeIndex(find_MemoryType(imageMR.memoryTypeBits,MemoryPropertyFlagBits::))
	//imageMR.memoryTypeBits

//	PhysicalDeviceMemoryProperties memoryProp = physicalDevice.get()->device->getMemoryProperties();
	cout << "test";
}

uint32_t RenderingEngineBuilder::find_MemoryType(uint32_t typeFilter, MemoryPropertyFlagBits properties) {

	PhysicalDeviceMemoryProperties memProperties = physicalDevice.get()->device->getMemoryProperties();
	
	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && 
			(memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

//
//pass = memory_type_from_properties(info, mem_reqs.memoryTypeBits,
//	0, /* No Requirements */
//	&mem_alloc.memoryTypeIndex);
//
//bool memory_type_from_properties(struct sample_info &info, uint32_t typeBits,
//	VkFlags requirements_mask,
//	uint32_t *typeIndex) {
//	// Search memtypes to find first index with those properties
//	for (uint32_t i = 0; i < info.memory_properties.memoryTypeCount; i++) {
//		if ((typeBits & 1) == 1) {
//			// Type is available, does it match user properties?
//			if ((info.memory_properties.memoryTypes[i].propertyFlags &
//				requirements_mask) == requirements_mask) {
//				*typeIndex = i;
//				return true;
//			}
//		}
//		typeBits >>= 1;
//	}
//	// No memory types matched, return failure
//	return false;
//}






void RenderingEngineBuilder::build_physicalDevice()
{
	//collect devices
	vector<PhysicalDevice> physicalDevices = instance.get()->enumeratePhysicalDevices();

	if (physicalDevices.size() > 0)
	{
		//get a physical map list fit for the ammount if devices found
		vector<PhysicalDeviceMap> maps(physicalDevices.size());

		//start a wait flag list if more then 1 gpu was found
		vector<flag> waitList(physicalDevices.size() - 1);

		//if i have more then one device, deploy other threads to map it
		for (size_t i = 1; i < physicalDevices.size(); i++)
			ThreadPool::add_command([&waitList, &physicalDevices, &maps, i, this]() {
			maps[i].set_device(physicalDevices[i]);
			maps[i].find_presentQueue(*surface.get());
			maps[i].get_swapchainConstraints(*surface.get());
			waitList[i - 1].go();
		});

		//while other threads are mapping other possible other devices, map the device 0 
		maps[0].set_device(physicalDevices[0]);
		maps[0].find_presentQueue(*surface.get());
		maps[0].get_swapchainConstraints(*surface.get());

		//if other threads are mapping the gpu wait for them to end their job before proceding 
		for (vector<flag>::iterator itr = waitList.begin(); itr != waitList.end(); itr++)
			itr->sleep();

		//initialize a pointer to the best fit device
		PhysicalDeviceMap* result = nullptr;

		//sort out the best device
		for (vector<PhysicalDeviceMap>::iterator itr = maps.begin(); itr != maps.end(); itr++)
			
			//if the device is fit for this application
			if (itr->check_fitness(requestMap, *surface.get()))

				//compare it with other found devices
				result = (!result) ? itr._Ptr : compare_devices(itr._Ptr, result);

		//if even so no devices was found throw an error
		if(!result)
			throw system_error(error_code(), "No available gpu found");
		
		//else send a accomplishment message
		cout << "physical device found" << endl;

		//save the found map in the timewrap
		physicalDevice.set(new PhysicalDeviceMap(*result));
	}
	else
		throw system_error(error_code(), "No available gpu found");

}

void RenderingEngineBuilder::build_device()
{
	//not using any features
	PhysicalDeviceFeatures features;

	//build the device info block
	DeviceCreateInfo deviceInfo;

	vector<char*> extensions{ "VK_KHR_swapchain" };

	//set everything that does not use any timewrap (besides the layers)
	deviceInfo
		.setPpEnabledExtensionNames(extensions.data())
		.setEnabledExtensionCount(extensions.size())
		.setPEnabledFeatures(&features)
		.setEnabledLayerCount(static_cast<uint32_t>(layers.get()->size()))
		.setPpEnabledLayerNames(layers.get()->data());

	//initialize info table fit for all requested command queues
	vector<DeviceQueueCreateInfo> familyInfoList(requestMap.size());

	//use the information from the request list/physical device family map to build all the info blocks nescessary
	int i = 0;
	for (map<QFB, vector<float>>::iterator itr = requestMap.begin(); itr != requestMap.end(); itr++, i++)
		familyInfoList[i]
			.setQueueCount(requestMap[itr->first].size())
			.setPQueuePriorities(requestMap[itr->first].data())
			.setQueueFamilyIndex(physicalDevice.get()->get_familyIndex(itr->first)->index);

	//set family information
	deviceInfo
		.setQueueCreateInfoCount(familyInfoList.size())
		.setPQueueCreateInfos(familyInfoList.data());

	//save info 
	engine.get()->device = new Device(physicalDevice.get()->device->createDevice(deviceInfo));
	
	//message
	cout << "logical device created" << endl;
	
	//set timewrap
	device.set(engine.get()->device);
}

void RenderingEngineBuilder::build_queueHandlers()
{
	//create queue handler map
	engine.get()->queueMap = new map <QFB, vector<Queue>>;

	//use the request map/physical device queue index map to generate a queue handler map
	for (map<QFB, vector<float>>::iterator itr = requestMap.begin(); itr != requestMap.end(); itr++)
		for (uint32_t i = 0; i < requestMap[itr->first].size(); i++)
			(*engine.get()->queueMap)[itr->first].push_back(device.get()->getQueue(physicalDevice.get()->get_familyIndex(itr->first)->index, i));

	cout << "queue handlers built" << endl;
	//save the queue handler map into the time wrap
	queueMap.set(engine.get()->queueMap);
}

void RenderingEngineBuilder::build_swapchain()
{
	//block with the info i need to create the swapchain
	SwapchainCreateInfoKHR createInfo;

	createInfo
		//The imageUsage bit field specifies what kind of operations I'll use the images in the swap chain for. 
		//I'am going to render directly to them, which means that they're used as color attachment. 
		//It is also possible that you'll render images to a separate image first to perform operations like post-processing.
		//In that case use a value like VK_IMAGE_USAGE_TRANSFER_DST_BIT instead 
		//and use a memory operation to transfer the rendered image to a swap chain image.
		.setImageUsage(ImageUsageFlagBits::eColorAttachment)
		.setImageArrayLayers(1)	//unless i am using stereoscopic 3d this should be 1
		.setClipped(VK_TRUE)//if i want clipped stuff (when i have a window in front of it) not to be calculated
		.setCompositeAlpha(CompositeAlphaFlagBitsKHR::eOpaque)//if i want to make the image to be blended with the os image behind window
		.setSurface(*surface.get());

	uint32_t imageCount = 3;

	//request the physical device
	PhysicalDeviceMap* physicalMap = physicalDevice.get();

	uint32_t familyIndex[]{
		(uint32_t)physicalMap->get_familyIndex(QFB::eGraphics)->index,
		(uint32_t)physicalMap->presentQueueIndex->index };


	if (physicalMap->capabilities.maxImageCount > 0 &&
		imageCount > physicalMap->capabilities.maxImageCount)
		imageCount = physicalMap->capabilities.maxImageCount;

	createInfo
		.setPreTransform(physicalMap->capabilities.currentTransform)
		.setMinImageCount(imageCount)
		.setImageFormat(physicalMap->availableFormat.format)
		.setImageColorSpace(physicalMap->availableFormat.colorSpace)
		.setImageExtent(physicalMap->availableExtent)
		.setPresentMode(physicalMap->availablePresentMode);

	if (physicalMap->presentQueueIndex->index != physicalMap->get_familyIndex(QFB::eGraphics)->index)
		createInfo
			.setImageSharingMode(SharingMode::eConcurrent)
			.setQueueFamilyIndexCount(2)
			.setPQueueFamilyIndices(familyIndex);
	else
		createInfo
			.setImageSharingMode(SharingMode::eExclusive)
			.setQueueFamilyIndexCount(0)
			.setPQueueFamilyIndices(nullptr);
	
	//create the swapchain, attach it to the engine
	engine.get()->swapchain = new SwapchainKHR(device.get()->createSwapchainKHR(createInfo));

	cout << "swapchain created" << endl;

	swapchain.set(engine.get()->swapchain);

	cout << "images created" << endl;

	engine.get()->images = new vector<Image>(device.get()->getSwapchainImagesKHR(*swapchain.get()));

	images.set(engine.get()->images);
}

void RenderingEngineBuilder::build_imageViews()
{
	ImageViewCreateInfo createInfo;
	createInfo
		.setViewType(ImageViewType::e2D)
		.setComponents(ComponentMapping())
		.setSubresourceRange((ImageSubresourceRange())
			.setAspectMask(ImageAspectFlagBits::eColor)
			.setLayerCount(1)
			.setLevelCount(1))
		.setFormat(physicalDevice.get()->availableFormat.format);

	engine.get()->imageView = new vector<ImageView>();
	this->layers;
  	int maxImages = images.get()->size();
	for (int i = 0; i < maxImages; i++) {
		createInfo.setImage(images.get()->at(i));
		engine.get()->imageView->push_back(device.get()->createImageView(createInfo));
	}

	imageView.set(engine.get()->imageView);
}



PhysicalDeviceMap* RenderingEngineBuilder::compare_devices(PhysicalDeviceMap* map1, PhysicalDeviceMap* map2)
{
	return (map1->grade > map2->grade) ? map1 : map2;
}

void RenderingEngineBuilder::build()
{
	cout << "builder initialized" << endl << endl;

	cout << "starting to build: validation layers" << endl;
	ThreadPool::add_command([this]() { build_layers(); });

	cout << "starting to build: instance" << endl;
	ThreadPool::add_command([this]() { build_instance(); });

	cout << "starting to build: physical device map" << endl;
	ThreadPool::add_command([this]() { build_physicalDevice(); });

	cout << "starting to build: surface" << endl;
	ThreadPool::add_command([this]() { build_surface(); });

	cout << "starting to build: logical device " << endl;
	ThreadPool::add_command([this]() { build_device(); });

	cout << "starting to build: queue handlers " << endl;
	ThreadPool::add_command([this]() { build_queueHandlers(); });

	cout << "starting to build: swapchain " << endl;
	ThreadPool::add_command([this]() { build_swapchain(); });

	cout << "starting to build: imageviews " << endl;
	ThreadPool::add_command([this]() { build_imageViews(); });

	/*cout << "starting to build: imageviews " << endl;
	ThreadPool::add_command([this]() { build_depthBuffer(); });
	*/
	//the window process should be in the main thread
	cout << "starting to build: window" << endl;
	build_window();

	surface.wait();
	instance.wait();
	window.wait();
	layers.wait();
	physicalDevice.wait();
	device.wait();
	queueMap.wait();
	swapchain.wait();
	imageView.wait();

	cout << endl << "building finished" << endl;
}


RenderingEngineBuilder::~RenderingEngineBuilder()
{
	physicalDevice.get()->destroy();
	physicalDevice.destroy();
}
