// RendimientoDlg.cpp : implementation file
//

#include "pch.h"
#include "Investments.h"
#include "afxdialogex.h"
#include "RendimientoDlg.h"


// RendimientoDlg dialog

IMPLEMENT_DYNAMIC(RendimientoDlg, CDialog)

RendimientoDlg::RendimientoDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_RendimientoDlg, pParent),
	m_list_rendimientosLB(m_list_rendimientos, [](Rendimiento& rend)
	{
			return Util::to_cstring(rend.simple_dump());
	}),
	m_list_fondosCB(m_list_fondos, [](Fondo& fondo)
	{
			return Util::to_cstring(fondo.simple_dump());
	})
{

}

RendimientoDlg::~RendimientoDlg()
{
}

void RendimientoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_E_RENDIMIENTO_ID, m_rendimiento_id);
	DDX_Control(pDX, IDC_E_RENDIMIENTO_UNITARIO, m_rendimiento_unitario);
	DDX_Control(pDX, IDC_DT_FECHA_RENDIMIENTO, m_fecha_rendimiento);
	DDX_Control(pDX, IDC_CB_FONDOS, m_list_fondos);
	DDX_Control(pDX, IDC_LIST_RENDIMIENTOS, m_list_rendimientos);
}


BEGIN_MESSAGE_MAP(RendimientoDlg, CDialog)
	ON_BN_CLICKED(ID_APPLY, &RendimientoDlg::OnBnClickedApply)
	ON_LBN_SELCHANGE(IDC_LIST_RENDIMIENTOS, &RendimientoDlg::OnLbnSelchangeListRendimientos)
END_MESSAGE_MAP()


// RendimientoDlg message handlers


BOOL RendimientoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	Refresh();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void RendimientoDlg::Refresh()
{
	m_list_rendimientosLB.loadLBOrderBy(&Rendimiento::fecha);
	m_list_fondosCB.loadLBOrderBy(&Fondo::nombre);
}


void RendimientoDlg::OnBnClickedApply()
{
	// TODO: Add your control notification handler code here
	m_rendimiento = getCurrent<Rendimiento>(m_rendimiento_id);

	auto fondo = m_list_fondosCB.current();
	if (!fondo)
	{
		MessageBoxW(L"Falta escoger Fondo");
		return;
	}

	auto rend_unit = GetAmount(m_rendimiento_unitario);
	std::chrono::sys_days fecha = GetDate(m_fecha_rendimiento);

	if (!m_rendimiento)	// insert
	{
		m_rendimiento = m_list_rendimientosLB.insert(fondo->id, rend_unit, fecha);
		m_list_rendimientosLB.insert_into_listbox(*m_rendimiento);
	}
	else				// update
	{
		m_rendimiento->fecha = fecha;
		m_rendimiento->fecha = fecha;
		m_rendimiento->fkey_fondo = fondo->id;
		m_list_rendimientosLB.update(*m_rendimiento);
	}

	setIdFromRecord<Rendimiento>(m_rendimiento_id, m_rendimiento->id);
	Refresh();
}




void RendimientoDlg::OnLbnSelchangeListRendimientos()
{
	// TODO: Add your control notification handler code here
	m_rendimiento = m_list_rendimientosLB.current();
	if (!m_rendimiento)	return;
	SetText(m_rendimiento_id, m_rendimiento->id);
	m_list_fondosCB.select(m_rendimiento->fkey_fondo);
	SetDate(m_fecha_rendimiento, m_rendimiento->fecha);
	SetAmount(m_rendimiento_unitario, m_rendimiento->rendimiento_unitario);
}
