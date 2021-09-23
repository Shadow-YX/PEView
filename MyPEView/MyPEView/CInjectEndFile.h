#pragma once


// CInjectEndFile 对话框

class CInjectEndFile : public CDialogEx
{
	DECLARE_DYNAMIC(CInjectEndFile)

public:
	CInjectEndFile(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CInjectEndFile();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	int m_Base;
	int m_fileSize;
	int m_PEEtry;
	HANDLE m_File;

public:
	afx_msg void OnBnClickedOk();
	void SetAtt(HANDLE m_hFile,int m_lpBase,int PEEntryPoint,int m_nFileSize);
	void AsciiToBcd(CString cs, unsigned char* p, int nlength);
	void ChangeDwordToString(DWORD dwValue, TCHAR* szBuf);
};
