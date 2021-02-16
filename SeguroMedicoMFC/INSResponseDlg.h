#pragma once


// INSResponseDlg dialog

class INSResponseDlg : public CDialog
{
	DECLARE_DYNAMIC(INSResponseDlg)

public:
	INSResponseDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~INSResponseDlg();

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
};
