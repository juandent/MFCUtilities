#include <sqlite_orm/sqlite_orm.h>
#include <string>
#include <vector>
#include <iostream>
#include <cassert>

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



int main()
{
	using namespace sqlite_orm;

	static auto storage = make_storage("SQLCookbook.sqlite",
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
		make_table("Dep",
			make_column("deptno", &Department::m_deptno, primary_key(), autoincrement()),
			make_column("deptname", &Department::m_deptname),
			make_column("loc", &Department::m_loc)));

	storage.sync_schema();

	std::vector<Employee> vec =
	{
		Employee{7369, "Smith", "Clerk", 7902, "17-DEC-1980",800,std::nullopt, 20},
		Employee{7499, "Allen", "SalesMan", 7698, "20-FEB-1981", 1600, 300, 30},
		Employee{7521,"Ward", "SalesMan", 7698,"22-feb-1981",1250,500.30},
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

	storage.replace_range(des.begin(), des.end());
	storage.replace_range(vec.begin(), vec.end());



	// storage.replace()
}