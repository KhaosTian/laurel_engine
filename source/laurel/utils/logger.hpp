#pragma once

#include <optional>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace laurel::utils {
class Logger {
  private:
    std::shared_ptr<spdlog::logger> m_spdlogger;

  private:
    Logger()                         = default;
    ~Logger()                        = default;
    Logger(const Logger&)            = delete;
    Logger& operator=(const Logger&) = delete;

  public:
    static Logger& getInstance() noexcept {
        static Logger instance;
        return instance;
    }

  public:
    std::shared_ptr<spdlog::logger>& get_logger() { return m_spdlogger; }

    void shutdown() { spdlog::shutdown(); }

    void init(size_t level = spdlog::level::trace, const std::optional<std::string>& log_file = std::nullopt);
};

} // namespace laurel::utils

#define LOGD(...) ::laurel::utils::Logger::getInstance().get_logger()->debug(__VA_ARGS__)
#define LOGW(...) ::laurel::utils::Logger::getInstance().get_logger()->warn(__VA_ARGS__)
#define LOGE(...) ::laurel::utils::Logger::getInstance().get_logger()->error(__VA_ARGS__)
