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

, m_asprose_claim_number(_T(""))
, m_asprose_case_number(_T(""))
, m_amount_presented(_T(""))
, m_other_system_id(_T(""))
, m_ins_claim_number(_T(""))
, m_total_claim_amount(_T(""))
, m_comments(_T(""))
, m_ole_date_time(COleDateTime::GetCurrentTime())
, m_patient_chosen(_T(""))
, m_ack_radio(0)
, m_id_claim(0)
, m_claim_id(0)
, m_sent(false)
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
	using namespace std::chrono;
	using namespace std::chrono_literals;

	// year_month_day ymd{ static_cast<year>(2021), static_cast<month>(8), static_cast<day>(31) };
	year_month_day ymd{ 2021y / 8 / 31d };
	m_std_date = ymd;

	{
		year_month_day ymd{ static_cast<year>(2021), static_cast<month>(9), static_cast<day>(6) };
		m_submission_date = ymd;
	}
	m_asprose_case_number_s = "ASP4500";

	m_patient_chosen = "Juan Dent Herrera";

	// static int index = 0;
	auto pDoc = GetDocument();
	m_claim = pDoc->get_claim();
	int id;

	CFormView::DoDataExchange(pDX);

	// auto pDoc = GetDocument();
	// m_claim = pDoc->get_claim();

	// from controls to data members
	if (pDX->m_bSaveAndValidate)
	{
		// auto patient = m_patientsCB.get_by_index(index);
		auto patient = m_patientsCB.current();
		pDoc->set_patient(patient);
		auto doctor = m_doctorsCB.current();
		pDoc->set_doctor(doctor);
		auto med = m_medicationsCB.current();
		pDoc->set_medication(med);

		// auto sdate = GetDate(m_start_date);
		// claim->start_date = sdate;
		//
		// auto sub_date = GetDate(m_submitted_date);
		// claim->submission_date = sub_date;
		// auto id = to_int(m_claim_id);
		// claim->id = id;
		// claim->amount = to_double(m_total_claim_amount);
		// // claim->

		if (m_claim_id == -1)
		{
			storage.insert(*m_claim);
		}
		else
		{
			storage.update(*m_claim);
		}
		// pDoc->changes_applied();
	}
	else
	{
		auto pt = pDoc->get_patient();
		m_patientsCB.select(pt);

		// m_claim_id = to_cstr(pDoc->get_id());
		// SetDate(m_start_date, pDoc->get_start_date());
		// SetDate(m_submitted_date, pDoc->get_submission_date());
	}

	DDX_Control(pDX, IDC_C_PATIENTS, m_patients);
	DDX_DateTimeCtrl(pDX, IDC_START_DATE, m_start_date);
	DDX_Check(pDX, IDC_C_SENT, m_sent);
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

	m_claim = GetDocument()->m_claim;

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
