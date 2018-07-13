
// 2015218737-CPServeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "2015218737-CPServe.h"
#include "2015218737-CPServeDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DEFAULT_PORT 2000//服务器监听端口
SOCKET m_Listening;//监听socket
HANDLE m_hListenThread;//线程句柄
char iKey[256] = "justfortest";//初始密钥，可变长，最大256
unsigned char sBox[256] = { 0 };//S-box
unsigned char s_Box[256] = { 0 };
// CAboutDlg dialog used for App About

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

void rc4_crypt(unsigned char*s, char*Data, unsigned long Len) /*加解密，s为S-box，Data为数据，Len为数据的长度*/
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
	}


	/***************************/
}


char * CSt2char(CString str)
{
	//注意：以下n和len的值大小不同,n是按字符计算的， len是按字节计算的
	int n = str.GetLength(); // n = 14, len = 18//获取宽字节字符的大小，大小是按字节计算的
	int len = WideCharToMultiByte(CP_ACP, 0, str, str.GetLength(), NULL, 0, NULL, NULL);
	//为多字节字符数组申请空间，数组大小为按字节计算的宽字节字节大小
	char * buff = new char[len + 1]; //以字节为单位
	//宽字节编码转换成多字节编码
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



// CMy2015218737CPServeDlg dialog

DWORD WINAPI ListenThread(LPVOID lpparam)
{
	int length = 0;//接收字符串长度
	CMy2015218737CPServeDlg* pDlg = (CMy2015218737CPServeDlg*)lpparam;
	if (pDlg == NULL) return 0;//获取当前主对话框的指针
	SOCKET ConnectSocket;//记录捕捉到的连接
	sockaddr_in ClientAddr;//记录数据包的IP地址
	int nLen = sizeof(sockaddr);
	if (listen(m_Listening, 40) == SOCKET_ERROR)//开始监听是否有客户端连接。{
		return 0;

	char szBuf[MAX_PATH];//数据缓冲区
	memset(szBuf,0,MAX_PATH);//初始化缓冲区
	char szBuf_key[20];//保存密钥
	memset(szBuf_key, 0, 20);//初始化缓冲区
	char szBuf_mi[200];//保存密文
	memset(szBuf_mi, 0, 200);//初始化缓冲区

	while(1)
	{
		ConnectSocket = accept(m_Listening, (sockaddr*)&ClientAddr, &nLen);//阻塞直到有客户端连
		//接，不然多浪费CPU资源
		char *pAddrname = inet_ntoa(ClientAddr.sin_addr);//得到客户端的IP地址
		memset(szBuf, 0, MAX_PATH);//每次接收数据前清空缓冲区
		if(recv(ConnectSocket, szBuf, sizeof szBuf, 0) != SOCKET_ERROR)
		{
			/*利用A字符完成对密钥与密文信息的分开提取*/
			length = strlen(szBuf);
			int i = 0,j = 0;
			for (; i < length; i++)
			{
				if (szBuf[i] != 'A')
					szBuf_mi[i] = szBuf[i];
				else
					break;
			}
			for (;i < length ;i++ )
				szBuf_key[j++] = szBuf[i];

			
			pDlg->m_get_mi.SetWindowTextW(char2CSt(szBuf_key));
			
			
			pDlg->m_encode.SetWindowTextW(char2CSt(szBuf_mi));
			rc4_init(sBox, szBuf_key, strlen(szBuf_key));
			
			for (int x = 0; x < 256; x++)
			{
				s_Box[x] = sBox[x];
			}
			rc4_crypt(s_Box, szBuf_mi, strlen(szBuf_mi));
			pDlg->m_edtMessage.SetWindowTextW(char2CSt(szBuf_mi));
			
			
			char buff[MAX_PATH] = "牛逼牛逼";
			strcat_s(buff, pAddrname);
			strcat_s(buff, "） ");
			send(ConnectSocket, buff, sizeof buff, 0);
			
		}
	}
	return 0;
}

CMy2015218737CPServeDlg::CMy2015218737CPServeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMy2015218737CPServeDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy2015218737CPServeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS_HOST, HostIP);
	DDX_Control(pDX, IDC_BUTTON_START, m_btnStart);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_btnStop);
	DDX_Control(pDX, IDC_EDIT_MESSAGE, m_edtMessage);
	DDX_Control(pDX, IDC_EDIT_MESSAGE2, m_encode);
	DDX_Control(pDX, IDC_EDIT_get_mi, m_get_mi);
}

BEGIN_MESSAGE_MAP(CMy2015218737CPServeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CMy2015218737CPServeDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_START, &CMy2015218737CPServeDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CMy2015218737CPServeDlg::OnBnClickedButtonStop)
END_MESSAGE_MAP()


// CMy2015218737CPServeDlg message handlers

BOOL CMy2015218737CPServeDlg::OnInitDialog()
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
	WSADATA wsaData;//结构被用来保存函数WSAStartup返回的Windows Socket 初始化信息
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
	{
		AfxMessageBox(_T("WSAStartup error！ "));
		return FALSE;
	}//当一个应用程序调用WSAStartup函数时，操作系统根据请求的Socket版本来搜索相应的Socket
	//库，然后绑定找到的Socket库到该应用程序中
		//表示要使用1.1版本的Socket
	GetLocalIP();//获取 IP 地址
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMy2015218737CPServeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMy2015218737CPServeDlg::OnPaint()
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
HCURSOR CMy2015218737CPServeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMy2015218737CPServeDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	WSACleanup();
	CDialogEx::OnCancel();
}


void CMy2015218737CPServeDlg::GetLocalIP()
{
	char HostName[128];//记录主机名
	char *IPAddress;//记录IP地址
	if (gethostname(HostName, 128) == 0)//获取主机名成功
	{
		struct hostent *pHost;
		pHost = gethostbyname(HostName);//用域名或主机名获取IP地址
		IPAddress = pHost->h_addr_list[0];//取第一个地址
		HostIP.SetAddress(*IPAddress, *(IPAddress + 1), *(IPAddress + 2), *(IPAddress + 3));//设置IP地址
		
		UpdateData(FALSE);//刷新界面
	}
}


void CMy2015218737CPServeDlg::OnBnClickedButtonStart()
{
	// TODO: Add your control notification handler code here
	m_btnStart.EnableWindow(FALSE);
	m_btnStop.EnableWindow(TRUE);

	sockaddr_in local;//使用sockaddr_in结构指定IP地址和端口信息
	local.sin_family = AF_INET;
	local.sin_port = htons(DEFAULT_PORT);//设置的端口为DEFAULT_PORT。将整型变量从主机字节顺序转变成网络字节顺序
	char IPAddress[MAX_PATH];//记录IP地址
	BYTE add1, add2, add3, add4;
	HostIP.GetAddress(add1, add2, add3, add4);
	sprintf_s(IPAddress, "%d.%d.%d.%d", add1, add2, add3, add4);
	local.sin_addr.S_un.S_addr = inet_addr(IPAddress);//将一个点分十进制的IP转换成一个长整数型数(u_long类型), 返回的整数形式是网络字节序
	m_Listening = socket(AF_INET, SOCK_STREAM, 0);//初始化Socket，采用TCP方式-SOCK_STREAM
	if (m_Listening == INVALID_SOCKET)//创建失败
	{
		AfxMessageBox(_T("初始化socket失败！ "));
		return;
	}
	if (bind(m_Listening, (LPSOCKADDR)& local, sizeof(local)) == SOCKET_ERROR)//将IP地址和端口绑定到所创建的套接字上
	{
		AfxMessageBox(_T("绑定失败！ "));
		closesocket(m_Listening);
		return;
	}
	//创建监听线程
	DWORD dwThreadID = 0;
	m_hListenThread = ::CreateThread(NULL, 0, ListenThread, this, 0, &dwThreadID);//ListenThread为线程函数， this为当前主对话框的指针
}


void CMy2015218737CPServeDlg::OnBnClickedButtonStop()
{
	// TODO: Add your control notification handler code here
	m_btnStart.EnableWindow(TRUE);
	m_btnStop.EnableWindow(FALSE);

	TerminateThread(m_hListenThread, 0);//关闭线程
	CloseHandle(m_hListenThread);//释放资源
	shutdown(m_Listening, 2);//关闭读写通道
	closesocket(m_Listening);//释放socket
}
