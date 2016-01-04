#include "stdafx.h"
#include "SakiFileLoader.h"
#include "SakiDay25.h"


SakiDay25::SakiDay25()
{
}

SakiDay25::~SakiDay25()
{
}

void SakiDay25::compute(int part)
{
	SakiDay25 day;
	if (part == 1)
		day.compute1();
	else
		day.compute2();
}

void SakiDay25::compute1()
{
	auto getIndex = [] (int row, int col) -> int
	{
		int x = (row + col) - 1;
		int sum = (1 + x) * x / 2;
		int index = sum - row + 1;
		return index;
	};


	int i1 = getIndex(1, 1);
	int i2 = getIndex(4, 2);
	int i3 = getIndex(1, 5);

	assert(i1 == 1);
	assert(i2 == 12);
	assert(i3 == 15);

	int i = getIndex(2981, 3075);
	unsigned long long val = 20151125;
	for (int j = 2; j <= i; j++)
	{
		val = (val * 252533) % 33554393;
	}

	std::cout << val << std::endl;
}

void SakiDay25::compute2()
{
}
