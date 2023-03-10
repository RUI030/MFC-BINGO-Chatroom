
// TCP_S_AsyncDlg.h: 標頭檔
//

#pragma once


// CTCPSAsyncDlg 對話方塊
class CTCPSAsyncDlg : public CDialogEx
{
// 建構
public:
	CTCPSAsyncDlg(CWnd* pParent = nullptr);	// 標準建構函式

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TCP_S_ASYNC_DIALOG };
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
	CString m_Edit1;
	afx_msg void OnBnClickedButton1();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	CEdit m_Edit11;
	CListCtrl m_List3;
	afx_msg void OnItemchangedList3(NMHDR *pNMHDR, LRESULT *pResult);
//	CEdit m_Edit3;
	CString m_Edit3;
	afx_msg void OnBnClickedButton2();
};
