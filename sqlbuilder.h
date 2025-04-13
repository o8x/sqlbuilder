#pragma once

#include <map>
#include <sstream>
#include "column.h"
#include "helpers.h"

namespace sqlbuilder {
    class base_builder {
    public:
        base_builder() = default;
        virtual ~base_builder() = default;

        void set_table_name(const std::string& table_name) {
            table_name_ = table_name;
        }

        void set_style(const sql_style style) {
            style_ = style;
        }

        // 构建 sql
        virtual std::string build() = 0;

    protected:
        std::stringstream s_{};
        sql_style style_ = LOWER_KEY;
        std::string table_name_;
    };

    class create_builder final : public base_builder {
        struct index {
            std::string name;
            index_type type;
            std::vector<std::string> columns;

            index(std::string name, const index_type type,
                  const std::vector<std::string>& columns)
                : name(std::move(name)),
                  type(type),
                  columns(columns) {
            }
        };

        std::string engine_;
        std::string table_comment_;
        std::vector<column> columns_;
        std::vector<index> indexes_;

    public:
        explicit create_builder(const std::string& table = "", const std::string& comment = "") {
            set_table_name(table);
            table_comment_ = comment;
        }

        std::string build() override;
        // 设置引擎
        create_builder& set_engine(const std::string& engine);
        // 添加组合索引
        create_builder& add_index(const std::string& columns...);
        // 添加普通索引
        create_builder& add_index(const std::string& name, const std::string& columns...);
        // 添加索引
        create_builder& add_index(const std::string& name, index_type type, const std::string& columns...);
        // 添加主键列
        create_builder& add_primary_key(const std::string& name, const column_type type = INTEGER,
                                        bool auto_increment = true);
        create_builder& add_column(const std::string& name, const column_type type, const std::string& comment = "");
        // 可为 null 的列
        create_builder& add_null_column(const std::string& name, column_type type, const std::string& comment = "");
        // 添加具有一个约束的列，例如 CHAR
        create_builder& add_column(const std::string& name, const column_type type, const uint32_t& constraint,
                                   const std::string& comment = "");
        // 添加具有两个约束的列，例如 DECIMAL
        create_builder& add_column(const std::string& name, const column_type type, const uint32_t m, const uint32_t m1,
                                   const std::string& comment = "");
        // 特殊类型
        create_builder& add_enum_column(const std::string& name, const std::vector<std::string>& list,
                                        const std::string& comment = "");

        create_builder& with_soft_delete();

        // 添加列
        create_builder& add_column(const column& column);

        // 原地构造 column 实例
        template <typename... Args>
        create_builder& emplace_column(Args&&... args) {
            add_column(sqlbuilder::make_column(std::forward<Args>(args)...));
            return *this;
        }
    };
}
