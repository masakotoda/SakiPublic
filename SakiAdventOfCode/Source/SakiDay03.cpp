#include "stdafx.h"
#include "SakiFileLoader.h"
#include "SakiDay03.h"


SakiDay03::SakiDay03()
{
}

SakiDay03::~SakiDay03()
{
}

void SakiDay03::compute(int part)
{
	SakiDay03 day;
	if (part == 1)
		day.compute1();
	else
		day.compute2();
}

void SakiDay03::compute1()
{
	std::string path = SakiFileLoader::s_defaultRoot + "Day03.txt";
	std::string contents = SakiFileLoader::GetFileContents(path.c_str());
		
	short x = 0;
	short y = 0;
	std::unordered_set<unsigned long> houses;
	houses.insert(0);

	for (size_t i = 0; i < contents.size(); i++)
	{
		auto c = contents.at(i);
		if (c == '^')
		{
			y--;
		}
		else if (c == 'v')
		{
			y++;
		}
		else if (c == '>')
		{
			x++;
		}
		else if (c == '<')
		{
			x--;
		}
		else
			throw;

		unsigned long key = ((unsigned long)x << 16) + (unsigned long)y;
		houses.insert(key);
	}

	std::cout << houses.size() << std::endl;
}

void SakiDay03::compute2()
{
	std::string path = SakiFileLoader::s_defaultRoot + "Day03.txt";
	std::string contents = SakiFileLoader::GetFileContents(path.c_str());

	short x1 = 0;
	short y1 = 0;
	short x2 = 0;
	short y2 = 0;
	std::unordered_set<unsigned long> houses;
	houses.insert(0);

	for (size_t i = 0; i < contents.size(); i++)
	{
		short* xx = (i % 2) == 0 ? &x1 : &x2;
		short* yy = (i % 2) == 0 ? &y1 : &y2;

		auto c = contents.at(i);
		if (c == '^')
		{
			(*yy)--;
		}
		else if (c == 'v')
		{
			(*yy)++;
		}
		else if (c == '>')
		{
			(*xx)++;
		}
		else if (c == '<')
		{
			(*xx)--;
		}
		else
			throw;

		if ((i % 2) == 0)
		{
			unsigned long key = ((unsigned long)x1 << 16) + (unsigned long)y1;
			houses.insert(key);
		}
		else
		{
			unsigned long key = ((unsigned long)x2 << 16) + (unsigned long)y2;
			houses.insert(key);
		}
	}

	std::cout << houses.size() << std::endl;
}
