
// 2015218737-CPServeDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CMy2015218737CPServeDlg dialog
class CMy2015218737CPServeDlg : public CDialogEx
{
// Construction
public:
	CMy2015218737CPServeDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MY2015218737CPSERVE_DIALOG };

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
	afx_msg void OnBnClickedCancel();
	CIPAddressCtrl HostIP;
	void GetLocalIP();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	CButton m_btnStart;
	CButton m_btnStop;
	CEdit m_edtMessage;
	CEdit m_encode;
	
	CEdit m_get_mi;
};
