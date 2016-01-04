#include "stdafx.h"
#include "SakiFileLoader.h"
#include "SakiDay15.h"


SakiDay15::SakiDay15()
{
}

SakiDay15::~SakiDay15()
{
}

void SakiDay15::compute(int part)
{
	SakiDay15 day;
	if (part == 1)
		day.compute1();
	else
		day.compute2();
}


/*
Sugar : capacity 3,  durability 0, flavor 0,  texture -3,  calories 2
Sprin : capacity -3, durability 3, flavor 0,  texture 0,   calories 9
Candy : capacity -1, durability 0, flavor 4,  texture 0,   calories 1
Choco : capacity 0,  durability 0, flavor -2, texture 2,   calories 8

Total of Sugar + total of choco must be > 0
Total of Candy + total of choco must be > 0
Total of sprin + total of candy + total of sugar must be > 0
*/

static long long int cookieValue(int sugar, int choco, int candy, int sprin)
{
	int capacity = 3 * sugar - 3 * sprin - candy;
	int durability = 3 * sprin;
	int flavor = 4 * candy - 2 * choco;
	int texture = (-3) * sugar + 2 * choco;
	if (capacity <= 0 || durability <= 0 || flavor <= 0 || texture <= 0)
		return 0;
	else
		return capacity * durability * flavor * texture;
}

void SakiDay15::compute1()
{
	int sugarMax = 0;
	int chocoMax = 0;
	int candyMax = 0;
	int sprinMax = 0;
	long long int maximum = 0;
	for (int sugar = 0; sugar <= 100; sugar++)
	{
		int chocoMin = sugar * (-3) / 2;
		for (int choco = chocoMin; choco <= 100 - sugar; choco++)
		{
			int candyMin = choco * (-2) / 4;
			for (int candy = candyMin; candy <= 100 - sugar - choco; candy++)
			{
				int sprin = 100 - sugar - choco - candy;
				long long int point = cookieValue(sugar, choco, candy, sprin);
				if (point > maximum)
				{
					maximum = point;
					sugarMax = sugar;
					chocoMax = choco;
					candyMax = candy;
					sprinMax = sprin;
				}
			}
		}
	}

	std::cout << maximum << std::endl;
}

void SakiDay15::compute2()
{
	int sugarMax = 0;
	int chocoMax = 0;
	int candyMax = 0;
	int sprinMax = 0;
	long long int maximum = 0;
	for (int sugar = 0; sugar <= 100; sugar++)
	{
		int chocoMin = sugar * (-3) / 2;
		for (int choco = chocoMin; choco <= 100 - sugar; choco++)
		{
			//sugar & choco are known value. solve the following equation:
			//
			//sugar + choco + candy + sprin = 100
			//sugar * 2 + choco * 8 + candy + 9 * sprin = 500
			//
			//sugar * 2 + choco * 8 + candy + 9 * (100 - sugar - choco - candy) = 500
			//candy = (400 - sugar * 7 - choco) / 8

			int magic = 400 - sugar * 7 - choco;
			if ((magic % 8) == 0 && magic >= 0)
			{
				int candy = magic / 8;
				int sprin = 100 - sugar - choco - candy;

				long long int point = cookieValue(sugar, choco, candy, sprin);
				if (point > maximum)
				{
					maximum = point;
					sugarMax = sugar;
					chocoMax = choco;
					candyMax = candy;
					sprinMax = sprin;
				}
			}
		}
	}

	std::cout << maximum << std::endl;
}
