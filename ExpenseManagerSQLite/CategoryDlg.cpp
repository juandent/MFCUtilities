// CategoryDlg.cpp : implementation file
//

#include "stdafx.h"

import Util;

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
		return Util::to_cstring(categoria.id_categoria) + L" - " + Util::to_cstring(categoria.name) + (categoria.is_expense_or_income ? L" - Real" : L" - Ignore");
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
	DDX_Control(pDX, IDC_E_ID_CATEGORIA, m_id_categoria);
}


BEGIN_MESSAGE_MAP(CategoryDlg, CDialog)
	ON_BN_CLICKED(ID_B_BORRAR, &CategoryDlg::OnBnClickedBBorrar)
	ON_BN_CLICKED(IDC_B_APLICAR_CATEGORY, &CategoryDlg::OnBnClickedBAplicarCategory)
	ON_BN_CLICKED(IDC_B_UPDATE, &CategoryDlg::OnBnClickedBUpdate)
	ON_LBN_SELCHANGE(IDC_L_CATEGORIAS, &CategoryDlg::OnLbnSelchangeLCategorias)
END_MESSAGE_MAP()


// CategoryDlg message handlers


BOOL CategoryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_categoriasLB.loadLBOrderBy(&Categoria::name);
	m_nombre.SetWindowTextW(Util::to_cstring(m_category_name));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CategoryDlg::OnBnClickedBBorrar()
{
	// TODO: Add your control notification handler code here
	m_categoriasLB.delete_current_sel();
}


void CategoryDlg::OnBnClickedBAplicarCategory()
{
	// TODO: Add your control notification handler code here
	auto categoria = getCurrent<Categoria>(m_id_categoria);

	CString rNombre;
	m_nombre.GetWindowTextW(rNombre);

	if (rNombre.IsEmpty())
	{
		MessageBoxW(L"Falta el nombre de la categoria");
		return;
	};

	int is_real = m_gasto_o_ingreso_real.GetCheck();
	
	auto nombre = Util::from_cstring(rNombre);

	if (! categoria)
	{
		auto whereClause = (c(&Categoria::name) == nombre.c_str());
		std::optional<Categoria> categoria_by_value = m_categoriasLB.exists(whereClause, &Categoria::id_categoria, &Categoria::name);

		if (categoria_by_value)
		{
			MessageBoxW(L"Categoria found for another primary key");
		}
		categoria = categoria_by_value;
	}


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

	setIdFromRecord<Categoria>(m_id_categoria, categoria->id_categoria);

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
		auto nombre = Util::from_cstring(rNombre);
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


void CategoryDlg::OnLbnSelchangeLCategorias()
{
	// TODO: Add your control notification handler code here
	auto categoria = m_categoriasLB.current();


	m_id_categoria.SetWindowTextW(Util::to_cstring(categoria->id_categoria));
	m_nombre.SetWindowTextW(Util::to_cstring(categoria->name));
	m_gasto_o_ingreso_real.SetCheck(categoria->is_expense_or_income);
}
