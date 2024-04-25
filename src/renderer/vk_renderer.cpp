#include <vulkan/vulkan.h>

#ifdef WINDOWS_BUILD
#include <vulkan/vulkan_win32.h>
#endif // Other OS

#include <iostream>

#define ArraySize(arr) sizeof(arr) / sizeof(arr[0])

#define VK_CHECK(result)                                        \
    if (result != VK_SUCCESS)                                   \
    {                                                           \
        std::cout << "Vulkan Error: " << result << std::endl;   \
        DebugBreak();                                           \
        return false;                                           \
    }           

struct VkContext
{
    VkInstance instance;
    VkSurfaceKHR surface;
    VkPhysicalDevice gpu;

    int graphicsIndex;
};

bool vk_init(VkContext* vkContext, void* window)
{
    // At this part of code i creating Vulkan instance and defining extensions for it
    {
        VkApplicationInfo appInfo {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Pong";
        appInfo.pEngineName = "PongEngine";
        appInfo.apiVersion = VK_API_VERSION_1_3;

        char* extensions[] = {
#ifdef WINDOWS_BUILD
            VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#endif
            VK_KHR_SURFACE_EXTENSION_NAME
        };

        VkInstanceCreateInfo instanceInfo {};
        instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceInfo.pApplicationInfo = &appInfo;
        instanceInfo.ppEnabledExtensionNames = extensions;
        instanceInfo.enabledExtensionCount = ArraySize(extensions);

        VK_CHECK(vkCreateInstance(&instanceInfo, nullptr, &vkContext->instance));
    }

    // At this part of code i creating surface for Vulkan.
    {
#ifdef WINDOWS_BUILD
        VkWin32SurfaceCreateInfoKHR surfaceInfo {};
        surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        surfaceInfo.hwnd = (HWND)window;
        surfaceInfo.hinstance = GetModuleHandleA(0);

        VK_CHECK(vkCreateWin32SurfaceKHR(vkContext->instance, &surfaceInfo, nullptr, &vkContext->surface))
#endif
    }

    // Choose GPU
    {
        vkContext->graphicsIndex = -1;

        uint32_t gpuCount = 0;

        // ToDo: Suballocation from main allocation
        VkPhysicalDevice gpus[10];

        VK_CHECK(vkEnumeratePhysicalDevices(vkContext->instance, &gpuCount, nullptr));
        VK_CHECK(vkEnumeratePhysicalDevices(vkContext->instance, &gpuCount, gpus));

        for (auto i = 0; i < gpuCount; i++)
        {
            VkPhysicalDevice gpu = gpus[i];

            uint32_t queueFamilyCount = 0;

            // ToDo: Suballocation from main allocation
            VkQueueFamilyProperties queueProperties[10];

            vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queueFamilyCount, nullptr);
            vkGetPhysicalDeviceQueueFamilyProperties(gpu, &queueFamilyCount, queueProperties);

            for (auto j = 0; j < queueFamilyCount; j++)
            {
                if (queueProperties[j].queueFlags && VK_QUEUE_GRAPHICS_BIT)
                {
                    VkBool32 surfaceSupport = VK_FALSE;
                    VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(gpu, j, vkContext->surface, &surfaceSupport));

                    if (surfaceSupport)
                    {
                        vkContext->graphicsIndex = j;
                        vkContext->gpu = gpu;
                        break;
                    }
                }
            }
        }
    }

    return true;
}