#pragma once


// CLocationCal 对话框

class CLocationCal : public CDialogEx
{
	DECLARE_DYNAMIC(CLocationCal)

public:
	CLocationCal(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CLocationCal();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_CALCULATOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	
	DECLARE_MESSAGE_MAP()
private:
	PIMAGE_DOS_HEADER pImgDosHeader = NULL;
	PIMAGE_NT_HEADERS32 pImgNtHeader = NULL;
	PIMAGE_SECTION_HEADER pImgSecHeader = NULL;
	HANDLE m_nFile = 0;
	LPVOID m_lpBase = 0;

public:

	void SetPEParameter(HANDLE File, LPVOID Base, 
		PIMAGE_DOS_HEADER DosHeader, PIMAGE_NT_HEADERS32 NtHeader,
		PIMAGE_SECTION_HEADER SecHeader);


	void HexStrToInt(char* szHex, DWORD* pdwHexVal);


	//afx_msg void OnBnClickedCalva();
	//afx_msg void OnBnClickedCalrva();

	CEdit m_edtVA;
	CEdit m_edtRVA;
	afx_msg void OnBnClickedOk();
};
