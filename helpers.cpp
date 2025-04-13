#include "helpers.h"

namespace sqlbuilder {
    std::string join(const std::vector<std::string>& vec, const std::string& delimiter) {
        std::string result;

        for (size_t i = 0; i < vec.size(); ++i) {
            result += vec[i];
            // 除了最后一个元素外，添加分隔符
            if (i < vec.size() - 1) {
                result += delimiter;
            }
        }

        return result;
    }

    std::string to_upper(const std::string& str, const std::locale& loc) {
        std::string upper_str;
        for (const char s : str) {
            upper_str += std::toupper(s, loc);
        }
        return upper_str;
    }

    std::string to_lower(const std::string& str, const std::locale& loc) {
        std::string upper_str;
        for (const char s : str) {
            upper_str += std::tolower(s, loc);
        }
        return upper_str;
    }

    std::string trim_space(const std::string& str) {
        const size_t first = str.find_first_not_of(' ');
        if (first == std::string::npos) {
            return ""; // 字符串全是空格
        }

        const size_t last = str.find_last_not_of(' ');
        return str.substr(first, (last - first + 1));
    }
}
