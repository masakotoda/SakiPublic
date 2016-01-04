#include "stdafx.h"
#include "SakiFileLoader.h"
#include "SakiDay18.h"


SakiDay18::SakiDay18()
{
}

SakiDay18::~SakiDay18()
{
}

void SakiDay18::compute(int part)
{
	SakiDay18 day;
	if (part == 1)
		day.compute1();
	else
		day.compute2();
}

void SakiDay18::compute1()
{
	compute(false);
}

void SakiDay18::compute2()
{
	compute(true);
}

static bool isLit(int x, int y, std::string& grid)
{
	if (x < 0 || y < 0)
		return false;
	if (x >= 100 || y >= 100)
		return false;

	int index = y * 100 + x;
	return grid.at(index) == '#';
}

static int countLit(int i, std::string& grid)
{
	int x = i % 100;
	int y = i / 100;

	int count = 0;
	count = isLit(x - 1, y - 1, grid) ? count + 1 : count;
	count = isLit(x, y - 1, grid) ? count + 1 : count;
	count = isLit(x + 1, y - 1, grid) ? count + 1 : count;
	count = isLit(x - 1, y, grid) ? count + 1 : count;
	count = isLit(x + 1, y, grid) ? count + 1 : count;
	count = isLit(x - 1, y + 1, grid) ? count + 1 : count;
	count = isLit(x, y + 1, grid) ? count + 1 : count;
	count = isLit(x + 1, y + 1, grid) ? count + 1 : count;

	return count;
}

void nextStep(std::string& grid, bool stuck)
{
	std::vector<char> next;
	next.resize(grid.length());
	for (size_t i = 0; i < grid.length(); i++)
	{
		auto x = grid.at(i);
		auto y = countLit(i, grid);
		if (x == '#')
		{
			if (y != 2 && y != 3)
				x = '.';
		}
		else
		{
			if (y == 3)
				x = '#';
		}
		next[i] = x;
	}

	if (stuck)
	{
		next[0] = '#';
		next[99] = '#';
		next[9900] = '#';
		next[9999] = '#';
	}

	grid.assign(next.begin(), next.end());
}

void SakiDay18::compute(bool stuck)
{
	std::string grid;

	std::string path = SakiFileLoader::s_defaultRoot + "Day18.txt";
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

		grid += line;
	}

	if (stuck)
	{
		grid[0] = '#';
		grid[99] = '#';
		grid[9900] = '#';
		grid[9999] = '#';
	}

	for (int i = 0; i < 100; i++)
	{
		nextStep(grid, stuck);
	}

	int count = 0;
	for (auto x : grid)
	{
		if (x == '#')
			count++;
	}

	std::cout << count << std::endl;
}
