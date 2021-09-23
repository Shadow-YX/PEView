// CMemDump.cpp: 实现文件
//

#include "pch.h"
#include "MyPEView.h"
#include "CMemDump.h"
#include "afxdialogex.h"
#include <TlHelp32.h> 

// CMemDump 对话框

IMPLEMENT_DYNAMIC(CMemDump, CDialogEx)

CMemDump::CMemDump(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_DUMP, pParent)
{

}

CMemDump::~CMemDump()
{
}

void CMemDump::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMemDump, CDialogEx)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDOK, &CMemDump::OnBnClickedOk)
END_MESSAGE_MAP()


// CMemDump 消息处理程序
//CMyTestDlg.cpp
void CMemDump::OnDropFiles(HDROP hDropInfo) {
	// 通过设置iFiles参数为0xFFFFFFFF,可以取得当前拖动的文件数量，
	// 当设置为0xFFFFFFFF,函数间忽略后面两个参数。
	int DropCount = DragQueryFile(hDropInfo, -1, NULL, 0);//取得被拖动文件的数目
	// 循环依次取得拖动文件的File Name信息

	for (int i = 0; i < DropCount; i++)
	{

		DragQueryFile(hDropInfo, i, filePath, MAX_PATH);//获得拖曳的第i个文件的文件名
		//此时 filePath已经获取到了文件路径
		//do something...
	}
	//MFB_SELFILE
	SetDlgItemText(MFB_SELFILE, filePath);
	m_DumpPath.Format("%s", filePath);
	//拖放结束后,释放内存
	DragFinish(hDropInfo);
	CDialog::OnDropFiles(hDropInfo);
}



void CMemDump::OnBnClickedOk()
{
	CString csImageBase;
	DWORD dwImageBase;//建议加载基址
	WORD wSections;//节的数目
	DWORD dwSectionAddress;//节表的首地址
	DWORD dwNTHeader;//PE头首地址
	WORD wSizeOfOptionalHeader;//扩展头的长度
	DWORD dwSizeOfHeaders;//所有头加节表的大小

	if (filePath == NULL)
	{
		AfxMessageBox("请输入需要dump文件路径/或拖入dump的文件");
		return;	}


	GetDlgItemText(MFB_INPORT, m_ImportPath);  
	if (m_ImportPath.GetLength()==0)
	{
		AfxMessageBox("请输入dump文件路径");
		return;
	}
	m_ImportPath = m_ImportPath + "\\dump.exe";//导出的文件名和路径

	//获取输入的镜像基址
	GetDlgItemText(MFB_IMAGEBASE, csImageBase);
	::sscanf(csImageBase.GetBuffer(), "%08x", &dwImageBase);

	//启动待dump进程
	UINT nRet = ::WinExec(
		filePath, // 路径名
		SW_SHOW
	);
	//获取进程pid
	if (!GetPIDByName(filePath))
	{
		AfxMessageBox("进程不存在,进程打开失败");
		return;
	}
	//获取进程句柄
	m_hProc = ::OpenProcess(PROCESS_ALL_ACCESS, NULL, m_pID);

	if (m_hProc == NULL)
	{
		AfxMessageBox("进程不存在,进程打开失败");
		return;
	}

	m_dwImageBase = dwImageBase;

	DWORD dwNumberOfBytesRead;
	DWORD dwNumberOfBytesWritten;	
	ReadProcessMemory(m_hProc, (LPVOID)(m_dwImageBase + 0x3C), &dwNTHeader, sizeof(dwNTHeader), &dwNumberOfBytesRead);
	ReadProcessMemory(m_hProc, (LPVOID)(m_dwImageBase + dwNTHeader + 0x6), &wSections, sizeof(wSections), &dwNumberOfBytesRead);
	ReadProcessMemory(m_hProc, (LPVOID)(m_dwImageBase + dwNTHeader + 0x14), &wSizeOfOptionalHeader, sizeof(wSizeOfOptionalHeader), &dwNumberOfBytesRead);

	dwSectionAddress = dwNTHeader + 4 + sizeof(IMAGE_FILE_HEADER) + wSizeOfOptionalHeader;
	ReadProcessMemory(m_hProc, (LPVOID)(m_dwImageBase + dwNTHeader + 0x54), &dwSizeOfHeaders, sizeof(dwSizeOfHeaders), &dwNumberOfBytesRead);

	m_hFile = CreateFile(m_ImportPath.GetBuffer(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox("Dump文件创建失败");
		return;
	}
	
	//先写入PE头
	char* pTmp = new char[dwSizeOfHeaders];
	memset(pTmp, 0, dwSizeOfHeaders);
	ReadProcessMemory(m_hProc, (LPVOID)(m_dwImageBase), pTmp, dwSizeOfHeaders, &dwNumberOfBytesRead);
	WriteFile(m_hFile, pTmp, dwSizeOfHeaders, &dwNumberOfBytesWritten, NULL);
	

	DWORD dwSum = 0;
	dwSum += dwSizeOfHeaders;
	delete[] pTmp;

	//循环写入节
	for (int i = 0; i < wSections; i++)
	{
		for (int j = 0; j < wSections; j++)
		{
			IMAGE_SECTION_HEADER sh;
			::ReadProcessMemory(m_hProc, (LPVOID)(m_dwImageBase + dwSectionAddress + j * 40), &sh, sizeof(sh), &dwNumberOfBytesRead);
			if (sh.PointerToRawData == dwSum)
			{
				char* pTmp = new char[sh.SizeOfRawData];
				memset(pTmp, 0, sh.SizeOfRawData);
				ReadProcessMemory(m_hProc, (LPVOID)(m_dwImageBase + sh.VirtualAddress), pTmp, sh.SizeOfRawData, &dwNumberOfBytesRead);
				WriteFile(m_hFile, pTmp, sh.SizeOfRawData, &dwNumberOfBytesWritten, NULL);
				delete[] pTmp;
				dwSum += sh.SizeOfRawData;
			}
		}
	}
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFile);
	}

	//关闭dump的进程
	TerminateProcess(m_hProc, 0);
	AfxMessageBox("Deump文件成功");
	CDialogEx::OnOK();
}


bool CMemDump::GetPIDByName(TCHAR* processName)
{
	// 创建系统快照
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	PROCESSENTRY32 ps;
	ZeroMemory(&ps, sizeof(PROCESSENTRY32));
	ps.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hSnapshot, &ps))
	{
		return false;
	}

	CString str;
	str.Format("%s", processName);

	int index = str.ReverseFind('\\');
	CString FileName = str.Mid(index + 1);

	do
	{
		if (lstrcmpi(ps.szExeFile, FileName) == 0)
		{
			// 保存进程ID
			m_pID = ps.th32ProcessID;
			CloseHandle(hSnapshot);
			return true;
		}
	} while (Process32Next(hSnapshot, &ps));

	return false;
}
