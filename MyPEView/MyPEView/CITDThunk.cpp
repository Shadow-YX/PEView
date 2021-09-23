// CITDThunk.cpp: 实现文件
//

#include "pch.h"
#include "MyPEView.h"
#include "CITDThunk.h"
#include "afxdialogex.h"


// CITDThunk 对话框

IMPLEMENT_DYNAMIC(CITDThunk, CDialogEx)

CITDThunk::CITDThunk(CWnd* pParent /*=nullptr*/)
	: CDialogEx(DLG_THUNK, pParent)
{

}

CITDThunk::~CITDThunk()
{
}

void CITDThunk::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, LST_THUNK, m_lstThunk);
}


BEGIN_MESSAGE_MAP(CITDThunk, CDialogEx)
END_MESSAGE_MAP()


// CITDThunk 消息处理程序


BOOL CITDThunk::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_lstThunk.InsertColumn(0, "保留", LVCFMT_CENTER, 0);
	m_lstThunk.InsertColumn(1, "ThunkRVA", LVCFMT_CENTER, 150);
	m_lstThunk.InsertColumn(2, "Hint", LVCFMT_CENTER, 120);
	m_lstThunk.InsertColumn(3 ,"API名称", LVCFMT_CENTER, 250);
	m_lstThunk.DeleteColumn(0);

	CString DllName;
	CString APIName;
	CString ThunkOffset;
	CString ThunkRva;
	CString ForwardString;
	CString Hint;
	CString AddressOfData;

	//拿输入表 Foa(NtHeader->OptHdr.DataDirectory.VitualAddress)
	PIMAGE_IMPORT_DESCRIPTOR m_pImgIID = NULL;
	m_pImgIID = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)m_lpBase + RvaToFoa(m_pImgNtHeader, m_pImgNtHeader->
		OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress));

	while (m_pImgIID->Name)
	{

		DllName.Format("%s", (DWORD)m_lpBase + RvaToFoa(m_pImgNtHeader, m_pImgIID->Name));
		if (DllName== SelDllName)
		{
			//遍历OriFiststThunk
			PIMAGE_THUNK_DATA pITD =(PIMAGE_THUNK_DATA) ((DWORD)m_lpBase + RvaToFoa(m_pImgNtHeader,m_pImgIID->OriginalFirstThunk));
		
			int nItem = 0;
			DWORD dwOffset = 0;
			while (pITD->u1.Ordinal)
			{
				//判断导入方式
				if (IMAGE_SNAP_BY_ORDINAL32(pITD->u1.Ordinal)) //判断最高位是否为1 &0x800000000
				{
					ThunkRva.Format("%08x", m_pImgIID->OriginalFirstThunk + dwOffset);
					dwOffset += 4;
					APIName.Format("%08X", IMAGE_ORDINAL(pITD->u1.Ordinal));

					//  序号导入
					ForwardString.Format("%08x", pITD->u1.Ordinal);
					m_lstThunk.InsertItem(nItem, ThunkRva);
					m_lstThunk.SetItemText(nItem, 1, "序号:");
					m_lstThunk.SetItemText(nItem, 2, APIName);
				}
				else
				{
					//不是序号，这个值是RVA指向IMAGE_IMPORT_BY_NAME
					ThunkRva.Format("%08x", m_pImgIID->OriginalFirstThunk + dwOffset);
					dwOffset += 4;

					PIMAGE_IMPORT_BY_NAME pIIBN =(PIMAGE_IMPORT_BY_NAME)( (DWORD)m_lpBase + RvaToFoa(m_pImgNtHeader, pITD->u1.Function));
					Hint.Format("%04x",pIIBN->Hint );
					//函数名导入
					APIName.Format("%s", pIIBN->Name);

					m_lstThunk.InsertItem(nItem, ThunkRva);
					//m_lstThunk.SetItemText(nItem, ++nColdx, AddressOfData);
					//m_lstThunk.SetItemText(nItem, ++nColdx, ForwardString);
					m_lstThunk.SetItemText(nItem,1,Hint);
					m_lstThunk.SetItemText(nItem,2, APIName);
				}
				nItem++;
				pITD++;
			}

		}

		m_pImgIID++;
	}


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CITDThunk::SetPEParameter(CString DllName,LPVOID lpBase, 
	 PIMAGE_NT_HEADERS32 pImgNtHeader,
	PIMAGE_SECTION_HEADER pImgSecHeader)
{
	SelDllName = DllName;
	m_pImgNtHeader = pImgNtHeader;
	m_lpBase = lpBase;
	m_pImgSecHeader = pImgSecHeader;
}

DWORD CITDThunk::RvaToFoa(PIMAGE_NT_HEADERS pImgNtHdr, DWORD dwRva)
{
	/*
1.判断RVA地址所属的节
*/
	DWORD dwFOA = 0;

	//获取节表起始位置
	m_pImgSecHeader = IMAGE_FIRST_SECTION(m_pImgNtHeader);
	//获取节表数量
	WORD wNum =m_pImgNtHeader->FileHeader.NumberOfSections;
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


