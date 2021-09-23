// CLocationCal.cpp: 实现文件
//

#include "pch.h"
#include "MyPEView.h"
#include "CLocationCal.h"
#include "afxdialogex.h"


// CLocationCal 对话框

IMPLEMENT_DYNAMIC(CLocationCal, CDialogEx)

CLocationCal::CLocationCal(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_CALCULATOR, pParent)
{

}

CLocationCal::~CLocationCal()
{
}

void CLocationCal::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, EDT_VA, m_edtVA);
	DDX_Control(pDX, EDT_RVA, m_edtRVA);
}

BEGIN_MESSAGE_MAP(CLocationCal, CDialogEx)
	//ON_BN_CLICKED(BTN_CALVA, &CLocationCal::OnBnClickedCalva)
	//ON_BN_CLICKED(BTN_CALRVA, &CLocationCal::OnBnClickedCalrva)
	ON_BN_CLICKED(IDOK, &CLocationCal::OnBnClickedOk)
END_MESSAGE_MAP()


void CLocationCal::SetPEParameter(HANDLE File, LPVOID Base, PIMAGE_DOS_HEADER DosHeader,
	PIMAGE_NT_HEADERS32 NtHeader, PIMAGE_SECTION_HEADER SecHeader)
{
	m_nFile = File;
	m_lpBase = Base;
	pImgDosHeader = DosHeader;
	pImgNtHeader = NtHeader;
	pImgSecHeader = SecHeader;

	return;
}


void CLocationCal::HexStrToInt(char* szHex, DWORD* pdwHexVal)
{
	char *pCH,c;
	DWORD dwVal = 0,dw;
	pCH = szHex;

	while (*pCH)
	{
		c = toupper(*pCH++);
		if (c >= 'A' && c <= 'F')
			dw = (DWORD)c - ((DWORD)'A' - 10);
		else if (c >= '0' && c <= '9')
			dw = (DWORD)c - (DWORD)'0';
		else
			return;
		dwVal = (dwVal << 4) + dw;
	}
	*pdwHexVal = dwVal;
}

/*
void CLocationCal::OnBnClickedCalva()
{
	m_edtVA.ShowWindow(SW_SHOW);
	m_edtVA.IsWindowVisible();     

	m_edtRVA.ShowWindow(FALSE);
	m_edtRVA.IsWindowVisible();
}


void CLocationCal::OnBnClickedCalrva()
{
	m_edtRVA.ShowWindow(SW_SHOW);
	 m_edtVA.IsWindowVisible();

	m_edtVA.ShowWindow(FALSE);
	m_edtVA.IsWindowVisible();
}
*/

void CLocationCal::OnBnClickedOk()
{
	m_edtVA.ShowWindow(SW_SHOW);
	m_edtRVA.ShowWindow(SW_SHOW);
	m_edtVA.IsWindowVisible();
	m_edtRVA.IsWindowVisible();

	char szVA[10] = { 0 };
	char szRVA[10] = { 0 };

	GetDlgItemText(EDT_VA, szVA,MAX_PATH);
	GetDlgItemText(EDT_RVA, szRVA,MAX_PATH);

	DWORD dwRVA=0;
	DWORD dwFOA=0;


	//RVA->FOA

	if (szRVA != NULL)
	{
		/*
		1.判断RVA地址所属的节
		*/
		DWORD dwFOA = 0;
		HexStrToInt(szRVA, &dwRVA);
		//获取节表起始位置
		pImgSecHeader = IMAGE_FIRST_SECTION(pImgNtHeader);
		//获取节表数量
		WORD wNum = pImgNtHeader->FileHeader.NumberOfSections;
		//获取节对齐后的长度
		WORD wLenSection = pImgSecHeader->SizeOfRawData;
		//   判断 rva < = rva  <rva+len
		WORD i=0;
		while(i++<wNum)
		{
			//获取节对齐后的长度
			WORD wLenSection = pImgSecHeader->SizeOfRawData;
			if (dwRVA >= pImgSecHeader->VirtualAddress && dwRVA < pImgSecHeader->VirtualAddress + wLenSection)
			{
				break;
			}
			pImgSecHeader++;
		}
		/*
		2.判断是否找到所属的节
		*/
		if (i <= wNum)
		{
			dwFOA = dwRVA - pImgSecHeader->VirtualAddress + pImgSecHeader->PointerToRawData;
		}

		if (dwFOA != 0)
		{
			//显示FOA
			char szFOA[10] = { 0 };
			wsprintf(szFOA, "%08X", dwFOA);
			SetDlgItemText(EDT_FOA, szFOA);
			//显示VA		
			CString strBase;
			strBase.Format("%08X", pImgNtHeader->OptionalHeader.ImageBase+dwRVA);	
			SetDlgItemText(EDT_VA, strBase);
			//显示所在的节
			CString  strSectionName;
			strSectionName.Format("%s", pImgSecHeader->Name);
			SetDlgItemText(EDT_CALTPYE, strSectionName);

			//显示hex
			DWORD dwhex = (DWORD)m_lpBase + dwFOA;
			char szbuff[20] = { 0 };
			TCHAR szDataMsg[128] = { 0 };
			memcpy_s(szbuff, 10, (char*)m_lpBase + dwFOA , 10);
			for (int j = 0;j<10; j++)
			{
				sprintf(szDataMsg, "%s%02x ", szDataMsg, (unsigned char)szbuff[j]);		//Hex  data
			}
			CString strDateHex;
			strDateHex.Format("%s", szDataMsg);

			SetDlgItemText(EDT_CALHEX, strDateHex);
			return;
		}
		else
		{
			AfxMessageBox("RVA超出范围");
		}
	}
	else if (szVA != NULL)
	{
		return;
	}
	else
	{
		AfxMessageBox("No Input");
		return;
	}
	

	CDialogEx::OnOK();
}
