#include <sqlite_orm/sqlite_orm.h>
#include <string>
#include <vector>
#include <iostream>
#include <cassert>

#if 0
struct Employee
{
	int m_empno;
	std::string m_ename;
	std::string m_job;
	std::optional<int> m_mgr;
	std::string m_hiredate;
	double m_salary;
	std::optional<double> m_commission;
	int m_depno;
};

struct Department
{
	int m_deptno;
	std::string m_deptname;
	std::string m_loc;

};

struct EmpBonus
{
	int m_id;
	int m_empno;
	std::string m_received;	// date
	int m_type;
};

using namespace sqlite_orm;

auto storage = make_storage("SQLCookbook.sqlite",
	make_table("Emp",
		make_column("empno", &Employee::m_empno, primary_key(), autoincrement()),
		make_column("ename", &Employee::m_ename),
		make_column("job", &Employee::m_job),
		make_column("mgr", &Employee::m_mgr),
		make_column("hiredate", &Employee::m_hiredate),
		make_column("salary", &Employee::m_salary),
		make_column("comm", &Employee::m_commission),
		make_column("depno", &Employee::m_depno),
		foreign_key(&Employee::m_depno).references(&Department::m_deptno)),
	make_table("Dept",
		make_column("deptno", &Department::m_deptno, primary_key(), autoincrement()),
		make_column("deptname", &Department::m_deptname),
		make_column("loc", &Department::m_loc)),
	make_table("Emp_bonus",
		make_column("id", &EmpBonus::m_id, primary_key(), autoincrement()),
		make_column("empno", &EmpBonus::m_empno),
		make_column("received", &EmpBonus::m_received),
		make_column("type", &EmpBonus::m_type),
		foreign_key(&EmpBonus::m_empno).references(&Employee::m_empno)));
#else
#include "SQLCookbook.h"
#endif

void SQL1_8();
void SQL1_13();
void SQL2_5();
void SQL2_6();
void SQL3_1();
void SQL3_2();
void SQL3_3();
void SQL3_4();
void SQL3_5();
void SQL3_6();

int main()
{
	using namespace sqlite_orm;

	storage.sync_schema();

	try
	{
		storage.remove_all<EmpBonus>();
		storage.remove_all<Employee>();
		storage.remove_all<Department>();
	}
	catch(std::exception& ex)
	{
		auto s = ex.what();
		std::ignore = s;
	}

	std::vector<Employee> vec =
	{
		Employee{7369, "Smith", "Clerk", 7902, "17-DEC-1980",800,std::nullopt, 20},
		Employee{7499, "Allen", "SalesMan", 7698, "20-FEB-1981", 1600, 300, 30},
		Employee{7521,"Ward", "SalesMan", 7698,"22-feb-1981",1250,500, 30},
		Employee{7566,"Jones", "Manager", 7839, "02-abr-1981",2975, std::nullopt,20},
		Employee{7654,"Martin","SalesMan", 7698, "28-sep-1981", 1250,1400,30},
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
		Department{10, "Accounting", "New York"},
		Department{20, "Research", "Dallas"},
		Department{30, "Sales", "Chicago"},
		Department{40, "Operations", "Boston"}
	};

	std::vector<EmpBonus> bonuses =
	{
		EmpBonus{-1, 7369, "14-Mar-2005", 1},
		EmpBonus{-1, 7900, "14-Mar-2005", 2},
		EmpBonus{-1, 7788, "14-Mar-2005", 3}
	};

	try
	{
		storage.replace_range(des.begin(), des.end());
		storage.replace_range(vec.begin(), vec.end());
		storage.insert_range(bonuses.begin(), bonuses.end());
	}
	catch(std::exception& ex)
	{
		auto s = ex.what();
		std::ignore = s;
	}

	SQL3_1();

	SQL1_8();
	SQL1_13();
	SQL2_5();
	SQL2_6();
	SQL3_1();
	SQL3_2();
	SQL3_3();
	SQL3_4();
	SQL3_5();
	SQL3_6();
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

void SQL1_13()
{
	auto statement = storage.prepare(select(columns(&Employee::m_ename, &Employee::m_job), where(in(&Employee::m_depno, { 10,20 }))));
	auto sql = statement.expanded_sql();
	auto rows = storage.execute(statement);

}

void SQL2_5()
{
	auto statement = storage.prepare(select(columns(&Employee::m_ename, &Employee::m_salary, &Employee::m_commission), 
		order_by(case_<int>().when(is_null(&Employee::m_commission), then(0)).else_(1).end()).desc()));
	auto sql = statement.expanded_sql();
	auto rows = storage.execute(statement);

}

void SQL2_6()
{
	auto statement = storage.prepare(select(columns(&Employee::m_ename, &Employee::m_salary, &Employee::m_commission),
		order_by(case_<double>().when(is_equal(&Employee::m_job, "SalesMan"), then(&Employee::m_commission)).else_(&Employee::m_salary).end()).desc()));
	auto sql = statement.expanded_sql();
	auto rows = storage.execute(statement);

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
		auto statement = storage.prepare(
			select(union_all(
				select(columns(as<NamesAlias>(&Department::m_deptname), as_optional(&Department::m_deptno))),
				select(union_all(
					select(columns(quote("--------------------"), std::optional<int>())),
					select(columns(as<NamesAlias>(&Employee::m_ename), as_optional(&Employee::m_depno))))))));
			
		auto sql = statement.expanded_sql();
		auto rows = storage.execute(statement);	// THIS HAS A BUG related to std::nullopt!
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

	auto statement = storage.prepare(select(columns(alias_column<als_e>(&Employee::m_ename), alias_column<als_d>(&Department::m_loc), as<EmpDptNo>(alias_column<als_e>(&Employee::m_depno)),
		as<DeptDptNo >(alias_column<als_d>(&Department::m_deptno))), from<als_e>(), join<als_d>(
			on(c(alias_column<als_e>(&Employee::m_depno)) == alias_column<als_d>(&Department::m_deptno))), where(alias_column<als_e>(&Employee::m_depno) == c(10))));
	auto sql = statement.expanded_sql();
	auto rows = storage.execute(statement);

}
void SQL3_3()
{
#if 1
	// storage.prepare(select(columns(&Employee::m_empno, &Employee::m_ename, &Employee::m_job, &Employee::m_salary, &Employee::m_depno)),
	// 	where(in(std::tuple(&Employee::m_ename, &Employee::m_job, &Employee::m_salary),
	// 		select(intersect(
	// 			select(columns(&Employee::m_ename, &Employee::m_job, &Employee::m_salary)),
	// 			select(columns(&Employee::m_ename, &Employee::m_job, &Employee::m_salary), where(c(&Employee::m_job) == "Clerk"))
	// 			)))));
	try
	{
		auto statement = storage.prepare(select(columns(&Employee::m_empno, &Employee::m_ename, &Employee::m_job, &Employee::m_salary, &Employee::m_depno),
			where(in(std::make_tuple(&Employee::m_ename, &Employee::m_job, &Employee::m_salary),
				select(intersect(
					select(columns(&Employee::m_ename, &Employee::m_job, &Employee::m_salary)),
					select(columns(&Employee::m_ename, &Employee::m_job, &Employee::m_salary), where(c(&Employee::m_job) == "Clerk")
					)))))));
		auto sql = statement.expanded_sql();
		auto rows0 = storage.execute(statement);
	}
	catch(std::exception& ex)
	{
		auto s = ex.what();
		std::ignore = s;
	}



	auto rows = storage.select(columns(&Employee::m_empno, &Employee::m_ename, &Employee::m_job, &Employee::m_salary, &Employee::m_depno),
		where(c(std::make_tuple( &Employee::m_ename, &Employee::m_job, &Employee::m_salary)).in(
			select(intersect(
				select(columns(&Employee::m_ename, &Employee::m_job, &Employee::m_salary)),
				select(columns(&Employee::m_ename, &Employee::m_job, &Employee::m_salary), where(c(&Employee::m_job) == "Clerk")
				))))));
#endif
}

void SQL3_4()
{
	auto statement = storage.prepare(select(except(select(&Department::m_deptno), select(&Employee::m_depno))));
	auto sql = statement.expanded_sql();
	auto rows = storage.execute(statement);

	{
		using als_d = alias_d<Department>;

		auto statement = storage.prepare(select(alias_column<als_d>(&Department::m_deptno), from<als_d>(), where(
			not exists(select(1, from<Employee>(), where(c(alias_column<als_d>(&Department::m_deptno)) == &Employee::m_depno))))));
		auto sql = statement.expanded_sql();
		auto rows = storage.execute(statement);

	}
}

void SQL3_5()
{
	/*
	 *	select d.* from dept d left outer join emp e
		on(d.deptno = e.depno)
		where e.depno is null
	 *
	 */
	using als_d = alias_d<Department>;
	using als_e = alias_e<Employee>;

	auto statement = storage.prepare(select(asterisk<als_d>(), from<als_d>(), left_join<als_e>(on
	(c(alias_column<als_d>(&Department::m_deptno)) == alias_column<als_e>(&Employee::m_depno))), where(is_null(alias_column<als_e>(&Employee::m_depno)))));
	auto sql = statement.expanded_sql();
	auto rows = storage.execute(statement);
	auto& row = rows[0];
	static_assert(std::is_same_v<decltype(row),std::tuple<>&>);	// BUG!
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
		where(c(alias_column<als_e>(&Employee::m_depno)) == alias_column<als_d>(&Department::m_deptno))));
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
			join<als_d>(on(c(alias_column<als_e>(&Employee::m_depno)) == alias_column<als_d>(&Department::m_deptno))),
			left_join<als_b>(on(c(alias_column<als_e>(&Employee::m_empno)) == alias_column<als_b>(&EmpBonus::m_empno))), order_by(alias_column<als_d>(&Department::m_loc))));
		auto sql = statement.expanded_sql();
		auto rows = storage.execute(statement);

	}

}

