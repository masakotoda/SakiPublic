#include "stdafx.h"
#include "SakiFileLoader.h"
#include "SakiDay13.h"


SakiDay13::SakiDay13()
{
}

SakiDay13::~SakiDay13()
{
}

void SakiDay13::compute(int part)
{
	SakiDay13 day;
	if (part == 1)
		day.compute1();
	else
		day.compute2();
}

void SakiDay13::compute1()
{
	compute(false);
}

void SakiDay13::compute2()
{
	compute(true);
}

void SakiDay13::compute(bool includeMe)
{
	struct member
	{
		int happiness[8];
	};

	member members[8] = { 0 };

	enum
	{
		Alice,
		Bob,
		Carol,
		David,
		Eric,
		Frank,
		George,
		Mallory,
	};

	members[Alice].happiness[Bob] = -2;
	members[Alice].happiness[Carol] = -62;
	members[Alice].happiness[David] = 65;
	members[Alice].happiness[Eric] = 21;
	members[Alice].happiness[Frank] = -81;
	members[Alice].happiness[George] = -4;
	members[Alice].happiness[Mallory] = -80;
	members[Bob].happiness[Alice] = 93;
	members[Bob].happiness[Carol] = 19;
	members[Bob].happiness[David] = 5;
	members[Bob].happiness[Eric] = 49;
	members[Bob].happiness[Frank] = 68;
	members[Bob].happiness[George] = 23;
	members[Bob].happiness[Mallory] = 29;
	members[Carol].happiness[Alice] = -54;
	members[Carol].happiness[Bob] = -70;
	members[Carol].happiness[David] = -37;
	members[Carol].happiness[Eric] = -46;
	members[Carol].happiness[Frank] = 33;
	members[Carol].happiness[George] = -35;
	members[Carol].happiness[Mallory] = 10;
	members[David].happiness[Alice] = 43;
	members[David].happiness[Bob] = -96;
	members[David].happiness[Carol] = -53;
	members[David].happiness[Eric] = -30;
	members[David].happiness[Frank] = -12;
	members[David].happiness[George] = 75;
	members[David].happiness[Mallory] = -20;
	members[Eric].happiness[Alice] = 8;
	members[Eric].happiness[Bob] = -89;
	members[Eric].happiness[Carol] = -69;
	members[Eric].happiness[David] = -34;
	members[Eric].happiness[Frank] = 95;
	members[Eric].happiness[George] = 34;
	members[Eric].happiness[Mallory] = -99;
	members[Frank].happiness[Alice] = -97;
	members[Frank].happiness[Bob] = 6;
	members[Frank].happiness[Carol] = -9;
	members[Frank].happiness[David] = 56;
	members[Frank].happiness[Eric] = -17;
	members[Frank].happiness[George] = 18;
	members[Frank].happiness[Mallory] = -56;
	members[George].happiness[Alice] = 45;
	members[George].happiness[Bob] = 76;
	members[George].happiness[Carol] = 63;
	members[George].happiness[David] = 54;
	members[George].happiness[Eric] = 54;
	members[George].happiness[Frank] = 30;
	members[George].happiness[Mallory] = 7;
	members[Mallory].happiness[Alice] = 31;
	members[Mallory].happiness[Bob] = -32;
	members[Mallory].happiness[Carol] = 95;
	members[Mallory].happiness[David] = 91;
	members[Mallory].happiness[Eric] = -66;
	members[Mallory].happiness[Frank] = -75;
	members[Mallory].happiness[George] = -99;


	int minimum = INT_MAX;
	int maximum = 0;
	int maxWithMe = 0;
	std::vector<int> s = { 0, 1, 2, 3, 4, 5, 6, 7 };
	do {
		int happiness[8];
		for (int i = 0; i < 8; i++)
		{
			happiness[i] = members[s[i]].happiness[s[(i + 1) % 8]]
				+ members[s[(i + 1) % 8]].happiness[s[i]];
		}
		int sum = 0;
		for (int i = 0; i < 8; i++)
		{
			sum += happiness[i];
		}
		maximum = std::max(maximum, sum);

		if (includeMe)
		{
			if (sum == maximum) // the recorde has been broken! It's worth updating maxWithMe.
			{
				int minCombi = INT_MAX;
				for (int i = 0; i < 8; i++)
				{
					minCombi = std::min(minCombi, happiness[i]);
				}
				maxWithMe = maximum - minCombi;
			}
		}

	} while (std::next_permutation(s.begin(), s.end()));

	if (includeMe)
	{
		std::cout << maxWithMe << std::endl;
	}
	else
	{
		std::cout << maximum << std::endl;
	}
}
