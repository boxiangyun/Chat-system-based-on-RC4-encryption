
// 2015218737TCPClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "2015218737TCPClient.h"
#include "2015218737TCPClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define DEFAULT_PORT 2000//服务器监听端口，注意与服务器端保持一致
char iKey[256] = "Ajustfortest";//初始密钥，可变长，最大256
unsigned char sBox[256] = { 0 };//S-box
unsigned char s_Box[256] = { 0 };


void get_rdom_key(char *k,int i)//生成随机密钥
{
	int x = 1;
	for (; x < i;x++)
		k[x] = 'a' + rand() % 26;
	
}

void rc4_init(unsigned char *s, char *key, unsigned long Len) /*初始化函数，s为S-box，key为初始密钥，Len为初始密钥的长度*/
{
	int i = 0, j = 0;
	char k[256] = { 0 };//存放密钥流
	unsigned char tmp = 0;
	//cout << "密钥长度：" << Len<<endl;
	//第一步	
	/***************************/
	//此处添加额外代码实现S-box和密钥流的初始化
	for (; i < 256; i++)
	{
		s[i] = i;
		k[i] = key[i % Len];
	}
	/***************************/

	/*第二步，用于打乱S-box，其中i循环用于保证S-box每位都保证得到混淆，key中每个字节的引入是为了增加随机因子*/
	/***************************/
	//此处添加额外代码实现S-box的置乱
	for (i = 0; i < 256; i++)
	{
		j = (j + s[i] + k[i]) % 256;
		tmp = s[j];
		s[j] = s[i];
		s[i] = tmp;
	}
	/***************************/

}

void rc4_crypt(unsigned char*s, char*Data, char*Data2, unsigned long Len) /*加解密，s为S-box，Data为数据，Len为数据的长度*/
{
	int i = 0, j = 0, t = 0;
	unsigned long k = 0;
	unsigned char tmp;
	//cout << "明文长度：" << Len << endl;
	/***************************/
	//此处添加额外代码加解密
	for (; k < Len; k++)
	{
		i = (i + 1) % 256;
		j = (j + s[i]) % 256;
		tmp = s[i];
		s[i] = s[j];
		s[j] = tmp;
		t = (s[i] + s[j]) % 256;
		Data[k] ^= s[t];
		Data2[k] = Data[k];

	}


	/***************************/
}

char * CSt2char(CString str)
{
	//注意：以下n和len的值大小不同,n是按字符计算的， len是按字节计算的
	int n = str.GetLength(); // n = 14, len = 18
	//获取宽字节字符的大小，大小是按字节计算的
	int len = WideCharToMultiByte(CP_ACP, 0, str, str.GetLength(), NULL, 0, NULL, NULL);
	//为多字节字符数组申请空间，数组大小为按字节计算的宽字节字节大小
	char * buff = new char[len + 1]; //以字节为单位//宽字节编码转换成多字节编码
	WideCharToMultiByte(CP_ACP, 0, str, str.GetLength() + 1, buff, len + 1, NULL, NULL);
	buff[len + 1] = '\0'; //多字节字符以'/0'结束
	return buff;
}

CString char2CSt(char * szBuf)
{
	//计算char *数组大小，以字节为单位，一个汉字占两个字节
	int charLen = strlen(szBuf);
	//计算多字节字符的大小，按字符计算。
	int len = MultiByteToWideChar(CP_ACP, 0, szBuf, charLen, NULL, 0);
	//为宽字节字符数组申请空间，数组大小为按字节计算的多字节字符大小
	TCHAR *buf = new TCHAR[len + 1];
	//多字节编码转换成宽字节编码
	MultiByteToWideChar(CP_ACP, 0, szBuf, charLen, buf, len);
	buf[len] = '\0'; //添加字符串结尾，注意不是len+1
	//将TCHAR数组转换为CString
	CString pWideChar;
	pWideChar.Append(buf);
	//删除缓冲区
	delete[]buf;
	return pWideChar;
}


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMy2015218737TCPClientDlg dialog



CMy2015218737TCPClientDlg::CMy2015218737TCPClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMy2015218737TCPClientDlg::IDD, pParent)
	, m_strSendMessage(_T(""))
	, m_encode(_T(""))
	, mikey_string(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy2015218737TCPClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS_SERVER, ServerIP);

	//DDX_Text(pDX, IDC_STATIC_RECV, m_strRecvMessage);
	DDX_Text(pDX, IDC_EDIT_MESSAGE, m_strSendMessage);
	DDX_Text(pDX, IDC_EDIT_ENCODE, m_encode);
	//DDX_Control(pDX, IDC_EDIT_mikey, m_key);
	DDX_Text(pDX, IDC_EDIT_mikey, mikey_string);
}

BEGIN_MESSAGE_MAP(CMy2015218737TCPClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CMy2015218737TCPClientDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CMy2015218737TCPClientDlg::OnBnClickedButtonSend)
	ON_EN_CHANGE(IDC_EDIT_MESSAGE, &CMy2015218737TCPClientDlg::OnEnChangeEditMessage)
	
	ON_EN_CHANGE(IDC_EDIT_ENCODE, &CMy2015218737TCPClientDlg::OnEnChangeEditEncode)
	//ON_BN_CLICKED(IDC_BUTTON_get_mi, &CMy2015218737TCPClientDlg::OnBnClickedButtongetmi)
END_MESSAGE_MAP()


// CMy2015218737TCPClientDlg message handlers

BOOL CMy2015218737TCPClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	WSADATA wsaData;//结构被用来保存函数 WSAStartup 返回的 Windows Socket 初始化信息
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
	{
		AfxMessageBox(_T("WSAStartup error！ "));
		return FALSE;
	}//当一个应用程序调用 WSAStartup 函数时， 操作系统根据请求的 Socket 版本来搜索相应的 Socket 库，然后绑定找到的 Socket 库到该应用程序中
	//表示要使用 1.1 版本的 Socket
	
	ServerIP.SetAddress(192, 168, 43, 192);
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMy2015218737TCPClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMy2015218737TCPClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMy2015218737TCPClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMy2015218737TCPClientDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
	WSACleanup();//释放资源
}


void CMy2015218737TCPClientDlg::OnBnClickedButtonSend()
{
	// TODO: Add your control notification handler code here
	sockaddr_in local;
	local.sin_family = AF_INET;//必须是AF_INET,表示该socket在Internet域中进行通信
	local.sin_port = htons(DEFAULT_PORT);//端口号
	char IPAddress[MAX_PATH];
	BYTE add1, add2, add3, add4;
	ServerIP.GetAddress(add1, add2, add3, add4);
	sprintf_s(IPAddress, "%d.%d.%d.%d", add1, add2, add3, add4);
	local.sin_addr.S_un.S_addr = inet_addr(IPAddress);//服务器的IP地址。

	SOCKET socketTmp = socket(AF_INET, SOCK_STREAM, 0);///初始化Socket，与服务器端保持一致if (connect(socketTmp, (LPSOCKADDR)& local, sizeof(local)) != 0)//连接服务器
	
	if (connect(socketTmp, (LPSOCKADDR)& local, sizeof(local)) != 0)//连接服务器
	{
		closesocket(socketTmp);
		AfxMessageBox(_T("连接服务器失败"));
		return;
	}
	get_rdom_key(iKey, 12);//生成随机密钥

	char szText[MAX_PATH];//数据缓冲区
	memset(szText, 0, MAX_PATH);
	UpdateData(TRUE);
	rc4_init(sBox, iKey, strlen(iKey));
	for (int x = 0; x < 256; x++)
	{
		s_Box[x] = sBox[x];
	}
	rc4_crypt(sBox, CSt2char(m_strSendMessage), szText, strlen(CSt2char(m_strSendMessage)));
	m_encode = szText;//将密文赋予控件变量
	mikey_string = iKey;//将密钥赋予控件变量
	strcat(szText,iKey);//密钥与密文拼接
	send(socketTmp, szText, sizeof szText, 0);//发送消息
	memset(szText, 0, MAX_PATH);//清空缓冲区
	
	UpdateData(FALSE);//刷新界面
	shutdown(socketTmp, 2);//关闭读写通道
	closesocket(socketTmp);
}

/*
void CMy2015218737TCPClientDlg::OnBnClickedButtongetmi()
{
	// TODO: Add your control notification handler code here
	sockaddr_in local;
	local.sin_family = AF_INET;//必须是AF_INET,表示该socket在Internet域中进行通信
	local.sin_port = htons(DEFAULT_PORT);//端口号
	char IPAddress[MAX_PATH];
	BYTE add1, add2, add3, add4;
	ServerIP.GetAddress(add1, add2, add3, add4);
	sprintf_s(IPAddress, "%d.%d.%d.%d", add1, add2, add3, add4);
	local.sin_addr.S_un.S_addr = inet_addr(IPAddress);//服务器的IP地址。

	SOCKET socketTmp = socket(AF_INET, SOCK_STREAM, 0);///初始化Socket，与服务器端保持一致if (connect(socketTmp, (LPSOCKADDR)& local, sizeof(local)) != 0)//连接服务器
	if (connect(socketTmp, (LPSOCKADDR)& local, sizeof(local)) != 0)//连接服务器
	{
		closesocket(socketTmp);
		AfxMessageBox(_T("连接服务器失败"));
		return;
	}

	get_rdom_key(iKey,11);
	send(socketTmp, iKey, sizeof iKey, 0);//发送消息
	mikey_string = iKey;
	UpdateData(FALSE);//刷新界面
}*/


void CMy2015218737TCPClientDlg::OnEnChangeEditMessage()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	
}




void CMy2015218737TCPClientDlg::OnEnChangeEditEncode()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

}

