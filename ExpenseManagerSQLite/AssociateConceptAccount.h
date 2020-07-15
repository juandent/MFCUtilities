#pragma once
#include "CSVFile.h"

// AssociateConceptAccount document

class AssociateConceptAccount : public CDocument
{
	friend class ConceptsAndAccounts;
	DECLARE_DYNCREATE(AssociateConceptAccount)

	CSVFile m_file;
	void LoadFile(const std::string& fileName);
public:
	AssociateConceptAccount();
	virtual ~AssociateConceptAccount();
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
	afx_msg void OnBnClickedBLoadCompoundDoc();
};
