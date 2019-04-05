#include "stdafx.h"
#include "PE.h"


WORD NumberOfSections;
DWORD SectionAlignment;
DWORD FileAlignment;
DWORD SizeOfImage;
DWORD ImageBase;

//TwoSetcion
DWORD Characteristic=0xE0000020;
DWORD TwoVirtualAddress;
DWORD TwoVirtualSize;
DWORD TwoPointerToRawData;
DWORD TwoSizeOfRawData;

//ThreeSetcion
DWORD ThreeVirtualAddress;
DWORD ThreePointerToRawData;

//Total
DWORD TotalVirtualAddress;
DWORD TotalOffset;
DWORD EndPointerToRawData;

//AddSection
HGLOBAL hGlobalSecData; 
DWORD NewSizeOfImage;
DWORD NewTotalOffset;
WORD  NewNumberOfSections;
IMAGE_SECTION_HEADER  NewPSectionHeader={0};
char SecName[8]=".text";


//PE
PIMAGE_DOS_HEADER PDosHeader;
PIMAGE_NT_HEADERS PNtHeader;
PIMAGE_FILE_HEADER PFileHeader;
PIMAGE_OPTIONAL_HEADER POptionalHeader;
PIMAGE_SECTION_HEADER  PSectionHeader;
//PE

//Overlay
DWORD dwSub;
BOOL IsOverlay;
HGLOBAL hGlobalOverlay;


//Files
HANDLE hFile;
HGLOBAL hGlobalFile;
DWORD hSize;


BOOL IsFilePE(LPVOID lpAddr)
{
	if (!lpAddr) {
		return FALSE;
	}
	PDosHeader=(PIMAGE_DOS_HEADER)lpAddr;
	if (PDosHeader->e_magic!=IMAGE_DOS_SIGNATURE) {
		return FALSE;
	}
	PNtHeader=(PIMAGE_NT_HEADERS)((DWORD)PDosHeader+PDosHeader->e_lfanew);
	if (PNtHeader->Signature!=IMAGE_NT_SIGNATURE) {
		return FALSE;
	}
	return TRUE;

}

void ReadOverlay()
{
	dwSub=hSize-EndPointerToRawData;
	if (dwSub==0) {
		IsOverlay=FALSE;
	}
	else
	{
		IsOverlay=TRUE;
        hGlobalOverlay=GlobalAlloc(GPTR,dwSub+1);
		DWORD dwRead;
		SetFilePointer(hFile,EndPointerToRawData,0,FILE_BEGIN);
		ReadFile(hFile,hGlobalOverlay,dwSub,&dwRead,NULL);
	}
}

void GetSection()
{

	int i;
	PFileHeader=(PIMAGE_FILE_HEADER)(&PNtHeader->FileHeader);
	NumberOfSections=PFileHeader->NumberOfSections;
	POptionalHeader=(PIMAGE_OPTIONAL_HEADER)(&PNtHeader->OptionalHeader);
	SectionAlignment=POptionalHeader->SectionAlignment;
	FileAlignment= POptionalHeader->FileAlignment;
	SizeOfImage= POptionalHeader->SizeOfImage;
	ImageBase=POptionalHeader->ImageBase;
	PSectionHeader=IMAGE_FIRST_SECTION(PNtHeader);
	for (i = 0; i < NumberOfSections; i++) {
		if (i==1)
		{
			memcpy(&PSectionHeader->Characteristics,&Characteristic,4);
			memcpy(&TwoVirtualAddress,&PSectionHeader->VirtualAddress,4);
			memcpy(&TwoVirtualSize,&PSectionHeader->Misc.VirtualSize,4);
			memcpy(&TwoPointerToRawData,&PSectionHeader->PointerToRawData,4);
			memcpy(&TwoSizeOfRawData,&PSectionHeader->SizeOfRawData,4);

		}
		if (i==2)
		{
            memcpy(&ThreePointerToRawData,&PSectionHeader->PointerToRawData,4);
		}
		++PSectionHeader;
	}
	--PSectionHeader;
	DWORD VirtualAddress=PSectionHeader->VirtualAddress;
	DWORD VirtualSize=PSectionHeader->Misc.VirtualSize;
	DWORD PointerToRawData= PSectionHeader->PointerToRawData;
	DWORD SizeOfRawData= PSectionHeader->SizeOfRawData;
	TotalVirtualAddress=VirtualAddress+VirtualSize;
	TotalOffset=PointerToRawData+SizeOfRawData;
	DWORD Mod=TotalVirtualAddress % SectionAlignment;
	if (Mod!=0) {
		TotalVirtualAddress=TotalVirtualAddress-Mod+SectionAlignment;
	}
	EndPointerToRawData=TotalOffset;
	Mod=TotalOffset % FileAlignment;
	if (Mod!=0) {
		TotalOffset=TotalOffset-Mod+FileAlignment;
	} 
	PSectionHeader++;
	ReadOverlay();
}

void AddSection()
{
	hGlobalSecData=GlobalAlloc(GPTR,Section_Size);
	NewSizeOfImage=Section_Size+TotalVirtualAddress;
	NewTotalOffset=Section_Size+TotalOffset;
	NewNumberOfSections=NumberOfSections+1;

	memcpy(NewPSectionHeader.Name,SecName,IMAGE_SIZEOF_SHORT_NAME);
	NewPSectionHeader.Misc.VirtualSize=Section_Size;
	NewPSectionHeader.VirtualAddress=TotalVirtualAddress;
	NewPSectionHeader.SizeOfRawData=Section_Size;
	NewPSectionHeader.PointerToRawData=TotalOffset;
	NewPSectionHeader.Characteristics=Characteristic;

}


void WriteSection()
{
	DWORD dwwritten;

	SetFilePointer(hFile,LONG(&PFileHeader->NumberOfSections)-LONG(hGlobalFile),0,FILE_BEGIN);
	WriteFile(hFile,&NewNumberOfSections,sizeof(WORD),&dwwritten,NULL);

	SetFilePointer(hFile,LONG(&POptionalHeader->SizeOfImage)-LONG(hGlobalFile),0,FILE_BEGIN);
	WriteFile(hFile,&NewSizeOfImage,sizeof(DWORD),&dwwritten,NULL);

	SetFilePointer(hFile,LONG(PSectionHeader)-LONG(hGlobalFile),0,FILE_BEGIN);
	WriteFile(hFile,&NewPSectionHeader,sizeof(IMAGE_SECTION_HEADER),&dwwritten,NULL);

	SetFilePointer(hFile,TotalOffset,0,FILE_BEGIN);
	WriteFile(hFile,hGlobalSecData,Section_Size,&dwwritten,NULL);
}




