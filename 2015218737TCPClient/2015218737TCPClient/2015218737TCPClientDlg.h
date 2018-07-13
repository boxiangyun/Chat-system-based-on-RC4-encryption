
// 2015218737TCPClientDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CMy2015218737TCPClientDlg dialog
class CMy2015218737TCPClientDlg : public CDialogEx
{
// Construction
public:
	CMy2015218737TCPClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MY2015218737TCPCLIENT_DIALOG };

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
	CIPAddressCtrl ServerIP;
	
	CString m_strRecvMessage;
	CString m_strSendMessage;
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnEnChangeEditMessage();
	
	CString m_encode;
	afx_msg void OnEnChangeEditEncode();
	//CEdit m_key;
	//afx_msg void OnBnClickedButtongetmi();
	CString mikey_string;
};
