// PaisesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ExpenseManagerSQLite.h"
#include "PaisesDlg.h"
#include "afxdialogex.h"
#include "RecordLinks.h"


// PaisesDlg dialog

IMPLEMENT_DYNAMIC(PaisesDlg, CDialog)

PaisesDlg::PaisesDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_PaisesDlg, pParent),
	m_paisLB{ m_list_paises, [](Pais& pais)
	{
		return JD::to_cstring(pais.id_pais) + L" - " + JD::to_cstring(pais.name);
	}}
{

}

PaisesDlg::~PaisesDlg()
{
}

void PaisesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_L_PAISES, m_list_paises);
	DDX_Control(pDX, IDC_E_ID_PAIS, m_id_pais);
	DDX_Control(pDX, IDC_E_NOMBRE_PAIS, m_nombre_pais);
}


BEGIN_MESSAGE_MAP(PaisesDlg, CDialog)
	ON_BN_CLICKED(IDOK, &PaisesDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_B_APLICAR_PAISES, &PaisesDlg::OnBnClickedBAplicarPaises)
	ON_LBN_SELCHANGE(IDC_L_PAISES, &PaisesDlg::OnLbnSelchangeLPaises)
	ON_BN_CLICKED(ID_B_BORRAR, &PaisesDlg::OnBnClickedBBorrar)
	ON_BN_CLICKED(IDC_B_UPDATE_PAIS, &PaisesDlg::OnBnClickedBUpdatePais)
END_MESSAGE_MAP()


// PaisesDlg message handlers


BOOL PaisesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_paisLB.loadLB();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void PaisesDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}


void PaisesDlg::OnBnClickedBAplicarPaises()
{
	// TODO: Add your control notification handler code here
	CString rStr;
	m_nombre_pais.GetWindowTextW(rStr);

	if (rStr.IsEmpty())
	{
		MessageBoxW(L"Falta el nombre del pais");
		return;
	}

	auto name = JD::from_cstring(rStr);
	auto whereClause = c(&Pais::name) == name.c_str();

	std::optional<Pais> pais = m_paisLB.exists(whereClause, &Pais::id_pais, &Pais::name);

	if (!pais)    // insert
	{
		pais = m_paisLB.insert(name);
		m_paisLB.insert_into_listbox(*pais);
		m_id_pais.SetWindowTextW(JD::to_cstring(pais->id_pais));
	}
	else           // update
	{
		pais->name = name;
		m_paisLB.update(*pais);
	}
	m_paisLB.loadLB();
	m_pais = pais;
}


void PaisesDlg::OnLbnSelchangeLPaises()
{
	// TODO: Add your control notification handler code here
	auto pais = m_paisLB.current();

	CString rNombre;

	m_nombre_pais.SetWindowTextW(JD::to_cstring(pais->name));
	m_id_pais.SetWindowTextW(JD::to_cstring(pais->id_pais));
}



void PaisesDlg::OnBnClickedBBorrar()
{
	// TODO: Add your control notification handler code here
	auto pais = m_paisLB.current();
	if (pais)
	{
		bool has_links = RecordLinks::has_links(*pais);
		if (!has_links)
		{
			m_paisLB.delete_current_sel();
		}
	}
}



void PaisesDlg::OnBnClickedBUpdatePais()
{
#if 0
	// TODO: Add your control notification handler code here
	auto pais = m_paisLB.current();
	if( ! pais)
	{
		MessageBoxW(L"Falta escoger pais");
		return;
	}
	CString rPais;
	m_nombre_pais.GetWindowTextW(rPais);
	pais->name = JD::from_cstring(rPais);

	m_paisLB.update(*pais);
	m_paisLB.loadLB();
#endif
}
