#include "Types.h"

void kPrintString(int iX, int iY, const char* pcString);

void Main()
{
  kPrintString(0,10,"Switched to IA-32e Mode");
  kPrintString(0,11,"IA-32e C Lang Kernel started");
}

void kPrintString(int iX, int iY, const char* pcString)
{
  CHARACTER* pstScreen=(CHARACTER *)0xB8000;
  int i;
  
  pstScreen+=(iY*80)+iX;
  for(i=0;pcString[i]!=0;i++)
  {
    pstScreen[i].bCharactor=pcString[i];
  }
}