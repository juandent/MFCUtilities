#include <sqlite_orm/sqlite_orm.h>

#include <cassert>
#include <string>
#include <iostream>

using std::cout;
using std::endl;





struct Invoice
{
	int id;
	int customerId;
	std::optional<std::string> invoiceDate;
};

using namespace sqlite_orm;


struct InvoiceYearAlias : alias_tag {
    static const std::string& get() {
        static const std::string res = "InvoiceYear";
        return res;
    }
};

struct InvoiceCountAlias : alias_tag {
    static const std::string& get() {
        static const std::string res = "InvoiceCount";
        return res;
    }
};

int main(int, char** argv) {
    cout << argv[0] << endl;

    auto storage = make_storage("aliases.sqlite",
        make_table("Invoices", make_column("id", &Invoice::id, primary_key(), autoincrement()),
            make_column("customerId", &Invoice::customerId),
            make_column("invoiceDate", &Invoice::invoiceDate, default_value(date("now")))));


    storage.sync_schema();
    storage.drop_table("Invoices");
    storage.sync_schema();


    {
        Invoice inv{ -1, 1, std::nullopt };
        auto statement = storage.prepare(insert(inv, columns(&Invoice::customerId)));
        auto sql = statement.expanded_sql();
        storage.execute(statement);

        {
            inv.id = 5;
            inv.customerId = 3;
            inv.invoiceDate = storage.select(date("now")).front();
            auto statement = storage.prepare(replace(inv));
            auto sql = statement.expanded_sql();
            storage.execute(statement);

            {
                // auto statement = storage.update(or_abort(),
                //     into<Invoice>(),
                //     columns(&Invoice::id, &Invoice::customerId, &Invoice::invoiceDate),
                //     values(std::make_tuple(1, "The Weeknd", date("now", "+1 month"))));
            }

        }

        storage.insert(into<Invoice>(), columns(&Invoice::id, &Invoice::customerId, &Invoice::invoiceDate), values(std::make_tuple(1, 1, date("now")), std::make_tuple(2, 1, date("now", "+1 year")),
            std::make_tuple(3, 1, date("now")), std::make_tuple(4, 1, date("now", "+1 year"))));
        {
            // INSERT INTO Invoices("customerId") VALUES(2), (4), (8)
            auto statement = storage.prepare(insert(into<Invoice>(), columns(&Invoice::customerId), values(std::make_tuple(2), std::make_tuple(4), std::make_tuple(8))));
            auto sql = statement.expanded_sql();
            storage.execute(statement);
            auto rows = storage.get_all<Invoice>();
            std::ignore = rows;
        }
    }
    auto statement = storage.prepare(select(columns(as<InvoiceYearAlias>(strftime("%Y", &Invoice::invoiceDate)), as<InvoiceCountAlias>(count(&Invoice::id))), group_by(get<InvoiceYearAlias>()), order_by(get<InvoiceYearAlias>()).desc()));
    auto sql = statement.expanded_sql();
    auto s = statement.sql();
    auto rows = storage.execute(statement);
    
}