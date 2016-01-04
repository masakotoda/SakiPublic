#include "stdafx.h"
#include "SakiFileLoader.h"
#include "SakiDay16.h"


SakiDay16::SakiDay16()
{
}

SakiDay16::~SakiDay16()
{
}

void SakiDay16::compute(int part)
{
	SakiDay16 day;
	if (part == 1)
		day.compute1();
	else
		day.compute2();
}

void SakiDay16::compute(std::function<bool(const char* key, int cnt)> checkSue)
{
	std::string path = SakiFileLoader::s_defaultRoot + "Day16.txt";
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

		const int maxKey = 63;
		int sueNum = 0;
		char key1[maxKey + 1] = { 0 };
		char key2[maxKey + 1] = { 0 };
		char key3[maxKey + 1] = { 0 };
		int cnt1 = 0;
		int cnt2 = 0;
		int cnt3 = 0;
		sscanf_s(line.c_str(), "Sue %d: %s %d, %s %d, %s %d",
			&sueNum, key1, maxKey, &cnt1, key2, maxKey, &cnt2, key3, maxKey, &cnt3);

		bool found = true;
		if (found)
		{
			found = checkSue(key1, cnt1);
		}
		if (found)
		{
			found = checkSue(key2, cnt2);
		}
		if (found)
		{
			found = checkSue(key3, cnt3);
		}

		if (found)
		{
			std::cout << sueNum << std::endl;
		}
	}
}

void SakiDay16::compute1()
{
	auto checkSue = [this](const char* key, int cnt) -> bool
	{
		std::string keyStr = key;
		auto it = this->c_sue.find(keyStr);
		if (it != this->c_sue.end())
		{
			if (cnt == it->second)
			{
				return true;
			}
		}

		return false;
	};

	compute(checkSue);
}

void SakiDay16::compute2()
{
	auto checkSue = [this](const char* key, int cnt) -> bool
	{
		std::string keyStr = key;
		auto it = this->c_sue.find(keyStr);
		if (it != this->c_sue.end())
		{
			if (keyStr == "cats:" || keyStr == "trees:")
			{
				if (cnt > it->second)
				{
					return true;
				}
			}
			else if (keyStr == "pomeranians:" || keyStr == "goldfish:")
			{
				if (cnt < it->second)
				{
					return true;
				}
			}
			else
			{
				if (cnt == it->second)
				{
					return true;
				}
			}
		}

		return false;
	};

	compute(checkSue);
}
