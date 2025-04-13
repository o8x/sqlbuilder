#pragma once

#include <string>
#include "helpers.h"

namespace sqlbuilder {
    enum column_type {
        TINYINT,
        SMALLINT,
        MEDIUMINT,
        INT, INTEGER,
        BIGINT,
        FLOAT,
        DOUBLE, REAL,
        DECIMAL, NUMERIC,
        DATE,
        TIME,
        DATETIME,
        TIMESTAMP,
        YEAR,
        CHAR,
        VARCHAR,
        TEXT,
        MEDIUMTEXT,
        LONGTEXT,
        BINARY,
        VARBINARY,
        BLOB,
        MEDIUMBLOB,
        LONGBLOB,
        ENUM,
        SET,
        BOOLEAN, BOOL,
        JSON,
    };

    std::string column_type_to_string(column_type t);

    struct column;
    using column_operation = std::function<void(column&)>;

    struct column {
        column_type type;
        std::string name;
        bool primary_key{};
        bool auto_increment{};
        std::optional<uint32_t> constraint;
        std::optional<uint32_t> constraint1;
        std::optional<std::vector<std::string>> constraint_list;
        bool nullable{};
        std::optional<std::string> comment;
        std::optional<std::string> default_value;

        static column_operation with_type(column_type type);
        static column_operation with_name(const std::string& name);
        static column_operation with_primary_key(const bool& value = true);
        static column_operation with_auto_increment(const bool& value = true);
        static column_operation with_nullable(const bool& value = true);
        static column_operation with_constraint(uint32_t m);
        static column_operation with_constraint(const uint32_t& m, const uint32_t& m1);
        static column_operation with_constraint_list(std::vector<std::string> list);
        static column_operation with_comment(const std::string& value);
        static column_operation with_default(const std::string& value);

        [[nodiscard]] std::string build_type_string(sql_style style) const;
        void build_by_operation(const std::vector<column_operation>& opts);
    };

    template <typename... Fn>
    column make_column(const Fn&... fns) {
        column c;
        std::ignore = std::initializer_list<int>{(fns(c), 0)...};

        return std::move(c);
    }
};
