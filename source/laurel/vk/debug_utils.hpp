#pragma once

#include <vulkan/vulkan.h>

namespace laurel::vk {

class DebugUtil {
  private:
  public:
  private:
  public:
    static DebugUtil& getInstance() {
        static DebugUtil instance;
        return instance;
    }

    void init(VkDevice device);
};

} // namespace laurel::vk