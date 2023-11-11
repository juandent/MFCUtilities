#pragma once
#include <string>
#include <sqlite_orm/sqlite_orm.h>

namespace version1 {

    using namespace sqlite_orm;

    struct User {
        int id = 0;
        std::string name;
    };

    auto getStorage(const DbConnection& con)
    {
        auto storage = make_storage(con,
            make_table("users",
                make_column("id", &User::id, primary_key()),
                make_column("name", &User::name)));
        return storage;
    }
}

namespace version2 {
    using namespace sqlite_orm;

    struct User {
        int id = 0;
        std::string name;
        std::string email;
    };

    auto getStorage(const DbConnection& con)
    {
        auto storage = make_storage(con,
            make_table("users",
                make_column("id", &User::id, primary_key()),
                make_column("name", &User::name),
                make_column("email", &User::email)));
        return storage;
    }
}

namespace version3 {
    using namespace sqlite_orm;

    struct User {
        int id = 0;
        std::string first_name;
        std::string last_name;
        std::string email;
    };

    auto getStorage(const DbConnection& con)
    {
        auto storage = make_storage(con,
            make_table("users",
                make_column("id", &User::id, primary_key()),
                make_column("first_name", &User::first_name),
                make_column("last_name", &User::last_name),
                make_column("email", &User::email)));
        return storage;
    }
}
