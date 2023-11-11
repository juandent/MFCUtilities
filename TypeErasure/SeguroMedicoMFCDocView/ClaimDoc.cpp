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

using als_c = alias_c<Claim>;
using als_p = alias_p<Patient>;
using als_d = alias_d<Doctor>;
using als_s = alias_s<Specialty>;
using als_m = alias_m<Medication>;
using als_i = alias_i<Invoice>;
using als_j = alias_j<INSResponse>;
using als_k = alias_k<INSResponseLine>;
using als_l = alias_l<Claim>;
using als_q = alias_q<Invoice>;


void ClaimDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		if (m_claim.id == -1)
		{
			storage.insert(m_claim);
		}
		else
		{
			storage.replace(m_claim);
		}
		for( auto& rec : m_claim_invoices)
		{
			rec.fkey_claim = m_claim.id;
			storage.replace(rec);
		}
	}
	else
	{
		// TODO: add loading code here
		if (m_claim_id == -1)	return;
		
		m_claim = storage.get<Claim>(m_claim_id);
		m_claim_invoices = storage.get_all<Invoice>(where(c(alias_column<als_i>(&Invoice::fkey_claim)) == m_claim.id));
		auto name = storage.column_name(&Invoice::fkey_INSResponse);
		
		m_claim_invoices_headers = { "A","B", "C","D", "E", "F", "G" };
		
	}
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
	if (!patient)
	{
		Storage::report_error(L"Paciente no escogido");
		return;
	}
	m_claim.fkey_patient = patient->id;
	SetModifiedFlag(true);
}

void ClaimDoc::set_medication(const std::optional<Medication>& medication)
{
	if (!medication) return;
	m_claim.fkey_medication = medication->id;
	SetModifiedFlag(true);
}

// ClaimDoc commands
void ClaimDoc::add_invoice(Invoice& invoice)
{
	if (m_claim.id < 0)
	{
		Storage::report_error(L"Claim debe estar almacenado para agregarle facturas");
		return;
	}
	
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

