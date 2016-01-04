#include "stdafx.h"
#include "SakiFileLoader.h"
#include "SakiDay14.h"


SakiDay14::SakiDay14()
{
}

SakiDay14::~SakiDay14()
{
}

void SakiDay14::compute(int part)
{
	SakiDay14 day;
	if (part == 1)
		day.compute1();
	else
		day.compute2();
}

void SakiDay14::compute1()
{
	int winningDistance = 0;
	int winningPoint = 0;
	compute(winningDistance, winningPoint);

	std::cout << winningDistance << std::endl;
}

void SakiDay14::compute2()
{
	int winningDistance = 0;
	int winningPoint = 0;
	compute(winningDistance, winningPoint);

	std::cout << winningPoint << std::endl;
}

void SakiDay14::compute(
	int& winningDistance,
	int& winningPoint)
{
	auto getCurrent = [](int sec, int speed, int endurance, int rest) -> int
	{
		int sum = endurance + rest;
		int running_time = endurance * (sec / sum);
		int mod = sec % sum;
		if (mod > endurance)
			running_time += endurance;
		else
			running_time += mod;

		return (running_time * speed);
	};

	struct reindeer
	{
		int current;
		int point;
		int speed;
		int endurance;
		int rest;
	};

	const int count_deers = 9;
	reindeer deers[count_deers] =
	{
		{ 0, 0, 22 , 8 , 165 },
		{ 0, 0, 8 , 17 , 114 },
		{ 0, 0, 18 , 6 , 103 },
		{ 0, 0, 25 , 6 , 145 },
		{ 0, 0, 11 , 12 , 125 },
		{ 0, 0, 21 , 6 , 121 },
		{ 0, 0, 18 , 3 , 50 },
		{ 0, 0, 20 , 4 , 75 },
		{ 0, 0, 7, 20, 119 }
	};

	for (int i = 1; i <= 2503; i++)
	{
		for (int j = 0; j < count_deers; j++)
		{
			auto& deer = deers[j];
			deer.current = getCurrent(i, deer.speed, deer.endurance, deer.rest);
		}

		int lead = 0;
		for (int j = 0; j < count_deers; j++)
		{
			auto& deer = deers[j];
			lead = std::max(lead, deer.current);
		}

		for (int j = 0; j < count_deers; j++)
		{
			auto& deer = deers[j];
			if (lead == deer.current)
				deer.point++;
		}
	}

	for (int i = 0; i < count_deers; i++)
	{
		auto& deer = deers[i];
		winningDistance = std::max(winningDistance, deer.current);
		winningPoint = std::max(winningPoint, deer.point);
	}
}
