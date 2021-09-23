// CInjectEndFile.cpp: 实现文件
//

#include "pch.h"
#include "MyPEView.h"
#include "CInjectEndFile.h"
#include "afxdialogex.h"
#include "MyPEViewDlg.h"


// CInjectEndFile 对话框

IMPLEMENT_DYNAMIC(CInjectEndFile, CDialogEx)

CInjectEndFile::CInjectEndFile(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CInjectEndFile::~CInjectEndFile()
{
}

void CInjectEndFile::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInjectEndFile, CDialogEx)
	ON_BN_CLICKED(IDOK, &CInjectEndFile::OnBnClickedOk)
END_MESSAGE_MAP()


// CInjectEndFile 消息处理程序


void CInjectEndFile::OnBnClickedOk()
{
	CString  csCode;
	GetDlgItemText(EDT_INJECTHEX, csCode);


  //文件末尾追加代码
	DWORD EndOffset = SetFilePointer(m_File, 0, NULL, FILE_END);
	if(EndOffset==-1)
    {
        AfxMessageBox("SetFilePointer error\n");
    }
	char buff[128] = { 0 };
	AsciiToBcd(csCode, (unsigned char*)buff, csCode.GetLength());
	DWORD dwWrite;
    if (!WriteFile(m_File, &buff, strlen(buff), &dwWrite, NULL))
    {
		AfxMessageBox("WriteFile error\n");
    }

	//获取新的入口地址
	DWORD dwNewEntryAddress = (DWORD)m_Base + EndOffset;

	//设置程序入口点为新的入口地址
	int nEntryPos =m_PEEtry+ 40;
	SetFilePointer(m_File, nEntryPos, NULL, FILE_BEGIN);
	char pBuffer[30] = { 0 };
	ChangeDwordToString(dwNewEntryAddress, pBuffer);
	WriteFile(m_File, pBuffer, 4, &dwWrite, NULL);

	AfxMessageBox("注入成功!");

	CDialogEx::OnOK();
}

void CInjectEndFile::SetAtt(HANDLE m_hFile,int m_lpBase,int PEEntryPoint, int m_nFileSize)
{
	m_File=m_hFile;
	m_Base = m_lpBase;
	m_fileSize = m_nFileSize;
	m_PEEtry = PEEntryPoint;

	return;
}

int chrToNum(int u)
{
	u = toupper(u);
	if ((u >= 'A' && u <= 'F'))
	{
		u = u - 'A' + 10;
	}
	else if (u >= '0' && u <= '9')
	{
		u = u - '0';
	}
	else
	{
		u = 0;
	}
	return u;
}

void CInjectEndFile ::AsciiToBcd(CString cs, unsigned char* p, int nlength/*字符串长度*/)
{

	while (cs.GetLength() < nlength)
		cs = "0" + cs;

	for (int i = 0; i < nlength / 2; i++)
		//*(p+i)=(cs.GetAt(i*2)-0x30)*16+(cs.GetAt(2*i+1)-0x30);
		*(p + i) = chrToNum(cs.GetAt(i * 2)) * 16 + chrToNum(cs.GetAt(2 * i + 1));
}

void CInjectEndFile::ChangeDwordToString(DWORD dwValue, TCHAR* szBuf)
{
	BYTE bValue;
	BYTE k;
	for (k = 0; k < 8; ++k)
	{
		bValue = (BYTE)((dwValue & 0xF0000000) >> 28);
		if (bValue == 0)
		{
			dwValue <<= 4;
			continue;
		}
		if (bValue < 10)
		{
			*szBuf = bValue + '0';
		}
		else
		{
			*szBuf = (bValue - 10) + 'A';
		}
		dwValue <<= 4;
		szBuf++;
	}
	*szBuf = '\0';
}