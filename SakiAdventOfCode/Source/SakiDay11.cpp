#include "stdafx.h"
#include "SakiFileLoader.h"
#include "SakiDay11.h"


SakiDay11::SakiDay11()
{
}

SakiDay11::~SakiDay11()
{
}

void SakiDay11::compute(int part)
{
	SakiDay11 day;
	if (part == 1)
		day.compute1();
	else
		day.compute2();
}

static void getNextPassword(std::string& password)
{
	for (auto it = password.rbegin(); it != password.rend(); it++)
	{
		if (*it == 'z')
		{
			*it = 'a';
		}
		else
		{
			++(*it);
			break;
		}
	}
}

static bool isGoodPassword(const std::string& password)
{
	const char* head = password.c_str();
	char prev1 = 0;
	char prev2 = 0;
	char dup1 = 0;
	char dup2 = 0;
	bool consecutive = false;
	while (*head)
	{
		char c = *head;
		if (c == 'i' || c == 'o' || c == 'l')
			return false;

		if (prev1 == c)
		{
			if (dup1 == 0)
				dup1 = c;
			else if (dup1 == c)
				;
			else if (dup2 == 0)
				dup2 = c;
		}

		if (prev2 + 1 == prev1 && prev1 + 1 == c)
			consecutive = true;

		prev2 = prev1;
		prev1 = c;
		head++;
	}

	if (dup2 == 0)
		return false;

	assert(dup1 != 0);
	assert(dup2 != 0);
	assert(dup1 != dup2);

	return consecutive;
}

void SakiDay11::compute1()
{
	std::string password = "hxbxwxba";
	while (true)
	{
		getNextPassword(password);
		if (isGoodPassword(password))
		{
			break; // next one
		}
	}

	std::cout << password << std::endl;
}

void SakiDay11::compute2()
{
	std::string password = "hxbxwxba";
	while (true)
	{
		getNextPassword(password);
		if (isGoodPassword(password))
		{
			break; // next one
		}
	}

	while (true)
	{
		getNextPassword(password);
		if (isGoodPassword(password))
		{
			break; // next to next one
		}
	}

	std::cout << password << std::endl;
}
