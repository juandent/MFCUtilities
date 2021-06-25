// ClaimDoc.cpp : implementation file
//

#include "pch.h"
#include "SeguroMedicoMFCDocView.h"
#include "ClaimDoc.h"


// ClaimDoc

IMPLEMENT_DYNCREATE(ClaimDoc, CDocument)

ClaimDoc::ClaimDoc()
{
}



BOOL ClaimDoc::OnNewDocument()
{
	using namespace std::chrono;
	
	if (!CDocument::OnNewDocument())
		return FALSE;
	
	
	return TRUE;
}

ClaimDoc::~ClaimDoc()
{
}


BEGIN_MESSAGE_MAP(ClaimDoc, CDocument)
END_MESSAGE_MAP()


// ClaimDoc diagnostics

#ifdef _DEBUG
void ClaimDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void ClaimDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// ClaimDoc serialization

void ClaimDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
#if TEST
	m_claim = storage.get<Claim>(1);
	auto inv = get_invoices(&Invoice::id, &Invoice::description);
#endif
}
#endif


std::optional<Doctor> ClaimDoc::get_doctor()
{
	return get_Card_1 < Doctor, &Doctor::id, Claim, &Claim::fkey_doctor>(m_claim);
}

std::optional<Patient> ClaimDoc::get_patient()
{
	return get_Card_1<Patient, &Patient::id, Claim, &Claim::fkey_patient>(m_claim);
}

std::optional<Medication> ClaimDoc::get_medication()
{
	return get_Card_1<Medication, &Medication::id, Claim, &Claim::fkey_medication>(m_claim);
}

void ClaimDoc::set_doctor(const std::optional<Doctor>& doc)
{
	if (!doc) return;
	m_claim.fkey_doctor = doc->id;
	SetModifiedFlag(true);
	UpdateAllViews(nullptr);
}

void ClaimDoc::set_patient(const std::optional<Patient>& patient)
{
	if (!patient)	return;
	m_claim.fkey_patient = patient->id;
	SetModifiedFlag(true);
	UpdateAllViews(nullptr);
}

void ClaimDoc::set_medication(const std::optional<Medication>& medication)
{
	if (!medication) return;
	m_claim.fkey_medication = medication->id;
	SetModifiedFlag(true);
	UpdateAllViews(nullptr);
}

// ClaimDoc commands
void ClaimDoc::add_invoice(Invoice& invoice)
{
	assert(m_claim.id >= 0);
	
	auto res = std::find_if(m_claim_invoices.cbegin(), m_claim_invoices.cend(), [&invoice](const Invoice& inv)
		{
			return inv.id == invoice.id;
		});
	// if not found:
	if(res == m_claim_invoices.cend())
	{
		invoice.fkey_claim = m_claim.id;
		m_claim_invoices.push_back(invoice);
		
		SetModifiedFlag(true);
		UpdateAllViews(nullptr);
	}
}

