// InvoiceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SeguroMedicoMFC.h"
#include "InvoiceDlg.h"
#include "afxdialogex.h"


// InvoiceDlg dialog

IMPLEMENT_DYNAMIC(InvoiceDlg, CDialog)

InvoiceDlg::InvoiceDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_InvoiceDlg, pParent),
m_invoice_type({&m_r_consulta,&m_r_examen, &m_r_medicina, &m_r_terapia, &m_r_proc}),
m_responseCB(m_list_ins_response, [](INSResponse& response)
{
		return Util::to_cstring(response.simple_dump());
}),
m_claimCB(m_list_reclamos, [](Claim& claim)
{
		return Util::to_cstring(claim.simple_dump());
}),
m_invoiceLB(m_list_facturas, [](Invoice& invoice)
{
		return Util::to_cstring(invoice.simple_dump());
})
{

}

InvoiceDlg::~InvoiceDlg()
{
}

void InvoiceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_E_ID_INVOICE, m_id_invoice);
	DDX_Control(pDX, IDC_C_CLAIM_LIST, m_list_reclamos);
	DDX_Control(pDX, IDC_E_NUMERO_FACTURA, m_factura_num);
	DDX_Control(pDX, IDC_E_AMOUNT, m_monto);
	DDX_Control(pDX, IDC_R_CONSULTA, m_r_consulta);
	DDX_Control(pDX, IDC_R_EXAMEN, m_r_examen);
	DDX_Control(pDX, IDC_R_MEDICATION, m_r_medicina);
	DDX_Control(pDX, IDC_R_THERAPY, m_r_terapia);
	DDX_Control(pDX, IDC_R_PROCEDIMIENTO, m_r_proc);
	DDX_Control(pDX, IDC_E_DESCRIPTION, m_descripcion);
	DDX_Control(pDX, IDC_C_INS_RESPONSE, m_list_ins_response);
	DDX_Control(pDX, IDC_L_INVOICES, m_list_facturas);
}


BEGIN_MESSAGE_MAP(InvoiceDlg, CDialog)
	ON_BN_CLICKED(ID_APPLY, &InvoiceDlg::OnBnClickedApply)
	ON_BN_CLICKED(ID_NUEVO, &InvoiceDlg::OnBnClickedNuevo)
	ON_BN_CLICKED(ID_BORRAR, &InvoiceDlg::OnBnClickedBorrar)
	ON_BN_CLICKED(ID_CERRAR, &InvoiceDlg::OnBnClickedCerrar)
END_MESSAGE_MAP()


// InvoiceDlg message handlers


BOOL InvoiceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	Refresh();
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void InvoiceDlg::Refresh()
{
	m_claimCB.loadLBOrderBy(&Claim::start_date);
	m_invoiceLB.loadLBOrderBy(&Invoice::number);
	m_responseCB.loadLBOrderBy(&INSResponse::total_a_pagar);

}


void InvoiceDlg::OnBnClickedApply()
{
	// TODO: Add your control notification handler code here
	auto invoice = getCurrent<Invoice>(m_id_invoice);

	std::optional<Claim> claim = m_claimCB.current();
	std::optional<INSResponse> ins_response = m_responseCB.current();

	// date::sys_days start_date = GetDate(m_start_date);
	// date::sys_days submission_date = GetDate(m_date_submitted);
	auto number = GetInteger(m_factura_num);
	auto amount = GetAmount(m_monto);

	auto type = static_cast<InvoiceType>(m_invoice_type.get_value());

	auto description= GetText(m_descripcion);

	if (!claim)
	{
		MessageBoxW(L"Falta escoger Reclamo");
		return;
	}
	// ins_response can be nullopt!
	if( ! ins_response)
	{
		MessageBoxW(L"Falta escoger INS Response");
		return;
	}

#if 0	
	if (!claim)
	{
		auto whereClause = (c(&Claim::first_name) == name.c_str() && c(&Doctor::last_name) == last_name.c_str());

		auto doctor_by_value = m_doctoresLB.exists(whereClause);
		claim = doctor_by_value;
	}
#endif
#if 1
	if (!invoice)	// insert
	{
		// std::optional<int> ins_response_id = ins_response ? ins_response->id : std::nullopt;
		invoice = m_invoiceLB.insert(claim->id, number, amount, static_cast<int>(type), description, ins_response->id);
		m_invoiceLB.insert_into_listbox(*invoice);
	}
	else                // update
	{
		invoice->amount = amount;
		invoice->fkey_claim = claim->id;
		invoice->description = description;
		invoice->fkey_INSResponse = ins_response->id;
		invoice->number = number;
		invoice->type = static_cast<int>(type);
		m_invoiceLB.update(*invoice);
	}
#endif
	m_invoiceLB.loadLBOrderBy(&Invoice::number);
	m_invoice = invoice;
	setIdFromRecord<Invoice>(m_id_invoice, m_invoice->id);

}


void InvoiceDlg::OnBnClickedNuevo()
{
	// TODO: Add your control notification handler code here
}


void InvoiceDlg::OnBnClickedBorrar()
{
	// TODO: Add your control notification handler code here
}


void InvoiceDlg::OnBnClickedCerrar()
{
	// TODO: Add your control notification handler code here
}
