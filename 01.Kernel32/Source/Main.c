#include "Types.h"
#include "Page.h"
#include "ModeSwitch.h"

void kPrintString(int iX, int iY, const char* pcString);
BOOL kInitializeKernel64Area();
BOOL kIsMemoryEnough();
void kCopyKernel64ImageTo2Mbyte();

void Main()
{
  DWORD i;
  DWORD dwEAX, dwEBX, dwECX, dwEDX;
  char vcVendorString[13] = {0,};
  kPrintString(0,3,"Protected mode C Lang Kernel Started");

  kPrintString(0,4,"Minimum memory size check");
  if(kIsMemoryEnough()==FALSE)
  {
    kPrintString(45,4,"Fail");
    kPrintString(0,5,"Not Enough Memory");
    while(1);
  }
  else
    kPrintString(45,4,"PASS");
  kPrintString(0,5,"Initializing IA-32e Kernel Area");

  if(kInitializeKernel64Area()==FALSE)
  {
    kPrintString(45,5,"Fail");
    kPrintString(0,6,"Kernel Area Initialization FAILED");
    while(1);
  }
  kPrintString(45,5,"PASS");

  //Generate Page Table
  kPrintString(0,6,"Initializing IA-32e Page Tables");
  kInitializePageTables();
  kPrintString(45,6,"PASS");

  //read processor data
  kReadCPUID(0x00,&dwEAX,&dwEBX,&dwECX,&dwEDX);
  *(DWORD*) vcVendorString=dwEBX;
  *((DWORD *) vcVendorString + 1)=dwEDX;
  *((DWORD *) vcVendorString + 2)=dwECX;
  kPrintString(0,7,"Processor Vendor string: ");
  kPrintString(45,7, vcVendorString);

  //check ability of 64bit
  kReadCPUID(0x80000001, &dwEAX,&dwEBX,&dwECX,&dwEDX);
  kPrintString(0,8,"64bit mode support check");
  if(dwEDX & (1<<29))
    kPrintString(45,8,"PASS");
  else
  {
    kPrintString(45,8,"FAIL");
    while(1);
  }

  // IA-32e mode kernel to 0x200000(2Mbyte) address
  kPrintString( 0, 9, "Copy IA-32e Kernel To 2M Address->" );
  kCopyKernel64ImageTo2Mbyte();
  kPrintString( 45, 9, "Pass" );
  
  //
  kPrintString( 0, 10, "Switch To IA-32e Mode" );
  kSwitchAndExecute64bitKernel();
  while (1);
}
void kPrintString(int iX, int iY, const char* pcString)
{
  CHARACTER* pstScreen = (CHARACTER* )0xB8000;
  int i;

  pstScreen+=(iY*80)+iX;
  for(i=0;pcString[i]!=0;i++)
  {
    pstScreen[i].bCharactor=pcString[i];
  }
}
BOOL kInitializeKernel64Area()
{
  DWORD* pdwCurrentAddress;

  //initializing start address 0x100000(1MB)
  pdwCurrentAddress=(DWORD*)0x100000;

  while((DWORD) pdwCurrentAddress < 0x600000 )
  {
    *pdwCurrentAddress=0x00;
    
    //error handling -> check if it initialized
    if(*pdwCurrentAddress!=0)
      return FALSE;

    //now move on to next address
    pdwCurrentAddress++;
  }
  return TRUE;
}

BOOL kIsMemoryEnough()
{
  DWORD* pdwCurrentAddress;

  pdwCurrentAddress = (DWORD*) 0x100000;

  while((DWORD)pdwCurrentAddress<0x400000)
  {
    *pdwCurrentAddress=0x12345678;
    if(*pdwCurrentAddress!=0x12345678)
      return FALSE;
  
    pdwCurrentAddress += (0x100000/4);
  }
  return TRUE;
}

void kCopyKernel64ImageTo2Mbyte()
{
    WORD wKernel32SectorCount, wTotalKernelSectorCount;
    DWORD* pdwSourceAddress,* pdwDestinationAddress;
    int i;
    
    wTotalKernelSectorCount = *( ( WORD* ) 0x7C05 );
    wKernel32SectorCount = *( ( WORD* ) 0x7C07 );

    pdwSourceAddress = ( DWORD* ) ( 0x10000 + ( wKernel32SectorCount * 512 ) );
    pdwDestinationAddress = ( DWORD* ) 0x200000;

    for( i = 0 ; i < 512 * ( wTotalKernelSectorCount - wKernel32SectorCount ) / 4;
        i++ )
    {
        *pdwDestinationAddress = *pdwSourceAddress;
        pdwDestinationAddress++;
        pdwSourceAddress++;
    }
}