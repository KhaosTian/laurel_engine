#pragma once

#include <vector>
#include <optional>
#include <set>

#include <vulkan/vulkan_core.h>

namespace laurel::vk {

struct QueueInfo {
    uint32_t family_index = ~0u;
    uint32_t queue_index  = ~0u;
    VkQueue  queue        = {};
};

struct QueueFamilyIndices {
    std::optional<uint32_t> graphics = {};
    std::optional<uint32_t> compute  = {};

    bool isComplete() const { return graphics.has_value() && compute.has_value(); }
};

} // namespace laurel::vk