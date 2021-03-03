// ClaimDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SeguroMedicoMFC.h"
#include "ClaimDlg.h"
#include "afxdialogex.h"
#include "DoctorDlg.h"
#include "InvoiceDlg.h"
#include "JoinedGridDisplayer.h"
#include "MedicationDlg.h"
#include "PatientDlg.h"


// ClaimDlg dialog

IMPLEMENT_DYNAMIC(ClaimDlg, CDialog)

ClaimDlg::ClaimDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_ClaimDlg, pParent),
m_patientCB(m_patient_list, [](Patient& patient)
{
	return Util::to_cstring(patient.simple_dump());
}),
m_doctorCB(m_doctor_list, [](Doctor& doctor)
{
	return Util::to_cstring(doctor.simple_dump());
}),
m_medicationCB(m_medicamento_list, [](Medication& med)
{
	return Util::to_cstring(med.simple_dump());
}),
m_claimLB(m_claim_list, [](Claim& claim)
{
		return Util::to_cstring(claim.dump());
}),
m_acknowledgement({ &m_acknowledge_none, &m_acknowledge_partial, &m_acknowledge_full  })
{

}

ClaimDlg::~ClaimDlg()
{
}

void ClaimDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_E_ID_CLAIM, m_id_reclamo);
	DDX_Control(pDX, IDC_C_PATIENTS, m_patient_list);
	DDX_Control(pDX, IDC_C_DOCTORES, m_doctor_list);
	DDX_Control(pDX, IDC_C_MEDICAMENTOS, m_medicamento_list);
	DDX_Control(pDX, IDC_START_DATE, m_start_date);
	DDX_Control(pDX, IDC_FECHA_ENTREGADO, m_date_submitted);
	DDX_Control(pDX, IDC_E_ASPROSE_CLAIM_NUMBER, m_asprose_claim_number);
	DDX_Control(pDX, IDC_E_ASPROSE_CASE_NUMBER2, m_asprose_case_number);
	DDX_Control(pDX, IDC_E_ASPROSE_AMOUNT_PRESENTED, m_asprose_amount_recognized);
	DDX_Control(pDX, IDC_R_FULL, m_acknowledge_full);
	DDX_Control(pDX, IDC_R_PARTIAL, m_acknowledge_partial);
	DDX_Control(pDX, IDC_R_NONE, m_acknowledge_none);
	DDX_Control(pDX, IDC_E_INS_CLAIM_NUMBER, m_ins_claim_number);
	DDX_Control(pDX, IDC_E_TOTAL_CLAIM_AMOUNT, m_total_claim_amount);
	DDX_Control(pDX, IDC_E_COMMENT, m_comment);
	DDX_Control(pDX, IDC_CLAIMS_GRID, m_grid_claims);
	DDX_Control(pDX, IDC_L_CLAIM_LIST, m_claim_list);
	DDX_Control(pDX, IDC_E_OTHER_SYSTEM_ID, m_other_system_id);
	DDX_Control(pDX, IDC_C_SENT, m_sent);
}


BEGIN_MESSAGE_MAP(ClaimDlg, CDialog)
	ON_NOTIFY(GVN_SELCHANGED, IDC_CLAIMS_GRID, OnGridStartSelChange)
	ON_BN_CLICKED(ID_APPLY, &ClaimDlg::OnBnClickedApply)
	ON_BN_CLICKED(ID_NUEVO, &ClaimDlg::OnBnClickedNuevo)
	ON_BN_CLICKED(ID_BORRAR, &ClaimDlg::OnBnClickedBorrar)
	ON_BN_CLICKED(ID_CERRAR, &ClaimDlg::OnBnClickedCerrar)
	ON_LBN_SELCHANGE(IDC_L_CLAIM_LIST, &ClaimDlg::OnLbnSelchangeLClaimList)
	ON_BN_CLICKED(ID_B_FILTER, &ClaimDlg::OnBnClickedBFilter)
	ON_BN_CLICKED(IDC_B_PATIENTS, &ClaimDlg::OnBnClickedBPatients)
	ON_BN_CLICKED(IDC_B_DOCTORES, &ClaimDlg::OnBnClickedBDoctores)
	ON_BN_CLICKED(IDC_B_MEDICATIONS, &ClaimDlg::OnBnClickedBMedications)
	ON_BN_CLICKED(IDC_B_ADD_FACTURA, &ClaimDlg::OnBnClickedBAddFactura)
END_MESSAGE_MAP()


// ClaimDlg message handlers


BOOL ClaimDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_acknowledge_none.SetCheck(BST_CHECKED);
	
	Refresh();

	if(m_id!= -1)
	{
		m_claimLB.select(m_id);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void ClaimDlg::Refresh()
{
	m_doctorCB.loadLBOrderBy(&Doctor::last_name);
	m_medicationCB.loadLBOrderBy(&Medication::name);
	m_patientCB.loadLBOrderBy(&Patient::last_name);
	m_claimLB.loadLBOrderByDesc(&Claim::start_date);
	// InitializeGrid(true);
	// grid	
}

#if 0
using als_c = alias_c<Claim>;
using als_p = alias_p<Patient>;
using als_d = alias_d<Doctor>;
using als_s = alias_s<Specialty>;
using als_m = alias_m<Medication>;
using als_i = alias_i<Invoice>;
using als_j = alias_j<INSResponse>;
#endif


void ClaimDlg::WhereParameters::executeWhere()
{
	auto fkey_claim = this->claim_id.value_or(1);

	auto whereClaim = c(alias_column<als_i>(&Invoice::fkey_claim)) == fkey_claim;

	// auto conceptoPattern = this->conceptoPattern.value_or("%");
	//
	// auto whereConcepto = like(alias_column<als_d>(&Concepto::name), conceptoPattern);
	//
	// auto categoriaPattern = this->categoriaPattern.value_or("%");
	//
	// auto whereCategoria = like(alias_column<als_c>(&Categoria::name), categoriaPattern);


	auto whereClause = whereClaim;  // whereStatement&& whereConcepto&& whereCategoria;

	dlg->InitializeGrid(whereClause);

}

template<typename T>
void ClaimDlg::InitializeGrid(const T& t)
{
#if 0
	auto otherlines = Storage::getStorage().select(columns(
		alias_column<als_c>(&Claim::id),
		alias_column<als_p>(&Patient::last_name),
		alias_column<als_p>(&Patient::first_name),
		alias_column<als_d>(&Doctor::last_name),
		alias_column<als_d>(&Doctor::first_name),
		alias_column<als_c>(&Claim::fkey_medication),
		left_join<als_p>(on(c(alias_column<als_c>(&Claim::fkey_patient)) == alias_column<als_p>(&Patient::id))),
		left_join<als_d>(on(c(alias_column<als_c>(&Claim::fkey_doctor)) == alias_column<als_d>(&Doctor::id)))));
#else
	// TODO: invoices go here!!
	
	auto otherlines = Storage::getStorage().select(columns(
		alias_column<als_i>(&Invoice::id),
		alias_column<als_i>(&Invoice::number),
		alias_column<als_i>(&Invoice::amount),
		alias_column<als_i>(&Invoice::fkey_claim),
		alias_column<als_c>(&Claim::start_date),
		alias_column<als_c>(&Claim::submission_date),
		alias_column<als_c>(&Claim::amount),
		alias_column<als_i>(&Invoice::fkey_INSResponse),
		alias_column<als_i>(&Invoice::description)),
		// sum(alias_column<als_i>(&Invoice::amount))),
		inner_join<als_c>(on(c(alias_column<als_i>(&Invoice::fkey_claim)) == alias_column<als_c>(&Claim::id))),
		// inner_join<als_c>(on(c(alias_column<als_i>(&Invoice::fkey_claim)) == alias_column<als_c>(&Claim::id)))
		where(t),
		order_by(alias_column<als_i>(&Invoice::fkey_claim)));


	auto sum_results = Storage::getStorage().select(columns(
		sum(alias_column<als_i>(&Invoice::amount))),
		where(t));

	auto&& line = sum_results[0];

	{
		auto&& pc = std::get<0>(line);
		if (pc)
		{
			SetAmount(m_total_claim_amount, *pc);
		}
	}
#endif

#if 0		
		,					// fkey_medication int nullable
		// alias_column<als_t>(&Transaccion::fkey_account_own),										// fkey account own int
		coalesce<std::string>(alias_column<als_p>(&AccountOwner::name), ""),
		coalesce<std::string>(alias_column<als_a>(&Account::number), ""),							// fkey account other number nullable
		alias_column<als_d>(&Concepto::id_concepto),
		alias_column<als_d>(&Concepto::name),														// concepto::name
		alias_column<als_b>(&Account::number),														// fkey account own number
		alias_column<als_c>(&Categoria::name),														// categoria::name
		alias_column<als_s>(&Statement::date),														// statement::date
		alias_column<als_t>(&Transaccion::line_date),
		alias_column<als_o>(&AccountOwner::name),													// account owner:: name
		alias_column<als_s>(&Statement::id_statement),
		alias_column<als_t>(&Transaccion::amount_colones),
		alias_column<als_t>(&Transaccion::amount_dolares)),
		left_join<als_a>(on(c(alias_column<als_t>(&Transaccion::fkey_account_other)) == alias_column<als_a>(&Account::id_account))),
		left_join< als_b>(on(c(alias_column<als_t>(&Transaccion::fkey_account_own)) == alias_column<als_b>(&Account::id_account))),
		left_join< als_c>(on(c(alias_column<als_t>(&Transaccion::fkey_category)) == alias_column<als_c>(&Categoria::id_categoria))),
		left_join< als_d>(on(c(alias_column<als_t>(&Transaccion::fkey_concepto)) == alias_column<als_d>(&Concepto::id_concepto))),
		left_join< als_s>(on(c(alias_column<als_t>(&Transaccion::fkey_statement)) == alias_column<als_s>(&Statement::id_statement))),
		left_join<als_o>(on(c(alias_column<als_b>(&Account::fkey_account_owner)) == alias_column<als_o>(&AccountOwner::id_owner))),
		left_join<als_p>(on(c(alias_column<als_a>(&Account::fkey_account_owner)) == alias_column<als_p>(&AccountOwner::id_owner))),
		where(t));
#endif
	long count = otherlines.size();
	auto strCount = Util::to_cstring(count);
	// m_countMainGrid.SetWindowTextW(strCount);

	std::vector<std::string> headers{ "ID FAC", "FAC NUMERO", "FAC MONTO", "ID RECLAMO", "FECHA INICIAL", "FECHA ENTREGA", "MONTO RECLAMO", "ID RESPUESTA INS", "DESCRIPCION" };

	m_displayer.reset(new JoinedGridDisplayer<decltype(otherlines[0]), IntegerList<3,7>, IntegerList<0>>(m_grid_claims, std::move(otherlines), std::move(headers))); // , ColonesFormat<14>{13}, DolaresFormat<14>{14}));
	m_displayer->display();
}


void ClaimDlg::OnBnClickedApply()
{
	// TODO: Add your control notification handler code here
	auto claim = getCurrent<Claim>(m_id_reclamo);

	std::optional<Patient> patient = m_patientCB.current();
	std::optional<Doctor> doctor = m_doctorCB.current();
	std::optional<Medication> medicina = m_medicationCB.current();

	date::sys_days start_date = GetDate(m_start_date);
	date::sys_days submission_date = GetDate(m_date_submitted);

	auto asprose_num_reclamo = GetText(m_asprose_claim_number);
	auto asprose_caso_numero = GetText(m_asprose_case_number);
	auto asprose_amount_recognized = GetAmount(m_asprose_amount_recognized);
	// auto monto_total = GetAmount(m_total_claim_amount);

	auto val = static_cast<AcknowledgementType>(m_acknowledgement.get_value());

	auto ins_num_reclamo = GetText(m_ins_claim_number);

	auto comments = GetText(m_comment);
	auto amount_claim = GetAmount(m_total_claim_amount);
	auto other_system_id = GetInteger(m_other_system_id);
	auto sent = m_sent.GetCheck();

	if(!patient)
	{
		MessageBoxW(L"Falta escoger paciente");
		return;
	}
	if(! doctor)
	{
		MessageBoxW(L"Falta escoger doctor");
		return;
	}
	if( ! medicina)
	{
		MessageBoxW(L"Falta escoger la medicina");
		return;
	}
	// medicina can be nullopt!

#if 0	
	if (!claim)
	{
		auto whereClause = (c(&Claim::first_name) == name.c_str() && c(&Doctor::last_name) == last_name.c_str());

		auto doctor_by_value = m_doctoresLB.exists(whereClause);
		claim = doctor_by_value;
	}
#endif
#if 1
	if (!claim)	// insert
	{
		claim = m_claimLB.insert(patient->id, doctor->id, medicina->id, start_date, submission_date, asprose_num_reclamo, asprose_caso_numero, asprose_amount_recognized, ins_num_reclamo, static_cast<int>(val), comments, amount_claim, other_system_id,sent);
		m_claimLB.insert_into_listbox(*claim);
	}
	else                // update
	{
		claim->amount = claim->get_total_amount();
		claim->start_date = start_date;
		claim->submission_date = submission_date;
		// claim->amount = amount_claim;
		claim->asprose_acknowledgement_type = static_cast<int>(val);
		claim->asprose_amount_presented = asprose_amount_recognized;
		claim->asprose_case_number = asprose_caso_numero;
		claim->asprose_claim_number = asprose_num_reclamo;
		claim->ins_claim_number = ins_num_reclamo;
		claim->comment = comments;
		claim->fkey_doctor = doctor->id;
		claim->fkey_medication = medicina->id;
		claim->fkey_patient = patient->id;
		claim->other_system_id = other_system_id;
		claim->status = sent;
		m_claimLB.update(*claim);
	}
#endif
	m_claimLB.loadLBOrderByDesc(&Claim::start_date);
	m_claim = claim;
	setIdFromRecord<Doctor>(m_id_reclamo, m_claim->id);
}


void ClaimDlg::OnBnClickedNuevo()
{
	// TODO: Add your control notification handler code here
	SetText(m_id_reclamo, ""s);
	// SetDate(m_start_date, claim->start_date);
	// SetDate(m_date_submitted, claim->submission_date);
	m_acknowledgement.set_value(0);
	SetText(m_asprose_case_number, ""s);
	SetText(m_asprose_claim_number, ""s );
	SetAmount(m_asprose_amount_recognized, 0);
	// SetAmount();
	m_doctorCB.select(std::nullopt);
	m_patientCB.select(std::nullopt);
	m_medicationCB.select(std::nullopt);
	m_claimLB.select(std::nullopt); 
	m_sent.SetCheck(0);
	SetAmount(m_total_claim_amount, 0);
	m_claim = std::nullopt;
	OnBnClickedBFilter();
}


void ClaimDlg::OnBnClickedBorrar()
{
	// TODO: Add your control notification handler code here
	if (m_claimLB.delete_current_sel())
	{
		OnBnClickedNuevo();
	}
}


void ClaimDlg::OnBnClickedCerrar()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}


void ClaimDlg::OnLbnSelchangeLClaimList()
{
	// TODO: Add your control notification handler code here
	auto claim = m_claimLB.current();
	if (!claim)	 return;
	SetText(m_comment, claim->comment);
	SetText(m_id_reclamo, claim->id);
	SetDate(m_start_date, claim->start_date);
	SetDate(m_date_submitted, claim->submission_date);
	m_acknowledgement.set_value(claim->asprose_acknowledgement_type);
	SetText(m_asprose_case_number, claim->asprose_case_number);
	SetText(m_asprose_claim_number, claim->asprose_claim_number);
	SetAmount(m_asprose_amount_recognized, claim->asprose_amount_presented);
	m_sent.SetCheck(claim->status);
	// SetAmount(m_total_claim_amount, claim->amount);
	m_doctorCB.select(claim->fkey_doctor);
	m_patientCB.select(claim->fkey_patient);
	m_medicationCB.select(claim->fkey_medication);
	m_claim = claim;


	auto sum_results = Storage::getStorage().select(columns(
		sum(alias_column<als_i>(&Invoice::amount))),
		where(c(alias_column<als_i>(&Invoice::fkey_claim)) == claim->id));

	auto&& line = sum_results[0];
	auto&& pc = std::get<0>(line);
	if (pc)
	{
		SetAmount(m_total_claim_amount, *pc);
		// set
		Storage::getStorage().update_all(set(assign(&Claim::amount, *pc)), where(is_equal(&Claim::id, claim->id)));
	}
	OnBnClickedBFilter();
}


void ClaimDlg::OnBnClickedBFilter()
{
	// TODO: Add your control notification handler code here
	if (!m_claim)
	{
		InitializeGrid(false);
		return;
	}
	
	whereParameters.claim_id = m_claim->id;
	whereParameters.executeWhere();
}


void ClaimDlg::OnBnClickedBPatients()
{
	// TODO: Add your control notification handler code here
	PatientDlg dlg;
	dlg.DoModal();
	Refresh();
}


void ClaimDlg::OnBnClickedBDoctores()
{
	// TODO: Add your control notification handler code here
	DoctorDlg dlg;
	dlg.DoModal();
	Refresh();
}


void ClaimDlg::OnBnClickedBMedications()
{
	// TODO: Add your control notification handler code here
	MedicationDlg dlg;
	dlg.DoModal();
	Refresh();
}

void ClaimDlg::OnGridStartSelChange(NMHDR* pNotifyStruct, LRESULT* /*pResult*/)
{
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNotifyStruct;
	auto row = pItem->iRow;
	auto col = pItem->iColumn;

	if (row < 1) return;

	auto invoice_id_cs = m_grid_claims.GetItemText(row, 1);
	auto invoice_id_s = Util::to_string(invoice_id_cs.GetBuffer());
	auto invoice_id = std::stoi(invoice_id_s);

	InvoiceDlg dlg;
	dlg.m_id = invoice_id;
	dlg.DoModal();
	Refresh();

}


void ClaimDlg::OnBnClickedBAddFactura()
{
	// TODO: Add your control notification handler code here
	InvoiceDlg dlg;
	// dlg.m_id = m
	dlg.DoModal();
	Refresh();
	OnBnClickedBFilter();
}
