// CategoryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExpenseManagerSQLite.h"
#include "CategoryDlg.h"
#include "afxdialogex.h"
#include "Data_Tier.h"
#include "RecordLinks.h"

using namespace sqlite_orm;

// CategoryDlg dialog

IMPLEMENT_DYNAMIC(CategoryDlg, CDialog)

CategoryDlg::CategoryDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CategoryDlg, pParent),
	m_categoriasLB{ m_list_categorias, [](Categoria& categoria)
	{
		return JD::to_cstring(categoria.id_categoria) + L" - " + JD::to_cstring(categoria.name) + (categoria.is_expense_or_income ? L" - Real" : L" - Ignore");
	}}
{

}

CategoryDlg::~CategoryDlg()
{
}

void CategoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_E_CATEGORIA, m_nombre);
	DDX_Control(pDX, IDC_C_IS_REAL, m_gasto_o_ingreso_real);
	DDX_Control(pDX, IDC_L_CATEGORIAS, m_list_categorias);
}


BEGIN_MESSAGE_MAP(CategoryDlg, CDialog)
	ON_BN_CLICKED(ID_B_BORRAR, &CategoryDlg::OnBnClickedBBorrar)
	ON_BN_CLICKED(IDC_B_APLICAR_CATEGORY, &CategoryDlg::OnBnClickedBAplicarCategory)
	ON_BN_CLICKED(IDC_B_UPDATE, &CategoryDlg::OnBnClickedBUpdate)
END_MESSAGE_MAP()


// CategoryDlg message handlers


BOOL CategoryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_categoriasLB.loadLB();
	m_nombre.SetWindowTextW(JD::to_cstring(m_category_name));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CategoryDlg::OnBnClickedBBorrar()
{
	// TODO: Add your control notification handler code here
	auto categoria = m_categoriasLB.current();
	if (!categoria)
	{
		MessageBoxW(L"Falta escoger categoría");
		return;
	}
	bool has_links = RecordLinks::has_links(*categoria);
	if (!has_links)
	{
		m_categoriasLB.delete_current_sel();
	}
}


void CategoryDlg::OnBnClickedBAplicarCategory()
{
	// TODO: Add your control notification handler code here
	CString rNombre;
	m_nombre.GetWindowTextW(rNombre);

	if (rNombre.IsEmpty())
	{
		MessageBoxW(L"Falta el nombre de la categoria");
		return;
	};

	int is_real = m_gasto_o_ingreso_real.GetCheck();
	
	auto nombre = JD::from_cstring(rNombre);

	auto whereClause = (c(&Categoria::name) == nombre.c_str());

	std::optional<Categoria> categoria = m_categoriasLB.exists(whereClause, &Categoria::id_categoria, &Categoria::name);

	if (!categoria)	// new categoria
	{
		categoria = m_categoriasLB.insert(nombre, is_real);
		m_categoriasLB.insert_into_listbox(*categoria);
	}
	else          //  update
	{
		categoria->name = nombre;
		categoria->is_expense_or_income = is_real;
		m_categoriasLB.update(*categoria);
	}
	m_categoriasLB.loadLB();
	m_category = categoria;
}


void CategoryDlg::OnBnClickedBUpdate()
{
#if 0
	// TODO: Add your control notification handler code here
	std::optional<Categoria> cat = m_categoriasLB.current();
	if (!cat)
	{
		MessageBoxW(L"Falta escoger la categoria");
		return;
	}

	bool changes = false;
	CString rNombre;
	m_nombre.GetWindowTextW(rNombre);
	
	if (!rNombre.IsEmpty())
	{
		auto nombre = JD::from_cstring(rNombre);
		cat->name = nombre;
		changes = true;
	}

	int is_real = m_gasto_o_ingreso_real.GetCheck();
	
	if (  cat->is_expense_or_income != is_real)
	{
		cat ->is_expense_or_income = is_real;
		changes = true;
	}

	if (!changes)
	{
		MessageBox(L"No hay cambios en este objeto");
		return;
	}

	m_categoriasLB.update(*cat);
	m_categoriasLB.loadLB();

	m_category = cat;
#endif
}
