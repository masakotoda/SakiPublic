#include "stdafx.h"
#include "SakiFileLoader.h"
#include "SakiDay02.h"


SakiDay02::SakiDay02()
{
}

SakiDay02::~SakiDay02()
{
}

void SakiDay02::compute(int part)
{
	SakiDay02 day;
	if (part == 1)
		day.compute1();
	else
		day.compute2();
}

void SakiDay02::compute1()
{
	int totalPaper = 0;

	std::string path = SakiFileLoader::s_defaultRoot + "Day02.txt";
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

		int x = 0;
		int y = 0;
		int z = 0;

		sscanf_s(line.c_str(), "%dx%dx%d", &x, &y, &z);
		int xy = x*y;
		int yz = y*z;
		int zx = z*x;
		int minimum = std::min(xy, std::min(yz, zx));
		int paper = 2 * (xy + yz + zx) + minimum;

		totalPaper += paper;
	}

	std::cout << totalPaper << std::endl;
}

void SakiDay02::compute2()
{
	int totalRibbon = 0;

	std::string path = SakiFileLoader::s_defaultRoot + "Day02.txt";
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

		int x = 0;
		int y = 0;
		int z = 0;

		sscanf_s(line.c_str(), "%dx%dx%d", &x, &y, &z);
		int min1 = std::min(x, y);
		int min2 = std::min(std::max(x, y), z);
		int ribbon = min1 + min1 + min2 + min2 + x * y * z;

		totalRibbon += ribbon;
	}

	std::cout << totalRibbon << std::endl;
}
