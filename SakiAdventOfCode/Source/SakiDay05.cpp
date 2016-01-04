#include "stdafx.h"
#include "SakiFileLoader.h"
#include "SakiDay05.h"


SakiDay05::SakiDay05()
{
}

SakiDay05::~SakiDay05()
{
}

void SakiDay05::compute(int part)
{
	SakiDay05 day;
	if (part == 1)
		day.compute1();
	else
		day.compute2();
}

void SakiDay05::compute1()
{
	compute(check1);
}

void SakiDay05::compute2()
{
	compute(check2);
}

void SakiDay05::compute(std::function<bool(const std::string&)> fnCheck)
{
	int count = 0;

	std::string path = SakiFileLoader::s_defaultRoot + "Day05.txt";
	std::string contents = SakiFileLoader::GetFileContents(path.c_str());

	const char* head = contents.c_str();
	while (head != nullptr)
	{
		std::string line;
		head = SakiFileLoader::GetLine(head, line);
		if (line.empty())
		{
			break;
		}

		if (fnCheck(line))
			count++;
	}

	std::cout << count << std::endl;
}

bool SakiDay05::check1(const std::string& line)
{
	int count = 0;
	bool consective = false;
	bool bad = false;
	TCHAR prev = 0;
	for (size_t i = 0; i < line.length(); i++)
	{
		auto c = line.at(i);
		if (prev == c)
			consective = true;
		if (c == 'a' ||
			c == 'i' ||
			c == 'u' ||
			c == 'e' ||
			c == 'o')
			count++;
		if (prev == 'a' && c == 'b')
			bad = true;
		if (prev == 'c' && c == 'd')
			bad = true;
		if (prev == 'p' && c == 'q')
			bad = true;
		if (prev == 'x' && c == 'y')
			bad = true;
		prev = c;

		if (bad)
			return false;
	}

	if (consective && count >= 3)
		return true;
	else
		return false;
}

bool SakiDay05::check2(const std::string& line)
{
	std::unordered_set<unsigned short> combi;

	bool b1 = false;
	bool b2 = false;

	unsigned short prevs = 0;
	char prev1 = 0;
	char prev2 = 0;
	for (size_t i = 0; i < line.length(); i++)
	{
		char c = (char)line.at(i);
		unsigned short us = ((unsigned short)prev1 << 8) | c;
		if (combi.find(us) != combi.end())
		{
			b1 = true;
		}
		combi.insert(prevs);
		prevs = us;

		if (prev2 == c)
		{
			b2 = true;
		}
		prev2 = prev1;
		prev1 = c;

		if (b1 && b2)
			return true;
	}

	return false;
}
