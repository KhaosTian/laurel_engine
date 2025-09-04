#include <cstdint>
#include <volk/volk.h>
#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan_core.h>

#include <laurel/utils/logger.hpp>

#include "context.hpp"
#include "check_error.hpp"

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
    }
}

void laurel::vk::Context::reset() {
    if (m_device) {
        vkDestroyDevice(m_device, context_info.alloc);
    }

    if (m_instance) {
        if (m_debug_messenger && vkDestroyDebugUtilsMessengerEXT) {
            vkDestroyDebugUtilsMessengerEXT(m_instance, m_debug_messenger, context_info.alloc);
            m_debug_messenger = VK_NULL_HANDLE;
        }
        vkDestroyInstance(m_instance, context_info.alloc);
    }

    m_device   = VK_NULL_HANDLE;
    m_instance = VK_NULL_HANDLE;
}

VkResult laurel::vk::Context::createInstance() {
    VkApplicationInfo app_info {
        .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName   = context_info.application_name,
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName        = context_info.engine_name,
        .engineVersion      = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion         = context_info.api_version,
    };

    std::vector<const char*> layers;
    if (context_info.enable_validation_layers) {
        layers.push_back("VK_LAYER_KHRONOS_validation");
    }

    VkInstanceCreateInfo create_info {
        .sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext            = context_info.instance_create_info_ext,
        .pApplicationInfo = &app_info,
        // 设置验证层
        .enabledLayerCount   = static_cast<uint32_t>(layers.size()),
        .ppEnabledLayerNames = layers.data(),
        // 设置instance扩展
        .enabledExtensionCount   = static_cast<uint32_t>(context_info.instance_extensions.size()),
        .ppEnabledExtensionNames = context_info.instance_extensions.data(),
    };

    // 创建instance
    VkResult result = vkCreateInstance(&create_info, context_info.alloc, &m_instance);
    if (result != VK_SUCCESS) {
        LOGE("Failed to create Vulkan instance: {}", string_VkResult(result));
        return result;
    }
    volkLoadInstance(m_instance);

    // 创建debug util messenger
    if (context_info.enable_validation_layers) {
        if (vkCreateDebugUtilsMessengerEXT) {
            VkDebugUtilsMessengerCreateInfoEXT messenger_create_info { .sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
                                                                       .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
                                                                       .messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
                                                                       .pfnUserCallback = VkContextDebugReport };
            VK_CHECK(vkCreateDebugUtilsMessengerEXT(m_instance, &messenger_create_info, context_info.alloc, &m_debug_messenger));
        } else {
            LOGW("Cannot set up debug messenger!");
        }
    }

    return VK_SUCCESS;
}

VkResult laurel::vk::Context::pickPhysicalDevice() {
    if (m_instance == VK_NULL_HANDLE) {
        LOGE("Instance is not created!");
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    uint32_t device_count = 0;
    VK_REPORT(vkEnumeratePhysicalDevices(m_instance, &device_count, nullptr));
}

VkResult laurel::vk::Context::createDevice() {
    return VK_SUCCESS;
}