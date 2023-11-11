// MedicationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SeguroMedicoMFC.h"
#include "MedicationDlg.h"
#include "afxdialogex.h"


// MedicationDlg dialog

IMPLEMENT_DYNAMIC(MedicationDlg, CDialog)

MedicationDlg::MedicationDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_MedicationDlg, pParent),
m_medicationLB(m_list_medications, [](Medication& med)
{
		return util::to_cstring(med.simple_dump());
})
{

}

MedicationDlg::~MedicationDlg()
{
}

void MedicationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_E_ID_MEDICATION, m_id_medication);
	DDX_Control(pDX, IDC_E_NOMBRE_MEDICATION, m_name);
	DDX_Control(pDX, IDC_L_MEDICATIONS, m_list_medications);
}


BEGIN_MESSAGE_MAP(MedicationDlg, CDialog)
	ON_BN_CLICKED(ID_APPLY, &MedicationDlg::OnBnClickedApply)
	ON_BN_CLICKED(ID_NUEVO, &MedicationDlg::OnBnClickedNuevo)
	ON_BN_CLICKED(ID_BORRAR, &MedicationDlg::OnBnClickedBorrar)
	ON_BN_CLICKED(ID_CERRAR, &MedicationDlg::OnBnClickedCerrar)
	ON_LBN_SELCHANGE(IDC_L_MEDICATIONS, &MedicationDlg::OnLbnSelchangeLMedications)
END_MESSAGE_MAP()


// MedicationDlg message handlers


BOOL MedicationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	Refresh();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void MedicationDlg::Refresh()
{
	m_medicationLB.loadLBOrderBy(&Medication::name);
}



void MedicationDlg::OnBnClickedApply()
{
	// TODO: Add your control notification handler code here
	auto medication = getCurrent<Medication>(m_id_medication);

	CString rNombre;
	m_name.GetWindowTextW(rNombre);

	if (rNombre.IsEmpty())
	{
		MessageBoxW(L"Falta el nombre de la medicación");
		return;
	};


	auto name = util::from_cstring(rNombre);

	if (!medication)
	{
		auto whereClause = (c(&Medication::name) == name.c_str());

		medication = m_medicationLB.exists(whereClause);
	}

	if (!medication)	// insert
	{
		medication = m_medicationLB.insert(name);
		m_medicationLB.insert_into_listbox(*medication);
	}
	else                // update
	{
		medication->name = name;
		m_medicationLB.update(*medication);
	}
	m_medicationLB.loadLBOrderBy(&Medication::name);
	m_medication = medication;
	setIdFromRecord<Medication>(m_id_medication, m_medication->id);
}


void MedicationDlg::OnBnClickedNuevo()
{
	// TODO: Add your control notification handler code here
	m_medication = std::nullopt;
	NullText(m_id_medication);
	NullText(m_name);
}


void MedicationDlg::OnBnClickedBorrar()
{
	// TODO: Add your control notification handler code here
	if (m_medicationLB.delete_current_sel())
	{
		OnBnClickedNuevo();
	}
}


void MedicationDlg::OnBnClickedCerrar()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}


void MedicationDlg::OnLbnSelchangeLMedications()
{
	// TODO: Add your control notification handler code here
	auto medication = m_medicationLB.current();
	if (!medication) return;
	SetText(m_id_medication, medication->id);
	SetText(m_name, medication->name);
}
