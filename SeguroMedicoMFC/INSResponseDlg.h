#pragma once

#include "BoxContents.h"
// #include "..\ExpenseManagerSQLite/IDisplayer.h"
import idisplayer;

// INSResponseDlg dialog

class INSResponseDlg : public CDialog
{
	DECLARE_DYNAMIC(INSResponseDlg)

	BoxContents<INSResponse, &INSResponse::id> m_list_insresponsesLB;

	

	void Refresh();
	template <class T>
	void InitializeGrid(const T& t);

	std::unique_ptr<IDisplayer> m_displayer;


public:
	std::optional<INSResponse> m_ins_response;

	INSResponseDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~INSResponseDlg();

	int m_id{ -1 };

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INSResponseDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CEdit m_id_insresponse;
	CEdit m_liquidacion;
	CEdit m_no_caso;
	CEdit m_total_bruto;
	CEdit m_deducciones;
	CEdit m_copagos;
	CEdit m_coaseguros;
	CEdit m_deducible_anual;
	CEdit m_tipo_cambio;
	CEdit m_neto;
	CEdit m_retencion;
	CEdit m_total_pagar;
	CEdit m_comentarios;
	CListBox m_list_INSResponses;
	CJDGridCtrl m_grid;
public:
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedNuevo();
	afx_msg void OnBnClickedBorrar();
	afx_msg void OnBnClickedCerrar();
private:
	CDateTimeCtrl m_date_response;
public:
	afx_msg void OnBnClickedCalculate();
	afx_msg void OnLbnSelchangeLInsresponsesList();
	afx_msg void OnBnClickedFilter();
	void OnGridStartSelChange(NMHDR* pNotifyStruct, LRESULT*);
private:
	CEdit m_sum_invoices_amount;
	CEdit m_porcentaje_pagado_INS_response;
	CEdit m_sum_pagos;
};
