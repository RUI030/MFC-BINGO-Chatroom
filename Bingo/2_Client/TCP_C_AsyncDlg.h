
// TCP_C_AsyncDlg.h: 標頭檔
//

#pragma once


// CTCPCAsyncDlg 對話方塊
class CTCPCAsyncDlg : public CDialogEx
{
// 建構
public:
	void GameWindowInit();
	void NewGame();
	void GameCMD(char STR[]);
	void EndGame();
	CTCPCAsyncDlg(CWnd* pParent = nullptr);	// 標準建構函式

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TCP_C_ASYNC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支援


// 程式碼實作
protected:
	HICON m_hIcon;

	// 產生的訊息對應函式
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	HDC hdc;
	afx_msg void OnBnClickedButton1();
	CString m_Edit1;
	CString m_Edit2;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedButton2();
	CString m_Edit_3;
	CString m_Edit4;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	CButton btn_Ctrl_4;
	CString m_Edit6;
//	CEdit m_Ctrl6;
	afx_msg void OnBnClickedButton5();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButton6();
	CEdit m_Ctrl6;
	CEdit m_Ctrl1;
};
