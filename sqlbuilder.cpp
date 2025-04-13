#include "sqlbuilder.h"
#include <iomanip>
#include "helpers.h"

namespace sqlbuilder {
    std::string create_builder::build() {
        int name_width = 0;
        for (const column& c : columns_) {
            if (c.name.length() > name_width) {
                name_width = c.name.length();
            }
        }

        s_ << std::format("{} `{}` (", k("create table"), table_name_) << std::endl;
        for (auto it = columns_.begin(); it != columns_.end(); ++it) {
            const column& c = *it;

            s_ << "    " << std::setw(name_width + 2) << std::left << std::format("`{}`", c.name);
            s_ << c.build_type_string(style_);

            if (std::next(it) != columns_.end()) {
                s_ << ", ";
            }

            s_ << std::endl;
        }

        s_ << std::format(") {}={} {}='{}';", k("engine"), engine_, k("comment"), table_comment_);
        s_ << std::endl;

        if (!indexes_.empty()) {
            s_ << std::endl;
        }

        for (const index& i : indexes_) {
            std::string start_key = i.type == UNIQUE ? k("CRATE UNIQUE INDEX") : k("CRATE INDEX");
            s_ << std::format("{} {} {} `{}` ({});", start_key, i.name, k("ON"), table_name_, join(i.columns, ","));
            s_ << std::endl;
        }

        return s_.str();
    }

    create_builder& create_builder::set_engine(const std::string& engine) {
        engine_ = engine;

        return *this;
    }

    create_builder& create_builder::add_index(const std::string& columns, ...) {
        const std::string name = std::format("{}_index", join(std::vector{columns}, "_"));
        add_index(name, SAMPLE, columns);

        return *this;
    }

    create_builder& create_builder::add_index(const std::string& name, const std::string& columns, ...) {
        add_index(name, SAMPLE, columns);

        return *this;
    }

    create_builder& create_builder::add_index(const std::string& name, index_type type,
                                              const std::string& columns...) {
        indexes_.emplace_back(name, type, std::vector{columns});

        return *this;
    }

    create_builder& create_builder::add_column(const std::string& name, const column_type type,
                                               const std::string& comment) {
        columns_.push_back(column{
            .type = type,
            .name = name,
            .comment = comment.empty() ? std::nullopt : std::make_optional(comment),
        });

        return *this;
    }

    create_builder& create_builder::add_null_column(const std::string& name, const column_type type,
                                                    const std::string& comment) {
        columns_.push_back(column{
            .type = type,
            .name = name,
            .nullable = true,
            .comment = comment.empty() ? std::nullopt : std::make_optional(comment),
        });

        return *this;
    }

    create_builder& create_builder::add_primary_key(const std::string& name, const column_type type,
                                                    const bool auto_increment) {
        columns_.push_back(column{
            .type = type,
            .name = name,
            .primary_key = true,
            .auto_increment = auto_increment,
        });

        return *this;
    }

    create_builder& create_builder::add_column(const column& column) {
        columns_.push_back(column);
        return *this;
    }

    create_builder& create_builder::add_column(const std::string& name, const column_type type,
                                               const uint32_t& constraint,
                                               const std::string& comment) {
        columns_.push_back(column{
            .type = type,
            .name = name,
            .constraint = constraint,
            .comment = comment.empty() ? std::nullopt : std::make_optional(comment),
        });

        return *this;
    }

    create_builder& create_builder::add_column(const std::string& name, const column_type type, const uint32_t m,
                                               const uint32_t m1, const std::string& comment) {
        columns_.push_back(column{
            .type = type,
            .name = name,
            .constraint = m,
            .constraint1 = m1,
            .comment = comment.empty() ? std::nullopt : std::make_optional(comment),
        });

        return *this;
    }

    create_builder& create_builder::add_enum_column(const std::string& name, const std::vector<std::string>& list,
                                                    const std::string& comment) {
        columns_.push_back(column{
            .type = ENUM,
            .name = name,
            .constraint_list = list,
            .comment = comment.empty() ? std::nullopt : std::make_optional(comment),
        });

        return *this;
    }

    create_builder& create_builder::with_soft_delete() {
        columns_.push_back(make_column(
            column::with_type(TIMESTAMP),
            column::with_name("created_at"),
            column::with_default(k("CURRENT_TIMESTAMP"))
        ));

        columns_.push_back(make_column(
            column::with_type(TIMESTAMP),
            column::with_name("update_at"),
            column::with_default(k("CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP"))
        ));

        columns_.push_back(make_column(
            column::with_type(TIMESTAMP),
            column::with_name("deleted_at"),
            column::with_nullable()
        ));

        return *this;
    }
}
