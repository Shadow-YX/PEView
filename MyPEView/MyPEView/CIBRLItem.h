#pragma once


// CIBRLItem 对话框

class CIBRLItem : public CDialogEx
{
	DECLARE_DYNAMIC(CIBRLItem)

public:
	CIBRLItem(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CIBRLItem();
public:
	CString	 csRVA;
	LPVOID m_lpBase = 0;
	PIMAGE_NT_HEADERS32 m_pImgNtHeader = NULL;
	PIMAGE_SECTION_HEADER m_pImgSecHeader = NULL;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_PBRLITEM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void SetPEParameter(CString cRVA, LPVOID m_lpBase,
		PIMAGE_NT_HEADERS32 pImgNtHeader,
		PIMAGE_SECTION_HEADER pImgSecHeader);
	DWORD RvaToFoa(PIMAGE_NT_HEADERS pImgNtHdr, DWORD dwRva);
	CListCtrl m_lstItem;
};
