#pragma once

#include "BoxContents.h"
#include "RadioButtonGroup.h"


// InvoiceDlg dialog

class InvoiceDlg : public CDialog
{
	DECLARE_DYNAMIC(InvoiceDlg)

	BoxContents<Claim, &Claim::id, CComboBox> m_claimCB;
	BoxContents<INSResponse, &INSResponse::id, CComboBox> m_responseCB;
	BoxContents<Invoice, &Invoice::id> m_invoiceLB;

	std::optional<Invoice> m_invoice;

	void Refresh();

	RadioButtonGroup m_invoice_type;
	
 public:
	InvoiceDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~InvoiceDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_InvoiceDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CEdit m_id_invoice;
	CComboBox m_list_reclamos;
	CEdit m_factura_num;
	CEdit m_monto;
	CButton m_r_consulta;
	CButton m_r_examen;
	CButton m_r_medicina;
	CButton m_r_terapia;
	CButton m_r_proc;
	CEdit m_descripcion;
	CComboBox m_list_ins_response;
	CListBox m_list_facturas;
public:
	afx_msg void OnBnClickedApply();
	afx_msg void OnBnClickedNuevo();
	afx_msg void OnBnClickedBorrar();
	afx_msg void OnBnClickedCerrar();
	afx_msg void OnLbnSelchangeLInvoices();
	afx_msg void OnBnClickedBReclamos();
	afx_msg void OnBnClickedBInsResponse();
};
