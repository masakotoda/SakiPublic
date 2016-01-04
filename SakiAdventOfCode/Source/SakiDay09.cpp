#include "stdafx.h"
#include "SakiFileLoader.h"
#include "SakiDay09.h"


SakiDay09::SakiDay09()
{
}

SakiDay09::~SakiDay09()
{
}

void SakiDay09::compute(int part)
{
	SakiDay09 day;
	if (part == 1)
		day.compute1();
	else
		day.compute2();
}

void SakiDay09::compute1()
{
	auto fnMin = [](int a, int b) -> int { return std::min(a, b); };
	compute(INT_MAX, fnMin);
}

void SakiDay09::compute2()
{
	auto fnMax = [](int a, int b) -> int { return std::max(a, b); };
	compute(0, fnMax);
}

void SakiDay09::compute(int initMinMax, std::function<int(int, int)> fnMinMax)
{
	enum
	{
		Tambi,
		Faerun,
		Norrath,
		Straylight,
		Tristram,
		Arbre,
		AlphaCentauri,
		Snowdin,
		MAX_CITY,
	};

	struct City
	{
		unsigned int distances[MAX_CITY];
	};

	City cities[MAX_CITY] = { 0 };

	cities[AlphaCentauri].distances[Snowdin] = 66;
	cities[AlphaCentauri].distances[Tambi] = 28;
	cities[AlphaCentauri].distances[Faerun] = 60;
	cities[AlphaCentauri].distances[Norrath] = 34;
	cities[AlphaCentauri].distances[Straylight] = 34;
	cities[AlphaCentauri].distances[Tristram] = 3;
	cities[AlphaCentauri].distances[Arbre] = 108;
	cities[Snowdin].distances[Tambi] = 22;
	cities[Snowdin].distances[Faerun] = 12;
	cities[Snowdin].distances[Norrath] = 91;
	cities[Snowdin].distances[Straylight] = 121;
	cities[Snowdin].distances[Tristram] = 111;
	cities[Snowdin].distances[Arbre] = 71;
	cities[Tambi].distances[Faerun] = 39;
	cities[Tambi].distances[Norrath] = 113;
	cities[Tambi].distances[Straylight] = 130;
	cities[Tambi].distances[Tristram] = 35;
	cities[Tambi].distances[Arbre] = 40;
	cities[Faerun].distances[Norrath] = 63;
	cities[Faerun].distances[Straylight] = 21;
	cities[Faerun].distances[Tristram] = 57;
	cities[Faerun].distances[Arbre] = 83;
	cities[Norrath].distances[Straylight] = 9;
	cities[Norrath].distances[Tristram] = 50;
	cities[Norrath].distances[Arbre] = 60;
	cities[Straylight].distances[Tristram] = 27;
	cities[Straylight].distances[Arbre] = 81;
	cities[Tristram].distances[Arbre] = 90;

	for (int i = 0; i < MAX_CITY; i++)
	{
		for (int j = 0; j < MAX_CITY; j++)
		{
			auto d1 = cities[i].distances[j];
			auto d2 = cities[j].distances[i];
			if (d1 != 0)
				cities[j].distances[i] = d1;
			else if (d2 != 0)
				cities[i].distances[j] = d2;
		}
	}

	std::vector<int> s;
	for (int i = 0; i < MAX_CITY; i++)
		s.push_back(i);

	int minMax = initMinMax;
	do {
		unsigned int d = 0;
		for (int i = 0; i < 7; i++)
		{
			d += cities[s[i]].distances[s[i + 1]];
		}
		minMax = fnMinMax(minMax, d);
	} while (std::next_permutation(s.begin(), s.end()));

	std::cout << minMax << std::endl;
}
