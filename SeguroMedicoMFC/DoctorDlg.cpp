// DoctorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SeguroMedicoMFC.h"
#include "DoctorDlg.h"
#include "afxdialogex.h"
#include "SpecialtyDlg.h"


// DoctorDlg dialog

IMPLEMENT_DYNAMIC(DoctorDlg, CDialog)

DoctorDlg::DoctorDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DoctorDlg, pParent),
m_doctoresLB(m_list_doctores, [](Doctor& doctor)
{
		return Util::to_cstring(doctor.simple_dump());
}),
m_specialtiesCB(m_cblist_Specialties, [](Specialty& specialty)
{
		return Util::to_cstring(specialty.simple_dump());
})
{

}

DoctorDlg::~DoctorDlg()
{
}

void DoctorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_E_ID_DOCTOR, m_id_doctor);
	DDX_Control(pDX, IDC_E_PRIMER_NOMBRE, m_first_name);
	DDX_Control(pDX, IDC_E_APELLIDOS, m_last_name);
	DDX_Control(pDX, IDC_CB_SPECIALIDADES, m_cblist_Specialties);
	DDX_Control(pDX, IDC_L_DOCTORES, m_list_doctores);
}


BEGIN_MESSAGE_MAP(DoctorDlg, CDialog)
	ON_LBN_SELCHANGE(IDC_L_DOCTORES, &DoctorDlg::OnLbnSelchangeLDoctores)
	ON_CBN_SELCHANGE(IDC_CB_SPECIALIDADES, &DoctorDlg::OnCbnSelchangeCbSpecialidades)
	ON_BN_CLICKED(ID_APLICAR_DOCTOR, &DoctorDlg::OnBnClickedAplicarDoctor)
	ON_BN_CLICKED(ID_NEW_DOCTOR, &DoctorDlg::OnBnClickedNewDoctor)
	ON_BN_CLICKED(ID_BORRAR_DOCTOR, &DoctorDlg::OnBnClickedBorrarDoctor)
	ON_BN_CLICKED(IDOK, &DoctorDlg::OnBnClickedOk)
	ON_BN_CLICKED(ID_EDIT_SPECIALTIES, &DoctorDlg::OnBnClickedEditSpecialties)
END_MESSAGE_MAP()


// DoctorDlg message handlers


BOOL DoctorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	Refresh();
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void DoctorDlg::OnLbnSelchangeLDoctores()
{
	// TODO: Add your control notification handler code here
	auto doctor = m_doctoresLB.current();
	SetText(m_id_doctor, doctor->id);
	SetText(m_first_name, doctor->first_name);
	SetText(m_last_name, doctor->last_name);
	m_specialtiesCB.select(doctor->fkey_specialty);
}


void DoctorDlg::OnCbnSelchangeCbSpecialidades()
{
	// TODO: Add your control notification handler code here
}


void DoctorDlg::OnBnClickedAplicarDoctor()
{
	// TODO: Add your control notification handler code here
	auto doctor = getCurrent<Doctor>(m_id_doctor);

	CString rNombre, rApellidos;
	m_first_name.GetWindowTextW(rNombre);
	m_last_name.GetWindowTextW(rApellidos);
	
	if (rNombre.IsEmpty() || rApellidos.IsEmpty())
	{
		MessageBoxW(L"Falta el nombre o apellido del doctor");
		return;
	};

	std::optional<Specialty> specialty = m_specialtiesCB.current();
	if( ! specialty)
	{
		MessageBoxW(L"Falta escoger especialidad");
		return;
	}

	auto name = Util::from_cstring(rNombre);
	auto last_name = Util::from_cstring(rApellidos);

	if (!doctor)
	{
		auto whereClause = (c(&Doctor::first_name) == name.c_str() && c(&Doctor::last_name) == last_name.c_str());

		auto doctor_by_value = m_doctoresLB.exists(whereClause);
		doctor = doctor_by_value;
	}

	if (!doctor)	// insert
	{
		doctor = m_doctoresLB.insert(name, last_name, specialty->id);
		m_doctoresLB.insert_into_listbox(*doctor);
	}
	else                // update
	{
		doctor->first_name = name;
		doctor->last_name = last_name;
		doctor->fkey_specialty = specialty->id;
		m_doctoresLB.update(*doctor);
	}
	m_doctoresLB.loadLBOrderBy(&Doctor::last_name);
	m_doctor = doctor;
	setIdFromRecord<Doctor>(m_id_doctor, m_doctor->id);
}


void DoctorDlg::OnBnClickedNewDoctor()
{
	// TODO: Add your control notification handler code here
	m_doctor = std::nullopt;
	NullText(m_id_doctor);
	NullText(m_first_name);
	NullText(m_last_name);
	m_specialtiesCB.SetCurSel(-1);
}


void DoctorDlg::OnBnClickedBorrarDoctor()
{
	// TODO: Add your control notification handler code here
	if (m_doctoresLB.delete_current_sel())
	{
		OnBnClickedNewDoctor();
	}
}


void DoctorDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}


void DoctorDlg::OnBnClickedEditSpecialties()
{
	// TODO: Add your control notification handler code here
	SpecialtyDlg dlg;
	dlg.DoModal();
	Refresh();
}

void DoctorDlg::Refresh()
{
	m_specialtiesCB.loadLBOrderBy(&Specialty::name);
	m_doctoresLB.loadLBOrderBy(&Doctor::last_name);
}

