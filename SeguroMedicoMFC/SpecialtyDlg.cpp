// SpecialtyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SeguroMedicoMFC.h"
#include "SpecialtyDlg.h"
#include "afxdialogex.h"


// SpecialtyDlg dialog

IMPLEMENT_DYNAMIC(SpecialtyDlg, CDialog)

SpecialtyDlg::SpecialtyDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_SpecialtyDlg, pParent),
	m_specialtyLB{ m_specialty_list, [](Specialty& specialty)
	{
		return util::to_cstring(specialty.simple_dump());
	}}
{

}

SpecialtyDlg::~SpecialtyDlg()
{
}

void SpecialtyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_E_ID_SPECIALTY, m_id_specialty);
	DDX_Control(pDX, IDC_E_NOMBRE_SPECIALTY, m_nombre_specialty);
	DDX_Control(pDX, IDC_L_SPECIALTIES, m_specialty_list);
}


BEGIN_MESSAGE_MAP(SpecialtyDlg, CDialog)
	ON_BN_CLICKED(ID_APPLY, &SpecialtyDlg::OnBnClickedApply)
	ON_BN_CLICKED(ID_BORRAR, &SpecialtyDlg::OnBnClickedBorrar)
	ON_BN_CLICKED(ID_CERRAR, &SpecialtyDlg::OnBnClickedCerrar)
	ON_LBN_SELCHANGE(IDC_L_SPECIALTIES, &SpecialtyDlg::OnLbnSelchangeLSpecialties)
	ON_BN_CLICKED(ID_NUEVO, &SpecialtyDlg::OnBnClickedNuevo)
END_MESSAGE_MAP()


// SpecialtyDlg message handlers


BOOL SpecialtyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	Refresh();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void SpecialtyDlg::Refresh()
{
	m_specialtyLB.loadLBOrderBy(&Specialty::name);
}

void SpecialtyDlg::OnBnClickedApply()
{
	// TODO: Add your control notification handler code here
	auto specialty = getCurrent<Specialty>(m_id_specialty);

	CString rNombre;
	m_nombre_specialty.GetWindowTextW(rNombre);

	if (rNombre.IsEmpty())
	{
		MessageBoxW(L"Falta el nombre de la especialidad");
		return;
	};


	auto name = util::from_cstring(rNombre);

	if( ! specialty)
	{
		auto whereClause = (c(&Specialty::name) == name.c_str());

		auto specialty_by_value = m_specialtyLB.exists(whereClause);
		specialty = specialty_by_value;
	}

	if( ! specialty)	// insert
	{
		specialty = m_specialtyLB.insert(name);
		m_specialtyLB.insert_into_listbox(*specialty);
	}
	else                // update
	{
		specialty->name = name;
		m_specialtyLB.update(*specialty);
	}
	m_specialtyLB.loadLBOrderBy(&Specialty::name);
	m_specialty = specialty;
	setIdFromRecord<Specialty>(m_id_specialty, m_specialty->id);
}


void SpecialtyDlg::OnBnClickedBorrar()
{
	// TODO: Add your control notification handler code here
	if (m_specialtyLB.delete_current_sel())
	{
		OnBnClickedNuevo();
	}
}


void SpecialtyDlg::OnBnClickedCerrar()
{
	// TODO: Add your control notification handler code here
	CDialog::EndDialog(0);
}


void SpecialtyDlg::OnLbnSelchangeLSpecialties()
{
	// TODO: Add your control notification handler code here
	auto specialty = m_specialtyLB.current();
	if (!specialty)  return;
	SetText(m_id_specialty, specialty->id);
	SetText(m_nombre_specialty, specialty->name);

}


void SpecialtyDlg::OnBnClickedNuevo()
{
	// TODO: Add your control notification handler code here
	m_specialty = std::nullopt;
	NullText(m_id_specialty );
	NullText(m_nombre_specialty);
}
