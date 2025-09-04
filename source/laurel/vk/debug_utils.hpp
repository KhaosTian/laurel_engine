#pragma once

#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan.h>

namespace laurel::vk {

class DebugUtil {
  private:
    VkDevice m_device;

  public:
  private:
    DebugUtil()                            = default;
    ~DebugUtil()                           = default;
    DebugUtil(const DebugUtil&)            = delete;
    DebugUtil& operator=(const DebugUtil&) = delete;

  public:
    static DebugUtil& getInstance() {
        static DebugUtil instance;
        return instance;
    }

    void init(VkDevice device) { m_device = device; }
};

} // namespace laurel::vk