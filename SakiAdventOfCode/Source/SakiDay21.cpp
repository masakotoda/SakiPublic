#include "stdafx.h"
#include "SakiFileLoader.h"
#include "SakiDay21.h"


SakiDay21::SakiDay21()
{
}

SakiDay21::~SakiDay21()
{
}

void SakiDay21::compute(int part)
{
	SakiDay21 day;
	if (part == 1)
		day.compute1();
	else
		day.compute2();
}

void SakiDay21::compute1()
{
	// Solved with paper & pencil...
	// 7 damage (40)
	// 2 armo (31)
	// +1 armo (20)
	std::cout << 91 << std::endl;
}

void SakiDay21::compute2()
{
	// Solved with paper & pencil...
	// 4 damage (8)
	// 0 armo (0)
	// +2 damage (50)
	// +3 damage (100)
	std::cout << 158 << std::endl;
}
