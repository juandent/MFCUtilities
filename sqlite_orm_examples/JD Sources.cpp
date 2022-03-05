#include <sqlite_orm/sqlite_orm.h>

#include <iostream>

using std::cerr;
using std::cout;
using std::endl;



struct Table {
    int a = 0;
    std::optional<std::string> b;
    int c = 0;
};
struct Employee {
    int id;
    std::string name;
    int age;
    std::optional<std::string> address;  //  optional
    std::optional<double> salary;  //  optional

    bool operator==(const Employee& other) const {
        return this->id == other.id && this->name == other.name && this->age == other.age &&
            this->address == other.address && this->salary == other.salary;
    }
};

int main()
{
    using namespace sqlite_orm;

    auto storage = make_storage(
        {},
        make_table("t", make_column("a", &Table::a), make_column("b", &Table::b), make_column("c", &Table::c)),
        make_table("Employee", make_column("id", &Employee::id, primary_key()), make_column("Address", &Employee::address), 
            make_column("name", &Employee::name), make_column("age", &Employee::age), make_column("salary", &Employee::salary)));
    storage.sync_schema();

    storage.insert(Table{ 1,"A",3 });
    storage.insert(Table{ 3,"b",5 });
    storage.insert(Table{ 3, std::nullopt,2 });

    //  create employees..
    Employee paul{ -1, "Paul", 32, "California", 20000.0 };
    Employee allen{ -1, "Allen", 25, "Texas", 15000.0 };
    Employee teddy{ -1, "Teddy", 23, "Norway", std::nullopt};
    Employee mark{ -1, "Mark", 25, "Rich-Mond", 65000.0 };
    Employee david{ -1, "David", 27, "Texas", 85000.0 };
    Employee kim{ -1, "Kim", 22, "South-Hall", 45000.0 };
    Employee james{ -1, "James", 24, "Houston", 10000.0 };

    //  insert employees. `insert` function returns id of inserted object..
    paul.id = storage.insert(paul);
    allen.id = storage.insert(allen);
    teddy.id = storage.insert(teddy);
    mark.id = storage.insert(mark);
    david.id = storage.insert(david);
    kim.id = storage.insert(kim);
    james.id = storage.insert(james);



    {
        auto rows0 = storage.select(10 / 5);
        auto rows = storage.select(columns(10/5, 2*4));
        std::ignore = rows;
    }
    {
        auto rows = storage.select(columns(&Table::a, &Table::b), from<Table>());
        std::ignore = rows;
    }
    {
        auto rows = storage.select(asterisk<Table>());
        std::ignore = rows;
    }
    {
        auto objects = storage.get_all<Table>();

        std::ignore = objects;
    }
    {
        auto rows = storage.select(columns(&Table::a, &Table::b), multi_order_by(order_by(&Table::a).asc(), order_by(&Table::b).desc()));

        std::ignore = rows;
    }
    {
        auto objects = storage.get_all<Table>(multi_order_by(order_by(&Table::a).asc(), order_by(&Table::b).desc()));
        std::ignore = objects;
    }
    {
	    auto rows = storage.select(columns(&Table::a, &Table::b), order_by(&Table::b).desc().collate_rtrim().collate_nocase());
        std::ignore = rows;
    }
    {
        auto objects = storage.get_all<Table>(order_by(&Table::b).desc());
        std::ignore = objects;
    }
    {
        auto rows = storage.select(columns(&Table::a, &Table::b), order_by(2).desc().collate_rtrim().collate_nocase());
        std::ignore = rows;

    }
    {
        auto rows = storage.select(distinct(&Employee::address));
        std::ignore = rows;
    }
    return 0;
}