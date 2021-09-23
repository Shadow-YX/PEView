// CIBRLItem.cpp: 实现文件
//

#include "pch.h"
#include "MyPEView.h"
#include "CIBRLItem.h"
#include "afxdialogex.h"


// CIBRLItem 对话框

IMPLEMENT_DYNAMIC(CIBRLItem, CDialogEx)

CIBRLItem::CIBRLItem(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_PBRLITEM, pParent)
{

}

CIBRLItem::~CIBRLItem()
{
}

void CIBRLItem::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, LIST_ITEM, m_lstItem);
}


BEGIN_MESSAGE_MAP(CIBRLItem, CDialogEx)
END_MESSAGE_MAP()


// CIBRLItem 消息处理程序


BOOL CIBRLItem::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_lstItem.InsertColumn(0, "保留", LVCFMT_CENTER, 0);
	m_lstItem.InsertColumn(1, "索引", LVCFMT_CENTER, 120);
	m_lstItem.InsertColumn(2, "RVA", LVCFMT_CENTER, 120);
	m_lstItem.InsertColumn(3, "偏移", LVCFMT_CENTER, 120);
	m_lstItem.InsertColumn(4, "类型", LVCFMT_CENTER, 120);
	m_lstItem.DeleteColumn(0);

	PIMAGE_BASE_RELOCATION pIBRL = (PIMAGE_BASE_RELOCATION)((DWORD)m_lpBase + RvaToFoa(m_pImgNtHeader,
		m_pImgNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress));

	CString csIndex;
	CString csLRVA;
	CString csOffset;
	CString csType;
	CString csFAR;

	int nItem = 0;
	while (pIBRL->SizeOfBlock && pIBRL->VirtualAddress)
	{
		CString RVA;

		RVA.Format("%08x", pIBRL->VirtualAddress);
		DWORD sizeblock =(pIBRL->SizeOfBlock - 8) / 2;

		if (RVA == csRVA)
		{
			for (DWORD i = 0; i < sizeblock; i++)
			{
				WORD addr = *((WORD*)((char*)pIBRL+ 8) + i);

				DWORD dwOffset= pIBRL->VirtualAddress +(addr&0x0FFF);  // 低四位是偏移地址
				WORD wType=addr>>12;  //高四位是有效判

				if (wType != 0)
				{
					csIndex.Format("%d", nItem + 1);
					csLRVA.Format("%08x", dwOffset);
					csOffset.Format("%08x", RvaToFoa(m_pImgNtHeader, dwOffset));
					csType.Format("HIGHLOW (%d)", wType);
					csFAR.Format("%08x", pIBRL[i+1]);

					m_lstItem.InsertItem(nItem, csIndex);
					m_lstItem.SetItemText(nItem, 1, csLRVA);
					m_lstItem.SetItemText(nItem, 2, csOffset);
					m_lstItem.SetItemText(nItem, 3, csType);
				}
				else
				{
					csIndex.Format("%d", nItem + 1);
					csType.Format("ABSOLUTE (%d)", wType);

					m_lstItem.InsertItem(nItem, csIndex);
					m_lstItem.SetItemText(nItem, 1, "-");
					m_lstItem.SetItemText(nItem, 2, "-");
					m_lstItem.SetItemText(nItem, 3, csType);
				}
				nItem++;
	
			}
			return TRUE;
		}
		pIBRL = PIMAGE_BASE_RELOCATION((char*)pIBRL + pIBRL->SizeOfBlock);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CIBRLItem::SetPEParameter(CString cRVA, LPVOID lpBase,
	PIMAGE_NT_HEADERS32 pImgNtHeader, PIMAGE_SECTION_HEADER pImgSecHeader)
{
	csRVA=cRVA;
	m_pImgNtHeader = pImgNtHeader;
	m_lpBase = lpBase;
	m_pImgSecHeader = pImgSecHeader;
}

DWORD CIBRLItem::RvaToFoa(PIMAGE_NT_HEADERS pImgNtHdr, DWORD dwRva)
{
	/*
1.判断RVA地址所属的节
*/
	DWORD dwFOA = 0;

	//获取节表起始位置
	m_pImgSecHeader = IMAGE_FIRST_SECTION(m_pImgNtHeader);
	//获取节表数量
	WORD wNum = m_pImgNtHeader->FileHeader.NumberOfSections;
	//获取节对齐后的长度
	DWORD wLenSection = m_pImgSecHeader->SizeOfRawData;
	//   判断 rva < = rva  <rva+len
	WORD i = 0;
	while (i++ < wNum)
	{
		//获取节对齐后的长度
		DWORD wLenSection = m_pImgSecHeader->SizeOfRawData;
		if (dwRva >= m_pImgSecHeader->VirtualAddress && dwRva < m_pImgSecHeader->VirtualAddress + wLenSection)
		{
			break;
		}
		m_pImgSecHeader++;
	}
	/*
	2.判断是否找到所属的节
	*/
	if (i <= wNum)
	{
		dwFOA = dwRva - m_pImgSecHeader->VirtualAddress + m_pImgSecHeader->PointerToRawData;
	}

	return dwFOA;
}
