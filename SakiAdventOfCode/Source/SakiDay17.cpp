#include "stdafx.h"
#include "SakiFileLoader.h"
#include "SakiDay17.h"


SakiDay17::SakiDay17()
{
}

SakiDay17::~SakiDay17()
{
}

void SakiDay17::compute(int part)
{
	SakiDay17 day;
	if (part == 1)
		day.compute1();
	else
		day.compute2();
}

static int calcAmount(std::vector<int>& vals)
{
	int ret = 0;
	for (auto x : vals)
	{
		ret += x;
	}
	return ret;
}

static void iter(SakiDay17::SakiDay17Context& context, int start, int amount, std::vector<int> indices)
{
	const int goal = 150;

	for (size_t i = start; i < context.containers.size(); i++)
	{
		int newAmount = amount + context.containers.at(i);

		std::vector<int> newIndices = indices;
		newIndices.push_back(context.containers.at(i));
		iter(context, i + 1, newAmount, newIndices);

		int amount = calcAmount(newIndices);
		if (amount == goal)
		{
			context.counts[newIndices.size()]++; // This is for Part 2
			context.count++; // This is for Part1
		}
	}
}

void SakiDay17::compute(SakiDay17Context& context)
{
	const std::vector<int> containers = {
		33,
		14,
		18,
		20,
		45,
		35,
		16,
		35,
		1,
		13,
		18,
		13,
		50,
		44,
		48,
		6,
		24,
		41,
		30,
		42 };

	std::vector<int> indices;
	context.containers = containers;
	iter(context, 0, 0, indices);
}

void SakiDay17::compute1()
{
	SakiDay17Context context;
	compute(context);

	std::cout << context.count << std::endl;
}

void SakiDay17::compute2()
{
	SakiDay17Context context;
	compute(context);

	auto it = context.counts.begin();
	std::cout << it->second << std::endl;
}
