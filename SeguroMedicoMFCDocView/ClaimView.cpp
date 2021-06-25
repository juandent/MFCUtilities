// ClaimView.cpp : implementation file
//

#include "pch.h"
#include "SeguroMedicoMFCDocView.h"
#include "ClaimView.h"

#include "ClaimDoc.h"


// ClaimView

IMPLEMENT_DYNCREATE(ClaimView, CFormView)

ClaimView::ClaimView()
	: CFormView(IDD_ClaimView),
	m_patientsCB(m_patients, [](Patient& patient)
		{
			return Util::to_cstring(patient.simple_dump());
		}),
	m_doctorsCB(m_doctors, [](Doctor& doctor)
	{
			return Util::to_cstring(doctor.simple_dump());
	}),
m_medicationsCB(m_medicamentos, [](Medication& med)
{
		return Util::to_cstring(med.simple_dump());
})

, m_claim_id(_T(""))
, m_asprose_claim_number(_T(""))
, m_asprose_case_number(_T(""))
, m_amount_presented(_T(""))
, m_other_system_id(_T(""))
, m_ins_claim_number(_T(""))
, m_total_claim_amount(_T(""))
, m_comments(_T(""))
{

}

ClaimView::~ClaimView()
{
}

ClaimDoc* ClaimView::GetDocument()
{
	return static_cast<ClaimDoc*>(CFormView::GetDocument());
}


void ClaimView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_E_ID_CLAIM, m_id_claim);
	DDX_Control(pDX, IDC_C_PATIENTS, m_patients);
	DDX_Control(pDX, IDC_CLAIMS_GRID, m_grid1);
	DDX_Control(pDX, IDC_START_DATE, m_start_date);
	DDX_Control(pDX, IDC_C_DOCTORES, m_doctors);
	DDX_Control(pDX, IDC_C_MEDICAMENTOS, m_medicamentos);
	DDX_Text(pDX, IDC_E_ID_CLAIM, m_claim_id);
	DDX_Control(pDX, IDC_FECHA_ENTREGADO, m_submitted_date);
	DDX_Text(pDX, IDC_E_ASPROSE_CLAIM_NUMBER, m_asprose_claim_number);

	DDX_Text(pDX, IDC_E_ASPROSE_CASE_NUMBER2, m_asprose_case_number);
	DDX_Text(pDX, IDC_E_ASPROSE_AMOUNT_PRESENTED, m_amount_presented);
	DDX_Control(pDX, IDC_R_NONE, m_acknowledgement);
	DDX_Text(pDX, IDC_E_OTHER_SYSTEM_ID, m_other_system_id);
	DDX_Text(pDX, IDC_E_INS_CLAIM_NUMBER, m_ins_claim_number);
	DDX_Control(pDX, IDC_L_CLAIM_LIST, m_claims);
	DDX_Text(pDX, IDC_E_TOTAL_CLAIM_AMOUNT, m_total_claim_amount);
	DDX_Text(pDX, IDC_E_COMMENT, m_comments);


	
	auto pDoc = GetDocument();
	Claim* claim = pDoc->get_claim();

	if (pDX->m_bSaveAndValidate)
	{
		auto patient = m_patientsCB.current();
		pDoc->set_patient(patient);
		auto doctor = m_doctorsCB.current();
		pDoc->set_doctor(doctor);
		auto med = m_medicationsCB.current();
		pDoc->set_medication(med);
		
		auto sdate = GetDate(m_start_date);
		claim->start_date = sdate;
		
		auto sub_date = GetDate(m_submitted_date);
		claim->submission_date = sub_date;
		auto id = to_int(m_claim_id);
		claim->id = id;
		claim->amount = to_double(m_total_claim_amount);
		claim->

		
		
		pDoc->changes_applied();
	}
	else
	{
		auto pt = pDoc->get_patient();
		m_patientsCB.select(pt);

		m_claim_id = to_cstr(pDoc->get_id());
		SetDate(m_start_date, pDoc->get_start_date());
		SetDate(m_submitted_date, pDoc->get_submission_date());
	}


}

BEGIN_MESSAGE_MAP(ClaimView, CFormView)
	ON_BN_CLICKED(ID_APPLY, &ClaimView::OnBnClickedApply)
	// ON_CBN_SELCHANGE(IDC_C_PATIENTS, &ClaimView::OnCbnSelchangeCPatients)
	// ON_NOTIFY(DTN_DATETIMECHANGE, IDC_START_DATE, &ClaimView::OnDtnDatetimechangeStartDate)
END_MESSAGE_MAP()


// ClaimView diagnostics

#ifdef _DEBUG
void ClaimView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void ClaimView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// ClaimView message handlers


void ClaimView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	// TODO: Add your specialized code here and/or call the base class
	CFormView::OnUpdate(pSender, lHint, pHint);
}


void ClaimView::OnBnClickedApply()
{
	// TODO: Add your control notification handler code here
	UpdateData(1);
}


void ClaimView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_patientsCB.loadLB();
	m_medicationsCB.loadLB();
	m_doctorsCB.loadLB();

	// TODO: Add your specialized code here and/or call the base class
}


// void ClaimView::OnCbnSelchangeCPatients()
// {
// 	// TODO: Add your control notification handler code here
// 	auto rec= m_patientsCB.current();
// 	auto* pDoc = GetDocument();
// 	pDoc->set_patient(rec);
// }


// void ClaimView::OnDtnDatetimechangeStartDate(NMHDR* pNMHDR, LRESULT* pResult)
// {
// 	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
// 	// TODO: Add your control notification handler code here
// 	// m_start_date
// 	*pResult = 0;
// }
