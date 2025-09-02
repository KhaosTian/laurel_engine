#include "logger.hpp"

namespace laurel::utils {

void Logger::init(size_t level, const std::optional<std::string>& log_file) {
    std::vector<spdlog::sink_ptr> sinks;

    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_pattern("%^[%Y-%m-%d %T%e] %v%$");
    sinks.push_back(console_sink);

    if (log_file) {
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_file.value(), true);
        file_sink->set_pattern("[%Y-%m-%d %T%e] %v");
        sinks.push_back(file_sink);
    }

    m_spdlogger = std::make_shared<spdlog::logger>("LOG", begin(sinks), end(sinks));
    spdlog::register_logger(m_spdlogger);
    m_spdlogger->set_level(static_cast<spdlog::level::level_enum>(level));
    m_spdlogger->flush_on(spdlog::level::trace);
}

} // namespace laurel::utils