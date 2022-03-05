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

using namespace sqlite_orm;

auto& storage = make_storage("SQLCookbook",
								make_table("Emp", make_column("empno", &Employee::m_empno)))