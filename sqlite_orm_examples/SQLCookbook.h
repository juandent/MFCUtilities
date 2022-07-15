#pragma once

#include <chrono>
#include <sqlite_orm/sqlite_orm.h>
#include <optional>
#include <string>

#include "..\ORM_Extensions/DateBinding.h"

// #include "..\ORM_Extensions/SchemaManager.h"
// #include <sqlite_orm/SchemaManager.h>


struct Employee
{
	int m_empno;
	std::string m_ename;
	std::string m_job;
	std::optional<int> m_mgr;
	std::string m_hiredate;
	double m_salary;
	std::optional<double> m_commission;
	std::optional<int> m_deptno;
};

struct Department
{
	std::optional<int> m_deptno;
	std::string m_deptname;
	std::string m_loc;
	std::optional<int> m_manager;
};

struct EmpBonus
{
	int m_id;
	int m_empno;
	std::string m_received;	// date
	int m_type;
};

struct Artist
{
	int m_id;
	std::string m_name;
};

struct Album
{
	int m_id;
	int m_artist_id;
};

struct Customer {
	int m_id;
	std::string m_name;
	int m_age;
	std::string m_address;
	double m_salary;
};

struct Order {
	int m_id;
	int m_fkey_customer;
	double m_amount;
    std::chrono::sys_days m_placed;
    std::chrono::sys_days m_due;
};

struct Invoice {
	int m_id;
	std::chrono::sys_days date;
	int m_fkey_customer;
	double m_total_amount;
};

struct InvoiceLine {
	int m_id;
	int m_fkey_invoice;
	double m_quantity;
	double m_unit_price;
	double m_discount_percent;
	double m_taxes_percent;
	double m_line_amount;	// m_quantity * m_unit_price * (1 + m_taxes_percent) * (1 - m_discount_percent)
};

using namespace sqlite_orm;


/** CIRCULAR DEPENDENCIES
 * Employee->Department
 * Department->Employee
 * Emp_bonus->Employee
 * Album->Artist
 * Invoice->Customer
 * Order->Customer
 * InvoiceLine->Invoice
 *
 * one possible remove order:
 * 1) Emp_bonus
 * 2) now Employee *could* be dropped but Department has FK into Employee so we must:
 * 3) REMOVE FK from Department table       ==> for this, we need to change storage schema for Department table (remove FK into Employee)
 *                                          ==> this kind of change is not detected by **sync_schema**
 *                                          ==> we are forced to drop this table but FK from Employee will not let us do it
 *                                          ==> not in sqlite-orm and not in sqlclient
 *                                          ==> thus, we need to disable FK checking before dropping Department table
 *                                          ==> how can we do this in sqlite_orm PLAIN (not PLUS)?
 *                                          ==> the only other possibility is to use a sqlite client to disable FK checking and to drop this table
 *                                          ==> BUT, what if the database is running in a mobile device and the use of sqlite client is impossible?
 *                                          ==> WE MUST disable FK checking in order to drop the table but before we drop it we need to create a backup of the table
 *                                          ==> this is contemplated in sqlite_orm PLUS but not in sqlite_orm PLAIN
 *                                          ==> how do you solve this in sqlite_orm PLAIN???
 * 3) Having removed the FK from Department table, we can continue:
 * 4) Employee
 * 5) Department
 * 6) Album
 * 7) Artist
 * 8) InvoiceLine
 * 9) Invoice
 * 10) Order
 * 11) Customer
 */

 /** NON-CIRCULAR DEPENDENCIES
  * Employee->Department
  * Emp_bonus->Employee
  * Album->Artist
  * Invoice->Customer
  * Order->Customer
  * InvoiceLine->Invoice
  *
  * one possible remove order:
  * 1) Emp_bonus
  * 2) Employee
  * 3) Department
  * 4) Album
  * 5) Artist
  * 6) InvoiceLine
  * 7) Invoice
  * 8) Order
  * 9) Customer
  */

auto storage = make_storage("SQLCookbook.sqlite",
                            make_table("Emp",
                                       make_column("empno", &Employee::m_empno, primary_key(), autoincrement()),
                                       make_column("ename", &Employee::m_ename),
                                       make_column("job", &Employee::m_job),
                                       make_column("mgr", &Employee::m_mgr),
                                       make_column("hiredate", &Employee::m_hiredate),
                                       make_column("salary", &Employee::m_salary),
                                       make_column("comm", &Employee::m_commission),
                                       make_column("deptno", &Employee::m_deptno),
                                       foreign_key(&Employee::m_deptno).references(&Department::m_deptno).on_delete.cascade(),
                                       check(c(&Employee::m_salary) > &Employee::m_commission)),
                            make_table("Dept",
                                       make_column("deptno", &Department::m_deptno, primary_key(), autoincrement()),
                                       make_column("deptname", &Department::m_deptname),
                                       make_column("loc", &Department::m_loc),
                                       make_column("mgr", &Department::m_manager)),
                                       // foreign_key(&Department::m_manager).references(&Employee::m_empno)),
                            make_table("Emp_bonus",
                                       make_column("id", &EmpBonus::m_id, primary_key(), autoincrement()),
                                       make_column("empno", &EmpBonus::m_empno),
                                       make_column("received", &EmpBonus::m_received),
                                       make_column("type", &EmpBonus::m_type),
                                       foreign_key(&EmpBonus::m_empno).references(&Employee::m_empno).on_update.restrict_()),
                            make_table("Artists",
                                       make_column("id", &Artist::m_id, primary_key(), autoincrement()),
                                       make_column("name", &Artist::m_name)),
                            make_table("Albums",
                                       make_column("id", &Album::m_id, primary_key(), autoincrement()),
                                       make_column("artist_id", &Album::m_artist_id),
                                       foreign_key(&Album::m_artist_id).references(&Artist::m_id)),
                            make_table("Invoice",
                                       make_column("id", &Invoice::m_id, primary_key(), autoincrement()),
                                       make_column("date", &Invoice::date),     //, default_value(Today())),
                                       make_column("fkey_customer", &Invoice::m_fkey_customer),
                                       make_column("total_amount", &Invoice::m_total_amount),
                                       foreign_key(&Invoice::m_fkey_customer).references(&Customer::m_id)),
                            make_table("Customer",
                                       make_column("id", &Customer::m_id, primary_key(), autoincrement()),
                                       make_column("name", &Customer::m_name, collate_nocase()),
                                       make_column("age", &Customer::m_age),
                                       make_column("address", &Customer::m_address),
                                       make_column("salary", &Customer::m_salary)),
                            make_table("Orders",
                                       make_column("id", &Order::m_id, primary_key(), autoincrement()),
                                       make_column("fkey_customer", &Order::m_fkey_customer),
                                       make_column("amount", &Order::m_amount),
                                        make_column("placed", & Order::m_placed),
                                        make_column("due", &Order::m_due),
                                       foreign_key(&Order::m_fkey_customer).references(&Customer::m_id)),
                            make_table("InvoiceLine",
                                       make_column("id", &InvoiceLine::m_id, primary_key(), autoincrement()),
                                       make_column("fkey_invoice", &InvoiceLine::m_fkey_invoice),
                                       make_column("quantity", &InvoiceLine::m_quantity),
                                       make_column("unit_price", &InvoiceLine::m_unit_price),
                                       make_column("discount_per100", &InvoiceLine::m_discount_percent),
                                       make_column("taxes_per100", &InvoiceLine::m_taxes_percent),
                                       make_column("line_amount",
                                                   &InvoiceLine::m_line_amount,
                                                   generated_always_as(c(&InvoiceLine::m_quantity) * c(&InvoiceLine::m_unit_price) * (1 + c(&InvoiceLine::m_taxes_percent))
                                                                       * (1 - c(&InvoiceLine::m_discount_percent)))),
                                       foreign_key(&InvoiceLine::m_fkey_invoice).references(&Invoice::m_id)));


/*
 * Employee -> Department
 * EmpBonus -> Employee
 * Album -> Artist
 *
 *
 *	INSERT ORDER:
 *
 * 1- Department
 * 2- Employee
 * 3- EmpBonus
 * 4- Artist
 * 5- Album
 *
 *	DROP ORDER
 *
 *	1- Album
 *	2- Artist
 *	3- EmpBonus
 *	4- Employee
 *	5- Department
 *
 */

//