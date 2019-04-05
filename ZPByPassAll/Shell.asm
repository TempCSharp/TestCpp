.586P
.MODEL FLAT,STDCALL
OPTION CASEMAP:NONE


PUBLIC				ShellStart

.code

ShellStart LABEL	DWORD
   l_OrgData  db 5 dup(?)
   l_FileCrCData  dd ?
   l_NONagOF      dd ?
   l_CallDecodeOF dd ?

   
      pushad
      call @@1
      @@1:
      pop edx
      mov ebx,eax
      mov eax,esp
      add eax,20H
      mov ecx,5
      mov edi,dword ptr ds:[eax]
      sub edi,ecx
      mov dword ptr ds:[eax],edi
      lea esi,dword ptr [edx+(l_OrgData-@@1)]
      rep movsb
      
      ;File Crc
      mov edi,ebx
	  add edi,3AC0EH
	  mov word ptr ds:[edi],850FH
      
      ;NO NAG
      mov eax,ebx
      add eax,0031555H
	  mov byte ptr ds:[eax],0EBH
      
	  ;Decode
	  mov edi,ebx
	  add edi,2D8E8H
	  lea eax,dword ptr [edx+(ZPDecode149-@@1)]
      sub eax,5
	  sub eax,edi
	  mov dword ptr ds:[edi+1],eax
      
      popad
      ret
ZPDecode149:
  push ebp
		mov ebp,esp
		sub esp,10H
		lea eax,dword ptr ss:[ebp-10H]
		push eax
		push dword ptr ss:[ebp+14H]
		push dword ptr ss:[ebp+10H]
		call Proc149
		xor ecx,ecx
		cmp dword ptr ss:[ebp+0CH],ecx
		jle L024
L011:
		mov eax,dword ptr ss:[ebp+8]
		mov edx,ecx
		add eax,ecx
		and edx,8000000FH
		jns L019
		dec edx
		or edx,0FFFFFFF0H
		inc edx
L019:
		mov dl,byte ptr ss:[ebp+edx-10H]
		xor byte ptr ds:[eax],dl
		inc ecx
		cmp ecx,dword ptr ss:[ebp+0CH]
		jl L011
L024:
		leave
		retn

Proc149:
		pushad
		call @@8
		@@8:
		pop edx
		mov edi,eax
		mov ecx,10H
		lea esi,dword ptr [edx+(Me_Decode-@@8)]
		rep movsb
		popad
		ret 0CH	
		

Me_Decode db 0A3H,093H,0AFH,0A9H,0CEH,080H,03CH,0D5H,085H,027H,0F5H,05EH,0D8H,02BH,03EH,08CH

szZPByPassAll db 'ZPByPassAll 1.0  RC3 by cektop',0
AnlyZero  db 400 dup(?)
end

