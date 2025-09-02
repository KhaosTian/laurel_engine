#pragma once

#include <vulkan/vulkan_core.h>

namespace laurel::vk {

struct QueueInfo {
    uint32_t family_index = ~0u;
    uint32_t queue_index  = ~0u;
    VkQueue  queue        = {};
};

} // namespace laurel::vk