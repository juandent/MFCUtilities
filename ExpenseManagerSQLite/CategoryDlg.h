#pragma once

#include "BoxContents.h"

// CategoryDlg dialog

class CategoryDlg : public CDialog
{
	DECLARE_DYNAMIC(CategoryDlg)

	BoxContents<Categoria, &Categoria::id_categoria> m_categoriasLB;

	std::string m_category_name;
	std::optional<Categoria> m_category;
public:
	CategoryDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CategoryDlg();

	void setDiscriminator( const std::string& name)
	{
		m_category_name = name;
	}

	auto getCompleteObject()
	{
		return m_category;
	}
	
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CategoryDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CEdit m_nombre;
	CButton m_gasto_o_ingreso_real;
	CListBox m_list_categorias;
public:
	afx_msg void OnBnClickedBBorrar();
	afx_msg void OnBnClickedBAplicarCategory();
	afx_msg void OnBnClickedBUpdate();
private:
	CEdit m_id_categoria;
};
