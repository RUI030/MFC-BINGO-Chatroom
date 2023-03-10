
// TCP_S_AsyncDlg.cpp: 實作檔案
//

#include "stdafx.h"
#include "TCP_S_Async.h"
#include "TCP_S_AsyncDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//
#define MAX 100
SOCKET  Sock[MAX];
int     No = 0;
char	IP[MAX][20];
char	Name[MAX][100];



//Game
struct Player
{
	int		Sock;
	int		Rank;
	int		F;
	char	Name[100];
}LST[MAX];
int pn,Rank;
char TMPS[100];
int PlayerNow;


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


// CTCPSAsyncDlg 對話方塊



CTCPSAsyncDlg::CTCPSAsyncDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TCP_S_ASYNC_DIALOG, pParent)
	, m_Edit1(_T(""))
	, m_Edit3(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTCPSAsyncDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Edit1);
	DDX_Control(pDX, IDC_EDIT1, m_Edit11);
	DDX_Control(pDX, IDC_LIST3, m_List3);
	//  DDX_Control(pDX, IDC_EDIT3, m_Edit3);
	DDX_Text(pDX, IDC_EDIT3, m_Edit3);
}

BEGIN_MESSAGE_MAP(CTCPSAsyncDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CTCPSAsyncDlg::OnBnClickedButton1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST3, &CTCPSAsyncDlg::OnItemchangedList3)
	ON_BN_CLICKED(IDC_BUTTON2, &CTCPSAsyncDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CTCPSAsyncDlg 訊息處理常式

BOOL CTCPSAsyncDlg::OnInitDialog()
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
	m_List3.InsertColumn(0, L"No."); m_List3.SetColumnWidth(0, 80);
	m_List3.InsertColumn(1, L"Name"); m_List3.SetColumnWidth(1, 140);
	m_List3.InsertColumn(2, L"IP"); m_List3.SetColumnWidth(2, 220);
	m_List3.InsertColumn(3, L"Socket"); m_List3.SetColumnWidth(3, 120);
	m_List3.InsertColumn(4, L"Status"); m_List3.SetColumnWidth(4, 200);
	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

void CTCPSAsyncDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTCPSAsyncDlg::OnPaint()
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
HCURSOR CTCPSAsyncDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// #####################################################
// #####################################################
#include "TCPIP_Async.cpp"
#define  SEVENT  (WM_USER+100)
SOCKET   SSock;
void CTCPSAsyncDlg::OnBnClickedButton1()
{
	// 啟動 TCP 伺服器
	Start_TCP_Server(&SSock, 6000, SEVENT, m_hWnd);
	UpdateData(1); m_Edit1 += L"Form:[SOCKET](IP)\r\n";  UpdateData(0);//顯示訊息
}





LRESULT CTCPSAsyncDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	int      i,Len = sizeof(sockaddr);
	char     S1[2000],S2[2000];
	wchar_t  SS1[2000];
	int		 j,now;
	char	 CMDtmp[2000];
	sockaddr		Addr;
	sockaddr_in		Addr1;
	if (message == SEVENT)
	{  switch(LOWORD(lParam))
	   {  case FD_ACCEPT: // 遠端要求連線事件
		       Sock[No] = accept(wParam, (sockaddr*)&Addr1, &Len);// 接受連線
			   //抓出IP
			   sprintf_s(IP[No], sizeof(IP[No]), "%d.%d.%d.%d\0",
				   ((Addr1.sin_addr.s_addr >> 0) & 0xFF),
				   ((Addr1.sin_addr.s_addr >> 8) & 0xFF),
				   ((Addr1.sin_addr.s_addr >> 16) & 0xFF),
				   ((Addr1.sin_addr.s_addr >> 24) & 0xFF));
			   //產生訊息
			   sprintf_s(S2, 2000, "[%d](%s)遠端連入\r\n",Sock[No],IP[No]);
			   Big5ToUniCode(S2, SS1, strlen(S2) + 1);
			   UpdateData(1); m_Edit1 += SS1;  UpdateData(0);//顯示訊息
			   //更新List
			   //No
			   sprintf_s(S2, 2000, "%d", No);
			   Big5ToUniCode(S2, SS1, strlen(S2) + 1);
			   m_List3.InsertItem(No, SS1);
			   //Name
			   sprintf_s(S2, 2000, "NULL");
			   Big5ToUniCode(S2, SS1, strlen(S2) + 1);
			   m_List3.SetItemText(No, 1, SS1);
			   //IP
			   Big5ToUniCode(IP[No], SS1, strlen(IP[No]) + 1);
			   m_List3.SetItemText(No, 2, SS1);
			   //Socket
			   sprintf_s(S2, 2000, "%d", Sock[No]);
			   Big5ToUniCode(S2, SS1, strlen(S2) + 1);
			   m_List3.SetItemText(No, 3, SS1);
			   //Status
			   sprintf_s(S2, 2000, "Connected");
			   Big5ToUniCode(S2, SS1, strlen(S2) + 1);
			   m_List3.SetItemText(No, 4, SS1);
			   //
			   No++;
		       break;
	      case FD_READ:   // 遠端送來資料事件
			   // 1.接收資料
			   i = recv(wParam, S1, sizeof(S1) - 1, 0);
			   if (i > 0)
			   {   // 2.資料轉換並顯示
				   S1[i] = 0;
				   int div = 0;
				   bool tmpf;
				   char CMD[2000];
				   int cmdn = 0;//1:SetName
				   //找到對應Sock的No
				   for (j = 0;j < No;j++){if (Sock[j] == wParam) { now = j;break; }}
				   //Special Cmd
				   if (S1[0]=='<')
				   {
					   //抓出指令
					   for (int k=0;k<=strlen(S1);k++)
					   {
						   if (!div)//指令
						   {
							   CMD[k] = S1[k];
							   if (S1[k]=='>') { div = k+1; CMD[k+1] = '\0'; }
						   }
						   else
						   {
							   S2[k-div]= S1[k];
						   }
					   }
					   //判斷指令
					   strcpy_s(CMDtmp,2000,"<SetName>");
					   if (!strcmp(CMD, CMDtmp)) { cmdn = 1; }
					   strcpy_s(CMDtmp, 2000, "<G_New>");
					   if (!strcmp(CMD, CMDtmp)) { cmdn = 11; }
					   strcpy_s(CMDtmp, 2000, "<G_Join>");
					   if (!strcmp(CMD, CMDtmp)) { cmdn = 12; }
					   strcpy_s(CMDtmp, 2000, "<G_Start>");
					   if (!strcmp(CMD, CMDtmp)) { cmdn = 13; }
					   strcpy_s(CMDtmp, 2000, "<G_Del>");
					   if (!strcmp(CMD, CMDtmp)) { cmdn = 14; }
					   strcpy_s(CMDtmp, 2000, "<G_Bingo>");
					   if (!strcmp(CMD, CMDtmp)) { cmdn = 15; }
				   }
				   switch (cmdn)
				   {
				   case 1://SetName
						//抓出暱稱
						strcpy_s(Name[now],S2);
						//產生訊息
						sprintf_s(S2, 2000, "[%d](%s)設定了暱稱:%s\r\n", Sock[now], IP[now], Name[now]);
						Big5ToUniCode(S2, SS1, strlen(S2) + 1);
						UpdateData(1); m_Edit1 += SS1;  UpdateData(0);
						//Auto Scroll
						m_Edit11.LineScroll(m_Edit11.GetLineCount());
						//List 更新
						//Name
						Big5ToUniCode(Name[now], SS1, strlen(Name[now]) + 1);
						m_List3.SetItemText(now, 1, SS1);
						break;
				   case 11://New
					   //初始化玩家個數
					   pn = 0;
					   Rank = 1;
					   //紀錄Host Info
					   LST[pn].Sock = Sock[now];
					   LST[pn].Rank = 0;
					   LST[pn].F = 0;
					   if (strcmp(Name[now], "")) sprintf_s(LST[pn].Name, 100, Name[now]);
					   else sprintf_s(LST[pn].Name, 100, "Host");
					   pn++;
					   //送給其他人
					   sprintf_s(S2, 2000, "<G_New>\r\n");
					   Big5ToUniCode(S2, SS1, strlen(S2) + 1);
					   UpdateData(1); m_Edit1 += SS1;  UpdateData(0);
					   //Auto Scroll
					   m_Edit11.LineScroll(m_Edit11.GetLineCount());
					   for (int i = 0; i < No; i++)
					   {
						   if (Sock[now] == Sock[i])continue;
						   send(Sock[i], S2, strlen(S2), 0);
					   }
					   
					   sprintf_s(S2, 2000, "<G_Msg>%s:發起了遊戲\r\n\0", LST[0].Name);
					   Big5ToUniCode(S2, SS1, strlen(S2) + 1);
					   UpdateData(1); m_Edit1 += SS1;  UpdateData(0);
					   //Auto Scroll
					   m_Edit11.LineScroll(m_Edit11.GetLineCount());
					   for (int i = 0; i < No; i++)
					   {
						   send(Sock[i], S2, strlen(S2), 0);
					   }
					   break;
				   case 12://Join
					   LST[pn].Sock = Sock[now];
					   LST[pn].Rank = 0;
					   LST[pn].F = 0;
					   sprintf_s(LST[pn].Name, 100, Name[now]);
					   if (strcmp(Name[now], "")) sprintf_s(LST[pn].Name, 100, Name[now]);
					   else sprintf_s(LST[pn].Name, 100, "Player %d", pn);
					   //送給其他人
					   sprintf_s(S2, 2000, "<G_Msg>%s:加入了遊戲\r\n\0", LST[pn].Name);
					   Big5ToUniCode(S2, SS1, strlen(S2) + 1);
					   UpdateData(1); m_Edit1 += SS1;  UpdateData(0);
					   //Auto Scroll
					   m_Edit11.LineScroll(m_Edit11.GetLineCount());
					   for (int i = 0; i < No; i++)
					   {
						   send(Sock[i], S2, strlen(S2), 0);
					   }
					   pn++;
					   break;
				   case 13://Start
					   //送給ALL
					   PlayerNow = 0;
					   LST[PlayerNow].F = 1;
					   sprintf_s(S2, 2000, "<G_Start>\r\n", LST[pn].Name);
					   Big5ToUniCode(S2, SS1, strlen(S2) + 1);
					   UpdateData(1); m_Edit1 += SS1;  UpdateData(0);
					   //Auto Scroll
					   m_Edit11.LineScroll(m_Edit11.GetLineCount());
					   for (int i = 0; i < No; i++)
					   {
						   send(Sock[i], S2, strlen(S2), 0);
					   }

					   sprintf_s(S2, 2000, "<G_Msg>[系統]:遊戲開始\r\n\0");
					   Big5ToUniCode(S2, SS1, strlen(S2) + 1);
					   UpdateData(1); m_Edit1 += SS1; UpdateData(0);
					   //Auto Scroll
					   m_Edit11.LineScroll(m_Edit11.GetLineCount());
					   for (int i = 0; i < No; i++)
					   {
						   send(Sock[i], S2, strlen(S2), 0);
					   }
					   
					   sprintf_s(S2, 2000, "<G_Msg>[系統]:請%s指定數字\r\n\0", LST[PlayerNow].Name);
					   Big5ToUniCode(S2, SS1, strlen(S2) + 1);
					   UpdateData(1); m_Edit1 += SS1; UpdateData(0);
					   //Auto Scroll
					   m_Edit11.LineScroll(m_Edit11.GetLineCount());
					   for (int i = 0; i < No; i++)
					   {
						   send(Sock[i], S2, strlen(S2), 0);
					   }
					   break;
				   case 14://Del
					   //找送數字的人
					   int msgnow;
					   msgnow = -1;
					   for (i = 0;i < pn;i++)
					   {
						   if (LST[i].Sock==Sock[now])
						   {
							   msgnow = i;
							   break;
						   }
					   }
					   //若輪到送數字的人才進行操作
					   if (LST[msgnow].F)
					   {
						   int numtmp;
						   numtmp = atoi(S2);
						   //所有人Del
						   sprintf_s(S2, 2000, "<G_Del>%d\r\n\0", numtmp);
						   Big5ToUniCode(S2, SS1, strlen(S2) + 1);
						   UpdateData(1); m_Edit1 += SS1; UpdateData(0);
						   //Auto Scroll
						   m_Edit11.LineScroll(m_Edit11.GetLineCount());
						   for (int i = 0; i < No; i++)
						   {
							   send(Sock[i], S2, strlen(S2), 0);
						   }
						   sprintf_s(S2, 2000, "<G_Msg>%s:指定數字%d\r\n\0", LST[PlayerNow].Name, numtmp);
						   Big5ToUniCode(S2, SS1, strlen(S2) + 1);
						   UpdateData(1); m_Edit1 += SS1; UpdateData(0);
						   //Auto Scroll
						   m_Edit11.LineScroll(m_Edit11.GetLineCount());
						   for (int i = 0; i < No; i++)
						   {
							   send(Sock[i], S2, strlen(S2), 0);
						   }
						   LST[PlayerNow].F = 0;
						   //下一位
						   PlayerNow++;
						   PlayerNow %= pn;
						   LST[PlayerNow].F = 1;
						   sprintf_s(S2, 2000, "<G_Msg>[系統]:請%s指定\r\n\0", LST[PlayerNow].Name );
						   Big5ToUniCode(S2, SS1, strlen(S2) + 1);
						   UpdateData(1); m_Edit1 += SS1; UpdateData(0);
						   //Auto Scroll
						   m_Edit11.LineScroll(m_Edit11.GetLineCount());
						   for (int i = 0; i < No; i++)
						   {
							   send(Sock[i], S2, strlen(S2), 0);
						   }
					   }
					   break;
				   case 15://Bingo
					   //根據Sock找人
					   int i;
					   for (i = 0;i < pn;i++)
					   {
						   if (LST[i].Sock == Sock[now])break;
					   }
					   LST[i].Rank = Rank;
					   Rank++;

					   sprintf_s(S2, 2000, "<G_Msg>%s:Bingo!(Rank:%d)\r\n\0", LST[i].Name, LST[i].Rank);
					   Big5ToUniCode(S2, SS1, strlen(S2) + 1);
					   UpdateData(1); m_Edit1 += SS1; UpdateData(0);
					   //Auto Scroll
					   m_Edit11.LineScroll(m_Edit11.GetLineCount());
					   for (int j = 0; j < No; j++)
					   {
						   send(Sock[i], S2, strlen(S2), 0);
					   }
					   
					   sprintf_s(S2, 2000, "<G_End>\r\n");
					   Big5ToUniCode(S2, SS1, strlen(S2) + 1);
					   UpdateData(1); m_Edit1 += SS1; UpdateData(0);
					   //Auto Scroll
					   m_Edit11.LineScroll(m_Edit11.GetLineCount());
					   //EndGame
					   if (Rank == pn)
					   {
						   for (int j = 0; j < No; j++)
						   {
							   send(Sock[i], S2, strlen(S2), 0);
						   }
					   }
					   break;
				   default:
						//產生訊息
						//有暱稱
						if (strcmp(Name[now], "")) { sprintf_s(S2, 2000, "%s:%s\r\n", Name[now], S1); }
						//無暱稱
						else { sprintf_s(S2, 2000, "[%d](%s):%s\r\n", Sock[now], IP[now], S1); }
						Big5ToUniCode(S2, SS1, strlen(S2) + 1);
						UpdateData(1); m_Edit1 += SS1;  UpdateData(0);
						//Auto Scroll
						m_Edit11.LineScroll(m_Edit11.GetLineCount());
						// 3.資料轉傳給所有人
						for (i = 0; i < No; i++)
						{
							if (Sock[now] == Sock[i])continue;
							send(Sock[i], S2, strlen(S2), 0);
						}
						//List 更新
						//Status
						Big5ToUniCode(S1, SS1, strlen(S1) + 1);
						m_List3.SetItemText(now, 4, SS1);
						break;
				   }
			   }
			   //更新List

		       break;
	      case FD_CLOSE:  // 遠端斷線事件
			   closesocket(wParam);
			   //找到對應Sock的No
			   for (j = 0;j < No;j++) { if (Sock[j] == wParam) { now = j;break; } }
			   //產生訊息
			   sprintf_s(S2, 2000, "[%d](%s)遠端斷線\r\n", Sock[now], IP[now], S1);
			   Big5ToUniCode(S2, SS1, strlen(S2) + 1);
			   UpdateData(1); m_Edit1 += SS1;  UpdateData(0);//顯示訊息
			   //更新List
			   //Status
			   sprintf_s(S2, 2000, "Disconnected");
			   Big5ToUniCode(S2, SS1, strlen(S2) + 1);
			   m_List3.SetItemText(now, 4, SS1);
		       break;
	   }

	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}


void CTCPSAsyncDlg::OnItemchangedList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此加入控制項告知處理常式程式碼
	*pResult = 0;
}


void CTCPSAsyncDlg::OnBnClickedButton2()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	int      mLen, saLen = sizeof(sockaddr);
	char     S1[2000];
	wchar_t  *S11;

	// 2.擷取輸入框資訊
	UpdateData(1); S11 = (wchar_t *)m_Edit3.GetBuffer();  UpdateData(0);

	// 3.轉換
	UniCodeToBig5(S11, S1, sizeof(S1) - 2);
	// 4.送出
	for (int i = 0; i < No; i++)
	{
		send(Sock[i], S1, strlen(S1), 0);
	}

}
