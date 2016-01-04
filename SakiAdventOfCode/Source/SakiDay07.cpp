#include "stdafx.h"
#include "SakiFileLoader.h"
#include "SakiDay07.h"


SakiDay07::SakiDay07()
{
}

SakiDay07::~SakiDay07()
{
}

void SakiDay07::compute(int part)
{
	SakiDay07 day;
	if (part == 1)
		day.compute1();
	else
		day.compute2();
}

void SakiDay07::compute1()
{
	unsigned short a = computeA(0);
	std::cout << a << std::endl;
}

void SakiDay07::compute2()
{
	unsigned short b = computeA(0);
	unsigned short a = computeA(b);
	std::cout << a << std::endl;
}

static unsigned short getKey(std::string& line, int pos)
{
	const char* head = line.c_str() + pos;
	const int bufSize = 7;
	char buf[bufSize + 1] = { 0 };
	sscanf_s(head, "%s ", buf, bufSize);

	unsigned short key = 0;
	char* p = &buf[0];
	while (true)
	{
		key |= *p;
		p++;
		if (*p)
		{
			key = key << 8;
		}
		else
		{
			break;
		}
	}

	return key;
}

static bool getVal(std::string& line, int pos, unsigned short& ret, std::unordered_map<unsigned short, unsigned short>& states)
{
	const char* head = line.c_str() + pos;
	if (isdigit(*head))
	{
		int i = 0;
		sscanf_s(head, "%d ", &i);
		ret = static_cast<unsigned short>(i);
		return true;
	}
	else
	{
		auto key = getKey(line, pos);
		if (states.find(key) != states.end())
		{
			ret = states[key];
			return true;
		}
		else
		{
			return false;
		}
	}
}

unsigned short SakiDay07::computeA(unsigned short initialB)
{
	// This is pretty dumb algorithm. I should be able to back track it...

	const std::string gateOR = " OR ";
	const std::string gateAND = " AND ";
	const std::string gateNOT = "NOT ";
	const std::string gateLSHIFT = " LSHIFT ";
	const std::string gateRSHIFT = " RSHIFT ";

	std::string path = SakiFileLoader::s_defaultRoot + "Day07.txt";
	std::string contents = SakiFileLoader::GetFileContents(path.c_str());

	std::unordered_map<unsigned short, unsigned short> states;
	if (initialB > 0)
	{
		states['b'] = initialB;
	}

	while (states.find('a') == states.end())
	{
		const char* head = contents.c_str();
		while (head != nullptr)
		{
			std::string line;
			head = SakiFileLoader::GetLine(head, line);

			if (line.empty())
				break;

			int outPos = line.find(" -> ") + 4;
			unsigned short keyOut = getKey(line, outPos);

			int pos = -1;

			if (initialB > 0 && keyOut == 'b')
			{
				;
			}
			else if (line.find(gateOR) != std::string::npos)
			{
				pos = line.find(gateOR);
				unsigned short in1 = 0;
				unsigned short in2 = 0;
				if (getVal(line, 0, in1, states) &&
					getVal(line, pos + gateOR.length(), in2, states))
					states[keyOut] = (in1 | in2);
			}
			else if (line.find(gateAND) != std::string::npos)
			{
				pos = line.find(gateAND);
				unsigned short in1 = 0;
				unsigned short in2 = 0;
				if (getVal(line, 0, in1, states) &&
					getVal(line, pos + gateAND.length(), in2, states))
					states[keyOut] = (in1 & in2);
			}
			else if (line.find(gateNOT) != std::string::npos)
			{
				pos = line.find(gateNOT);
				unsigned short in1 = 0;
				if (getVal(line, pos + gateNOT.length(), in1, states))
					states[keyOut] = ~in1;
			}
			else if (line.find(gateLSHIFT) != std::string::npos)
			{
				pos = line.find(gateLSHIFT);
				unsigned short in1 = 0;
				unsigned short in2 = 0;
				if (getVal(line, 0, in1, states) &&
					getVal(line, pos + gateLSHIFT.length(), in2, states))
					states[keyOut] = (in1 << in2);
			}
			else if (line.find(gateRSHIFT) != std::string::npos)
			{
				pos = line.find(gateRSHIFT);
				unsigned short in1 = 0;
				unsigned short in2 = 0;
				if (getVal(line, 0, in1, states) &&
					getVal(line, pos + gateRSHIFT.length(), in2, states))
					states[keyOut] = (in1 >> in2);
			}
			else // :=
			{
				unsigned short in1 = 0;
				if (getVal(line, 0, in1, states))
					states[keyOut] = in1;
			}
		}
	}

	if (states.find('a') == states.end())
	{
		assert(false);
	}

	return states['a'];
}
