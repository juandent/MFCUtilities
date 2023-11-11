#include <sqlite_orm/sqlite_orm.h>
#include <string>
#include <vector>
#include <iostream>
#include "DateBinding.h"

namespace {

    struct Supplier_Invoice;
    struct Invoice;
    struct Supplier;
    using namespace sqlite_orm;
    using namespace std::literals;

    // persistent objects are Mature when id > 0!!


    struct Supplier {

        int id = 0;
        std::string name{};

        // inv must be Mature
        void attach_invoice(const Invoice& inv);
        void unattach_invoice(const Invoice& inv);

        void unattach_invoices();
        bool is_attached(const Invoice& inv);

        std::vector<Invoice> get_invoices() const;
    };




    struct Supplier_Invoice {
        int supplier_id = 0;
        int invoice_id = 0;

        // member variables must be set!
        void attach();
        void unattach();
        void unattach_invoices();
        void unattach_suppliers();
        bool exists();
    };

    struct InvoiceLine {
        int id = 0;
        int invoice_id = 0;
        double quantity;
        double unit_price;
        double line_amount;
    };

    struct Invoice {
        int id = 0;
        std::chrono::sys_days date;
        int count_lines;
        double total_amount;

        std::vector<InvoiceLine> lines;

    private:
        void erase_invoice_lines();


    public:
        // Invoice + Supplier
        void attach_supplier(const Supplier& sup);
        bool is_attached(const Supplier& sup);
        void unattach_supplier(const Supplier& sup);

        void unattach_suppliers();

        std::vector<Supplier> get_suppliers() const;

        void set_invoice_lines(std::vector<InvoiceLine>& inv_lines);


        void deep_insert();

        void deep_get();

    };



    static auto storage = make_storage("compound_invoice.sqlite",
        make_table("Invoice",
            make_column("id", &Invoice::id, primary_key(), autoincrement()),
            make_column("date", &Invoice::date),
            make_column("count_lines", &Invoice::count_lines),
            make_column("total_amount", &Invoice::total_amount)),
        make_table("InvoiceLines",
            make_column("id", &InvoiceLine::id, primary_key(), autoincrement()),
            make_column("invoice_id", &InvoiceLine::invoice_id),
            make_column("quantity", &InvoiceLine::quantity),
            make_column("unit_price", &InvoiceLine::unit_price),
            make_column("line_amount", &InvoiceLine::line_amount, generated_always_as(&InvoiceLine::quantity * c(&InvoiceLine::unit_price))),
            foreign_key(&InvoiceLine::invoice_id).references(&Invoice::id).on_delete.restrict_().on_update.restrict_()),
        make_table("Supplier",
            make_column("id", &Supplier::id, primary_key(), autoincrement()),
            make_column("name", &Supplier::name)),
        make_table("Supplier_Invoice",
            make_column("supplier_id", &Supplier_Invoice::supplier_id),
            make_column("invoice_id", &Supplier_Invoice::invoice_id),
            foreign_key(&Supplier_Invoice::supplier_id).references(&Supplier::id),
            foreign_key(&Supplier_Invoice::invoice_id).references(&Invoice::id),
            primary_key(&Supplier_Invoice::supplier_id, &Supplier_Invoice::invoice_id)));


    /**
     * \brief
     * \return ///////// implementation of methods
     */

    void Supplier::attach_invoice(const Invoice& inv) {

        Supplier_Invoice si{ this->id, inv.id };
        si.attach();
    }

    void Supplier::unattach_invoice(const Invoice& inv) {

        Supplier_Invoice si{ this->id, inv.id };
        si.unattach();
    }


    void Supplier::unattach_invoices() {
        Supplier_Invoice si{ this->id, -1 };
        si.unattach_invoices();
    }


    bool Supplier::is_attached(const Invoice& inv) {
        Supplier_Invoice si{ this->id, inv.id };
        return si.exists();
    }



    std::vector<Invoice> Supplier::get_invoices() const
    {
        return storage.get_all<Invoice>(join<Supplier_Invoice>(on(&Invoice::id == c(&Supplier_Invoice::invoice_id))), where(c(&Supplier_Invoice::supplier_id) == this->id));
    }

    void Supplier_Invoice::attach() {
        storage.replace(*this);
    }

    void Supplier_Invoice::unattach() {
        storage.remove_all<Supplier_Invoice>(where(c(&Supplier_Invoice::invoice_id) == this->invoice_id and c(&Supplier_Invoice::supplier_id) == this->supplier_id));
    }

    void Supplier_Invoice::unattach_invoices()
    {
        storage.remove_all<Supplier_Invoice>(where(c(&Supplier_Invoice::supplier_id) == this->supplier_id));
    }

    void Supplier_Invoice::unattach_suppliers() {
        storage.remove_all<Supplier_Invoice>(where(c(&Supplier_Invoice::invoice_id) == this->invoice_id));
    }

    bool Supplier_Invoice::exists()
    {
        auto g = storage.get_optional<Supplier_Invoice>(this->supplier_id, this->invoice_id);
        int ct = storage.count<Supplier_Invoice>(where(c(&Supplier_Invoice::supplier_id) == this->supplier_id and c(&Supplier_Invoice::invoice_id) == this->invoice_id));
        return g.has_value();
    }

    void Invoice::unattach_suppliers()
    {
        Supplier_Invoice si{ -1, this->id };
        si.unattach_suppliers();
    }

    void Invoice::erase_invoice_lines() {
        storage.remove_all<InvoiceLine>(where(c(&InvoiceLine::invoice_id) == this->id));
    }

    void Invoice::attach_supplier(const Supplier& sup)
    {
        Supplier_Invoice si{ sup.id, this->id };
        si.attach();
    }

    bool Invoice::is_attached(const Supplier& sup) {
        Supplier_Invoice si{ sup.id, this->id };
        return si.exists();
    }


    std::vector<Supplier> Invoice::get_suppliers() const {
        // auto vec = storage.
        auto expression = get_all<Supplier>(join<Supplier_Invoice>(on(&Supplier::id == c(&Supplier_Invoice::supplier_id))), where(c(&Supplier_Invoice::invoice_id) == this->id));
        auto sql = storage.dump(expression);
        auto statement = storage.prepare(expression);
        return storage.execute(statement);
    }

    void Invoice::set_invoice_lines(std::vector<InvoiceLine>& inv_lines)
    {
        auto guard = storage.transaction_guard();
        try {
            if (id < 1) {
                id = storage.insert(*this);
            }
            this->total_amount = 0;
            for (auto& line : inv_lines) {
                line.invoice_id = id;
                if (line.id < 1) {
                    line.id = storage.insert(line);
                }
                else {
                    storage.replace(line);
                }
                line = storage.get<InvoiceLine>(line.id);
                this->total_amount += line.line_amount;
            }
            this->count_lines = inv_lines.size();
            storage.update(*this);
            guard.commit();
        }
        catch (const std::exception& ex) {
            auto s = ex.what();
            guard.rollback();
        }
    }


    auto lambda_for_line = [](Invoice& inv, InvoiceLine& line, double& accumulation) {

        line.invoice_id = inv.id;
        if (line.id < 1) {
            line.id = storage.insert(line);
        }
        else {
            storage.replace(line);
        }
        line = storage.get<InvoiceLine>(line.id);
        accumulation += line.line_amount;
    };

    void Invoice::deep_insert() {
        auto guard = storage.transaction_guard();
        try
        {
            if (id < 1) {
                id = storage.insert(*this);
            }
            else {
                erase_invoice_lines();
            }
            auto line_amounts = 0.0;
            for (auto& line : lines) {
                line.invoice_id = id;
                if (line.id < 1) {
                    line.id = storage.insert(line);
                }
                else {
                    storage.replace(line);
                }
                line = storage.get<InvoiceLine>(line.id);
                line_amounts += line.line_amount;
            }
            this->total_amount = line_amounts;
            this->count_lines = lines.size();
            storage.update(*this);

            guard.commit();
        }
        catch (const std::exception& ex) {
            auto s = ex.what();
            guard.rollback();
        }
    }

    void Invoice::deep_get() {
        *this = storage.get<Invoice>(this->id);
        lines.clear();
        lines = storage.get_all<InvoiceLine>(where(c(&InvoiceLine::invoice_id) == this->id));
    }

    template<typename T>
    constexpr bool IsLeft = false;

    template<>
    constexpr bool IsLeft<Invoice> = true;

    template<typename A, typename B, typename AB, int A::* PKa, int B::* PKb, int AB::* PKaba, int AB::* PKabb>
    struct ManyToMany {
        static_assert(IsLeft<A>);

        ManyToMany() {}

        void attach(const A& a, const B& b) {
            AB ab;
            ab.*PKaba = a.*PKa;
            ab.*PKabb = b.*PKb;
            storage.replace(ab);
        }

        void unattach(const A& a, const B& b) {
            storage.remove_all<AB>(where(c(PKaba) == a.*PKa and c(PKabb) == b.*PKb));
        }

        bool are_attached(const A& a, const B& b) {
            int ct = storage.count<AB>(where(c(PKaba) == a.*PKa and c(PKabb) == b.*PKb));
            return ct != 0;
        }

        void unattach_all_from(const B& b) {
            storage.remove_all<AB>(where(c(PKabb) == b.*PKb));
        }

        void unattach_all_from(const A& a) {
            storage.remove_all<AB>(where(c(PKaba) == a.*PKa));
        }

        std::vector<B> get_all_for(const A& a) {
            return storage.get_all<B>(join<AB>(on(PKb == c(PKabb))), where(c(PKaba) == a.*PKa));
        }

        std::vector<A> get_all_for(const B& b) {
            return storage.get_all<A>(join<AB>(on(PKa == c(PKaba))), where(c(PKabb) == b.*PKb));
        }

    };
}

    int main() {

    ::remove(storage.filename().c_str());

    try {
        storage.sync_schema();

        // these are dependent pbjects! they cannot exist in the database by themselves
        std::vector<InvoiceLine> lines1 = {
            InvoiceLine{-1, -1, 10.5, 20000, 0},
            InvoiceLine{-1, -1, 12, 30000, 0},
        };

        // these are dependent pbjects! they should be given to an Invoice for it to have responsibility to save and load
        std::vector<InvoiceLine> lines2 = {
            InvoiceLine{-1, -1, 0.5, 10000, 0},
            InvoiceLine{-1, -1, 19, 22000, 0},
        };

        // these are independent objects! they should have the responsibility to save and load themselves
        std::vector<Supplier> sups = {Supplier{-1, "Abonos Agro"},
                                      Supplier{-1, "Universal"},
                                      Supplier{-1, "Monge"},
                                      Supplier{-1, "Lagar"},
                                      Supplier{-1, "Jimenez y Tanzi"}};
        storage.remove_all<Supplier>();
        storage.insert_range(sups.begin(), sups.end());
        // suppliers have no PK yet!

        // after next statement suppliers are Mature
        auto suppliers = storage.get_all<Supplier>(limit(5));

        Invoice inv{-1, today(), 0, 0.0};
        inv.set_invoice_lines(lines1);
        // now inv is Mature

        for(auto& il: inv.lines) {
            auto s = storage.dump(il);
            std::cout << s << std::endl;
        }

        // inv.attach_supplier(suppliers[0]);
        inv.attach_supplier(suppliers[1]);

        ManyToMany<Invoice,
                   Supplier,
                   Supplier_Invoice,
                   &Invoice::id,
                   &Supplier::id,
                   &Supplier_Invoice::invoice_id,
                   &Supplier_Invoice::supplier_id>
            mtom;
        mtom.attach(inv, suppliers[0]);
        mtom.unattach(inv, suppliers[0]);
        bool are = mtom.are_attached(inv, suppliers[0]);
        mtom.attach(inv, suppliers[0]);
        mtom.attach(inv, suppliers[0]);
        mtom.attach(inv, suppliers[2]);
        mtom.unattach_all_from(inv);
        mtom.unattach_all_from(suppliers[0]);

        auto sup = inv.get_suppliers();
        auto ss = mtom.get_all_for(inv);
        auto invoices = suppliers[0].get_invoices();
        auto invsss = mtom.get_all_for(suppliers[0]);

        bool ok = sup[0].is_attached(inv);

        auto yesterday = today() - std::chrono::days{1};
        Invoice inv2{-1, yesterday, 0, 0.0, lines2};
        inv2.deep_insert();
        // now inv2 is Mature

        inv2.attach_supplier(suppliers[0]);
        inv2.attach_supplier(suppliers[1]);

        auto sis = storage.get_all<Supplier_Invoice>();

        suppliers[0].unattach_invoice(inv2);

        auto sis2 = storage.get_all<Supplier_Invoice>();

        // assert(sis.size() == (sis2.size() + 1));

        sup[1].attach_invoice(inv);
        sup[1].attach_invoice(inv2);

        auto ve = sup[1].get_invoices();
        assert(ve.size() == 2);

        auto vec = inv.get_suppliers();
        auto vec2 = inv2.get_suppliers();

        auto v1 = sup[1].get_invoices();
        sup[1].unattach_invoice(inv2);
        auto v = sup[1].get_invoices();

        assert(v1.size() == v.size() + 1);
    } catch(const std::exception& ex) {
        auto s = ex.what();
        std::ignore = s;
    }
}

