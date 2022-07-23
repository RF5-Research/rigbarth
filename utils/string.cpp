#include "pch-il2cpp.h"
#include <ranges>
#include <algorithm>
#include <string>

//https://stackoverflow.com/questions/11635/case-insensitive-string-comparison-in-c/4119881#4119881
namespace utils
{
    namespace string
    {
        bool iequals(const std::string_view& lhs, const std::string_view& rhs) {
            auto to_lower{ std::ranges::views::transform(std::tolower) };
            return std::ranges::equal(lhs | to_lower, rhs | to_lower);
        }
    }
}
