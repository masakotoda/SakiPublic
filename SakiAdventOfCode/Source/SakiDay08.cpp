#include "stdafx.h"
#include "SakiFileLoader.h"
#include "SakiDay08.h"


SakiDay08::SakiDay08()
{
}

SakiDay08::~SakiDay08()
{
}

void SakiDay08::compute(int part)
{
	SakiDay08 day;
	if (part == 1)
		day.compute1();
	else
		day.compute2();
}

void SakiDay08::compute1()
{
	int total = 0;

	std::string path = SakiFileLoader::s_defaultRoot + "Day08.txt";
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
		
		int count = 0;
		const char* p = line.c_str() + 1;
		while (true)
		{
			if (*p == '\\')
			{
				p++;
				if (*p == 'x')
				{
					count++;
					p += 3;
				}
				else
				{
					count++;
					p++;
				}
			}
			else if (*p == '"')
			{
				break;
			}
			else
			{
				count++;
				p++;
			}
		}

		total += (line.length() - count);
	}

	std::cout << total << std::endl;
}

void SakiDay08::compute2()
{
	int total = 0;

	std::string path = SakiFileLoader::s_defaultRoot + "Day08.txt";
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

		int count = 2; // For extra " and "
		const char* p = line.c_str();
		while (*p)
		{
			if (*p == '"')
				count += 2;
			else if (*p == '\\')
				count += 2;
			else
				count++;
			p++;
		}

		total += (count - line.length());
	}

	std::cout << total << std::endl;
}
