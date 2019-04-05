// ZPByPassAllDlg.h : header file
//


#pragma once


// CZPByPassAllDlg dialog

class CZPByPassAllDlg : public CDialog
{
// Construction
public:
	CZPByPassAllDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_ZPBYPASSALL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


public:
	//user Here
     
	 

public:
	afx_msg void OnBnClickedButton3();
public:
	afx_msg void OnBnClickedBtnexit();
public:
	afx_msg void OnBnClickedBtngenerate();
public:
	afx_msg void OnBnClickedBtnidcBtngenerate();
public:
	afx_msg void OnBnClickedBtnbrowse();
};

