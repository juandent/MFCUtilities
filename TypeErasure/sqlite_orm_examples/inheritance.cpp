#include <sqlite_orm/sqlite_orm.h>
#include <string>
#include <vector>
#include <iostream>

using namespace sqlite_orm;
using namespace std::literals;


struct TbBase {
    int id;
    bool deleted;
    std::string firstName;
    std::string lastName;
};

struct XUser : public TbBase {
    // using mine = std::string;

};

auto storage = sqlite_orm::make_storage("",
    make_table<XUser>("tb_xuser",
        make_column("id", &XUser::id, autoincrement(), primary_key()),
        make_column("deleted", &XUser::deleted),
        make_column("first_name", &XUser::firstName),
        make_column("last_name", &XUser::lastName),
        sqlite_orm::unique(column<XUser>(&XUser::firstName), column<XUser>(&XUser::lastName))
    ));

int main()

{

    for (const auto& res : storage.sync_schema()) {

        (void)res;

        // check sync results here..

    }

}

