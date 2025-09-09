#pragma once

#include <cstdint>
#include <vector>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include <laurel/utils/utils.hpp>
#include "resources.hpp"

namespace laurel::vk {

struct ExtensionInfo {
    const char* extension_name     = nullptr;
    void*       feature            = nullptr;
    bool        required           = true;
    uint32_t    spec_version       = 0;
    bool        exact_spec_version = true;
};

struct ContextInitInfo {
    std::vector<const char*>               instance_extensions      = {};
    std::vector<laurel::vk::ExtensionInfo> device_extensions        = {};
    std::vector<VkQueueFlags>              queues                   = { VK_QUEUE_GRAPHICS_BIT };
    void*                                  instance_create_info_ext = nullptr;
    const char*                            application_name         = "Laurel";
    const char*                            engine_name              = "Laurel Engine";
    uint32_t                               api_version              = VK_API_VERSION_1_3;
    VkAllocationCallbacks*                 alloc                    = nullptr;
    bool                                   enable_all_features      = true;
#if LDEBUG
    bool enable_validation_layers = true;
    bool verbose                  = true;
#else
    bool enable_validation_layers = false;
    bool verbose                  = false;
#endif
};

class Context {
  private:
    VkInstance       m_instance        = {};
    VkDevice         m_device          = {};
    VkPhysicalDevice m_physical_device = {};

    // 用于device的创建
    VkPhysicalDeviceFeatures2        m_device_features    = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2 };
    VkPhysicalDeviceVulkan11Features m_device_features_11 = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES };
    VkPhysicalDeviceVulkan12Features m_device_features_12 = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES };
    VkPhysicalDeviceVulkan13Features m_device_features_13 = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES };

    // 用于queue的创建
    std::vector<VkQueueFlags>            m_desired_queues     = { VK_QUEUE_GRAPHICS_BIT };
    std::vector<VkDeviceQueueCreateInfo> m_queue_create_infos = {};
    std::vector<laurel::vk::QueueInfo>   m_queue_infos        = {};
    std::vector<std::vector<float>>      m_queue_priorities   = {};

    VkDebugUtilsMessengerEXT m_debug_messenger = {};

  public:
    ContextInitInfo context_info = {};

  private:
    NODISCARD VkResult createInstance();
    NODISCARD VkResult createDevice();
    NODISCARD VkResult pickPhysicalDevice();
    NODISCARD bool     findQueueFamilies();

    bool filterAvailableExtensions(const std::vector<VkExtensionProperties>& available_extensions, const std::vector<const char*>& desired_extensions, std::vector<ExtensionInfo>& filtered_extensions);

    static VkResult getDeviceExtensions(VkPhysicalDevice physical_device, std::vector<VkExtensionProperties>& extension_properties);

  public:
    Context() = default;
    ~Context() {
        //assert(m_instance == VK_NULL_HANDLE;
    };

    NODISCARD VkResult init(const ContextInitInfo& init_info);
    void               reset();

    VkInstance                                getInstance() const { return m_instance; }
    VkDevice                                  getDevice() const { return m_device; }
    VkPhysicalDevice                          getPhysicalDevice() const { return m_physical_device; }
    const std::vector<laurel::vk::QueueInfo>& getQueues() const { return m_queue_infos; }
    const std::vector<laurel::vk::QueueInfo>& getQueueInfos(VkQueueFlags flags) const { return m_queue_infos; }
};

} // namespace laurel::vk