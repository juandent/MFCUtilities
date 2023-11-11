#include <sqlite_orm/sqlite_orm.h>
#include <string>
#include <vector>
#include <iostream>



struct FootballerDTO {
    int id;
    std::string name;
    int type_id;
    std::string club;
};

struct CricketerDTO {
    int id;
    std::string name;
    int type_id;
    double batting_average;
};

struct Player {
    int id;
    std::string name;
    int type_id;

    Player(int id, const std::string& name, int type_id) :
        id(id),
        name(name),
        type_id(type_id) {
    }

    virtual ~Player() = default;
};

// type == 1
struct Footballer : Player {
    std::string club;

    Footballer(int id, const std::string& name, int type_id, const std::string& club) :
        Player(id, name, type_id),
        club(club) {
    }

    Footballer(const FootballerDTO& dto) : Player{ dto.id, dto.name, dto.type_id }, club{dto.club} {}

    FootballerDTO dto() const{ return *this; }

    operator FootballerDTO() const {
        FootballerDTO dto{ this->id, this->name, this->type_id, this->club };
        return dto;
    }

};

// type == 2
struct Cricketer : Player {
    double batting_average;

    Cricketer(int id, const std::string& name, int type_id, double batting_average) :
        Player(id, name, type_id),
        batting_average(batting_average) {
    }

    Cricketer(const CricketerDTO& dto) :
        Player(dto.id, dto.name, dto.type_id),
        batting_average(dto.batting_average) {
    }
};


using namespace sqlite_orm;
using namespace std::literals;

auto storage = make_storage("InheritanceWithDTOs.sqlite",
    make_table("Footballers",
        make_column("id", &FootballerDTO::id, primary_key()),
        make_column("name", &FootballerDTO::name),
        make_column("type_id", &FootballerDTO::type_id, default_value(1)),
        make_column("club", &FootballerDTO::club)),
    make_table("Cricketer",
        make_column("id", &CricketerDTO::id, primary_key()),
        make_column("name", &CricketerDTO::name),
        make_column("type_id", &CricketerDTO::type_id, default_value(2)),
        make_column("batting_average", &CricketerDTO::batting_average)));

template<typename R, typename T, typename Storage, typename Context >
class DbSet {
    R T::* PK;
    Storage& storage;
    std::vector<T> objects;
    Context& context;
public:
    DbSet(R T::* pK, Storage& storage, Context& context) : PK{pK}, storage(storage), context(context) {}
    void register_dirty(const T & t) {
        objects.push_back(t);
    }

    void save_changes() {
        for(auto& x : objects) {
            if( x.*PK > 0) {
                storage.replace(x);
            }
            else {
                x.*PK = storage.insert(x);
            }
        }
    }

};



// unit of work
class DbContext {
public:
    virtual void save_changes() = 0;

    // std::vector, DbContext
};



class Context : public DbContext {

public:
    void save_changes() override {}

    template<typename T>
    void add(const T& t) {  // add a person to 
        
    }
    std::vector<Footballer> footballers_;
};

int main(int, char**) {

    storage.sync_schema(true);

    Context context;

    DbSet db_set{ &FootballerDTO::id, storage, context };


    Footballer fb{ 1, "Jose", 1, "Saprissa" };

    storage.replace(fb.dto());
    
    // Cricketer cb;
    // cb.id = 2;
    // cb.name = "Raul";
    // cb.batting_average = 32.5;

    

    // storage.replace(fb);
    // storage.replace(cb);

    storage.replace(into<FootballerDTO>(), columns(&FootballerDTO::id, &FootballerDTO::name, &FootballerDTO::club), values(std::make_tuple(1, "Jose Saprissa"s, "Saprissa"s)));
    // storage.replace(into<Footballer>(), columns( fb_id, fb_name, &Footballer::club), values(std::make_tuple(1, "Jose Saprissa"s, "Saprissa"s)));

    auto vec = storage.get_all_pointer<FootballerDTO>();
    
    auto v = storage.get_all<FootballerDTO>();

    v[0].name += " Jr";
    storage.update(v[0]);

    Footballer fb2 = v[0];

    // auto q1 = columns(fb_id, fb_name, fb_type);
    // auto q2 = select(columns(cb_id, cb_name, cb_type));

    // auto q2 = select(columns(&Cricketer::id, &Cricketer::name), from<Cricketer>());
    // // auto sql = storage.dump(q1);
    // //
    // auto objs1 = storage.get_all_pointer<Footballer>();
    // auto objs2 = storage.get_all_pointer<Cricketer>();
    // std::vector<std::unique_ptr<Player>> vec;
    // for (auto&& o : objs1) {
    //     vec.push_back(std::move(o));
    // }
    // for (auto&& o : objs2) {
    //     vec.push_back(std::move(o));
    // }

    // auto players = storage.select(q1, from<Footballer>());
    // auto players = storage.select(union_(q1, q2));

    // auto vec = storage.get_all_pointer<Footballer>();
    // auto v = storage.select(union_(select(columns(&Footballer::id)), select(columns(&Cricketer::id))));
}
