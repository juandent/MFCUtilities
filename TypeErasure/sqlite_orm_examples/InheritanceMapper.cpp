#include <sqlite_orm/sqlite_orm.h>
#include <string>
#include <vector>
#include <iostream>
#include "DateBinding.h"


using namespace sqlite_orm;
using namespace std::literals;

struct Team;

struct Player {
    int id;
    std::string name;
    int type_id;
    int team_id;
    Player() = default;
    Player(int id, const std::string& name, int type_id, int team_id) :
        id(id),
        name(name),
        type_id(type_id),
        team_id(team_id)
    {
    }
    Team team() const;
    void team(const Team& t);

    static std::vector<Player> get_all(auto whereClause);
    static std::vector<std::unique_ptr<Player>> get_all_polymorphic(auto whereClause);

    //  REQUIRED for polymorphic collection
    virtual ~Player() = default;

    enum TypeId {
        _Footballer = 10,
        _Cricketer = 11
    };
};


// type == 10
struct Footballer : Player {
    Footballer() = default;

    Footballer(int id, const std::string& name, int team_id, const std::string& club ) : Player{id,name, Player::_Footballer,team_id}, club{club}
    {}

    std::string club;
};

// type == 11
struct Cricketer : Player {
    Cricketer() = default;

    Cricketer(int id, const std::string& name, int team_id, double average) : Player{ id,name, Player::_Cricketer,team_id }, batting_average{ average}
    {}

    double batting_average;
};

struct Team {
    int id = 0;
    std::string name;
    std::chrono::sys_days started;
    int type_id;

    enum TeamType {
        _FootballTeam = 1,
        _CricketTeam = 2
    };
};



auto storage = make_storage("Inheritance.sqlite",
    make_table("Players",   // Not to be used for storage only to silence the compiler
        make_column("id", &Player::id),
        make_column("name", &Player::name),
        make_column("type", &Player::type_id),
        make_column("team_id", &Player::team_id),
        primary_key(&Player::id)),
    make_table<Footballer>("Footballers",
        make_column("id", &Footballer::id, primary_key().autoincrement()),
        make_column("name", &Footballer::name),
        make_column("type_id", &Footballer::type_id, generated_always_as((int)Player::_Footballer)),
        make_column("team_id", &Footballer::team_id),
        make_column("club", &Footballer::club),
    foreign_key(&Footballer::team_id).references(&Team::id).on_delete.cascade().on_update.cascade()),
    make_table<Cricketer>("Cricketer",
        make_column("id", &Cricketer::id, primary_key().autoincrement()),
        make_column("name", &Cricketer::name),
        make_column("type_id", &Cricketer::type_id, generated_always_as((int)Player::_Cricketer)),
        make_column("team_id", &Cricketer::team_id),
        make_column("batting_average", &Cricketer::batting_average),
    foreign_key(&Cricketer::team_id).references(&Team::id).on_delete.cascade().on_update.cascade()),
    make_table("Teams",
        make_column("id", &Team::id, primary_key().autoincrement()),
        make_column("name", &Team::name),
        make_column("started", &Team::started),
        make_column("type_id", &Team::type_id)));


int next_id() {
    //storage.select(max()) 
    return 1;
}


Team Player::team() const
{
    Team team = storage.get<Team>(this->team_id);
    return team;
}

void Player::team(const Team& t)
{
    assert(t.id > 0);
    this->team_id = t.id;
}

std::vector<Player> Player::get_all(auto whereClause) {

    auto q1 = select(columns(column<Footballer>(&Player::id), column<Footballer>(&Player::name), column<Footballer>(&Player::type_id),
        column<Footballer>(&Player::team_id)), where(whereClause));
    auto q2 = select(columns(column<Cricketer>(&Player::id), column<Cricketer>(&Player::name), column<Cricketer>(&Player::type_id),
        column<Cricketer>(&Player::team_id)) , where(whereClause));

    auto s = storage.dump(q1);
    auto rows = storage.select(union_(q1, q2));
    
    std::vector<Player> players;
    
    std::transform(rows.begin(), rows.end(), std::back_inserter(players), [](std::tuple<int, std::string, int,int >& row) {
        Player p{ std::get<0>(row), std::get<1>(row), std::get<2>(row), std::get<3>(row)};
        return p;
    });

    for (auto& v : players) {
        auto s = storage.dump(v);
        std::cout << s << "\n";
        std::ignore = v;
    }
    return players;
}

// struct 

std::vector<std::unique_ptr<Player>> Player::get_all_polymorphic(auto whereClause) {

    auto fbs = storage.get_all_pointer<Footballer>(where(whereClause));
    auto cs = storage.get_all_pointer<Cricketer>(where(whereClause));
    std::vector<std::unique_ptr<Player>> vec;
    for(auto&& o : fbs) {
        vec.push_back(std::move(o));
    }
    for (auto&& o : cs) {
        vec.push_back(std::move(o));
    }
    return vec;
}

using namespace std::chrono;
using namespace std::chrono_literals;

int main(int, char**) {

    constexpr static auto xxxx = NAN;
    constinit static bool ok = xxxx > 4.5;
    ok = xxxx == 4.5;
    ok = xxxx < 4.5;
    constinit static auto yyyy = 1e+300;
    yyyy *= yyyy;
    bool eq = yyyy == INFINITY;

    std::cout << "Hi\n";

    ::remove(storage.filename().c_str());

    try
    {
        storage.sync_schema(false);
        storage.remove_all<Footballer>();
        storage.remove_all<Cricketer>();
        storage.remove_all<Team>();


        year_month_day year_ago = today();
        year_ago -= std::chrono::years{ 1 };
        sys_days yago = year_ago;
        yago -= std::chrono::days{ 2 };
        year_month_day yyago = yago;
    
        Team team{ 1, "Liga Alajuelense", year_ago, Team::_FootballTeam };
        storage.replace(team);

        Team cteam{ 2, "Los Crickets", yyago, Team::_CricketTeam};
        storage.replace(cteam);

        Footballer f1( 1, "Footballer main player"s, 1, "Football club");
        storage.replace(f1);

        Cricketer c1(2, "Cricketer main player"s, 2, 35.77);
        storage.replace(c1);

        // storage.replace(into<Footballer>(), columns(column<Footballer>(&Player::id), column<Footballer>(&Player::name),column<Footballer>(&Player::team_id),   &Footballer::club), 
        //     values(std::make_tuple(1, "Footballer main player"s, 1,  "Football club"s)));
        // storage.replace(into<Cricketer>(), columns(column<Cricketer>(&Player::id), column<Cricketer>(&Player::name), column<Cricketer>(&Player::team_id), &Cricketer::batting_average), 
        //     values(std::make_tuple(1, "Cricketer main player"s,2, 35.77)));

        auto ff = storage.get_all<Footballer>();
        // auto fff = ff[0];

        //auto ret = storage.insert(fff);

        //storage.replace((Player)ff[0]);

        auto cc = storage.get_all<Cricketer>();

        auto vec = Player::get_all(true);
        for (auto& v : vec) {
            auto s = storage.dump(v);
            s += " "s + v.team().name;
            std::cout << s << "\n";
            std::ignore = v;
        }

        auto ve = Player::get_all_polymorphic(true);
        for (auto&& v : ve) {
            switch (v->type_id) {
            case Player::_Footballer: {
                Footballer* p = (Footballer*)v.get();
                auto s = storage.dump(*p);
                s += " "s + p->team().name;
                std::cout << s << "\n";
            } break;
            case Player::_Cricketer: {
                Cricketer* p = (Cricketer*)v.get();
                auto s = storage.dump(*p);
                s += " "s + p->team().name;
                std::cout << s << "\n";
            } break;
            }
        }
#if 0
        auto f = storage.get<Footballer>(1);

        f.team(cteam);
        storage.replace(f);
        // storage.remove_all<Team>();
        storage.remove<Team>(team.id);   // NO, SHOULD'NT BE OK even if FK is NO_ACTION!
#endif
    }
    catch(const std::exception& ex) {
        auto s = ex.what();
        std::ignore = s;
    }
}
