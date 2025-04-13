#include <iostream>

#include "sqlbuilder.h"

void ddl_example() {
    sqlbuilder::create_builder b{"users", "用户表"};
    b.set_style(sqlbuilder::LOWER_KEY);
    b.set_engine("InnoDB");

    b.add_index("gender")
     .add_index("idx_date_of_birth", sqlbuilder::UNIQUE, "date_of_birth")
     .add_primary_key("id")
     .add_primary_key("info_id", sqlbuilder::INTEGER, false)
     .add_column("username", sqlbuilder::VARCHAR)
     .add_column("email", sqlbuilder::VARCHAR, 100)
     .add_column("password_hash", sqlbuilder::VARCHAR, 255)
     .add_column("full_name", sqlbuilder::VARCHAR, 100)
     .add_enum_column("gender", {"Male", "Female", "Other"})
     .add_column("date_of_birth", sqlbuilder::DATE, "生日")
     .add_column("registration_date", sqlbuilder::DATE, "注册时间")
     .add_column("phone_number", sqlbuilder::VARCHAR, 15)
     .add_column("address", sqlbuilder::TEXT)
     .add_column("age", sqlbuilder::INT)
     .add_column("intro", sqlbuilder::VARCHAR, 200, "个人介绍")
     .emplace_column(
         sqlbuilder::column::with_type(sqlbuilder::VARCHAR),
         sqlbuilder::column::with_name("complex_column"),
         sqlbuilder::column::with_primary_key(),
         sqlbuilder::column::with_nullable(),
         sqlbuilder::column::with_constraint(200),
         sqlbuilder::column::with_constraint_list({"A", "B", "C", "D"}),
         sqlbuilder::column::with_comment("评论"),
         sqlbuilder::column::with_default("默认值")
     );

    std::cout << b.with_soft_delete().build() << std::endl;
}

int main(int argc, char* argv[]) {
    ddl_example();
}
