// ZPByPassAllDlg.cpp : implementation file
//

#include "User.h"
#include "stdafx.h"
#include <stdio.h>
#include "ZPByPassAll.h"
#include "ZPByPassAllDlg.h"
#include "Shlwapi.h"
#include "PE.h"

extern "C" DWORD ShellStart;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//Start
char szFilter[]="Executable Files(*.exe)|*.exe|Dynamic Link Library Files(*.dll)|*.dll|All Fils(*.*)|*.*||";
//User Here
CFileDialog OpenFileDialog(TRUE,NULL,NULL,OFN_HIDEREADONLY,szFilter);

BOOL hSelectFile;
BOOL IsSDK;

//Stub
DWORD hHookStartOffset;
DWORD hHookStartVA;

DWORD hPatchSecStartOffset;
DWORD hPatchSecStartVA;


#define OF_ZP1_6CRC 0x40

 
// CAboutDlg dialog used for App About


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CZPByPassAllDlg dialog




CZPByPassAllDlg::CZPByPassAllDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CZPByPassAllDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CZPByPassAllDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CZPByPassAllDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	//ON_BN_CLICKED(IDC_BUTTON3, &CZPByPassAllDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BTNEXIT, &CZPByPassAllDlg::OnBnClickedBtnexit)
	ON_BN_CLICKED(IDC_BTNGENERATE, &CZPByPassAllDlg::OnBnClickedBtngenerate)
//	ON_BN_CLICKED(IDC_BTNIDC_BTNGENERATE, &CZPByPassAllDlg::OnBnClickedBtnidcBtngenerate)
	ON_BN_CLICKED(IDC_BTNBrowse, &CZPByPassAllDlg::OnBnClickedBtnbrowse)
END_MESSAGE_MAP()


// CZPByPassAllDlg message handlers

BOOL CZPByPassAllDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	SetDlgItemText(IDC_EDITLOG,"E-Mail:cektop@163.com");
	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CZPByPassAllDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CZPByPassAllDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CZPByPassAllDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CZPByPassAllDlg::OnBnClickedBtnexit()
{
	// TODO: Add your control notification handler code here
	ExitProcess(NULL);
}


BOOL ScanHookStart()
{
    BOOL IFFind;
	DWORD SaveEdi;

	
	   
		if (TwoPointerToRawData==0)
		{
            return FALSE;
		}
		_asm
		{
           mov edi,hGlobalFile
		   mov ecx,TwoPointerToRawData
           add edi,ecx
		   mov ecx,TwoSizeOfRawData 
l_Next:
		   xor eax,eax
		   mov ax,word ptr ds:[edi]
		   cmp ax,0xD1FF
		   jne l_IF
		   mov al,byte ptr ds:[edi+2]
		   cmp al,0xE9
		   jne l_ISNext
l_IS:
		   mov esi,TRUE
		   mov SaveEdi,edi
		   jmp l_End
l_ISNext:
		   cmp al,0x68
		   je l_IS
l_IF:
		   mov esi,FALSE
		   inc edi
		   loop l_Next
l_End:
		   mov IFFind,esi
		}
		if (IFFind)
		{
			hHookStartOffset=SaveEdi-(DWORD)hGlobalFile;
		}
		return IFFind;

}

#pragma pack(1)
typedef struct _StubData
{
	BYTE OldData[5];
    DWORD CrcData;
	DWORD NONagOF;
	DWORD CallDecodeOF;
}StubData;

StubData hStubData;

void IniData()
{
  hHookStartVA=ImageBase+((TwoVirtualAddress-TwoPointerToRawData)+hHookStartOffset);
  hPatchSecStartOffset=EndPointerToRawData;
  hPatchSecStartVA=TotalVirtualAddress+ImageBase;
  BYTE Data[5]={0xE8,0,0,0,0};
  *((PDWORD)(&(Data[1])))=(hPatchSecStartVA+sizeof(hStubData))- hHookStartVA-5;

  
  OutputDebugString("This Debug");
  memcpy(&hStubData.OldData,HGLOBAL((DWORD)hGlobalFile+hHookStartOffset),5);
  memcpy(&hStubData.CrcData,HGLOBAL(DWORD(hGlobalFile)+ThreePointerToRawData+OF_ZP1_6CRC),4);

  DWORD dwwritten;

  SetFilePointer(hFile,hHookStartOffset,0,FILE_BEGIN);
  WriteFile(hFile,&Data,5,&dwwritten,NULL);

  memcpy(hGlobalSecData,&ShellStart,500);
  memcpy(hGlobalSecData,&hStubData,sizeof(hStubData));
  
}


void CZPByPassAllDlg::OnBnClickedBtngenerate()
{
	
	SetDlgItemText(IDC_EDITLOG,"E-Mail:cektop@163.com");




	if (hSelectFile)
	{

		CString GetPath=OpenFileDialog.GetPathName();
        PathRemoveFileSpec((LPSTR)(LPCTSTR)(GetPath));
        GetPath=GetPath.GetString();
        CString szNewFile=GetPath+"\\"+OpenFileDialog.GetFileTitle()+"_cektop."+OpenFileDialog.GetFileExt();
		CopyFile(OpenFileDialog.GetPathName(),szNewFile,FALSE);

		hFile=CreateFile(szNewFile,GENERIC_READ |GENERIC_WRITE ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
        if (hFile!=INVALID_HANDLE_VALUE)
        {

            hSize=GetFileSize(hFile,NULL);
            HANDLE hMapFile=CreateFileMapping(hFile,NULL,PAGE_READWRITE,0,0,NULL);
			if (hMapFile!=NULL)
			{
                hGlobalFile=MapViewOfFile(hMapFile,FILE_MAP_READ |FILE_MAP_WRITE ,NULL,NULL,hSize);
				if (hGlobalFile!=NULL)
				{
				    if (IsFilePE(hGlobalFile)==TRUE)
				    {
                        GetSection();         
                        AddSection();
						if (ScanHookStart())
						{
							IniData();
							WriteSection();
							if (IsOverlay)
							{
								DWORD dwwritten;
								SetFilePointer(hFile,NewTotalOffset,0,FILE_BEGIN);
								WriteFile(hFile,hGlobalOverlay,dwSub,&dwwritten,NULL);
							}
							char SuC[100];
							wsprintf(SuC,"Succeeded! Patch Address: %08X",hHookStartVA);
							SetDlgItemText(IDC_EDITLOG,SuC);
							MessageBox("Done.",NULL,MB_ICONINFORMATION);
							UnmapViewOfFile(hGlobalFile);
							CloseHandle(hMapFile);
							CloseHandle(hFile);		
							
						}
						else
						{
							SetDlgItemText(IDC_EDITLOG,"This is not a valid ZProtect File");
							
							UnmapViewOfFile(hGlobalFile);
							CloseHandle(hMapFile);
							CloseHandle(hFile);	
							DeleteFile(szNewFile);
						}

				    }
				    else
					{
						 SetDlgItemText(IDC_EDITLOG,"This is not a valid PE File");
						 
						 UnmapViewOfFile(hGlobalFile);
						 CloseHandle(hMapFile);
						 CloseHandle(hFile);
						 DeleteFile(szNewFile);
					}
					
				}
				else
				{
				    CloseHandle(hMapFile);	
					CloseHandle(hFile);
                    SetDlgItemText(IDC_EDITLOG,"Error:MapViewOfFile failed!");
				}
			}
			else
			{
				CloseHandle(hFile);
				SetDlgItemText(IDC_EDITLOG,"Error:CreateFileMapping failed!"); 
			}


            
			
        }
		else
		{
			SetDlgItemText(IDC_EDITLOG,"Error:Open failed!");    
		}
		
		
	}
	else
	{
		SetDlgItemText(IDC_EDITLOG,"Error:Opening input file!");
	}

}

void CZPByPassAllDlg::OnBnClickedBtnidcBtngenerate()
{
	// TODO: Add your control notification handler code here
}

void CZPByPassAllDlg::OnBnClickedBtnbrowse()
{
	// TODO: Add your control notification handler code here
    if (OpenFileDialog.DoModal()==IDOK)
    {
       hSelectFile=TRUE;
	   SetDlgItemText(IDC_EDITFile,OpenFileDialog.GetPathName());
    }
	else
	{
       hSelectFile=FALSE;
       SetDlgItemText(IDC_EDITFile,NULL);
	}
}
