#pragma once

#include "BoxContents.h"

// INSResponseLineDlg dialog

class INSResponseLineDlg : public CDialog
{
	DECLARE_DYNAMIC(INSResponseLineDlg)

	std::optional<INSResponseLine> m_INSResponseLine;

	BoxContents<INSResponseLine, &INSResponseLine::id> m_INSResponseLinesLB;
	BoxContents<INSResponse, &INSResponse::id, CComboBox> m_ins_responseCB;
	BoxContents<Invoice, &Invoice::id, CComboBox> m_invoicesCB;

	void Refresh();
	
public:
	INSResponseLineDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~INSResponseLineDlg();

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
};
