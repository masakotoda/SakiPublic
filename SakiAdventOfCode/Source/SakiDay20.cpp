#include "stdafx.h"
#include "SakiFileLoader.h"
#include "SakiDay20.h"


SakiDay20::SakiDay20()
{
}

SakiDay20::~SakiDay20()
{
}

void SakiDay20::compute(int part)
{
	SakiDay20 day;
	if (part == 1)
		day.compute1();
	else
		day.compute2();
}

void SakiDay20::compute1()
{
	// It gotta be multiple of 2, 3, 4, 5 because they are so effective... it must have been proven mathematically.
	const int multi = 10;
	for (int i = 0; ; i += (3 * 4 * 5))
	{
		int count = i == 1 ? multi : i * multi + multi;
		for (int j = 2; j <= i / 2; j++)
		{
			if ((i % j) == 0)
			{
				count += (j * multi);
			}
		}
		if (count >= 29000000)
		{
			std::cout << i << std::endl;
			break;
		}
	}
}

void SakiDay20::compute2()
{
	const int multi = 11;
	for (int i = 0; ; i += (3 * 4 * 5))
	{
		int count = i == 1 ? 0 : i * multi;
		for (int j = 2; j <= i / 2; j++)
		{
			if ((i % j) == 0)
			{
				if (j * 50 >= i)
					count += (j * multi);
			}
		}
		if (count >= 29000000)
		{
			std::cout << i << std::endl;
			break;
		}
	}
}
