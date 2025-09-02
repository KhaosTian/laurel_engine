#pragma once

#include <cstdint>
#include <vector>

#include <vulkan/vulkan.h>
#include "vulkan/vulkan_core.h"

#include "laurel/utils/utils.hpp"
#include "resources.hpp"

namespace laurel::vk {

struct ExtensionInfo {
    const char* extension_name = nullptr;
    void*       feature        = nullptr;
    bool        required       = true;
};

struct ContextInitInfo {
    std::vector<const char*>               instance_extensions = {};
    std::vector<laurel::vk::ExtensionInfo> device_extensions   = {};
    std::vector<VkQueueFlags>              queues              = { VK_QUEUE_GRAPHICS_BIT };
    const char*                            application_name    = "Laurel";
    const char*                            engine_name         = "Laurel Engine";
    uint32_t                               api_version         = VK_API_VERSION_1_3;
    bool                                   enable_all_features = true;
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

    // 用于queue的创建
    std::vector<VkQueueFlags>            m_desiredQueues      = {};
    std::vector<VkDeviceQueueCreateInfo> m_queue_create_infos = {};
    std::vector<laurel::vk::QueueInfo>   m_queue_infos        = {};
    std::vector<std::vector<float>>      m_queue_priorities   = {};

  public:
    ContextInitInfo context_info = {};

  private:
    NODISCARD VkResult createInstance();
    NODISCARD VkResult createDevice();
    NODISCARD VkResult pickPhysicalDevice();
    NODISCARD bool     findQueueFamilies();

    static VkResult getDeviceExtensions(VkPhysicalDevice physical_device, std::vector<VkExtensionProperties>& extension_properties);

  public:
    Context() = default;
    ~Context() {
        //assert(m_instance == VK_NULL_HANDLE;
    };

    NODISCARD VkResult init(const ContextInitInfo& init_info);
    void               deinit();

    VkInstance                                getInstance() const { return m_instance; }
    VkDevice                                  getDevice() const { return m_device; }
    VkPhysicalDevice                          getPhysicalDevice() const { return m_physical_device; }
    const std::vector<laurel::vk::QueueInfo>& getQueues() const { return m_queue_infos; }
    const std::vector<laurel::vk::QueueInfo>& getQueueInfos(VkQueueFlags flags) const { return m_queue_infos; }
};

} // namespace laurel::vk