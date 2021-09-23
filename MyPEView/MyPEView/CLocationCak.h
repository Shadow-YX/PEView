#pragma once


// CLocationCak 对话框

class CLocationCak : public CDialogEx
{
	DECLARE_DYNAMIC(CLocationCak)

public:
	CLocationCak(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CLocationCak();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_CALCULATOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
