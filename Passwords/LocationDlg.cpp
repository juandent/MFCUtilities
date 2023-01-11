// LocationDlg.cpp : implementation file
//

#include "pch.h"
#include "Passwords.h"
#include "afxdialogex.h"
#include "LocationDlg.h"


// LocationDlg dialog

IMPLEMENT_DYNAMIC(LocationDlg, CDialog)

LocationDlg::LocationDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_LocationDlg, pParent),
	m_list_all_locationsLB(m_list_locations, [](const Location& location)
		{
			return Util::to_cstring(location.simple_dump());
		})

{

}

LocationDlg::~LocationDlg()
{
}

void LocationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_E_LOCATION_EMAIL, m_email);
	DDX_Control(pDX, IDC_E_LOCATION_ID, m_id);
	DDX_Control(pDX, IDC_E_LOCATION_NAME, m_name);
	DDX_Control(pDX, IDC_E_LOCATION_URL, m_url);
	DDX_Control(pDX, IDC_LIST_LOCATIONS, m_list_locations);
}


BEGIN_MESSAGE_MAP(LocationDlg, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST_LOCATIONS, &LocationDlg::OnSelchangeListLocations)
	ON_BN_CLICKED(ID_APPLY, &LocationDlg::OnBnClickedApply)
	ON_BN_CLICKED(ID_ERASE, &LocationDlg::OnBnClickedErase)
	ON_BN_CLICKED(ID_NEW, &LocationDlg::OnBnClickedNew)
END_MESSAGE_MAP()


// LocationDlg message handlers


BOOL LocationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	m_list_all_locationsLB.loadLBOrderBy(&Location::name);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void LocationDlg::OnSelchangeListLocations()
{
	// TODO: Add your control notification handler code here
	auto location = m_list_all_locationsLB.current();

	if (!location)	return;

	SetText( m_email ,location->email);
	SetAmount( m_id, location->id);
	SetText( m_name, location->name);
	SetText( m_url, location->url);


}


void LocationDlg::OnBnClickedApply()
{

	auto location = getCurrent<Location>(m_id);

	auto name = Util::touppercase( GetText(m_name));
	auto url = GetText(m_url);
	auto email = GetText(m_email);

#if 0
	auto num_caso = GetLongLong(m_no_caso);
	auto total_bruto = GetAmount(m_total_bruto);
	auto deducciones = GetAmount(m_deducciones);
	auto total_neto = GetDolares(m_neto).getAsLongDouble();
	auto retenciones = GetAmount(m_retencion);
	auto total_pagar = GetAmount(m_total_pagar);
	auto comentarios = GetText(m_comentarios);
#endif
	// calculados
	// auto monto_cubierto_p100 = 100 * monto_cubierto != 0 ? total_a_pagar / monto_cubierto : 0; //GetAmount(m_monto_cubierto_p100);
	// auto factura_cubierta_p100 = 100 * monto_factura != 0 ? total_a_pagar / monto_factura : 0;

//	auto count = Storage::getStorage().count<Location>(where(is_equal(&Location::id, liquidacion)));

	if (name.empty())
	{
		MessageBoxW(L"No se puede dejar el nombre vacio");
		return;
	}


	if (!location)	// insert
	{
		location = m_list_all_locationsLB.insert(name, url, email);
		m_list_all_locationsLB.insert_into_listbox(*location);
	}
	else                // update
	{
		location->email = email;
		location->name = name;
		location->url = url;
		m_list_all_locationsLB.update(*location);
	}
	m_list_all_locationsLB.loadLBOrderBy(&Location::name);
//	m_ins_response = ins_response;
	setIdFromRecord<Location>(m_id, location->id);

}



void LocationDlg::OnBnClickedErase()
{
	// TODO: Add your control notification handler code here
	if ( m_list_all_locationsLB.delete_current_sel())
	{
		OnBnClickedNew();
	}

}


void LocationDlg::OnBnClickedNew()
{
	// TODO: Add your control notification handler code here
	SetText(m_id, ""s);

	SetText(m_name, ""s);
	SetText(m_url, ""s);
	SetText(m_email, ""s);
}
