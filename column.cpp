#include "column.h"
#include "helpers.h"
#include <sstream>

namespace sqlbuilder {
    std::string column_type_to_string(const column_type t) {
        switch (t) {
        case TINYINT: return "TINYINT";
        case SMALLINT: return "SMALLINT";
        case MEDIUMINT: return "MEDIUMINT";
        case INT: return "INT";
        case INTEGER: return "INTEGER";
        case BIGINT: return "BIGINT";
        case FLOAT: return "FLOAT";
        case DOUBLE: return "DOUBLE";
        case REAL: return "REAL";
        case DECIMAL: return "DECIMAL";
        case NUMERIC: return "NUMERIC";
        case DATE: return "DATE";
        case TIME: return "TIME";
        case DATETIME: return "DATETIME";
        case TIMESTAMP: return "TIMESTAMP";
        case YEAR: return "YEAR";
        case CHAR: return "CHAR";
        case VARCHAR: return "VARCHAR";
        case TEXT: return "TEXT";
        case MEDIUMTEXT: return "MEDIUMTEXT";
        case LONGTEXT: return "LONGTEXT";
        case BINARY: return "BINARY";
        case VARBINARY: return "VARBINARY";
        case BLOB: return "BLOB";
        case MEDIUMBLOB: return "MEDIUMBLOB";
        case LONGBLOB: return "LONGBLOB";
        case ENUM: return "ENUM";
        case SET: return "SET";
        case BOOLEAN: return "BOOLEAN";
        case BOOL: return "BOOL";
        case JSON: return "JSON";
        }

        return "UNKNOWN";
    }

    column_operation column::with_type(const column_type type) {
        return [&](column& c) {
            c.type = type;
        };
    }

    column_operation column::with_name(const std::string& name) {
        return [&](column& c) {
            c.name = name;
        };
    }

    column_operation column::with_primary_key(const bool& value) {
        return [&](column& c) {
            c.primary_key = value;
        };
    }

    column_operation column::with_auto_increment(const bool& value) {
        return [&](column& c) {
            c.auto_increment = value;
        };
    }

    column_operation column::with_nullable(const bool& value) {
        return [&](column& c) {
            c.nullable = value;
        };
    }

    column_operation column::with_constraint(const uint32_t m) {
        return [&](column& c) {
            c.constraint = m;
        };
    }

    column_operation column::with_constraint(const uint32_t& m, const uint32_t& m1) {
        return [&](column& c) {
            c.constraint = m;
            c.constraint1 = m1;
        };
    }

    column_operation column::with_constraint_list(std::vector<std::string> list) {
        return [&](column& c) {
            c.constraint_list = list;
        };
    }

    column_operation column::with_comment(const std::string& value) {
        return [&](column& c) {
            c.comment = value;
        };
    }

    column_operation column::with_default(const std::string& value) {
        return [&](column& c) {
            c.default_value = value;
        };
    }

    std::string column::build_type_string(const sql_style style) const {
        std::stringstream s;

        s << std::format(" {}", sk(style, column_type_to_string(type)));

        if (constraint_list.has_value()) {
            std::vector<std::string> list;
            list.reserve(constraint_list.value().size());
            for (const auto& str : constraint_list.value()) {
                list.push_back(std::format("'{}'", trim_space(str)));
            }

            s << std::format("({})", join(list, ", "));
        } else {
            if (constraint.has_value() && constraint1.has_value()) {
                s << std::format("({}, {})", constraint.value(), constraint1.value());
            } else if (constraint.has_value()) {
                s << std::format("({})", constraint.value());
            }
        }

        if (primary_key) {
            s << sk(style, " primary_key");

            if (auto_increment) {
                s << sk(style, " auto_increment");
            }
        } else {
            if (nullable) {
                s << sk(style, " null");
            } else {
                s << sk(style, " not null");
            }

            if (default_value.has_value()) {
                s << std::format(" {} {}", sk(style, "default"), default_value.value());
            }
        }

        if (comment.has_value()) {
            s << std::format(" {} '{}'", sk(style, "comment"), comment.value());
        }

        return s.str();
    }

    void column::build_by_operation(const std::vector<column_operation>& opts) {
        for (const column_operation& opt : opts) {
            opt(*this);
        }
    }
}
