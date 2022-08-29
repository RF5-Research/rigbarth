#include "logging.hpp"

void vlog(LogType type, fmt::string_view format,
    fmt::format_args args
) {
    std::string prefix;
    switch (type)
    {
    case LogType::Info:
    {
        prefix = fmt::format(fmt::fg(fmt::color::white), "[{}: INFO]", Constants::PLUGIN_NAME);
        break;
    }
    case LogType::Warning:
    {
        prefix = fmt::format(fmt::fg(fmt::color::yellow), "[{}: WARNING]", Constants::PLUGIN_NAME);
        break;
    }
    case LogType::Error:
    {
        prefix = fmt::format(fmt::fg(fmt::color::red), "[{}: ERROR]", Constants::PLUGIN_NAME);
        break;
    }
    }
    fmt::print("{}\t{}\n", prefix, fmt::vformat(format, args));
}
