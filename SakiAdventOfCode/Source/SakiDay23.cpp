#include "stdafx.h"
#include "SakiFileLoader.h"
#include "SakiDay23.h"


SakiDay23::SakiDay23()
{
}

SakiDay23::~SakiDay23()
{
}

void SakiDay23::compute(int part)
{
	SakiDay23 day;
	if (part == 1)
		day.compute1();
	else
		day.compute2();
}

void SakiDay23::compute1()
{
	computeWithAssembly(0);
}

void SakiDay23::compute2()
{
	computeWithAssembly(1);
}

void SakiDay23::computeWithAssembly(long initialA)
{
	// eax -> a
	// ebx -> b

	int a = 0;
	int b = 0;

	__asm {
		mov eax, initialA
		mov ebx, 0
		mov esi, 2

		cmp eax, 1
		je l16

		inc eax
		inc eax
		imul eax, 3
		imul eax, 3
		imul eax, 3
		inc eax
		inc eax
		imul eax, 3
		inc eax
		inc eax
		imul eax, 3
		imul eax, 3
		imul eax, 3
		inc eax
		jmp l23
	l16:
		imul eax, 3
		inc eax
		inc eax
		imul eax, 3
		inc eax
		inc eax
		imul eax, 3
		imul eax, 3
		inc eax
		inc eax
		imul eax, 3
		inc eax
		imul eax, 3
		inc eax
		imul eax, 3
		inc eax
		inc eax
		imul eax, 3
		inc eax
		imul eax, 3
		imul eax, 3
		inc eax
	l23:
		cmp eax, 1
		je l8
		
		inc ebx

		mov edx, eax
		and edx, 1
		cmp edx, 0
		je l4

		imul eax, 3
		inc eax
		jmp l2
	l4:
		mov edx, 0
		idiv esi
	l2:
		jmp l23
	l8:	
		mov a, eax
		mov b, ebx
	}

	std::cout << b << std::endl;
}