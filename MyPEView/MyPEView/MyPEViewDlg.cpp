
// MyPEViewDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MyPEView.h"
#include "MyPEViewDlg.h"
#include "afxdialogex.h"
#define  GET_NAME(x) #x

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMyPEViewDlg 对话框



CMyPEViewDlg::CMyPEViewDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_PE, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON2);
}

void CMyPEViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, LST_PE, m_lstPE);
	DDX_Control(pDX, TREE_PE, m_treePE);
}

BEGIN_MESSAGE_MAP(CMyPEViewDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(MENU_OPEN, &CMyPEViewDlg::OnOpen)
	ON_COMMAND(MENU_CLOSE, &CMyPEViewDlg::OnClose)
	ON_COMMAND(MENU_EXIT, &CMyPEViewDlg::OnExit)
	ON_WM_DROPFILES()
	ON_NOTIFY(NM_DBLCLK, TREE_PE, OnNMClickTree)
	ON_COMMAND(MENU_INJ_MODENTRY, &CMyPEViewDlg::OnInjectModEntry)
	ON_COMMAND(MENU_ADD_SECTION, &CMyPEViewDlg::OnAddSection)
	ON_COMMAND(MENU_CALVIRTUAL, &CMyPEViewDlg::OnCalVirtual)
	ON_COMMAND(MENU_DUMP, &CMyPEViewDlg::OnMemDump)
	ON_NOTIFY(NM_DBLCLK, LST_PE, &CMyPEViewDlg::OnDblclkLstPe)
END_MESSAGE_MAP()


// CMyPEViewDlg 消息处理程序

BOOL CMyPEViewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	ModifyStyleEx(0, WS_EX_ACCEPTFILES, 0);
	LoadSafeSkin();
	m_Menu.LoadMenuA(IDR_MENU);
	SetMenu(&m_Menu);
	// TODO: 在此添加额外的初始化代码

	m_lstPE.InsertColumn(0, "保留", LVCFMT_CENTER, 0);
	m_lstPE.InsertColumn(1, ("pFile"), LVCFMT_CENTER, 120);
	m_lstPE.InsertColumn(2, ("RwaData"), LVCFMT_CENTER, 550);
	m_lstPE.InsertColumn(3, ("Value"), LVCFMT_CENTER, 180);
	m_lstPE.DeleteColumn(0);

	//设置风格
	LONG lStyle;
	lStyle = GetWindowLong(m_lstPE.m_hWnd, GWL_STYLE);//获取当前窗口style
	DWORD dwStyle = m_lstPE.GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP;
	m_lstPE.SetExtendedStyle(dwStyle);
	//获取选中项
	int nIdx = m_lstPE.GetSelectionMark();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMyPEViewDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMyPEViewDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMyPEViewDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMyPEViewDlg::OnOpen()
{
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("exe File(*.exe)|*.exe|All File (*.*)|*.*||"), this);
	CString  str_path;
	if (dlg.DoModal() == IDOK)
	{
		//获取文件绝对路径
		str_path = dlg.GetPathName();
	}

	m_strPath = str_path;
	OpenPEFile();
}


void CMyPEViewDlg::OnClose()
{
	if (!m_hFile)
	{
		CloseHandle(m_hFile);
	}
	if (!m_hMap)
	{
		CloseHandle(m_hFile);
	}
	if (!m_lpBase)
	{
		UnmapViewOfFile(m_lpBase);
	}
	m_treePE.DeleteAllItems();
	DelListAll();
}


void CMyPEViewDlg::OnExit()
{
	OnClose();
	exit(0);
}

BOOL CMyPEViewDlg::isPE()
{
	pImgDosHeader = (PIMAGE_DOS_HEADER)m_lpBase;

	if (pImgDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return FALSE;
	}

	pImgNtHeader = (PIMAGE_NT_HEADERS)((DWORD)m_lpBase + (DWORD)pImgDosHeader->e_lfanew);

	//程序入口偏移
	PEEntryPoint = (int)((PIMAGE_DOS_HEADER)m_lpBase)->e_lfanew;


	if (pImgNtHeader->Signature != IMAGE_NT_SIGNATURE)
	{
		return FALSE;
	}

	pImgSecHeader = IMAGE_FIRST_SECTION(pImgNtHeader);



	return TRUE;
}

void CMyPEViewDlg::ShowPEView(int Startaddr, int EndAddr, int interval)
{
	DelListAll();
	m_lstPE.InsertColumn(0, "保留", LVCFMT_CENTER, 0);
	m_lstPE.InsertColumn(1, ("pFile"), LVCFMT_CENTER, 120);
	m_lstPE.InsertColumn(2, ("RwaData"), LVCFMT_CENTER, 550);
	m_lstPE.InsertColumn(3, ("Value"), LVCFMT_CENTER, 180);
	m_lstPE.DeleteColumn(0);

	//设置风格
	LONG lStyle;
	lStyle = GetWindowLong(m_lstPE.m_hWnd, GWL_STYLE);//获取当前窗口style
	DWORD dwStyle = m_lstPE.GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	m_lstPE.SetExtendedStyle(dwStyle);
	int nItem = 0;
	//offset
	for (int i = Startaddr; i < EndAddr; i += interval)
	{
		CString strIndexHex;
		strIndexHex.Format("%08x", i);
		m_lstPE.InsertItem(nItem, strIndexHex);//offset

		TCHAR szbuff[16] = { 0 };
		TCHAR szDataMsg[128] = { 0 };
		TCHAR szAsciiMsg[128] = { 0 };
		memcpy_s(szbuff, 16, (char*)m_lpBase + i, 16);
		for (int j = 0; j < interval; j++)
		{
			sprintf(szDataMsg, "%s %02x", szDataMsg, (unsigned char)szbuff[j]);		//Hex  data
			sprintf(szAsciiMsg, "%s%c", szAsciiMsg, ('!' < szbuff[j] && szbuff[j] <= '~') ? szbuff[j] : '.');   		//ASCII
		}
		CString strDateHex;
		CString strAsciiHex;
		strDateHex.Format("%s", szDataMsg);
		strAsciiHex.Format("%s", szAsciiMsg);
		m_lstPE.SetItemText(nItem, 1, strDateHex);
		m_lstPE.SetItemText(nItem, 2, strAsciiHex);
		nItem++;
	}

	return;
}


void CMyPEViewDlg::OpenPEFile()
{
	//如果已经打开了文件先关闭文件
	OnClose();

	//打开文件
	m_hFile = CreateFile(
		m_strPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	//创建文件内存映射内核对象
	m_hMap = CreateFileMapping(m_hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
	//把文件映射到内存

	m_lpBase = (char*)MapViewOfFile(m_hMap, FILE_MAP_WRITE | FILE_MAP_READ, 0, 0, 0);

	m_nFileSize = GetFileSize(m_hFile, NULL); //大小

	ShowPEView(0, m_nFileSize,16);
	ShowTreelist();

	return;
}

void CMyPEViewDlg::ShowTreelist()
{
	int index = m_strPath.ReverseFind('\\');
	m_hFileName=m_strPath.Mid( index+1);

	HTREEITEM hExe= m_treePE.InsertItem(m_hFileName); // 根
	m_treePE.ModifyStyle(0, TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES);  //设置风格

	if (!isPE())
	{	
		return;
	}
	else
	{
		// 获取当前选中节点的句柄   
		//HTREEITEM hItem = m_treePE.GetSelectedItem();
		HTREEITEM  hDosHeader = m_treePE.InsertItem("IMAGE_DOS_HEADER", hExe);
		HTREEITEM  hDosStub = m_treePE.InsertItem("MS_DOS Stub Program", hExe);
		HTREEITEM  hNH = m_treePE.InsertItem("IMAGE_NT_HEADER", hExe);
		HTREEITEM  hImgHeader_S = m_treePE.InsertItem("SIGNATUREL", hNH);
		HTREEITEM  hImgHeader_IFH = m_treePE.InsertItem("IMAGE_FILE_HEADER", hNH);
		HTREEITEM  hImgHeader_IOH = m_treePE.InsertItem("IMAGE_OPTIONAL_HEADER", hNH);
		HTREEITEM  hImgHeader_TABLE = m_treePE.InsertItem("TABLE", hNH);
		HTREEITEM  hImgSectionHeader_code = m_treePE.InsertItem("IMAGE_SECTION_HEADER", hExe);
		HTREEITEM  hImgImportTable= m_treePE.InsertItem("IMPORT_TABLE", hExe);
		/*
		判断是否存在导出表
		*/
		if ((pImgNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size) != 0)
		{
			//解析导出表
			HTREEITEM  hImgExportTable = m_treePE.InsertItem("EXPORT_TABLE", hExe);
			HTREEITEM  hImgIETFunction= m_treePE.InsertItem("IET_FUNCTION", hImgExportTable);
		}
		//判断是否存在重定位表
		if ((pImgNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size) != 0)
		{
			//解析导出表
			HTREEITEM  hImgExportTable = m_treePE.InsertItem("RELOCATION_DIRECTORY_TABLE", hExe);
		}
		//判断是否存在DEbug表
		if ((pImgNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].Size) != 0)
		{
			//解析Debug表
			HTREEITEM  hImgExportTable = m_treePE.InsertItem("DEBUG_DIRECTOR_TABLE", hExe);
		}
		//判断是否存在TLS表
		if ((pImgNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].Size) != 0)
		{
			//解析TLS
			HTREEITEM  hImgExportTable = m_treePE.InsertItem("TLS_DIRECTORY_TABLE", hExe);
		}

		//判断是否存在资源表
		if ((pImgNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].Size) != 0)
		{
			//解析资源表
			HTREEITEM  hImgExportTable = m_treePE.InsertItem("RESOURCE_DIRECTORY_TABLE", hExe);

			//DFS解析资源表三层结构
			IMAGE_RESOURCE_DIRECTORY* IPRD = (IMAGE_RESOURCE_DIRECTORY*)((DWORD)m_lpBase + RvaToFoa(pImgNtHeader,
				pImgNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress));

			if (IPRD->NumberOfIdEntries + IPRD->NumberOfNamedEntries != 0)
			{
				//递归显示资源树
				ShowResTree_DFS(0, IPRD , IPRD,hImgExportTable);
			}
		}
		
	}
}

VOID CMyPEViewDlg::ShowResTree_DFS(DWORD dwDeep, IMAGE_RESOURCE_DIRECTORY* pNewIRDE,
	IMAGE_RESOURCE_DIRECTORY* pOriIRDE ,HTREEITEM  hImgTree)
{
	/*
	一级子目录按照资源类型分类。
	二级子目录按照资源ID分类。
	三级子目录按照资源代码页分类。
	三级子目录后即为节点，也就是所说的文件。这里的文件其实就是包含了资源数据的指针和大小等信息的一个数据结构而已。
	对于所有资源数据块的访问均可以从这里开始。
	*/
	CString csFirstName;
	CString csFirstID;
	HTREEITEM  hImgRes;
	dwDeep++;
	IMAGE_RESOURCE_DIRECTORY_ENTRY* pResourceBaseAddress = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)((PBYTE)
		pNewIRDE + sizeof(IMAGE_RESOURCE_DIRECTORY));

	for (int i = 0; i < pNewIRDE->NumberOfIdEntries + pNewIRDE->NumberOfNamedEntries; i++)
	{
		//每次都增加上一个sizeof(结构体)的偏移
		IMAGE_RESOURCE_DIRECTORY_ENTRY* pNowResourceAddress = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)
			((DWORD)pResourceBaseAddress + i * sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY));

		if (pNowResourceAddress->NameIsString) //名称显示
		{
			IMAGE_RESOURCE_DIR_STRING_U* wsName = (IMAGE_RESOURCE_DIR_STRING_U*)
				((DWORD)pNowResourceAddress->NameOffset + (DWORD)pOriIRDE);
			
			csFirstName.Format("%s", wsName);
			hImgRes= m_treePE.InsertItem(csFirstName, hImgTree);
		}
		else {
			if (dwDeep == 1) //深度为1时，ID号显示资源名称
			{
				hImgRes = m_treePE.InsertItem(mapResType[pNowResourceAddress->Id], hImgTree);
			}
			else
			{
				csFirstName.Format("%d", pNowResourceAddress->Id);
				hImgRes = m_treePE.InsertItem(csFirstName, hImgTree);
			}

		}
		//递归直到DataIsDirectory值为0
		if (pNowResourceAddress->DataIsDirectory) {
			ShowResTree_DFS(dwDeep, (IMAGE_RESOURCE_DIRECTORY*)((DWORD)
				pNowResourceAddress->OffsetToDirectory + (DWORD)pOriIRDE),
				pOriIRDE, hImgRes);
		}
	}
	return;
}


void CMyPEViewDlg::OnNMClickTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	HTREEITEM hItem = m_treePE.GetSelectedItem();
	HTREEITEM hParent = m_treePE.GetParentItem(hItem);
	HTREEITEM hGrandParent = m_treePE.GetParentItem(hParent);

	CString strTree = m_treePE.GetItemText(hItem);
	CString strTreeParent = m_treePE.GetItemText(hParent);
	CString strTreeGrandParent = m_treePE.GetItemText(hGrandParent);

	if (strTree == m_hFileName)
	{
		int nFileSize = GetFileSize(m_hFile, NULL); //大小
		ShowPEView(0, nFileSize, 16);
		return;
	}
	if (strTree == "IMAGE_DOS_HEADER")
	{
		ShowListIDH();
		return;
	}
	if (strTree == "MS_DOS Stub Program")
	{
		PIMAGE_DOS_HEADER pImgDosHdr = NULL;
		ShowPEView(0x40, PEEntryPoint, 16);
		return;
	}
	if (strTree == "IMAGE_NT_HEADER")
	{
		ShowPEView((int)((PIMAGE_DOS_HEADER)m_lpBase)->e_lfanew, 0x1F0, 16);
		return;
	}
	if (strTree == "SIGNATUREL")
	{
		ShowListSignature();
		return;
	}
	if (strTree == "IMAGE_FILE_HEADER")
	{
		ShowListIFH();
		return;
	}
	if (strTree == "TABLE")
	{
		ShowListTable();
		return;
	}
	if (strTree == "IMAGE_OPTIONAL_HEADER")
	{
		ShowListIOH();
		return;
	}
	if (strTree == "IMAGE_SECTION_HEADER")
	{
		ShowImgListSection();
		return;
	}
	if (strTree == "IMPORT_TABLE")
	{
		ShowImgImportTableList();
		return;
	}
	if (strTree == "EXPORT_TABLE")
	{
		ShowImgExportTable();
		return;
	}
	if (strTree == "IET_FUNCTION")
	{
		ShowImgIETFunction();
		return;
	}
	if (strTree == "RELOCATION_DIRECTORY_TABLE")
	{
		ShowRelocationDirectory();
		return;
	}
	if (strTree == "RESOURCE_DIRECTORY_TABLE")
	{
		ShowResourceDirectory();
		return;
	}
	if (strTree == "DEBUG_DIRECTOR_TABLE")
	{
		ShowDebugDirectory();
		return;
	}
	if (strTree == "TLS_DIRECTORY_TABLE")
	{
		ShowTLSDirectory();
		return;
	}
	/*	
	递归显示资源表数据, 三层结构:
	1.strTreeGrandParen
	2.strTreeParent
	3.strTree
	*/
	for (int i = 0; i < 25; i++)
	{
		if (strTreeGrandParent == mapResType[i])
		{
			ShowResource( i,atoi(strTreeParent));
		}
	}
}

void CMyPEViewDlg::DelListAll()
{
	CHeaderCtrl* pHeaderCtrl = m_lstPE.GetHeaderCtrl();

	if (pHeaderCtrl != NULL)
	{
		int nColumnCount = pHeaderCtrl->GetItemCount();
		for (int i = 0; i < nColumnCount; i++)
		{
			m_lstPE.DeleteColumn(0);
		}
	}
	m_lstPE.DeleteAllItems();
}

char* CMyPEViewDlg::HexDump(char* buf, int len, int addr)
{
	int i, j, k;
	char* binstr = (char*)malloc(16);

	for (i = 0; i < len; i++) {
		if (0 == (i % 16)) {
			sprintf(binstr, "%08x -", i + addr);
			sprintf(binstr, "%s %02x", binstr, (unsigned char)buf[i]);
		}
		else if (15 == (i % 16)) {
			sprintf(binstr, "%s %02x", binstr, (unsigned char)buf[i]);
			sprintf(binstr, "%s  ", binstr);
			for (j = i - 15; j <= i; j++) {
				sprintf(binstr, "%s%c", binstr, ('!' < buf[j] && buf[j] <= '~') ? buf[j] : '.');
			}
			return binstr;
		}
		else {
			sprintf(binstr, "%s %02x", binstr, (unsigned char)buf[i]);
		}
	}
	if (0 != (i % 16)) {
		k = 16 - (i % 16);
		for (j = 0; j < k; j++) {
			sprintf(binstr, "%s   ", binstr);
		}
		sprintf(binstr, "%s  ", binstr);
		k = 16 - k;
		for (j = i - k; j < i; j++) {
			sprintf(binstr, "%s%c", binstr, ('!' < buf[j] && buf[j] <= '~') ? buf[j] : '.');
		}
		return binstr;
	}

	return 0;
}

void CMyPEViewDlg::ShowListIDH()
{
	UpdataListDosHeader();
	for (int i = 0; i <0x40; i += 2)
	{
		CString strIndexHex;
		strIndexHex.Format("%08x", i);
		m_lstPE.InsertItem(i/2, strIndexHex);//offset

		TCHAR szbuff[4] = { 0 };
		TCHAR szDataMsg[32] = { 0 };
		TCHAR szAsciiMsg[32] = { 0 };

		if (mapIMHdes[i / 2].Type == "WORD")
		{
			memcpy_s(szbuff, 2, (char*)m_lpBase + i, 2);
			for (int j = 1; j >=0; j--)
			{
				sprintf(szDataMsg, "%s%02x", szDataMsg, (unsigned char)szbuff[j]);		//Hex  data
			}
			for (int j = 0; j < 2; j++)
			{
				sprintf(szAsciiMsg, "%s%c", szAsciiMsg, ('!' < szbuff[j] && szbuff[j] <= '~') ? szbuff[j] : ' ');   		//ASCII
			}
			CString strDateHex;
			CString strAsciiHex;
			strDateHex.Format("%s", szDataMsg);
			strAsciiHex.Format("%s", szAsciiMsg);
			m_lstPE.SetItemText(i / 2, 1, mapIMHdes[i / 2].Type);
			m_lstPE.SetItemText(i / 2, 2, mapIMHdes[i / 2].Header);
			m_lstPE.SetItemText(i / 2, 3, strDateHex);
			m_lstPE.SetItemText(i / 2, 4, mapIMHdes[i / 2].Description);
			m_lstPE.SetItemText(i / 2, 5, strAsciiHex);
		}
		else if (mapIMHdes[i / 2].Type == "LONG")
		{
			memcpy_s(szbuff, 4, (char*)m_lpBase + i, 4);
			for (int j = 3; j >= 0; j--)
			{
				sprintf(szDataMsg, "%s%02x", szDataMsg, (unsigned char)szbuff[j]);		//Hex  data
			}
			for (int j = 0; j < 4; j++)
			{
				sprintf(szAsciiMsg, "%s%c", szAsciiMsg, ('!' < szbuff[j] && szbuff[j] <= '~') ? szbuff[j] : ' ');   		//ASCII
			}
			CString strDateHex;
			CString strAsciiHex;
			strDateHex.Format("%s", szDataMsg);
			strAsciiHex.Format("%s", szAsciiMsg);
			m_lstPE.SetItemText(i / 2, 1, mapIMHdes[i / 2].Type);
			m_lstPE.SetItemText(i / 2, 2, mapIMHdes[i / 2].Header);
			m_lstPE.SetItemText(i / 2, 3, strDateHex);
			m_lstPE.SetItemText(i / 2, 4, mapIMHdes[i / 2].Description);
			m_lstPE.SetItemText(i / 2, 5, strAsciiHex);
			i += 2;
		}
	}

}
void CMyPEViewDlg::ShowListSignature()
{
	UpdataListDosHeader();
	//IMAGE_NT_SIGNATURE
	PIMAGE_DOS_HEADER pImgDosHdr = (PIMAGE_DOS_HEADER)m_lpBase;
	CString strIndexHex;
	strIndexHex.Format("%08x", (int)pImgDosHdr->e_lfanew);
	m_lstPE.InsertItem(1, strIndexHex);//offset
	m_lstPE.SetItemText(0, 1, "WORD");

	TCHAR szbuff[4] = { 0 };
	TCHAR szDataMsg[32] = { 0 };
	memcpy_s(szbuff, 2, (char*)m_lpBase + (int)pImgDosHdr->e_lfanew, 2);
	for (int j = 3; j >= 0; j--)
	{
		sprintf(szDataMsg, "%s%02x", szDataMsg, (unsigned char)szbuff[j]);		//Hex  data
	}
	CString strDateHex;
	strDateHex.Format("%s", szDataMsg);
	m_lstPE.SetItemText(0, 2, "Signature");
	m_lstPE.SetItemText(0, 3, strDateHex);
	m_lstPE.SetItemText(0, 4, "Signature");
	m_lstPE.SetItemText(0, 5,"PIMAGE_NT_HEADERS PE");
}

void CMyPEViewDlg::ShowListIFH()
{
	UpdataListNTHeader();

	int StartIFHOffset = PEEntryPoint+4;
	int EndIFHOffset = PEEntryPoint +24;
	int nItem = 0;

	for (int i = StartIFHOffset; i < EndIFHOffset ; i += 2)
	{

		CString strIndexHex;
		strIndexHex.Format("%08x", i);
		m_lstPE.InsertItem(nItem, strIndexHex);//offset

		TCHAR szbuff[4] = { 0 };
		TCHAR szDataMsg[32] = { 0 };
		TCHAR szAsciiMsg[32] = { 0 };

		if (mapIFH[nItem].Type == "WORD")
		{
			memcpy_s(szbuff, 2, (char*)m_lpBase + i, 2);
			for (int j = 1; j >= 0; j--)
			{
				sprintf(szDataMsg, "%s%02x", szDataMsg, (unsigned char)szbuff[j]);		//Hex  data
			}
			CString strDateHex;
			CString strAsciiHex;
			strDateHex.Format("%s", szDataMsg);
			strAsciiHex.Format("%s", szAsciiMsg);
			m_lstPE.SetItemText(nItem, 1, mapIFH[nItem].Type);
			m_lstPE.SetItemText(nItem, 2, mapIFH[nItem].Header);
			m_lstPE.SetItemText(nItem, 3, strDateHex);
		}
		else if (mapIFH[nItem].Type == "DWORD")
		{
			memcpy_s(szbuff, 4, (char*)m_lpBase + i, 4);
			for (int j = 3; j >= 0; j--)
			{
				sprintf(szDataMsg, "%s%02x", szDataMsg, (unsigned char)szbuff[j]);		//Hex  data
			}
			CString strDateHex;
			CString strAsciiHex;
			strDateHex.Format("%s", szDataMsg);
			strAsciiHex.Format("%s", szAsciiMsg);
			m_lstPE.SetItemText(nItem, 1, mapIFH[nItem].Type);
			m_lstPE.SetItemText(nItem, 2, mapIFH[nItem].Header);
			m_lstPE.SetItemText(nItem, 3, strDateHex);
			i += 2;
		}
		nItem++;
	}

}

void CMyPEViewDlg::ShowListIOH()
{
	UpdataListNTHeader();

	int StartIOHOffset = PEEntryPoint + 24;
	int EndIOHOffset = PEEntryPoint + 120;
	int nItem = 0;

	for (int i = StartIOHOffset; i < EndIOHOffset; i ++)
	{

		CString strIndexHex;
		strIndexHex.Format("%08x", i);
		m_lstPE.InsertItem(nItem, strIndexHex);//offset

		TCHAR szbuff[4] = { 0 };
		TCHAR szDataMsg[32] = { 0 };
		TCHAR szAsciiMsg[32] = { 0 };

		if (mapIOH[nItem].Type == "BYTE")
		{
			memcpy_s(szbuff, 1, (char*)m_lpBase + i, 1);

			sprintf(szDataMsg, "%s%02x", szDataMsg, (unsigned char)szbuff[0]);		//Hex  data	
			CString strDateHex;
			strDateHex.Format("%s", szDataMsg);

			m_lstPE.SetItemText(nItem, 1, mapIOH[nItem].Type);
			m_lstPE.SetItemText(nItem, 2, mapIOH[nItem].Header);
			m_lstPE.SetItemText(nItem, 3, strDateHex);
		}
		if (mapIOH[nItem].Type == "WORD")
		{
			memcpy_s(szbuff, 2, (char*)m_lpBase + i, 2);
			for (int j = 1; j >= 0; j--)
			{
				sprintf(szDataMsg, "%s%02x", szDataMsg, (unsigned char)szbuff[j]);		//Hex  data
			}
			CString strDateHex;
			CString strAsciiHex;
			strDateHex.Format("%s", szDataMsg);
			strAsciiHex.Format("%s", szAsciiMsg);
			m_lstPE.SetItemText(nItem, 1, mapIOH[nItem].Type);
			m_lstPE.SetItemText(nItem, 2, mapIOH[nItem].Header);
			m_lstPE.SetItemText(nItem, 3, strDateHex);
			i += 1;
		}
		else if (mapIOH[nItem].Type == "DWORD")
		{
			memcpy_s(szbuff, 4, (char*)m_lpBase + i, 4);
			for (int j = 3; j >= 0; j--)
			{
				sprintf(szDataMsg, "%s%02x", szDataMsg, (unsigned char)szbuff[j]);		//Hex  data
			}
			CString strDateHex;
			CString strAsciiHex;
			strDateHex.Format("%s", szDataMsg);
			strAsciiHex.Format("%s", szAsciiMsg);
			m_lstPE.SetItemText(nItem, 1, mapIOH[nItem].Type);
			m_lstPE.SetItemText(nItem, 2, mapIOH[nItem].Header);
			m_lstPE.SetItemText(nItem, 3, strDateHex);
			i += 3;
		}
		nItem++;
	}
}

void CMyPEViewDlg::ShowListTable()
{
	DelListAll();

	m_lstPE.InsertColumn(0, "保留", LVCFMT_CENTER, 0);
	m_lstPE.InsertColumn(1, "pFile", LVCFMT_CENTER, 120);
	m_lstPE.InsertColumn(2, "Member", LVCFMT_CENTER, 240);
	m_lstPE.InsertColumn(3, "Type", LVCFMT_CENTER, 120);
	m_lstPE.InsertColumn(4, "Data", LVCFMT_CENTER, 120);
	m_lstPE.InsertColumn(5, "Section", LVCFMT_CENTER, 120);
	m_lstPE.DeleteColumn(0);

	//设置风格
	LONG lStyle;
	lStyle = GetWindowLong(m_lstPE.m_hWnd, GWL_STYLE);//获取当前窗口style
	DWORD dwStyle = m_lstPE.GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP;
	m_lstPE.SetExtendedStyle(dwStyle);
	/*
	IMAGE_DATA_DIRECTORY pImgNtHeaderDD =(IMAGE_DATA_DIRECTORY((DWORD)m_lpBase + (pImgNtHeader->
		OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress)));
		*/
	int nItem =0 ;
	int nIndexSize = 0;
	int nIndexRVA = 0;

	for (int i = PEEntryPoint + 120; i < PEEntryPoint + 236; i += 4)
	{
		//RVA:
		CString strIndexHex;
		strIndexHex.Format("%08x", i);
		m_lstPE.InsertItem(nItem, strIndexHex);//offset

		m_lstPE.SetItemText(nItem, 1, mapIOHDD[nItem]);
		m_lstPE.SetItemText(nItem, 2, "DWORD");

		CString csData;
		csData.Format("%08x", pImgNtHeader->OptionalHeader.DataDirectory[nIndexRVA].VirtualAddress);
		m_lstPE.SetItemText(nItem, 3, csData);
		//判断是否存在节
		if (DWORD(pImgNtHeader->OptionalHeader.DataDirectory[nIndexRVA].VirtualAddress) != 0)
		{	

			pImgSecHeader = IMAGE_FIRST_SECTION(pImgNtHeader);
			//获取节表数量
			WORD wNum = pImgNtHeader->FileHeader.NumberOfSections;

			//判断是哪一个节
			int nSec = 0;
			while (nSec<wNum-1)
			{
				if (pImgNtHeader->OptionalHeader.DataDirectory[nIndexRVA].VirtualAddress<pImgSecHeader[nSec+1].VirtualAddress)
				{
					break;
				}
				nSec++;
			}

			CString SectionName;
			SectionName.Format("%s", pImgSecHeader[nSec].Name);//显示所在的节
			m_lstPE.SetItemText(nItem, 4, SectionName);
		}

		//size:
		nItem++;
		i += 4;
		strIndexHex.Format("%08x", i);
		m_lstPE.InsertItem(nItem, strIndexHex);//offset+4
		m_lstPE.SetItemText(nItem, 1, mapIOHDD[nItem]);
		m_lstPE.SetItemText(nItem, 2, "DWORD");

		csData.Format("%08x", pImgNtHeader->OptionalHeader.DataDirectory[nIndexSize].Size);
		m_lstPE.SetItemText(nItem, 3, csData);
		nIndexSize++;
		nIndexRVA++;
		nItem++;
	}

	return;
}

//CMyTestDlg.cpp
void  CMyPEViewDlg::OnDropFiles(HDROP hDropInfo) {
	int DropCount = DragQueryFile(hDropInfo, -1, NULL, 0);//取得被拖动文件的数目
	// 循环依次取得拖动文件的File Name信息
	TCHAR filePath[MAX_PATH];
	for (int i = 0; i < DropCount; i++)
	{
		// 定义一个缓冲区来存放读取的文件名信息

		DragQueryFile(hDropInfo, i, filePath, MAX_PATH);//获得拖曳的第i个文件的文件名
		//此时 filePath已经获取到了文件路径
		//do something...
	}

	m_strPath.Format("%s", filePath);

	OpenPEFile();
	//拖放结束后,释放内存
	DragFinish(hDropInfo);
	CDialog::OnDropFiles(hDropInfo);
}

void CMyPEViewDlg::LoadSafeSkin()
{
	HMODULE hModule = LoadLibrary(TEXT("MySafeSkin.dll"));
	if (hModule)
	{
		typedef  int  (WINAPI* pMySafeSkin)(void);
		pMySafeSkin MySafeSkin;
		MySafeSkin = (pMySafeSkin)GetProcAddress(hModule, "MySafeSkin");
		MySafeSkin();
	}
}

DWORD CMyPEViewDlg::RvaToFoa(PIMAGE_NT_HEADERS pImgNtHdr, DWORD dwRva)
{

	/*
	1.判断RVA地址所属的节
	*/
	DWORD dwFOA = 0;

	//获取节的数量
	pImgSecHeader = IMAGE_FIRST_SECTION(pImgNtHeader);
	//获取节表数量
	WORD wNum = pImgNtHeader->FileHeader.NumberOfSections;
	//获取节对齐后的长度
	DWORD wLenSection = pImgSecHeader->SizeOfRawData;
	//   判断 rva < = rva  <rva+len
	WORD i = 0;
	while (i< wNum)
	{
		//获取节对齐后的长度
		DWORD wLenSection = pImgSecHeader->SizeOfRawData;
		if (dwRva >= pImgSecHeader->VirtualAddress && dwRva < pImgSecHeader->VirtualAddress + wLenSection)
		{
			break;
		}
		pImgSecHeader++;
		i++;
	}
	/*
	2.判断是否找到所属的节
	*/
	if (i <= wNum)
	{
		dwFOA = dwRva - pImgSecHeader->VirtualAddress + pImgSecHeader->PointerToRawData;
	}

	return dwFOA;
}

CString CMyPEViewDlg::WhereRva(PIMAGE_NT_HEADERS pImgNtHdr, DWORD dwRva)
{
	/*
	1.判断RVA地址所属的节
	*/
	DWORD dwFOA = 0;

	//获取节的数量
	pImgSecHeader = IMAGE_FIRST_SECTION(pImgNtHeader);
	//获取节表数量
	WORD wNum = pImgNtHeader->FileHeader.NumberOfSections;
	//获取节对齐后的长度
	DWORD wLenSection = pImgSecHeader->SizeOfRawData;
	//   判断 rva < = rva  <rva+len
	WORD i = 0;
	while (i < wNum)
	{
		//获取节对齐后的长度
		DWORD wLenSection = pImgSecHeader->SizeOfRawData;
		if (dwRva >= pImgSecHeader->VirtualAddress && dwRva < pImgSecHeader->VirtualAddress + wLenSection)
		{
			break;
		}
		pImgSecHeader++;
		i++;
	}
	/*
	2.判断是否找到所属的节
	*/
	if (i <= wNum)
	{
		dwFOA = dwRva - pImgSecHeader->VirtualAddress + pImgSecHeader->PointerToRawData;
	}

	CString csSecName;
	csSecName = pImgSecHeader->Name;

	return csSecName;
}


void CMyPEViewDlg::UpdataListDosHeader()
{
	DelListAll();

	m_lstPE.InsertColumn(0, "保留", LVCFMT_CENTER, 0);
	m_lstPE.InsertColumn(1,"pFile", LVCFMT_CENTER, 100);
	m_lstPE.InsertColumn(2,"Type", LVCFMT_CENTER, 80);
	m_lstPE.InsertColumn(3,"header", LVCFMT_CENTER, 100);
	m_lstPE.InsertColumn(4,"Data", LVCFMT_CENTER, 100);
	m_lstPE.InsertColumn(5,"Description", LVCFMT_CENTER, 300);
	m_lstPE.InsertColumn(6,"Meaning", LVCFMT_CENTER, 260);
	m_lstPE.DeleteColumn(0);

	//设置风格
	LONG lStyle;
	lStyle = GetWindowLong(m_lstPE.m_hWnd, GWL_STYLE);//获取当前窗口style
	DWORD dwStyle = m_lstPE.GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP;
	m_lstPE.SetExtendedStyle(dwStyle);
}

void CMyPEViewDlg::UpdataListNTHeader()
{
	DelListAll();

	m_lstPE.InsertColumn(0, "保留", LVCFMT_CENTER, 0);
	m_lstPE.InsertColumn(1, "pFile", LVCFMT_CENTER, 100);
	m_lstPE.InsertColumn(2, "Type", LVCFMT_CENTER, 80);
	m_lstPE.InsertColumn(3, "Member", LVCFMT_CENTER, 220);
	m_lstPE.InsertColumn(4, "Value", LVCFMT_CENTER, 100);
	m_lstPE.InsertColumn(5, "Meaning", LVCFMT_CENTER, 120);
	m_lstPE.DeleteColumn(0);

	//设置风格
	LONG lStyle;
	lStyle = GetWindowLong(m_lstPE.m_hWnd, GWL_STYLE);//获取当前窗口style
	DWORD dwStyle = m_lstPE.GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP;
	m_lstPE.SetExtendedStyle(dwStyle);
}

void CMyPEViewDlg::ShowImgListSection()
{
	DelListAll();

	m_lstPE.InsertColumn(0, "保留", LVCFMT_CENTER, 0);
	m_lstPE.InsertColumn(1, "Name", LVCFMT_CENTER, 160);
	m_lstPE.InsertColumn(2, "Virtual Address", LVCFMT_CENTER, 160);
	m_lstPE.InsertColumn(3, "Misc.Virtual Size", LVCFMT_CENTER, 160);
	m_lstPE.InsertColumn(4, "Point Raw Size", LVCFMT_CENTER, 160);
	m_lstPE.InsertColumn(5, "Raw Data Size", LVCFMT_CENTER, 160);
	m_lstPE.InsertColumn(6, "Characteristics", LVCFMT_CENTER, 160);
	m_lstPE.DeleteColumn(0);

	//设置风格
	LONG lStyle;
	lStyle = GetWindowLong(m_lstPE.m_hWnd, GWL_STYLE);//获取当前窗口style
	DWORD dwStyle = m_lstPE.GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP;
	m_lstPE.SetExtendedStyle(dwStyle);

	//获取节的数量
	pImgSecHeader = IMAGE_FIRST_SECTION(pImgNtHeader);
	WORD nNum = pImgNtHeader->FileHeader.NumberOfSections;
	CString pImgName;
	CString pImgVA;
	CString pImgVS;
	CString pImgPRS;
	CString pImgRDS;
	CString pImgChara;

	int nColld = 0;
	for (int i = 0; i < nNum; i++)
	{
		pImgName.Format("%s", pImgSecHeader[i].Name);
		pImgVA.Format("%08x", pImgSecHeader[i].VirtualAddress);
		pImgVS.Format("%08x", pImgSecHeader[i].Misc.VirtualSize);
		pImgPRS.Format("%08x", pImgSecHeader[i].PointerToRawData);
		pImgRDS.Format("%08x", pImgSecHeader[i].SizeOfRawData);
		pImgChara.Format("%08x", pImgSecHeader[i].Characteristics);

		m_lstPE.InsertItem(i, pImgName);
		m_lstPE.SetItemText(i, ++nColld, pImgVA);
		m_lstPE.SetItemText(i, ++nColld, pImgVS);
		m_lstPE.SetItemText(i, ++nColld, pImgPRS);
		m_lstPE.SetItemText(i, ++nColld, pImgRDS);
		m_lstPE.SetItemText(i, ++nColld, pImgChara);
		nColld = 0;
	}


	return;
}

void CMyPEViewDlg::ShowImgImportTableList()
{
	DelListAll();
	m_lstPE.InsertColumn(0, "保留", LVCFMT_CENTER, 0);
	m_lstPE.InsertColumn(1, "DLL Name", LVCFMT_CENTER, 160);
	m_lstPE.InsertColumn(2, "OriginalFirstThunk", LVCFMT_CENTER, 160);
	m_lstPE.InsertColumn(3, " 日期时间标志", LVCFMT_CENTER, 160);
	m_lstPE.InsertColumn(4, "ForwarderChain", LVCFMT_CENTER, 160);
	m_lstPE.InsertColumn(5, "Characteristics", LVCFMT_CENTER, 160);
	m_lstPE.InsertColumn(6, "FirsrtThunk", LVCFMT_CENTER, 160);
	m_lstPE.DeleteColumn(0);

	//设置风格
	LONG lStyle;
	lStyle = GetWindowLong(m_lstPE.m_hWnd, GWL_STYLE);//获取当前窗口style
	DWORD dwStyle = m_lstPE.GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP| CS_DBLCLKS;
	m_lstPE.SetExtendedStyle(dwStyle);

	//解析DLL名称
	int nItem = 0;
	int nCloldx = 0;
	CString	 csDllName;
	CString csOriFirstThunk;
	CString csTimeDateStamp;
	CString csCharacteristics;
	CString csForwarderChain;
	CString csFirstThunk;
	PIMAGE_IMPORT_DESCRIPTOR pImgpIID = NULL;
	/*
1、 读取IID的Name成员，获取库名称字符串（“Kernel32.dll”）
2、 装载相应库，LoadLibrary("Kernel32.dll")
3、 读取IID的OriginalFirstThunk成员，获取INT地址
4、 读取INT数组中的值，获取相应IMAGE_IMPORT_BY_NAME地址（RVA）
5、 使用IMAGE_IMPORT_BY_NAME的Hint或Name项，获取相应函数的起始地址
6、 读取IID的FirstThunk（IAT）成员，获得IAT地址
7、 将上面获得的函数地址输入相应的IAT数组值
8、 重复4 - 7步骤，直到INT结束（遇到NULL时）
	*/

	//重定位导入表的位置    先RVA->FOA
	pImgpIID = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)m_lpBase + RvaToFoa(pImgNtHeader, pImgNtHeader->
		OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress));

	while (pImgpIID->Name)
	{
		//RVA->FOA
		DWORD n = (DWORD)m_lpBase + RvaToFoa(pImgNtHeader, pImgpIID->Name);
		csDllName.Format("%s", (DWORD)m_lpBase + RvaToFoa(pImgNtHeader, pImgpIID->Name));
		csOriFirstThunk.Format("%08x", pImgpIID->OriginalFirstThunk);
		csTimeDateStamp.Format("%08x",  pImgpIID->TimeDateStamp);
		csCharacteristics.Format("%08x",  pImgpIID->Characteristics);
		csForwarderChain.Format("%08x", pImgpIID->ForwarderChain);
		csFirstThunk.Format("%08x", pImgpIID->FirstThunk);

		m_lstPE.InsertItem(nItem, csDllName);
		m_lstPE.SetItemText(nItem, ++nCloldx, csOriFirstThunk);
		m_lstPE.SetItemText(nItem, ++nCloldx, csTimeDateStamp);
		m_lstPE.SetItemText(nItem, ++nCloldx, csForwarderChain);
		m_lstPE.SetItemText(nItem, ++nCloldx, csCharacteristics);
		m_lstPE.SetItemText(nItem, ++nCloldx, csFirstThunk);

		nCloldx = 0;
		nItem++;
		pImgpIID++;
	}

}

void CMyPEViewDlg::ShowImgExportTable()
{

	DelListAll();
	m_lstPE.InsertColumn(0, "保留", LVCFMT_CENTER, 0);
	m_lstPE.InsertColumn(1, "Office", LVCFMT_CENTER, 160);
	m_lstPE.InsertColumn(2, "Member", LVCFMT_CENTER, 160);
	m_lstPE.InsertColumn(3, "Size", LVCFMT_CENTER, 160);
	m_lstPE.InsertColumn(4, "Value", LVCFMT_CENTER, 160);
	m_lstPE.DeleteColumn(0);

	PIMAGE_EXPORT_DIRECTORY pIED = NULL;
	//获取导出表的位置
	DWORD dwIED = RvaToFoa(pImgNtHeader, pImgNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	//解析导出表
	int nItem = 0;
	for (DWORD i = dwIED; i <= dwIED + 36; i += 2)
	{
		CString strIndexHex;
		strIndexHex.Format("%08x", i);
		m_lstPE.InsertItem(nItem, strIndexHex);//offset

		TCHAR szbuff[8] = { 0 };
		TCHAR szDataMsg[32] = { 0 };
		if (mapIED[nItem].Type == "WORD")
		{
			memcpy_s(szbuff, 2, (char*)m_lpBase + i, 2);
			for (int j = 1; j >= 0; j--)
			{
				sprintf(szDataMsg, "%s%02x", szDataMsg, (unsigned char)szbuff[j]);		//Hex  data
			}
			CString strDateHex;
			CString strAsciiHex;
			strDateHex.Format("%s", szDataMsg);
			m_lstPE.SetItemText(nItem, 1, mapIED[nItem].Member);
			m_lstPE.SetItemText(nItem, 2, mapIED[nItem].Type);
			m_lstPE.SetItemText(nItem, 3, strDateHex);
		}
		else if (mapIED[nItem].Type == "DWORD")
		{
			memcpy_s(szbuff, 4 ,(char*)m_lpBase + i, 4);
			for (int j = 3; j >= 0; j--)
			{
				sprintf(szDataMsg, "%s%02x", szDataMsg, (unsigned char)szbuff[j]);		//Hex  data
			}
			CString strDateHex;
			CString strAsciiHex;
			strDateHex.Format("%s", szDataMsg);
			m_lstPE.SetItemText(nItem, 1, mapIED[nItem].Member);
			m_lstPE.SetItemText(nItem, 2, mapIED[nItem].Type);
			m_lstPE.SetItemText(nItem, 3, strDateHex);
			i += 2;
		}
		nItem++;
	}

	return;
}

void CMyPEViewDlg::ShowImgIETFunction()
{
	DelListAll();
	m_lstPE.InsertColumn(0, "保留", LVCFMT_CENTER, 0);
	m_lstPE.InsertColumn(1, "序号", LVCFMT_CENTER, 160);
	m_lstPE.InsertColumn(2, "RVA", LVCFMT_CENTER, 160);
	m_lstPE.InsertColumn(3, "FOA", LVCFMT_CENTER, 160);
	m_lstPE.InsertColumn(4, "函数名地址", LVCFMT_CENTER, 160);
	m_lstPE.InsertColumn(5, "函数名", LVCFMT_CENTER, 300);
	m_lstPE.DeleteColumn(0);

	//PIMAGE_EXPORT_DIRECTORY pIED = NULL;
	//获取函数数量

	CString csNumber = 0;
	CString csRVA = 0;
	CString csFOA = 0;
	CString csFuncNameAddr = 0;
	CString csFuncName = 0;

	//获得IED
	IMAGE_EXPORT_DIRECTORY* IED = (IMAGE_EXPORT_DIRECTORY*)( (DWORD)m_lpBase + RvaToFoa(pImgNtHeader, pImgNtHeader->
		OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress));

	DWORD* dwaddrFunc = (DWORD*) ((DWORD)m_lpBase + RvaToFoa(pImgNtHeader, IED->AddressOfFunctions));
	DWORD* dwaddrName = (DWORD*) ((DWORD)m_lpBase + RvaToFoa(pImgNtHeader, IED->AddressOfNames));
	WORD* dwaddrOrdi = (WORD*) ((DWORD)m_lpBase + RvaToFoa(pImgNtHeader, IED->AddressOfNameOrdinals));

	DWORD dwNumberOfFunc = IED->NumberOfFunctions;
	DWORD dwNumberOfName = IED->NumberOfNames;

	DWORD i,j;
	for (i = 0; i < dwNumberOfFunc; i++)
	{
		//地址为0表示序号没有对应的函数
		if ( 0==dwaddrFunc[i] )  
		{
			continue;
		}

		//序号表序号有dwNumberOfName个
		for (j = 0; j < dwNumberOfName; j++)
		{
			//序号表值为地址表索引时
			if (dwaddrOrdi[j] == i)
			{
				csNumber.Format("%04x", i+IED->Base);
				csRVA.Format("%08x", dwaddrFunc[i]);
				csFOA.Format("%08x", RvaToFoa(pImgNtHeader, dwaddrFunc[i]));
				csFuncNameAddr.Format("%08x", dwaddrName[j]);

				csFuncName.Format("%s", (DWORD)m_lpBase +RvaToFoa(pImgNtHeader, dwaddrName[j]));
				m_lstPE.InsertItem(i, csNumber);
				m_lstPE.SetItemText(i, 1, csRVA);
				m_lstPE.SetItemText(i, 2, csFOA);
				m_lstPE.SetItemText(i, 3, csFuncNameAddr);
				m_lstPE.SetItemText(i, 4, csFuncName);
				break;
			}
		}
		//存在dwaddrOrd[j]
		if ( j!=dwNumberOfName)
		{
			continue;//可以在序号表找到
		}
		else//序号表没有地址表索引，说明导出函数是以地址导出的匿名函数
		{
			csNumber.Format("%04x", i+IED->Base);
			csRVA.Format("%08x", dwaddrFunc[i]);
			csFOA.Format("%08x", RvaToFoa(pImgNtHeader, dwaddrFunc[i]));
			csFuncNameAddr.Format("%08x", dwaddrName[i]);
			m_lstPE.InsertItem(i, csNumber);
			m_lstPE.SetItemText(i, 1, csRVA);
			m_lstPE.SetItemText(i, 2, csFOA);
			m_lstPE.SetItemText(i, 3, csFuncNameAddr);
			m_lstPE.SetItemText(i, 4, "NoName匿名函数");
		}
	}

}

void CMyPEViewDlg::ShowRelocationDirectory()
{
	DelListAll();

	m_lstPE.InsertColumn(0, "保留", LVCFMT_CENTER, 0);
	m_lstPE.InsertColumn(1, "索引", LVCFMT_CENTER, 160);
	m_lstPE.InsertColumn(2, "RVA", LVCFMT_CENTER, 160);
	m_lstPE.InsertColumn(3, "Size Of Block", LVCFMT_CENTER, 160);
	m_lstPE.InsertColumn(4, "所在的节", LVCFMT_CENTER, 160);
	m_lstPE.InsertColumn(5, "项目", LVCFMT_CENTER, 160);
	m_lstPE.DeleteColumn(0);
	PIMAGE_BASE_RELOCATION pIBRL =(PIMAGE_BASE_RELOCATION) ((DWORD)m_lpBase + RvaToFoa(pImgNtHeader,
		pImgNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress));

	int nItem = 0;
	while(pIBRL->SizeOfBlock && pIBRL->VirtualAddress  )
	{

		CString csIndex;
		CString csRVA;
		CString csSize;
		CString csItem;
		CString csSection;

		csIndex.Format("%d", nItem + 1);
		csRVA.Format("%08x", pIBRL->VirtualAddress);
		csSize.Format("%08x", pIBRL->SizeOfBlock);
		csSection.Format("%s", WhereRva(pImgNtHeader, pIBRL->VirtualAddress));
		//获取重定位表总个数  (size-8)/2
		csItem.Format("%xh/ %dd", (pIBRL->SizeOfBlock - 8) / 2, (pIBRL->SizeOfBlock + 8) / 2);

		m_lstPE.InsertItem(nItem, csIndex);
		m_lstPE.SetItemText(nItem, 1, csRVA);
		m_lstPE.SetItemText(nItem, 2, csSize);
		m_lstPE.SetItemText(nItem, 3, csSection);
		m_lstPE.SetItemText(nItem, 4, csItem);

		nItem++;
		pIBRL =PIMAGE_BASE_RELOCATION( (char*)pIBRL+ pIBRL->SizeOfBlock);
	}
}

void CMyPEViewDlg::ShowResourceDirectory()
{
	DelListAll();

	m_lstPE.InsertColumn(0, "保留", LVCFMT_CENTER, 0);
	m_lstPE.InsertColumn(1, "pFile", LVCFMT_CENTER, 160);
	m_lstPE.InsertColumn(2, "Type", LVCFMT_CENTER, 160);
	m_lstPE.InsertColumn(3, "Member", LVCFMT_CENTER, 200);
	m_lstPE.InsertColumn(4, "Value", LVCFMT_CENTER, 160);
	m_lstPE.DeleteColumn(0);
	PIMAGE_RESOURCE_DIRECTORY pIPRD = (PIMAGE_RESOURCE_DIRECTORY)((DWORD)m_lpBase + RvaToFoa(pImgNtHeader,
		pImgNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress));

	DWORD SizeOfIRes = RvaToFoa(pImgNtHeader,
		pImgNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].Size);

	int nItem = 0;
	for (DWORD i = SizeOfIRes; i < SizeOfIRes + 0xF; i += 2)
	{
		CString strIndexHex;
		strIndexHex.Format("%08x", i);
		m_lstPE.InsertItem(nItem, strIndexHex);//offset

		if (mapResourse[nItem].Type == "WORD")
		{
			m_lstPE.SetItemText(nItem, 1, mapResourse[nItem].Type);
			m_lstPE.SetItemText(nItem, 2, mapResourse[nItem].Member);
		}
		else if (mapResourse[nItem].Type == "DWORD")
		{
			CString strDateHex;
			CString strAsciiHex;
			m_lstPE.SetItemText(nItem, 1, mapResourse[nItem].Type);
			m_lstPE.SetItemText(nItem, 2, mapResourse[nItem].Member);
			i += 2;
		}
		nItem++;
	}
	CString M0;
	CString M1;
	CString M2;
	CString M3;
	CString M4;
	CString M5;

	M0.Format("%08x", pIPRD->Characteristics);
	M1.Format("%08x", pIPRD->TimeDateStamp);
	M2.Format("%04x", pIPRD->MajorVersion);
	M3.Format("%04x", pIPRD->MinorVersion);
	M4.Format("%04x", pIPRD->NumberOfNamedEntries);
	M5.Format("%04x", pIPRD->NumberOfIdEntries);

	m_lstPE.SetItemText(0, 3, M0);
	m_lstPE.SetItemText(1, 3, M1);
	m_lstPE.SetItemText(2, 3, M2);
	m_lstPE.SetItemText(3, 3, M3);
	m_lstPE.SetItemText(4, 3, M4);
	m_lstPE.SetItemText(5, 3, M5);

	return;
}




void CMyPEViewDlg::ShowDebugDirectory()
{
	DelListAll();

	m_lstPE.InsertColumn(0, "保留", LVCFMT_CENTER, 0);
	m_lstPE.InsertColumn(1, "特征值", LVCFMT_CENTER, 110);
	m_lstPE.InsertColumn(2, "日期时间标志", LVCFMT_CENTER, 110);
	m_lstPE.InsertColumn(3, "主版本", LVCFMT_CENTER, 110);
	m_lstPE.InsertColumn(4, "子版本", LVCFMT_CENTER, 110);
	m_lstPE.InsertColumn(5, "类型", LVCFMT_CENTER, 110);
	m_lstPE.InsertColumn(6, "数据大小", LVCFMT_CENTER, 110);
	m_lstPE.InsertColumn(7, "原始数据地址", LVCFMT_CENTER, 110);
	m_lstPE.InsertColumn(8, "原始数据指针", LVCFMT_CENTER, 110);
	m_lstPE.DeleteColumn(0);

	PIMAGE_DEBUG_DIRECTORY pIDBD = (PIMAGE_DEBUG_DIRECTORY)((DWORD)m_lpBase + RvaToFoa(pImgNtHeader,
		pImgNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress));
	
	DWORD SizeOfIDBD= RvaToFoa(pImgNtHeader,
		pImgNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].Size);

	int nIDBD = SizeOfIDBD / 28;


	CString csCharact;
	CString csTimeData;
	CString csMajorVerson;
	CString csMiniorVerson;
	CString csType;
	CString csSizeOfData;
	CString csAdd;
	CString csPoint;

	int nItem = 0;
	for(int i=0;i<nIDBD;i++)
	{
	 csCharact.Format("%08x",pIDBD->Characteristics);
	 csTimeData.Format("%08x",pIDBD->TimeDateStamp);
	 csMajorVerson.Format("%08x",pIDBD->MajorVersion);
	 csMiniorVerson.Format("%08x",pIDBD->MajorVersion);
	 csType.Format("%08x",pIDBD->Type);
	 csSizeOfData.Format("%08x",pIDBD->SizeOfData);
	 csAdd.Format("%08x",pIDBD->AddressOfRawData);
	 csPoint.Format("%08x",pIDBD->PointerToRawData);

	m_lstPE.InsertItem(nItem,csCharact);
	m_lstPE.SetItemText(nItem,1,csTimeData);
	m_lstPE.SetItemText(nItem, 2, csMajorVerson);
	m_lstPE.SetItemText(nItem, 3, csMiniorVerson);
	m_lstPE.SetItemText(nItem, 4, csType);
	m_lstPE.SetItemText(nItem, 5, csSizeOfData);
	m_lstPE.SetItemText(nItem, 6, csAdd);
	m_lstPE.SetItemText(nItem, 7, csPoint);
	 pIDBD++;
	 nItem++;
	}

}


void CMyPEViewDlg::ShowTLSDirectory()
{
	DelListAll();

	m_lstPE.InsertColumn(0, "保留", LVCFMT_CENTER, 0);
	m_lstPE.InsertColumn(1, "pFile", LVCFMT_CENTER, 160);
	m_lstPE.InsertColumn(2, "Type", LVCFMT_CENTER, 160);
	m_lstPE.InsertColumn(3, "Member", LVCFMT_CENTER, 200);
	m_lstPE.InsertColumn(4, "Value", LVCFMT_CENTER, 160);
	m_lstPE.InsertColumn(5, "描述", LVCFMT_CENTER, 160);
	m_lstPE.DeleteColumn(0);
	PIMAGE_TLS_DIRECTORY32 pTLS = (PIMAGE_TLS_DIRECTORY32)((DWORD)m_lpBase + RvaToFoa(pImgNtHeader,
		pImgNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress));

	int nItem = 0;

	CString cspFile;
	CString csType;
	CString csMember;
	CString csValue;
	CString csDec;

	DWORD TLSOffsetStart=(DWORD)pTLS-(DWORD)m_lpBase;
	DWORD TLSOffsetEnd= TLSOffsetStart + 24;

	for (DWORD i = TLSOffsetStart; i < TLSOffsetEnd; i += 4)
	{
		cspFile.Format("%08x",i);
		csType.Format("%s",mapTLS[nItem].Type);
		csMember.Format("%s",mapTLS[nItem].Member);
		csDec.Format("%s",mapTLS[nItem].Describe);

		m_lstPE.InsertItem(nItem, cspFile);
		m_lstPE.SetItemText(nItem, 1,csType);
		m_lstPE.SetItemText(nItem, 2,csMember);
		m_lstPE.SetItemText(nItem, 4,csDec);

		nItem++;
	}
	CString M0; 
	CString M1; 
	CString M2;
	CString M3;
	CString M4;
	CString M5;

	M0.Format("%08x", pTLS->StartAddressOfRawData);
	M1.Format("%08x", pTLS->EndAddressOfRawData);
	M2.Format("%08x", pTLS->AddressOfIndex);
	M3.Format("%08x", pTLS->AddressOfCallBacks);
	M4.Format("%08x", pTLS->SizeOfZeroFill);
	M5.Format("%08x", pTLS->Characteristics);

	m_lstPE.SetItemText(0, 3, M0);
	m_lstPE.SetItemText(1, 3, M1);
	m_lstPE.SetItemText(2, 3, M2);
	m_lstPE.SetItemText(3, 3, M3);
	m_lstPE.SetItemText(4, 3, M4);
	m_lstPE.SetItemText(5, 3, M5);
}

void CMyPEViewDlg::ShowResource(int nIndexDeep1,int nIndexDeep2)
{
	DelListAll();

	m_lstPE.InsertColumn(0, "保留", LVCFMT_CENTER, 0);
	m_lstPE.InsertColumn(1, "RVA", LVCFMT_CENTER, 200);
	m_lstPE.InsertColumn(2, "偏移", LVCFMT_CENTER, 200);
	m_lstPE.InsertColumn(3, "大小", LVCFMT_CENTER, 200);
	m_lstPE.InsertColumn(4, "Reserved", LVCFMT_CENTER, 160);
	m_lstPE.DeleteColumn(0);

	nDeepIndex1 = 0;
	nDeepIndex2 = 0;

	//DFS解析资源表三层结构
	IMAGE_RESOURCE_DIRECTORY* IPRD = (IMAGE_RESOURCE_DIRECTORY*)((DWORD)m_lpBase + RvaToFoa(pImgNtHeader,
		pImgNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress));

	if (IPRD->NumberOfIdEntries + IPRD->NumberOfNamedEntries != 0)
	{
		//递归显示资源树
		ShowRes_DFS(0, IPRD, IPRD, nIndexDeep1,nIndexDeep2);
	}

}

VOID CMyPEViewDlg::ShowRes_DFS(DWORD dwDeep, IMAGE_RESOURCE_DIRECTORY* pNewIRDE,
	IMAGE_RESOURCE_DIRECTORY* pOriIRDE,  int nIndexDeep1,int nIndexDeep2)
{

	CString csFirstName;
	CString csFirstID;
	HTREEITEM  hImgRes;
	dwDeep++;
	IMAGE_RESOURCE_DIRECTORY_ENTRY* pResourceBaseAddress = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)((PBYTE)
		pNewIRDE + sizeof(IMAGE_RESOURCE_DIRECTORY));

	for (int i = 0; i < pNewIRDE->NumberOfIdEntries + pNewIRDE->NumberOfNamedEntries; i++)
	{
		IMAGE_RESOURCE_DIRECTORY_ENTRY* pNowResourceAddress = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)
			((DWORD)pResourceBaseAddress + i * sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY));

		//选中的资源为第一级树
		if (pNowResourceAddress->Id == nIndexDeep1 && dwDeep==1)
		{
			nDeepIndex1 = pNowResourceAddress->Id;
			ShowRes_DFS(dwDeep, (IMAGE_RESOURCE_DIRECTORY*)((DWORD)
				pNowResourceAddress->OffsetToDirectory + (DWORD)pOriIRDE),
				pOriIRDE, nIndexDeep1, nIndexDeep2);
		}

		//选中的资源为第二级树
		if ( pNowResourceAddress->Id == nIndexDeep2 && dwDeep == 2)
		{
			nDeepIndex2 = pNowResourceAddress->Id;
			ShowRes_DFS(dwDeep, (IMAGE_RESOURCE_DIRECTORY*)((DWORD)
				pNowResourceAddress->OffsetToDirectory + (DWORD)pOriIRDE),
				pOriIRDE, nIndexDeep1, nIndexDeep2);

		}
		//选中的资源为第三级树
		if (dwDeep == 3)
		{
			if (pNowResourceAddress->DataIsDirectory) {
				ShowRes_DFS(dwDeep, (IMAGE_RESOURCE_DIRECTORY*)((DWORD)
					pNowResourceAddress->OffsetToDirectory + (DWORD)pOriIRDE),
					pOriIRDE, nIndexDeep1, nIndexDeep2);
			}
			else if (nDeepIndex1== nIndexDeep1&& nDeepIndex2==nIndexDeep2)
			{
				IMAGE_RESOURCE_DATA_ENTRY* pFileRes = (IMAGE_RESOURCE_DATA_ENTRY*)((DWORD)
					pNowResourceAddress->OffsetToDirectory + (DWORD)pOriIRDE);

				CString csRva;
				CString csOffset;
				CString csSize;
				CString csReserve;

				csRva.Format("%08x", pFileRes->OffsetToData);
				csOffset.Format("%08x", pFileRes->CodePage);
				csSize.Format("%08x", pFileRes->Size);
				csReserve.Format("%08x", pFileRes->Reserved);

				m_lstPE.InsertItem(0, csRva);
				m_lstPE.SetItemText(0, 1, csOffset);
				m_lstPE.SetItemText(0, 2, csSize);
				m_lstPE.SetItemText(0, 3, csReserve);

				nDeepIndex1 = 0;
				nDeepIndex2 = 0;

				return;
			}

		}	
		else
		{
			ShowRes_DFS(dwDeep, (IMAGE_RESOURCE_DIRECTORY*)((DWORD)
				pNowResourceAddress->OffsetToDirectory + (DWORD)pOriIRDE),
				pOriIRDE, nIndexDeep1, nIndexDeep2);
		}

	}
	return;
}



void CMyPEViewDlg::OnInjectModEntry()
{
	if (m_hFile!=0)
	{
		CInjectEndFile IFE;
		IFE.SetAtt(m_hFile, (int)m_lpBase, PEEntryPoint, m_nFileSize);
		IFE.DoModal();
	}
	else
	{
		AfxMessageBox("请先打开文件");
	}
	return;
}


void CMyPEViewDlg::OnAddSection()
{
	if (m_hFile == 0)
	{
		AfxMessageBox("请先打开文件");
		return;
	}

	//获取节的数量
	pImgSecHeader = IMAGE_FIRST_SECTION(pImgNtHeader);
	WORD nNum = pImgNtHeader->FileHeader.NumberOfSections;
	//节大小
	DWORD dwSzie = pImgNtHeader->OptionalHeader.FileAlignment;

	PIMAGE_SECTION_HEADER pImgNewSection = (PIMAGE_SECTION_HEADER)((DWORD)pImgSecHeader + nNum * sizeof(IMAGE_SECTION_HEADER));
	/*
	// 在最后一个节后边追加增加512字节
	PBYTE pByte = (PBYTE)malloc(dwSzie);
	memset(pByte, 0,dwSzie);
	DWORD dwImgSectionOffset = (DWORD)pImgNewSection - (DWORD)pImgDosHeader;
	SetFilePointer(m_hFile,dwImgSectionOffset,0,FILE_BEGIN);

	DWORD dwNum = 0;
	BOOL b = WriteFile(m_hFile, pByte, dwSzie, &dwNum, NULL);
	free(pByte);
	*/

	// 在文件末尾追加增加512字节
	PBYTE pByte = (PBYTE)malloc(dwSzie);
	memset(pByte, 0, dwSzie);

	SetFilePointer(m_hFile, 0, 0, FILE_END);
	DWORD dwNum = 0;
	BOOL b = WriteFile(m_hFile, pByte, dwSzie, &dwNum, NULL);

	//构造新的节
	//char* cInjName = "test";
	pImgNewSection->Name, " ";
	pImgNewSection->Misc.VirtualSize = pImgNtHeader->OptionalHeader.SectionAlignment;
	pImgNewSection->VirtualAddress =pImgSecHeader[nNum - 1].VirtualAddress + pImgNtHeader->OptionalHeader.SectionAlignment;
	pImgNewSection->SizeOfRawData = dwSzie;
	pImgNewSection->PointerToRawData = pImgSecHeader[nNum - 1].PointerToRawData + pImgSecHeader[nNum - 1].SizeOfRawData;
	pImgNewSection->Characteristics = 0xE00000F0;

	//重新修改节的数量
	pImgNtHeader->FileHeader.NumberOfSections += 1;
	//重新修改镜像大小
	pImgNtHeader->OptionalHeader.SizeOfImage+=pImgNtHeader->OptionalHeader.SectionAlignment;

	return;

}


void CMyPEViewDlg::OnCalVirtual()
{
	if (m_hFile != 0)
	{
		CLocationCal cal;
		cal.SetPEParameter(m_hFile, m_lpBase, pImgDosHeader, pImgNtHeader, pImgSecHeader);
		cal.DoModal();
	}
	else
	{
		AfxMessageBox("请先打开文件");
	}
	return;
}


void CMyPEViewDlg::OnMemDump()
{

	CMemDump dumpDlg;
	dumpDlg.DoModal();

}




void CMyPEViewDlg::OnDblclkLstPe(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	int nIdx = m_lstPE.GetSelectionMark();
	//导出表加载Thunk
	CString csDllName = m_lstPE.GetItemText(nIdx, 0);
	if (csDllName.Right(3)== "dll")
	{
		CITDThunk itddlg;
		itddlg.SetPEParameter(csDllName, m_lpBase, pImgNtHeader,pImgSecHeader);
		itddlg.DoModal();
	}

	//重定位表加载ITEM
	CHeaderCtrl* pCtrl = m_lstPE.GetHeaderCtrl();
	HDITEM item = { 0 };
	TCHAR szBuffer[MAX_PATH] = { 0 };
	item.mask = HDI_TEXT;
	item.pszText = szBuffer;
	item.cchTextMax = _countof(szBuffer);
	pCtrl->GetItem(4, &item);

	CString csItem;
	csItem.Format("%s", szBuffer);
	if (csItem== "项目")
	{
		CIBRLItem IBRLDlg;
		CString csRva = m_lstPE.GetItemText(nIdx,1);
		IBRLDlg.SetPEParameter(csRva, m_lpBase, pImgNtHeader, pImgSecHeader);
		IBRLDlg.DoModal();
	}

	*pResult = 0;
}
