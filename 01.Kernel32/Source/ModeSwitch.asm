[BITS 32]

;combine with C code
global kReadCPUID, kSwitchAndExecute64bitKernel

SECTION .text

;return cpu id
  ;PARAM: DWORD dwEAX, DWORD* pdwEAX, * pdwEBX, * pdwECX, *pdwEDX
kReadCPUID:
  push ebp
  mov ebp, esp
  push eax
  push ebx
  push ecx
  push edx
  push esi

  mov eax, dword[ebp+8]
  cpuid
  
  ;* pdwEAX
  mov esi, dword[ebp+12]
  mov dword[esi],eax
  ;* pdwEBX
  mov esi, dword[ebp+16]
  mov dword[esi],ebx
  ;* pdwECX
  mov esi, dword[ebp+20]
  mov dword[esi],ecx
  ;* pdwEDX
  mov esi, dword[ebp+24]
  mov dword[esi],edx

  pop esi
  pop edx
  pop ecx
  pop ebx
  pop eax
  pop ebp
  ret

;change to IA-32e mode and launch 64bit kernel
kSwitchAndExecute64bitKernel:
  ;CR4:: PAE to 1

  mov eax,cr4
  or eax,0x20
  mov cr4,eax

  ;CR3:: PML4 address, and cache enable
  mov eax, 0x100000
  mov cr3, eax

  ;enable IA32_EFER.LME to 1
  mov ecx, 0xC0000080
  rdmsr
  or eax,0x0100
  wrmsr

  ;CR0 control register:: NW=0, CD=0, PG=1
  ;enable cache and paging

  mov eax,cr0
  or eax, 0xE0000000
  xor eax,0x60000000
  mov cr0, eax

  jmp 0x08:0x200000 ;replace CS segment selector to IA-32e moded Segment descriptor, then move to 2MB address

  jmp $