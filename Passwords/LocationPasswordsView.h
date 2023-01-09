#pragma once

#include "..\ORM_Extensions/IDisplayer.h"
#include "..\ORM_Extensions/GridDisplayer.h"
#include "Data.h"
#include "..\ORM_Extensions/JoinedGridDisplayer.h"

#include "..\ORM_Extensions/BoxContents.h"
#include "LocationGrid.h"


// LocationPasswordsView form view

class LocationPasswordsView : public CFormView
{
	DECLARE_DYNCREATE(LocationPasswordsView)

		int get_location_id() const noexcept;

protected:
	LocationPasswordsView();           // protected constructor used by dynamic creation
	virtual ~LocationPasswordsView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LocationPasswordsView };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	void OnGrid1StartSelChange(NMHDR* pNotifyStruct, LRESULT*);
public:
	LocationGrid m_location_grid;
	//CJDGridCtrl m_GridLocations;
	CJDGridCtrl m_GridPasswords;
	//std::unique_ptr<IDisplayer> m_displayer_locations;
	std::unique_ptr<IDisplayer> m_displayer_passwords;

	template<typename T>
	void InitializeGridLocations(const T& t);

	template<typename T>
	void InitializeGridPasswords(const T& t);

	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedBLocationDlg();
	afx_msg void OnBnClickedBPasswordDlg();
	afx_msg void OnBnClickedBUnify();
private:
	CEdit m_location_search;
	std::vector<Location> m_found_locations;
	int m_location_index = -1;
public:
	afx_msg void OnBnClickedBSearchLocation();
	afx_msg void OnKillfocusESearchLocation();
	afx_msg void OnBnClickedBGetselected();
private:
	CEdit m_row_selected;
};


