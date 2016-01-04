#include "stdafx.h"
#include "SakiFileLoader.h"
#include "SakiDay12.h"


SakiDay12::SakiDay12()
{
}

SakiDay12::~SakiDay12()
{
}

void SakiDay12::compute(int part)
{
	SakiDay12 day;
	if (part == 1)
		day.compute1();
	else
		day.compute2();
}

void SakiDay12::compute1()
{
	std::string path = SakiFileLoader::s_defaultRoot + "Day12.txt";
	std::string contents = SakiFileLoader::GetFileContents(path.c_str());

	compute(contents);
}

void SakiDay12::compute2()
{
	auto isInSquare = [](const char* head) -> bool
	{
		const char* h1 = head;
		int curly = 0;
		int square = 0;
		while (h1)
		{
			if (*h1 == '{')
			{
				if (curly == 0)
					return false;
				else
					curly--;
			}
			else if (*h1 == '}')
				curly++;
			else if (*h1 == ']')
				square++;
			else if (*h1 == '[')
			{
				if (square == 0)
					return true;
				else
					square--;
			}
			h1--;
		}

		assert(false);
		return false;
	};

	const char* red = "\"red\"";

	std::string path = SakiFileLoader::s_defaultRoot + "Day12.txt";
	std::string contents = SakiFileLoader::GetFileContents(path.c_str());

	if (contents.size() > 0)
	{
		std::unordered_map<int, int> buffer;
		int curly = 0;
		std::set<int> redDepth;

		char* head = &contents[0];
		while (*head)
		{
			if (strncmp(head, red, strlen(red)) == 0)
			{
				//is in []?
				if (!isInSquare(head))
				{
					char* h1 = head;
					while (h1)
					{
						if (*h1 == '{')
						{
							if (curly == 0)
								break;
							else
								curly--;
						}
						else if (*h1 == '}')
							curly++;
						else if (isdigit(*h1))
							*h1 = '0';
						h1--;
					}
					h1 = head;
					while (h1)
					{
						if (*h1 == '}')
						{
							if (curly == 0)
								break;
							else
								curly--;
						}
						else if (*h1 == '{')
							curly++;
						else if (isdigit(*h1))
							*h1 = '0';
						h1++;
					}
				}
			}
			head++;
		}
	}

	compute(contents);
}

void SakiDay12::compute(std::string contents)
{
	int total = 0;

	if (contents.size() > 0)
	{
		char* head = &contents[0];
		while (*head)
		{
			if (isdigit(*head) || *head == '-')
			{
				char* tail = head + 1;
				while (isdigit(*tail))
				{
					tail++;
				}
				*tail = 0;
				int n = atoi(head);
				total += n;
				head = tail;
			}
			head++;
		}
	}

	std::cout << total << std::endl;
}
