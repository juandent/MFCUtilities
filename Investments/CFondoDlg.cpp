// CFondoDlg.cpp : implementation file
//

#include "pch.h"
#include "Investments.h"
#include "afxdialogex.h"
#include "CFondoDlg.h"


// CFondoDlg dialog

IMPLEMENT_DYNAMIC(CFondoDlg, CDialog)

CFondoDlg::CFondoDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CFondoDlg, pParent),
	m_tipo_cupon({ &m_mensual, &m_trimestral }, {Fondo::mensual, Fondo::trimestral}),
	m_list_all_fondosLB(m_list_fondos, [](Fondo& fondo)
		{
			return Util::to_cstring(fondo.simple_dump());
		})
{

}

CFondoDlg::~CFondoDlg()
{
}

void CFondoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FONDOS, m_list_fondos);
	DDX_Control(pDX, IDC_RADIO1, m_mensual);
	DDX_Control(pDX, IDC_RADIO2, m_trimestral);
	DDX_Control(pDX, IDC_E_FONDO_ID, m_fondo_id);
	DDX_Control(pDX, IDC_E_FONDO_ABREVIACION, m_fondo_abrev);
	DDX_Control(pDX, IDC_E_FONDO_NAME, m_fondo_name);
}


BEGIN_MESSAGE_MAP(CFondoDlg, CDialog)
	ON_BN_CLICKED(ID_APPLY, &CFondoDlg::OnBnClickedApply)
	ON_LBN_SELCHANGE(IDC_LIST_FONDOS, &CFondoDlg::OnSelchangeListFondos)
	ON_BN_CLICKED(ID_NEW, &CFondoDlg::OnBnClickedNew)
	ON_BN_CLICKED(ID_ERASE, &CFondoDlg::OnBnClickedErase)
END_MESSAGE_MAP()


// CFondoDlg message handlers


BOOL CFondoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_tipo_cupon.Initialize();

	Refresh();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CFondoDlg::Refresh()
{
	m_list_all_fondosLB.loadLBOrderBy(&Fondo::nombre);
}

void CFondoDlg::OnBnClickedApply()
{
	// TODO: Add your control notification handler code here
	m_fondo = getCurrent<Fondo>(m_fondo_id);

	auto tipo = m_tipo_cupon.get_value();
	auto abrev = GetText(m_fondo_abrev);
	auto name = GetText(m_fondo_name);

	if (!m_fondo)	// insert
	{
		m_fondo = m_list_all_fondosLB.insert(abrev, name, tipo);
		m_list_all_fondosLB.insert_into_listbox(*m_fondo);

	}
	else
	{
		m_fondo->abreviacion = abrev;
		m_fondo->nombre = name;
		m_fondo->tipo_cupon = tipo;
		m_list_all_fondosLB.update(*m_fondo);
	}

	setIdFromRecord<Fondo>(m_fondo_id, m_fondo->id);
	Refresh();
}


void CFondoDlg::OnSelchangeListFondos()
{
	// TODO: Add your control notification handler code here
	m_fondo = m_list_all_fondosLB.current();
	if (!m_fondo)	return;
	SetText(m_fondo_id, m_fondo->id);
	m_tipo_cupon.set_value(m_fondo->tipo_cupon);
	SetText(m_fondo_abrev, m_fondo->abreviacion);
	SetText(m_fondo_name, m_fondo->nombre);
}


void CFondoDlg::OnBnClickedNew()
{
	// TODO: Add your control notification handler code here
	SetText(m_fondo_id, ""s);
	m_tipo_cupon.set_value(Fondo::trimestral);
	SetText(m_fondo_abrev, ""s);
	SetText(m_fondo_name, ""s);
	m_fondo = std::nullopt;
}


void CFondoDlg::OnBnClickedErase()
{
	// TODO: Add your control notification handler code here
	if (m_list_all_fondosLB.delete_current_sel())
	{
		OnBnClickedNew();
	}
}
