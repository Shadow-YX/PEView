#pragma once


// CITDThunk 对话框

class CITDThunk : public CDialogEx
{
	DECLARE_DYNAMIC(CITDThunk)

public:
	CITDThunk(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CITDThunk();
public:
	CString SelDllName;
	PIMAGE_NT_HEADERS32 m_pImgNtHeader = NULL;
	PIMAGE_SECTION_HEADER m_pImgSecHeader = NULL;
	LPVOID m_lpBase = 0;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_THUNK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void SetPEParameter(CString DllName,LPVOID m_lpBase, 
		PIMAGE_NT_HEADERS32 pImgNtHeader, 
		PIMAGE_SECTION_HEADER pImgSecHeader);
	CListCtrl m_lstThunk;

	DWORD RvaToFoa(PIMAGE_NT_HEADERS pImgNtHdr, DWORD dwRva);
};
