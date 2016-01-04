#include "stdafx.h"
#include "SakiFileLoader.h"
#include "SakiDay10.h"


SakiDay10::SakiDay10()
{
}

SakiDay10::~SakiDay10()
{
}

void SakiDay10::compute(int part)
{
	SakiDay10 day;
	if (part == 1)
		day.compute1();
	else
		day.compute2();
}

static std::string look_and_say(std::string in)
{
	std::string out;
	const char* head = in.c_str();
	int count = 0;
	while (*head)
	{
		char c = *head;
		char n = *(head + 1);
		count++;
		if (c != n)
		{
			out += std::to_string(count);
			out += c;
			count = 0;
		}
		head++;
	}

	return out;
}

void SakiDay10::compute1()
{
	std::string in = "1113122113";

	for (int i = 0; i < 40; i++)
	{
		in = look_and_say(in);
	}

	std::cout << in.length() << std::endl;
}

void SakiDay10::compute2()
{
	std::string in = "1113122113";

	for (int i = 0; i < 50; i++)
	{
		in = look_and_say(in);
	}

	std::cout << in.length() << std::endl;
}
