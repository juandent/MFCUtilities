#pragma once
#include <vector>
#include "PersistentClasses.h"

// ClaimsCollection document


class ClaimsCollection : public CDocument
{
	DECLARE_DYNCREATE(ClaimsCollection)
	std::vector<Claim> m_claims;
public:
	ClaimsCollection();
	virtual ~ClaimsCollection();
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
	// disable copy ctor
	ClaimsCollection(const ClaimsCollection& other) = delete;

	// enable move assignment
	ClaimsCollection& operator=(ClaimsCollection&& other)
	{
		if (this == &other)
			return *this;
		m_claims = std::move(other.m_claims);
		return *this;
	}
	
	// conversion operators!!
	ClaimsCollection( std::vector<Claim>&& claims) : m_claims{ std::move(claims) } {}
	operator std::vector<Claim> & () { return m_claims; }
};
