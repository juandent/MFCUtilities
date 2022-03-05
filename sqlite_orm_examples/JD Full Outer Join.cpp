#include <sqlite_orm/sqlite_orm.h>

#include <iostream>

using std::cerr;
using std::cout;
using std::endl;






auto full_outer_join()
{
    struct Dog
    {
        std::optional<std::string> type;
        std::optional<std::string> color;
    };


    struct Cat
    {
        std::optional<std::string> type;
        std::optional<std::string> color;
    };

    using namespace sqlite_orm;

    auto storage = make_storage(
        { "full_outer.sqlite" },
        make_table("Dogs", make_column("type", &Dog::type), make_column("color", &Dog::color)),
        make_table("Cats", make_column("type", &Cat::type), make_column("color", &Cat::color)));

    storage.sync_schema();
    storage.remove_all<Dog>();
    storage.remove_all<Cat>();

    storage.insert(into<Dog>(), columns(&Dog::type, &Dog::color), values(std::make_tuple("Hunting", "Black"), std::make_tuple("Guard","Brown")));
    storage.insert(into<Cat>(), columns(&Cat::type, &Cat::color), values(std::make_tuple("Indoor", "White"), std::make_tuple("Outdoor", "Black")));


    auto left = select(columns(&Dog::type, &Dog::color, &Cat::type, &Cat::color), left_join<Cat>(using_(&Cat::color)));
    auto right = select(columns(&Dog::type, &Dog::color, &Cat::type, &Cat::color), from<Cat>(), left_join<Dog>(using_(&Dog::color)), where(is_null(&Dog::color)));




/*  FULL OUTER JOIN simulation:
 *
        SELECT d.type,
        d.color,
        c.type,
        c.color
        FROM dogs d
        LEFT JOIN cats c USING(color)
        UNION ALL
        SELECT d.type,
        d.color,
        c.type,
        c.color
        FROM cats c
        LEFT JOIN dogs d USING(color)
        WHERE d.color IS NULL;
*/
    static auto rows = storage.select(
        union_all(select(columns(&Dog::type, &Dog::color, &Cat::type, &Cat::color),
            left_join<Cat>(using_(&Cat::color))),
            select(columns(&Dog::type, &Dog::color, &Cat::type, &Cat::color), from<Cat>(),
                left_join<Dog>(using_(&Dog::color)), where(is_null(&Dog::color)))));


    return rows;
}

int main()
{
    auto rows = full_outer_join();

}