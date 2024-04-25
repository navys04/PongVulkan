#include <vulkan/vulkan.h>

struct VkContext
{
    VkInstance instance;
};

bool vk_init(VkContext* vkContext)
{
    VkApplicationInfo appInfo {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Pong";
    appInfo.pEngineName = "PongEngine";
    appInfo.apiVersion = VK_API_VERSION_1_3;

    VkInstanceCreateInfo instanceInfo {};
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pApplicationInfo = &appInfo;

    const VkResult result = vkCreateInstance(&instanceInfo, nullptr, &vkContext->instance);

    if (result != VK_SUCCESS)
    {
        return false;
    }

    return true;
}