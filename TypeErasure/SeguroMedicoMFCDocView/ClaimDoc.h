#pragma once

// ClaimDoc document
// #include "PersistentClasses.h"
// #include "Data.h"

class ClaimDoc : public CDocument
{
	DECLARE_DYNCREATE(ClaimDoc)
	friend class ClaimView;
	
	int m_claim_id;
	std::vector<std::string> m_claim_invoices_headers;
	Claim m_claim;
	std::vector<Invoice> m_claim_invoices;
public:
	ClaimDoc();

	virtual ~ClaimDoc();
#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
public:
	Claim* get_claim() { return &m_claim; }

	void changes_applied()
	{
		SetModifiedFlag(true);
		UpdateAllViews(nullptr);
	}
	
	// relationships:
	std::vector<Invoice> get_invoices(auto order_clause);
	template<typename ...order_clauses>
	std::vector<Invoice> get_invoices(order_clauses...clauses);

	// 0-1 side
	std::optional<Doctor> get_doctor();
	std::optional<Patient> get_patient();
	std::optional<Medication>  get_medication();

	// void set_claim( Claim& claim);
	void set_doctor(const std::optional<Doctor>& doc);
	// preferred
	void set_patient(const std::optional<Patient>& patient);

	void set_medication(const std::optional<Medication>& medication);

	void add_invoice(Invoice& invoice);

#if 0	
	void set_asprose_case_number(const std::string& number) { m_claim.asprose_case_number = number; }
	void set_asprose_claim_number(const std::string& claim_number) { m_claim.asprose_claim_number = claim_number; }
	void set_asprose_amount_presented(double d) { m_claim.asprose_amount_presented = d; }
	void set_ins_claim_number(const std::string& number) { m_claim.ins_claim_number = number; }
	void set_asprose_acknowledgement(int ack) { m_claim.asprose_acknowledgement_type = ack; }
	void set_comment(const std::string& comment) { m_claim.comment = comment; }
	void set_amount(double d) { m_claim.amount = d; }
	void set_other_system_id(const std::optional<int>& other_id) { m_claim.other_system_id = other_id; }
#endif

	
	/// scalar fields - gets
	int get_id() const { return m_claim.id; }
	bool get_sent() const { return m_claim.status != 0; }
	std::chrono::sys_days get_start_date() const { return m_claim.start_date; }
	std::chrono::sys_days get_submission_date() const { return m_claim.submission_date; }
	
	

	/// scalar fields -sets
	void set_id(int id) { m_claim.id = id; }
	void set_sent(bool sent) { m_claim.status = sent ? 1 : 0; }
	void set_start_date(std::chrono::sys_days date)
	{
		m_claim.start_date = date;
	}
	void set_submission_date(std::chrono::sys_days date)
	{
		m_claim.submission_date = date;
	}


	// disable copy ctor
	ClaimDoc(const ClaimDoc& other) = delete;

	// copy claim
	// ClaimDoc& operator=(const Claim& claim)
	// {
	// 	m_claim = claim;
	// 	return *this;
	// }
	// conversion operators!!
	// ClaimDoc(const Claim& claim) : m_claim{ claim } {}
	// operator Claim&() { return m_claim; }


	struct Where
	{
		ClaimDoc* pThis;
		Where(ClaimDoc* p) : pThis{p} {}

		auto for_patient()
		{
			auto patient = pThis->get_patient();
			int patient_id = patient ? patient->id : -1;
			auto patientWhere = (not patient or (c(alias_column<als_p>(&Patient::id)) == patient_id));
			return patientWhere;
		}
		auto for_all()
		{
			return for_patient();	// && XXX && YYY && ZZZ
		}
		
	} where_clauses{ this };
};

/// get all invoices for this claim
std::vector<Invoice> ClaimDoc::get_invoices(auto order_clause)
{
	return get_Card_N<Invoice, &Invoice::fkey_claim, Claim, &Claim::id, order_clause>(m_claim, &Invoice::id);
}

template<typename ...order_clauses>
std::vector<Invoice> ClaimDoc::get_invoices(order_clauses...clauses)
{
	return get_Card_N<Invoice, &Invoice::fkey_claim, Claim, &Claim::id>(m_claim, clauses...);
}

