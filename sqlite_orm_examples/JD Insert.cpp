#include <sqlite_orm/sqlite_orm.h>
#include <string>
#include <vector>
#include <iostream>
#include <cassert>

using std::cout;
using std::endl;

struct User {
    int id;
    std::string name;
    std::vector<char> hash;  //  binary format
};
struct UserBackup {
    int id = 0;
    std::string name;
    std::vector<char> hash;  //  binary format
};


int main(int, char**) {
    using namespace sqlite_orm;
    auto storage = make_storage("blob_name.sqlite",
        make_table("users",
            make_column("id", &User::id, primary_key(), autoincrement()),
            make_column("name", &User::name),
			make_column("hash", &User::hash)),
        make_table("user_backup",
            make_column("id", &UserBackup::id, primary_key(), autoincrement()),
            make_column("name", &UserBackup::name),
            make_column("hash", &UserBackup::hash)));

    storage.sync_schema();
    storage.remove_all<User>();

    UserBackup alex{
        -1,
        "Juan",
        {0x10, 0x20, 0x30, 0x40},
    };
    storage.insert(alex);
    storage.insert(alex);

    auto statement = storage.prepare(insert(into<User>(), select(columns(&UserBackup::id, &UserBackup::name, &UserBackup::hash))));
    auto sql = statement.expanded_sql();
	storage.execute(statement);
    auto r = storage.select(last_insert_rowid());

    auto rows2 = storage.select(date("now"));
    auto users = storage.get_all<User>();

    {
        auto statement = storage.prepare(insert(into<User>(), select(columns(&UserBackup::id, &UserBackup::hash, &UserBackup::name))));
        auto sql = statement.expanded_sql();
        storage.execute(statement);
        auto r = storage.select(last_insert_rowid());
        auto users = storage.get_all<User>();

    }

    std::ignore = users;
}
