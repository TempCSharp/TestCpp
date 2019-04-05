BOOL IsFilePE(LPVOID lpAddr);
void GetSection();
void AddSection();
void WriteSection();

#define Section_Size 0x2000

extern  DWORD ImageBase;

extern DWORD TotalOffset;
extern DWORD TotalVirtualAddress;
extern DWORD EndPointerToRawData;

extern DWORD TwoVirtualSize;
extern DWORD TwoPointerToRawData;
extern DWORD TwoSizeOfRawData;
extern DWORD TwoVirtualAddress;

extern DWORD ThreePointerToRawData;

extern DWORD NewTotalOffset;

extern BOOL IsOverlay;
extern  HGLOBAL hGlobalOverlay;
extern  DWORD dwSub;

extern HGLOBAL hGlobalFile;
extern DWORD hSize;
extern HANDLE hFile;

extern HGLOBAL hGlobalSecData; 

