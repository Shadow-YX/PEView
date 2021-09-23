#pragma once


// CMemDump 对话框

class CMemDump : public CDialogEx
{
	DECLARE_DYNAMIC(CMemDump)

public:
	CMemDump(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMemDump();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_DUMP };
#endif
public:
	TCHAR filePath[MAX_PATH];
	CString m_DumpPath;
	CString m_ImportPath;
	DWORD m_dwImageBase;
	UINT m_pID;
	HANDLE m_hProc;
	HANDLE m_hThread;
	HANDLE m_hFile;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedOk();
	bool GetPIDByName(TCHAR* processName);
};
