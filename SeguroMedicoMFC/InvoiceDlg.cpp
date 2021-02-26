// InvoiceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SeguroMedicoMFC.h"
#include "InvoiceDlg.h"
#include "afxdialogex.h"
#include "ClaimDlg.h"
#include "INSResponseDlg.h"


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
		return Util::to_cstring(claim.dump());
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
	ON_LBN_SELCHANGE(IDC_L_INVOICES, &InvoiceDlg::OnLbnSelchangeLInvoices)
	ON_BN_CLICKED(ID_B_RECLAMOS, &InvoiceDlg::OnBnClickedBReclamos)
	ON_BN_CLICKED(ID_B_INS_RESPONSE, &InvoiceDlg::OnBnClickedBInsResponse)
END_MESSAGE_MAP()


// InvoiceDlg message handlers


BOOL InvoiceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_invoice_type.Initialize();
	
	Refresh();

	if(m_id != -1)
	{
		m_invoiceLB.select(m_id);
	}
	
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
	auto number = GetText(m_factura_num);
	auto amount = GetAmount(m_monto);

	auto type = static_cast<InvoiceType>(m_invoice_type.get_value());

	auto description= GetText(m_descripcion);

	if (!claim)
	{
		MessageBoxW(L"Falta escoger Reclamo");
		return;
	}
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
	if (!invoice)	// insert
	{
		// std::optional<int> ins_response_id = ins_response ? ins_response->id : std::nullopt;
		std::optional<int> ins = std::nullopt;
		if(ins_response)
		{
			ins = ins_response->id;
		}
		invoice = m_invoiceLB.insert(claim->id, number, amount, static_cast<int>(type), description, ins);
		m_invoiceLB.insert_into_listbox(*invoice);

		INSResponseLine line{ -1, ins_response->id, invoice->id,0,0,0,0,0,0,0,0 };
		Storage::getStorage().insert(line);
	}
	else                // update
	{
		std::optional<int> ins = std::nullopt;
		if (ins_response)
		{
			ins = ins_response->id;
		}

		invoice->amount = amount;
		invoice->fkey_claim = claim->id;
		invoice->description = description;
		invoice->fkey_INSResponse = ins;
		invoice->number = number;
		invoice->type = static_cast<int>(type);
		m_invoiceLB.update(*invoice);

		auto storage = Storage::getStorage();
		
		// auto resp_line = storage.get<INSResponseLine>(where(c(alias_column<als_k>(&INSResponseLine::fkey_factura)) == invoice->id));

		auto res = storage.select(columns(&INSResponseLine::fkey_factura, &INSResponseLine::id), where(c(&INSResponseLine::fkey_factura) == invoice->id));
		if(! res.empty())
		{
			auto l = res[0];
			auto id_response_line = std::get<1>(l);
			auto line = storage.get<INSResponseLine>(id_response_line);
			line.fkey_INSResponse = *ins;
			storage.replace(line);
		}
	}
	claim->get_total_amount();
	Storage::getStorage().replace(*claim);
	// SetAmount(m_monto, claim->amount);
	m_invoiceLB.loadLBOrderBy(&Invoice::number);
	m_invoice = invoice;
	setIdFromRecord<Invoice>(m_id_invoice, m_invoice->id);
}


void InvoiceDlg::OnBnClickedNuevo()
{
	// TODO: Add your control notification handler code here
	SetText(m_id_invoice, ""s);
	m_claimCB.select(std::nullopt);
	m_invoice_type.set_value(0);
	SetText(m_factura_num, ""s);
	SetText(m_descripcion, ""s);
	SetAmount(m_monto, 0);
	m_responseCB.select(std::nullopt);
}


void InvoiceDlg::OnBnClickedBorrar()
{
	// TODO: Add your control notification handler code here
	if (m_invoiceLB.delete_current_sel())
	{
		OnBnClickedNuevo();
	}
}


void InvoiceDlg::OnBnClickedCerrar()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}


void InvoiceDlg::OnLbnSelchangeLInvoices()
{
	// TODO: Add your control notification handler code here
	auto invoice = m_invoiceLB.current();
	if (!invoice)	return;
	SetText(m_id_invoice, invoice->id);
	m_claimCB.select(invoice->fkey_claim);
	m_invoice_type.set_value(invoice->type);
	SetText(m_factura_num, invoice->number);
	SetText(m_descripcion, invoice->description);
	SetAmount(m_monto, invoice->amount);
	m_responseCB.select(invoice->fkey_INSResponse);
	m_invoice = invoice;
}


void InvoiceDlg::OnBnClickedBReclamos()
{
	// TODO: Add your control notification handler code here
	ClaimDlg dlg;
	dlg.DoModal();
	Refresh();
}


void InvoiceDlg::OnBnClickedBInsResponse()
{
	// TODO: Add your control notification handler code here
	INSResponseDlg dlg;
	auto resp = m_responseCB.current();
	dlg.m_id = resp ? resp->id : -1;
	dlg.DoModal();
	Refresh();
}


BOOL InvoiceDlg::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class
	if(message == WM_COMMAND)
	{
		if(wParam== 0xFFFF && lParam == 0xFFFF)
		{
			int i = 0;
		}
	}
	return CDialog::OnWndMsg(message, wParam, lParam, pResult);
}
