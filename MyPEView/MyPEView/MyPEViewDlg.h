
// MyPEViewDlg.h: 头文件
//

#pragma once
#include<map>
#include <typeinfo>
#include"CInjectEndFile.h"
#include"CLocationCal.h"
#include"CMemDump.h"
#include"CITDThunk.h"
#include"CIBRLItem.h"

using namespace std;


// CMyPEViewDlg 对话框
class CMyPEViewDlg : public CDialogEx
{
// 构造
public:
	CMyPEViewDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_PE };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CMenu m_Menu;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CListCtrl m_lstPE;
	CTreeCtrl m_treePE;

public:
	PIMAGE_DOS_HEADER pImgDosHeader=NULL;
	PIMAGE_NT_HEADERS32 pImgNtHeader=NULL;
	PIMAGE_SECTION_HEADER pImgSecHeader=NULL;
	HANDLE m_hFile = 0;
	LPVOID m_lpBase=0;
	HANDLE m_hMap=0;
	CString m_strPath;
	CString m_hFileName;
	int nDeepIndex1 = 0;
	int nDeepIndex2 = 0;

public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnOpen();
	afx_msg void OnClose();
	afx_msg void OnExit();
	afx_msg void OnNMClickTree(NMHDR* pNMHDR, LRESULT* pResult);
	VOID ShowResTree_DFS(DWORD dwDeep, IMAGE_RESOURCE_DIRECTORY* pNewIRDE,
		IMAGE_RESOURCE_DIRECTORY* pOriIRDE, HTREEITEM hImgTree);
	VOID ShowRes_DFS(DWORD dwDeep, IMAGE_RESOURCE_DIRECTORY* pNewIRDE,
		IMAGE_RESOURCE_DIRECTORY* pOriIRDE, int nIndexDeep1, int nIndexDeep2);

public:

	BOOL isPE();
	void ShowPEView(int Startaddr, int EndAddr,int interval);
	void OpenPEFile();
	void ShowTreelist();
	void DelListAll();
	char* HexDump(char* buf, int len, int addr);
	void ShowListIDH();
	void	ShowListSignature();
	void	ShowListIFH(); 
	 void ShowListIOH();
	 void ShowListTable();
	 void UpdataListDosHeader();  //Dos 表头
	 void UpdataListNTHeader();  //NT表头
	 void ShowImgListSection();//节表
	 void ShowImgImportTableList();//输入表
	 void ShowImgExportTable();//导入表
	 void ShowImgIETFunction();//导入函数
	 void ShowRelocationDirectory();//重定位表
	 void ShowResourceDirectory();// 资源表
	 void ShowDebugDirectory();//调试信息表
	 void ShowTLSDirectory();//TLS表
	 void ShowResource(int nIndexDeep1, int nIndexDeep2); //显示资源条目
	 void LoadSafeSkin();
	 int PEEntryPoint;
	 int m_nFileSize;
	 //转换:
	 DWORD RvaToFoa(PIMAGE_NT_HEADERS pImgNtHdr, DWORD dwRva);
	 CString WhereRva(PIMAGE_NT_HEADERS pImgNtHdr, DWORD dwRva);

public:
	typedef struct csListInfo
	{
		CString Type;
		CString Header;
		CString Description;
	}CSLISTINFO,*pcsListInfo;

	//DOS_HEADER
	map<int,csListInfo>mapIMHdes = {       
			{0,{"WORD","ccagic","Magic number"}},
			{1,{"WORD","e_cblp","Bytes on last page of file"}},
			{2,{"WORD","e_cp","Pages in fil "}},
			{3,{"WORD","e_crlc"," Relocations"}},
			{4,{"WORD","e_cparhdr","Size of header in paragraphs"}},
			{5,{"WORD","e_minalloc","Minimum extra paragraphs needed"}},
			{6,{"WORD","e_maxallo","Maximum extra paragraphs needed"}},
			{7,{"WORD","e_ss"," Initial (relative} SS value"}},
			{8,{"WORD","e_sp"," Initial SP value"}},
			{9,{"WORD","e_csum","Checksum"}},
			{10,{"WORD","e_ip","Initial IP value"}},
			{11,{"WORD","e_cs","Initial (relative} CS value"}},
			{12,{"WORD","e_lfarlc","File address of relocation table"}},
			{13,{"WORD","e_ovno","Overlay number"}},
			{14,{"WORD","e_res","Reserved words"}},
			{15,{"WORD","e_res"," Reserved words"}},
			{16,{"WORD","e_res"," Reserved words"}},
			{17,{"WORD","e_res"," Reserved words"}},
			{18,{"WORD","e_oemid","OEM identifier (for e_oeminfo}"}},
			{19,{"WORD","e_oeminfo","OEM information; e_oemid specific"}},
			{20,{"WORD","e_res2","Reserved words"}},
			{21,{"WORD","e_res2","Reserved words"}},
			{22,{"WORD","e_res2","Reserved words"}},
			{23,{"WORD","e_res2","Reserved words"}},
			{24,{"WORD","e_res2","Reserved words"}},
			{25,{"WORD","e_res2","Reserved words"}},
			{26,{"WORD","e_res2","Reserved words"}},
			{27,{"WORD","e_res2","Reserved words"}},
			{28,{"WORD","e_res2","Reserved words"}},
			{29,{"WORD","e_res2","Reserved words"}},
			{30,{"LONG","e_lfanew", "File address of new exe header"}}
	}; 	
	//FILE_HEADER
	typedef struct csListIFHInfo
	{
		CString Type;
		CString Header;
	}CSLISTIFHINFO, * PCSLISTIFHINFO;

	map<int, csListIFHInfo>mapIFH =
	{
		{0,{"WORD","Machine"}},
		{1,{"WORD","NumberOfSections"}},
		{2,{"DWORD"," TimeDateStamp"}},
		{3,{"DWORD"," PointerToSymbolTable"}},
		{4,{"DWORD"," NumberOfSymbols"}},
		{5,{"WORD","SizeOfOptionalHeader"}},
		{6,{"WORD","Characteristics"}}
	};

	//FILE_HEADER DATA Directories
	map<int, CString>mapIOHDD =
	{
			{0,"Export Directory RVA"},
			{1,"Export Directory Size"},
			{2,"Import Directory RVA"},
			{3,"Import Directory Size"},
			{4,"Resource Directory RVA"},
			{5,"Resource Directory Size"},
			{6,"Exception Directory RVA"},
			{7,"Exception Directory Size"},
			{8,"Security Directory RVA"},
			{9,"Security Directory Size"},
			{10,"Base Relocation Table RVA"},
			{11,"Base Relocation Table Size"},
			{12,"Debug Directory RVA"},
			{13,"Debug Directory Size"},
			{14,"Architecture Specific Data RVA"},
			{15,"Architecture Specific Data Size"},
			{16,"RVA of GP"},
			{17,"RVA of GP"},
			{18,"TLS Directory RVA"},
			{19,"TLS Directory Size"},
			{20,"Load Configuration Directory RVA"},
			{21,"Load Configuration Directory Size"},
			{22,"Bound Import Directory in headers RVA"},
			{23,"Bound Import Directory in headers Size"},
			{24,"Import Address Table RVA"},
			{25,"Import Address Table Size"},
			{26,"Delay Load Import Descriptors RVA"},
			{27,"Delay Load Import Descriptors Size"},
			{28,"COM Runtime descriptor RVA"},
			{29,"COM Runtime descriptor Size"}
		};
	
	//option header
	map<int, csListIFHInfo>mapIOH =
	{
		{0,{"WORD","Magic"}},
		{1,{"BYTE","MajorLinkerVersion"}},
		{2,{"BYTE","MinorLinkerVersion"}},
		{3,{"DWORD","SizeOfCode"}},
		{4,{"DWORD","SizeOfInitializedData"}},
		{5,{"DWORD","SizeOfUninitializedData"}},
		{6,{"DWORD","AddressOfEntryPoint"}},
		{7,{"DWORD","BaseOfCode"}},
		{8,{"DWORD","BaseOfData"}},
		{9,{"DWORD","ImageBase"}},
		{10,{"DWORD","SectionAlignment"}},
		{11,{"DWORD","FileAlignment"}},
		{12,{"WORD","MajorOperatingSystemVersion"}},
		{13,{"WORD","MinorOperatingSystemVersion"}},
		{14,{"WORD","MajorImageVersion"}},
		{15,{"WORD","MinorImageVersion"}},
		{16,{"WORD","MajorSubsystemVersion"}},
		{17,{"WORD","MinorSubsystemVersion"}},
		{18,{"DWORD","Win32VersionValue"}},
		{19,{"DWORD","SizeOfImage"}},
		{20,{"DWORD","SizeOfHeaders"}},
		{21,{"DWORD","CheckSum"}},
		{22,{"WORD","Subsystem"}},
		{23,{"WORD","DllCharacteristics"}},
		{24,{"DWORD","SizeOfStackReserve"}},
		{25,{"DWORD","SizeOfStackCommit"}},
		{26,{"DWORD","SizeOfHeapReserve"}},
		{27,{"DWORD","SizeOfHeapCommit"}},
		{28,{"DWORD","LoaderFlags"}},
		{29,{"DWORD","NumberOfRvaAndSizes"}}
	};

	typedef struct csListIEDnfo
	{
		CString Type;
		CString Member;
	}CSLISTIEDINFO, * PCSLISTIEDINFO;

	map<int, csListIEDnfo>mapIED
	{
		{0,{"DWORD","Characteristics"}},
		{1,{"DWORD", "TimeDateStamp"}},
		{2,{"WORD", "MajorVersion"}},
		{3,{"WORD","MinorVersion"}},
		{4,{"DWORD","Name"}},
		{5,{"DWORD","Base"}},
		{6,{"DWORD","NumberOfFunctions"}},
		{7,{"DWORD","NumberOfNames"}},
		{8,{"DWORD","AddressOfFunctions"}},     
		{9,{"DWORD","AddressOfNames"}},        
		{10,{"DWORD","AddressOfNameOrdinals"}}
	};
	//TLS表结构体信息
	typedef struct csListTLS
	{
		CString Type;
		CString Member;
		CString Describe;
	}CSLISTTLS, * PCSLISTTLS;

	map<int, csListTLS>mapTLS
	{
		{0,{"DWORD"," StartAddressOfRawData","数据块开始VA"}},
		{1,{"DWORD", "EndAddressOfRawData","数据块开始VA"}},
		{2,{"DWORD", "AddressOfIndex","索引变量VA"}},
		{3,{"DWORD","AddressOfCallBacks","回调表VA"}},
		{4,{"DWORD","SizeOfZeroFill;","填0大小"}},
		{5,{"DWORD","Characteristics","特征值"}},
	};
	//Debug表结构信息
	typedef struct csListDebug
	{
		CString Type;
		CString Member;
		CString Describe;
	}CSLISTDEBUG, * PCSLISTDEBUG;

	map<int, csListTLS>mapDebug
	{
		{0,{"DWORD"," StartAddressOfRawData","数据块开始VA"}},
		{1,{"DWORD", "EndAddressOfRawData","数据块结束VA"}},
		{2,{"DWORD", "AddressOfIndex","索引变量VA"}},
		{3,{"DWORD","AddressOfCallBacks","回调表VA"}},
		{4,{"DWORD","SizeOfZeroFill;","填0大小"}},
		{5,{"DWORD","Characteristics","特征值"}},
	};

	afx_msg void OnInjectModEntry();
	afx_msg void OnAddSection();
	afx_msg void OnCalVirtual();
	afx_msg void OnMemDump();
	afx_msg void OnDblclkLstPe(NMHDR* pNMHDR, LRESULT* pResult);

	//Resourse表结构信息
	typedef struct csListResourse
	{
		CString Type;
		CString Member;
	}CSLISTRESOURSE, * PCSLISTRESUORSE;

	map<int, csListResourse>mapResourse
	{
		{0,{"DWORD","Characteristics"}},
		{1,{"DWORD", "TimeDateStamp"}},
		{2,{"WORD", "MajorVersion"}},
		{3,{"WORD","MinorVersion"}},
		{4,{"WORD","NumberOfNamedEntries"}},
		{5,{"WORD","NumberOfIdEntries"}},
	};

	map<int, CString>mapResType
	{
		{0,"0"},
		{1,"光标"},
		{2,"位图"},
		{3,"图标"},
		{4,"菜单"},
		{5,"对话框"},
		{6,"字符串列表"},
		{7,"字体目录"},
		{8,"字体"},
		{9,"快捷键"},
		{10,"非格式化资源"},
		{11,"消息列表"},
		{12,"光标组"},
		{13,"13"},
		{14,"图标组"},
		{15,"15"},
		{16,"版本信息"},
		{24,"24"},
	};
};
			 
	//heade e_lfanew;  r
/*

typedef struct _IMAGE_DOS_HEADER {      // DOS .EXE header
	WORD   ccagic;                     // Magic number
	WORD   e_cblp;                      // Bytes on last page of file
	WORD   e_cp;                        // Pages in file
	WORD   e_crlc;                      // Relocations
	WORD   e_cparhdr;                   // Size of header in paragraphs
	WORD   e_minalloc;                  // Minimum extra paragraphs needed
	WORD   e_maxalloc;                  // Maximum extra paragraphs needed
	WORD   e_ss;                        // Initial (relative) SS value
	WORD   e_sp;                        // Initial SP value
	WORD   e_csum;                      // Checksum
	WORD   e_ip;                        // Initial IP value
	WORD   e_cs;                        // Initial (relative) CS value
	WORD   e_lfarlc;                    // File address of relocation table
	WORD   e_ovno;                      // Overlay number
	WORD   e_res[4];                    // Reserved words
	WORD   e_oemid;                     // OEM identifier (for e_oeminfo)
	WORD   e_oeminfo;                   // OEM information; e_oemid specific
	WORD   e_res2[10];                  // Reserved words
	LONG    e_lfanew;                    // File address of new exe header
  } IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;
*/

/*
typedef struct _IMAGE_FILE_HEADER {
	WORD    Machine;
	WORD    NumberOfSections;
	DWORD  TimeDateStamp;
	DWORD  PointerToSymbolTable;
	DWORD  NumberOfSymbols;
	WORD    SizeOfOptionalHeader;
	WORD    Characteristics;
} IMAGE_FILE_HEADER, * PIMAGE_FILE_HEADER;

typedef struct _IMAGE_OPTIONAL_HEADER {
	WORD      Magic;
	BYTE         MajorLinkerVersion;
	BYTE         MinorLinkerVersion;
	DWORD   SizeOfCode;
	DWORD   SizeOfInitializedData;
	DWORD   SizeOfUninitializedData;
	DWORD   AddressOfEntryPoint;
	DWORD   BaseOfCode;
	DWORD   BaseOfData;
	DWORD   ImageBase;
	DWORD   SectionAlignment;
	DWORD   FileAlignment;
	WORD      MajorOperatingSystemVersion;
	WORD      MinorOperatingSystemVersion;
	WORD      MajorImageVersion;
	WORD      MinorImageVersion;
	WORD      MajorSubsystemVersion;
	WORD      MinorSubsystemVersion;
	DWORD   Win32VersionValue;
	DWORD   SizeOfImage;
	DWORD   SizeOfHeaders;
	DWORD   CheckSum;
	WORD      Subsystem;
	WORD      DllCharacteristics;
	DWORD   SizeOfStackReserve;
	DWORD   SizeOfStackCommit;
	DWORD   SizeOfHeapReserve;
	DWORD   SizeOfHeapCommit;
	DWORD   LoaderFlags;
	DWORD   NumberOfRvaAndSizes;
	IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
} IMAGE_OPTIONAL_HEADER32, * PIMAGE_OPTIONAL_HEADER32;
*/

/*
typedef struct _IMAGE_EXPORT_DIRECTORY {
	DWORD   Characteristics;
	DWORD   TimeDateStamp;
	WORD      MajorVersion;
	WORD      MinorVersion;
	DWORD   Name;
	DWORD   Base;
	DWORD   NumberOfFunctions;
	DWORD   NumberOfNames;
	DWORD   AddressOfFunctions;     // RVA from base of image
	DWORD   AddressOfNames;         // RVA from base of image
	DWORD   AddressOfNameOrdinals;  // RVA from base of image
} IMAGE_EXPORT_DIRECTORY, * PIMAGE_EXPORT_DIRECTORY;
*/



/*
typedef struct _IMAGE_TLS_DIRECTORY32 {
	DWORD   StartAddressOfRawData;
	DWORD   EndAddressOfRawData;
	DWORD   AddressOfIndex;             // PDWORD
	DWORD   AddressOfCallBacks;         // PIMAGE_TLS_CALLBACK *
	DWORD   SizeOfZeroFill;
	union {
		DWORD Characteristics;
		struct {
			DWORD Reserved0 : 20;
			DWORD Alignment : 4;
			DWORD Reserved1 : 8;
		} DUMMYSTRUCTNAME;
	} DUMMYUNIONNAME;

} IMAGE_TLS_DIRECTORY32;

*/

/*
typedef struct _IMAGE_DEBUG_DIRECTORY {
    DWORD   Characteristics;
    DWORD   TimeDateStamp;
    WORD    MajorVersion;
    WORD    MinorVersion;
    DWORD   Type;
    DWORD   SizeOfData;
    DWORD   AddressOfRawData;
    DWORD   PointerToRawData;
} IMAGE_DEBUG_DIRECTORY, *PIMAGE_DEBUG_DIRECTORY;

*/

/*
typedef struct _IMAGE_RESOURCE_DIRECTORY {
	DWORD   Characteristics;
	DWORD   TimeDateStamp;
	WORD    MajorVersion;
	WORD    MinorVersion;
	WORD    NumberOfNamedEntries;
	WORD    NumberOfIdEntries;
	//  IMAGE_RESOURCE_DIRECTORY_ENTRY DirectoryEntries[];
} IMAGE_RESOURCE_DIRECTORY, * PIMAGE_RESOURCE_DIRECTORY;

*/