#include "stdafx.h"
#include "SakiFileLoader.h"
#include "SakiDay06.h"


SakiDay06::SakiDay06()
{
}

SakiDay06::~SakiDay06()
{
}

void SakiDay06::compute(int part)
{
	SakiDay06 day;
	if (part == 1)
		day.compute1();
	else
		day.compute2();
}

void SakiDay06::compute1()
{
	auto fnToggle = [](unsigned short& val) { if (val == 1) val = 0; else val = 1; };
	auto fnTurnOn = [](unsigned short& val) { val = 1; };
	auto fnTurnOff = [](unsigned short& val) { val = 0; };

	compute(fnToggle, fnTurnOn, fnTurnOff);
}

void SakiDay06::compute2()
{
	auto fnToggle = [](unsigned short& val) { val += 2; };
	auto fnTurnOn = [](unsigned short& val) { val += 1; };
	auto fnTurnOff = [](unsigned short& val) { val = (val == 0 ? 0 : val - 1); };

	compute(fnToggle, fnTurnOn, fnTurnOff);
}

void SakiDay06::compute(
	std::function<void(unsigned short&)> fnToggle,
	std::function<void(unsigned short&)> fnTurnOn,
	std::function<void(unsigned short&)> fnTurnOff)
{
	auto getXY = [](std::string& line, int pos, int& x1, int& y1, int& x2, int& y2)
	{
		sscanf_s(line.c_str() + pos, "%d,%d through %d,%d", &x1, &y1, &x2, &y2);
	};

	auto applyFunc = [](int x1, int y1, int x2, int y2, std::vector<unsigned short>& grid,
		std::function<void(unsigned short&)> func)
	{
		assert(x1 <= x2);
		assert(y1 <= y2);
		for (int y = y1; y <= y2; y++)
		{
			for (int x = x1; x <= x2; x++)
			{
				int index = y * 1000 + x;
				auto val = grid.at(index);
				func(val);
				grid[index] = val;
			}
		}
	};


	std::vector<unsigned short> grid(1000 * 1000);
	const std::string toggle = "toggle ";
	const std::string turn_on = "turn on ";
	const std::string turn_off = "turn off ";

	std::string path = SakiFileLoader::s_defaultRoot + "Day06.txt";
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

		int x1 = 0;
		int y1 = 0;
		int x2 = 0;
		int y2 = 0;
		if (line.find(toggle) == 0)
		{
			getXY(line, toggle.length(), x1, y1, x2, y2);
			applyFunc(x1, y1, x2, y2, grid, fnToggle);
		}
		else if (line.find(turn_on) == 0)
		{
			getXY(line, turn_on.length(), x1, y1, x2, y2);
			applyFunc(x1, y1, x2, y2, grid, fnTurnOn);
		}
		else if (line.find(turn_off) == 0)
		{
			getXY(line, turn_off.length(), x1, y1, x2, y2);
			applyFunc(x1, y1, x2, y2, grid, fnTurnOff);
		}
	}

	long long int count = 0;
	for (auto x : grid)
	{
		count += x;
	}

	std::cout << count << std::endl;
}
