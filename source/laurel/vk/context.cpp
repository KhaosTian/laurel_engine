#include <volk/volk.h>
#include <vulkan/vk_enum_string_helper.h>

#include <laurel/utils/logger.hpp>

#include "context.hpp"
#include "check_error.hpp"
#include "debug_utils.hpp"
#include "vulkan/vk_platform.h"
#include "vulkan/vulkan_core.h"

// vulkan 错误回调
static VKAPI_ATTR VkBool32 VKAPI_CALL VkContextDebugReport(VkDebugUtilsMessageSeverityFlagBitsEXT      message_severity,
                                                           VkDebugUtilsMessageTypeFlagsEXT             message_type,
                                                           const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
                                                           void*                                       user_data) {
    const spdlog::level::level_enum log_level = (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) != 0     ? spdlog::level::err
                                                : (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) != 0 ? spdlog::level::warn
                                                                                                                            : spdlog::level::info;

    const char* level_string = (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) != 0     ? "error"
                               : (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) != 0 ? "warning"
                                                                                                           : "info";

    laurel::utils::Logger::getInstance().get_logger()->log(log_level, "Vulkan Debug: {} - {} - {}", level_string, callback_data->pMessageIdName, callback_data->pMessage);
    return VK_FALSE;
}

VkResult laurel::vk::Context::init(const laurel::vk::ContextInitInfo& init_info) {
    context_info = init_info;

    VK_REPORT(volkInitialize());

    {
        VK_REPORT(createInstance());
        VK_REPORT(pickPhysicalDevice());
        VK_REPORT(createDevice());

        laurel::vk::DebugUtil::getInstance().init(m_device); // 初始化debug util
    }
}