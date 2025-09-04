#pragma once

#include <functional>
#include <utility>
#include <vulkan/vulkan_core.h>

namespace laurel::vk {

class CheckError {
  private:
    using Callback = std::function<void(VkResult)>;
    Callback m_callback;

  private:
    CheckError()                             = default;
    ~CheckError()                            = default;
    CheckError(const CheckError&)            = delete;
    CheckError& operator=(const CheckError&) = delete;

  public:
    static CheckError& getInstance() {
        static CheckError instance;
        return instance;
    }
    void setCallback(Callback callback) { m_callback = std::forward<Callback>(callback); }

    void     check(VkResult result, const char* expression, const char* file, int line);
    VkResult report(VkResult result, const char* expression, const char* file, int line);
};

#define CHECK(vkFunc) \
    { \
        const VkResult result = (vkFunc); \
        laurel::vk::CheckError::getInstance().check(result, #vkFunc, __FILE__, __LINE__); \
    }

#define FAIL_RETURN(result) \
    { \
        const VkResult res = (result); \
        laurel::vk::CheckError::getInstance().report(res, #result, __FILE__, __LINE__); \
        return res; \
    }

} // namespace laurel::vk