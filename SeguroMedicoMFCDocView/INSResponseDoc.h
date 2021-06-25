#pragma once

// INSResponseDoc document

class INSResponseDoc : public CDocument
{
	DECLARE_DYNCREATE(INSResponseDoc)
	INSResponse m_ins_response;

public:
	INSResponseDoc();
	virtual ~INSResponseDoc();
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
	// relationships:
	std::vector<Invoice> get_invoices(auto order_clause);
	template<typename ...order_clauses>
	std::vector<Invoice> get_invoices(order_clauses...clauses);

	std::vector<INSResponseLine> get_ins_response_lines(auto order_clause);
	template<typename ...order_clauses>
	std::vector<Invoice> get_ins_response_lines(order_clauses...clauses);

	// 0-1 side

	void set_INSResponse(const INSResponse& ins);

	// also set Invoice 1--------1 INSReponseLine
	void add_lines(const INSResponseLine& resp_inline, const Invoice& invoice);
	
};
