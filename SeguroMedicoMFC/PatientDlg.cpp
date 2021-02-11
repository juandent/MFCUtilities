// PatientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SeguroMedicoMFC.h"
#include "PatientDlg.h"
#include "afxdialogex.h"


// PatientDlg dialog

IMPLEMENT_DYNAMIC(PatientDlg, CDialog)

PatientDlg::PatientDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_PatientDlg, pParent),
m_patientLB(m_list_patients, [](Patient& patient)
{
		return Util::to_cstring(patient.simple_dump());
})
{

}

PatientDlg::~PatientDlg()
{
}

void PatientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_E_ID_PATIENT, m_id_patient);
	DDX_Control(pDX, IDC_E_PRIMER_NOMBRE, m_first_name);
	DDX_Control(pDX, IDC_E_APELLIDOS, m_last_name);
	DDX_Control(pDX, IDC_L_PACIENTES, m_list_patients);
}


BEGIN_MESSAGE_MAP(PatientDlg, CDialog)
	ON_BN_CLICKED(ID_APLICAR_PACIENTE, &PatientDlg::OnBnClickedAplicarPaciente)
	ON_BN_CLICKED(ID_NEW_PATIENT, &PatientDlg::OnBnClickedNewPatient)
	ON_BN_CLICKED(ID_BORRAR_PATIENT, &PatientDlg::OnBnClickedBorrarPatient)
	ON_BN_CLICKED(IDOK, &PatientDlg::OnBnClickedOk)
	ON_LBN_SELCHANGE(IDC_L_PACIENTES, &PatientDlg::OnLbnSelchangeLPacientes)
END_MESSAGE_MAP()


// PatientDlg message handlers


void PatientDlg::OnBnClickedAplicarPaciente()
{
	// TODO: Add your control notification handler code here
	auto patient = getCurrent<Patient>(m_id_patient);

	CString rNombre, rApellidos;
	m_first_name.GetWindowTextW(rNombre);
	m_last_name.GetWindowTextW(rApellidos);

	if (rNombre.IsEmpty() || rApellidos.IsEmpty())
	{
		MessageBoxW(L"Falta el nombre o apellido del paciente");
		return;
	};

	auto name = Util::from_cstring(rNombre);
	auto last_name = Util::from_cstring(rApellidos);

	if (!patient)
	{
		auto whereClause = (c(&Patient::first_name) == name.c_str() && c(&Patient::last_name) == last_name.c_str());

		patient = m_patientLB.exists(whereClause);
	}

	if (! patient)	// insert
	{
		patient = m_patientLB.insert(name, last_name);
		m_patientLB.insert_into_listbox(*patient);
	}
	else                // update
	{
		patient->first_name = name;
		patient->last_name = last_name;
		m_patientLB.update(*patient);
	}
	m_patientLB.loadLBOrderBy(&Patient::last_name);
	m_patient = patient;
	setIdFromRecord<Doctor>(m_id_patient, m_patient->id);
}


void PatientDlg::OnBnClickedNewPatient()
{
	// TODO: Add your control notification handler code here
	m_patient = std::nullopt;
	NullText(m_id_patient);
	NullText(m_first_name);
	NullText(m_last_name);
}


void PatientDlg::OnBnClickedBorrarPatient()
{
	// TODO: Add your control notification handler code here
	if (m_patientLB.delete_current_sel())
	{
		OnBnClickedNewPatient();
	}
}


void PatientDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}


BOOL PatientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	Refresh();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void PatientDlg::Refresh()
{
	m_patientLB.loadLBOrderBy(&Patient::last_name);
}



void PatientDlg::OnLbnSelchangeLPacientes()
{
	// TODO: Add your control notification handler code here
	auto patient = m_patientLB.current();
	SetText(m_id_patient, patient->id);
	SetText(m_first_name, patient->first_name);
	SetText(m_last_name, patient->last_name);
}
