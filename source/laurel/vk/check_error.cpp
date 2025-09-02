#pragma once

#include <cassert>
#include <cstdlib>
#include <vulkan/vk_enum_string_helper.h>

#include <laurel/utils/logger.hpp>

#include "check_error.hpp"

namespace laurel::vk {
void CheckError::check(VkResult result, const char* expression, const char* file, int line) {
    if (result < VK_SUCCESS) {
        LOGE("Vulkan error: {} at {}:{} in expression `{}`", string_VkResult(result), file, line, expression);
        assert((result >= VK_SUCCESS));
        exit(EXIT_FAILURE);
    }
}

VkResult CheckError::report(VkResult result, const char* expression, const char* file, int line) {
    if (result < VK_SUCCESS) {
        LOGE("Vulkan error: {} at {}:{} in expression `{}`", string_VkResult(result), file, line, expression);
    }
    return result;
}

} // namespace laurel::vk