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

	std::vector<Employee> vec = {}


	storage.replace()
}