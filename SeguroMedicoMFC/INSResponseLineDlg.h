#pragma once

#include "BoxContents.h"

// INSResponseLineDlg dialog

class INSResponseLineDlg : public CDialog
{
	DECLARE_DYNAMIC(INSResponseLineDlg)

	std::optional<INSResponseLine> m_INSResponseLine;
	std::optional<Invoice> m_invoice;
	std::optional<INSResponse> m_ins_response;

	BoxContents<INSResponseLine, &INSResponseLine::id> m_INSResponseLinesLB;
	BoxContents<INSResponse, &INSResponse::id, CComboBox> m_ins_responseCB;
	BoxContents<Invoice, &Invoice::id, CComboBox> m_invoicesCB;

	void Refresh();
	
public:
	INSResponseLineDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~INSResponseLineDlg();

	int m_id{ -1 };
	std::string m_factura_numero;
	
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_m_INSResponseLineDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CComboBox m_list_ins_response;
	CComboBox m_list_facturas;
	CEdit m_monto_cubierto;
	CEdit m_deducciones;
	CEdit m_copago;
	CEdit m_coaseguros;
	CEdit m_deducible_anual;
	CEdit m_total_pagar;
	CEdit m_monto_factura;
	CEdit m_monto_cubierto_p100;
	CEdit m_factura_cubierta_p100;
	CListBox m_list_INSResponseLines;
public:
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedNuevo();
	afx_msg void OnBnClickedBorrar();
	afx_msg void OnBnClickedCerrar();
	CEdit m_id_response_line;
	afx_msg void OnCbnSelchangeCFkeyInsresponse();
private:
	CEdit m_tipo_cambio;
public:
	afx_msg void OnCbnSelchangeCFkeyInvoice();
	afx_msg void OnEnChangeEMontoAPagar();
	afx_msg void OnLbnSelchangeLInsresponseline();
	afx_msg void OnBnClickedCalculate();
	afx_msg void OnBnClickedBInsResponses();
	afx_msg void OnBnClickedBInvoices();
private:
	CEdit m_total_pagar_colones;
	CEdit m_factura_num;
};
