// GenericGrid.cpp : implementation file
//

#include "pch.h"
#include "Investments.h"
#include "afxdialogex.h"
#include "GenericGrid.h"


// GenericGrid dialog

IMPLEMENT_DYNAMIC(GenericGrid, CDialog)

GenericGrid::GenericGrid(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_GenericGrid, pParent)
{

}

GenericGrid::~GenericGrid()
{
}

void GenericGrid::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_GENERIC, m_generic_grid);
}


BEGIN_MESSAGE_MAP(GenericGrid, CDialog)
END_MESSAGE_MAP()


// GenericGrid message handlers




struct Employee {
    int id;
    std::string name;
    int age;
    std::optional<std::string> address;
    std::optional<double> salary;

    bool operator==(const Employee& other) const {
        return this->id == other.id && this->name == other.name && this->age == other.age &&
            this->address == other.address && this->salary == other.salary;
    }
};

inline namespace ORM
{
    auto storage = make_storage({ "generic.sqlite" },
        make_table("COMPANY",
            make_column("ID", &Employee::id, primary_key()),
            make_column("NAME", &Employee::name),
            make_column("AGE", &Employee::age),
            make_column("ADDRESS", &Employee::address),
            make_column("SALARY", &Employee::salary)));

    void insertEmployees()
    {
        storage.sync_schema();

        storage.remove_all<Employee>();

        //  create employees..
        Employee paul{ -1, "Paul", 32, std::nullopt, 20000.0 };
        Employee allen{ -1, "Allen", 25, "Texas", 15000.0 };
        Employee teddy{ -1, "Teddy", 23, "Norway", 20000.0 };
        Employee mark{ -1, "Mark", 25, "Rich-Mond", 65000.0 };
        Employee david{ -1, "David", 27, "Texas", 85000.0 };
        Employee kim{ -1, "Kim", 22, "South-Hall", 45000.0 };
        Employee james{ -1, "James", 24, "Houston", 10000.0 };

        //  insert employees. `insert` function returns id of inserted object..
        paul.id = storage.insert(paul);
        allen.id = storage.insert(allen);
        teddy.id = storage.insert(teddy);
        mark.id = storage.insert(mark);
        david.id = storage.insert(david);
        kim.id = storage.insert(kim);
        james.id = storage.insert(james);

    }
}


BOOL GenericGrid::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  Add extra initialization here
    insertEmployees();
    InitializeGridRendimientos(true);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

auto as_optional()
{
    using namespace sqlite_orm;

    struct Employee
    {
        int m_empno;
        std::string m_ename;
        std::string m_job;
        std::optional<int> m_mgr;
        std::string m_hiredate;
        double m_salary;
        std::optional<double> m_commission;
        int m_deptno;
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
            make_column("deptno", &Employee::m_deptno),
            foreign_key(&Employee::m_deptno).references(&Department::m_deptno)),
        make_table("Dept",
            make_column("deptno", &Department::m_deptno, primary_key(), autoincrement()),
            make_column("deptname", &Department::m_deptname),
            make_column("loc", &Department::m_loc)),
        make_table("Emp_bonus",
            make_column("id", &EmpBonus::m_id, primary_key(), autoincrement()),
            make_column("empno", &EmpBonus::m_empno),
            make_column("received", &EmpBonus::m_received),
            make_column("type", &EmpBonus::m_type),
            foreign_key(&EmpBonus::m_empno).references(&Employee::m_empno)),
        make_table("Artists",
            make_column("id", &Artist::m_id, primary_key(), autoincrement()),
            make_column("name", &Artist::m_name)),
        make_table("Albums",
            make_column("id", &Album::m_id, primary_key(), autoincrement()),
            make_column("artist_id", &Album::m_artist_id),
            foreign_key(&Album::m_artist_id).references(&Artist::m_id)));




	storage.sync_schema();

	storage.remove_all<Album>();
	storage.remove_all<Artist>();
	storage.remove_all<EmpBonus>();
	storage.remove_all<Employee>();
	storage.remove_all<Department>();

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
		storage.replace_range(art.begin(), art.end());
		storage.replace_range(albums.begin(), albums.end());
		storage.replace_range(des.begin(), des.end());
		storage.replace_range(vec.begin(), vec.end());
		storage.insert_range(bonuses.begin(), bonuses.end());
	}
	catch (std::exception& ex)
	{
		auto s = ex.what();
		std::ignore = s;
	}





    struct NamesAlias : alias_tag {
        static const std::string& get() {
            static const std::string res = "ENAME_AND_DNAME";
            return res;
        }
    };


    auto statement = storage.prepare(
        select(union_all(
            select(columns(as<NamesAlias>(&Department::m_deptname), as_optional(&Department::m_deptno))),
            select(union_all(
                select(columns(quote("--------------------"), std::optional<int>())),
                select(columns(as<NamesAlias>(&Employee::m_ename), as_optional(&Employee::m_deptno))))))));

    auto sql = statement.expanded_sql();
    static auto rows = storage.execute(statement);
    return rows;
}

auto full_outer_join()
{
    struct Dog
    {
        std::optional<std::string> type;
        std::optional<std::string> color;
    };


    struct Cat
    {
        std::optional<std::string> type;
        std::optional<std::string> color;
    };

    using namespace sqlite_orm;

    auto storage = make_storage(
        { "full_outer.sqlite" },
        make_table("Dogs", make_column("type", &Dog::type), make_column("color", &Dog::color)),
        make_table("Cats", make_column("type", &Cat::type), make_column("color", &Cat::color)));

    storage.sync_schema();
    storage.remove_all<Dog>();
    storage.remove_all<Cat>();

    storage.insert(into<Dog>(), columns(&Dog::type, &Dog::color), values(std::make_tuple("Hunting", "Black"), std::make_tuple("Guard", "Brown")));
    storage.insert(into<Cat>(), columns(&Cat::type, &Cat::color), values(std::make_tuple("Indoor", "White"), std::make_tuple("Outdoor", "Black")));


    auto left = select(columns(&Dog::type, &Dog::color, &Cat::type, &Cat::color), left_join<Cat>(using_(&Cat::color)));
    auto right = select(columns(&Dog::type, &Dog::color, &Cat::type, &Cat::color), from<Cat>(), left_join<Dog>(using_(&Dog::color)), where(is_null(&Dog::color)));




    /*  FULL OUTER JOIN simulation:
     *
            SELECT d.type,
            d.color,
            c.type,
            c.color
            FROM dogs d
            LEFT JOIN cats c USING(color)
            UNION ALL
            SELECT d.type,
            d.color,
            c.type,
            c.color
            FROM cats c
            LEFT JOIN dogs d USING(color)
            WHERE d.color IS NULL;
    */
    static auto rows = storage.select(
        union_all(select(columns(&Dog::type, &Dog::color, &Cat::type, &Cat::color),
            left_join<Cat>(using_(&Cat::color))),
            select(columns(&Dog::type, &Dog::color, &Cat::type, &Cat::color), from<Cat>(),
                left_join<Dog>(using_(&Dog::color)), where(is_null(&Dog::color)))));


    return rows;
}





template <typename T>
void GenericGrid::InitializeGridRendimientos(const T& t)
{
// #define distinct
    // auto rows = storage.select(distinct(columns(&Employee::address, &Employee::name)), from<Employee>(), where(t));
    // auto rows = full_outer_join();
    auto rows = as_optional();

    std::vector<std::string> headers{ "ENAME_AND_DNAME", "DEPTNO"}; // "TYPE", "COLOR", "TYPE", "COLOR"


	m_grid_displayer.reset(new JoinedGridDisplayer<decltype(rows[0]), IntegerList<>, IntegerList<>>(m_generic_grid, std::move(rows), std::move(headers)));
	m_grid_displayer->display();
}


void GenericGrid::PostNcDestroy()
{
    // TODO: Add your specialized code here and/or call the base class

    CDialog::PostNcDestroy();
    if (!m_is_modal)
    {
        AfxGetMainWnd()->SendMessage(WM_USER_DIALOG_DESTROYED, IDD_GenericGrid, 0);
        delete this;
    }
}




INT_PTR GenericGrid::DoModal()
{
    // TODO: Add your specialized code here and/or call the base class
    m_is_modal = true;
    return CDialog::DoModal();
}

bool GenericGrid::isModal() const noexcept
{
    return ! GetParent()->IsWindowEnabled();
}

void GenericGrid::OnOK()
{
    // TODO: Add your specialized code here and/or call the base class
    if (m_is_modal)
        CDialog::OnOK();
    else
        DestroyWindow();
}


void GenericGrid::OnCancel()
{
    // TODO: Add your specialized code here and/or call the base class
    if (m_is_modal)
        CDialog::OnCancel();
    else
        DestroyWindow();
}
