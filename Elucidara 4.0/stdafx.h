// Enable the WSI extensions
#if defined(__ANDROID__)
#define VK_USE_PLATFORM_ANDROID_KHR
#elif defined(__linux__)
#define VK_USE_PLATFORM_XLIB_KHR
#elif defined(_WIN32)
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#define QFB QueueFlagBits

#define SDL_MAIN_HANDLED

//rendering stuff
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <vulkan/vulkan.hpp>

//programming stuff
#include <iostream>
#include <vector>
#include <map>
#include <queue>

//delegate stuff
#include <functional>

//thread stuff
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>


//using stuff
using namespace vk;
using namespace std;

//my stuff
#include "ThreadPool.h"
#include "RenderingEngine.h"