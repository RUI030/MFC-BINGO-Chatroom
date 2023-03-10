
// TCP_C_AsyncDlg.cpp: 實作檔案
//

#include "stdafx.h"
#include "TCP_C_Async.h"
#include "TCP_C_AsyncDlg.h"
#include "afxdialogex.h"

//TCPIP
#include "TCPIP_Async.cpp"
#define   CEVENT  (WM_USER+100)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//Define+Declare Game
#define INIT_X		100
#define INIT_Y		250
#define WIN_Size	750
#define N			5
#define	PointID		6
#define Margin		5

int		GameBtnMode=0;

int		R, D;
int		Seg;					//七段顯示邊長
int		NowSelect=0;			
int		NSX, NSY;				//Now Select X,Y
bool	Start;
bool	MLB;					//Mouse Left Button Flag
int		MX, MY;					//Mouse X,Y
int		POS_X[26],POS_Y[26];

wchar_t	LW[100];

struct  OBJ
{
	int		X, Y;						//圓心座標
	int		DOT_X[2][PointID];			//定位點:[位數](0:左 1:右) [定位點編號](0:左上 1:右上 2:左中 3:右中 4:左下 5:右下)
	int		DOT_Y[2][PointID];			//定位點:[位數](0:左 1:右) [定位點編號](0:左上 1:右上 2:左中 3:右中 4:左下 5:右下)
	int		NUM;						//數字
	bool	Flag = 0;
}Map[N][N];

#define	bit 2
#define seg 7

HDC		hdc;

//##########Draw##########
void Line(HDC hdc, int x1, int y1, int x2, int y2, int C,int B)
{
	CPen P1(PS_SOLID, B, C);
	SelectObject(hdc, P1);
	MoveToEx(hdc, x1, y1, 0);
	LineTo(hdc, x2, y2);
}
void Rect(HDC hdc, int x1, int y1, int x2, int y2, int C, int F)
{
	CPen P1(PS_SOLID, 1, C);
	CBrush B1(C);
	SelectObject(hdc, P1);
	if (F) { SelectObject(hdc, B1); }
	else { SelectObject(hdc, ::GetStockObject(NULL_BRUSH)); }
	Rectangle(hdc, min(x1, x2), min(y1, y2), max(x1, x2), max(y1, y2));
}
void Ball(HDC hdc, int cx, int cy, int r, int C)
{
	CPen P1(PS_SOLID, 1, C);
	CBrush B1(C);
	SelectObject(hdc, P1);//外框
	SelectObject(hdc, B1);//填滿
	Ellipse(hdc, cx - r, cy - r, cx + r, cy + r);
}
void _7Seg(HDC hdc,int num,int X,int Y)
{
	//防呆
	num %= 26;
	X %= 5;
	Y %= 5;
	//HDC
	CPen P1(PS_SOLID, 5, 0xEEEEEE);
	SelectObject(hdc, P1);
	//數字轉亮的燈的位置
	int		i,j,k,tmp,tmp2;
	int		DOT_X[6];
	int		DOT_Y[6];
	for (i = 0;i < bit;i++)
	{
		//抓出座標
		for (k = 0;k < 6;k++)
		{
			DOT_X[k] = Map[X][Y].DOT_X[i][k];
			DOT_Y[k] = Map[X][Y].DOT_Y[i][k];
		}
		//繪製
		for (j = 0;j < seg;j++)
		{
			tmp2 = pow(10.0,(double)i);
			tmp = (num/tmp2)%10;
			switch (tmp)
			{
				case 0:
					if (!i)//只畫個位數
					{
						//左上->右上
						MoveToEx(hdc, DOT_X[0], DOT_Y[0], 0);
						LineTo(hdc, DOT_X[1], DOT_Y[1]);
						//->右下
						LineTo(hdc, DOT_X[5], DOT_Y[5]);
						//->左下
						LineTo(hdc, DOT_X[4], DOT_Y[4]);
						//->左上
						LineTo(hdc, DOT_X[0], DOT_Y[0]);
					}
					break;
				case 1:
					//右上->右下
					MoveToEx(hdc, DOT_X[1], DOT_Y[1], 0);
					LineTo(hdc, DOT_X[5], DOT_Y[5]);
					break;
				case 2:
					//左上->右上
					MoveToEx(hdc, DOT_X[0], DOT_Y[0], 0);
					LineTo(hdc, DOT_X[1], DOT_Y[1]);
					//->右中
					LineTo(hdc, DOT_X[3], DOT_Y[3]);
					//->左下
					LineTo(hdc, DOT_X[4], DOT_Y[4]);
					//->右下
					LineTo(hdc, DOT_X[5], DOT_Y[5]);
					break;
				case 3:
					//左上->右上
					MoveToEx(hdc, DOT_X[0], DOT_Y[0], 0);
					LineTo(hdc, DOT_X[1], DOT_Y[1]);
					//->右下
					LineTo(hdc, DOT_X[5], DOT_Y[5]);
					//->左下
					LineTo(hdc, DOT_X[4], DOT_Y[4]);
					//右中->左中
					MoveToEx(hdc, DOT_X[3], DOT_Y[3], 0);
					LineTo(hdc, DOT_X[2], DOT_Y[2]);
					break;
				case 4:
					//右下->右上
					MoveToEx(hdc, DOT_X[5], DOT_Y[5], 0);
					LineTo(hdc, DOT_X[1], DOT_Y[1]);
					//左中
					LineTo(hdc, DOT_X[2], DOT_Y[2]);
					//右中
					LineTo(hdc, DOT_X[3], DOT_Y[3]);
					break;
				case 5:
					//右上->左上
					MoveToEx(hdc, DOT_X[1], DOT_Y[1], 0);
					LineTo(hdc, DOT_X[0], DOT_Y[0]);
					//->左中
					LineTo(hdc, DOT_X[2], DOT_Y[2]);
					//->右中
					LineTo(hdc, DOT_X[3], DOT_Y[3]);
					//->右下
					LineTo(hdc, DOT_X[5], DOT_Y[5]);
					//->左下
					LineTo(hdc, DOT_X[4], DOT_Y[4]);
					break;
				case 6:
					//右上->左中
					MoveToEx(hdc, DOT_X[1], DOT_Y[1], 0);
					LineTo(hdc, DOT_X[2], DOT_Y[2]);
					//->左下
					LineTo(hdc, DOT_X[4], DOT_Y[4]);
					//->右下
					LineTo(hdc, DOT_X[5], DOT_Y[5]);
					//->右中
					LineTo(hdc, DOT_X[3], DOT_Y[3]);
					//->左中
					LineTo(hdc, DOT_X[2], DOT_Y[2]);
					break;
				case 7:
					//左上->右上
					MoveToEx(hdc, DOT_X[0], DOT_Y[0], 0);
					LineTo(hdc, DOT_X[1], DOT_Y[1]);
					//->左下
					LineTo(hdc, DOT_X[4], DOT_Y[4]);
					break;
				case 8:
					//左上->右上
					MoveToEx(hdc, DOT_X[0], DOT_Y[0], 0);
					LineTo(hdc, DOT_X[1], DOT_Y[1]);
					//->右下
					LineTo(hdc, DOT_X[5], DOT_Y[5]);
					//->左下
					LineTo(hdc, DOT_X[4], DOT_Y[4]);
					//->左上
					LineTo(hdc, DOT_X[0], DOT_Y[0]);
					//左中->右中
					MoveToEx(hdc, DOT_X[3], DOT_Y[3], 0);
					LineTo(hdc, DOT_X[2], DOT_Y[2]);
					break;
				case 9:
					//右中->左中
					MoveToEx(hdc, DOT_X[3], DOT_Y[3], 0);
					LineTo(hdc, DOT_X[2], DOT_Y[2]);
					//->左上
					LineTo(hdc, DOT_X[0], DOT_Y[0]);
					//->右上
					LineTo(hdc, DOT_X[1], DOT_Y[1]);
					//->右中
					LineTo(hdc, DOT_X[3], DOT_Y[3]);
					//->左下
					LineTo(hdc, DOT_X[4], DOT_Y[4]);
					break;
				default:
					break;
			}
		}
	}
}

//########################

//##########GAME###########
int randint(int min, int max)
{
	int delta = max - min;
	int i = rand() + rand() + 8763;
	i %= delta;
	i += min;
	return i;
}

void CTCPCAsyncDlg::GameWindowInit()
{
	int		i, j;
	//直徑
	D = (WIN_Size - (Margin*(N + 1))) / N;
	//半徑
	R = D / 2;	
	//七段顯示邊長
	Seg = R * 0.5;

	//座標
	for (i = 0;i < N; i++)
	{
		for (j = 0;j < N;j++)
		{
			//圓心
			Map[i][j].X = INIT_X + (((i + 1)*(Margin)) + (i*D))+R;
			Map[i][j].Y = INIT_Y + (((j + 1)*(Margin)) + (j*D))+R;
			//定位點
				//左:十位(1)
					//右中(3)
					Map[i][j].DOT_X[1][3] = Map[i][j].X-(R*0.1);			//圓心往左0.1R
					Map[i][j].DOT_Y[1][3] = Map[i][j].Y;
					//右上(1)
					Map[i][j].DOT_X[1][1] = Map[i][j].DOT_X[1][3];	
					Map[i][j].DOT_Y[1][1] = Map[i][j].DOT_Y[1][3] - Seg;	//右中往上Seg
					//右下(5)
					Map[i][j].DOT_X[1][5] = Map[i][j].DOT_X[1][3];
					Map[i][j].DOT_Y[1][5] = Map[i][j].DOT_Y[1][3] + Seg;	//右中往下Seg

					//左中(2)
					Map[i][j].DOT_X[1][2] = Map[i][j].DOT_X[1][3] - Seg;	//右中往左Seg
					Map[i][j].DOT_Y[1][2] = Map[i][j].DOT_Y[1][3];
					//左上(0)
					Map[i][j].DOT_X[1][0] = Map[i][j].DOT_X[1][2];
					Map[i][j].DOT_Y[1][0] = Map[i][j].DOT_Y[1][2] - Seg;	//左中往上Seg
					//左下(4)
					Map[i][j].DOT_X[1][4] = Map[i][j].DOT_X[1][2];
					Map[i][j].DOT_Y[1][4] = Map[i][j].DOT_Y[1][2] + Seg;	//左中往下Seg

				//右:個位(0)
					//左中(2)
					Map[i][j].DOT_X[0][2] = Map[i][j].X + (R*0.1);			//圓心往右0.1R
					Map[i][j].DOT_Y[0][2] = Map[i][j].Y;
					//左上(0)
					Map[i][j].DOT_X[0][0] = Map[i][j].DOT_X[0][2];
					Map[i][j].DOT_Y[0][0] = Map[i][j].DOT_Y[0][2] - Seg;	//左中往上Seg
					//左下(4)
					Map[i][j].DOT_X[0][4] = Map[i][j].DOT_X[0][2];
					Map[i][j].DOT_Y[0][4] = Map[i][j].DOT_Y[0][2] + Seg;	//左中往下Seg

					//右中(3)
					Map[i][j].DOT_X[0][3] = Map[i][j].DOT_X[0][2] + Seg;	//左中往右Seg
					Map[i][j].DOT_Y[0][3] = Map[i][j].DOT_Y[0][2];
					//右上(1)
					Map[i][j].DOT_X[0][1] = Map[i][j].DOT_X[0][3];
					Map[i][j].DOT_Y[0][1] = Map[i][j].DOT_Y[0][3] - Seg;	//右中往上Seg
					//右下(5)
					Map[i][j].DOT_X[0][5] = Map[i][j].DOT_X[0][3];
					Map[i][j].DOT_Y[0][5] = Map[i][j].DOT_Y[0][3] + Seg;	//右中往下Seg
		}
	}
}

void CTCPCAsyncDlg::NewGame()
{
	Start = 0;
	int i, j,k,tmp;
	int x1, y1, x2, y2;
	//邏輯
	tmp = 1;
	for (i = 0;i < N;i++)
	{
		for (j = 0;j < N;j++)
		{
			//初始化數字
			Map[i][j].NUM = tmp;
			//初始化Flag
			tmp++;
			Map[i][j].Flag = 0;
		}
	}
	//洗牌法打亂排序
	
	for (k = 0;k < 25;k++)
	{
		//隨機位置1
		x1 = rand()*rand() % N;
		y1 = rand()*rand() % N;
		//隨機位置2
		x2 = rand()*rand() % N;
		y2 = rand()*rand() % N;
		//交換
		tmp = Map[x1][y1].NUM;
		Map[x1][y1].NUM = Map[x2][y2].NUM;
		Map[x2][y2].NUM = tmp;
	}
	//建立數字位置對照表
	for (i = 0;i < N;i++)
	{
		for (j = 0;j < N;j++)
		{
			tmp = Map[i][j].NUM;
			POS_X[tmp] = i;
			POS_Y[tmp] = j;
		}
	}
	//畫面
		//清空畫面
		Rect(hdc,INIT_X,INIT_Y,INIT_X+WIN_Size, INIT_Y + WIN_Size,0xF0F0F0,1);
		for (i = 0;i < N;i++)
		{
			for (j = 0;j < N;j++)
			{
				//繪製圓形
				Ball(hdc,Map[i][j].X, Map[i][j].Y,R,0xB48246);
				//繪製數字
				_7Seg(hdc,Map[i][j].NUM,i,j);
			}
		}
}
//Return Line Num
int LineNum_X(HDC hdc)	//斜線
{
	//HDC
	CPen P1(PS_SOLID, 10, 0x2222A2);
	SelectObject(hdc, P1);

	int i;
	bool isLine;
	int LineNum = 0;
	// 左上右下
	isLine = 1;
	for (i = 0;i < N;i++)
	{
		isLine *= Map[i][i].Flag;
	}
	//畫線
	if (isLine) 
	{ 
		MoveToEx(hdc, Map[0][0].X, Map[0][0].Y,0);
		LineTo(hdc,Map[4][4].X, Map[4][4].Y);
	}
	LineNum += isLine;
	// 右上左下
	isLine = 1;
	for (i = 0;i < N;i++)
	{
		isLine *= Map[i][N - i - 1].Flag;
	}
	LineNum += isLine;
	//畫線
	if (isLine)
	{
		MoveToEx(hdc, Map[0][4].X, Map[0][4].Y, 0);
		LineTo(hdc, Map[4][0].X, Map[4][0].Y);
	}
	return LineNum;
}
int LineNum_V(HDC hdc)	//垂直線
{
	CPen P1(PS_SOLID, 10, 0x2222A2);
	SelectObject(hdc, P1);

	int LineNum = 0;
	int i, j;
	bool isLine;
	for (i = 0;i < N;i++)
	{
		isLine = 1;
		for (j = 0;j < N;j++)
		{
			isLine *= Map[i][j].Flag;
		}
		if (isLine) 
		{
			MoveToEx(hdc, Map[i][0].X, Map[i][0].Y, 0);
			LineTo(hdc, Map[i][4].X, Map[i][4].Y);
		}
		LineNum += isLine;
	}
	return LineNum;
}
int LineNum_H(HDC hdc)	//水平線
{
	CPen P1(PS_SOLID, 10, 0x2222A2);
	SelectObject(hdc, P1);

	int LineNum = 0;
	int i, j;
	bool isLine;
	for (j = 0;j < N;j++)
	{
		isLine = 1;
		for (i = 0;i < N;i++)
		{
			isLine *= Map[i][j].Flag;
		}
		if (isLine) 
		{
			MoveToEx(hdc, Map[0][j].X, Map[0][j].Y, 0);
			LineTo(hdc, Map[4][j].X, Map[4][j].Y);
		}
		LineNum += isLine;
	}
	return LineNum;
}


void CTCPCAsyncDlg::EndGame()
{
	Start = 0;
	NowSelect = 0;
	//清空螢幕
	Rect(hdc, INIT_X, INIT_Y, INIT_X + WIN_Size, INIT_Y + WIN_Size, 0xF0F0F0, 1);
	//按鈕初始化
	GameBtnMode = 0;
	btn_Ctrl_4.SetWindowTextW(_T("New Game"));
}
//########################



// 對 App About 使用 CAboutDlg 對話方塊

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

// 程式碼實作
protected:
	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTCPCAsyncDlg 對話方塊



CTCPCAsyncDlg::CTCPCAsyncDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TCP_C_ASYNC_DIALOG, pParent)
	, m_Edit1(_T(""))
	, m_Edit2(_T(""))
	, m_Edit_3(_T(""))
	, m_Edit4(_T(""))
	, m_Edit6(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTCPCAsyncDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Edit1);
	DDX_Text(pDX, IDC_EDIT2, m_Edit2);
	DDX_Text(pDX, IDC_EDIT3, m_Edit_3);
	DDX_Text(pDX, IDC_EDIT4, m_Edit4);
	DDX_Control(pDX, IDC_BUTTON4, btn_Ctrl_4);
	DDX_Text(pDX, IDC_EDIT6, m_Edit6);
	//  DDX_Control(pDX, IDC_EDIT6, m_Ctrl6);
	DDX_Control(pDX, IDC_EDIT6, m_Ctrl6);
	DDX_Control(pDX, IDC_EDIT1, m_Ctrl1);
}

BEGIN_MESSAGE_MAP(CTCPCAsyncDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CTCPCAsyncDlg::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT2, &CTCPCAsyncDlg::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_BUTTON2, &CTCPCAsyncDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTCPCAsyncDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CTCPCAsyncDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CTCPCAsyncDlg::OnBnClickedButton5)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_BUTTON6, &CTCPCAsyncDlg::OnBnClickedButton6)
END_MESSAGE_MAP()


// CTCPCAsyncDlg 訊息處理常式

BOOL CTCPCAsyncDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 將 [關於...] 功能表加入系統功能表。

	// IDM_ABOUTBOX 必須在系統命令範圍之中。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

	// TODO: 在此加入額外的初始設定
	//數字隨機種子
	srand(time(NULL));
	GameWindowInit();
	GameBtnMode = 0;
	hdc = ::GetDC(m_hWnd);
	Start = 0;
	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

void CTCPCAsyncDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。

void CTCPCAsyncDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 繪製的裝置內容

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 將圖示置中於用戶端矩形
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 描繪圖示
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 當使用者拖曳最小化視窗時，
// 系統呼叫這個功能取得游標顯示。
HCURSOR CTCPCAsyncDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// ########################################################
// ########################################################

SOCKET    CSock;
BOOL      F = 0;
sockaddr_in		CAddr;
char	 IP[20] = { "127.0.0.1" };//Default

void CTCPCAsyncDlg::OnBnClickedButton1()
{
	int      mLen,saLen=sizeof(sockaddr);
	char     S1[2000];
	wchar_t  *S11;
	

	// 1.啟動 TCP Client(第一次才會啟動)
	if (F == 0) 
	{  F = 1; Start_TCP_Client(&CSock, 6000, IP, CEVENT, m_hWnd);	}
	
	// 2.擷取輸入框資訊
	UpdateData(1); S11 = (wchar_t *)m_Edit2.GetBuffer();  UpdateData(0);
	
	// 3.轉換
	UniCodeToBig5(S11, S1, sizeof(S1) - 2);
	// 4.送出
	send(CSock, S1, strlen(S1), 0);
	
}


LRESULT CAboutDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return CDialogEx::WindowProc(message, wParam, lParam);
}


LRESULT CTCPCAsyncDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	int			i, Len = sizeof(sockaddr);
	char		S1[2000],S2[2000],CMDtmp[100],Backward[100];
	wchar_t		SS1[2000];
	if (message == CEVENT)
	{
		switch (LOWORD(lParam))
		{
		case FD_CONNECT: // 連線伺服器成功事件
			UpdateData(1); m_Edit1 += L"連線伺服器成功\r\n"; UpdateData(0);
			break;
		case FD_READ:   // 遠端送來資料事件
			i = recv(wParam, S1, sizeof(S1) - 1, 0);
			if (i > 0)
			{	
				S1[i] = 0;
				int div = 0;
				bool tmpf;
				char CMD[30];
				int cmdn = 0;
				//Special Cmd
				if (S1[0] == '<')
				{
					int k;
					//抓出指令
					for (k = 0;k <= strlen(S1);k++)
					{
						if (!div)//指令
						{
							CMD[k] = S1[k];
							if (S1[k] == '>') { div = k + 1; CMD[k + 1] = '\0'; }
						}
						else
						{
							S2[k - div] = S1[k];
						}
					}
					//判斷指令
					sprintf_s(CMDtmp, 100, "<G_New>");
					if (!strcmp(CMD, CMDtmp)) { cmdn = 1; }
					sprintf_s(CMDtmp, 100, "<G_Start>");
					if (!strcmp(CMD, CMDtmp)) { cmdn = 2; }
					sprintf_s(CMDtmp, 100, "<G_Del>");
					if (!strcmp(CMD, CMDtmp)) { cmdn = 3; }
					sprintf_s(CMDtmp, 100, "<G_End>");
					if (!strcmp(CMD, CMDtmp)) { cmdn = 4; }
					sprintf_s(CMDtmp, 100, "<G_Msg>");
					if (!strcmp(CMD, CMDtmp)) { cmdn = 5; }
				}
				switch (cmdn)
				{
				case 1://New
					btn_Ctrl_4.SetWindowTextW(_T("Join"));
					GameBtnMode = 2;
					break;
				case 2://Start
					Start = 1;
					btn_Ctrl_4.SetWindowTextW(_T("Choose"));
					GameBtnMode = 3;
					break;
				case 3://Del
					int num;
					num = atoi(S2);
					int LineSum;
					LineSum = 0;
					int x, y;
					//抓出位置
					x = POS_X[num];
					y = POS_Y[num];
					//更改Flag
					Map[x][y].Flag = 1;
					//重繪
					//繪製圓形
					Ball(hdc, Map[x][y].X, Map[x][y].Y, R, 0xCCCCCC);
					//繪製數字
					_7Seg(hdc, Map[x][y].NUM, x, y);
					//檢查連線
					LineSum += LineNum_X(hdc);
					LineSum += LineNum_V(hdc);
					LineSum += LineNum_H(hdc);
					if (LineSum >= 5)
					{
						GameBtnMode = 9;
						btn_Ctrl_4.SetWindowTextW(_T("BINGO!"));
					}
					break;
				case 4:
					EndGame();
					break;
				case 5://Msg
					Big5ToUniCode(S2, SS1, strlen(S2) + 1);
					UpdateData(1); m_Edit6 += SS1;  UpdateData(0);
					//Auto Scroll
					m_Ctrl6.LineScroll(m_Ctrl6.GetLineCount());
					break;
				default:
					//普通訊息
					Big5ToUniCode(S1, SS1, strlen(S1) + 1);
					UpdateData(1); m_Edit1 += SS1;  UpdateData(0);
					break;
				}
			}
		break;
		case FD_CLOSE:  // 遠端斷線事件
			closesocket(wParam);
			UpdateData(1); m_Edit1 += L"遠端斷線\r\n"; UpdateData(0);
			break;
		}

	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}


void CTCPCAsyncDlg::OnEnChangeEdit2()
{
	// TODO:  如果這是 RICHEDIT 控制項，控制項將不會
	// 傳送此告知，除非您覆寫 CDialogEx::OnInitDialog()
	// 函式和呼叫 CRichEditCtrl().SetEventMask()
	// 讓具有 ENM_CHANGE 旗標 ORed 加入遮罩。

	// TODO:  在此加入控制項告知處理常式程式碼
}


void CTCPCAsyncDlg::OnBnClickedButton2()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	F = 1;
	wchar_t  *S11;
	//取得m_Edit3輸入的內容
	UpdateData(1); S11 = (wchar_t *)m_Edit_3.GetBuffer();  UpdateData(0);
	//Unicode轉普通Str，並存入IP
	UniCodeToBig5(S11, IP, sizeof(IP) - 2);
	//連線
	Start_TCP_Client(&CSock, 6000, IP, CEVENT, m_hWnd);
}


void CTCPCAsyncDlg::OnBnClickedButton3()
{
	//防呆
	// 0.啟動 TCP Client(第一次才會啟動)
	if (F == 0)
	{F = 1; Start_TCP_Client(&CSock, 6000, IP, CEVENT, m_hWnd);}

	// TODO: 在此加入控制項告知處理常式程式碼
	int      mLen, saLen = sizeof(sockaddr);
	char     S1[2000],S2[2000];
	wchar_t  *S11;

	// 1.擷取輸入框資訊
	UpdateData(1); S11 = (wchar_t *)m_Edit4.GetBuffer();  UpdateData(0);
	// 2.轉換
	UniCodeToBig5(S11, S1, sizeof(S1) - 2);
	// 3.加上前綴
	sprintf_s(S2,"<SetName>%s",S1);
	// 4.送出
	send(CSock, S2, strlen(S2), 0);
}


void CTCPCAsyncDlg::OnBnClickedButton4()
{
	char     S1[2000], S2[2000];
	// TODO: 在此加入控制項告知處理常式程式碼
	switch (GameBtnMode)
	{
		//Host發起新遊戲
		case 0:
			btn_Ctrl_4.SetWindowTextW(_T("Start"));
			//Start = 1;
			NewGame();
			// 1.加上前綴
			sprintf_s(S2, "<G_New>");
			// 2.送出
			send(CSock, S2, strlen(S2), 0);
			GameBtnMode = 1;
			break;
		//Host 下令開始
		case 1:
			// 1.加上前綴
			sprintf_s(S2, "<G_Start>");
			// 2.送出
			send(CSock, S2, strlen(S2), 0);
			Start = 1;
			break;
		//參加遊戲
		case 2:
			NewGame();
			btn_Ctrl_4.SetWindowTextW(_T("Waiting..."));
			// 1.加上前綴
			sprintf_s(S2, "<G_Join>");
			// 2.送出
			send(CSock, S2, strlen(S2), 0);
			GameBtnMode = 3;
			break;
		//送出選擇數字
		case 3:
			// 1.加上前綴
			sprintf_s(S2, "<G_Del>%d\0", NowSelect);
			// 2.送出
			send(CSock, S2, strlen(S2), 0);
			break;
		//Bingo
		case 9:
			// 1.加上前綴
			sprintf_s(S2, "<G_Bingo>");
			// 2.送出
			send(CSock, S2, strlen(S2), 0);
			Start = 0;
			GameBtnMode = 100;
			break;
		default:
			break;
	}
}


void CTCPCAsyncDlg::OnBnClickedButton5()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	//動到視窗
	int i, j;
	for (i = 0;i < N;i++)
	{
		for (j = 0;j < N;j++)
		{
			//繪製圓形
			Ball(hdc, Map[i][j].X, Map[i][j].Y, R, Map[i][j].Flag?0xCCCCCC: 0xB48246);
			//繪製數字
			_7Seg(hdc, Map[i][j].NUM, i, j);
		}
	}
}


void CTCPCAsyncDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	MLB = 1;
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CTCPCAsyncDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	MX = point.x;
	MY = point.y;
	CDialogEx::OnMouseMove(nFlags, point);
}


void CTCPCAsyncDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	int mx, my;//放開瞬間滑鼠的X,Y
	mx = MX;
	my = MY;
	int		i, j, k,delt,dx,dy;
	int		clkx = -1;
	int		clky = -1;
	double	dist;
	char	S1[100];
	if (MLB)
	{
		if (Start)
		{
			//點到的ICON
			//判斷X
			for (i = 0;i < N;i++)
			{
				dx = mx - Map[i][0].X;
				if (dx < 0)delt = -dx;
				if (dx < R) { clkx = i; break; }
			}
			//判斷Y
			for (j = 0;j < N;j++)
			{
				dy = my - Map[0][j].Y;
				if (dy < 0)delt = -dy;
				if (dy < R) { clky = j; break; }
			}
			if ((clkx >= 0) && (clky >= 0))//clkx,clky皆>=0
			{
				if (!Map[clkx][clky].Flag)//未消除
				{
					//距離判斷
					dist = dx * dx + dy * dy;
					dist = sqrt(dist);
					if (dist < R)
					{
						if (NowSelect == Map[clkx][clky].NUM)//點選已選中的 ->取消選取
						{
							NSX = -1;
							NSY = -1;
							NowSelect = 0;
							//繪製圓形
							Ball(hdc, Map[clkx][clky].X, Map[clkx][clky].Y, R, Map[clkx][clky].Flag?0xCCCCCC:0xB48246);
							//繪製數字
							_7Seg(hdc, Map[clkx][clky].NUM, clkx, clky);
							btn_Ctrl_4.SetWindowTextW(_T("Choose"));
						}
						else
						{
							if (NowSelect)//若有舊的選取
							{
								//清掉上個選取的
								//繪製圓形
								Ball(hdc, Map[NSX][NSY].X, Map[NSX][NSY].Y, R, Map[NSX][NSY].Flag ? 0xCCCCCC : 0xB48246);
								//繪製數字
								_7Seg(hdc, Map[NSX][NSY].NUM, NSX, NSY);
							}

							//更改新選取的部分
							NowSelect = Map[clkx][clky].NUM;
							NSX = clkx;
							NSY = clky;
							//繪製圓形
							Ball(hdc, Map[clkx][clky].X, Map[clkx][clky].Y, R, 0x808000);
							//繪製數字
							_7Seg(hdc, Map[clkx][clky].NUM, clkx, clky);
							//更改按鈕文字
							sprintf_s(S1, 100, "Choose:%d", NowSelect);
							Big5ToUniCode(S1, LW, 100);
							btn_Ctrl_4.SetWindowTextW(LW);
						}
					}
				}
			}
		}
	}
	MLB = 0;
	CDialogEx::OnLButtonUp(nFlags, point);
}

//取消選取
void CTCPCAsyncDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值

	CDialogEx::OnLButtonDblClk(nFlags, point);
}


void CTCPCAsyncDlg::OnBnClickedButton6()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	if (!Start) {NewGame();}
}
