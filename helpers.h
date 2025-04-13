#pragma once

#include <iostream>

#define sk(s, x) (s == LOWER_KEY ? to_lower(x) : to_upper(x))
#define k(x) (style_ == LOWER_KEY ? to_lower(x) : to_upper(x))

namespace sqlbuilder {
    enum sql_style {
        LOWER_KEY,
        UPPER_KEY,
    };

    enum index_type {
        SAMPLE,
        UNIQUE,
    };

    std::string join(const std::vector<std::string>& vec, const std::string& delimiter);
    // 转换字符串为大写
    std::string to_upper(const std::string& str, const std::locale& loc = std::locale());
    // 转换字符串为小写
    std::string to_lower(const std::string& str, const std::locale& loc = std::locale());
    // 清除首尾空格
    std::string trim_space(const std::string& str);
}
