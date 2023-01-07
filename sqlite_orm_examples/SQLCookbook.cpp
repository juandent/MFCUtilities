#include <sqlite_orm/sqlite_orm.h>
#include <string>
#include <vector>
#include <iostream>
#include <cassert>

// #include "SQLCookbook.h"
#include "SQLCookbook.h"

void Course_8();

void SQL1_8();
void SQL1_12();
void SQL1_13();
void SQL2_3();
void SQL2_5();
void SQL2_6();
void SQL3_1();
void SQL3_2();
void SQL3_3();
void SQL3_4();
void SQL3_5();
void SQL3_6();
void SQL3_9();
void Except();
void usingUpdate();
void usingDelete();
void usingObjectAPI();


int main()
{
	using namespace sqlite_orm;
	using namespace std::literals;

	auto guard = storage.transaction_guard();
	try
	{
		// foreign_key_disable_checking fk_off(storage);
		// storage.drop_table("Customer");
		// storage.drop_table("Invoice");
		// storage.drop_table("InvoiceLine");
		// storage.drop_table("Orders");
		// storage.drop_table("Customer");
#if 0
        if(storage.table_exists("Emp_bonus")) {
            storage.remove_all<EmpBonus>();
        }
		if (storage.table_exists("InvoiceLine")) {
			storage.remove_all<InvoiceLine>();
			auto inv = storage.get_all<InvoiceLine>();
		}

		if (storage.table_exists("Invoice")) {
			auto inv = storage.get_all<Invoice>();
			storage.remove_all<Invoice>();
			auto inv2 = storage.get_all<Invoice>();
		}
		if (storage.table_exists("Orders")) {
			storage.remove_all<Order>();
			// auto ords = storage.get_all<Order>();
		}
		if( storage.table_exists("Customer")) {
			
			auto expr = remove_all<Customer>();
			auto sql = storage.dump(expr);
			auto statement = storage.prepare(expr);
			storage.execute(statement);
			// auto cust2 = storage.get_all<Customer>();
		}
#endif
		storage.sync_schema(true);

#if 0
		Customer c{ 1, "Juan Dent" };
		// storage.replace(c);

		storage.replace(into<Customer>(), columns(&Customer::m_id, &Customer::m_name), values(std::make_tuple(1, "Juan")));
		// Invoice i{ 1, Today(), c.m_id, 20000 };
		// storage.insert(into<Invoice>(), columns(&Invoice::m_id, &Invoice::m_fkey_customer, &Invoice::total_amount),
		// 	values(std::make_tuple(1, 1, 20000)));

		storage.replace(into<Invoice>(), columns(&Invoice::m_id, &Invoice::m_fkey_customer, &Invoice::m_total_amount, &Invoice::date),
			values(std::make_tuple(1, 1, 20000, Today())));

		// storage.replace(i);
		InvoiceLine il{ 1,1,23, 100, 0.10, 0.13 };
		storage.replace(il);

		auto inv_line = storage.get<InvoiceLine>(1);
		Invoice inv = storage.get<Invoice>(1);
		std::chrono::year_month_day  date = inv.date;

		auto names = storage.select(distinct(columns(&Employee::m_deptno, &Employee::m_hiredate)));

		// SchemaManager sm(storage, temp_storage);
		//
		// sm.load_drop_sync_replace<Employee>();
		// sm.load_drop_sync_replace<Employee>();
		//
		// auto pair = sm.find_duplicate_in_column<Employee, &Employee::m_ename>();
		// if (pair.first)
		// {
		// 	// duplicates found
		// 	auto x = *pair.second;
		// 	std::ignore = x;
		// }
#endif

		storage.remove_all<Album>();
		storage.remove_all<Artist>();
		storage.remove_all<EmpBonus>();
		storage.remove_all<Employee>();
		storage.remove_all<Department>();
		guard.commit();
	}
	catch(std::exception& ex)
	{
		auto s = ex.what();
		guard.rollback();
		std::ignore = s;
	}

	std::vector<Artist> art =
	{
		Artist{1, "Elton John"},
		Artist{2, "Prince"}
	};

	std::vector<Album> albums =
	{
		Album{1, 1}
	};

	std::vector<Employee> vec =
	{
		Employee{7369, "Smith", "Clerk", 7902, "17-DEC-1980",800,std::nullopt,std::nullopt},
		Employee{7499, "Allen", "SalesMan", 7698, "20-FEB-1981", 1600, 300, 30},
		Employee{7521,"Ward", "SalesMan", 7698,"22-feb-1981",1250,500, 30},
		Employee{7566,"Jones", "Manager", 7839, "02-abr-1981",2975, std::nullopt,20},
		Employee{7654,"Martin","SalesMan", 7698, "28-sep-1981", 1250,140,30},
		Employee{7698,"Blake", "Manager", 7839, "01-may-1981", 2850, std::nullopt, 30},
		Employee{7782, "Clark", "Manager", 7839, "09-jun-1981", 2450, std::nullopt, 10},
		Employee{7788, "Scott", "Analyst", 7566, "09-Dec-1982", 3000, std::nullopt, 20},
		Employee{7839, "King", "President", std::nullopt, "17-nov-1981", 5000, std::nullopt,10},
		Employee{7844,"Turner","SalesMan", 7698, "08-Sep-1981", 1500, 0, 30},
		Employee{7876, "Adams", "Clerk", 7788, "12-JAN-1983", 1100, std::nullopt, 20},
		Employee{7900,"James", "Clerk", 7698,"03-DEC-1981", 950, std::nullopt, 30},
		Employee{7902,"Ford", "Analyst", 7566, "03-DEC-1981", 3000, std::nullopt, 20},
		Employee{7934, "Miller", "Clerk", 7782,"23-JAN-1982", 1300, std::nullopt, 10}
	};

	std::vector<Department> des =
	{
		Department{10, "Accounting", "New York",  std::nullopt}, // 7369 },
		Department{20, "Research", "Dallas", std::nullopt}, //   7566 },
		Department{30, "Sales", "Chicago", std::nullopt }, //7782 },
		Department{40, "Operations", "Boston", std::nullopt }, //7900 }
	};

	std::vector<EmpBonus> bonuses =
	{
		// EmpBonus{-1, 7369, "14-Mar-2005", 1},
		// EmpBonus{-1, 7900, "14-Mar-2005", 2},
		// EmpBonus{-1, 7788, "14-Mar-2005", 3},
		EmpBonus{-1, 7934, "17-Mar-2005", 1},
		EmpBonus{-1, 7934, "15-Feb-2005", 2},
		EmpBonus{-1, 7839, "15-Feb-2005", 3},
		EmpBonus{-1, 7782, "15-Feb-2005", 1}
	};

	std::vector<Customer> custs = {
			Customer{.m_id = 1, .m_name = "Erika Smith"s, .m_age = 21, .m_address = "Norway"s, .m_salary = 20000 },
			Customer{.m_id = 2, .m_name = "Rajesh Tripathi"s, .m_age = 30, .m_address = "Dhanbad"s, .m_salary = 15000},
			Customer{.m_id = 3, .m_name = "Sunu Verma"s, .m_age = 20, .m_address = "Dehradhun"s, .m_salary = 25000}
	};

	std::vector<Order> orders = {
			Order{.m_id = 1, .m_fkey_customer = 1, .m_amount = 2000},
			Order{.m_id = 2, .m_fkey_customer = 2, .m_amount = 3500},
			Order{.m_id = 3, .m_fkey_customer = 2, .m_amount = 5000}
	};

	std::vector<Invoice> inv = {
			Invoice{.m_id = 1, .date = today(),.m_fkey_customer = 1, .m_total_amount = 3000},
			Invoice{.m_id=2, .date = today(), .m_fkey_customer = 2, .m_total_amount = 6000},
			Invoice{.m_id=3, .date = today(), .m_fkey_customer = 3, .m_total_amount = 8000},
	};

	try
	{
		storage.replace_range(custs.begin(), custs.end());
		storage.replace_range(inv.begin(), inv.end());
		storage.replace_range(orders.begin(), orders.end());
		storage.replace_range(art.begin(), art.end());
		storage.replace_range(albums.begin(), albums.end());
		storage.replace_range(des.begin(), des.end());
		storage.replace_range(vec.begin(), vec.end());
		storage.insert_range(bonuses.begin(), bonuses.end());

			    // add FK values to cycle
		des[0].m_manager = 7369;
		des[1].m_manager = 7566;
		des[2].m_manager = 7782;
		des[3].m_manager = 7900;
		storage.update(des[0]);
		storage.update_all(des);
		storage.replace(des[0]);
		//storage.replace_range(des.begin(), des.end());

			
	}
	catch(std::exception& ex)
	{
		auto s = ex.what();
		std::ignore = s;
	}

	auto cs = storage.get_all<Customer>();

	Course_8();

	usingObjectAPI();
	usingDelete();
	usingUpdate();
	SQL1_8();
	SQL1_12();
	SQL1_13();
	SQL2_3();
	SQL2_5();
	SQL2_6();
	SQL3_1();
	SQL3_2();
	SQL3_3();
	SQL3_4();
	SQL3_5();
	SQL3_6();
	SQL3_9();
	Except();
}

void Course_8() {
	// 8
    auto expression = select(asterisk<Employee>());
	auto sql = storage.dump(expression);
	auto statement = storage.prepare(expression);
	auto rows = storage.execute(statement);
	// 9
	for (auto& employee : storage.iterate<Employee>()) {
		std::cout << storage.dump(employee) << std::endl;
	}
	{
		// 10
		auto rows = storage.select(columns(&Employee::m_ename, &Employee::m_salary),
			order_by(&Employee::m_ename).desc().collate_nocase());
		auto objects = storage.get_all<Employee>(order_by(&Employee::m_ename).desc().collate_nocase());
	}
    // 11
	{
		auto rows = storage.select(columns(&Employee::m_ename, &Employee::m_salary), multi_order_by(
			order_by(&Employee::m_salary).desc(),
			order_by(&Employee::m_ename).asc()));

		auto objects = storage.get_all<Employee>(multi_order_by(
			order_by(&Employee::m_salary).desc(),
			order_by(&Employee::m_ename).asc()));
	}
	// 12
    {
		bool order_by_salary_too = true;	// or false
		auto orderBy = dynamic_order_by(storage);
		orderBy.push_back(order_by(&Employee::m_ename).asc());
		if (order_by_salary_too) {
			orderBy.push_back(order_by(&Employee::m_salary).desc());
		}
		auto objects = storage.get_all<Employee>(orderBy);
    }
	// 13
    {
		SQL2_3();
    }
	// 13
	{
		auto expression = select(columns(&Employee::m_ename, &Employee::m_salary, &Employee::m_commission),
			order_by(is_null(&Employee::m_commission)).asc());
		auto sql = storage.dump(expression);
		auto statement = storage.prepare(expression);
		auto rows = storage.execute(statement);
	}
    // 14
    {
		SQL2_5();
    }
	// 15
    {
		SQL2_6();
    }
	// 16
    {
		auto dept_hiredates = storage.select(distinct(columns(&Employee::m_deptno, &Employee::m_hiredate)));
    }
	// 17
    {
		auto rows = storage.select(columns(&Employee::m_empno, &Employee::m_ename, &Employee::m_hiredate, &Department::m_deptname),
			where(is_equal(&Employee::m_deptno, &Department::m_deptno)));


		auto rows2  = storage.select(columns(&Employee::m_empno, &Employee::m_ename, &Employee::m_hiredate, &Department::m_deptname),
			where(c(&Employee::m_deptno) == &Department::m_deptno));
    }
	// 18
    {
		auto objects = storage.get_all<Employee>(where(lesser_or_equal(&Employee::m_empno, 20000)
			and (like(&Employee::m_ename, "T%") or glob(&Employee::m_ename, "*S"))));
    }
	// 19
    {
		auto rows = storage.select(columns(&Employee::m_empno, &Employee::m_ename, &Employee::m_hiredate, &Department::m_deptname),
			where(c(&Employee::m_deptno) == &Department::m_deptno), limit(4));
		auto objects = storage.get_all<Employee>(limit(4));
    }
	// 20
    {
		auto rows = storage.select(columns(&Employee::m_empno, &Employee::m_ename, &Employee::m_hiredate, &Department::m_deptname),
			where(c(&Employee::m_deptno) == &Department::m_deptno), limit(4, offset(3)));
		auto objects = storage.get_all<Employee>(limit(4, offset(3)));
    }
	// 21
    {
		auto rows = storage.select(columns(&Employee::m_ename, &Employee::m_salary), order_by(&Employee::m_salary).desc(),
			limit(2, offset(1)));
		auto objects = storage.get_all<Employee>(order_by(&Employee::m_salary).desc(), limit(2, offset(1)));
    }
	// 22
    {
		// SELECT DEPARTMENT_NAME FROM departments 
        // WHERE DEPARTMENT_NUMBER
        // BETWEEN 100 AND 200

		auto rows = storage.select(&Department::m_deptname, where(between(&Department::m_deptno, 100, 200)));

		{
			// SELECT DEPARTMENT_NO FROM departments 
			// WHERE DEPARTMENT_NAME 
			// BETWEEN “D” AND “F” 
			auto rows = storage.select(&Department::m_deptno, where(between(&Department::m_deptname, "D", "F")));
			auto objects = storage.get_all<Department>(where(between(&Department::m_deptname, "D", "F")));
		}
    }
	// 23
    {
		auto rows = storage.select(columns(&Employee::m_ename, &Employee::m_job, &Employee::m_deptno),
			where(in(&Employee::m_deptno,
				select(&Department::m_deptno, where(c(&Department::m_loc) == "Dallas")))));
    }
	// 24
    {
		// SELECT "Emp"."empno", "Emp"."ename", "Emp"."job", "Emp"."salary", "Emp"."deptno" FROM 'Emp’ 
		// WHERE(("Emp"."ename", "Emp"."job", "Emp"."salary")
		// IN(SELECT "Emp"."ename", "Emp"."job", "Emp"."salary" FROM 'Emp' WHERE(("Emp"."job" = "Clerk"))))

		auto rows = storage.select(columns(
			&Employee::m_empno, &Employee::m_ename, &Employee::m_job, &Employee::m_salary, &Employee::m_deptno),
			where(in(std::make_tuple(&Employee::m_ename, &Employee::m_job, &Employee::m_salary),
				select(columns(&Employee::m_ename, &Employee::m_job, &Employee::m_salary), where(c(&Employee::m_job) == "Clerk")))));

		{
			auto col_list = columns(&Employee::m_empno, &Employee::m_ename, &Employee::m_job, &Employee::m_salary, &Employee::m_deptno);
			auto tuple = std::make_tuple(&Employee::m_ename, &Employee::m_job, &Employee::m_salary);
			auto col_list1 = columns(&Employee::m_ename, &Employee::m_job, &Employee::m_salary);
			auto condition = c(&Employee::m_job) == "Clerk";
			using Table1 = Employee;
			using Table2 = Employee;

			auto rows = storage.select(col_list, from<Table1>(), where(in(tuple, select(col_list1, from<Table2>(), where(condition)))));
		}

    }
	// 25
    {
		// SELECT m_ename, m_deptno 
        // FROM employees 
        // WHERE m_deptno IN (10,20,30)

		std::vector<int> ids{ 10,20,30 };
		auto rows = storage.select(columns(&Employee::m_ename, &Employee::m_deptno),
			where(in(&Employee::m_deptno, ids)));
		auto objects = storage.get_all<Employee>(where(in(&Employee::m_deptno, { 10,20,30 })));
    }
	// 26
    {
		// SELECT m_ename, m_deptno 
		// FROM employees 
		// WHERE m_deptno NOT IN (10,20,30)

		std::vector<int> ids{ 10,20,30 };
		auto rows = storage.select(columns(&Employee::m_ename, &Employee::m_deptno),
			where(not_in(&Employee::m_deptno, ids)));
		auto objects = storage.get_all<Employee>(where(not_in(&Employee::m_deptno, { 10,20,30 })));

    }
	// 27
    {
		auto whereCondition = where(like(&Employee::m_ename, "_S%")); 
			auto users = storage.get_all<Employee>(whereCondition);
		auto rows = storage.select(&Employee::m_salary, whereCondition);
		auto rows1 = storage.select(like(&Employee::m_ename , "%S%a"));
		auto rows2 = storage.select(like(&Employee::m_job, "^%a").escape("^"));

    }
	// 28
    {
		auto rows = storage.select(columns(&Employee::m_ename), where(glob(&Employee::m_ename, "[^A-J]*")));
		auto employees = storage.get_all<Employee>(where(glob(&Employee::m_ename, "[^A-J]*")));
    }
	// 31
    {
		//  SELECT artists.m_id, albums.m_id FROM artists
        //  LEFT JOIN albums ON albums.m_artist_id = artists.m_id
        //  WHERE albums.m_id IS NULL;

		auto rows = storage.select(columns(&Artist::m_id, &Album::m_id),
			left_join<Album>(on(c(&Album::m_artist_id) == &Artist::m_id)),
			where(is_null(&Album::m_id)));
    }
    {
        // SELECT a.*, alb.* FROM artists a JOIN albums alb ON alb.artist_id = a.id

		auto expression = select(columns(asterisk<Artist>(), asterisk<Album>()), join<Album>(on(c(&Album::m_artist_id) == &Artist::m_id)));
		//auto sql = storage.dump(expression);
	//	auto statement = storage.prepare(expression);
	//	auto rows = storage.execute(statement);
    }
    // 32
    {
		// Transforming null values into real values
        // SELECT COALESCE(comm,0), comm FROM EMP

		auto expression = select(columns(coalesce<double>(&Employee::m_commission, 0), &Employee::m_commission));
		auto sql = storage.dump(expression);
		auto statement = storage.prepare(expression);
		auto rows = storage.execute(statement);
    }
	// 35
    {
        // cross-join
		// auto expression = select(asterisk<Employee>(), asterisk<Department>()); // cross_join<Department>()); // ) > ); // , cross_join<Department>());
		{
			auto expression = select(asterisk<Employee>(), cross_join<Department>()); // 
			auto sql = storage.dump(expression);
			auto statement = storage.prepare(expression);
			auto rows = storage.execute(statement);
			auto c7 = std::get<7>(rows[0]);
		}
		{
			auto expression = select(object<Employee>(), cross_join<Department>()); //
			auto sql = storage.dump(expression);
			auto statement = storage.prepare(expression);
			auto rows = storage.execute(statement);
			// auto c7 = std::get<7>(rows[0]);
		}

#if 0
		using als_c = alias_c<Customer>;
		using als_d = alias_d<Order>;
		auto rowsWithTableAliases = storage.select(columns(
			alias_column<als_c>(&Customer::m_id),
			alias_column<als_c>(&Customer::m_name),
			alias_column<als_c>(&Customer::m_age),
			alias_column<als_c>(&Customer::m_address)),
			where(is_equal(alias_column<als_c>(&Employee::id), alias_column<als_d>(&Department::empId))));
#endif
		// 35 (without asterisk)
		{
			try
			{
				auto expression = select(columns(
					&Customer::m_id,
					&Customer::m_name,
					&Customer::m_age,
					&Customer::m_address,
					&Customer::m_salary,
					&Order::m_id,
					&Order::m_fkey_customer,
					&Order::m_amount), cross_join<Order>());

				auto sql = storage.dump(expression);
				auto statement = storage.prepare(expression);
				auto rows = storage.execute(statement);

			} catch(const std::exception& ex) {
                    auto s = ex.what();
                    std::ignore = s;
            }
		}
		// 50
		{
			//  If you want to know the total amount of salary on each customer, then GROUP BY query would be as follows:
//  SELECT NAME, SUM(SALARY)
//  FROM COMPANY
//  GROUP BY NAME;

			auto totalSalaryPerName = storage.select(columns(&Employee::m_ename, sum(&Employee::m_salary)), group_by(&Employee::m_ename));

		}
		// 51
		{
			// SELECT (STRFTIME(“%Y”, "Invoices"."invoiceDate")) AS InvoiceYear,
            // (COUNT("Invoices"."id")) AS InvoiceCount FROM 'Invoices' GROUP BY 
            // InvoiceYear ORDER BY InvoiceYear DESC
			struct InvoiceYearAlias : alias_tag {
				static const std::string& get() {
					static const std::string res = "INVOICE_YEAR"; return res;
				}
			};
			struct InvoiceCountAlias : alias_tag {
				static const std::string& get() {
					static const std::string res = "INVOICE_COUNT"; return res;
				}
			};
			auto expression = select(columns(as<InvoiceYearAlias>(strftime("%Y", &Invoice::date)), as<InvoiceCountAlias>(count(&Invoice::m_id))), 
				group_by(get<InvoiceYearAlias>()), order_by(get<InvoiceYearAlias>()).desc());
			auto sql = storage.dump(expression);
			auto statement = storage.prepare(expression);
			auto rows = storage.execute(statement);
		}
		// 52
		{
			//  SELECT M_ENAME, SUM(M_SALARY)
            //  FROM COMPANY
            //  WHERE M_ENAME is like "%l%"
            //  GROUP BY M_ENAME
            //  HAVING SUM(M_SALARY) > 10000
			auto namesWithHigherSalaries = storage.select(columns(&Employee::m_ename, sum(&Employee::m_salary)),
				where(like(&Employee::m_ename, "%l%")),
				group_by(&Employee::m_ename).having(sum(&Employee::m_salary) > 10000));

		}
		// 54
		{
			//  SELECT EMP_ID, NAME, DEPT
			//  FROM COMPANY
			//  INNER JOIN DEPARTMENT
			//  ON COMPANY.ID = DEPARTMENT.EMP_ID
			//  UNION
			//  SELECT EMP_ID, NAME, DEPT
			//  FROM COMPANY
			//  LEFT OUTER JOIN DEPARTMENT
			//  ON COMPANY.ID = DEPARTMENT.EMP_ID;
			{
				auto rows1 = storage.select(columns(&Employee::m_ename, &Employee::m_empno, &Department::m_manager, &Department::m_deptname),
					inner_join<Department>(on(is_equal(&Employee::m_empno, &Department::m_manager))), order_by(&Employee::m_ename));

				auto rows2 = storage.select(columns(&Employee::m_ename, &Employee::m_empno, &Department::m_manager, &Department::m_deptname),
					left_outer_join<Department>(on(is_equal(&Employee::m_deptno, &Department::m_deptno))), order_by(&Employee::m_ename));


				auto expression = select(union_(select(columns(&Employee::m_ename, &Employee::m_empno, &Department::m_manager, &Department::m_deptname),
					inner_join<Department>(on(is_equal(&Employee::m_empno, &Department::m_manager)))),
					select(columns(&Employee::m_ename, &Employee::m_empno, &Department::m_manager, &Department::m_deptname),
						left_outer_join<Department>(on(is_equal(&Employee::m_deptno, &Department::m_deptno))))), order_by(&Employee::m_ename));
				auto sql = storage.dump(expression);
				auto statement = storage.prepare(expression);
				auto rows = storage.execute(statement);
			}
			{
				auto rows1 = storage.select(columns(&Employee::m_ename, &Employee::m_empno, &Department::m_manager, &Department::m_deptname),
					inner_join<Department>(on(is_equal(&Employee::m_empno, &Department::m_manager))), order_by(&Employee::m_ename));

				auto rows2 = storage.select(columns(&Employee::m_ename, &Employee::m_empno, &Department::m_manager, &Department::m_deptname),
					left_outer_join<Department>(on(is_equal(&Employee::m_deptno, &Department::m_deptno))), order_by(&Employee::m_ename));


				auto expression = select(union_all(select(columns(&Employee::m_ename, &Employee::m_empno, &Department::m_manager, &Department::m_deptname),
					inner_join<Department>(on(is_equal(&Employee::m_empno, &Department::m_manager)))),
					select(columns(&Employee::m_ename, &Employee::m_empno, &Department::m_manager, &Department::m_deptname),
						left_outer_join<Department>(on(is_equal(&Employee::m_deptno, &Department::m_deptno))))), order_by(&Employee::m_ename));
				auto sql = storage.dump(expression);
				auto statement = storage.prepare(expression);
				auto rows = storage.execute(statement);

			}
			{
				auto rows1 = storage.select(columns(&Customer::m_name, as_optional(&Order::m_id), as_optional(&Order::m_amount)), left_join<Order>(on(c(&Order::m_fkey_customer) == &Customer::m_id)));
				auto rows2 = storage.select(columns(&Employee::m_ename, &Department::m_deptno, as_optional(&Employee::m_salary)), inner_join<Department>(on(c(&Employee::m_empno) == &Department::m_manager)));

				auto expr1 = select(columns(&Customer::m_name, as_optional(&Order::m_id), as_optional(&Order::m_amount)), left_join<Order>(on(c(&Order::m_fkey_customer) == &Customer::m_id)));
				auto expr2 = select(columns(&Employee::m_ename, &Department::m_deptno, as_optional(&Employee::m_salary)), inner_join<Department>(on(c(&Employee::m_empno) == &Department::m_manager)));

				auto expr = select(union_(expr1, expr2));
				auto sql = storage.dump(expr);
				auto statement = storage.prepare(expr);
				auto rows = storage.execute(statement);
			}
		

		}
    }
	// 82
    {
		// SELECT "e"."empno", "e"."ename", "e"."hiredate", "d"."deptname" FROM "Dept" "d", "Emp" "e" WHERE (("e"."deptno" = "d"."mgr"))

        using als_e = alias_e<Employee>;
        using als_d = alias_d<Department>;

        auto expression = select(columns(
	        alias_column<als_e>(&Employee::m_empno),
	        alias_column<als_e>(&Employee::m_ename),
	        alias_column<als_e>(&Employee::m_hiredate),
	        alias_column<als_d>(&Department::m_deptname)),
	        where(is_equal(alias_column<als_e>(&Employee::m_deptno),
		        alias_column<als_d>(&Department::m_manager))));
		auto sql = storage.dump(expression);
		auto statement = storage.prepare(expression);
		auto rows = storage.execute(statement);
        {
			auto rows = storage.select(columns(
				alias_column<als_e>(&Employee::m_empno),
				alias_column<als_e>(&Employee::m_ename),
				alias_column<als_e>(&Employee::m_hiredate),
				alias_column<als_d>(&Department::m_deptname)),
				where(is_equal(alias_column<als_e>(&Employee::m_deptno),
					alias_column<als_d>(&Department::m_manager))));
        }

    }
	// 83
    {
        struct EmployeeIdAlias : alias_tag {
            static const std::string& get() {
                static const std::string res = "EMPLOYEE_ID";
                return res;
            }
        };

        struct EmployeeNameAlias : alias_tag {
            static const std::string& get() {
                static const std::string res = "EMPLOYEE_NAME";
                return res;
            }
        };

		// SELECT "Emp"."empno" AS "EMPLOYEE_ID", "Emp"."ename" AS "EMPLOYEE_NAME", "Emp"."hiredate", "Dept"."deptname"
		// FROM "Dept", "Emp" WHERE(("EMPLOYEE_ID" = "Dept"."mgr"))
			
		auto expression = select(columns(as<EmployeeIdAlias>(&Employee::m_empno),
                                                            as<EmployeeNameAlias>(&Employee::m_ename),
                                                            &Employee::m_hiredate,
                                                            &Department::m_deptname),
                                                    where(is_equal(get<EmployeeIdAlias>(), &Department::m_manager)));
		auto sql = storage.dump(expression);
		auto statement = storage.prepare(expression);
		auto rows = storage.execute(statement);

        {
            auto rows = storage.select(columns(as<EmployeeIdAlias>(&Employee::m_empno),
				as<EmployeeNameAlias>(&Employee::m_ename),
				&Employee::m_hiredate,
				&Department::m_deptname),
				where(is_equal(get<EmployeeIdAlias>(), &Department::m_manager)));
			std::ignore = rows;
        }
    }
	// 84
    {
		struct EmployeeIdAlias : alias_tag {
			static const std::string& get() {
				static const std::string res = "EMPLOYEE_ID";
				return res;
			}
		};

		struct EmployeeNameAlias : alias_tag {
			static const std::string& get() {
				static const std::string res = "EMPLOYEE_NAME";
				return res;
			}
		};

		// SELECT "e"."empno" AS "EMPLOYEE_ID", "e"."ename" AS "EMPLOYEE_NAME", "e"."hiredate", "d"."deptname"
		// FROM "Dept" "d", "Emp" "e" WHERE(("e"."empno" = "d"."mgr"))

		using als_e = alias_e<Employee>;
		using als_d = alias_d<Department>;

        auto expression = select(columns(
			as<EmployeeIdAlias>(alias_column<als_e>(&Employee::m_empno)),
			as<EmployeeNameAlias>(alias_column<als_e>(&Employee::m_ename)),
			alias_column<als_e>(&Employee::m_hiredate),
			alias_column<als_d>(&Department::m_deptname)),
			where(is_equal(alias_column<als_e>(&Employee::m_empno), alias_column<als_d>(&Department::m_manager))));
		auto sql = storage.dump(expression);
		auto statement = storage.prepare(expression);
		auto rows = storage.execute(statement);


		{
			auto rowsWithBothTableAndColumnAliases = storage.select(columns(
				as<EmployeeIdAlias>(alias_column<als_e>(&Employee::m_empno)),
				as<EmployeeNameAlias>(alias_column<als_e>(&Employee::m_ename)),
				alias_column<als_e>(&Employee::m_hiredate),
				alias_column<als_d>(&Department::m_deptname)),
				where(is_equal(alias_column<als_e>(&Employee::m_empno), alias_column<als_d>(&Department::m_manager))));

		}
    }
	// 85
    {
		// //  SELECT ID, NAME, MARKS,
  //       //      CASE
  //       //      WHEN MARKS >=80 THEN 'A+'
  //       //      WHEN MARKS >=70 THEN 'A'
  //       //      WHEN MARKS >=60 THEN 'B'
  //       //      WHEN MARKS >=50 THEN 'C'
  //       //      ELSE 'Sorry!! Failed'
  //       //      END as 'Grade'
  //       //      FROM STUDENT;
  //
		// auto rows = storage.select(columns(
		// 	&Student::id,
		// 	&Student::name,
		// 	&Student::marks,
		// 	as<GradeAlias>(case_<std::string>()
		// 		.when(greater_or_equal(&Student::marks, 95), then("A+"))
		// 		.when(greater_or_equal(&Student::marks, 90), then("A"))
		// 		.when(greater_or_equal(&Student::marks, 80), then("B"))
		// 		.when(greater_or_equal(&Student::marks, 70), then("C"))
		// 		.else_("Sorry!! Failed")
		// 		.end())));

    }
	// 92
    {
		try {
			// INSERT INTO "Customer" SELECT "Emp"."empno", "Emp"."ename", "Emp"."empno", "Emp"."job", "Emp"."salary" FROM "Emp"
			auto expression = insert(into<Customer>(),
				select(columns(&Employee::m_empno, &Employee::m_ename, &Employee::m_empno, &Employee::m_job, &Employee::m_salary)));
			auto sql = storage.dump(expression);
			auto statement = storage.prepare(expression);
			storage.execute(statement);
			auto r = storage.select(last_insert_rowid());

			{
				storage.insert(into<Customer>(),
					select(columns(&Employee::m_empno, &Employee::m_ename, &Employee::m_empno, &Employee::m_job, &Employee::m_salary)));
				auto r = storage.select(last_insert_rowid());
			}
		}
		catch(const std::exception& ex) {
			auto s = ex.what();
			std::ignore = s;
		}
		// storage.get
	}
}

void CreateView()
{
	struct Salary_Qualified	// this would be the interface of the view
	{
		std::string m_ename;
		std::string m_job;
	};
	auto statement = storage.prepare(select(columns(&Employee::m_ename, &Employee::m_job), where(in(&Employee::m_deptno, { 10,20 }))));

	// make_view("salary_qualified", &Salary_Qualified::m_ename, &Salary_Qualified::m_job, as(statement));
}

void SQL1_6()
{
	struct SalaryAlias : alias_tag {
		static const std::string& get() {
			static const std::string res = "SALARY";
			return res;
		}
	};


	//    select * 
//    from (select salary, comm as commission 
//          from emp)
//    where salary< 5000
	// storage.prepare(select(asterisk<Employee>(), from<Employee>()))
	struct EmpCte {
		int salary = 0;
		int comm = 0;
	};
	auto empcte = make_table("emp_inter",
		make_column("salary", &EmpCte::salary),
		make_column("comm", &EmpCte::comm));
	// auto statement = storage.prepare(select(asterisk<EmpCte>(), from<EmpCte>(), where(c(&EmpCte::salary) < 5000)));
	// auto sql = statement.expanded_sql();
	// auto rows = storage.execute(statement);

}

void SQL1_8()
{
	auto statement = storage.prepare(select(columns(&Employee::m_ename, &Employee::m_salary,
		case_<std::string>()
		.when(lesser_or_equal(&Employee::m_salary, 2000), then("UNDERPAID"))
		.when(greater_or_equal(&Employee::m_salary, 4000), then("OVERPAID"))
		.else_("OK").end())));
	auto sql = statement.expanded_sql();
	auto rows = storage.execute(statement);
}
void SQL1_12()
{
	// Transforming null values into real values
	// SELECT COALESCE(comm,0), comm FROM EMP

	auto statement = storage.prepare(select(columns(coalesce<double>(&Employee::m_commission, 0), &Employee::m_commission)));
	auto sql = statement.expanded_sql();
	auto rows = storage.execute(statement);
}

void SQL1_13()
{
	auto statement = storage.prepare(select(columns(&Employee::m_ename, &Employee::m_job), where(in(&Employee::m_deptno, { 10,20 }))));
	auto sql = statement.expanded_sql();
	auto rows = storage.execute(statement);

}

void SQL2_3()
{
	// SELECT ename, job from EMP order by substring(job, len(job)-1,2)
	auto expression = select(columns(&Employee::m_ename, &Employee::m_job), order_by(substr(&Employee::m_job, length(&Employee::m_job) - 1, 2)));
	auto sql = storage.dump(expression);
	auto statement = storage.prepare(expression);
	auto rows = storage.execute(statement);

	// auto statement = storage.prepare(select(columns(&Employee::m_ename, &Employee::m_job), order_by(substr(&Employee::m_job, length(&Employee::m_job) - 1, 2))));
	// auto sql = statement.expanded_sql();
	// auto rows = storage.execute(statement);

}

void SQL2_5()
{
	auto statement = storage.prepare(select(columns(&Employee::m_ename, &Employee::m_salary, &Employee::m_commission), 
		order_by(case_<int>().when(is_null(&Employee::m_commission), then(0)).else_(1).end()).desc()));
	auto sql = statement.expanded_sql();
	auto rows = storage.execute(statement);

	{
		auto statement = storage.prepare(select(columns(&Employee::m_ename, &Employee::m_salary, &Employee::m_commission),
			order_by(is_null(&Employee::m_commission)).asc()));
		auto sql = statement.expanded_sql();
		auto rows = storage.execute(statement);

	}
	{
		auto statement = storage.prepare(select(columns(&Employee::m_ename, &Employee::m_salary, &Employee::m_commission),
			order_by(2))); // DOES NOT WORK!
		auto sql = statement.expanded_sql();
		auto rows = storage.execute(statement);


	}
}

void SQL2_6()
{
	// Sorting on a data dependent key
	auto statement = storage.prepare(select(columns(&Employee::m_ename, &Employee::m_salary, &Employee::m_commission, &Employee::m_job),
		order_by(case_<double>().when(is_equal(&Employee::m_job, "SalesMan"), then(&Employee::m_commission)).else_(&Employee::m_salary).end()).desc()));
	auto sql = statement.expanded_sql();
	auto rows = storage.execute(statement);

	{
		auto expression = select(columns(&Employee::m_ename, &Employee::m_salary, &Employee::m_commission, &Employee::m_job),
			order_by(case_<double>().when(is_equal(&Employee::m_job, "SalesMan"), then(&Employee::m_commission)).else_(&Employee::m_salary).end()).desc());
		std::string sql = storage.dump(expression);
		auto statement = storage.prepare(expression);
		auto rows = storage.execute(statement);
	}
	{
		try
		{
			auto expression = insert(into<Employee>(), columns(&Employee::m_ename, &Employee::m_salary, &Employee::m_commission, &Employee::m_job), values(std::make_tuple("Juan", 224000, 200, "Eng")));
			std::string sql = storage.dump(expression);
			auto statement = storage.prepare(expression);
			storage.execute(statement);
		}
		catch(std::exception& ex)
		{
			std::string s = ex.what();
			std::ignore = s;
		}
	}
}


void SQL3_1()
{
	struct NamesAlias : alias_tag {
		static const std::string& get() {
			static const std::string res = "ENAME_AND_DNAME";
			return res;
		}
	};

	try
	{
		/*
		 *
// SELECT "Dept"."deptname" AS ENAME_AND_DNAME, "Dept"."deptno" FROM 'Dept' UNION ALL SELECT (QUOTE("------------------")), NULL UNION ALL
// SELECT "Emp"."ename" AS ENAME_AND_DNAME, "Emp"."deptno" FROM 'Emp'

		 *
		 *
		 */
		auto statement = storage.prepare(
			select(union_all(
				select(columns(as<NamesAlias>(&Department::m_deptname), &Department::m_deptno)),
				select(union_all(
					select(columns(quote("--------------------"), std::optional<int>())),
					select(columns(as<NamesAlias>(&Employee::m_ename), &Employee::m_deptno)))))));
			
		auto sql = statement.expanded_sql();
		auto rows = storage.execute(statement);
	}
	catch(std::exception& ex)
	{
		auto s = ex.what();
		std::ignore = s;
	}
}

void SQL3_2()
{
	struct EmpDptNo : alias_tag
	{
		static constexpr std::string get() { return "EmpDeptNo"; }
	};
	struct DeptDptNo : alias_tag
	{
		static constexpr std::string get() { return "DeptDeptNo"; }
	};
	using als_e = alias_e<Employee>;
	using als_d = alias_d<Department>;

	auto statement = storage.prepare(select(columns(alias_column<als_e>(&Employee::m_ename), alias_column<als_d>(&Department::m_loc), as<EmpDptNo>(alias_column<als_e>(&Employee::m_deptno)),
		as<DeptDptNo >(alias_column<als_d>(&Department::m_deptno))), from<als_e>(), join<als_d>(
			on(c(alias_column<als_e>(&Employee::m_deptno)) == alias_column<als_d>(&Department::m_deptno))), where(alias_column<als_e>(&Employee::m_deptno) == c(10))));
	auto sql = statement.expanded_sql();
	auto rows = storage.execute(statement);

	{
		try
		{
			auto expression = select(columns(&Employee::m_ename, &Department::m_loc, &Employee::m_deptno, &Department::m_deptno),
				from<Employee>(),
				natural_join<Department>());

			auto sql = storage.dump(expression);
			auto statement = storage.prepare(expression);
			auto rows = storage.execute(statement);
		}
		catch(const std::exception& ex) {
			auto s = ex.what();
			std::ignore = s;
		}
	}
}
void SQL3_3()
{
#if 1
	// select empno, ename, job, salary, depno from Emp
	// 	where(ename, job, salary) in
	// (select ename, job, salary from emp
	// 	intersect
	// 	select ename, job, salary from emp where job = "Clerk")

	try
	{
		auto statement = storage.prepare(select(columns(&Employee::m_empno, &Employee::m_ename, &Employee::m_job, &Employee::m_salary, &Employee::m_deptno),
			where(in(std::make_tuple(&Employee::m_ename, &Employee::m_job, &Employee::m_salary),
				intersect(
					select(columns(&Employee::m_ename, &Employee::m_job, &Employee::m_salary)),
					select(columns(&Employee::m_ename, &Employee::m_job, &Employee::m_salary), where(c(&Employee::m_job) == "Clerk")
					))))));
		auto sql = statement.expanded_sql();
		auto rows0 = storage.execute(statement);
	}
	catch(std::exception& ex)
	{
		auto s = ex.what();
		std::ignore = s;
	}
	// SELECT "Emp"."empno", "Emp"."ename", "Emp"."job", "Emp"."salary", "Emp"."deptno" FROM 'Emp' WHERE
	// (("Emp"."ename", "Emp"."job", "Emp"."salary") IN (
	// SELECT "Emp"."ename", "Emp"."job", "Emp"."salary" FROM 'Emp'
	// INTERSECT
	// SELECT "Emp"."ename", "Emp"."job", "Emp"."salary" FROM 'Emp' WHERE (("Emp"."job" = “Clerk”))))

	try
	{
		auto statement = storage.prepare(select(columns(&Employee::m_empno, &Employee::m_ename, &Employee::m_job, &Employee::m_salary, &Employee::m_deptno),
			where(c(std::make_tuple(&Employee::m_ename, &Employee::m_job, &Employee::m_salary)).in(
				intersect(
					select(columns(&Employee::m_ename, &Employee::m_job, &Employee::m_salary)),
					select(columns(&Employee::m_ename, &Employee::m_job, &Employee::m_salary), where(c(&Employee::m_job) == "Clerk")
					))))));
		auto sql = statement.expanded_sql();
		auto rows = storage.execute(statement);
	}
	catch(std::exception& ex)
	{
		auto s = ex.what();
		std::ignore = s;
	}

	{
		try
		{
			// SELECT "Emp"."empno", "Emp"."ename", "Emp"."job", "Emp"."salary", "Emp"."deptno" FROM 'Emp'
			// WHERE(("Emp"."ename", "Emp"."job", "Emp"."salary")
			// IN(SELECT "Emp"."ename", "Emp"."job", "Emp"."salary" FROM 'Emp' WHERE(("Emp"."job" = "Clerk" ))))

			auto expression = select(columns(
				&Employee::m_empno, &Employee::m_ename, &Employee::m_job, &Employee::m_salary, &Employee::m_deptno),
				where(in(std::make_tuple(&Employee::m_ename, &Employee::m_job, &Employee::m_salary),
					select(columns(&Employee::m_ename, &Employee::m_job, &Employee::m_salary), where(c(&Employee::m_job) == "Clerk")))));
			auto sql = storage.dump(expression);
			auto statement = storage.prepare(expression);
			auto rows = storage.execute(statement);
			
			{
				auto statement = storage.prepare(select(columns(
					&Employee::m_empno, &Employee::m_ename, &Employee::m_job, &Employee::m_salary, &Employee::m_deptno),
					where(in(std::make_tuple(&Employee::m_ename, &Employee::m_job, &Employee::m_salary),
						select(columns(&Employee::m_ename, &Employee::m_job, &Employee::m_salary), where(c(&Employee::m_job) == "Clerk"))))));

				auto sql = statement.expanded_sql();
				auto rows = storage.execute(statement);
				{
					auto statement = storage.prepare(select(columns(
						&Employee::m_empno, &Employee::m_ename, &Employee::m_job, &Employee::m_salary, &Employee::m_deptno),
						where(c(std::make_tuple(&Employee::m_ename, &Employee::m_job, &Employee::m_salary)).in(select(columns(&Employee::m_ename, &Employee::m_job, &Employee::m_salary), where(c(&Employee::m_job) == "Clerk"))))));

				}
			}
		}
		catch(std::exception& ex)
		{
			auto s = ex.what();
			std::ignore = s;
		}

	}
#endif
}

void Except()
{
	// Find all artists ids of artists who do not have any album in the albums table:
	// SELECT ArtistId FROM artists EXCEPT SELECT ArtistId FROM albums;

	auto statement = storage.prepare(select(except(select(&Artist::m_id), select(&Album::m_artist_id))));
	auto sql = statement.expanded_sql();
	auto rows = storage.execute(statement);
}

void SQL3_4()
{
	auto statement = storage.prepare(select(except(select(&Department::m_deptno), select(&Employee::m_deptno))));
	auto sql = statement.expanded_sql();
	auto rows = storage.execute(statement);

	{
		using als_d = alias_d<Department>;

		auto statement = storage.prepare(select(alias_column<als_d>(&Department::m_deptno), from<als_d>(), where(
			not exists(select(1, from<Employee>(), where(c(alias_column<als_d>(&Department::m_deptno)) == &Employee::m_deptno))))));
		auto sql = statement.expanded_sql();
		auto rows = storage.execute(statement);

	}
}

void SQL3_5()
{
	/*
	* Retrieving rows from one table that do not correspond to rows in another...
	* 
	 *	select d.* from dept d left outer join emp e
		on(d.deptno = e.depno)
		where e.depno is null
	 *
	 */
	using als_d = alias_d<Department>;
	using als_e = alias_e<Employee>;

	auto statement = storage.prepare(select(asterisk<als_d>(), from<als_d>(), left_join<als_e>(on
	(c(alias_column<als_d>(&Department::m_deptno)) == alias_column<als_e>(&Employee::m_deptno))), where(is_null(alias_column<als_e>(&Employee::m_deptno)))));
	auto sql = statement.expanded_sql();
	auto rows = storage.execute(statement);
	auto& row = rows[0];
	//static_assert(std::is_same_v<decltype(row),std::tuple<>&>);	// BUG!
	// auto fld1 = std::get<0>(row);

	std::ignore = row;
}

void SQL3_6()
{
	/*
	 * select e.ename, d.loc from emp e, dept d where e.depno = d.deptno
	 *
	 *
	 */
	using als_d = alias_d<Department>;
	using als_e = alias_e<Employee>;

	auto statement = storage.prepare(select(columns(alias_column<als_e>(&Employee::m_ename), alias_column<als_d>(&Department::m_loc)),
		where(c(alias_column<als_e>(&Employee::m_deptno)) == alias_column<als_d>(&Department::m_deptno))));
	auto sql = statement.expanded_sql();
	auto rows = storage.execute(statement);


	/*
	 * select e.ename, d.loc, b.received from emp e join dept d on (e.depno == d.deptno)
	 * left_join emp_bonus b on (e.empno = b.empno)
	 * order by d.loc
	 *
	 *
	 */
	using als_b = alias_b<EmpBonus>;

	{
		auto statement = storage.prepare(select(
			columns(alias_column<als_e>(&Employee::m_ename), alias_column<als_d>(&Department::m_loc), alias_column<als_b>(&EmpBonus::m_received)), from<als_e>(),
			join<als_d>(on(c(alias_column<als_e>(&Employee::m_deptno)) == alias_column<als_d>(&Department::m_deptno))),
			left_join<als_b>(on(c(alias_column<als_e>(&Employee::m_empno)) == alias_column<als_b>(&EmpBonus::m_empno))), order_by(alias_column<als_d>(&Department::m_loc))));
		auto sql = statement.expanded_sql();
		auto rows = storage.execute(statement);

	}

}

void SQL3_9()
{
	// DYNAMIC FROM:
	/*
	select deptno, sum(salary) as total_sal, sum(bonus) as total_bonus from
	(
	select e.empno
	, e.ename
	, e.salary
	, e.deptno
	, b.type
	, e.salary *
	case
	when b.type = 1 then .1
	when b.type = 2 then .2
	else .3
	end as bonus

	from emp e, emp_bonus b
	where e.empno = b.empno
	and e.deptno =20
	)
	group by deptno
	*/


	// INNER SELECT:

	/*
	 * select e.empno
, e.ename
, e.salary
, e.deptno
, b.type
, e.salary *
case
when b.type = 1 then .1
when b.type = 2 then .2
else .3
end as bonus

from emp e, emp_bonus b
where e.empno = eb.empno
and e.deptno =20
	 *
	 */

	struct Bonus : alias_tag
	{
		static constexpr std::string get() { return "bonus"; }
	};
	using als_e = alias_e<Employee>;
	using als_b = alias_b<EmpBonus>;
        {
            auto expression =
                select(columns(alias_column<als_e>(&Employee::m_empno),
                               alias_column<als_e>(&Employee::m_ename),
                               alias_column<als_e>(&Employee::m_salary),
                               alias_column<als_e>(&Employee::m_deptno),
                               alias_column<als_b>(&EmpBonus::m_type),
                               as<Bonus>(mul(alias_column<als_e>(&Employee::m_salary),
                                             case_<double>()
                                                 .when(c(alias_column<als_b>(&EmpBonus::m_type)) == 1, then(0.1))
                                                 .when(c(alias_column<als_b>(&EmpBonus::m_type)) == 2, then(0.2))
                                                 .else_(0.3)
                                                 .end()))),
                       where(c(alias_column<als_e>(&Employee::m_empno)) == alias_column<als_b>(&EmpBonus::m_empno) and
                             c(alias_column<als_e>(&Employee::m_deptno)) == 20));

            auto sql = storage.dump(expression);
			auto statement = storage.prepare(expression);
			auto rows = storage.execute(statement);
        }

	auto statement = storage.prepare(select(columns(
		alias_column<als_e>(&Employee::m_empno),
		alias_column<als_e>(&Employee::m_ename),
		alias_column<als_e>(&Employee::m_salary),
		alias_column<als_e>(&Employee::m_deptno),
		alias_column<als_b>(&EmpBonus::m_type),
		as<Bonus>(mul(alias_column<als_e>(&Employee::m_salary),
			case_<double>().
			when(c(alias_column<als_b>(&EmpBonus::m_type)) == 1, then(0.1)).
			when(c(alias_column<als_b>(&EmpBonus::m_type)) == 2, then(0.2)).else_(0.3).end()))),
		where(c(alias_column<als_e>(&Employee::m_empno)) == alias_column<als_b>(&EmpBonus::m_empno)
			and c(alias_column<als_e>(&Employee::m_deptno)) == 20)));

	auto sql = statement.expanded_sql();
	auto rows = storage.execute(statement);


}

void usingUpdate()
{
	storage.update_all(set(c(&Employee::m_salary) = c(&Employee::m_salary) * 1.3), where(c(&Employee::m_job) == "Clerk"));
	auto expression = update_all(set(c(&Employee::m_salary) = c(&Employee::m_salary) * 1.3), where(c(&Employee::m_job) == "Clerk"));
	std::string sql = storage.dump(expression);
	auto statement = storage.prepare(expression);
	storage.execute(statement);

	auto objects = storage.get_all<Employee>(where(c(&Employee::m_job) == "Clerk"));
}

void usingDelete()
{
	storage.remove_all<Employee>(where(c(&Employee::m_empno) == 5));
	auto expression = remove_all<Employee>(where(c(&Employee::m_empno) == 6));
	std::string sql = storage.dump(expression);
	auto statement = storage.prepare(expression);
	storage.execute(statement);
}

void usingObjectAPI()
{
	try
	{
		auto objects = storage.get_all<Employee>();		// SELECT * FROM EMP
		auto employee = storage.get<Employee>(7499);	// SELECT * FROM EMP WHERE id = 7499

		Employee emp{ -1, "JOSE", "ENG", std::nullopt, "17-DEC-1980", 32000, std::nullopt, 10 };
		emp.m_empno = storage.insert(emp);
		// INSERT INTO EMP ( 'ALL COLUMNS EXCEPT PRIMARY KEY COLUMNS' )
		// VALUES (	'VALUES TAKEN FROM emp OBJECT')


		emp.m_salary *= 1.3;
		storage.update(emp);
		//	UPDATE Emp
		//	SET
		//		column_name = emp.field_name	// for all columns except primary key columns
		//		// ....
		//	WHERE empno = emp.m_empno;

		storage.remove<Employee>(emp.m_empno);
		// DELETE FROM Emp WHERE empno = emp.m_empno

		storage.remove_all<Employee>(where(c(&Employee::m_salary) < 1000));
		// DELETE FROM Emp WHERE 'where clause'

		storage.remove_all<Employee>();
		// DELETE FROM Emp
	}
	catch(std::exception& ex)
	{
		auto s = ex.what();
		std::ignore = s;
	}
}

void SQL3_9a()
{
	// select  deptno, sum(salary) as total_sal, sum(bonus) as total_bonus from(
	// 	SELECT e.empno,
	// 	e.ename,
	// 	e.salary,
	// 	e.deptno,
	// 	sum(e.salary* case when eb.type = 1 then .1
	// 		when eb.type = 2 then  .2
	// 				   else .3
	// 				   end) as bonus
	// 	FROM Emp e, emp_bonus eb
	// 	where e.empno = eb.empno
	// 	and e.deptno = 10
	// 		group by e.empno)	// group emp_bonus by employee id


}

void SQL3_9b()
{
	// select d.deptno, d.total_sal, sum(e.salary * case
	// 	when eb.type = 1 then .1
	// 	when eb.type = 2 then .2
	// 										else .3
	// 										end) as total_bonus
	// 	from emp e, emp_bonus eb,
	// 	(
	// 	select deptno, sum(salary) as total_sal
	// 	from emp
	// 	where deptno = 10
	// 		group by deptno
	// 		) d
	// 		where e.deptno = d.deptno
	// 		and e.empno = eb.empno
	// 			group by d.deptno, d.total_sal
}

void SQL3_8c()
{
	// select d.deptno, d.total_sal, sum(e.salary * case
	// 	when eb.type = 1 then .1
	// 	when eb.type = 2 then .2
	// 										else .3
	// 										end) as total_bonus
	// 	from emp e, emp_bonus eb, dep_salary d
	// 	where e.deptno = d.deptno
	// 	and e.empno = eb.empno
	// 		group by d.deptno, d.total_sal
	//
	//
	// dep_salary view:
	//
	// 	select deptno, sum(salary) as total_sal
	// 		from emp
	// 	where deptno = 10
	// 		group by deptno

}