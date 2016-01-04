#include "stdafx.h"
#include "SakiFileLoader.h"
#include "SakiDay24.h"


SakiDay24::SakiDay24()
{
}

SakiDay24::~SakiDay24()
{
}

void SakiDay24::compute(int part)
{
	SakiDay24 day;
	if (part == 1)
		day.compute1();
	else
		day.compute2();
}

void SakiDay24::compute1()
{
	SakiDay24Context context;
	context.goal = 520;
	compute(context);

	std::cout << *context.quantumE.begin() << std::endl;
}

void SakiDay24::compute2()
{
	SakiDay24Context context;
	context.goal = 390;
	compute(context);

	std::cout << *context.quantumE.begin() << std::endl;
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

static void iter(SakiDay24::SakiDay24Context& context, int start, int amount, std::vector<int> indices)
{
	//If the group includes 2, the # of the package must be odd. 
	//If it contains 2, the remaining is 518. 518 must be at least 5 packages. 
	//Then it will be at least 6 and odd, meaning 7.
	//
	//Otherwise, it must be even. The minimum packages is 5. So it must be 6.
	//
	//Anyawy, possible minimum is 6.
	//
	//1,3,7,...,113 (28 numbers)
	//Average 87, at least 3 should be above 87 7C3 x 25C3 = 435 x 2300
	//
	//Here is an example found with paper & pencil
	//
	//71, 79, 83, 89, 97, 101
	//405925792351
	//
	//41, 47, 103,107,109,113
	//261581881039
	//
	//1,2,3,7,11,13,17,19,23,31,37,43,53,59,61,67,73

	const int goal = context.goal;

	for (size_t i = start; i < context.containers.size(); i++)
	{
		int newAmount = amount + context.containers.at(i);

		std::vector<int> newIndices = indices;
		newIndices.push_back(context.containers.at(i));
		if (newIndices.size() <= 6)
		{
			iter(context, i + 1, newAmount, newIndices);
			if (newIndices.size() <= 6)
			{
				int amount = calcAmount(newIndices);
				if (amount == goal)
				{
					long long int qe = 1;
					for (auto x : newIndices)
					{
						qe *= x;
					}
					context.quantumE.insert(qe);
				}
			}
		}
	}
}

void SakiDay24::compute(SakiDay24Context& context)
{
	const std::vector<int> containers = {
		1,
		2,
		3,
		7,
		11,
		13,
		17,
		19,
		23,
		31,
		37,
		41,
		43,
		47,
		53,
		59,
		61,
		67,
		71,
		73,
		79,
		83,
		89,
		97,
		101,
		103,
		107,
		109,
		113 };

	context.containers = containers;
	std::vector<int> indices;
	iter(context, 0, 0, indices);
}
